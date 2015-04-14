#include <map>
#include <vector>
#include <queue>
#include <iostream>

#include "Calls.h"
#include "ProcTable.h"

map<int, vector<int>> Calls::callingToCalledTable;
map<int, vector<int>> Calls::calledToCallingTable;
map<int, vector<bool>> Calls::callingToCalledBitVector;
int Calls::noOfCallsRelationships;
int Calls::maxNoOfProcs;
bool Calls::bitVectorIsBuilt;
vector<vector<bool>> Calls::bitVector;
vector<vector<bool>> Calls::callingToCalledTBV;
vector<vector<int>> Calls::callingToCalledTTable;
vector<vector<int>> Calls::calledToCallingTTable;

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
		}
}

void Calls::CreateBitVector() {
	// this method transfers the r'nships in tables to bitvectors
	// bitVector[calling][called]
	std::vector <bool> a (maxNoOfProcs, false);

	for (int i=0;i<maxNoOfProcs;i++) {
		bitVector.push_back(a);
	}
	
	typedef map<int, vector<int>>::iterator map_it;

	for (map_it callsIt = callingToCalledTable.begin(); callsIt != callingToCalledTable.end(); callsIt++) {
		if (!callsIt->second.empty()) {
			int size2 = callsIt->second.size();
			for (int j = 0; j < size2; j++) {
				int x = callsIt->second.at(j);
				bitVector[callsIt->first][x]=true;
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

bool Calls::IsCalls(int procCalling, int procCalled) {
	if (callingToCalledBitVector.count(procCalling) != 0) {
		if ((procCalled + 1) <= (int) callingToCalledBitVector[procCalling].size())
			return callingToCalledBitVector[procCalling].at(procCalled);
	}

	return false;
}

bool Calls::IsCallsBV(int procCalling, int procCalled) {
	if (procCalling >= 0 && procCalling <= maxNoOfProcs && procCalled <=maxNoOfProcs) {

		return bitVector[procCalling][procCalled];
	}
	else return false;
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

bool Calls::IsCallsTBV(int procCalling, int procCalled) {
	int totalNoOfProcs = ProcTable::GetNoOfProcs();

	if (procCalling >= 0 && procCalling < totalNoOfProcs
		&& procCalled >= 0 && procCalled < totalNoOfProcs) {
			return callingToCalledTBV.at(procCalling).at(procCalled);
	} else {
		return false;
	}

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

vector<int> Calls::GetStoredProcsCalledTBy(int procCalling) {
	int totalNoOfProcs = ProcTable::GetNoOfProcs();

	if (procCalling >= 0 && procCalling < totalNoOfProcs) {
		if ((int) callingToCalledTTable.size() >= procCalling) {
			return callingToCalledTTable.at(procCalling);
		} else {
			cout << "\nwarning: unable to find " << procCalling << " in callingToCalledTTable. \n";
			return GetProcsCalledTBy(procCalling);
		}
	
	}

	vector<int> emptyList;
	return emptyList;
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

vector<int> Calls::GetStoredProcsCallingT(int procCalled) {
	int totalNoOfProcs = ProcTable::GetNoOfProcs();

	if (procCalled >= 0 && procCalled < totalNoOfProcs) {
		if ((int) calledToCallingTTable.size() >= procCalled) {
			return calledToCallingTTable.at(procCalled);
		} else {
			cout << "\nwarning: unable to find " << procCalled << " in calledToCallingTTable. \n";
			return GetProcsCallingT(procCalled);
		}
	
	}

	vector<int> emptyList;
	return emptyList;

}

queue<int> Calls::AddToQueue(queue<int> procsToCheck, vector<int> additions) {
	for (unsigned int i = 0; i < additions.size(); i++) {
		procsToCheck.push(additions.at(i));
	}

	return procsToCheck;

}

void Calls::CreateCallingToCalledTBV() {
	int totalNoOfProcs = ProcTable::GetNoOfProcs();
	vector<bool> emptyRow(totalNoOfProcs + 1, false);
	callingToCalledTBV = vector<vector<bool>>(totalNoOfProcs + 1, emptyRow);

	vector<int> calledT;
	for (int i = 0; i < totalNoOfProcs; i++) {
		calledT = GetProcsCalledTBy(i);

		for(vector<int>::iterator it = calledT.begin(); it != calledT.end(); it++) {
			callingToCalledTBV.at(i).at(*it) = true;
		}
	
	}

}

void Calls::CreateCallingToCalledTTable() {
	int totalNoOfProcs = ProcTable::GetNoOfProcs();
	vector<int> emptyRow;
	callingToCalledTTable = vector<vector<int>>(totalNoOfProcs + 1, emptyRow);

	vector<int> calledT;
	for (int i = 0; i < totalNoOfProcs; i++) {
		calledT = GetProcsCalledTBy(i);
		callingToCalledTTable.at(i) = calledT;
	
	}

}

void Calls::CreateCalledToCallingTTable() {
	int totalNoOfProcs = ProcTable::GetNoOfProcs();
	vector<int> emptyRow;
	calledToCallingTTable = vector<vector<int>>(totalNoOfProcs + 1, emptyRow);

	vector<int> callingT;
	for (int i = 0; i < totalNoOfProcs; i++) {
		callingT = GetProcsCallingT(i);
		calledToCallingTTable.at(i) = callingT;
	}

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

	callingToCalledTBV.clear();
	callingToCalledTTable.clear();
	calledToCallingTTable.clear();

}