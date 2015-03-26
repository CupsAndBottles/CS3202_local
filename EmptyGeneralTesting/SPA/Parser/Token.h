#pragma once

#include <iostream>

using namespace std;

class Token {
public:
	static const string StartOfStmtList;
	static const string EndOfStmtList;
	static const string OpenBrace;
	static const string CloseBrace;
	static const string EndOfStmt;

	static const string OperatorPlus;
	static const string OperatorMinus;
	static const string OperatorMultiply;
	static const string OperatorAssign;

	static const string KeywordWhile;
	static const string KeywordProcedure;
	static const string KeywordCall;
	static const string KeywordIf;
	static const string KeywordThen;
	static const string KeywordElse;

	enum Type {
		START_OF_STMT_LIST = 0,
		END_OF_STMT_LIST,
		OPEN_BRACE,
		CLOSE_BRACE,
		END_OF_STMT,

		PLUS,
		MINUS,
		MULTIPLY,
		ASSIGN,

		WHILE,
		CALL,
		IF,
		THEN,
		ELSE,
		PROCEDURE,
		NUMBER,
		IDENTIFIER,
		NONTOKEN
	};


	Token& operator=(const Token& element);

	Token(string content, Type type);

	Type type;
	string content;

private:

};