#pragma once

#include "Parser.h"
#include "Tokenizer.h"
#include "DesignExtractor.h"
#include "..\Program\Program.h"
#include "..\Program\TNode\TNode.h"
#include "..\QueryProcessor\Grammar.h"
#include "..\PKB\Follows.h"
#include "..\PKB\Modifies.h"
#include "..\PKB\Uses.h"
#include "..\PKB\Parent.h"
#include "..\PKB\Calls.h"
#include "..\PKB\StmtTypeTable.h"
#include "..\PKB\VarTable.h"
#include "..\PKB\ConstTable.h"
#include "..\PKB\ProcTable.h"
#include "..\..\AutoTester\source\AbstractWrapper.h"
#include "..\QueryProcessor\Grammar.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

static map<Token::Type, int> CreateMap() {
	map<Token::Type, int> result;

	result[Token::PLUS]			= 1;
	result[Token::MINUS]		= 1;
	result[Token::MULTIPLY]		= 2;
	result[Token::END_OF_STMT]	= -1;
	result[Token::CLOSE_BRACE]	= -1;

	return result;
}

map<Token::Type, int> Parser::operatorPrecedenceMap = CreateMap();

void SyntaxError() {
	throw (string) "Syntax error!";
}

TNode* ConstructIfTNode(int lineNumber) {
	return new TNode(TNode::ConstructIfTNode(lineNumber));
}

TNode* ConstructWhileTNode(int lineNumber) {
	return new TNode(TNode::ConstructWhileTNode(lineNumber));
}

TNode* ConstructCallTNode(int lineNumber, string procName) {
	return new TNode(TNode::ConstructCallTNode(lineNumber, procName));
}

TNode* ConstructAssignmentTNode(int lineNumber) {
	return new TNode(TNode::ConstructAssignmentTNode(lineNumber));
}

TNode* ConstructConstTNode(string value) {
	return new TNode(TNode::ConstructConstTNode(value));
}

TNode* ConstructVarTNode(string symbol) {
	return new TNode(TNode::ConstructVarTNode(symbol));
}

TNode* ConstructOperatorTNode(string symbol) {
	return new TNode(TNode::ConstructOperatorTNode(symbol));
}

TNode* ConstructStmtListTNode(string name) {
	return new TNode(TNode::ConstructStmtListTNode(name));
}

TNode* ConstructProcedureTNode(string name) {
	return new TNode(TNode::ConstructProcedureTNode(name));
}

TNode* ConstructStmtTNode(TNode::Type type, int lineNumber) {
	return new TNode(TNode::ConstructStmtTNode(type, lineNumber));
}

Parser::Parser(vector<Token> tokenVector)
	: tokens(tokenVector.begin(), tokenVector.end())
	, currentLineNumber(0) 
	, currentProcNumber(0) {
}

void Parser::Parse(string fileName) {
	ifstream sourceFile(fileName);
	stringstream buffer;
	buffer << sourceFile.rdbuf();
	sourceFile.close();

	TNode::resetNodeCounter();

	vector<Token> tokens = Tokenizer::Tokenize(buffer.str());
	Parser parser(tokens);
	parser.Parse();
}

Token Parser::ConsumeTopToken() {
	Token token = tokens.front();
	tokens.pop_front();
	return token;
}

Token Parser::PeekAtTopToken() {
	return tokens.front();
}

bool Parser::TopTokenIsType(Token::Type type) {
	return (tokens.front().type == type);
}

void Parser::VerifyTopTokenIs(Token::Type type) {
	if (!TopTokenIsType(type)) {
		SyntaxError();
	}
}

Token Parser::ConsumeTopTokenOfType(Token::Type type) {
	// verifies that top token is of given type
	// then consumes it
	VerifyTopTokenIs(type);
	return ConsumeTopToken();
}

int Parser::compare(Token::Type first, Token::Type second) {
	int valFirst = operatorPrecedenceMap[first];
	int valSecond = operatorPrecedenceMap[second];
	return (valFirst - valSecond);
}

void Parser::Parse() {
	TNode& rootNode = Program::GetASTRootNode();

	while (tokens.size() != 0) {
		// stop point
		if (AbstractWrapper::GlobalStop) {
			break;
		}
		TNode* procedureNode = ParseProcedure();
		
		rootNode.AddChild(procedureNode);
	}

	// post processing step
	DesignExtractor::Extract();
}

