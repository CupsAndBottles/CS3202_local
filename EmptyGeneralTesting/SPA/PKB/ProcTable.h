#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

/** to store a mapping of all processes, and
	- their indexes
	- the stmts they contain **/

class ProcTable {
public:
	// methods
	// Default constructor
	ProcTable();

	// Methods
	static int InsertProc(string procName);
	static int GetIndexOfProc(string procName); 
	static string GetProcName(int varIndex);
	static vector<string> GetAllProcNames();
	
	static void SetFirstStmtNoOfProc(int procIndex, int firstStmtNo);
	static void SetLastStmtNoOfProc(int procIndex, int lastStmtNo);
	
	static int GetFirstStmtNoOfProc(int procIndex);
	static int GetLastStmtNoOfProc(int procIndex);
	static int GetProcOfStmt(int stmtNo);

	static int GetNoOfProcs();
	
	// method(s) to aid testing
	static void ClearData();

private:
	static vector<string>  procNames;
	static map<int, int> firstStmtNosList;
	static map<int, int> lastStmtNosList;

	static vector<string>::iterator SearchFor(string procName);

};
