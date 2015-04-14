#include <vector>
#include <map>
#include <iostream>

#include "Parent.h"
#include "Calls.h"
#include "Modifies.h"

using namespace std;

map <int, vector<int>> Modifies::stmtToVarTable;
map <int, vector<int>> Modifies::varToStmtTable;
map <int, vector<int>> Modifies::procToVarTable;
map <int, vector<int>> Modifies::varToProcTable;
bool Modifies::bitVectorIsBuilt;
int Modifies::sizeOfModifies;
int Modifies::maxStmtOrVar;
int Modifies::maxProcOrVar;
vector<vector<bool>> Modifies::stmtToVarBitVector;
vector<vector<bool>> Modifies::procToVarBitVector;
		
Modifies::Modifies() {
	bitVectorIsBuilt = false;
	sizeOfModifies = 0;
	maxStmtOrVar = 0;
	maxProcOrVar = 0;
};

// API
void Modifies::SetStmtModifiesVar(int stmtModifying, int varModified) {
	if (!IsStmtModifyingVar(stmtModifying, varModified)) {
        stmtToVarTable[stmtModifying].push_back(varModified);
        varToStmtTable[varModified].push_back(stmtModifying);

		sizeOfModifies++;
		maxStmtOrVar = (maxStmtOrVar > stmtModifying) ? maxStmtOrVar : stmtModifying;
		maxStmtOrVar = (maxStmtOrVar > varModified) ? maxStmtOrVar : varModified;

    }

}

void Modifies::CreateBitVector() {
	// this method transfers the r'nships in tables to bitvectors
	/* bug1: should be re-initialising the bit vectors, otherwise, all information created in this method will be lost */
	//std::vector<vector<bool>> stmtToVarBitVector;		//stmtToVarBitVector[stmt][var]
	//std::vector<vector<bool>> procToVarBitVector;		//procToVarBitVector[proc][var]
	
	/* bug2: should add 1 to the size, because index starts from 0, 
	example, a SIMPLE code with 5 vars will have to access index 5, since index starts from 0, the size of the bitvector should be 6*/
	std::vector <bool> a (maxStmtOrVar+1, false);
	std::vector <bool> b (maxProcOrVar+1, false);
	
	typedef map<int, vector<int>>::iterator map_it;

	/*cout << "\nmaxStmtOrVar: " << maxStmtOrVar << "\n";
	cout << "maxProcOrVar: " << maxProcOrVar << "\n";*/

	for (int i = 0; i <= maxStmtOrVar; i++) {
		stmtToVarBitVector.push_back(a);
		//cout << "i: " << i << "\n";
	}
	for (int i = 0; i <= maxProcOrVar; i++) {
		procToVarBitVector.push_back(b);
		//cout << "i: " << i << "\n";
	}

	/* bug3: use an iterator to go through the map, becuase the size of the map is not representative of the index you stored inside
	for example, if you store procs 3 and 4, the size of the map will still be 2. thus, you'll be accessing procToVarTable[0] and procToVarTable[1], 
	which do not exist*/
	for(map_it stmtToVarIt = stmtToVarTable.begin(); stmtToVarIt != stmtToVarTable.end(); stmtToVarIt++) {
		if (!stmtToVarIt->second.empty()) {
			int size2 = stmtToVarIt->second.size();
			for (int j = 0; j < size2; j++) {
				// cout << "j: " << j << "\n";
				int x = stmtToVarIt->second.at(j);
				stmtToVarBitVector[stmtToVarIt->first][x] = true;
				// cout << "added stmt " << stmtToVarIt->first << " " << x << "\n";
			}
		}
	}
	//cout << "size of stmtbitvector: " << stmtToVarBitVector.size() << "\n";
	for(map_it procToVarIt = procToVarTable.begin(); procToVarIt != procToVarTable.end(); procToVarIt++) {
		if (!procToVarIt->second.empty()) {
			int size2 = procToVarIt->second.size();
			for (int j = 0; j < size2; j++) {
				// cout << "j: " << j << "\n";
				int x=procToVarIt->second.at(j);
				procToVarBitVector[procToVarIt->first][x] = true;
				// cout << "added proc " << procToVarIt->first << " " << x << "\n";
			}
		}
	}
	//cout << "size of procbitvector: " << procToVarBitVector.size() << "\n";
}
bool Modifies::IsStmtModifyingVar(int stmtModifying, int varModified) {
	
	if (stmtToVarTable.count(stmtModifying) != 0) {
		for (vector<int>::iterator it = stmtToVarTable[stmtModifying].begin(); it != stmtToVarTable[stmtModifying].end(); it++) {
			if (*it == varModified)
				return true;
		}
		
	}
	return false;

}
bool Modifies::IsStmtModifyingVarBV(int stmtModifying, int varModified) {
	//cout << "size of stmtbitvector: " << stmtToVarBitVector.size() << "\n";
	//return true;
	if (stmtModifying > 0 && stmtModifying <= maxStmtOrVar) {

		return stmtToVarBitVector[stmtModifying][varModified];

	}
	else return false; 
}
vector<int> Modifies::GetStmtModifyingVar(int varModified) {    
    if (varToStmtTable.count(varModified) == 0) {
		vector<int> stmtsModifyingVarModified;
		return stmtsModifyingVarModified;

	} else { 
		return varToStmtTable.at(varModified);

	}
}

