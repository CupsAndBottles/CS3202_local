#pragma once

#include "Token.h"
#include "..\Program\Program.h"

#include <vector>
#include <deque>
#include <map>

using namespace std;

class Parser {
	friend class PatternMatcherTest;

public:
	static void Parse(string);
	static int compare(Token::Type, Token::Type);

protected:
	Parser(vector<Token>);

	deque<Token> tokens;
	int currentLineNumber;
	int currentProcNumber;
	static map<Token::Type, int> operatorPrecedenceMap;

	Token ConsumeTopToken();
	Token PeekAtTopToken();
	bool TopTokenIsType(Token::Type);
	void VerifyTopTokenIs(Token::Type);
	Token ConsumeTopTokenOfType(Token::Type);

	void Parse();

	TNode* ParseProcedure();
	TNode* ParseStmtList(string, TNode*);
	TNode* ParseStmt();
	TNode* ParseStmt(TNode*);
	TNode* ParseAssignmentStmt();
	TNode* ParseCallStmt();
	TNode* ParseIfStmt();
	TNode* ParseWhileStmt();
	TNode* ParseExpr(bool isBracket);
	TNode* ParseExpr(TNode*, bool);
	TNode* ParseAtomicToken();
	TNode* ParseConstTNode();
	TNode* ParseVariableTNode(bool isModifies);
};
