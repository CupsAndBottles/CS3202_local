#pragma once
#include <string>
#include <vector>
#include "Grammar.h"

/*
Class for storing query string data, accessed by QueryPreProcessor and QueryEvaluator
*/
class QueryData 
{
private:
	static std::vector<Declaration> declarations;
	static std::vector<SelectClause> selectClauses;	
	static std::vector<SuchThatClause> suchThatClauses;
	static std::vector<PatternClause> patternClauses;
	static std::vector<WithClause> withClauses;

	/*
	store clause sequence from left to right, string refer to clause type, int refer to index of that clause vector
	e.g.
	Select a such that Modifies(a,v) with a=a2 Pattern a(_,_) with a2=5 such that Parent(w,s) and Uses(w,v)
		
	suchThatClauses - Modifies(a,v) , Parent(w,s) , Uses(w,v)
	patternClauses - a(_,_)
	withClauses - a=a2 , a2=5

	clauseSequence
	0 - pair(SUCHTHAT	, 0)
	1 - pair(WITH		, 0)
	2 - pair(PATTERN	, 0)
	3 - pair(WITH		, 1)
	4 - pair(SUCHTHAT	, 1)
	5 - pair(SUCHTHAT	, 2)
	*/
	static std::vector<std::pair<ClauseType,int>> clauseSequence;

public:
	void InsertDeclaration(Synonym);
	void InsertSelect(Synonym);
	void InsertPattern(Synonym, Argument, Argument);
	void InsertSuchThat(RelationshipType, Argument, Argument);
	void InsertWith(Argument, Argument);

	std::vector<Declaration> GetDeclarations();
	std::vector<SelectClause> GetSelects();	
	std::vector<SuchThatClause> GetSuchThats();
	std::vector<PatternClause> GetPatterns();
	std::vector<WithClause> GetWiths();
	int GetSizeOfClause(ClauseType);

	static bool IsSynonymExist(std::string);
	static bool IsSynonymExist(std::string, SynonymType);
	static bool IsSynonymExist(std::string, SynonymType*);
	static bool IsSynonymExist(std::string, std::vector<SynonymType>);

	//void PushBackClause(ClauseType type , int index);
	std::vector<std::pair<ClauseType,int>> GetClauseSequence();
	void ClearData();
};