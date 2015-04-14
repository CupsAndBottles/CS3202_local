#pragma once

#include <vector>
#include <queue>
#include <utility>

using namespace std;

class Affects {
public:
	Affects(void);

	// API
	static bool IsAffects(int stmtAffecting, int stmtAffected);
	static vector<int> GetStmtsAffectedBy(int stmtAffecting);
	static vector<int> GetStmtsAffecting(int stmtAffected);

	static bool IsAffectsT(int stmtAffecting, int stmtAffected);
	static vector<int> GetStmtsAffectedTBy(int stmtAffecting);
	static vector<int> GetStmtsAffectingT(int stmtAffected);

	~Affects(void);

private:
	static bool CheckCFG(int stmtAffecting, int stmtAffected, int varModified);
	static vector<int> TraverseDownCFG(int stmtAffecting, int varModified);
	static vector<int> TraverseUpCFG(int stmtAffected, vector<int> varsUsed);

	static pair<vector<int>, vector<bool>> RecurTraverseUpCFG(int currStmt, vector<int> varsUsed, vector<bool> stmtsIsChecked);

	static bool NoVarsOverlap(vector<int> varsVect1, vector<int> varsVect2);
	static vector<int> OrOperationOnVects(vector<int> vect1, vector<int> vect2);
	static vector<bool> OrOperationOnBoolVects(vector<bool> vect1, vector<bool> vect2);
	//static bool IsAllFalse(vector<bool> boolVect);
	// may want to add cache here

};

