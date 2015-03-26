#ifndef FOLLOWS_H
#define FOLLOWS_H

#include <utility>
#include <vector>
#include <map>

using namespace std;

class Follows {
public:
	// Default constructor
	Follows();

	// API
	static void SetFollows(int stmtBefore, int stmtAfter);
	static bool IsFollows(int stmtBefore, int stmtAfter);
	static int GetFollowsBefore(int stmtAfter);
	static int GetFollowsAfter(int stmtBefore);
	
	static bool IsFollowsT(int stmtBefore, int stmtAfter);
	static vector<int> GetFollowsTBefore(int stmtAfter);
	static vector<int> GetFollowsTAfter(int stmtBefore);
	

	static bool HasAnyFollows();

	// helper methods for testing
	static int SizeOfFollows();
	static void ClearData();

private:
	static map <int, int> beforeToAfterTable;
	static map <int, int> afterToBeforeTable;

	static bool NoRelationshipConflicts(int stmtBefore, int stmtAfter);
	static bool HasNoStmtAfter(int stmtBefore);
	static bool HasNoStmtBefore(int stmtAfter);

};
#endif