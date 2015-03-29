#include "QueryPreProcessor.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "RelTable.h"
#include "..\..\AutoTester\source\AbstractWrapper.h"

const std::string QueryPreProcessor::de[] = {"stmt", "assign", "while", "if", "variable", "procedure", "prog_line", "constant"};
const std::string QueryPreProcessor::rel[] = {"Modifies", "Uses", "Parent", "Parent*", "Follows", "Follows*", "Calls", "Calls*", "Next", "Next*", "Affects", "Affects*"};
 
QueryPreProcessor::QueryPreProcessor(void) {}

bool QueryPreProcessor::ValidateQuery(std::string query, QueryData &queryData)
{
	std::vector<std::string> tokenList;
	std::string token;
	std::string delim = " ,();";

	//std::cout << "Before tokenize\n";

	//tokenize query
	if(!Tokenize(query,tokenList)) {
		std::cout << "Invalid Query: Invalid Syntax.\n";
		return false;
	}

	//std::cout << "AFter tokenize\n";

	std::vector<std::string>::iterator it = tokenList.begin();
	token = *it;
	//std::cout << "token: " << token << "\n";
	//validate declaration
	while(IsDeclaration(token))	//if token is design entity
	{
		//assign a;while w;Select a

		//std::cout << "\nIn declaration\n";
		std::string type = token;

		if(++it == tokenList.end())	return false;
		token = *it;
		//std::cout << "token: " << token << "\n";

		//get the synonym
		while(!IsSemiColon(token))
		{
			if(token == ",") {
				if(++it == tokenList.end())	return false;
					token = *it;
			}

			else {
				//std::cout << "\nIn Check Semicolon\n";
				Synonym synonym;
				synonym.value = token;

				//validate declaration
				if(!ValidateDeclaration(synonym, type))	return false;

				//save declaration
				queryData.InsertDeclaration(synonym);

				if(++it == tokenList.end())	return false;
				token = *it;
				//std::cout << "token: " << token << "\n";
			}
		}
		//std::cout << "\nAfer Check Semicolon\n";

		if(++it == tokenList.end())	return false;
		token = *it;
		//std::cout << "token: " << token << "\n";
	}

	//std::cout << "\nafter declaration\n";

	//check if declaration is empty
	if(queryData.GetDeclarations().empty()) {
		std::cout << "Invalid Query: No declaration.\n";
		return false;
	}

	if(AbstractWrapper::GlobalStop)	return false;

	//check if next token is select, and validate select
	if(IsSelect(token))
	{
		//std::cout << "\nIn Select\n";
		if(++it == tokenList.end())	{//std::cout << "\nno more token\n";
			return false;
		}
		token = *it;
		//std::cout << "token: " << token << "\n";

		//select <a,w,v>
		if(IsOpenBracket(token))
		{
			if(++it == tokenList.end())	return false;
			token = *it;

			while(!IsCloseBracket(token))
			{
				Synonym synonym;
				synonym.value = token;

				if(!ValidateSelect(synonym))	return false;
				else queryData.InsertSelect(synonym);

				if(++it == tokenList.end())	return false;
				token = *it;
			}

			//if no data in selectclause
			if(queryData.GetSizeOfClause(SELECT) == 0)	return false;
		}

		//Select BOOLEAN
		else if(IsBoolean(token))
		{
			Synonym synonym;
			synonym.type = BOOLEAN;
			synonym.value = "BOOLEAN";

			queryData.InsertSelect(synonym);
		}
		

		//select a
		else
		{
			Synonym synonym;
			synonym.value = token;

			if(!ValidateSelect(synonym))	return false;			
			else queryData.InsertSelect(synonym);
		}
		
	}

	//no select
	else return false;

	if(AbstractWrapper::GlobalStop)	return false;

	//get next token
	if(++it == tokenList.end())	return true;
	token = *it;

	//validate such that, pattern
	while(IsSuchThat(token) || IsPattern(token) || IsWith(token)) 
	{
		bool endOfQuery = false, hasSuchThat = false, hasPattern = false, hasWith = false;
		bool repeat = false;	//repeat after AND

		//such that ... and such that ... will not work
		while(IsSuchThat(token) || repeat)	//such that 
		{
			if(!repeat)
			{
				if(++it == tokenList.end())	return false;	//ignore "that"
				token = *it;
			}

			if(++it == tokenList.end())	return false;	//get RelationshipType
			token = *it;

			Argument arg1, arg2;

			if(IsRelationship(token)) //uses, modifies, parents, etc.
			{
				std::string rel = token;			

				if(++it == tokenList.end())	return false;	//get arg1
				token = *it;
				arg1.value = token;

				if(++it == tokenList.end())	return false;	//get arg2
				token = *it;
				arg2.value = token;

				RelationshipType rel_enum;

				if(!ValidateRelationship(rel, rel_enum, arg1, arg2))	return false;

				queryData.InsertSuchThat(rel_enum, arg1, arg2);
			}

			else return false;

			if(++it == tokenList.end())	return true;
			else token = *it;

			if(IsAnd(token))	repeat = true;
			else				repeat = false;

			if(AbstractWrapper::GlobalStop)	return false;
		}

		while(IsPattern(token) || repeat)	//pattern or and
		{		
			//std::cout << "In Pattern\n";
			//pattern a("x",_)
			//pattern if("x",_,_)
			//pattern w("w",_)

			Argument arg1, arg2;
			Synonym synonym;

			if(++it == tokenList.end())	return false;	//get type
			token = *it;
			synonym.value = token;

			if(++it == tokenList.end())	return false;	//get arg1
			token = *it;
			arg1.value = token;

			if(++it == tokenList.end())	return false;	//get arg2
			token = *it;
			arg2.value = token;


			//get the type of synonym
			if(!QueryData::IsSynonymExist(synonym.value, synonym.type)) return false;
			
			if(synonym.type == IF)
			{
				if(++it == tokenList.end())	return false;	//get arg3 and ignore it, since it must be _
				Argument arg3;
				arg3.value = *it;
				
				if(!ValidatePattern(synonym, arg1, arg2, arg3))	return false;
				queryData.InsertPattern(synonym, arg1, arg2);	//just insert 2, because only the first one matter for IF
			}
			
			else if(synonym.type == WHILE || synonym.type == ASSIGN)
			{
				if(!ValidatePattern(synonym, arg1, arg2))	return false;
				queryData.InsertPattern(synonym, arg1, arg2);
			}

			else {
				std::cout << "Invalid Query: Pattern synonym is not declared.\n";
				return false;
			}

			if(++it == tokenList.end())	return true;
			else token = *it;

			if(IsAnd(token))	repeat = true;
			else				repeat = false;

			if(AbstractWrapper::GlobalStop)	return false;
		}

		while(IsWith(token) || repeat)	//with or and
		{		
			Argument arg1, arg2;

			if(++it == tokenList.end())	return false;	//get arg1
			token = *it;
			arg1.value = token;
			
			//skip "="
			if(++it == tokenList.end())	return false;	
			if(*it != "=") return false;

			if(++it == tokenList.end())	return false;	//get arg2
			arg2.value = *it;

			if(!ValidateWith(arg1, arg2))	return false;
			queryData.InsertWith(arg1, arg2);

			if(++it == tokenList.end())	return true;
			else token = *it;

			if(IsAnd(token))	repeat = true;
			else				repeat = false;

			if(AbstractWrapper::GlobalStop)	return false;
		}
	}

	return false;
}

