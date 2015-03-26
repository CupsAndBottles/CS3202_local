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

	/*struct IntermediateResult {
		Synonym synonym;
		vector<int> resultInt;
		vector<string> resultVar;

		IntermediateResult(Synonym s) : synonym(s) , resultInt(), resultVar() {}
	};*/

	IntermediateResult intermediateResult;

	//bool EvaluateModifies(SuchThatClause, vector<IntermediateResult>&);
	//bool EvaluateParent(SuchThatClause, vector<IntermediateResult>&);
	//bool EvaluateFollows(SuchThatClause, vector<IntermediateResult>&);
	//bool EvaluatePattern(PatternClause, vector<IntermediateResult>&);

	string ToString(int);
	Pattern CreatePatternObject(string);

public:
	QueryEvaluator(void);
	bool EvaluateQuery(QueryData, list<string>&);
};

