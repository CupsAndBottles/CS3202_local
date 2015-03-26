#include <utility>
#include <vector>
#include <map>
#include <set>

using namespace std;

class ConstTable {
public:
	// methods\
	// Default constructor
	ConstTable();

	// API
	static void SetStmtUsesConst(int stmtUsing, int constUsed);
	static bool IsStmtUsingConst(int stmtUsing, int constUsed);
    static vector<int> GetStmtUsingConst(int constUsed);
    static vector<int> GetConstUsedByStmt(int stmtUsing);

	static bool HasAnyConst();
    static vector<int> GetAllConst();
	static int SizeOfConstTable();
    static void ClearData();

	/*void SetProcUsesConst(int procUsing, int constUsed);
	bool IsConstUsesProc(int procUsing, int constUsed);
	vector<int> GetProcUsingConst(int constUsed);
	vector<int> GetConstUsedByProc(int porcUsing);*/

private:
	static map <int, vector<int> > stmtToConstTable;
    static map <int, vector<int> > constToStmtTable;

	// static map <int, vector<int>> procToConstTable;
	// static map <int, vector<int>> constToProcTable;

};
