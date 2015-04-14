#include <algorithm>
#include <utility>
#include <map>
#include <vector>
#include <iostream>

#include "Follows.h"
#include "StmtTypeTable.h"

map <int, int> Follows::beforeToAfterTable;
map <int, int> Follows::afterToBeforeTable;
vector<vector<bool>> Follows::beforeToAfterTBV;
vector<vector<int>> Follows::beforeToAfterTTable;
vector<vector<int>> Follows::afterToBeforeTTable;

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

bool Follows::IsFollowsTBV(int stmtBefore, int stmtAfter) {
	int totalNoOfStmts = StmtTypeTable::GetMaxStmtIndex();

	if (stmtBefore >= 0 && stmtBefore <= totalNoOfStmts
		&& stmtAfter >= 0 && stmtAfter <= totalNoOfStmts) {
			return beforeToAfterTBV.at(stmtBefore).at(stmtAfter);
	} else {
		return false;
	}

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

vector<int> Follows::GetStoredFollowsTBefore(int stmtAfter) {
	int maxStmtIndex = StmtTypeTable::GetMaxStmtIndex();

	if (stmtAfter >= 0 && stmtAfter <= maxStmtIndex) {
		if ((int) afterToBeforeTTable.size() >= stmtAfter) {
			return afterToBeforeTTable.at(stmtAfter);
		} else {
			cout << "\nwarning: unable to find " << stmtAfter << " in afterToBeforeTTable.\n";
			return GetFollowsTBefore(stmtAfter);
		}
	}

	vector<int> emptyList;
	return emptyList;
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

vector<int> Follows::GetStoredFollowsTAfter(int stmtBefore) {
	int maxStmtIndex = StmtTypeTable::GetMaxStmtIndex();

	if (stmtBefore >= 0 && stmtBefore <= maxStmtIndex) {
		if ((int) beforeToAfterTTable.size() >= stmtBefore) {
			return beforeToAfterTTable.at(stmtBefore);
		} else {
			cout << "\nwarning: unable to find " << stmtBefore << " in beforeToAfterTTable.\n";
			return GetFollowsTAfter(stmtBefore);
		}
	}

	vector<int> emptyList;
	return emptyList;
}

void Follows::CreateBeforeToAfterTBV() {
	int totalNoOfStmts = StmtTypeTable::GetMaxStmtIndex();
	vector<bool> emptyRow(totalNoOfStmts + 1, false);
	beforeToAfterTBV = vector<vector<bool>>(totalNoOfStmts + 1, emptyRow);

	vector<int> allStmts = StmtTypeTable::GetAllStmtsOfType(STMT);
	vector<int> afterT;
	for (vector<int>::iterator it1 = allStmts.begin(); it1 != allStmts.end(); it1++) {
		afterT = GetFollowsTAfter(*it1);

		for (vector<int>::iterator it2 = afterT.begin(); it2 != afterT.end(); it2++) {
			beforeToAfterTBV.at(*it1).at(*it2) = true;
		}
	}

}

void Follows::CreateBeforeToAfterTTable() {
	int maxStmtIndex = StmtTypeTable::GetMaxStmtIndex();
	vector<int> emptyRow;
	beforeToAfterTTable = vector<vector<int>>(maxStmtIndex + 1, emptyRow);

	vector<int> allStmts = StmtTypeTable::GetAllStmtsOfType(STMT);
	vector<int> afterT;
	for (vector<int>::iterator it = allStmts.begin(); it != allStmts.end(); it++) {
		afterT = GetFollowsTAfter(*it);
		beforeToAfterTTable.at(*it) = afterT;
	}

}

void Follows::CreateAfterToBeforeTTable() {
	int maxStmtIndex = StmtTypeTable::GetMaxStmtIndex();
	vector<int> emptyRow;
	afterToBeforeTTable = vector<vector<int>>(maxStmtIndex + 1, emptyRow);

	vector<int> allStmts = StmtTypeTable::GetAllStmtsOfType(STMT);
	vector<int> beforeT;
	for (vector<int>::iterator it = allStmts.begin(); it != allStmts.end(); it++) {
		beforeT = GetFollowsTBefore(*it);
		afterToBeforeTTable.at(*it) = beforeT;
	}

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
	beforeToAfterTBV.clear();
	beforeToAfterTTable.clear();
	afterToBeforeTTable.clear();
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