#include "QueryPreProcessor.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "RelTable.h"
#include "..\..\AutoTester\source\AbstractWrapper.h"

//#define DEBUG_MSG_TOKENIZER
//#define DEBUG_MSG_VALIDATEQUERY

const std::string QueryPreProcessor::de[] = {"stmt", "assign", "while", "if", "variable", "procedure", "prog_line", "constant"};
const std::string QueryPreProcessor::rel[] = {"Modifies", "Uses", "Parent", "Parent*", "Follows", "Follows*", "Calls", "Calls*", "Next", "Next*", "Affects", "Affects*"};
 
QueryPreProcessor::QueryPreProcessor(void) {}

bool QueryPreProcessor::ValidateQuery(std::string query, QueryData &queryData)
{
	std::vector<std::string> tokenList;
	std::string token;

	DebugMessage("Before tokenize\n" , VALIDATEQUERY);

	//tokenize query
	if(!Tokenize(query,tokenList)) {
		std::cout << "Invalid Query: Invalid Syntax.\n";
		return false;
	}

	DebugMessage("After tokenize\n" , VALIDATEQUERY);

	//get the first token iterator
	std::vector<std::string>::iterator it = tokenList.begin();
	token = *it;

	DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

	//validate declaration
	while(IsDeclaration(token))	
	{
		DebugMessage("\nIn declaration\n" , VALIDATEQUERY);

		std::string type = token;

		if(++it == tokenList.end())	return false;
		token = *it;

		DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

		//get the synonym
		while(!IsSemiColon(token))
		{
			if(token == ",") {
				if(++it == tokenList.end())	return false;
					token = *it;
			}

			else {
				DebugMessage("\nIn Check Semicolon\n" , VALIDATEQUERY);

				Synonym synonym;
				synonym.value = token;

				//validate declaration
				if(!ValidateDeclaration(synonym, type))	return false;

				//save declaration
				queryData.InsertDeclaration(synonym);

				if(++it == tokenList.end())	return false;
				token = *it;
			}

			DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);
		}
		DebugMessage("\nAfer Check Semicolon\n" , VALIDATEQUERY);

		if(++it == tokenList.end())	return false;
		token = *it;
		DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);
	}

	DebugMessage("\nAfer declaration\n" , VALIDATEQUERY);

	//check if declaration is empty
	if(queryData.GetSizeOfClause(DECLARATION) == 0)	
	{
		std::cout << "Invalid Query: No declaration.\n";
		return false;
	}

	if(AbstractWrapper::GlobalStop)	return false;

	//check if next token is select, and validate select
	if(IsSelect(token))
	{
		DebugMessage("\nIn Select\n" , VALIDATEQUERY);

		if(++it == tokenList.end())	return false;
		token = *it;
		DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

		//select <a,w,v>
		if(IsOpenBracket(token))
		{
			DebugMessage("\nIn <\n" , VALIDATEQUERY);

			if(++it == tokenList.end())	return false;
			token = *it;
			DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

			while(!IsCloseBracket(token))
			{
				Synonym synonym;
				synonym.value = token;

				if(!ValidateSelect(synonym))	return false;
				else queryData.InsertSelect(synonym);

				if(++it == tokenList.end())	return false;
				token = *it;
				DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);
			}

			//if no data in selectclause
			if(queryData.GetSizeOfClause(SELECT) == 0)	
			{
				std::cout << "Invalid Query: No synonym in Select<>\n";
				return false;
			}
		}

		//Select BOOLEAN
		else if(IsBoolean(token))
		{
			DebugMessage("\nIn BOOLEAN\n" , VALIDATEQUERY);

			Synonym synonym;
			synonym.type = BOOLEAN;
			synonym.value = "BOOLEAN";

			queryData.InsertSelect(synonym);
		}
		
		//select a
		else
		{
			DebugMessage("\nIn normal Select\n" , VALIDATEQUERY);
			Synonym synonym;
			synonym.value = token;

			if(!ValidateSelect(synonym))	return false;			
			else queryData.InsertSelect(synonym);
		}
		
	}

	//no select
	else 
	{
		std::cout << "Invalid Query: No select\n";
		return false;
	}

	if(AbstractWrapper::GlobalStop)	return false;

	//get next token
	if(++it == tokenList.end())	return true;
	token = *it;
	DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

	//validate such that, pattern
	while(IsSuchThat(token) || IsPattern(token) || IsWith(token)) 
	{
		bool endOfQuery = false, hasSuchThat = false, hasPattern = false, hasWith = false;
		bool repeat = false;	//repeat after AND

		while(IsSuchThat(token) || repeat)	
		{
			DebugMessage("\nIn such that\n" , VALIDATEQUERY);

			//First time such that
			if(!repeat)
			{
				DebugMessage("\nIn ignore \"that\"\n" , VALIDATEQUERY);
				if(++it == tokenList.end())	return false;	//ignore "that"
				if(*it != "that") return false;		//must be "that"
				token = *it;
				DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);
			}

			//and ...
			else
			{
				//and such that or and Modifies(), to check which, peek into it + 1, but don't ++it
				//if it + 1 = tokenList.end() return false
				//if it + 1 is such, ++it 2 times and check for tokenlist.end
				//if it + 1 is not such, then do nothing
				std::vector<std::string>::iterator nextIter = it + 1;
				if(nextIter == tokenList.end()) return false;

				if(IsSuchThat(*nextIter))
				{
					DebugMessage("\nIn ignore \"that\"\n" , VALIDATEQUERY);

					if(++it == tokenList.end()) return false;
					DebugMessage(std::string("tokens: " + *it + "\n") , VALIDATEQUERY);
					if(*it != "such") return false;		//must be "such"

					if(++it == tokenList.end()) return false;
					DebugMessage(std::string("tokens: " + *it + "\n") , VALIDATEQUERY);
					if(*it != "that") return false;		//must be "that"
				}
			}

			if(++it == tokenList.end())	return false;	//get RelationshipType
			token = *it;
			DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

			Argument arg1, arg2;

			if(IsRelationship(token)) //uses, modifies, parents, etc.
			{
				DebugMessage("\nIn relationship\n" , VALIDATEQUERY);
				std::string rel = token;			

				if(++it == tokenList.end())	return false;	//get arg1
				token = *it;
				arg1.value = token;
				DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

				if(++it == tokenList.end())	return false;	//get arg2
				token = *it;
				arg2.value = token;
				DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

				RelationshipType rel_enum;

				if(!ValidateRelationship(rel, rel_enum, arg1, arg2))	return false;

				queryData.InsertSuchThat(rel_enum, arg1, arg2);
			}

			else return false;

			if(++it == tokenList.end())	return true;
			else token = *it;
			DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

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

			DebugMessage("\nIn pattern\n" , VALIDATEQUERY);

			//and pattern a(_,_) / and w(_,_)
			if(repeat)
			{
				std::vector<std::string>::iterator nextIter = it + 1;
				if(nextIter == tokenList.end()) return false;

				if(IsPattern(*nextIter))
				{
					DebugMessage("\n++it from and to pattern\n" , VALIDATEQUERY);
					if(++it == tokenList.end()) return false;
					token = *it;
					DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);
				}
			}

			Argument arg1, arg2;
			Synonym synonym;

			if(++it == tokenList.end())	return false;	//get type
			token = *it;
			synonym.value = token;
			DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

			if(++it == tokenList.end())	return false;	//get arg1
			token = *it;
			arg1.value = token;
			DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

			if(++it == tokenList.end())	return false;	//get arg2
			token = *it;
			arg2.value = token;
			DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

			//get the type of synonym
			if(!QueryData::IsSynonymExist(synonym.value, synonym.type)) return false;
			
			if(synonym.type == IF)
			{
				DebugMessage("\nIn if\n" , VALIDATEQUERY);

				if(++it == tokenList.end())	return false;	//get arg3 and ignore it, since it must be _
				DebugMessage(std::string("tokens: " + *it + "\n") , VALIDATEQUERY);
				Argument arg3;
				arg3.value = *it;
				
				if(!ValidatePattern(synonym, arg1, arg2, arg3))	return false;
				queryData.InsertPattern(synonym, arg1, arg2);	//just insert 2, because only the first one matter for IF
			}
			
			else if(synonym.type == WHILE || synonym.type == ASSIGN)
			{
				DebugMessage("\nIn while/assign\n" , VALIDATEQUERY);
				if(!ValidatePattern(synonym, arg1, arg2))	return false;
				queryData.InsertPattern(synonym, arg1, arg2);
			}

			else {
				std::cout << "Invalid Query: Pattern synonym is not declared.\n";
				return false;
			}

			if(++it == tokenList.end())	return true;
			else token = *it;
			DebugMessage(std::string("tokens: " + *it + "\n") , VALIDATEQUERY);

			if(IsAnd(token))	repeat = true;
			else				repeat = false;

			if(AbstractWrapper::GlobalStop)	return false;
		}

		while(IsWith(token) || repeat)	//with or and
		{	
			DebugMessage("\nIn with\n" , VALIDATEQUERY);

			//and with a=b / and a=b
			if(repeat)
			{
				std::vector<std::string>::iterator nextIter = it + 1;
				if(nextIter == tokenList.end()) return false;

				if(IsWith(*nextIter))
				{
					DebugMessage("\n++it from and to with\n" , VALIDATEQUERY);
					if(++it == tokenList.end()) return false;
					token = *it;
					DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);
				}
			}

			Argument arg1, arg2;
			std::string lhs, rhs;

			if(++it == tokenList.end())	return false;	//get arg1
			token = *it;
			lhs = token;
			DebugMessage(std::string("tokens: " + *it + "\n") , VALIDATEQUERY);

			//ok when tokenize, a.stmt# is 1 token, dont separarate a . stmt#, when validateWith only check
			//get first arg token, then get next token must be =
			//then get get next token
			//then validatewith with this 2 token
			//e.g. token 1 = a.stmt# ,token 2 = "x" , then in validatewith only separate them and check


			//skip "="
			if(++it == tokenList.end())	return false;	
			DebugMessage(std::string("tokens: " + *it + "\n") , VALIDATEQUERY);
			if(!IsEqual(*it)) return false;

			if(++it == tokenList.end())	return false;	//get arg2
			DebugMessage(std::string("tokens: " + *it + "\n") , VALIDATEQUERY);
			rhs = *it;

			if(!ValidateWith(arg1, arg2, lhs, rhs))	return false;
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
	const SynonymType list[] = {ASSIGN, STMT, WHILE, IF, PROCEDURE, VARIABLE, CONSTANT, PROG_LINE};
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

