#ifndef NEXT_H
#define NEXT_H

#include <vector>
#include <map>
#include <queue>

using namespace std;

class Next {
public:
	// Default constructor
	Next(void);

	// API
	static void SetNext(int progLineBefore, int progLineAfter);
	static bool IsNext(int progLineBefore, int progLineAfter);
	static bool IsNextBV(int progLineBefore, int progLineAfter);
	static vector<int> GetNextAfter(int progLineBefore);
	static vector<int> GetNextBefore(int progLineAfter);

	static bool IsNextT(int progLineBefore, int progLineAfter);
	static vector<int> GetNextTAfter(int progLineBefore);
	static vector<int> GetNextTBefore(int progLineAfter);

//	static bool IsNotModifiedInPath(int progLineBefore, int progLineAfter);

	static bool HasAnyNext();
	static void CreateBitVector();
	// helper methods for testing
	static int SizeOfNext();
	static void ClearData();

private:
	static map<int, vector<int>> beforeToAfterTable;
	static map<int, vector<int>> afterToBeforeTable;
	static int sizeOfNext;
	static vector<vector<bool>> bitVector;
	static bool bitVectorIsBuilt;
	static int maxNoOfLines;

	static bool NoRelationshipConflicts(int progLineBefore, int progLineAfter);
	static bool BeforeNodeNotFull(int progLineBefore, int progLineAfter);
	static bool AfterNodeIsValid(int progLineBefore, int progLineAfter);
	// each node can only have max, 2 children, but unlimited number of parents
	static queue<int> AddToQueue(queue<int> linesToCheck, vector<int> additions);

};
#endif