void QueryPreProcessor::Tokenize(std::string str, std::vector<std::string> &tokens, std::string delim)
{
	std::stringstream stringStream(str);
	std::string line;
	
	while(std::getline(stringStream, line)) 
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of(delim, prev)) != std::string::npos)
		{
			if (pos > prev)
				tokens.push_back(line.substr(prev, pos-prev));
			prev = pos+1;
		}
		if (prev < line.length())
			tokens.push_back(line.substr(prev, std::string::npos));
	}
}

bool QueryPreProcessor::Tokenize(std::string query, std::vector<std::string> &tokens) {
	bool isIdent = false, isExpression = false, isProgLine = false;

	query += " ";	//add a whitespace behind, to handle case like assign a;Select a, if not the a will not get push back

	for (unsigned int pos = 0; pos < query.length(); pos++) { // loop through string
		char currentChar = query[pos];
		static std::string integer = "";
		static std::string alphaString = "";


		//std::cout << "char[" << pos << "] : " << currentChar << "\n";

		if (currentChar >= 48 && currentChar <= 57) { // integer
			//std::cout << "In Integer\n";
			if (alphaString == "") {
				integer = integer + currentChar;
			} else {
				alphaString = alphaString + currentChar;
			}

		} else if ((currentChar >= 65 && currentChar <= 90) ||
				   (currentChar >= 97 && currentChar <= 122)) { // alpha
			
					   //std::cout << "In alphabet\n";
			if (integer != "") {
				//throw (std::string) "Invalid idenitifier.";
				return false;
			} else {
				alphaString = alphaString + currentChar;
			}

		} else { // symbol, whitespace or endline
			//std::cout << "In Others\n";
			if (integer != "") { // previous substring is integer
				tokens.push_back(integer);
				integer = "";
			} 
			else if (alphaString != "") { // previous substring is... string
				if(alphaString == "prog") 
					isProgLine = true;

				else if(currentChar == '*') {
					tokens.push_back(alphaString+"*");
					alphaString = "";
				}
	
				//if string begins with " or _ which means an expression or IDENT, do not push back
				else if(!(alphaString.at(0) == '"' || alphaString.at(0) == '_'))
				{
					tokens.push_back(alphaString);
					alphaString = "";
				}
			}

			/*
			( - ignore
			) - ignore
			, - ignore
			; - push back
			" - push back
			_ - push back
			*/
			//if is ( or ) or , just ignore
			if (currentChar == '(' || currentChar == ')') {
				if(currentChar == ')'){
					if(alphaString != "") {	//handle _, e.g. Parent(w,_)
						tokens.push_back(alphaString);
						alphaString = "";
						isExpression = false;
						isIdent = false;
					}
					else continue;
				}

				else continue;
			}

			else if(currentChar == ',') {
				//std::cout << "In ,\n";
				if(isExpression)	//current string start with underscore, push back the first char
				{
					//std::cout << "In isExpression\n";
					std::string cs(1,alphaString.at(0));
					tokens.push_back(cs);
					isExpression = false;
					alphaString = "";
				}

				else continue;
			}

			//if is white space, depends on previous string
			else if (currentChar == '	' || currentChar == '\r' || currentChar == '\n' || currentChar == ' ') {
				//std::cout << "In space\n";
				//if previous string is not empty means it is an expression or IDENT " x " , " x + y " , _ " x + 5 " _ or just _ 
				//if former case, do not ignore white space
				//if latter case, pushback tokens

				if (alphaString != "")
					alphaString += currentChar;
				
				//if previous string is empty, ignore whitespace
				else continue;
			} 
			else {
				if(currentChar == ';')
				{
					//std::cout << "In ;\n";
					//push back tokens
					tokens.push_back(";");
				}
				else if(currentChar == '"')
				{
					//std::cout << "In \"\n";
					//if alphastring is empty, add to alphastring
					//else if alphastring is not empty, check if first char is either " or _, if yes add to alphastring and push back, clear alphastring
					//else return false

					if(alphaString == "") {
						//std::cout << "In empty string\n";
						alphaString += currentChar;
						isIdent = true;
					}

					else {
						if(isIdent) {
							//std::cout << "In isIdent \"\n";
							alphaString += currentChar;
							tokens.push_back(alphaString);
							alphaString = "";
							isIdent = false;
						}

						else if(isExpression) {
							//std::cout << "In isExpression\n";
							alphaString += currentChar;
						}

						else {
							//std::cout << "Whaaa??\n";
							return false;
						}
					}
				}

				else if(currentChar == '_')
				{
					//std::cout << "In _\n";
					if(alphaString == "") {
						//std::cout << "In empty string\n";
						alphaString += currentChar;
						isExpression = true;
					}

					else {
						if(isExpression) {
							//std::cout << "In isExpression\n";
							alphaString += currentChar;
							tokens.push_back(alphaString);
							alphaString = "";
							isExpression = false;
						}
						else if(isProgLine) {
							alphaString += currentChar;
							isProgLine = false;
						}
						else {
							std::cout << "Whaaa??\n";
							return false;
						}
					}
				}

				else if(currentChar == '+') 
				{
					//std::cout << "In +\n";
					if(isExpression) {
						//std::cout << "In isExpression\n";
						alphaString += currentChar;
					}

					else {
						std::cout << "Invalid character +\n";
						return false;
					}
				}

				else if(currentChar == '*') {}

				else {
					std::cout << "Invalid character.\n";
					return false;
				}
			}
		}

		//std::cout << "fine\n";
	}

	return true;
}

