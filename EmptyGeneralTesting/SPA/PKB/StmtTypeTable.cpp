#include <utility>
#include <map>
#include <vector>
#include <iostream>

#include "..\QueryProcessor\Grammar.h"
#include "StmtTypeTable.h"

using namespace std;

map<int, SynonymType> StmtTypeTable::indexTypeTable;
map<SynonymType, vector<int> > StmtTypeTable::typeIndexTable;

vector<int> StmtTypeTable::allAssignStmts;
vector<int> StmtTypeTable::allWhileStmts;
vector<int> StmtTypeTable::allCallStmts;
vector<int> StmtTypeTable::allIfThenStmts;

int StmtTypeTable::maxStmtIndex;

// constructor
StmtTypeTable::StmtTypeTable() {
	maxStmtIndex = 0;
};

vector<int> StmtTypeTable::GetAllStmtsOfType(SynonymType type) {
	if(type == STMT || type == PROG_LINE) { 
		vector<int> stmts;
		
		stmts.insert(stmts.end(), allAssignStmts.begin(), allAssignStmts.end());
		stmts.insert(stmts.end(), allWhileStmts.begin(), allWhileStmts.end());
		stmts.insert(stmts.end(), allCallStmts.begin(), allCallStmts.end());
		stmts.insert(stmts.end(), allIfThenStmts.begin(), allIfThenStmts.end());
		
		return stmts;

	}

    else {
		switch (type) {
			case ASSIGN:
				return allAssignStmts;
			case WHILE:
				return allWhileStmts;
			case CALL:
				return allCallStmts;
			case IF:
				return allIfThenStmts;
			default:
				cout << "\nerror: unable to find type, StmtTypeTable::GetAllStmtsOfType()\n";
				vector<int> emptyVector;
				return emptyVector;

		} 

	}
}

SynonymType StmtTypeTable::GetStmtTypeOf(int stmtIndex) {
	if (indexTypeTable.count(stmtIndex) != 0) {
		return indexTypeTable[stmtIndex];
	} else {
		return STMT;
	}

}

bool StmtTypeTable::CheckIfStmtOfType(int stmtIndex, SynonymType type) {
	if(type == STMT || type == PROG_LINE) {
		if(indexTypeTable.at(stmtIndex) == ASSIGN || indexTypeTable.at(stmtIndex) == WHILE || indexTypeTable.at(stmtIndex) == IF)
			return true;
	}

    return (indexTypeTable.at(stmtIndex) == type);
}

//API-PKB and DE
void StmtTypeTable::Insert(int stmtIndex, SynonymType type) {
	if (indexTypeTable.count(stmtIndex) == 0 && ValidType(type)) {
		//cout << "\ninserting stmtIndex: " << stmtIndex;
		InsertByTypes(stmtIndex, type);
		indexTypeTable[stmtIndex] = type;
		typeIndexTable[type].push_back(stmtIndex);
		
		maxStmtIndex = maxStmtIndex > stmtIndex ? maxStmtIndex : stmtIndex;
	}
}

void StmtTypeTable::InsertByTypes(int stmtIndex, SynonymType type) {
	switch (type) {
		case ASSIGN:
			allAssignStmts.push_back(stmtIndex);
			break;
		case WHILE:
			allWhileStmts.push_back(stmtIndex);
			break;
		case CALL:
			allCallStmts.push_back(stmtIndex);
			break;
		case IF:
			allIfThenStmts.push_back(stmtIndex);
			break;
		default:
			cout << "\n Error: unable to find type, StmtTypeTable::InsertByTypes()\n";
			return;

	}
}

bool StmtTypeTable::ValidType(SynonymType type) {
	return (type == ASSIGN || type == WHILE || type == CALL || type == IF);
}

int StmtTypeTable::GetNoOfStmts() {
	return indexTypeTable.size();
}

int StmtTypeTable::GetNoOfStmtsOfType(SynonymType type) {
	switch (type) {
		case ASSIGN:
			return allAssignStmts.size();
		case WHILE:
			return allWhileStmts.size();
		case CALL:
			return allCallStmts.size();
		case IF:
			return allIfThenStmts.size();
		default:
			cout << "\nerror: unable to find type, StmtTypeTable::GetNoOfStmtOfType()\n";
			return GetNoOfStmts();

	}
}

int StmtTypeTable::GetMaxStmtIndex() {
	return maxStmtIndex;
}

void StmtTypeTable::ClearData() {
	indexTypeTable.clear();
	typeIndexTable.clear();
	allAssignStmts.clear();
	allWhileStmts.clear();
	allCallStmts.clear();
	allIfThenStmts.clear();

}
