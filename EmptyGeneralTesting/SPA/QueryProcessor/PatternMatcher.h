#pragma once
#include <string>
#include <vector>
#include "..\Program\TNode\TNode.h"

using namespace std;

struct Pattern {
	Pattern() : expr(""), leftPattern(nullptr), rightPattern(nullptr) {};

	Pattern(string e, Pattern* l, Pattern* r) :
		expr(e), leftPattern(l), rightPattern(r) {
	};

	string expr;
	Pattern* leftPattern;
	Pattern* rightPattern;

};

class PatternMatcher {
    public:
        PatternMatcher() { };

		static vector<int> MatchPatternFromRoot(Pattern object, bool partialMatch);
		static bool MatchPatternAtLeaves(TNode* node, Pattern object, bool partialMatch);
};