/*
- Check if there is any duplicated synonym in declaration. E.g. assign a1; while a1;
- Design entities allowed in declaration are stmt, assign, while, if, variable, constant, procedure, prog_line, call
  (Although assignnment 4 only have stmt, assign, while, variable, constant, prog_line)
*/
bool QueryPreProcessor::ValidateDeclaration(Synonym &synonym, std::string type)
{
	//check duplicated declaration
	if(QueryData::IsSynonymExist(synonym.value)) {
		std::cout << "Synonym already declared.\n";
		return false;
	}

	//convert type(string) to enum
	if(!GetEnumSynonymType(type,synonym.type)) {
		std::cout << "Invalid Query: No matching design entity type for declaration.\n";
		return false;
	}

	//Design entity must be one of the following
	const SynonymType list[] = {ASSIGN, STMT, WHILE, VARIABLE, CONSTANT, PROG_LINE};
	std::vector<SynonymType> list_vec(list, list + sizeof(list) / sizeof(list[0]));

	if(std::find(list_vec.begin(), list_vec.end(), synonym.type) != list_vec.end())
		return true;

	return false;
}

/*
- Check if select BOOLEAN, if yes return synonym=type="BOOLEAN"
- Otherwise, check if synonym is declared in declaration, if yes get the type and return type
*/
bool QueryPreProcessor::ValidateSelect(Synonym &synonym)
{
	//pass reference twice, not sure will work or not
	if(QueryData::IsSynonymExist(synonym.value, &synonym.type))
		return true;

	else {
		std::cout << "Invalid Query: In ValidateSelect, synonym is not declared.\n";
		return false;
	}
}