TNode* Parser::ParseProcedure() {
		ConsumeTopTokenOfType(Token::PROCEDURE);
		string procedureName = ConsumeTopTokenOfType(Token::IDENTIFIER).content;
		TNode* procedureNode = ConstructProcedureTNode(procedureName);

		currentProcNumber = ProcTable::InsertProc(procedureName);

		ProcTable::SetFirstStmtNoOfProc(currentProcNumber, currentLineNumber + 1);

		TNode* procedureBody = ParseStmtList("", nullptr);
		procedureNode->AddChild(procedureBody);

		ProcTable::SetLastStmtNoOfProc(currentProcNumber, currentLineNumber);
		return procedureNode;
}

TNode* Parser::ParseStmtList(string name, TNode* parent) {
	// first token should be start of stmt list token
	// consumes tokens until a matching close brace is found
	// if tokens are fully consumed, throw exception
	
	ConsumeTopTokenOfType(Token::START_OF_STMT_LIST);

	TNode* stmtListNode = ConstructStmtListTNode("");
	TNode* prevStmt = nullptr;
	while (!TopTokenIsType(Token::END_OF_STMT_LIST)) {
		// stop point
		if (AbstractWrapper::GlobalStop) {
			return stmtListNode;
		}

		TNode* stmt = ParseStmt(parent);
		stmtListNode->AddChild(stmt);
		if (prevStmt != nullptr) {
			Follows::SetFollows(prevStmt->GetLineNumber(), stmt->GetLineNumber());
		}
		prevStmt = stmt;
	}

	ConsumeTopTokenOfType(Token::END_OF_STMT_LIST);

	return stmtListNode;
}

TNode* Parser::ParseStmt(TNode* parentStmt) {
	Token firstToken = PeekAtTopToken(); // peek at next token
	TNode* stmt;
	currentLineNumber++;

	if (parentStmt != nullptr) {
		Parent::SetParent(parentStmt->GetLineNumber(), currentLineNumber);
	}

	switch (firstToken.type) {
		case Token::IDENTIFIER:
			stmt = ParseAssignmentStmt();
			StmtTypeTable::Insert(stmt->GetLineNumber(), ASSIGN);
			break;
		case Token::WHILE:
			stmt = ParseWhileStmt();
			StmtTypeTable::Insert(stmt->GetLineNumber(), WHILE);
			break;
		case Token::CALL:
			stmt = ParseCallStmt();
			StmtTypeTable::Insert(stmt->GetLineNumber(), CALL);
			break;
		case Token::IF:
			stmt = ParseIfStmt();
			StmtTypeTable::Insert(stmt->GetLineNumber(), IF);
			break;
		default:
			SyntaxError();
	}

	if (parentStmt != nullptr) {
		stmt->SetLogicalParent(parentStmt);
	}

	Program::InsertStmt(stmt, stmt->GetLineNumber());
	StmtTypeTable::Insert(stmt->GetLineNumber(), STMT);
	return stmt;
}

TNode* Parser::ParseAssignmentStmt() {
	TNode* LHS = ParseVariableTNode(true);
	ConsumeTopTokenOfType(Token::ASSIGN);

	TNode* assignmentStmt = ConstructAssignmentTNode(currentLineNumber);
	TNode* RHS = ParseExpr(false);

	assignmentStmt->AddChild(LHS);
	assignmentStmt->AddChild(RHS);

	return assignmentStmt;
}

TNode* Parser::ParseExpr(bool isBracket) {
	Token::Type terminatingCondition = isBracket ? Token::CLOSE_BRACE : Token::END_OF_STMT;
	TNode* result = nullptr;
	while (!TopTokenIsType(terminatingCondition)) {
		if (result == nullptr) {
			if (TopTokenIsType(Token::OPEN_BRACE)) {
				ConsumeTopTokenOfType(Token::OPEN_BRACE);
				result = ParseExpr(true);
			} else {
				result = ParseAtomicToken();
			}
		} else {
			result = ParseExpr(result, isBracket);
		}
	}
	ConsumeTopTokenOfType(terminatingCondition);
	return result;
}

