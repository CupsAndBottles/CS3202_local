#ifndef USES_H
#define USES_H

#include <utility>
#include <vector>
#include <map>

using namespace std;

class Uses {
public:
	// methods
	// Default constructor
	Uses();

	// API
	static void SetStmtUsesVar(int stmtUsing, int varUsed);
	static bool IsStmtUsingVar(int stmtUsing, int varUsed);
	static bool IsStmtUsingVarBV(int stmtUsing, int varUsed);
    static vector<int> GetStmtUsingVar(int varUsed);
    static vector<int> GetVarUsedByStmt(int stmtUsing);

	static void SetProcUsesVar(int procUsing, int varUsed);
	static bool IsProcUsingVar(int procUsing, int varUsed);
	static bool IsProcUsingVarBV(int procUsing, int varUsed);
	static vector<int> GetProcUsingVar(int varUsed);
	static vector<int> GetVarUsedByProc(int procUsing);

	static bool HasAnyUses();
	static void CreateBitVector();

	// helper methods for testing
	static int SizeOfUses();
	static void ClearData();

private:
	static map <int, vector<int>> stmtToVarTable;
    static map <int, vector<int>> varToStmtTable;
	static vector<vector<bool>> stmtToVarBitVector;

	static map <int, vector<int>> procToVarTable;
	static map <int, vector<int>> varToProcTable;
	static vector<vector<bool>> procToVarBitVector;
	
	static int maxProcOrVar;
	static int maxStmtOrVar;
	static bool bitVectorIsBuilt;
	static int sizeOfUses;

};
#endif