/*
f
*/
bool QueryPreProcessor::ValidatePattern(Synonym synonym, Argument &arg1, Argument &arg2)
{
	if(synonym.type == ASSIGN)
	{
		if(arg1.value == "_") arg1.type = UNDERSCORE;
		
		else if(QueryData::IsSynonymExist(arg1.value, VARIABLE))	//must be variable
		{
			arg1.type = SYNONYM;
			arg1.syn = Synonym(arg1.value, VARIABLE);
		}
			
		else if(IsIdent(arg1.value)) arg1.type = IDENT;

		else {
			std::cout << "Invalid Query: In ValidatePattern, Argument 1 type not found.\n";
			return false;
		}

		if(arg2.value == "_") arg2.type = UNDERSCORE;

		else if(IsExpression(arg2.value)) arg2.type = EXPRESSION;

		else {
			std::cout << "Invalid Query: In ValidatePattern, Argument 2 type not found.\n";
			return false;
		}

		return true;
	}

	
	//pattern w(v,_)
	//pattern w("x",_)
	//pattern w(_,_)
	else if(synonym.type == WHILE)
	{
		if(!IsUnderscore(arg2.value)) return false;		//arg2 must be _

		if(IsUnderscore(arg1.value)) arg1.type = UNDERSCORE;
		
		else if(QueryData::IsSynonymExist(arg1.value, VARIABLE))	//must be variable
		{
			arg1.type = SYNONYM;
			arg1.syn = Synonym(arg1.value, VARIABLE);
		}
			
		else if(IsIdent(arg1.value)) arg1.type = IDENT;

		else {
			std::cout << "Invalid Query: In ValidatePattern, Argument 1 type not found.\n";
			return false;
		}
	}

	else 
	{
		std::cout << "Invalid Query: In ValidatePattern, Synonym type is not ASSIGN or WHILE.\n";
		return false;
	}
}