TNode* Parser::ParseExpr(TNode* LHS, bool isBracket) {
	// if next op is of lower precedence, construct RHS and return
	// if next op is of equal precedence, construct and loop
	// if next op is of higher precedence, perform recursive call
	// since all operations are left associative, no attempt to worry about
	//		assoiciativity is made

	Token::Type terminatingCondition = isBracket ? Token::CLOSE_BRACE : Token::END_OF_STMT;

	if (PeekAtTopToken().type == terminatingCondition) {
		return LHS;
	}
	
	// TODO combine operator token types into one type for type checking.
	Token op1 = ConsumeTopToken();
	TNode* RHS;
	if (TopTokenIsType(Token::OPEN_BRACE)) {
		ConsumeTopTokenOfType(Token::OPEN_BRACE);
		RHS = ParseExpr(true);
	} else {
		RHS = ParseAtomicToken();
	}
	
	Token nextOp = PeekAtTopToken(); // peek
	int comparison = compare(op1.type, nextOp.type);

	if (comparison < 0) { // nextOp is of lower precedence than currentOp
		RHS = ParseExpr(RHS, isBracket);
	}
	TNode* expression = ConstructOperatorTNode(op1.content);
	expression->AddChild(LHS);
	expression->AddChild(RHS);
	if (comparison > 0) { // nextOp is of higher precedence than currentOp
		return expression;
	} else { // equal precedence
		return ParseExpr(expression, isBracket);
	}
}

TNode* Parser::ParseAtomicToken() {
	Token currentToken = PeekAtTopToken(); // peek
	switch (currentToken.type) {
		case Token::IDENTIFIER:
			return ParseVariableTNode(false);
		case Token::NUMBER:
			return ParseConstTNode();
		default:
			SyntaxError();
	}
}

TNode* Parser::ParseConstTNode() {
	string constant = ConsumeTopTokenOfType(Token::NUMBER).content;
	TNode* result = ConstructConstTNode(constant);
	ConstTable::SetStmtUsesConst(currentLineNumber, stoi(constant));
	return result;
}

TNode* Parser::ParseVariableTNode(bool isModifies) {
	string variable = ConsumeTopTokenOfType(Token::IDENTIFIER).content;
	TNode* result = ConstructVarTNode(variable);
	int varIndex = VarTable::InsertVar(variable);
	if (isModifies) {
		Modifies::SetStmtModifiesVar(currentLineNumber, varIndex);
		Modifies::SetProcModifiesVar(currentProcNumber, varIndex);
	} else {
		Uses::SetStmtUsesVar(currentLineNumber, varIndex);
		Uses::SetProcUsesVar(currentProcNumber, varIndex);
	}
	return result;
}

TNode* Parser::ParseWhileStmt() {
	ConsumeTopTokenOfType(Token::WHILE);

	TNode* whileStmt = ConstructWhileTNode(currentLineNumber);

	// parse condition
	TNode* conditionNode = ParseVariableTNode(false);

	// parse loop body
	TNode* loopBody = ParseStmtList("", whileStmt);

	// create while statement
	whileStmt->AddChild(conditionNode);
	whileStmt->AddChild(loopBody);
	return whileStmt;
}

TNode* Parser::ParseCallStmt() {
	ConsumeTopTokenOfType(Token::CALL);
	string procedure = ConsumeTopTokenOfType(Token::IDENTIFIER).content;
	TNode* callNode = ConstructCallTNode(currentLineNumber, procedure);
	ConsumeTopTokenOfType(Token::END_OF_STMT);
	return callNode;
}

TNode* Parser::ParseIfStmt() {
	ConsumeTopTokenOfType(Token::IF);

	TNode* ifStmt = ConstructIfTNode(currentLineNumber);

	// parse condition
	TNode* conditionNode = ParseVariableTNode(false);

	// parse then body
	ConsumeTopTokenOfType(Token::THEN);
	TNode* thenBody = ParseStmtList("then", ifStmt);
	
	// parse else body
	ConsumeTopTokenOfType(Token::ELSE);
	TNode* elseBody = ParseStmtList("else", ifStmt);

	// create if statement
	ifStmt->AddChild(conditionNode);
	ifStmt->AddChild(thenBody);
	ifStmt->AddChild(elseBody);
	return ifStmt;
}