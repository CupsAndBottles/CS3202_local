#include "QueryData.h"
#include <iostream>
#include <utility>

std::vector<Declaration> QueryData::declarations;
std::vector<SelectClause> QueryData::selectClauses;	
std::vector<SuchThatClause> QueryData::suchThatClauses;
std::vector<PatternClause>QueryData::patternClauses;
std::vector<WithClause> QueryData::withClauses;
std::vector<std::pair<ClauseType,int>> QueryData::clauseSequence;


void QueryData::InsertDeclaration(Synonym synonym)
{
	declarations.push_back(Declaration(synonym));
}

void QueryData::InsertSelect(Synonym synonym)
{
	selectClauses.push_back(SelectClause(synonym));
}

void QueryData::InsertPattern(Synonym synonym, Argument arg1, Argument arg2)
{
	clauseSequence.push_back(std::make_pair(PATTERN , patternClauses.size()));
	patternClauses.push_back(PatternClause(synonym, arg1, arg2));
}

void QueryData::InsertSuchThat(RelationshipType rel, Argument arg1, Argument arg2)
{
	clauseSequence.push_back(std::make_pair(SUCHTHAT , suchThatClauses.size()));
	suchThatClauses.push_back(SuchThatClause(rel, arg1, arg2));
}

void QueryData::InsertWith(Argument arg1, Argument arg2)
{
	clauseSequence.push_back(std::make_pair(WITH , withClauses.size()));
	withClauses.push_back(WithClause(arg1, arg2));
}

std::vector<Declaration> QueryData::GetDeclarations()
{
	return declarations;
}

std::vector<SelectClause> QueryData::GetSelects()
{
	return selectClauses;
}

std::vector<PatternClause> QueryData::GetPatterns()
{
	return patternClauses;
}

std::vector<SuchThatClause> QueryData::GetSuchThats()
{
	return suchThatClauses;
}

std::vector<WithClause> QueryData::GetWiths()
{
	return withClauses;
}

void QueryData::ClearData()
{
	declarations.clear();
	selectClauses.clear();
	suchThatClauses.clear();
	patternClauses.clear();
}

/*
Check if a synonym "synonym" of type "type" is declared
Used when the type is known
*/
bool QueryData::IsSynonymExist(std::string synonym, SynonymType type)
{
	for(std::vector<Declaration>::iterator it = declarations.begin(); it != declarations.end(); ++it)
	{
		if((*it).synonym.value == synonym && (*it).synonym.type == type)
				return true;
	}

	return false;
}

/*
Check if a synonym "synonym" is declared, if it is, return the type to "type"
Used to get the type of a synonym
*/
bool QueryData::IsSynonymExist(std::string synonym, SynonymType *type)
{
	for(std::vector<Declaration>::iterator it = declarations.begin(); it != declarations.end(); ++it)
	{
		if((*it).synonym.value == synonym) {
			*type = (*it).synonym.type;
			return true;
		}
	}

	return false;
}

bool QueryData::IsSynonymExist(std::string synonym)
{
	for(std::vector<Declaration>::iterator it = declarations.begin(); it != declarations.end(); ++it)
	{
		//std::cout << "here " << (*it).synonym.value << "\n";

		if((*it).synonym.value == synonym) 
			return true;
	}

	return false;
}

/*
Check if a synonym "synonym" is declared as one of the type in "typeList"
*/
bool QueryData::IsSynonymExist(std::string synonym, std::vector<SynonymType> typeList)
{
	for(std::vector<Declaration>::iterator i = declarations.begin(); i != declarations.end(); ++i)
	{
		if((*i).synonym.value == synonym) {
			for(std::vector<SynonymType>::iterator j = typeList.begin(); j != typeList.end(); ++j)
			{
				if((*i).synonym.type == (*j))
					return true;
			}
		}
	}

	return false;
}
/*
void QueryData::PushBackClause(ClauseType type , int index)
{
	clauseSequence.push_back(std::make_pair(type,index));
}*/

std::vector<std::pair<ClauseType,int>> QueryData::GetClauseSequence()
{
	return clauseSequence;
}

int QueryData::GetSizeOfClause(ClauseType clauseType)
{
	switch(clauseType)
	{
	case DECLARATION:
		return declarations.size();

	case SELECT:
		return selectClauses.size();

	case SUCHTHAT:
		return suchThatClauses.size();

	case PATTERN:
		return patternClauses.size();

	case WITH:
		return withClauses.size();

	default:
		return 0;
	}
}