//Only cater to IF
//this function can call the above validatepattern with while
bool QueryPreProcessor::ValidatePattern(Synonym synonym, Argument &arg1, Argument &arg2, Argument &arg3)
{
	//pattern if(v,_,_)
	//pattern if("x",_,_)
	//pattern if(_,_,_)
	if(synonym.type == IF)
	{
		if(!IsUnderscore(arg2.value) || !IsUnderscore(arg3.value)) return false;		//arg2 and arg3 must be _

		if(IsUnderscore(arg1.value)) arg1.type = UNDERSCORE;
		
		else if(QueryData::IsSynonymExist(arg1.value, VARIABLE))	//must be variable
		{
			arg1.type = SYNONYM;
			arg1.syn = Synonym(arg1.value, VARIABLE);
		}
			
		else if(IsIdent(arg1.value)) arg1.type = IDENT;

		else {
			std::cout << "Invalid Query: In ValidatePattern, Argument 1 type not found.\n";
			return false;
		}
	}

	else 
	{
		std::cout << "Invalid Query: In ValidatePattern, Synonym type is not IF.\n";
		return false;
	}
}

bool QueryPreProcessor::ValidateRelationship(std::string rel, RelationshipType &rel_enum, Argument &arg1, Argument &arg2)
{
	//convert relationship string to enum
	if(!GetEnumRelationshipType(rel,rel_enum)) {
		std::cout << "Invalid Query: No matching relationship type for such that.\n";
		return false;
	}

	SynonymType synonymType;

	//validate argument 1
	if(IsUnderscore(arg1.value))	arg1.type = UNDERSCORE;

	else if(IsInteger(arg1.value))	arg1.type = INTEGER;

	else if(IsIdent(arg1.value))	arg1.type = IDENT;

	else if(QueryData::IsSynonymExist(arg1.value, &synonymType)) {
		arg1.type = SYNONYM;
		arg1.syn = Synonym(arg1.value, synonymType);
	}
		
	else {
		std::cout << "Invalid Query: In ValidateRelationship, Argument 1 type not found.\n";
		return false;
	}

	try {
		//get stored argument type
		std::vector<ArgumentType> argType = RelTable::GetRelArgType(rel_enum, ARG1);
		std::vector<SynonymType> argSynonymType = RelTable::GetRelArgSynonymType(rel_enum, ARG1);

		//check whether arg1.type is one of the allowed type
		if(std::find(argType.begin(), argType.end(), arg1.type) != argType.end()) {
			//if type is synonym
			if(arg1.type == SYNONYM) {
				//check whether synonym type is one of the allowed type
				if(std::find(argSynonymType.begin(), argSynonymType.end(), synonymType) != argSynonymType.end()) {}
				else {
					std::cout << "Invalid Query: In ValidateRelationship, Argument 1 Synonym type not allowed.\n";
					return false;
				}
			}
		}

		else {
			std::cout << "Invalid Query: In ValidateRelationship, Argument 1 type not allowed.\n";
			return false;
		}

		//validate argument 2
		if(IsUnderscore(arg2.value))	arg2.type = UNDERSCORE;

		else if(IsInteger(arg2.value))	arg2.type = INTEGER;

		else if(IsIdent(arg2.value))	arg2.type = IDENT;

		else if(QueryData::IsSynonymExist(arg2.value, &synonymType)) {
			arg2.type = SYNONYM;
			arg2.syn = Synonym(arg2.value, synonymType);
		}
		
		else {
			std::cout << "Invalid Query: In ValidateRelationship, Argument 2 type not found.\n";
			return false;
		}

		//get stored argument type
		argType = RelTable::GetRelArgType(rel_enum, ARG2);
		argSynonymType = RelTable::GetRelArgSynonymType(rel_enum, ARG2);
	
		//check whether arg1.type is one of the allowed type
		if(std::find(argType.begin(), argType.end(), arg2.type) != argType.end()) {
			//if type is synonym
			if(arg2.type == SYNONYM) {
				//check whether synonym type is one of the allowed type
				if(std::find(argSynonymType.begin(), argSynonymType.end(), synonymType) != argSynonymType.end()) {}
				else {
					std::cout << "Invalid Query: In ValidateRelationship, Argument 2 Synonym type not allowed.\n";
					return false;
				}
			}
		}

		else {
			std::cout << "Invalid Query: In ValidateRelationship, Argument 2 type not allowed.\n";
			return false;
		}

		return true;
	}
	catch(const std::invalid_argument &e) {
		std::cout << e.what() << std::endl;
		return false;
	}
}

