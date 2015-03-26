#ifndef CALLS_H
#define CALLS_H

#include <vector>
#include <map>
#include <queue>

using namespace std;

class Calls {
public:
	// Default constructor
	Calls();

	// API
	static void SetCalls(int procCalling, int procCalled);
	static bool IsCalls(int procCalling, int procCalled);
	static bool IsCallsBV(int procCalling, int procCalled);
	static vector<int> GetProcsCalledBy(int procCalling);
	static vector<int> GetProcsCalling(int procCalled);
	
	static bool IsCallsT(int procCalling, int procCalled);
	static vector<int> GetProcsCalledTBy(int procCalling);
	static vector<int> GetProcsCallingT(int procCalled);

	static bool HasAnyCalls();
	static void CreateBitVector();
	// helper methods for testing
	static int SizeOfCalls();
	static void ClearData();

private:
	static map<int, vector<int>> callingToCalledTable;
	static map<int, vector<int>> calledToCallingTable;
	static map<int, vector<bool>> callingToCalledBitVector;
	static vector<vector<bool>> bitVector;
	static bool bitVectorIsBuilt;
	static int maxNoOfProcs;

	static int noOfCallsRelationships;

	static void SetCallingToCalledBitVector(int procCalling, int procCalled);
	static queue<int> AddToQueue(queue<int> procsToCheck, vector<int> additions);

};
#endif
