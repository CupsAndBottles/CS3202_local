#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

#include "ProcTable.h"
#include "..\Exception\IndexNotFoundException.h"

using namespace std;

vector<string>  ProcTable::procNames;
map<int, int> ProcTable::firstStmtNosList;
map<int, int> ProcTable::lastStmtNosList;

// empty constructor
ProcTable::ProcTable() {}

// API
int ProcTable::InsertProc(string procName) {
	vector<string>::iterator searchResult = SearchFor(procName);

	if (searchResult != procNames.end()) {
		return distance(procNames.begin(), searchResult);

	} else {
		procNames.push_back(procName);
		return procNames.size() - 1;

	}
}

int ProcTable::GetIndexOfProc(string procName) {
	vector<string>::iterator searchResult = SearchFor(procName);

	if (searchResult != procNames.end()) {
		return distance(procNames.begin(), searchResult);

	} else {
		return -1;

	}

}

string ProcTable::GetProcName(int procIndex) {
	if (procIndex >= 0 && procIndex < (int) procNames.size()) {
		return procNames[procIndex];
	} else {
		throw IndexNotFoundException();
	}

}

vector<string> ProcTable::GetAllProcNames() {
	return procNames;
}

void ProcTable::SetFirstStmtNoOfProc(int procIndex, int firstStmtNo) {
	if (procIndex >= 0 && procIndex < (int) procNames.size()) {
		// assumes that stmt#s are inserted in order
		if (GetProcOfStmt(firstStmtNo) == -1) {
			firstStmtNosList[procIndex] = firstStmtNo;
		} else {
			cout << "\nConflicting proc indexes during insertion!!\n";
			cout << firstStmtNo << " already exists in previous process";

			return;	// don't make any changes
		}
	} else {
		throw IndexNotFoundException();
	}
}

void ProcTable::SetLastStmtNoOfProc(int procIndex, int lastStmtNo) {
	if (procIndex >= 0 && procIndex < (int) procNames.size()) {
		// check that firstStmt has been set and lastStmt has NOT been set
		if (firstStmtNosList.count(procIndex) != 0 && lastStmtNosList.count(procIndex) == 0) {
			if (lastStmtNo > firstStmtNosList.at(procIndex))
				lastStmtNosList[procIndex] = lastStmtNo;
		}
	} else {
		throw IndexNotFoundException();
	}
}

int ProcTable::GetFirstStmtNoOfProc(int procIndex) {
	if (firstStmtNosList.count(procIndex) != 0)
		return firstStmtNosList.at(procIndex);
	else if (procIndex < 0 || procIndex >= (int) procNames.size())
		throw IndexNotFoundException();
	else
		return -1;	// index not set yet
}

int ProcTable::GetLastStmtNoOfProc(int procIndex) {
	if (firstStmtNosList.count(procIndex) != 0)
		return lastStmtNosList.at(procIndex);
	else if (procIndex < 0 || procIndex >= (int) procNames.size())
		throw IndexNotFoundException();
	else
		return -1;	// index not set yet
}

int ProcTable::GetProcOfStmt(int stmtNo) {
	for(map<int, int>::iterator it = lastStmtNosList.begin(); it != lastStmtNosList.end(); it++) {
		if (it->second >= stmtNo) {
			if (firstStmtNosList[it->first] <= stmtNo)
				return it->first;

		}
	}

	return -1; // proc not found

}

int ProcTable::GetNoOfProcs() {
	return procNames.size();
}

// method(s) to aid testing
void ProcTable::ClearData() {
	procNames.clear();
	firstStmtNosList.clear();
	lastStmtNosList.clear();

}

/*Private Methods*/
vector<string>::iterator ProcTable::SearchFor(string procName) {
	return find(procNames.begin(), procNames.end(), procName);

}