bool QueryPreProcessor::ValidateWith(Argument& arg1, Argument& arg2)
{


	return false;
}




/*
_"factor"_
_"factor+factor"_
_
factor : NAME | INTEGER
assume whitespace hasn't been remove
" x ", _ " x + y" _ are also valid
assume _ and " " is included, e.g. _"x"_
*/
bool QueryPreProcessor::IsExpression(std::string str)
{
	//remove white spaces
	str.erase(std::remove_if(str.begin(), str.end(), [](char x){return isspace(x);}), str.end());
	
	//eliminates " " or _" "_ and get the content
	//if(str.at(0) == '\"' && str.at(str.length()-1) == '\"')		//"..."
	//	str = str.substr(1, str.length()-2);

	//str.length() must > 4? _""_
	if(str.length() < 5)	return false;

	if (str.at(0) == '_' && str.at(str.length()-1) == '_')	//_..._
	{
		if(str.at(1) == '\"' && str.at(str.length()-2) == '\"')	//_"..."_
		{
			int length = str.length() - 4;
			str = str.substr(2, length);
		}

		else return false;
	}

	else return false;

	std::vector<std::string> tokenList;
	std::string delim = "+";

	Tokenize(str, tokenList, delim);

	for(std::vector<std::string>::iterator it = tokenList.begin(); it != tokenList.end(); ++it) {
		if(!(IsName(*it) || IsInteger(*it)))
			return false;
	}
	
	return true;
}

/*
IDENT: LETTER (LETTER| DIGIT | '#')*
IDENT must be an alphabet followed by 0 or more times of alphanumeric character or '#'
assume " " is included, e.g. "x"
*/
bool QueryPreProcessor::IsIdent(std::string str)
{
	//remove white spaces
	str.erase(std::remove_if(str.begin(), str.end(), [](char x){return isspace(x);}), str.end());
	
	//eliminates " " and get the content
	if(str.at(0) == '\"' && str.at(str.length()-1) == '\"')		//"..."
		str = str.substr(1, str.length()-2);

	else return false;

	std::string::iterator it = str.begin();
	
	//first character must be alphabet
	if(!str.empty() && isalpha(*it))	++it;
	else return false;

	//subsequent character must be alphanumeric or "#"
	while (it != str.end() && (isalnum(*it) || (*it) == '#')) ++it;
    return !str.empty() && it == str.end();
}

/*
Name : LETTER (LETTER | DIGIT)*
Name must be an alphabet followed by 0 or more times of alphanumeric character
*/
bool QueryPreProcessor::IsName(std::string s)
{
	std::string::iterator it = s.begin();

	//first character must be alphabet
	if(!s.empty() && isalpha(*it))	++it;
	else return false;

	//subsequent character must be alphanumeric
	while (it != s.end() && (isalnum(*it))) ++it;
    return !s.empty() && it == s.end();
}

/*
Integer : DIGIT+
Integer must be repetition of 1 or more times of number
*/
bool QueryPreProcessor::IsInteger(const std::string& s)
{
    std::string::const_iterator it = s.begin();

    while (it != s.end() && isdigit(*it)) ++it;

    return !s.empty() && it == s.end();
}

/*
Convert synonym type string to SynonymType enum
*/
bool QueryPreProcessor::GetEnumSynonymType(std::string type, SynonymType &enumType) 
{
	if(type == "assign")			enumType = ASSIGN;
	else if(type == "stmt")			enumType = STMT;
	else if(type == "while")		enumType = WHILE;
	else if(type == "variable")		enumType = VARIABLE;
	else if(type == "prog_line")	enumType = PROG_LINE;
	else if(type == "constant")		enumType = CONSTANT;
	else if(type == "if")			enumType = IF;
	else if(type == "procedure")	enumType = PROCEDURE;
	else if(type == "call")			enumType = CALL;
	else if(type == "BOOLEAN")		enumType = BOOLEAN;
	else return false;

	return true;
}

