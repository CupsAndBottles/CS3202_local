#pragma once

#include <vector>
#include <map>
#include <queue>

using namespace std;

class NextBip {
public:
	// default constructor
	NextBip();

	// API
	static void setReturnPoint(int procedure, int returnPoint);
	static void setEndOfProc(int procedure, int endOfProc);
	static vector<int> GetNextAfter(int progLineBefore);
	static vector<int> GetNextBefore(int progLineAfter);
	static vector<int> GetReturnTo(int terminatingStmt, int callingProc); // returns the return point in the calling proc
	static vector<int> GetEntryPoint(int startingStmt, int callingProc); // returns the entry call stmt in the calling proc

	static bool IsNextBipT(int progLineBefore, int progLineAfter);
	static vector<int> GetNextBipTAfter(int progLineBefore);
	static vector<int> GetNextBipTBefore(int progLineAfter);

	static void ClearData();

private:
	static map <int, vector<int>> returnPoints;
	static map <int, vector<int>> endsOfProcs;
	static queue<int> AddToQueue(queue<int> linesToCheck, vector<int> additions);
};

