#include <map>
#include <vector>
#include <queue>

#include "Calls.h"
#include "ProcTable.h"

map<int, vector<int>> Calls::callingToCalledTable;
map<int, vector<int>> Calls::calledToCallingTable;
map<int, vector<bool>> Calls::callingToCalledBitVector;
int Calls::noOfCallsRelationships;
int Calls::maxNoOfProcs;
bool Calls::bitVectorIsBuilt;
vector<vector<bool>> Calls::bitVector;
// empty constructor
Calls::Calls() {
	noOfCallsRelationships = 0;
	maxNoOfProcs = 0;
	bitVectorIsBuilt = false;
}

// API
void Calls::SetCalls(int procCalling, int procCalled) {
	if (!IsCalls(procCalling, procCalled)  && !IsCalls(procCalled, procCalling)) {
		callingToCalledTable[procCalling].push_back(procCalled);
		calledToCallingTable[procCalled].push_back(procCalling);

		SetCallingToCalledBitVector(procCalling, procCalled);

		noOfCallsRelationships++;

		maxNoOfProcs = maxNoOfProcs > procCalling ? maxNoOfProcs : procCalling;
		maxNoOfProcs = maxNoOfProcs > procCalled ? maxNoOfProcs : procCalled;
		//// initialize if not yet done
		//if (!bitVectorIsBuilt) {
		//	bitVectorIsBuilt=true;
		//	std::vector<vector<bool>> bitVector;
		//}
		//// if the current number of lines is bigger than size of bitVector, expand beginning with the current bitVector
		//if (maxNoOfProcs>(int)bitVector.size()) {
		//	for (int i=0;i<(int)bitVector.size();i++) {
		//		bitVector[i].push_back(0);
		//	}
		//	std::vector <bool> a (maxNoOfProcs, false);
		//	for (int i=0; i<maxNoOfProcs;i++) {
		//		bitVector.push_back(a);
		//	}
		//}
		//// after expanding, insert the new Calls r'ship
		//bitVector[procCalling][procCalled]=1;
		//bitVector[procCalled][procCalling]=1;
	}

}

void Calls::CreateBitVector() {
	// this method transfers the r'nships in tables to bitvectors
	std::vector<vector<bool>> bitVector;		// bitVector[calling][called]
	std::vector <bool> a (maxNoOfProcs, false);

	for (int i=0;i<maxNoOfProcs;i++) {
		bitVector.push_back(a);
	}
	int size1=callingToCalledTable.size();
	for (int i=0;i<size1;i++) {
		if (!callingToCalledTable[i].empty()) {
			int size2=callingToCalledTable[i].size();
			for (int j=0;j<size2;j++) {
				int x=callingToCalledTable[i].at(j);
				bitVector[i][x]=1;
			}
		}
	}
}
void Calls::SetCallingToCalledBitVector(int procCalling, int procCalled) {
	if ((procCalled + 1) > (int) callingToCalledBitVector[procCalling].size()) {
		for (int i = 0; i < ((procCalled + 1) * 2); i++) {
			callingToCalledBitVector[procCalling].push_back(false);
		}
	}

	callingToCalledBitVector[procCalling].at(procCalled) = true;

}

bool Calls::IsCalls(int stmtCalling, int procCalled) {
	if (callingToCalledBitVector.count(stmtCalling) != 0) {
		if ((procCalled + 1) <= (int) callingToCalledBitVector[stmtCalling].size())
			return callingToCalledBitVector[stmtCalling].at(procCalled);
	}

	return false;
}

bool Calls::IsCallsBV(int stmtCalling, int procCalled) {
	return bitVector[stmtCalling][procCalled];
}
vector<int> Calls::GetProcsCalledBy(int procCalling) {
	if(callingToCalledTable.count(procCalling) != 0) {
		return callingToCalledTable[procCalling];
	} else {
		vector<int> emptyVector;
		return emptyVector;
	}
}

vector<int> Calls::GetProcsCalling(int procCalled) {
	if(calledToCallingTable.count(procCalled) != 0) {
		return calledToCallingTable[procCalled];
	} else {
		vector<int> emptyVector;
		return emptyVector;
	}
}

bool Calls::IsCallsT(int procCalling, int procCalled) {
	
	queue<int> procsToCheck;
		
	int maxNoOfProcsSoFar = maxNoOfProcs > ProcTable::GetNoOfProcs() ? maxNoOfProcs : ProcTable::GetNoOfProcs();
	vector<bool> checkedProcs (maxNoOfProcsSoFar + 1, false);
		
	int currProc;

	procsToCheck.push(procCalling);

	while (!procsToCheck.empty()) {
		currProc = procsToCheck.front();

		if (!checkedProcs.at(currProc)) {
			if (IsCalls(currProc, procCalled)) {
				return true;
			}
			checkedProcs[currProc] = true;
			procsToCheck = AddToQueue(procsToCheck, GetProcsCalledBy(currProc));

		}
		procsToCheck.pop();
	}
	return false;
}

vector<int> Calls::GetProcsCalledTBy(int procCalling) {
	queue<int> procsToCheck;
	vector<int> procsCalledTBy;
	
	int maxNoOfProcsSoFar = maxNoOfProcs > ProcTable::GetNoOfProcs() ? maxNoOfProcs : ProcTable::GetNoOfProcs();
	vector<bool> checkedProcs (maxNoOfProcsSoFar + 1, false);

	int currProc;

	procsToCheck = AddToQueue(procsToCheck, GetProcsCalledBy(procCalling));

	while (!procsToCheck.empty()) {
		currProc = procsToCheck.front();
		if (!checkedProcs.at(currProc)) {
			procsCalledTBy.push_back(currProc);
			procsToCheck = AddToQueue(procsToCheck, GetProcsCalledBy(currProc));

			checkedProcs[currProc] = true;
		}
		procsToCheck.pop();
	
	}

	return procsCalledTBy;

}

vector<int> Calls::GetProcsCallingT(int procCalled) {
	queue<int> procsToCheck;
	vector<int> procsCallingT;
	
	int maxNoOfProcsSoFar = maxNoOfProcs > ProcTable::GetNoOfProcs() ? maxNoOfProcs : ProcTable::GetNoOfProcs();
	vector<bool> checkedProcs (maxNoOfProcsSoFar + 1, false);
	
	int currProc;

	procsToCheck = AddToQueue(procsToCheck, GetProcsCalling(procCalled));

	while (!procsToCheck.empty()) {
		currProc = procsToCheck.front();
		if(!checkedProcs.at(currProc)) {
			procsCallingT.push_back(currProc);
			procsToCheck = AddToQueue(procsToCheck, GetProcsCalling(currProc));

			checkedProcs[currProc] = true;
		}
		procsToCheck.pop();

	}

	return procsCallingT;
}

queue<int> Calls::AddToQueue(queue<int> procsToCheck, vector<int> additions) {
	for (unsigned int i = 0; i < additions.size(); i++) {
		procsToCheck.push(additions.at(i));
	}

	return procsToCheck;

}

bool Calls::HasAnyCalls() {
	return noOfCallsRelationships > 0;
}

int Calls::SizeOfCalls() {
	return noOfCallsRelationships;
}

void Calls::ClearData() {
	callingToCalledTable.clear();
	calledToCallingTable.clear();
	callingToCalledBitVector.clear();
	noOfCallsRelationships = 0;
	bitVectorIsBuilt=false;
	bitVector.clear();

}