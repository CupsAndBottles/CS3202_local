#include "QueryEvaluator.h"
#include "..\PKB\Parent.h"
#include "..\PKB\Follows.h"
#include "..\PKB\Modifies.h"
#include "..\PKB\Uses.h"
#include "..\PKB\StmtTypeTable.h"
#include "..\PKB\ConstTable.h"
#include "..\PKB\VarTable.h"
#include "..\PKB\ProcTable.h"
#include "..\QueryProcessor\QueryPreProcessor.h"
#include "..\QueryProcessor\QueryData.h"
#include <iostream>
#include <algorithm>
#include <iterator> 
#include <sstream>
#include "..\..\AutoTester\source\AbstractWrapper.h"

using namespace std;

QueryEvaluator::QueryEvaluator(void) {}

bool QueryEvaluator::EvaluateQuery(QueryData queryData, list<string> &resultList)
{
	vector<Declaration> declarations = queryData.GetDeclarations();
	vector<SelectClause> selects = queryData.GetSelects();
	vector<PatternClause> patterns = queryData.GetPatterns();
	vector<SuchThatClause> suchThats = queryData.GetSuchThats();
	vector<WithClause> withs = queryData.GetWiths();
	std::vector<std::pair<ClauseType,int>> clauseSequence = queryData.GetClauseSequence();
	bool hasAnswer = true;
	bool isSelectAll = false;

	//only select, no suchthat/pattern/with
	if(patterns.size() == 0 && suchThats.size() == 0 && withs.size() == 0)
		isSelectAll = true;

	//Initialize result table with declared synonyms
	intermediateResult.Initialize(declarations);

	//if there is no such that/pattern/with, just skip
	if(!isSelectAll)
	{
		//loop clause sequence
		for(std::vector<std::pair<ClauseType,int>>::iterator it = clauseSequence.begin(); it != clauseSequence.end(); ++it)
		{
			ClauseType clauseType = it->first;
			int clauseIndex = it->second;

			if(clauseType == SUCHTHAT)
			{
				SuchThatClause suchThat = suchThats.at(clauseIndex);

				switch (suchThat.relationship)
				{
					case MODIFIES:
					case USES:
						hasAnswer = EvaluateModifies(suchThat);
						break;

					case PARENT:
					case PARENTT:
						hasAnswer = EvaluateParent(suchThat);
						break;

					case FOLLOWS:
					case FOLLOWST:
						hasAnswer = EvaluateFollows(suchThat);
						break;

					case CALLS:
					case CALLST:
						hasAnswer = EvaluateCalls(suchThat);
						break;

					case NEXT:
					case NEXTT:
						hasAnswer = EvaluateNext(suchThat);
						break;

					case AFFECTS:
					case AFFECTST:
						hasAnswer = EvaluateAffects(suchThat);
						break;

					case CONTAINS:
					case CONTAINST:
						hasAnswer = EvaluateContains(suchThat);
						break;

					case SIBLING:
						hasAnswer = EvaluateSibling(suchThat);
						break;

					case AFFECTSBIP:
						hasAnswer = EvaluateAffectsBip(suchThat);
						break;

					case NEXTBIP:
					case NEXTBIPT:
						hasAnswer = EvaluateNextBip(suchThat);
						break;

					case INVALID_RELATIONSHIP_TYPE:
					default:
						cout << "No matching relationship in Such That clause.\n";
						resultList.clear();
						return true;
				}

				if(!hasAnswer) {
					cout << "No answer in Such That clause.\n";
					break;
					//return true;
				}
			}

			else if(clauseType == PATTERN)
			{	
				hasAnswer = EvaluatePattern(patterns[clauseIndex]);

				if(!hasAnswer) 
				{
					cout << "No answer in Pattern clause.\n";
					break;
				}
			}
			
			else if(clauseType == WITH)
			{
				hasAnswer = EvaluateWith(withs[clauseIndex]);
				
				if(!hasAnswer) 
				{
					cout << "No answer in With clause.\n";
					break;
				}
			}

			else 
			{
				cout << "In EvaluateQuery: invalid clause type.\n";
				return false;
			}

			if(AbstractWrapper::GlobalStop)	return false;
		}		
	}

	//select all
	else
	{
		//if BOOLEAN, no need waste time insert PKB data
		if(!selects.empty() && selects[0].synonym.type != BOOLEAN)
		{
			for(int i=0; i < selects.size(); ++i)
			{
				Synonym selectSyn = selects[i].synonym;

				if(selectSyn.type == ASSIGN || selectSyn.type == STMT || selectSyn.type == WHILE ||
					selectSyn.type == IF || selectSyn.type == CALL|| selectSyn.type == PROG_LINE)
				{
					//stmtTypeTable
					vector<int> result_int;
					vector<string> result;
					result_int = StmtTypeTable::GetAllStmtsOfType(selectSyn.type);

					//remove duplicate
					set<int> s( result_int.begin(), result_int.end() );
					result_int.assign( s.begin(), s.end() );

					intermediateResult.InsertList(selectSyn.value , result_int);
				}

				else if(selectSyn.type == CONSTANT)
				{
					//constTable
					vector<int> result_int;
					vector<string> result;
					result_int = ConstTable::GetAllConst();

					//remove duplicate
					set<int> s( result_int.begin(), result_int.end() );
					result_int.assign( s.begin(), s.end() );

					intermediateResult.InsertList(selectSyn.value , result_int);
				}

				else if(selectSyn.type == PROCEDURE)
				{
					//procTable
					vector<string> result = ProcTable::GetAllProcNames();
					intermediateResult.InsertList(selectSyn.value , result);
				}

				else if(selectSyn.type == VARIABLE)
				{
					//varTable
					vector<string> result = VarTable::GetAllVarNames(); 
					intermediateResult.InsertList(selectSyn.value , result);
				}

				else
				{
					cout << "In EvaluateQuery: Invalid select type.\n";
					resultList.clear();
					return false;
				}		
			}
		}
	}
	
	//select single
	if(selects.size() == 1)
	{
		Synonym selectSyn = selects[0].synonym;

		//BOOLEAN
		if(selectSyn.type == BOOLEAN)
		{	
			//select BOOLEAN
			if(isSelectAll)
			{
				resultList.push_back("true");
				return true;
			}

			//select BOOLEAN such that...
			else
			{
				if(hasAnswer)
				{
					resultList.push_back("true");
					return true;
				}

				else
				{
					resultList.push_back("false");
					return true;
				}			
			}
		}

		//All the other SynonymType
		else 
		{
			//select a
			//select a such that...
			if(hasAnswer)
			{
				intermediateResult.GetResultSingle(selectSyn.value ,resultList);
			}
			else
			{
				resultList.clear();
				return false;
			}
		}
	}

	//select <tuple>
	else if(selects.size() > 1)
	{
		vector<string> selectSynList;
		for(int i=0; i < selects.size(); ++i)
			selectSynList.push_back(selects[i].synonym.value);

		//select <a,w,v> , return all possible permutation of synonyms
		//select <a,w,v> such that...
		if(hasAnswer)
		{
			intermediateResult.GetResultTuple(selectSynList ,resultList);
		}

		else
		{
			resultList.clear();
			return false;
		}
	}

	else
	{
		cout << "In EvaluateQuery: Invalid select size.\n";
		resultList.clear();
		return false;
	}

	//resultList.sort();
	//resultList.unique();

	return true;
}


