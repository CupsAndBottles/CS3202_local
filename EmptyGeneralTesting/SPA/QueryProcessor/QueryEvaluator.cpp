#include "QueryEvaluator.h"
#include "..\PKB\Parent.h"
#include "..\PKB\Follows.h"
#include "..\PKB\Modifies.h"
#include "..\PKB\Uses.h"
#include "..\PKB\StmtTypeTable.h"
#include "..\PKB\ConstTable.h"
#include "..\PKB\VarTable.h"
#include "..\QueryProcessor\QueryPreProcessor.h"
#include "..\QueryProcessor\QueryData.h"
#include <iostream>
#include <algorithm>
#include <iterator> 
#include <sstream>
#include "..\..\AutoTester\source\AbstractWrapper.h"

using namespace std;

QueryEvaluator::QueryEvaluator(void) {}

//modified version
bool QueryEvaluator::EvaluateQuery(QueryData queryData, list<string> &resultList)
{
	vector<Declaration> declarations = queryData.GetDeclarations();
	vector<SelectClause> selects = queryData.GetSelects();	//support tuple <>
	vector<PatternClause> patterns = queryData.GetPatterns();
	vector<SuchThatClause> suchThats = queryData.GetSuchThats();
	vector<WithClause> withs = queryData.GetWiths();
	//vector<IntermediateResult> interResultList;
	bool noAnswerFlag = false;
	bool selectAllFlag = false;

	//only select, no suchthat/pattern/with
	if(patterns.size() == 0 && suchThats.size() == 0 && withs.size() == 0)
		selectAllFlag = true;

	//Initialize result table with declared synonyms
	intermediateResult.Initialize(declarations);

	//if there is no such that/pattern/with, just skip
	if(!selectAllFlag)
	{
		//evaluate pattern in a loop
		for(int i = patterns.size()-1; i >= 0; i--)
		{
			//cout << "here";
			PatternClause pattern = patterns.at(i);

			//send in whole interResultList, let evaluatePattern do the work?
			if(!EvaluatePattern(pattern, interResultList)) {
				noAnswerFlag = true;
				break;
			}
		}

		//no match in pattern
		if(noAnswerFlag) {
			cout << "No answer in Pattern clause.\n";
			resultList.clear();
			return true;
		}

		if(AbstractWrapper::GlobalStop)	return false;


		//evaluate such that in a loop
		for(int i = suchThats.size()-1; i >= 0; i--)
		{
			SuchThatClause suchThat = suchThats.at(i);

			switch (suchThat.relationship)
			{
			case MODIFIES:
				//cout << "\nheree";
				noAnswerFlag = EvaluateModifies(suchThat, interResultList);
				//cout <<"\nnoanswerflag:" << noAnswerFlag;
				break;

			case USES:
				noAnswerFlag = EvaluateModifies(suchThat, interResultList);
				break;

			case PARENT:
				noAnswerFlag = EvaluateParent(suchThat, interResultList);
				break;

			case PARENTT:
				noAnswerFlag = EvaluateParent(suchThat, interResultList);
				break;

			case FOLLOWS:
				noAnswerFlag = EvaluateFollows(suchThat, interResultList);
				break;

			case FOLLOWST:
				noAnswerFlag = EvaluateFollows(suchThat, interResultList);
				break;

			case INVALID_RELATIONSHIP_TYPE:

			default:
				cout << "No matching relationship in Such That clause.\n";
				resultList.clear();
				return true;
			}

			//no match in pattern
			//the flag is reverse here!
			if(!noAnswerFlag) {
				cout << "No answer in Such That clause.\n";
				resultList.clear();
				return true;
			}

			if(AbstractWrapper::GlobalStop)	return false;
		}
	}

	//choose select
	for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) 
	{
		noAnswerFlag = true;

		if((*it).synonym.value == select.synonym.value) 
		{
			//cout << "\nsynonym value: " << (*it).resultInt.size();
			if((*it).synonym.type != VARIABLE/* && (*it).synonym.type != PROCEDURE*/) 
			{
				vector<int> result;

				if(!(*it).resultInt.empty()) 
				{	
					//cout << "here1";
					//cout << "\nsize: " << (*it).resultInt.size();
					//CONVERT TO STRING
					result = (*it).resultInt;			
				}

				else		//query only have select w/o such that, pattern, with
				{		
					cout << "\nEmpty result.\n";
					SynonymType type = select.synonym.type;

					//if(selectAllFlag) {
						if((*it).synonym.type != CONSTANT)		//ASSIGN, WHILE, STMT, PROG_LINE
							result = StmtTypeTable::GetAllStmtsOfType(type);

						else	//CONSTANT
						{
							result = ConstTable::GetAllConst();

							set<int> s( result.begin(), result.end() );
							result.assign( s.begin(), s.end() );
						}
					//}

					/*else {
						cout << "\nWeird?";


						return true;	//select a1 pattern a2(_,_)
					}*/
				}

				for(vector<int>::iterator it = result.begin(); it != result.end(); ++it) 
						resultList.push_back(ToString(*it));
			}

			else 
			{
				vector<string> result;

				if(!(*it).resultVar.empty()) 
					copy((*it).resultVar.begin(), (*it).resultVar.end(), back_inserter(resultList));

				else {
					result = VarTable::GetAllVarNames(); // need to extend to get procedure, GetAllProc

					copy(result.begin(), result.end(), back_inserter(resultList));
				}
			}

			noAnswerFlag = false;
			break;
		}
	}

	if(noAnswerFlag) {
		cout << "No answer, somehow Select don't exist in interResultList.\n";
		resultList.clear();
		return true;
	}

	resultList.sort();
	resultList.unique();
	
	cout<< "\nFinal result list: ";
	for(list<string>::iterator it = resultList.begin(); it != resultList.end(); ++it)
		cout << *it << " ";
	cout<< "\n";

	return true;
}
/*
//Evaluate Parent and Parent*
bool QueryEvaluator::EvaluateParent(SuchThatClause suchThat, vector<IntermediateResult> &interResultList)
{
	Argument arg1 = suchThat.arg1;
	Argument arg2 = suchThat.arg2;
	Synonym arg1Syn = suchThat.arg1.syn;
	Synonym arg2Syn = suchThat.arg2.syn;
	RelationshipType rel = suchThat.relationship;

	if(arg1.type == SYNONYM) 
	{
		if(arg2.type == SYNONYM)
		{
			if(arg1Syn.value == arg2Syn.value) {
				cout << "In EvaluateParent, both arg1 and arg2 has the same synonym\n";
				return false;
			}

			vector<int>* parent, *child;
			vector<int> tempResultParent, tempResultChild;
			vector<int> tempParent, tempChild;
			
			//get appropriate a/w/s/n
			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg1.value)
					parent = &((*it).resultInt);
			}

			if(parent->empty())	tempParent = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			else				tempParent = *parent;

			//cout << "\nstmts size: " << tempStmts.size();

			//get appropriate v
			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg2.value)
					child = &((*it).resultInt);
			}

			if(child->empty())	tempChild = StmtTypeTable::GetAllStmtsOfType(arg2.syn.type);
			else				tempChild = *child;

			//cout << "\nvars size: " << tempVars.size();
				
			for(vector<int>::iterator it_parent = tempParent.begin(); it_parent != tempParent.end(); ++it_parent) {
				for(vector<int>::iterator it_child = tempChild.begin(); it_child != tempChild.end(); ++it_child) {
					bool isParent = false;
		
					if(rel == PARENT)	isParent = Parent::IsParent(*it_parent, *it_child);
					else				isParent = Parent::IsParentT(*it_parent, *it_child);

					if(isParent) {
						tempResultParent.push_back(*it_parent);;
						tempResultChild.push_back(*it_child);;
					}
				}					
			}

			//remove duplicates
			set<int> s( tempResultParent.begin(), tempResultParent.end() );
			tempResultParent.assign( s.begin(), s.end() );

			set<int> t( tempResultChild.begin(), tempResultChild.end() );
			tempResultChild.assign( t.begin(), t.end() );

			cout << "\ntempResultStmts size: " << tempResultParent.size();
			cout << "\ntempResultVars size: " << tempResultChild.size();

			*parent = tempResultParent;
			*child = tempResultChild;		

			if(parent->empty() || child->empty())	return false;
			else									return true;
		}

		else if(arg2.type == INTEGER)
		{
			vector<int>* stmts;
			vector<int> tempStmts;
			vector<int> tempResult;

			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg1.value)
					stmts = &((*it).resultInt);
			}

			if(stmts->empty())	tempStmts = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			else				tempStmts = *stmts;

			for(vector<int>::iterator it = tempStmts.begin(); it != tempStmts.end(); ++it) 
			{
				int arg2Value = atoi(arg2.value.c_str());
				bool isParent = false;

				if(rel == PARENT)	isParent = Parent::IsParent(*it, arg2Value);
				else				isParent = Parent::IsParentT(*it, arg2Value);

				if(isParent)		tempResult.push_back(*it);
			}

			*stmts = tempResult;

			if(stmts->empty())	return false;
			else				return true;
		}

		else if(arg2.type == UNDERSCORE)
		{
			vector<int>* stmts;
			vector<int> tempStmts;
			vector<int> tempResult;

			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg1.value)
					stmts = &((*it).resultInt);
			}

			if(stmts->empty())	tempStmts = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			else				tempStmts = *stmts;

			for(vector<int>::iterator it = tempStmts.begin(); it != tempStmts.end(); ++it) {
				vector<int> children;
			
				if(rel == PARENT)	children = Parent::GetChildrenOf(*it);
				else				children = Parent::GetChildrenTOf(*it);
			
				if(!children.empty())
					tempResult.push_back(*it);
			}

			*stmts = tempResult;

			if(stmts->empty())	return false;
			else				return true;
		}

		else 
		{
			cout << "\nIn EvaluateParent, invalid Parent argument 2 type.\n";
			return false;
		}
	}

	else if(arg2.type == SYNONYM)
	{
		vector<int>* stmts;
		vector<int> tempStmts;
		vector<int> tempResult;

		for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
			if((*it).synonym.value == arg2.value)
				stmts = &((*it).resultInt);
		}

		if(stmts->empty())	tempStmts = StmtTypeTable::GetAllStmtsOfType(arg2.syn.type);
		else				tempStmts = *stmts;

		for(vector<int>::iterator it = tempStmts.begin(); it != tempStmts.end(); ++it) {
			if(arg1.type == INTEGER)
			{
				int arg1Value = atoi(arg1.value.c_str());
				bool isParent = false;

				if(rel == PARENT)	isParent = Parent::IsParent(arg1Value, *it);
				else				isParent = Parent::IsParentT(arg1Value, *it);

				if(isParent)		tempResult.push_back(*it);
			}

			else if(arg1.type == UNDERSCORE)
			{
				if(rel == PARENT) {
					int parent = Parent::GetParentOf(*it);
					if(parent != -1)
						tempResult.push_back(*it);
				}

				else {
					vector<int> parent = Parent::GetParentTOf(*it);
					if(!parent.empty())	
						tempResult.push_back(*it);
				}
			}

			else 
			{
				cout << "\nIn EvaluateParent, invalid Parent argument 1 type.\n";
				return false;
			}
		}

		*stmts = tempResult;

		if(stmts->empty())	return false;
		else				return true;
	}

	else if(arg1.type == UNDERSCORE && arg2.type == INTEGER)
	{
		int arg2Value = atoi(arg2.value.c_str());

		if(rel == PARENT) {
			if(Parent::GetParentOf(arg2Value) == -1)
				return false;

			else return true;
		}

		else {
			vector<int> parent = Parent::GetParentTOf(arg2Value);
			if(parent.empty())	return false;
			else return true;
		}
	}

	else if(arg1.type == INTEGER && arg2.type == UNDERSCORE)
	{
		int arg1Value = atoi(arg1.value.c_str());

		vector<int> children;
		if(rel == PARENT)	children = Parent::GetChildrenOf(arg1Value);
		else				children = Parent::GetChildrenTOf(arg1Value);

		if(children.empty()) return false;

		return true;
	}

	else if(arg1.type == INTEGER && arg2.type == INTEGER)
	{
		int arg1Value = atoi(arg1.value.c_str());
		int arg2Value = atoi(arg2.value.c_str());
		
		if(rel == PARENT)	return Parent::IsParent(arg1Value, arg2Value);
		else				return Parent::IsParentT(arg1Value, arg2Value);
	}

	else if(arg1.type == UNDERSCORE && arg2.type == UNDERSCORE)
	{
		return Parent::HasAnyParents();
	}

	else {
		cout << "In EvaluateParent, no matching arguments.\n";		
		return false;
	}
}
*/
/*
//Evaluate Follows and Follows*
bool QueryEvaluator::EvaluateFollows(SuchThatClause suchThat, vector<IntermediateResult> &interResultList)
{
	Argument arg1 = suchThat.arg1;
	Argument arg2 = suchThat.arg2;
	Synonym arg1Syn = suchThat.arg1.syn;
	Synonym arg2Syn = suchThat.arg2.syn;
	RelationshipType rel = suchThat.relationship;


	if(arg1.type == SYNONYM) 
	{
		if(arg2.type == SYNONYM)
		{
			if(arg1Syn.value == arg2Syn.value) {
				cout << "In EvaluateFollows, both arg1 and arg2 has the same synonym\n";
				return false;
			}

			vector<int>* parent, *child;
			vector<int> tempResultParent, tempResultChild;
			vector<int> tempParent, tempChild;
			
			//get appropriate a/w/s/n
			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg1.value)
					parent = &((*it).resultInt);
			}

			if(parent->empty())	tempParent = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			else				tempParent = *parent;

			//cout << "\nstmts size: " << tempStmts.size();

			//get appropriate v
			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg2.value)
					child = &((*it).resultInt);
			}

			if(child->empty())	tempChild = StmtTypeTable::GetAllStmtsOfType(arg2.syn.type);
			else				tempChild = *child;

			//cout << "\nvars size: " << tempVars.size();
				
			for(vector<int>::iterator it_parent = tempParent.begin(); it_parent != tempParent.end(); ++it_parent) {
				for(vector<int>::iterator it_child = tempChild.begin(); it_child != tempChild.end(); ++it_child) {
					bool isParent = false;
		
					if(rel == FOLLOWS)	isParent = Follows::IsFollows(*it_parent, *it_child);
					else				isParent = Follows::IsFollowsT(*it_parent, *it_child);

					if(isParent) {
						tempResultParent.push_back(*it_parent);;
						tempResultChild.push_back(*it_child);;
					}
				}					
			}

			//remove duplicates
			set<int> s( tempResultParent.begin(), tempResultParent.end() );
			tempResultParent.assign( s.begin(), s.end() );

			set<int> t( tempResultChild.begin(), tempResultChild.end() );
			tempResultChild.assign( t.begin(), t.end() );

			cout << "\ntempResultStmts size: " << tempResultParent.size();
			cout << "\ntempResultVars size: " << tempResultChild.size();

			*parent = tempResultParent;
			*child = tempResultChild;		

			if(parent->empty() || child->empty())	return false;
			else									return true;
		}

		else if(arg2.type == INTEGER)
		{
			vector<int>* stmts;
			vector<int> tempStmts;
			vector<int> tempResult;

			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg1.value)
					stmts = &((*it).resultInt);
			}

			if(stmts->empty())	tempStmts = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			else				tempStmts = *stmts;

			for(vector<int>::iterator it = tempStmts.begin(); it != tempStmts.end(); ++it) 
			{
				int arg2Value = atoi(arg2.value.c_str());
				bool isFollows = false;

				if(rel == FOLLOWS)	isFollows = Follows::IsFollows(*it, arg2Value);
				else				isFollows = Follows::IsFollowsT(*it, arg2Value);

				if(isFollows)		tempResult.push_back(*it);
			}

			*stmts = tempResult;

			if(stmts->empty())	return false;
			else				return true;
		}

		else if(arg2.type == UNDERSCORE)
		{
			vector<int>* stmts;
			vector<int> tempStmts;
			vector<int> tempResult;

			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg1.value)
					stmts = &((*it).resultInt);
			}

			if(stmts->empty())	tempStmts = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			else				tempStmts = *stmts;

			for(vector<int>::iterator it = tempStmts.begin(); it != tempStmts.end(); ++it) {
				if(rel == FOLLOWS) {
					int followsAfter = Follows::GetFollowsAfter(*it);
					if(followsAfter != -1)
						tempResult.push_back(*it);
				}

				else {
					vector<int> followsAfter = Follows::GetFollowsTAfter(*it);
					if(!followsAfter.empty())	
						tempResult.push_back(*it);
				}
			}

			*stmts = tempResult;

			if(stmts->empty())	return false;
			else				return true;
		}

		else 
		{
			cout << "\nIn EvaluateParent, invalid Parent argument 2 type.\n";
			return false;
		}
	}

	else if(arg2.type == SYNONYM)
	{
		vector<int>* stmts;
		vector<int> tempStmts;
		vector<int> tempResult;

		for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
			if((*it).synonym.value == arg2.value)
				stmts = &((*it).resultInt);
		}

		if(stmts->empty())	tempStmts = StmtTypeTable::GetAllStmtsOfType(arg2.syn.type);
		else				tempStmts = *stmts;

		for(vector<int>::iterator it = tempStmts.begin(); it != tempStmts.end(); ++it) {
			if(arg1.type == INTEGER)
			{
				int arg1Value = atoi(arg1.value.c_str());
				bool isFollows = false;

				if(rel == FOLLOWS)	isFollows = Follows::IsFollows(arg1Value, *it);
				else				isFollows = Follows::IsFollowsT(arg1Value, *it);

				if(isFollows)		tempResult.push_back(*it);
			}

			else if(arg1.type == UNDERSCORE)
			{
				if(rel == FOLLOWS) {
					int followsBefore = Follows::GetFollowsBefore(*it);
					if(followsBefore != -1)
						tempResult.push_back(*it);
				}

				else {
					vector<int> followsBefore = Follows::GetFollowsTBefore(*it);
					if(!followsBefore.empty())	
						tempResult.push_back(*it);
				}
			}

			else 
			{
				cout << "\nIn EvaluateParent, invalid Parent argument 1 type.\n";
				return false;
			}
		}

		*stmts = tempResult;

		if(stmts->empty())	return false;
		else				return true;
	}

	else if(arg1.type == UNDERSCORE && arg2.type == INTEGER)
	{
		int arg2Value = atoi(arg2.value.c_str());

		if(rel == FOLLOWS)	
		{
			int followsBefore = Follows::GetFollowsBefore(arg2Value);
			if(followsBefore == -1) return false;
		}
		else 
		{
			vector<int> followsBefore = Follows::GetFollowsTBefore(arg2Value);
			if(followsBefore.empty()) return false;
		}

		return true;
	}

	else if(arg1.type == INTEGER && arg2.type == UNDERSCORE)
	{
		int arg1Value = atoi(arg1.value.c_str());

		if(rel == FOLLOWS)	
		{
			int followsAfter = Follows::GetFollowsAfter(arg1Value);
			if(followsAfter == -1) return false;
		}
		else 
		{
			vector<int> followsAfter = Follows::GetFollowsTAfter(arg1Value);
			if(followsAfter.empty()) return false;
		}

		return true;
	}

	else if(arg1.type == INTEGER && arg2.type == INTEGER)
	{
		int arg1Value = atoi(arg1.value.c_str());
		int arg2Value = atoi(arg2.value.c_str());
		
		if(rel == FOLLOWS)	return Follows::IsFollows(arg1Value, arg2Value);
		else				return Follows::IsFollowsT(arg1Value, arg2Value);
	}

	else if(arg1.type == UNDERSCORE && arg2.type == UNDERSCORE)
	{
		return Follows::HasAnyFollows();
	}

	else {
		cout << "In EvaluateParent, no matching arguments.\n";		
		return false;
	}
}
*/
/*
//Evaluate Pattern
bool QueryEvaluator::EvaluatePattern(PatternClause pattern, vector<IntermediateResult> &interResultList)
{
	Synonym patternSyn = pattern.synonym;
	Argument arg1 = pattern.arg1;
	Argument arg2 = pattern.arg2;
	ArgumentType arg1Type = pattern.arg1.type;
	ArgumentType arg2Type = pattern.arg2.type;
	string arg1Value = pattern.arg1.value;
	string arg2Value = pattern.arg2.value;
	
	if(patternSyn.type == ASSIGN)
	{
		vector<int>* stmts;

		//pointer points to pattern synonym intermediate result, it is used by all cases below, e.g. the "a" in pattern a(_,_)
		for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
			if((*it).synonym.value == patternSyn.value)
				stmts = &((*it).resultInt);
		}

		if(arg2Type == UNDERSCORE)
		{
			vector<int> tempResult;

			//pattern a(_,_)
			if(arg1Type == UNDERSCORE)
			{
				//check if interResultList is empty, if yes then add new, if no then use the existing result		
				if(stmts->empty()) {
					tempResult = StmtTypeTable::GetAllStmtsOfType(ASSIGN);
					
					for(vector<int>::iterator it = tempResult.begin(); it != tempResult.end(); ++it)
						stmts->push_back(*it);

					cout << "\nsize: " << stmts->size();
				}

				if(stmts->empty())	return false;
				else				return true;
			}

			//pattern a(v,_)
			else if(arg1Type == SYNONYM)
			{
				//check if a is empty, if yes add all a, if not use interResult
				if(stmts->empty()) {
					tempResult = StmtTypeTable::GetAllStmtsOfType(ASSIGN);
					
					for(vector<int>::iterator it = tempResult.begin(); it != tempResult.end(); ++it)
						stmts->push_back(*it);

					//cout << "\nsize: " << stmts->size();
				}

				//check if v is empty, if yes foreach a get v, if no foreach a check if v is modified
				vector<string>* vars;

				for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
					if((*it).synonym.value == arg1.value)
						vars = &((*it).resultVar);
				}

				if(vars->empty())
				{	
					//foreach a, add corresponding v in interResult
					for(vector<int>::iterator it = stmts->begin(); it != stmts->end(); ++it) {
						vector<int> var = Modifies::GetVarModifiedByStmt(*it);		//GetVarModifiedByStmt should return a single int

						if(!var.empty()) 
							vars->push_back(VarTable::GetVarName(var.at(0)));
					}
				}

				else
				{
					vector<string> tempVarResult;
					//foreach a, check if v in interResult is modified by a
					for(vector<int>::iterator it_stmts = stmts->begin(); it_stmts != stmts->end(); ++it_stmts) {
						for(vector<string>::iterator it_vars = vars->begin(); it_vars != vars->end(); ++it_vars) {
							vector<int> var = Modifies::GetVarModifiedByStmt(*it_stmts);

							if(!var.empty()) {
								string curVar = VarTable::GetVarName(var.at(0));
								if(curVar == *it_vars)
									tempVarResult.push_back(curVar);
							}
						}
					}

					//replace v interResult with tempVarResult
					*vars = tempVarResult;
					
				}

				//check if both a and v is empty, if yes return false, otherwise true
				if(stmts->empty() || vars->empty())	return false;
				else return true;
			}
			  
			else if(arg1Type == IDENT)
			{
				string ident = arg1Value;
				ident.erase(remove_if(ident.begin(), ident.end(), [](char x){return isspace(x);}), ident.end());
				ident = ident.substr(1, ident.length()-2);

				int varIndex = VarTable::GetIndexOfVar(ident);
				
				if(varIndex != -1)
				{
					//if a is empty, add all a that modify the ident
					if(stmts->empty()) {
						tempResult = StmtTypeTable::GetAllStmtsOfType(ASSIGN);
					
						for(vector<int>::iterator it = tempResult.begin(); it != tempResult.end(); ++it) {
							if(Modifies::IsStmtModifyingVar((*it), varIndex))
								stmts->push_back(*it);
						}

						//cout << "\nsize: " << stmts->size();
					}

					//if a is not empty, foreach a check if it modiy the ident, if yes replace existing interResult
					else {
						vector<int> tempStmtResult;

						for(vector<int>::iterator it = stmts->begin(); it != stmts->end(); ++it) {
							if(Modifies::IsStmtModifyingVar((*it), varIndex))
								tempStmtResult.push_back(*it);
						}

						*stmts = tempStmtResult;					
					}

					if(stmts->empty())	return false;
					else				return true;
				}

				else
				{
					cout << "\nIn EvaluatePattern, pattern argument 1 IDENT not found.\n";
					return false;
				}
			}

			else 
			{
				cout << "\nIn EvaluatePattern, invalid pattern argument 1 type.\n";
				return false;
			}
		}

		else if(arg2Type == EXPRESSION)
		{
			vector<int> tempResult;

			Pattern patternObj = CreatePatternObject(arg2Value);
			if(patternObj.expr == "")	return false;
			cout << "here";
			vector<int> rightResult = PatternMatcher::MatchPatternFromRoot(patternObj,true);
			if(rightResult.empty())		return false;
			cout << "here";
			//vector<string> rightResult;
			//for(vector<int>::iterator it = rightResultInt.begin(); it != rightResultInt.end(); ++it)
				//rightResult.push_back(*it);
			
			if(arg1Type == UNDERSCORE)
			{
				//check if a is empty, if yes, add. if no, foreach a in interResult, check if it is in the rightResult
				if(stmts->empty()) {
					
					*stmts = rightResult;
					//cout << "\nsize: " << stmts->size();
				}

				else {
					vector<int> tempStmtResult;

					for(vector<int>::iterator it_cur = rightResult.begin(); it_cur != rightResult.end(); ++it_cur) {		//a function to get intersection between two vectors?
						for(vector<int>::iterator it_prev = stmts->begin(); it_prev != stmts->end(); ++it_prev) {
							if((*it_cur) == (*it_prev))
								tempStmtResult.push_back(*it_cur);
						}
					}

					*stmts = tempStmtResult;					
				}

				cout << "\nstmts size: " << stmts->size();
				cout << "\nstmts: ";
				for(vector<int>::iterator it = stmts->begin(); it != stmts->end(); ++it) 
					cout << *it << " ";
				cout << endl;

				if(stmts->empty())	return false;
				else				return true;
			}

			else if(arg1Type == SYNONYM)
			{
				cout << "here";
				//if a is empty, add rightresult
				if(stmts->empty()) {
					
					*stmts = rightResult;
					cout << "\nsize: " << stmts->size();
				}

				//if a is not empty, find intersection of a and rightresult
				else {
					vector<int> tempStmtResult;

					for(vector<int>::iterator it_cur = rightResult.begin(); it_cur != rightResult.end(); ++it_cur) {		//a function to get intersection between two vectors?
						for(vector<int>::iterator it_prev = stmts->begin(); it_prev != stmts->end(); ++it_prev) {
							if((*it_cur) == (*it_prev))
								tempStmtResult.push_back(*it_cur);
						}
					}

					*stmts = tempStmtResult;	
				}

				vector<string>* vars;

				for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
					if((*it).synonym.value == arg1.value)
						vars = &((*it).resultVar);
				}
								
				if(vars->empty())
				{	
					//foreach a, add corresponding v in interResult
					for(vector<int>::iterator it = stmts->begin(); it != stmts->end(); ++it) {
						vector<int> var = Modifies::GetVarModifiedByStmt(*it);		//GetVarModifiedByStmt should return a single int

						if(!var.empty()) 
							vars->push_back(VarTable::GetVarName(var.at(0)));
					}
				}

				else
				{
					vector<string> tempVarResult;
					//foreach a, check if v in interResult is modified by a
					for(vector<int>::iterator it_stmts = stmts->begin(); it_stmts != stmts->end(); ++it_stmts) {
						for(vector<string>::iterator it_vars = vars->begin(); it_vars != vars->end(); ++it_vars) {
							if(Modifies::IsStmtModifyingVar((*it_stmts),VarTable::GetIndexOfVar((*it_vars))))
								tempVarResult.push_back(*it_vars);
						}
					}

					//replace v interResult with tempVarResult
					*vars = tempVarResult;
				}

				//check if both a and v is empty, if yes return false, otherwise true
				if(stmts->empty() || vars->empty())	return false;
				else return true;
			}

			else if(arg1Type == IDENT)
			{
				vector<int> leftResult;

				string ident = arg1Value;
				ident.erase(remove_if(ident.begin(), ident.end(), [](char x){return isspace(x);}), ident.end());
				ident = ident.substr(1, ident.length()-2);

				int varIndex = VarTable::GetIndexOfVar(ident);
				
				if(varIndex != -1)
				{
					vector<int> temp = Modifies::GetStmtModifyingVar(varIndex);

					if(!temp.empty())
					{
						for(vector<int>::iterator it = temp.begin(); it != temp.end(); ++it)
						{
							if(StmtTypeTable::CheckIfStmtOfType(*it,ASSIGN))
								leftResult.push_back(*it);
						}
					}

					else return false;

					if(leftResult.empty())	return false;

					set_intersection(leftResult.begin(), leftResult.end(), rightResult.begin(), rightResult.end(), back_inserter(tempResult));

					//if a is empty, add all a that modify the ident
					if(stmts->empty()) {
						*stmts = tempResult;
						//cout << "\nsize: " << stmts->size();
					}

					//if a is not empty, foreach a check if it modiy the ident, if yes replace existing interResult
					else {
						vector<int> tempStmtResult;

						for(vector<int>::iterator it_cur = tempResult.begin(); it_cur != tempResult.end(); ++it_cur) {		//a function to get intersection between two vectors?
							for(vector<int>::iterator it_prev = stmts->begin(); it_prev != stmts->end(); ++it_prev) {
								if((*it_cur) == (*it_prev))
									tempStmtResult.push_back(*it_cur);
							}
						}

						*stmts = tempStmtResult;					
					}

					if(stmts->empty())	return false;
					else				return true;
				}

				else
				{
					cout << "\nIn EvaluatePattern, pattern argument 1 IDENT not found.\n";
					return false;
				}
			}

			else 
			{
				cout << "\nIn EvaluatePattern, invalid pattern argument 1 type.\n";
				return false;
			}
		}

		else
		{
			cout << "\nIn EvaluatePattern, invalid pattern argument 2 type.\n";
			return false;
		}
	}

	else 
	{
		cout << "\nIn EvaluatePattern, invalid pattern synonym type.\n";
		return false; //only pattern a() in assignment 4
	}

	return true;
}
*/
/*
//Evaluate Modifies and Uses
bool QueryEvaluator::EvaluateModifies(SuchThatClause suchThat, vector<IntermediateResult> &interResultList)
{
	Argument arg1 = suchThat.arg1;
	Argument arg2 = suchThat.arg2;
	Synonym arg1Syn = suchThat.arg1.syn;
	Synonym arg2Syn = suchThat.arg2.syn;
	RelationshipType rel = suchThat.relationship;


	if(arg1.type == SYNONYM)
	{
		cout << "\nhere1";
		if(arg2.type == SYNONYM)
		{
			cout << "\nhere2";
			//check if a is empty, if yes, get all awsn. check if v is empty, if yes, foreach a add v modified by a. if no, for each a, foreach v,check if a modify v
			//if a is not empty, use current a. check if v is empty, if yes, foreach a add v modified by a. if no, for each a, foreach v,check if a modify v
			vector<int>* stmts;
			vector<string>* vars;
			vector<int> tempResultStmts;
			vector<string> tempResultVars;
			vector<int> tempStmts;
			vector<string> tempVars;

			//get appropriate a/w/s/n
			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg1.value)
					stmts = &((*it).resultInt);
			}

			if(stmts->empty())	tempStmts = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			else				tempStmts = *stmts;

			//cout << "\nstmts size: " << tempStmts.size();

			//get appropriate v
			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg2.value)
					vars = &((*it).resultVar);
			}

			if(vars->empty())	tempVars = VarTable::GetAllVarNames();
			else				tempVars = *vars;

			//cout << "\nvars size: " << tempVars.size();
				
			for(vector<int>::iterator it_stmts = tempStmts.begin(); it_stmts != tempStmts.end(); ++it_stmts) {
				for(vector<string>::iterator it_vars = tempVars.begin(); it_vars != tempVars.end(); ++it_vars) {
					int varIndex = VarTable::GetIndexOfVar(*it_vars);

					bool doesModifiesOrUses = false;
		
					if(rel == MODIFIES)		doesModifiesOrUses = Modifies::IsStmtModifyingVar(*it_stmts,varIndex);
					else					doesModifiesOrUses = Uses::IsStmtUsingVar(*it_stmts,varIndex);

					if(doesModifiesOrUses) {
						tempResultStmts.push_back(*it_stmts);
						tempResultVars.push_back(*it_vars);
					}
				}
			}

			//remove duplicates
			set<int> s( tempResultStmts.begin(), tempResultStmts.end() );
			tempResultStmts.assign( s.begin(), s.end() );

			set<string> t( tempResultVars.begin(), tempResultVars.end() );
			tempResultVars.assign( t.begin(), t.end() );

			cout << "\ntempResultStmts size: " << tempResultStmts.size();
			cout << "\ntempResultVars size: " << tempResultVars.size();

			*stmts = tempResultStmts;
			*vars = tempResultVars;		

			if(stmts->empty() || vars->empty())	return false;
			else								return true;
		}

		if(arg2.type == IDENT)
		{
			string ident = arg2.value;
			ident.erase(std::remove_if(ident.begin(), ident.end(), [](char x){return isspace(x);}), ident.end());
			ident = ident.substr(1, ident.length()-2);

			int varIndex = VarTable::GetIndexOfVar(ident);
			
			if(varIndex == -1) return false;


			vector<int>* stmts;
			vector<int> tempResult;

			//get pointer to interResultList synonym
			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg1.value)
					stmts = &((*it).resultInt);
			}

			vector<int> tempStmts;

			if(stmts->empty())	tempStmts = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			else				tempStmts = *stmts;

			for(vector<int>::iterator it = tempStmts.begin(); it != tempStmts.end(); ++it) 
			{
				bool doesModifiesOrUses = false;

				if(rel == MODIFIES)	doesModifiesOrUses = Modifies::IsStmtModifyingVar(*it, varIndex);
				else				doesModifiesOrUses = Uses::IsStmtUsingVar(*it, varIndex);

				if(doesModifiesOrUses)	tempResult.push_back(*it);
			}

			*stmts = tempResult;

			if(stmts->empty())	return false;
			else				return true;
		}

		if(arg2.type == UNDERSCORE)
		{
			vector<int>* stmts;
			vector<int> tempResult;

			//get pointer to interResultList synonym
			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg1.value)
					stmts = &((*it).resultInt);
			}

			vector<int> tempStmts;

			if(stmts->empty())	tempStmts = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			else				tempStmts = *stmts;
			
			for(vector<int>::iterator it = tempStmts.begin(); it != tempStmts.end(); ++it) {
				vector<int> var;
			
				if(rel == MODIFIES)	var = Modifies::GetVarModifiedByStmt(*it);
				else				var = Uses::GetVarUsedByStmt(*it);
				
				//for extension to procedure
				if(rel == MODIFIES) {
					if(arg1.syn.type == PROCEDURE) {}
					else {
						var = Modifies::GetVarModifiedByStmt(*it);
						var = Uses::GetVarUsedByStmt(*it);
					}
				}
				else {
					if(arg1.syn.type == PROCEDURE) {}
					else {
						var = Modifies::GetVarModifiedByStmt(*it);
						var = Uses::GetVarUsedByStmt(*it);
					}
				}

				if(!var.empty())
					tempResult.push_back(*it);
			}

			*stmts = tempResult;

			if(stmts->empty())	return false;
			else				return true;
		}

		else 
		{
			cout << "\nIn EvaluateModifies, invalid Modifies argument 2 type.\n";
			return false;
		}
	}

	else if(arg1.type == INTEGER)
	{
		if(arg2.type == SYNONYM)
		{
			//check if v is empty, if yes, get all v modified by arg1 integer
			//if v is not empty, for each v, check if it is modified by arg 1 integer, if yes add it in a temp vector, then finally replace 
			vector<string>* vars;
			vector<string> tempResult;

			//get pointer to interResultList synonym
			for(vector<IntermediateResult>::iterator it = interResultList.begin(); it != interResultList.end(); ++it) {
				if((*it).synonym.value == arg2.value)
					vars = &((*it).resultVar);
			}

			int arg1Value = atoi(arg1.value.c_str());
			vector<string> tempVars;

			if(vars->empty())	tempVars = VarTable::GetAllVarNames();
			else				tempVars = *vars;
				
			for(vector<string>::iterator it = tempVars.begin(); it != tempVars.end(); ++it) {
				int varIndex = VarTable::GetIndexOfVar(*it);

				bool doesModifiesOrUses = false;
		
				if(rel == MODIFIES)		doesModifiesOrUses = Modifies::IsStmtModifyingVar(arg1Value,varIndex);
				else					doesModifiesOrUses = Uses::IsStmtUsingVar(arg1Value,varIndex);

				if(doesModifiesOrUses)	tempResult.push_back(*it);
			}

			*vars = tempResult;

			if(vars->empty())	return false;
			else				return true;
		}

		if(arg2.type == IDENT)
		{
			string ident = arg2.value;
			ident.erase(std::remove_if(ident.begin(), ident.end(), [](char x){return isspace(x);}), ident.end());
	
			//eliminates " " and get the content
			ident = ident.substr(1, ident.length()-2);

			//get index of ident
			int varIndex = VarTable::GetIndexOfVar(ident);

			int arg1Value = atoi(arg1.value.c_str());
			bool doesModifiesOrUses = false;

			if(rel == MODIFIES)	doesModifiesOrUses = Modifies::IsStmtModifyingVar(arg1Value, varIndex);
			else				doesModifiesOrUses = Uses::IsStmtUsingVar(arg1Value, varIndex);

			if(doesModifiesOrUses)	return true;
		
			else return true;
		}

		if(arg2.type == UNDERSCORE)
		{
			int arg1Value = atoi(arg1.value.c_str());

			vector<int> var;
			if(rel == MODIFIES)	var = Modifies::GetVarModifiedByStmt(arg1Value);
			else				var = Uses::GetVarUsedByStmt(arg1Value);

			if(var.empty()) return false;

			return true;
		}

		else 
		{
			cout << "\nIn EvaluateModifies, invalid Modifies argument 2 type.\n";
			return false;
		}
	}
	
	else if(arg1.type == IDENT) //for procedure name, Modifies("procedure",v) etc
	{
		if(arg2.type == SYNONYM)
		{

		}

		if(arg2.type == IDENT)
		{

		}

		if(arg2.type == UNDERSCORE)
		{

		}

		else 
		{
			cout << "\nIn EvaluateModifies, invalid Modifies argument 2 type.\n";
			return false;
		}
	}

	else 
	{
		cout << "\nIn EvaluateModifies, invalid Modifies argument 1 type.\n";
		return false;
	}
}
*/
//Convert interger to string
string QueryEvaluator::ToString(int i)
{
	string s;
	stringstream out;
	out << i;
	s = out.str();

	return s;
}

//Construct Pattern struct
Pattern QueryEvaluator::CreatePatternObject(string expr)
{
	//remove white spaces and get expression content
	expr.erase(remove_if(expr.begin(), expr.end(), [](char x){return isspace(x);}), expr.end());
	int length = expr.length() - 4;
	expr = expr.substr(2, length);

	vector<string> tokenList;
	string delim = "+";

	QueryPreProcessor::Tokenize(expr, tokenList, delim);

	if(tokenList.size() == 1)
	{
		return Pattern(tokenList.at(0), NULL, NULL);
	}

	else if(tokenList.size() == 2)
	{
		Pattern *left = new Pattern(tokenList.at(0), NULL, NULL);
		Pattern *right = new Pattern(tokenList.at(1), NULL, NULL);
		Pattern p("+", left, right);
		return p;
	}

	else
	{
		cout << "\nIn CreatePatternObject, invalid expression\n";
		return Pattern("", NULL, NULL);
	}
}