vector<int> Modifies::GetStmtsModifyingBV(int varModified) {
	vector<int> result;
	for (int i=0; i<stmtToVarBitVector.size(); i++) {
		if (stmtToVarBitVector[i][varModified])
			result.push_back(i);
	}

	return result;
}
vector<int> Modifies::GetVarModifiedByStmt(int stmtModifying) {
    if (stmtToVarTable.count(stmtModifying) == 0) {
		vector<int> varsModifyiedByStmtModifying;
		return varsModifyiedByStmtModifying;

	} else {
		return stmtToVarTable.at(stmtModifying);

	}

}
vector<int> Modifies::GetVarsModifiedByBV(int stmtModifying) {
	vector<int> result;
	for (int i=0; i<stmtToVarBitVector[stmtModifying].size(); i++) {
		if (stmtToVarBitVector[stmtModifying][i])
			result.push_back(i);
	}

	return result;
}
void Modifies::SetProcModifiesVar(int procModifying, int varModified) {
	if (!IsProcModifyingVar(procModifying, varModified)) {
        procToVarTable[procModifying].push_back(varModified);
        varToProcTable[varModified].push_back(procModifying);

		sizeOfModifies++;
		maxProcOrVar = (maxProcOrVar > procModifying) ? maxProcOrVar : procModifying;
		maxProcOrVar = (maxProcOrVar > varModified) ? maxProcOrVar : varModified;

    }
}


bool Modifies::IsProcModifyingVar(int procModifying, int varModified) {

	if (procToVarTable.count(procModifying) != 0) {
		for (vector<int>::iterator it = procToVarTable[procModifying].begin(); it != procToVarTable[procModifying].end(); it++) {
			if (*it == varModified)
				return true;
		}
	
	}
	return false;
}

bool Modifies::IsProcModifyingVarBV(int procModifying, int varModified) {
	if (procModifying > 0 && procModifying <= maxProcOrVar && varModified <=maxProcOrVar) {

		return procToVarBitVector[procModifying][varModified];

	}
	else return false; 
}
vector<int> Modifies::GetProcModifyingVar(int varModified) {
	if (varToProcTable.count(varModified) == 0) {
		vector<int> procsModifyingVarModified;
		return procsModifyingVarModified;

	} else { 
		return varToProcTable.at(varModified);

	}

}

vector<int> Modifies::GetVarModifiedByProc(int procModifying) {
	if (procToVarTable.count(procModifying) == 0) {
		vector<int> varsModifyiedByProcModifying;
		return varsModifyiedByProcModifying;

	} else {
		return procToVarTable.at(procModifying);

	}

}

bool Modifies::HasAnyModifies() {
    return !stmtToVarTable.empty();

}

int Modifies::SizeOfModifies() {
	return sizeOfModifies;

}

void Modifies::ClearData() {
	stmtToVarTable.clear();
	varToStmtTable.clear();
	procToVarTable.clear();
	varToProcTable.clear();
	sizeOfModifies = 0;
	bitVectorIsBuilt=false;
	stmtToVarBitVector.clear();
	procToVarBitVector.clear();	
	maxStmtOrVar = 0;
	maxProcOrVar = 0;
}

// driver code to test out Modifies
//
//void main() {
//	Modifies::ClearData();
//	Modifies::SetStmtModifiesVar(1, 0);
//	Modifies::SetStmtModifiesVar(2, 0);
//	Modifies::SetProcModifiesVar(3, 0);
//	Modifies::SetProcModifiesVar(4, 0);
//	Modifies::CreateBitVector();
//
//	cout << Modifies::IsStmtModifyingVarBV(1,0);
//	//Modifies::IsStmtModifyingVarBV(2,0);
//}
//
