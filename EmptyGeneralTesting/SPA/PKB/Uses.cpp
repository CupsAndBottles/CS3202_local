#include <utility>
#include <map>

#include "Parent.h"
#include "Calls.h"
#include "Uses.h"

using namespace std;

map <int, vector<int>> Uses::stmtToVarTable;
map <int, vector<int>> Uses::varToStmtTable;
map <int, vector<int>> Uses::procToVarTable;
map <int, vector<int>> Uses::varToProcTable;
bool Uses::bitVectorIsBuilt;
int Uses::sizeOfUses;
int Uses::maxStmtOrVar;
int Uses::maxProcOrVar;
vector<vector<bool>> Uses::stmtToVarBitVector;
vector<vector<bool>> Uses::procToVarBitVector;
Uses::Uses() {
	bitVectorIsBuilt = false;
	sizeOfUses = 0;
};

// API
void Uses::SetStmtUsesVar(int stmtUsing, int varUsed) {
    if (!IsStmtUsingVar(stmtUsing, varUsed)) {
        stmtToVarTable[stmtUsing].push_back(varUsed);
        varToStmtTable[varUsed].push_back(stmtUsing);

		sizeOfUses++;
    }
	maxStmtOrVar=  maxStmtOrVar > stmtUsing ? maxStmtOrVar : stmtUsing;
	maxStmtOrVar=  maxStmtOrVar > varUsed ? maxStmtOrVar : varUsed;
	//// initialize if not yet done
	//	if (!bitVectorIsBuilt) {
	//		bitVectorIsBuilt=true;
	//		std::vector<vector<bool>> stmtToVarBitVector;
	//	}
	//	// if the current know number of procs or var is bigger than size of bitVector, expand beginning with the current bitVector
	//	if (maxStmtOrVar>(int)stmtToVarBitVector.size()) {
	//		for (int i=0;i<(int)stmtToVarBitVector.size();i++) {
	//			stmtToVarBitVector[i].push_back(0);
	//		}
	//		std::vector <bool> a (maxStmtOrVar,false);
	//		for (int i=0; i<maxStmtOrVar;i++) {
	//			stmtToVarBitVector.push_back(a);
	//		}
	//	}
	//	// after expanding, insert the new Uses r'ship
	//	stmtToVarBitVector[stmtUsing][varUsed]=1;
	//	stmtToVarBitVector[varUsed][stmtUsing]=1;
}
void Uses::CreateBitVector() {
	// this method transfers the r'nships in tables to bitvectors
	std::vector<vector<bool>> stmtToVarBitVector;		//stmtToVarBitVector[stmt][var]
	std::vector<vector<bool>> procToVarBitVector;		//procToVarBitVector[proc][var]
	std::vector <bool> a (maxStmtOrVar, false);
	std::vector <bool> b (maxProcOrVar, false);

	for (int i=0;i<maxStmtOrVar;i++) {
		stmtToVarBitVector.push_back(a);
	}
	for (int i=0;i<maxProcOrVar;i++) {
		procToVarBitVector.push_back(b);
	}
	int size1=stmtToVarTable.size();
	for (int i=0;i<size1;i++) {
		if (!stmtToVarTable[i].empty()) {
			int size2=stmtToVarTable[i].size();
			for (int j=0;j<size2;j++) {
				int x=stmtToVarTable[i].at(j);
				stmtToVarBitVector[i][x]=1;
			}
		}
	}
	
	size1=procToVarTable.size();
	for (int i=0;i<size1;i++) {
		if (!procToVarTable[i].empty()) {
			int size2=procToVarTable[i].size();
			for (int j=0;j<size2;j++) {
				int x=procToVarTable[i].at(j);
				procToVarBitVector[i][x]=1;
			}
		}
	}
}

bool Uses::IsStmtUsingVarBV(int stmtUsing, int varUsed) {

	return stmtToVarBitVector[stmtUsing][varUsed];
}
bool Uses::IsStmtUsingVar(int stmtUsing, int varUsed) {
	if (stmtToVarTable.count(stmtUsing) != 0) {
		for (vector<int>::iterator it = stmtToVarTable[stmtUsing].begin(); it != stmtToVarTable[stmtUsing].end(); it++) {
			if (*it == varUsed) return true;
		}
		
	}
	return false;

}
vector<int> Uses::GetStmtUsingVar(int varUsed) {
  
    if (varToStmtTable.count(varUsed) == 0) {
		vector<int> stmtsUsingVarUsed;
		return stmtsUsingVarUsed;

	}

    else {
		return varToStmtTable.at(varUsed);

	}
}

