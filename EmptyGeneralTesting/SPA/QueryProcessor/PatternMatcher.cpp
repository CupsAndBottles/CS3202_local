#include <string>
#include <vector>
#include "PatternMatcher.h"
#include "..\Program\TNode\TNode.h"
#include "..\Program\Program.h"
#include "..\PKB\StmtTypeTable.h"
#include "Grammar.h"

#include <iostream>
#include <stdio.h>

using namespace std;

bool PatternMatcher::MatchPatternAtLeaves(TNode* node, Pattern object, bool partialMatch) {
	// match tree
	// if no need to match children, return match
	
	bool match = (node->GetContent() == object.expr);

	// check for children
	// if both empty, return match
	// if pattern has children but not tree, return false
	// if tree has children but not pattern. return partialMatch && match
	// if both have children, match left tree and right tree

	bool hasChildNodes = false;
	vector<TNode*> children;

	if (node->HasChildren()) {
		children = node->GetChildren();
		if (children.size() != 2) {
			hasChildNodes = false;
		} else {
			hasChildNodes = true;
		}
	}

	bool hasChildPatterns = (object.leftPattern != nullptr) && (object.rightPattern != nullptr);
	if (hasChildPatterns && hasChildNodes) {
		// match subtrees
		match = (match && MatchPatternAtLeaves(children[0], *object.leftPattern, false) && MatchPatternAtLeaves(children[1], *object.rightPattern, false));
		if (!match && partialMatch) {
			// recursive call
			bool matchLeft = MatchPatternAtLeaves(children[0], object, partialMatch);
			bool matchRight = MatchPatternAtLeaves(children[1], object, partialMatch);

			return (matchLeft || matchRight);
		} else {
			return match;
		}
	} else if (!hasChildPatterns && hasChildNodes) {
		if (match && partialMatch) {
			return true;
		} else if (!match && partialMatch) {
			// recursive call
			bool matchLeft = MatchPatternAtLeaves(children[0], object, partialMatch);
			bool matchRight = MatchPatternAtLeaves(children[1], object, partialMatch);

			return (matchLeft || matchRight);
		} else {
			return false;
		}
	} else if (hasChildPatterns && !hasChildNodes) {
		return false;
	} else if (!hasChildPatterns && !hasChildNodes) {
		return match;
	} else {
		return false;
	}
}

vector<int> PatternMatcher::MatchPatternFromRoot(Pattern object, bool partialMatch) {
	// printf("1 %s 2 %s 3 %s\n", object.expr.c_str(), object.leftPattern->expr.c_str(), object.rightPattern->expr.c_str());

	vector<int> assignmentStmts = StmtTypeTable::GetAllStmtsOfType(ASSIGN);
	vector<int> results;

	for (unsigned int currentStmt = 0; currentStmt < assignmentStmts.size(); currentStmt++) {
		TNode& currentStmtTNode = Program::GetStmtFromNumber(assignmentStmts[currentStmt]);
		// go to RHS (ignore left of assignment statement. Match expression)
		if (MatchPatternAtLeaves(&currentStmtTNode.GetChild(1), object, partialMatch)) {
			results.push_back(assignmentStmts[currentStmt]);
		}
	}

	return results;
}
