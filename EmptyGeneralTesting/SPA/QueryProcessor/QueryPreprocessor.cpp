#include "QueryPreProcessor.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <deque>   
#include "RelTable.h"
#include "..\..\AutoTester\source\AbstractWrapper.h"


const std::string QueryPreProcessor::de[] = {"stmt", "assign", "while", "if", "variable", "procedure", "prog_line", "constant", "call"};
const std::string QueryPreProcessor::rel[] = {"Modifies", "Uses", "Parent", "Parent*", "Follows", "Follows*", "Calls", "Calls*", "Next", "Next*", "Affects", "Affects*"};
 
QueryPreProcessor::QueryPreProcessor(void) {}

bool QueryPreProcessor::ValidateQuery(std::string query, QueryData &queryData)
{
	std::vector<std::string> tokenList;
	std::string token;

	DebugMessage(std::string("\nValidating query: " + query + "\n") , VALIDATEQUERY);
	DebugMessage("Before tokenize\n" , VALIDATEQUERY);

	//tokenize query
	if(!Tokenize(query,tokenList)) {
		std::cout << "Invalid Query: Invalid Syntax.\n";
		return false;
	}

	DebugMessage("After tokenize\n" , VALIDATEQUERY);
	DebugMessage("Token list: " , VALIDATEQUERY);
	for(int i=0; i<tokenList.size(); ++i)
		DebugMessage(std::string(tokenList[i] + " | ") , VALIDATEQUERY);

	//get the first token iterator
	std::vector<std::string>::iterator it = tokenList.begin();
	token = *it;

	DebugMessage(std::string("\ntokens: " + token + "\n") , VALIDATEQUERY);

	//validate declaration
	while(IsDeclaration(token))	
	{
		DebugMessage("In declaration\n" , VALIDATEQUERY);

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
				DebugMessage("In Check Semicolon\n" , VALIDATEQUERY);

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
		DebugMessage("Afer Check Semicolon\n" , VALIDATEQUERY);

		if(++it == tokenList.end())	return false;
		token = *it;
		DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);
	}

	DebugMessage("Afer declaration\n" , VALIDATEQUERY);

	//check if declaration is empty
	int declarationSize = queryData.GetSizeOfClause(DECLARATION);
	std::vector<Declaration> dec = queryData.GetDeclarations();
	if(declarationSize == 0)	
	{
		std::cout << "Invalid Query: No declaration.\n";
		return false;
	}

	DebugMessage("Declaration: " , VALIDATEQUERY);
	for(int i=0; i < declarationSize; ++i)
		DebugMessage(std::string(dec[i].synonym.value + " "), VALIDATEQUERY);
	DebugMessage("\n" , VALIDATEQUERY);

	if(AbstractWrapper::GlobalStop)	return false;

	//check if next token is select, and validate select
	if(IsSelect(token))
	{
		DebugMessage("In Select\n" , VALIDATEQUERY);

		if(++it == tokenList.end())	return false;
		token = *it;
		DebugMessage(std::string("tokens: " + token + "\n") , VALIDATEQUERY);

		//select <a,w,v>
		if(IsOpenBracket(token))
		{
			DebugMessage("In <\n" , VALIDATEQUERY);

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
			DebugMessage("In normal Select\n" , VALIDATEQUERY);
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
			DebugMessage("In such that\n" , VALIDATEQUERY);

			//First time such that
			if(!repeat)
			{
				DebugMessage("In ignore \"that\"\n" , VALIDATEQUERY);
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
					DebugMessage("In ignore \"that\"\n" , VALIDATEQUERY);

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
				DebugMessage("In relationship\n" , VALIDATEQUERY);
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

		repeat = false;

		while(IsPattern(token) || repeat)	//pattern or and
		{		
			//std::cout << "In Pattern\n";
			//pattern a("x",_)
			//pattern if("x",_,_)
			//pattern w("w",_)

			DebugMessage("In pattern\n" , VALIDATEQUERY);

			//and pattern a(_,_) / and w(_,_)
			if(repeat)
			{
				DebugMessage("In repeat\n" , VALIDATEQUERY);

				std::vector<std::string>::iterator nextIter = it + 1;
				if(nextIter == tokenList.end()) return false;

				if(IsPattern(*nextIter))
				{
					DebugMessage("++it from and to pattern\n" , VALIDATEQUERY);
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
			if(!QueryData::IsSynonymExist(synonym.value, &synonym.type)) 
			{
				DebugMessage(std::string("Pattern synonym " + synonym.value + " is not declared\n"));
				return false;
			}

			if(synonym.type == IF)
			{
				DebugMessage("In if\n" , VALIDATEQUERY);

				if(++it == tokenList.end())	return false;	//get arg3 
				DebugMessage(std::string("tokens: " + *it + "\n") , VALIDATEQUERY);
				Argument arg3;
				arg3.value = *it;
				
				if(!ValidatePattern(synonym, arg1, arg2, arg3))	
				{
					DebugMessage("Invalid pattern\n");
					return false;
				}
				//if expression, remove white space first
				queryData.InsertPattern(synonym, arg1, arg2);	//just insert 2, because only the first one matter for IF
			}
			
			else if(synonym.type == WHILE || synonym.type == ASSIGN)
			{
				DebugMessage("In while/assign\n" , VALIDATEQUERY);
				if(!ValidatePattern(synonym, arg1, arg2))
				{
					DebugMessage("Invalid pattern\n");
					return false;
				}

				queryData.InsertPattern(synonym, arg1, arg2);
			}

			else {
				DebugMessage("Pattern synonym is not IF, WHILE or ASSIGN.\n");
				return false;
			}

			if(++it == tokenList.end())	return true;
			else token = *it;
			DebugMessage(std::string("tokens: " + *it + "\n") , VALIDATEQUERY);

			if(IsAnd(token))	repeat = true;
			else				repeat = false;

			if(AbstractWrapper::GlobalStop)	return false;
		}

		repeat = false;

		while(IsWith(token) || repeat)	//with or and
		{	
			DebugMessage("In with\n" , VALIDATEQUERY);

			//and with a=b / and a=b
			if(repeat)
			{
				std::vector<std::string>::iterator nextIter = it + 1;
				if(nextIter == tokenList.end()) return false;

				if(IsWith(*nextIter))
				{
					DebugMessage("++it from and to with\n" , VALIDATEQUERY);
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

//General tokenizer with delimiters
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
	const SynonymType list[] = {ASSIGN, STMT, WHILE, IF, CALL, PROCEDURE, VARIABLE, CONSTANT, PROG_LINE};
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


bool QueryPreProcessor::ValidatePattern(Synonym synonym, Argument &arg1, Argument &arg2)
{
	DebugMessage(std::string("In ValidatePattern ASSIGN/WHILE, syn: " + synonym.value + " , arg1: " + arg1.value + " , arg2: " + arg2.value + "\n") , VALIDATEPATTERN);

	if(synonym.type == ASSIGN && QueryData::IsSynonymExist(synonym.value , ASSIGN))
	{
		DebugMessage("Synonym is is ASSIGN\n", VALIDATEPATTERN);

		if(arg1.value == "_") 
		{
			DebugMessage("ARG1 is UNDERSCORE\n", VALIDATEPATTERN);
			arg1.type = UNDERSCORE;
		}

		else if(QueryData::IsSynonymExist(arg1.value, VARIABLE))	//must be variable
		{
			DebugMessage("ARG1 is a VARIABLE SYNONYM\n", VALIDATEPATTERN);
			arg1.type = SYNONYM;
			arg1.syn = Synonym(arg1.value, VARIABLE);
		}
			
		else if(IsIdent(arg1.value)) 
		{
			DebugMessage("ARG1 is IDENT\n", VALIDATEPATTERN);
			std::string ident = arg1.value;
			ident.erase(std::remove_if(ident.begin(), ident.end(), [](char x){return isspace(x);}), ident.end());
			arg1.value = ident;
			arg1.type = IDENT;
		}

		else {
			DebugMessage("In ValidatePattern ASSIGN/WHILE, invalid Argument 1 type for ASSIGN pattern.\n");
			return false;
		}

		if(arg2.value == "_")
		{
			DebugMessage("ARG2 is UNDERSCORE\n", VALIDATEPATTERN);
			arg2.type = UNDERSCORE;
		}

		else if(IsExpression(arg2.value)) 
		{
			DebugMessage("ARG2 is EXPRESSION\n", VALIDATEPATTERN);
			std::string exp = arg2.value;
			//remove white space, make life easer when doing pattern matching later
			exp.erase(std::remove_if(exp.begin(), exp.end(), [](char x){return isspace(x);}), exp.end());
			arg2.value = exp;
			arg2.type = EXPRESSION;
		}

		else {
			DebugMessage("In ValidatePattern ASSIGN/WHILE, invalid Argument 2 type for ASSIGN pattern.\n");
			return false;
		}

		return true;
	}
	
	else if(synonym.type == WHILE && QueryData::IsSynonymExist(synonym.value , WHILE))
	{
		DebugMessage("Synonym is is WHILE\n", VALIDATEPATTERN);

		if(!IsUnderscore(arg2.value)) 
		{
			DebugMessage("In ValidatePattern ASSIGN/WHILE, invalid Argument 2 type for WHILE pattern, must be _.\n");
			return false;		//arg2 must be _
		}

		if(IsUnderscore(arg1.value))
		{
			DebugMessage("ARG1 is UNDERSCORE\n", VALIDATEPATTERN);
			arg1.type = UNDERSCORE;
		}
		else if(QueryData::IsSynonymExist(arg1.value, VARIABLE))	//must be variable
		{
			DebugMessage("ARG1 is VARIABLE SYNONYM\n", VALIDATEPATTERN);
			arg1.type = SYNONYM;
			arg1.syn = Synonym(arg1.value, VARIABLE);
		}
			
		else if(IsIdent(arg1.value)) 
		{
			DebugMessage("ARG1 is IDENT\n", VALIDATEPATTERN);
			std::string ident = arg1.value;
			ident.erase(std::remove_if(ident.begin(), ident.end(), [](char x){return isspace(x);}), ident.end());
			arg1.value = ident;
			arg1.type = IDENT;
		}

		else 
		{
			DebugMessage("In ValidatePattern ASSIGN/WHILE, invalid Argument 1 type for WHILE pattern.\n");
			return false;
		}

		return true;
	}

	else 
	{
		DebugMessage("In ValidatePattern ASSIGN/WHILE, pattern has 2 argument but the synonym type is not ASSIGN or WHILE.\n");
		return false;
	}
}


//For IF pattern , take 3 argument
//this function can call the above validatepattern with while
bool QueryPreProcessor::ValidatePattern(Synonym synonym, Argument &arg1, Argument &arg2, Argument &arg3)
{
	//pattern if(v,_,_)
	//pattern if("x",_,_)
	//pattern if(_,_,_)

	//make sure the syn type is IF, and it is declared
	if(synonym.type == IF && QueryData::IsSynonymExist(synonym.value , IF))
	{
		DebugMessage("SYNONYM is IF is UNDERSCORE\n", VALIDATEPATTERN);
		//take care of arg2 and arg3 first
		if(!IsUnderscore(arg2.value) || !IsUnderscore(arg3.value)) 
		{
			DebugMessage("In ValidatePattern IF, invalid Argument 2 and 3 type for IF pattern, both must be _.\n");
			return false;		//arg2 and arg3 must be _
		}

		else 
		{
			DebugMessage("ARG2 and ARG3 are UNDERSCORE\n", VALIDATEPATTERN);
			arg2.type = UNDERSCORE;
			arg3.type = UNDERSCORE;
		}


		if(IsUnderscore(arg1.value)) 
		{
			DebugMessage("ARG1 is UNDERSCORE\n", VALIDATEPATTERN);
			arg1.type = UNDERSCORE;
		}

		else if(QueryData::IsSynonymExist(arg1.value, VARIABLE))	//must be variable
		{
			DebugMessage("ARG1 is VARIABLE SYNONYM\n", VALIDATEPATTERN);
			arg1.type = SYNONYM;
			arg1.syn = Synonym(arg1.value, VARIABLE);
		}
			
		else if(IsIdent(arg1.value)) 
		{
			DebugMessage("ARG1 is IDENT\n", VALIDATEPATTERN);
			std::string ident = arg1.value;
			ident.erase(std::remove_if(ident.begin(), ident.end(), [](char x){return isspace(x);}), ident.end());
			arg1.value = ident;
			arg1.type = IDENT;
		}

		else {
			DebugMessage("In ValidatePattern IF, invalid Argument 1 type for IF pattern.\n");
			return false;
		}

		return true;
	}

	else 
	{
		DebugMessage("In ValidatePattern IF, pattern has 3 argument but the synonym type is not IF.\n");
		return false;
	}
}


bool QueryPreProcessor::ValidateWith(Argument& arg1, Argument& arg2, std::string lhs, std::string rhs)
{
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
	DebugMessage(std::string("In ValidateWith, lhs :" + lhs + " , rhs: " + rhs + "\n") , VALIDATEWITH);

	Synonym syn1 , syn2;
	AttrNameType attrName1, attrName2;

	if(IsValidAttrRef(lhs , syn1 , attrName1))
	{
		DebugMessage("LHS is valid attrRef\n" , VALIDATEWITH);

		arg1.type = SYNONYM;
		arg1.value = syn1.value;
		arg1.syn = syn1;

		if(attrName1 == STMTNUM || attrName1 == VALUE)
		{
			DebugMessage("LHS attrName is STMT\\VALUE\n" , VALIDATEWITH);

			if(IsInteger(rhs))
			{
				DebugMessage("RHS is integer\n" , VALIDATEWITH);
				arg2.type = INTEGER;
				arg2.value = rhs;
			}

			else if(IsValidAttrRef(rhs , syn2 , attrName2))
			{
				DebugMessage("RHS is valid attrRef\n" , VALIDATEWITH);

				if(attrName2 == STMTNUM || attrName2 == VALUE)
				{
					DebugMessage("RHS attrName is STMTNUM\\VALUE\n" , VALIDATEWITH);
					arg2.type = SYNONYM;
					arg2.value = syn2.value;
					arg2.syn = syn2;
				}

				else 
				{
					DebugMessage("In ValidateWith, LHS is .stmt\\.value, RHS is a valid attrRef but the attrName does not match.\n");
					return false;
				}
			}

			else if(QueryData::IsSynonymExist(rhs , PROG_LINE))
			{
				DebugMessage("RHS is prog_line, synonym exist\n" , VALIDATEWITH);
				arg2.type = SYNONYM;
				arg2.value = lhs;
				arg2.syn.type = PROG_LINE;
				arg2.syn.value = lhs;
			}

			else
			{
				DebugMessage("In ValidateWith, LHS is .stmt\\.value, RHS is not integer, prog_line, or a valid attrRef.\n");
				return false;
			}
		}
		
		else if(attrName1 == VARNAME || attrName1 == PROCNAME)
		{
			DebugMessage("LHS attrName is STMT/VALUE\n" , VALIDATEWITH);

			if(IsIdent(rhs))
			{
				DebugMessage("RHS is ident\n" , VALIDATEWITH);
				std::string ident = rhs;
				ident.erase(std::remove_if(ident.begin(), ident.end(), [](char x){return isspace(x);}), ident.end());
				arg2.value = ident;
				arg2.type = IDENT;
			}

			else if(IsValidAttrRef(rhs , syn2 , attrName2))
			{
				DebugMessage("RHS is valid attrRef\n" , VALIDATEWITH);
				
				if(attrName2 == VARIABLE || attrName2 == PROCEDURE)
				{
					DebugMessage("RHS attrName is VARIABLE\\PROCEDURE\n" , VALIDATEWITH);
					arg2.type = SYNONYM;
					arg2.value = syn2.value;
					arg2.syn = syn2;
				}

				else 
				{
					DebugMessage("In ValidateWith, LHS is .varName/.procName, RHS is a valid attrRef but the attrName does not match.\n");
					return false;
				}
			}

			else
			{
				DebugMessage("In ValidateWith, LHS is .varName/.procName, RHS is not IDENT or a valid attrRef.\n");
				return false;
			}
		}

		else
		{
			DebugMessage("In ValidateWith, LHS has invalid attrName.\n");
			return false;
		}
	}

	else if(QueryData::IsSynonymExist(lhs , PROG_LINE))
	{
		DebugMessage("LHS is prog_line, synonym exist\n" , VALIDATEWITH);

		arg1.type = SYNONYM;
		arg1.value = lhs;
		arg1.syn.type = PROG_LINE;
		arg1.syn.value = lhs;


		if(IsInteger(rhs))
		{
			DebugMessage("RHS is integer\n" , VALIDATEWITH);
			arg2.type = INTEGER;
			arg2.value = rhs;
		}

		else if(IsValidAttrRef(rhs , syn2 , attrName2))
		{
			DebugMessage("RHS is valid attrRef\n" , VALIDATEWITH);

			if(attrName2 == STMTNUM || attrName2 == VALUE)
			{
				DebugMessage("RHS attrName is STMTNUM\\VALUE\n" , VALIDATEWITH);
				arg2.type = SYNONYM;
				arg2.value = syn2.value;
				arg2.syn = syn2;
			}

			else
			{
				DebugMessage("In ValidateWith, LHS is prog_line, RHS is a valid attrRef but the attrName does not match.\n");
				return false;
			}
		}

		else if(QueryData::IsSynonymExist(rhs , PROG_LINE))
		{
			DebugMessage("RHS is prog_line, synonym exist\n" , VALIDATEWITH);
			arg2.type = SYNONYM;
			arg2.value = lhs;
			arg2.syn.type = PROG_LINE;
			arg2.syn.value = lhs;
		}

		else
		{
			DebugMessage("In ValidateWith, LHS is prog_line, RHS is not IDENT, prog_line or a valid attrRef.\n");
			return false;
		}
	}
	
	else
	{
		DebugMessage("In ValidateWith, LHS is not a valid attrRef or prog_line");
		return false;
	}

	return true;
}

//with lhs = rhs
//tokenize lhs either rhs and check whether it is a valid attrRef
bool QueryPreProcessor::IsValidAttrRef(std::string attrRef , Synonym& syn , AttrNameType& attrName)
{
	//tokenize rhs by .
	//check tokens, size = 3, middle is dot
	//check is syn declared, if not return false
	//if yes assign to syn
	DebugMessage("In IsValidAttrRef\n" , ISVALIDATTRREF);

	std::vector<std::string> tokenList;
	std::string delim = ".";

	Tokenize(attrRef, tokenList, delim);

	//attrRef must have 3 component - synonym . attrName
	//tokenize return 2 component after remove .
	if(tokenList.size () != 2)
		return false;

	for(unsigned int i=0; i<2; ++i)
		DebugMessage(std::string("token " + std::to_string(long long(i)) + ": " + tokenList[i] + "\n") , ISVALIDATTRREF);

	SynonymType synType = INVALID_SYNONYM_TYPE;;
	std::string synValue = tokenList[0];
	std::string attrNameStr = tokenList[1];

	if(QueryData::IsSynonymExist(tokenList[0] , &synType))
	{
		DebugMessage("Synonym exist\n" , ISVALIDATTRREF);

		//a/s/w/i . stmt#
		if(synType == ASSIGN || synType == STMT || synType == WHILE || synType == IF)
		{
			DebugMessage("Synonym type exist in A/S/W/I\n" , ISVALIDATTRREF);
			syn.type = synType;
			syn.value = synValue;

			AttrNameType type = GetEnumAttrNameType(attrNameStr);
			if(type == STMTNUM)		attrName = type;
			else return false;
		}

		//c.value
		else if(synType == CONSTANT)
		{
			DebugMessage("Synonym type exist in C\n" , ISVALIDATTRREF);
			syn.type = synType;
			syn.value = synValue;

			AttrNameType type = GetEnumAttrNameType(attrNameStr);
			if(type == VALUE)	attrName = type;
			else return false;
		}

		//v.varName
		else if(synType == VARIABLE)
		{
			DebugMessage("Synonym type exist in V\n" , ISVALIDATTRREF);
			syn.type = synType;
			syn.value = synValue;

			AttrNameType type = GetEnumAttrNameType(attrNameStr);
			if(type == VARNAME)	attrName = type;
			else return false;
		}

		//p.procName
		else if(synType == PROCEDURE)
		{
			DebugMessage("Synonym type exist in P\n" , ISVALIDATTRREF);
			syn.type = synType;
			syn.value = synValue;

			AttrNameType type = GetEnumAttrNameType(attrNameStr);
			if(type == PROCNAME) attrName = type;
			else return false;
		}
		
		else return false;
	}

	else return false;

	return true;
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

	if (str.size() > 4 && str.at(0) == '_' && str.at(str.length()-1) == '_')	//_..._
	{
		if(str.at(1) == '\"' && str.at(str.length()-2) == '\"')	//_"..."_
		{
			int length = str.length() - 4;
			str = str.substr(2, length);
		}

		else return false;
	}

	else if(str.size() > 2 && str.at(0) == '\"' && str.at(str.length()-1) == '\"')	//"..."
	{
		int length = str.length() - 2;
		str = str.substr(1, length);
		std::cout << str << "  here\n";
	}

	else return false;
	std::cout << str << "  here2\n";
	std::vector<std::string> tokenList;
	std::string delim = "+-*()";

	TokenizeExpression(str, tokenList, delim);

	if(tokenList.empty()) return false;


	//recursively check each (), starting from the innermost one
	//get the indices of (, and ) separately
	//if empty, just check basic
	//if not empty, both vector size must be same
	//for (.size times, get index of last (, delete it from vector, get index of first ), delete it
	//get the content of between ( ), which is index of ( + 1 to index of ) - 1
	//check basic

	std::deque<int> openBracketIndex , closeBracketIndex;


	std::size_t next = 0, pos;
	//get indices of open bracket (
	while((pos = str.find("(" , next)) != std::string::npos)
	{
		openBracketIndex.push_back(pos);
		next = pos + 1;
	}

	//get indices of open bracket )
	while((pos = str.find(")" , next)) != std::string::npos)
	{
		closeBracketIndex.push_front(pos);
		next = pos + 1;
	}

	//if no brackets, just check expression
	if(openBracketIndex.empty() && closeBracketIndex.empty())
	{
		//check subexpression
		if(IsValidExpression(tokenList))	return true;
		else						return false;
	}

	//has bracket(s), recursively check sub-expression
	else
	{
		//both brackets count must be same
		if(openBracketIndex.size() == openBracketIndex.size())
		{
			std::string exp = str;
			int OBIndex_prev = -1 , CBIndex_prev = -1;

			while(!openBracketIndex.empty())
			{
				int OBIndex_current = openBracketIndex.back();
				int CBIndex_current = closeBracketIndex.back();
				openBracketIndex.pop_back();
				closeBracketIndex.pop_back();

				//std::string innerExp = str.substr(OBIndex_current + 1 , CBIndex_current - OBIndex_current - 1);
				std::vector<std::string> innerExp(tokenList.begin() + OBIndex_current + 1, tokenList.begin() + CBIndex_current);

				//update prev bracket index to match current innerexp length
				if(OBIndex_prev != -1 && CBIndex_prev != -1)
				{
					OBIndex_prev -= OBIndex_current + 1;
					CBIndex_prev -= OBIndex_current + 1;
				}

				//if nothing between () 
				if(innerExp.empty()) 
				{
					return false;
				}

				else
				{
					//check subexpression
					if(IsValidExpression(innerExp , OBIndex_prev , CBIndex_prev))
					{
						OBIndex_prev = OBIndex_current;
						CBIndex_prev = CBIndex_current;
						return true;
					}
					else return false;
				}
			}
		}

		else return false;
	}




	/*Tokenize(str, tokenList, delim);

	for(std::vector<std::string>::iterator it = tokenList.begin(); it != tokenList.end(); ++it) {
		if(!(IsName(*it) || IsInteger(*it)))
			return false;
	}*/
	
	return true;
}

bool QueryPreProcessor::IsValidExpression(std::vector<std::string>& exp , int OBIndex_prev , int CBIndex_prev)
{
	//if both -1, just check usual
	//else, check from right of closebracket to end , and beginning to left of open bracket 

	if(OBIndex_prev == -1 && CBIndex_prev == -1)
	{
		
		//most basic expession must be odd number - x , x + 1 , x * y + z
		if(exp.size() % 2 == 0)	return false;

		for(int i = 0; i < exp.size(); ++i)
		{
			//even index must be name/integer
			if(i % 2 == 0)
			{
				if(!(IsName(exp[i]) || IsInteger(exp[i])))
					return false;

			
			}

			//odd index must be operator
			else
			{
				if(!(IsOperator(exp[i])))
					return false;
			}
		}
	}

	//x + ( x + y ) + z
	//( x + y ) * z
	//x * ( y + z)
	//wont have other brack
	else
	{
		
		//(x+y)+z
		if(OBIndex_prev == 0 && CBIndex_prev != exp.size() - 1)
		{
			std::cout << "Checking from the right\n";

			std::vector<std::string> right(exp.begin() + CBIndex_prev + 1 , exp.end());

			//right side cannot empty or only 1 token
			//cannot (x+y) or (x+y)+ or (x+y)z
			if(right.empty() || right.size() % 2 != 0)	return false;

			std::cout << "right size: " << right.size() << "\n";

			for(int i = 0; i < right.size(); ++i)
			{
				//even index must be operator
				if(i % 2 == 0)
				{
					std::cout << "even: " << right[i] << "\n";
					if(!(IsOperator(right[i])))
						return false;	
				}

				//odd index must be name/integer
				else
				{
					std::cout << "odd: " << right[i] << "\n";
					if(!(IsName(right[i]) || IsInteger(right[i])))
						return false;
				}
			}
		}

		//x+(y*z)
		else if(CBIndex_prev == exp.size() - 1 && OBIndex_prev != 0)
		{
			std::cout << "Checking from the left\n";

			std::vector<std::string> left(exp.begin() , exp.begin() + OBIndex_prev);

			//left side cannot empty or only 1 token
			if(left.empty() || left.size() % 2 != 0)	return false;

			for(int i = 0; i < left.size() ; ++i)
			{
				//even index must be operator
				if(i % 2 == 0)
				{
					if(!(IsName(left[i]) || IsInteger(left[i])))
						return false;	
				}

				//odd index must be name/integer
				else
				{
					if(!(IsOperator(left[i])))
						return false;
				}
			}
		}

		//x*(a+b)-y
		else
		{
			std::cout << "Checking from left and right\n";

			std::vector<std::string> left(exp.begin() , exp.begin() + OBIndex_prev);
			std::vector<std::string> right(exp.begin() + CBIndex_prev + 1 , exp.end());

			//left side cannot empty or only 1 token
			if((left.empty() && right.empty()) || ((left.size() % 2 != 0) && (right.size() % 2 != 0)))	return false;

			for(int i = 0; i < left.size(); ++i)
			{
				//even index must be name/integer
				if(i % 2 == 0)
				{
					if(!(IsName(left[i]) || IsInteger(left[i])))
						return false;	
				}

				//odd index must be operator
				else
				{
					if(!(IsOperator(left[i])))
						return false;
				}
			}

			for(int i = 0; i < right.size(); ++i)
			{
				//even index must be operator
				if(i % 2 == 0)
				{
					if(!(IsOperator(right[i])))
						return false;	
				}

				//odd index must be name/integer
				else
				{
					if(!(IsName(right[i]) || IsInteger(right[i])))
						return false;
				}
			}
		}
	}
	return true;
}

void QueryPreProcessor::TokenizeExpression(std::string exp, std::vector<std::string> &tokens, std::string &delim)
{
	std::cout << "In TokenizeExpression: "  << exp << "\n";
	std::size_t prev = 0, pos;
	while ((pos = exp.find_first_of(delim, prev)) != std::string::npos)
	{

		if (pos > prev) 
		{
			tokens.push_back(exp.substr(prev, pos-prev)); //push operand
			tokens.push_back(exp.substr(pos,1));	//push operator
		}

		else
		{
			tokens.push_back(exp.substr(pos,1));	//push operator
		}
		prev = pos+1;
	}

	if (prev < exp.length())
		tokens.push_back(exp.substr(prev, std::string::npos));
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

bool QueryPreProcessor::IsOperator(std::string s)
{
	if(s == "+" || s == "-" || s == "*")
		return true;

	return false;
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
	else if(type == "Affects*")		enumType = AFFECTST;
	else return false;

	return true;
}

AttrNameType QueryPreProcessor::GetEnumAttrNameType(std::string type) 
{
	if(type == "stmt#")				return STMTNUM;
	else if(type == "value")		return VALUE;
	else if(type == "procName")		return PROCNAME;
	else if(type == "varName")		return VARNAME;
	else							return INVALID_ATTRNAME_TYPE;
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
					//"x"
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

//output general debugging message useful
void QueryPreProcessor::DebugMessage(std::string msg)
{
	#ifdef DEBUG_MSG
		std::cout << "Invalid query: ";
		std::cout << msg;
	#endif
}

//output debugging messsage for a specific function for testing purpose
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
	
	else if(function == ISVALIDATTRREF)
	{
		#ifdef DEBUG_MSG_ISVALIDATTRREF
			std::cout << msg;
		#endif
	}

	else if(function == ISVALIDATTRREF)
	{
		#ifdef DEBUG_MSG_ISVALIDATTRREF
			std::cout << msg;
		#endif
	}

	else if(function == VALIDATERELATIONSHIP)
	{
		#ifdef DEBUG_MSG_VALIDATERELATIONSHIP
			std::cout << msg;
		#endif
	}

	else if(function == VALIDATEPATTERN)
	{
		#ifdef DEBUG_MSG_VALIDATEPATTERN
			std::cout << msg;
		#endif
	}

	else if(function == VALIDATEWITH)
	{
		#ifdef DEBUG_MSG_VALIDATEWITH
			std::cout << msg;
		#endif
	}

	
}