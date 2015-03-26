#include <algorithm>
#include <utility>
#include <map>
#include <vector>

#include "Follows.h"
#include "StmtTypeTable.h"

map <int, int> Follows::beforeToAfterTable;
map <int, int> Follows::afterToBeforeTable;

/** public methods **/
Follows::Follows() {
	// empty constructor
}

// Methods
void Follows::SetFollows(int stmtBefore, int stmtAfter) {
	if (NoRelationshipConflicts(stmtBefore, stmtAfter)) {
		beforeToAfterTable[stmtBefore] = stmtAfter;
		afterToBeforeTable[stmtAfter] = stmtBefore;

	}

}

bool Follows::IsFollows(int stmtBefore, int stmtAfter) {
	if (beforeToAfterTable.count(stmtBefore) != 0) {
		return beforeToAfterTable[stmtBefore] == stmtAfter;
	}

	return false;

}

int Follows::GetFollowsBefore(int stmtAfter) {
	if(!HasNoStmtBefore(stmtAfter)) {
		return afterToBeforeTable[stmtAfter];

	} else {
		return -1;

	}

}

int Follows::GetFollowsAfter(int stmtBefore) {
	if(!HasNoStmtAfter(stmtBefore)) {
		return beforeToAfterTable[stmtBefore];

	} else {
		return -1;

	}

}

bool Follows::IsFollowsT(int stmtBefore, int stmtAfter) {
	int currBefore = stmtBefore;

	while (!HasNoStmtAfter(currBefore)) {
		if (beforeToAfterTable[currBefore] == stmtAfter) {
			return true;

		}

		currBefore = beforeToAfterTable[currBefore];
	
	}

	return false;

}

vector<int> Follows::GetFollowsTBefore(int stmtAfter) {
	vector<int> beforeList;
	int stmtBefore = GetFollowsBefore(stmtAfter);

	while (stmtBefore != -1) {
		beforeList.push_back(stmtBefore);

		stmtAfter = stmtBefore;
		stmtBefore = GetFollowsBefore(stmtAfter);
	
	}

	return beforeList;
}

vector<int> Follows::GetFollowsTAfter(int stmtBefore) {
	vector<int> afterList;

	int stmtAfter = GetFollowsAfter(stmtBefore);
	while (stmtAfter != -1) {
		afterList.push_back(stmtAfter);

		stmtBefore = stmtAfter;
		stmtAfter = GetFollowsAfter(stmtBefore);
	
	}


	return afterList;
}

bool Follows::HasAnyFollows() {
	return SizeOfFollows() > 0;
}

int Follows::SizeOfFollows() {
	return beforeToAfterTable.size();
}

void Follows::ClearData() {
	beforeToAfterTable.clear();
	afterToBeforeTable.clear();
}

/** private methods **/
bool Follows::NoRelationshipConflicts(int stmtBefore, int stmtAfter) {
	return HasNoStmtAfter(stmtBefore) && HasNoStmtBefore(stmtAfter);

}

bool Follows::HasNoStmtAfter(int stmtBefore) {
	return beforeToAfterTable.find(stmtBefore) == beforeToAfterTable.end();
}

bool Follows::HasNoStmtBefore(int stmtAfter) {
	return afterToBeforeTable.find(stmtAfter) == afterToBeforeTable.end();
}