bool QueryPreProcessor::ValidateWith(Argument& arg1, Argument& arg2, std::string lhs, std::string rhs)
{
	//verify synonym has the correct attrRef
	//a.value ccannot, a.procname cannot
	//lhs can be a.stmt# , c.value , p.procName , v.varName , n
	//rhs can be a.stmt# , c.value , p.procName , v.varName , "ident" , 5
	
	//first check what is lhs, must be _._ or a delared n

	//if IsAttrRef(lhs, syn, attrName) 
	//	arg1.type = SYNONYM, arg1.value = syn.value, arg1.syn = syn
	//	if attrName = stmt# || attrName == value
	//		if IsInteger(rhs)	arg2.type = INTEGER arg2.value = rhs
	//		else if IsAttrRef(rhs,syn,attrname)	if(attrName == stmt# || attrName == value) arg2.type = SYNONYM, .value =syn.value .syn = syn
	//		else return false
	//	else if attrName = varName || attrName == procName
	//		if IsIdent(rhs)	arg2.type = IDENT arg2.value = rhs
	//		else if IsAttrRef(rhs,syn,attrname)	if(attrName == varname || attrName == procName) arg2.type = SYNONYM, .value =syn.value .syn = syn
	//		else return false

	//else if IsSynonymExist(lhs,PROG_LINE)
	//	if isInteger(rhs)	arg2.type = INTEGER arg2.value = rhs
	//	else if isValidAttrRef(rhs , syn , attrName) if(attrName == stmt# || attrName == value) arg2.type = SYNONYM, .value =syn.value .syn = syn	no need to store attrName, if its valid here then it doesnt matter in evaluation, not gonna use it
	//	else return false

	//else return false



	//isValidAttrRef() will tokenize and check whether syn is declared, and whether syn has correct attrRef, then return it

	return false;
}

