#pragma once

#include "Token.h"

#include <iostream>

using namespace std;

const string Token::StartOfStmtList		= "{";
const string Token::EndOfStmtList		= "}";
const string Token::OpenBrace			= "(";
const string Token::CloseBrace			= ")";
const string Token::EndOfStmt			= ";";

const string Token::OperatorPlus		= "+";
const string Token::OperatorMinus		= "-";
const string Token::OperatorMultiply	= "*";
const string Token::OperatorAssign		= "=";

const string Token::KeywordWhile		= "while";
const string Token::KeywordProcedure	= "procedure";
const string Token::KeywordCall			= "call";
const string Token::KeywordIf			= "if";
const string Token::KeywordThen			= "then";
const string Token::KeywordElse			= "else";

Token& Token::operator=(const Token& element) {
	content = element.content;
	type = element.type;
	return *this;
}

Token::Token(string content, Type type) : content(content), type(type) {}