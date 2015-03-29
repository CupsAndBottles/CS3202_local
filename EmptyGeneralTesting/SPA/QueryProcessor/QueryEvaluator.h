#pragma once
#include "QueryData.h"
#include "PatternMatcher.h"
#include "IntermediateResult.h"
#include <vector>
#include <list>

using namespace std;

class QueryEvaluator
{
	friend class QueryEvaluatorTest;

protected:

	IntermediateResult intermediateResult;

	bool EvaluateModifies(SuchThatClause);
	bool EvaluateParent(SuchThatClause);
	bool EvaluateFollows(SuchThatClause);
	bool EvaluatePattern(PatternClause);

	string ToString(int);
	Pattern CreatePatternObject(string);

public:
	QueryEvaluator(void);
	bool EvaluateQuery(QueryData, list<string>&);
};