/*
Convert relationship type string to SynonymType enum
*/
bool QueryPreProcessor::GetEnumRelationshipType(std::string type, RelationshipType &enumType) 
{
	if(type == "Modifies")			enumType = MODIFIES;
	else if(type == "Uses")			enumType = USES;
	else if(type == "Parent")		enumType = PARENT;
	else if(type == "Parent*")		enumType = PARENTT;
	else if(type == "Follows")		enumType = FOLLOWS;
	else if(type == "Follows*")		enumType = FOLLOWST;
	else if(type == "Calls")		enumType = CALLS;
	else if(type == "Calls*")		enumType = CALLST;
	else if(type == "Next")			enumType = NEXT;
	else if(type == "Next*")		enumType = NEXTT;
	else if(type == "Affects")		enumType = AFFECTS;
	else if(type == "Affects&")		enumType = AFFECTST;
	else return false;

	return true;
}




//string matching functions
bool QueryPreProcessor::IsSelect(std::string str)
{
	if(str == "Select")	return true;
	return false;
}

bool QueryPreProcessor::IsSemiColon(std::string str)
{
	if(str == ";")	return true;
	return false;
}

bool QueryPreProcessor::IsSuchThat(std::string str)
{
	if(str == "such")	return true;
	return false;
}

bool QueryPreProcessor::IsPattern(std::string str)
{
	if(str == "pattern")	return true;
	return false;
}

bool QueryPreProcessor::IsUnderscore(std::string str)
{
	if(str == "_")	return true;
	return false;
}

bool QueryPreProcessor::IsWith(std::string str)
{
	if(str == "with")	return true;
	return false;
}

bool QueryPreProcessor::IsOpenBracket(std::string str)
{
	if(str == "<")	return true;
	return false;
}

bool QueryPreProcessor::IsCloseBracket(std::string str)
{
	if(str == ">")	return true;
	return false;
}

bool QueryPreProcessor::IsAnd(std::string str)
{
	if(str == "and")	return true;
	return false;
}

bool QueryPreProcessor::IsBoolean(std::string str)
{
	if(str == "BOOLEAN")	return true;
	return false;
}


/*
Check whether string is one of the design entities
*/
bool QueryPreProcessor::IsDeclaration(std::string str)
{
	std::vector<std::string> DesignEntity(de, de + sizeof(de) / sizeof(de[0]));

	if (std::find(DesignEntity.begin(), DesignEntity.end(), str) != DesignEntity.end())
		return true;

	return false;
}

/*
check whether string is one of the RelationshipType
*/
bool QueryPreProcessor::IsRelationship(std::string str)
{
	std::vector<std::string> rel_vec(rel, rel + sizeof(rel) / sizeof(rel[0]));

	if (std::find(rel_vec.begin(), rel_vec.end(), str) != rel_vec.end())
		return true;

	return false;
}







