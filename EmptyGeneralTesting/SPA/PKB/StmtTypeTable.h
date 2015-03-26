#ifndef STMTTYPETABLE_H
#define STMTTYPETABLE_H

#include <utility>
#include <vector>
#include <map>

#include "..\QueryProcessor\Grammar.h"

using namespace std;

class StmtTypeTable {
public:
	StmtTypeTable();

	//API-Query
	static vector<int> GetAllStmtsOfType(SynonymType type);
	// static vector<int> GetAllStmtsOfTypeFrmProc(SynonymType type, int procIndex);
	static bool CheckIfStmtOfType(int stmtIndex, SynonymType type);

	//API-PKB and DE
	static void Insert(int stmtIndex, SynonymType type);
	
	static int GetNoOfStmts();
	static int GetNoOfStmtsOfType(SynonymType type);

	static void ClearData();

private:
	//data structures
	static map<int, SynonymType> indexTypeTable;
	static map<SynonymType, vector<int> > typeIndexTable;

	static vector<int> allAssignStmts;
	static vector<int> allWhileStmts;
	static vector<int> allCallStmts;
	static vector<int> allIfThenStmts;

	static void InsertByTypes(int stmtIndex, SynonymType type);
	static bool ValidType(SynonymType type);
};

#endif