vector<int> Uses::GetVarUsedByStmt(int stmtUsing) {
 	if (stmtToVarTable.count(stmtUsing) == 0) {
		vector<int> varsUsedByStmtUsing;
		return varsUsedByStmtUsing;

	} else {
		return stmtToVarTable.at(stmtUsing);

	}

}

void Uses::SetProcUsesVar(int procUsing, int varUsed) {
	if (!IsProcUsingVar(procUsing, varUsed)) {
        procToVarTable[procUsing].push_back(varUsed);
        varToProcTable[varUsed].push_back(procUsing);

		sizeOfUses++;
    }

	maxProcOrVar=  maxProcOrVar > procUsing ? maxProcOrVar : procUsing;
	maxProcOrVar=  maxProcOrVar > varUsed ? maxProcOrVar : varUsed;
	//// initialize if not yet done
	//	if (!bitVectorIsBuilt) {
	//		bitVectorIsBuilt=true;
	//		std::vector<vector<bool>> procToVarBitVector;
	//	}
	//	// if the current number of lines is bigger than size of bitVector, expand beginning with the current bitVector
	//	if (maxProcOrVar>(int)procToVarBitVector.size()) {
	//		for (int i=0;i<(int)procToVarBitVector.size();i++) {
	//			procToVarBitVector[i].push_back(0);
	//		}
	//		std::vector <bool> a (maxProcOrVar,false);
	//		for (int i=0; i<maxProcOrVar;i++) {
	//			procToVarBitVector.push_back(a);
	//		}
	//	}
	//	// after expanding, insert the new Uses r'ship
	//	procToVarBitVector[procUsing][varUsed]=1;
	//	procToVarBitVector[varUsed][procUsing]=1;
}

bool Uses::IsProcUsingVarBV(int procUsing, int varUsed) {

	return procToVarBitVector[procUsing][varUsed];
}
bool Uses::IsProcUsingVar(int procUsing, int varUsed) {
	if (procToVarTable.count(procUsing) != 0) {
		for (vector<int>::iterator it = procToVarTable[procUsing].begin(); it != procToVarTable[procUsing].end(); it++) {
			if (*it == varUsed)
				return true;
		}
	}
	return false;
}
vector<int> Uses::GetProcUsingVar(int varUsed) {
	if (varToProcTable.count(varUsed) == 0) {
		vector<int> procsUsingVarUsed;
		return procsUsingVarUsed;

	} else return varToProcTable.at(varUsed);

}

vector<int> Uses::GetVarUsedByProc(int procUsing) {
	if (procToVarTable.count(procUsing) == 0) {
		vector<int> varsUsedByProcUsing;
		return varsUsedByProcUsing;

	} else {
		return procToVarTable.at(procUsing);

	}

}

bool Uses::HasAnyUses() {
    return !stmtToVarTable.empty();
}

int Uses::SizeOfUses() {
     return sizeOfUses;
}

void Uses::ClearData() {
    stmtToVarTable.clear();
    varToStmtTable.clear();
	procToVarTable.clear();
	varToProcTable.clear();
	sizeOfUses = 0;
	bitVectorIsBuilt=false;
	stmtToVarBitVector.clear();
	procToVarBitVector.clear();	

}

// driver code to test out Uses
/*
   int main() {
   cout << "testing driver program\n";
   cout << "0 to stop\n1 to setUses(stmt, var)\n2 to getUsesByStmt(stmt)\n3 to getUsesStmt(var)\n";
   int action;
   cin >> action;

   Uses obj = Uses();
   while (action!=0) {
   if (action==1) {
   int stmt, var;
   cin >> stmt >> var;
   obj.SetStmtUsesVar(stmt,var);
   }
   else if (action==2) {
   int stmt;
   cin >> stmt;
   set<int> foo = obj.GetVarUsedByStmt(stmt);

//print out all elems in set
set<int>::iterator it;
for (it=foo.begin(); it!=foo.end(); it++) {
cout << *it << " ";
}
cout << endl;
}
else if (action==3) {
int var;
cin >> var;
set<int> foo = obj.GetStmtUsingVar(var);

//print out all elems in set
set<int>::iterator it;
for (it=foo.begin(); it!=foo.end(); it++) {
cout << *it << " ";
}
cout << endl;
}
else cout << "action not recognized\n";
cin >> action;
}

return 0;
}
*/