bool QueryPreProcessor::Tokenize(std::string query, std::vector<std::string> &tokens) 
{
	bool isIdent = false, isExpression = false, isProgLine = false;

	query += " ";	//add a whitespace behind, to handle case like assign a;Select a, if not the a will not get push back

	for (unsigned int pos = 0; pos < query.length(); pos++) 
	{ 
		char currentChar = query[pos];
		static std::string integer = "";
		static std::string alphaString = "";

		//std::cout << "char[" << pos << "] : " << currentChar << "\n";

		//integer
		if (currentChar >= 48 && currentChar <= 57) 
		{
			//std::cout << "In Integer\n";
			if (alphaString == "")	integer = integer + currentChar;	
			else					alphaString = alphaString + currentChar;	//the integer belong to part of a string
		} 
		
		//character
		else if ((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122)) 
		{
			//std::cout << "In alphabet\n";
			if (integer != "")	return false;	//should not happen
			else				alphaString = alphaString + currentChar;
		} 
		
		else 
		{ // symbol, whitespace or endline
			//std::cout << "In Others\n";
			//push back integer/string first 
			if (integer != "") 
			{ 
				tokens.push_back(integer);
				integer = "";
			} 

			else if (alphaString != "") 
			{ 
				//prog_line, wait for _ , don't push
				if(alphaString == "prog") 
					isProgLine = true;

				//Parent*/Next* etc
				else if(currentChar == '*') 
				{
					tokens.push_back(alphaString+"*");
					alphaString = "";
					continue;
				}
	
				//if string begins with " or _ which means an expression or IDENT, don't push
				else if(!(alphaString.at(0) == '"' || alphaString.at(0) == '_'))
				{
					tokens.push_back(alphaString);
					alphaString = "";
				}
			}

			/*
			( - ignore
			) - ignore
			, - ignore
			; - push back
			" - push back
			_ - push back
			*/
			//if is ( or ) or , just ignore
			if (currentChar == '(' || currentChar == ')') 
			{
				if(currentChar == ')')
				{
					if(alphaString != "")
					{	//handle _, e.g. Parent(w,_)
						tokens.push_back(alphaString);
						alphaString = "";
						isExpression = false;
						isIdent = false;
					}
					else continue;
				}

				else continue;
			}

			else if(currentChar == ',') {
				//std::cout << "In ,\n";
				if(isExpression)	//current string start with underscore, push back the first char
				{
					//std::cout << "In isExpression\n";
					std::string cs(1,alphaString.at(0));
					tokens.push_back(cs);
					isExpression = false;
					alphaString = "";
				}

				else continue;
			}

			//if is white space, depends on previous string
			else if (currentChar == '	' || currentChar == '\r' || currentChar == '\n' || currentChar == ' ') {
				//std::cout << "In space\n";
				//if previous string is not empty means it is an expression or IDENT " x " , " x + y " , _ " x + 5 " _ or just _ 
				//if former case, do not ignore white space
				//if latter case, pushback tokens

				if (alphaString != "")
					alphaString += currentChar;
				
				//if previous string is empty, ignore whitespace
				else continue;
			} 

			else {
				if(currentChar == ';')
				{
					//std::cout << "In ;\n";
					tokens.push_back(";");
				}

				else if(currentChar == '"')
				{
					//std::cout << "In \"\n";
					//if alphastring is empty, add to alphastring
					//else if alphastring is not empty, check if first char is either " or _, if yes add to alphastring and push back, clear alphastring
					//else return false

					if(alphaString == "") {
						//std::cout << "In empty string\n";
						alphaString += currentChar;
						isIdent = true;
					}

					else {
						if(isIdent) {
							//std::cout << "In isIdent \"\n";
							alphaString += currentChar;
							tokens.push_back(alphaString);
							alphaString = "";
							isIdent = false;
						}

						else if(isExpression) {
							//std::cout << "In isExpression\n";
							alphaString += currentChar;
						}

						else {
							//std::cout << "Whaaa??\n";
							return false;
						}
					}
				}

				else if(currentChar == '_')
				{
					//std::cout << "In _\n";
					//start of an expression
					if(alphaString == "") {
						//std::cout << "In empty string\n";
						alphaString += currentChar;
						isExpression = true;
					}

					else {
						//end of an expression
						if(isExpression) {
							//std::cout << "In isExpression\n";
							alphaString += currentChar;
							tokens.push_back(alphaString);
							alphaString = "";
							isExpression = false;
						}
						//prog_
						else if(isProgLine) {
							alphaString += currentChar;
							isProgLine = false;
						}
						else {
							std::cout << "Whaaa??\n";
							return false;
						}
					}
				}

				//+ can only occur in an expression
				else if(currentChar == '+') 
				{
					//std::cout << "In +\n";
					//part of an expression
					if(isExpression) {
						//std::cout << "In isExpression\n";
						alphaString += currentChar;
					}

					else {
						std::cout << "Invalid character +\n";
						return false;
					}
				}

				else {
					std::cout << "Invalid character.\n";
					return false;
				}
			}
		}

		//std::cout << "fine\n";
	}

	return true;
}