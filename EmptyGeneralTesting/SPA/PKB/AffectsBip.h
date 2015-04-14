#pragma once

#include <vector>
#include <queue>
#include <utility>

using namespace std;

class AffectsBip {
public:
	AffectsBip();

	// API
	static bool IsAffectsBip(int stmtAffecting, int stmtAffected);
	static vector<int> GetStmtsAffectedBipBy(int stmtAffecting);
	static vector<int> GetStmtsAffectingBip(int stmtAffected);

	/*static bool IsAffectsT(int stmtAffecting, int stmtAffected);
	static vector<int> GetStmtsAffectedTBy(int stmtAffecting);
	static vector<int> GetStmtsAffectingT(int stmtAffected);*/

private:
	static bool CheckBipCFG(int stmtAffecting, int stmtAffected, int varModified);
	static vector<int> TraverseDownBipCFG(int stmtAffecting, int varModified);
	static vector<int> TraverseUpBipCFG(int stmtAffected, vector<int> varsUsed);
	static pair<vector<int>, vector<bool>> RecurTraverseUpBipCFG(int currStmt, vector<int> varsUsed, vector<bool> stmtsIsChecked);

	static vector<int> OrOperationOnVects(vector<int> vect1, vector<int> vect2);
	static vector<bool> OrOperationOnBoolVects(vector<bool> vect1, vector<bool> vect2);

};
