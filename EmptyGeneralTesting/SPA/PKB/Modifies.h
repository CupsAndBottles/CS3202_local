#ifndef MODIFIES_H
#define MODIFIES_H

#include <utility>
#include <vector>
#include <map>

using namespace std;

class Modifies {
    public:
		// methods
		// Default constructor
        Modifies();

        // API
        static void SetStmtModifiesVar(int stmtModifying, int varModified); 
        static bool IsStmtModifyingVar(int stmtModifying, int varModified);
        static bool IsStmtModifyingVarBV(int stmtModifying, int varModified);
        static vector<int> GetStmtModifyingVar(int varModified);
		static vector<int> GetVarModifiedByStmt(int stmtModifying);

		static void SetProcModifiesVar(int procModifying, int varModified);
		static bool IsProcModifyingVar(int procModifying, int varModified);
		static bool IsProcModifyingVarBV(int procModifying, int varModified);
		static vector<int> GetProcModifyingVar(int varModified);
		static vector<int> GetVarModifiedByProc(int procModifying);

        static bool HasAnyModifies();
		static void CreateBitVector();
		// helper methods for testing
        static int SizeOfModifies();
		static void ClearData();

    private:
        static map <int, vector<int>> stmtToVarTable;
        static map <int, vector<int>> varToStmtTable;
		static vector<vector<bool>> stmtToVarBitVector;

		static map <int, vector<int>> procToVarTable;
		static map <int, vector<int>> varToProcTable;
		static vector<vector<bool>> procToVarBitVector;
		
		static int maxStmtOrVar;
		static int maxProcOrVar;
		static bool bitVectorIsBuilt;
		static int sizeOfModifies;

};
#endif
