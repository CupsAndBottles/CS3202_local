#pragma once

#include "Program.h"
#include "..\Parser\Token.h"
#include "..\PKB\Modifies.h"
#include "..\PKB\Uses.h"
#include "..\PKB\ProcTable.h"
#include "..\PKB\Calls.h"
#include "..\PKB\ConstTable.h"
#include "..\PKB\Follows.h"
#include "..\PKB\Next.h"
#include "..\PKB\Parent.h"
#include "..\PKB\StmtTypeTable.h"
#include "..\PKB\VarTable.h"

void Program::ClearAll() { // clears all PKB elements
	Program::ClearData();
	Modifies::ClearData();
	Uses::ClearData();
	ProcTable::ClearData();
	Calls::ClearData();
	ConstTable::ClearData();
	Follows::ClearData();
	Next::ClearData();
	Parent::ClearData();
	StmtTypeTable::ClearData();
	VarTable::ClearData();
}

Program::Program() {}

TNode Program::program = TNode::ConstructProgramTNode("");
map<int, TNode*> Program::stmtNumberMap = map<int, TNode*>();

TNode& Program::GetASTRootNode() {
	return program;
}

TNode& Program::GetStmtFromNumber(int stmtNum) {
	return *(stmtNumberMap[stmtNum]);
}

int Program::GetNumOfStmts() {
	return stmtNumberMap.size();
}

void Program::InsertStmt(TNode* stmt, int stmtNum) {
	stmtNumberMap[stmtNum] = stmt;
}

void Program::ClearData() { // leaky method
	Program::program = TNode::ConstructProgramTNode("");
	Program::stmtNumberMap = map<int, TNode*>();
}