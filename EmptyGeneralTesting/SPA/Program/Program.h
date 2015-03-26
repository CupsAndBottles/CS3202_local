#pragma once

#include "..\Parser\Token.h"
#include "TNode\TNode.h"

#include <map>

class Program {
public:
	friend class Parser;
	static TNode& GetASTRootNode();
	static TNode& GetStmtFromNumber(int);
	static int GetNumOfStmts();
	static void ClearData();
	static void ClearAll();

protected:
	static void InsertStmt(TNode*, int);

private:
	Program();

	static TNode program;
	static map<int, TNode*> stmtNumberMap;
};