bool QueryPreProcessor::IsValidAttrRef(std::string attrRef , Synonym& syn , AttrName& attrName)
{
	//tokenize rhs by .
	//check tokens, size = 3, middle is dot
	//check is syn declared, if not return false
	//if yes assign to syn

}



/*
_"factor"_
_"factor+factor"_
_
factor : NAME | INTEGER
assume whitespace hasn't been remove
" x ", _ " x + y" _ are also valid
assume _ and " " is included, e.g. _"x"_

can include () in expression
assume the expression is valid, does not check for things like _"x++y**z"_ , "()x+-y" etc, this need check 1 char at a time
and push ( on a stack, pop when reach )
*/
bool QueryPreProcessor::IsExpression(std::string str)
{
	//remove white spaces
	str.erase(std::remove_if(str.begin(), str.end(), [](char x){return isspace(x);}), str.end());
	
	//eliminates " " or _" "_ and get the content
	//if(str.at(0) == '\"' && str.at(str.length()-1) == '\"')		//"..."
	//	str = str.substr(1, str.length()-2);

	if (str.at(0) == '_' && str.at(str.length()-1) == '_')	//_..._
	{
		if(str.at(1) == '\"' && str.at(str.length()-2) == '\"')	//_"..."_
		{
			int length = str.length() - 4;
			str = str.substr(2, length);
		}

		else return false;
	}

	else if(str.at(0) == '\"' && str.at(str.length()-1) == '\"')	//"..."
	{
		int length = str.length() - 4;
		str = str.substr(2, length);
	}

	else return false;

	std::vector<std::string> tokenList;
	std::string delim = "+-*()";

	Tokenize(str, tokenList, delim);

	for(std::vector<std::string>::iterator it = tokenList.begin(); it != tokenList.end(); ++it) {
		if(!(IsIdent(*it) || IsInteger(*it)))
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

bool QueryPreProcessor::IsDot(std::string str)
{
	if(str == ".")	return true;
	return false;
}

bool QueryPreProcessor::IsEqual(std::string str)
{
	if(str == "=")	return true;
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

		DebugMessage(std::string("char[" + std::to_string(long long(pos)) + "] : " + currentChar + "\n") , TOKENIZER);

		//integer 0-9
		if (currentChar >= 48 && currentChar <= 57) 
		{
			//std::cout << "In Integer\n";
			if (alphaString == "")	integer = integer + currentChar;	
			else					alphaString = alphaString + currentChar;	//the integer belong to part of a string
		} 
		
		//character A-Z | a-z | # | .
		else if ((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122) || (currentChar == 35)) 
		{
			//std::cout << "In alphabet\n";
			if (integer != "")	return false;	//should not happen
			else				alphaString = alphaString + currentChar;
		} 
		
		// symbol, whitespace or endline
		else 
		{ 
			//std::cout << "In Others\n";
			//push back integer/string first 
			if (integer != "") 
			{ 
				tokens.push_back(integer);
				integer = "";
			} 

			else if (alphaString != "") 
			{ 
				//only those that need to push together with alphastring like stmt#, affects* need to be here
				//or need to set some shit like isProgLine

				//prog_line, wait for _ , don't push
				if(alphaString == "prog") 
					isProgLine = true;

				//Parent*/Next* etc
				else if(currentChar == '*') 
				{
					//* can be Affects* or in the middle of an expression _"x*y"_
					if(isExpression || isIdent)
					{
						alphaString += currentChar;
						isIdent = false;
						continue;
					}

					else
					{
						tokens.push_back(alphaString+"*");
						alphaString = "";
						continue;
					}
				}

				else if(currentChar == '.')
				{
					alphaString += currentChar;
				}

				//if string begins with " or _ which means an expression or IDENT, don't push
				else if(!(alphaString.at(0) == '"' || alphaString.at(0) == '_'))
				{
					tokens.push_back(alphaString);
					alphaString = "";
				}
			}

		//if it is ( , just ignore
			//if it is ) and has string before it, push that string, else ignore
			if (currentChar == '(' || currentChar == ')') 
			{
				//Parent(w,_), the underscore will set isExpression to true
				//"(x+y)*z"
			
				DebugMessage("In ()\n" , TOKENIZER);
				//handle _, e.g. Parent(w,_)
				//ACHTUNGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
				//IN ORDER TO SUPPORT BOTH (A,_  ) AND _"(X+Y)*Z"_ / "(X+Y)*Z"
				//if(isExpression)
				//find " from the end of alphastring, if no " is found, that means not an expression, just underscore
				//in that case remove any white space, then push the _ back, reset, continue
				//if a * is found, then its an expression
				//alphaString += currentChar, continue

				//ALSO, PUT ALL KIND OF SPACE IN THE QUERY IN UNIT TEST, ALL KIND OF GANINSZZZZZZZ


				//for (, only need check whether its in expression
				//for ), need check after _ or in expression
					
			
				if(isExpression)
				{
					//just concatenate with alphastring
					if(currentChar == '(')
					{
						alphaString += currentChar;
						//should set isIdent to false since ident cannot have (
					}

					else
					{
						//find " from the end of alphastring, if no " is found, that means not an expression, just underscore
						//in that case remove any white space, then push the _ back, reset, continue
						//if a * is found, then its an expression
						//alphaString += currentChar, continue
						std::size_t found = alphaString.find_first_of("\"");
						//found, _"(
						if (found!=std::string::npos)
						{
							alphaString += currentChar;
						}
						//not found, (a,_  )
						else
						{
							tokens.push_back("_");
							alphaString = "";
							isExpression = false;
							isIdent = false;
							//should set isIdent to false since ident cannot have (
						}

					}
				}

				//normal (), push back string before ( and )
				else
				{
					if(currentChar == ')')
					{
						if(alphaString != "")
						{	
							tokens.push_back(alphaString);
							alphaString = "";
						}
						else continue;
					}
				}			
				
			}	
			
			else if(currentChar == '=')
			{
				DebugMessage("In =\n" , TOKENIZER);
				tokens.push_back("=");
			}	

			else if(currentChar == '<')
			{
				DebugMessage("In <\n" , TOKENIZER);
				tokens.push_back("<");
			}

			else if(currentChar == '>')
			{
				DebugMessage("In >\n" , TOKENIZER);
				tokens.push_back(">");
			}

			else if(currentChar == ',') 
			{
				DebugMessage("In ,\n" , TOKENIZER);

				//this means before , is a _ , so it cannot be an expression, just push the underscore, which is the first char of the string
				if(isExpression)	
				{
					DebugMessage("In isExpression\n" , TOKENIZER);

					std::string cs(1,alphaString.at(0));
					tokens.push_back(cs);
					isExpression = false;
					alphaString = "";
				}

				else continue;
			}

			//if is white space, depends on previous string
			else if (currentChar == '	' || currentChar == '\r' || currentChar == '\n' || currentChar == ' ') {
				DebugMessage("In white space\n" , TOKENIZER);

				//if previous string is not empty means it is an expression or IDENT " x " , " x + y " , _ " x + 5 " _ or just _ 
				//if former case, do not ignore white space
				//if latter case, pushback tokens

				//situation like (a,_ ), just += whitespace with _, let ) do the work
				if (alphaString != "")
					alphaString += currentChar;
				
				//if previous string is empty, ignore whitespace
				else continue;
			} 

			else if(currentChar == ';')
			{
				DebugMessage("In ;\n" , TOKENIZER);
				tokens.push_back(";");
			}

			else if(currentChar == '"')
			{
				DebugMessage("In \"\n" , TOKENIZER);

				//if alphastring is empty, add to alphastring
				//else if alphastring is not empty, check if first char is either " or _, if yes add to alphastring and push back, clear alphastring
				//else return false

				//when encounter a " , we dont know its an expression or ident, can be  "x+y" or "x"
				//so set both isIdent and isExpression to true
				//when encounter integer or name, if any of them is true dont push
				//when we encounter the first operator +/-/*, set isIdent to false
				//when encounter the second ", if isIdent is still true, then it is an ident
				//push, and turn both flag to false
				//if isident is false when encounter second ", then it is expression (coz operator switch off the flag)
				//check if first char is _, if yes dont push, wait for second_, if no then push, reset everything

				//beginning of an IDENT or EXP, add to current string
				if(alphaString == "") {
					DebugMessage("In empty string\n" , TOKENIZER);

					alphaString += currentChar;
					isIdent = true;
					isExpression = true;
				}


				//I dont know if "x" can be an expression like pattern a("x","y"), now I assume cannot
				//only _"x"_ , _"x+y"_, "x+y"

				//end of an IDENT or EXP, or middle of an EXP just before _
				else {
					//"x" or "x*y"
					if(isIdent) {
						DebugMessage("In isIdent\n" , TOKENIZER);

						alphaString += currentChar;
						tokens.push_back(alphaString);
						alphaString = "";
						isIdent = false;
						isExpression = false;
					}

					//_"x + y"_
					else if(isExpression) {
						DebugMessage("In isExpression\n" , TOKENIZER);
						
						//_"x+y"_
						if(alphaString[0] == '_')
						{
							alphaString += currentChar;
						}
						//"x+y"
						else 
						{
							alphaString += currentChar;
							tokens.push_back(alphaString);
							alphaString = "";
							isIdent = false;
							isExpression = false;
						}
					}

					//Mordor, one does not simply walk into here
					else {
						DebugMessage("One does not simply walk into here\n" , TOKENIZER);
						return false;
					}
				}
			}

			else if(currentChar == '_')
			{
				DebugMessage("In _\n" , TOKENIZER);

				//no string before _ means start of an expression or just an underscore
				if(alphaString == "") {
					DebugMessage("In empty string\n" , TOKENIZER);

					alphaString += currentChar;
					isExpression = true;
				}

				//end of an expression
				else {
					if(isExpression) {
						DebugMessage("In isExpression\n" , TOKENIZER);

						alphaString += currentChar;
						tokens.push_back(alphaString);
						alphaString = "";
						isExpression = false;
					}
					//prog_
					else if(isProgLine) {
						DebugMessage("In isProgLine\n" , TOKENIZER);
						alphaString += currentChar;
						isProgLine = false;
					}
					else {
						DebugMessage("There is character before _ but !isProgLine and !isExpression\n" , TOKENIZER);
						return false;
					}
				}
			}

			//+ - * can only occur in an expression
			else if(currentChar == '+' || currentChar == '-') 
			{
				std::stringstream ss;
				ss << "In " << currentChar << "\n";
				DebugMessage(ss.str() , TOKENIZER);

				//part of an expression
				if(isExpression || isIdent) {
					DebugMessage("In isExpression\n" , TOKENIZER);
					alphaString += currentChar;
					isIdent = false;
				}

				else {
					std::cout << "Invalid character " << std::string(1,currentChar) << "\n";
					return false;
				}
			}
		}

		DebugMessage("End of checking current character\n" , TOKENIZER);
	}

	return true;
}

void QueryPreProcessor::DebugMessage(std::string msg , FUNCTION function)
{
	if(function == TOKENIZER)
	{
		#ifdef DEBUG_MSG_TOKENIZER
			std::cout << msg;
		#endif
	}

	else if(function == VALIDATEQUERY)
	{
		#ifdef DEBUG_MSG_VALIDATEQUERY
			std::cout << msg;
		#endif
	}

	
}