bool QueryEvaluator::EvaluateParent(SuchThatClause suchThat)
{
	Argument arg1 = suchThat.arg1;
	Argument arg2 = suchThat.arg2;
	Synonym arg1Syn = suchThat.arg1.syn;
	Synonym arg2Syn = suchThat.arg2.syn;
	RelationshipType rel = suchThat.relationship;
	
	cout << "Evaluating Parent( " << arg1.value << " , " << arg2.value << ")\n";

	if(arg1.type == SYNONYM) 
	{
		int validCount = 0;

		if(arg2.type == SYNONYM)
		{
			if(arg1Syn.value == arg2Syn.value) {
				cout << "In EvaluateParent, both arg1 and arg2 has the same synonym\n";
				return false;
			}

			vector<int> parent, child;
			bool valid = false;
			bool usingIntermediateResult_parent = false, usingIntermediateResult_child = false;

			//get appropriate stmt, while, if, prog_line
			if(intermediateResult.IsListEmpty(arg1.syn))
			{
				std::cout << "No intermediate result for " << arg1.syn.value << ", get all stmts\n";
				parent = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			}

			else 
			{
				std::cout << "Get " << arg1.syn.value << " from intermediate result table";
				intermediateResult.GetList(arg1.syn.value , parent);
				usingIntermediateResult_parent = true;
			}

			//get appropriate stmt, assign, while, if, prog_line, call
			if(intermediateResult.IsListEmpty(arg2.syn))
			{
				std::cout << "No intermediate result for " << arg2.syn.value << ", get all stmts\n";
				child = StmtTypeTable::GetAllStmtsOfType(arg2.syn.type);
			}

			else 
			{
				std::cout << "Get " << arg2.syn.value << " from intermediate result table";
				intermediateResult.GetList(arg2.syn.value , child);
				usingIntermediateResult_child = true;
			}

			//loop parent.size() * child.size() times, if all invalid, validCount will be 0, return false
			validCount = parent.size() * child.size();

			for(vector<int>::iterator it_parent = parent.begin(); it_parent != parent.end(); ++it_parent) 
			{
				for(vector<int>::iterator it_child = child.begin(); it_child != child.end(); ++it_child) 
				{
					bool isParent = false;
					//convert int to string to use with intermediateResult
					string parent_str = ITOS(*it_parent);
					string child_str = ITOS(*it_child);
		
					if(rel == PARENT)	isParent = Parent::IsParent(*it_parent, *it_child);
					else				isParent = Parent::IsParentT(*it_parent, *it_child);
				
					//both synonym list taken from result, so both must exist in result, question is whether there is a link between them
					if(usingIntermediateResult_parent && usingIntermediateResult_child)
					{
						if(isParent) {
							//check HasLink(), if yes, do nothing, else make pair
							bool isDirectLink;
							if(!intermediateResult.HasLinkBetweenColumns(arg1Syn.value, parent_str, arg2Syn.value, child_str , isDirectLink))
							{
								//if has link, do nothing, 
								//if no link && (1 and 2 both has no links), insert pair
								//if no link && (either 1 or 2 both has links), insert pair
								//if no link && (1 and 2 both have links)		
								//		if HasDirectLink(1 has link to synonym of 2 or vice versa), insert pair
								//		if HasIndirectLink(1 has no link to synonym of 2), do nothing

								//HasLink will return an bool hasDirectLink to state whether the link is direct or indirect
								//true = direct, false = indirect, only correct if HasLink is true


								//both have links
								if(intermediateResult.HasLink(arg1Syn.value, parent_str) && intermediateResult.HasLink(arg2Syn.value, child_str))
								{
									if(isDirectLink)
									{
										intermediateResult.InsertPair(arg1Syn.value, parent_str, arg2Syn.value, child_str);
									}

									else
									{
										//indirect link, do nothing
									}
								}

								//at least one no links
								else
								{
									intermediateResult.InsertPair(arg1Syn.value, parent_str, arg2Syn.value, child_str);
								}
							}
						}
						else 
						{
							//dont remove, if has link just remove the link, if no link do nothing, update table to remove excess
							bool dummy;
							if(intermediateResult.HasLinkBetweenColumns(arg1Syn.value, parent_str, arg2Syn.value, child_str, dummy))
								intermediateResult.Unlink(arg1Syn.value, parent_str, arg2Syn.value, child_str);

							//intermediateResult.RemovePair(arg1Syn.value, *it_parent, arg2Syn.value, *it_child);
							--validCount;
						}
					}

					//at least one of the synonym list is new, or both
					else
					{
						if(isParent) {
							//insert pair
							intermediateResult.InsertPair(arg1Syn.value, *it_parent, arg2Syn.value, *it_child);
						}
						else 
						{
							//do nothing
							--validCount;
						}
					}
				}
			}

			//after looping, do a update to remove any element without link between this 2 column
			if(usingIntermediateResult_parent && usingIntermediateResult_child)
			{
				intermediateResult.RemoveElementsWithoutLink(arg1Syn.value , arg2Syn.value);
			}
		}

		else if(arg2.type == INTEGER)
		{
			vector<int> stmts;
			bool valid = false;
			bool usingIntermediateResult = false;

			if(intermediateResult.IsListEmpty(arg1.syn))
			{
				stmts = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			}

			else 
			{
				intermediateResult.GetList(arg1.syn.value , stmts);
				usingIntermediateResult = true;
			}

			validCount = stmts.size();
			for(vector<int>::iterator it = stmts.begin(); it != stmts.end(); ++it) 
			{
				int arg2Value = atoi(arg2.value.c_str());
				bool isParent = false;

				if(rel == PARENT)	isParent = Parent::IsParent(*it, arg2Value);
				else				isParent = Parent::IsParentT(*it, arg2Value);

				if(usingIntermediateResult)
				{
					if(isParent) {}
					else 
					{
						intermediateResult.Remove(arg2Syn.value , *it);
						--validCount;
					}
				}
				else
				{
					if(isParent) intermediateResult.Insert(arg2Syn.value , *it);
					else --validCount;
				}
			}
		}

		else if(arg2.type == UNDERSCORE)
		{
			vector<int> stmts;
			bool valid = false;
			bool usingIntermediateResult = false;

			if(intermediateResult.IsListEmpty(arg1.syn))
			{
				stmts = StmtTypeTable::GetAllStmtsOfType(arg1.syn.type);
			}

			else 
			{
				intermediateResult.GetList(arg1.syn.value , stmts);
				usingIntermediateResult = true;
			}

			validCount = stmts.size();

			for(vector<int>::iterator it = stmts.begin(); it != stmts.end(); ++it) 
			{
				vector<int> children;
			
				if(rel == PARENT)	children = Parent::GetChildrenOf(*it);
				else				children = Parent::GetChildrenTOf(*it);
			
				if(usingIntermediateResult)
				{
					if(!children.empty()) {}
					else 
					{
						intermediateResult.Remove(arg2Syn.value , *it);
						--validCount;
					}
				}
				else
				{
					if(!children.empty()) intermediateResult.Insert(arg2Syn.value , *it);
					else --validCount;
				}
			}
		}

		else 
		{
			cout << "\nIn EvaluateParent, invalid Parent argument 2 type.\n";
			return false;
		}

		//all statements do not satisfy Parent()
		if(validCount == 0) 
		{
			cout << "Parent() is not satisfied.\n";			
			return false;
		}
		return true;
	}

	else if(arg2.type == SYNONYM)
	{
		vector<int> stmts;
		bool usingIntermediateResult = false;

		if(intermediateResult.IsListEmpty(arg2.syn))
		{
			stmts = StmtTypeTable::GetAllStmtsOfType(arg2.syn.type);
		}

		else 
		{
			intermediateResult.GetList(arg2.syn.value , stmts);
			usingIntermediateResult = true;
		}

		int validCount = stmts.size();

		for(vector<int>::iterator it = stmts.begin(); it != stmts.end(); ++it) 
		{
			if(arg1.type == INTEGER)
			{
				int arg1Value = STOI(arg1.value);
				bool isParent = false;

				if(rel == PARENT)	isParent = Parent::IsParent(arg1Value, *it);
				else				isParent = Parent::IsParentT(arg1Value, *it);

				//add another function HasLink(arg1, arg2) for intermediateresult, need?

				if(usingIntermediateResult)
				{
					//remove if invalid, do nothing if valid
					if(isParent) {}
					else
					{
						intermediateResult.Remove(arg2Syn.value , *it);
						--validCount;
					}
				}

				else
				{
					//insert if valid, do nothing if invalid
					if(isParent) intermediateResult.Insert(arg2Syn.value , *it);
					else --validCount;
				}
			}

			//Parent(_ , a)
			else if(arg1.type == UNDERSCORE)
			{
				if(rel == PARENT) {
					int parent = Parent::GetParentOf(*it);

					if(usingIntermediateResult)
					{
						//remove if invalid, do nothing if valid
						if(parent != -1) {}
						else 
						{
							intermediateResult.Remove(arg2Syn.value , *it);
							--validCount;
						}
					}
					else
					{
						//insert if valid, do nothing if invalid
						if(parent != -1) intermediateResult.Insert(arg2Syn.value , *it);
						else --validCount;
					}
				}

				else {
					vector<int> parent = Parent::GetParentTOf(*it);

					if(usingIntermediateResult)
					{
						if(!parent.empty())	 {}
						else 
						{
							intermediateResult.Remove(arg2Syn.value , *it);
							--validCount;
						}
					}
					else
					{
						if(!parent.empty())	 intermediateResult.Insert(arg2Syn.value , *it);
						else --validCount;
					}
				}
			}

			else 
			{
				cout << "\nIn EvaluateParent, invalid Parent argument 1 type.\n";
				return false;
			}
		}

		//all statements do not satisfy Parent()
		if(validCount == 0)
		{
			cout << "Parent() is not satisfied.\n";			
			return false;
		}
		return true;
	}

	else if(arg1.type == UNDERSCORE && arg2.type == INTEGER)
	{
		int arg2Value = STOI(arg2.value);

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
		int arg1Value = STOI(arg1.value);

		vector<int> children;
		if(rel == PARENT)	children = Parent::GetChildrenOf(arg1Value);
		else				children = Parent::GetChildrenTOf(arg1Value);

		if(children.empty()) return false;

		return true;
	}

	else if(arg1.type == INTEGER && arg2.type == INTEGER)
	{
		int arg1Value = STOI(arg1.value);
		int arg2Value = STOI(arg2.value);
		
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


bool QueryEvaluator::EvaluateFollows(SuchThatClause suchThat)
{
	return true;
	/*
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
	}*/
}


bool QueryEvaluator::EvaluateModifies(SuchThatClause suchThat)
{
	return true;
	/*
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
	*/
}


bool QueryEvaluator::EvaluateCalls(SuchThatClause)
{
	return true;
}


bool QueryEvaluator::EvaluateNext(SuchThatClause)
{
	return true;
}


bool QueryEvaluator::EvaluateAffects(SuchThatClause)
{
	return true;
}


bool QueryEvaluator::EvaluateContains(SuchThatClause)
{
	return true;
}


bool QueryEvaluator::EvaluateSibling(SuchThatClause)
{
	return true;
}


bool QueryEvaluator::EvaluateAffectsBip(SuchThatClause)
{
	return true;
}


bool QueryEvaluator::EvaluateNextBip(SuchThatClause)
{
	return true;
}


bool QueryEvaluator::EvaluatePattern(PatternClause pattern)
{
	return true;
/*
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
	*/
}


bool QueryEvaluator::EvaluateWith(WithClause with)
{

	return true;
}	




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


string QueryEvaluator::ITOS(int num)
{
	return to_string(long long(num));
}

vector<string> QueryEvaluator::ITOS(vector<int> intList)
{
	vector<string> strList;

	for(int i=0; i < intList.size(); ++i)
		strList.push_back(ITOS(intList[i]));
	
	return strList;
}

int QueryEvaluator::STOI(string s)
{
	int i;
    istringstream ss(s);
    ss >> i;
    
	return i;
}

vector<int> QueryEvaluator::STOI(vector<string> strList)
{
	vector<int> intList;

	for(int i=0; i < strList.size(); ++i)
		intList.push_back(STOI(strList[i]));

	return intList;
}
