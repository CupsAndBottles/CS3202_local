#pragma once

#include "Tokenizer.h"
#include "Token.h"

#include <string>

using namespace std;

const char Tokenizer::COMMENT_DELINEATOR = '\\';

// splits a source string into tokens
// classifies tokens by type
// collapses integers e.g. 00123 => 123
vector<Token> Tokenizer::Tokenize(string source) {
	source.push_back('\n'); // append endl to source
	vector<Token> tokens;
	string integer = "";
	string alphaString = "";

	for (unsigned int pos = 0; pos < source.length(); pos++) { // loop through string
		char currentChar = source[pos];


		if (currentChar >= 48 && currentChar <= 57) { // integer
			if (alphaString == "") {
				integer.push_back(currentChar);
			} else {
				alphaString.push_back(currentChar);
			}
		} else if ((currentChar >= 65 && currentChar <= 90) ||
				   (currentChar >= 97 && currentChar <= 122)) { // alpha
			if (integer != "") {
				throw (string) "Invalid idenitifier.";
			} else {
				alphaString.push_back(currentChar);
			}

		} else { // symbol, whitespace or endline
			if (integer != "") { // previous substring is integer
				int numOfPrecedingZeroes = 0; // remove preceding zeroes

				while (integer[numOfPrecedingZeroes] == '0' && numOfPrecedingZeroes < (int)(integer.size() - 1)) {
					numOfPrecedingZeroes++;
				}
				integer.erase(0, numOfPrecedingZeroes);
				tokens.push_back(Token(integer, Token::NUMBER));
				integer = "";
			} else if (alphaString != "") { // previous substring is... string
				Token::Type type = StringToToken(alphaString);
				if (type == Token::NONTOKEN) {
					tokens.push_back(Token(alphaString, Token::IDENTIFIER));
				} else {
					tokens.push_back(Token(alphaString, type));
				}
				alphaString = "";
			}

			if (currentChar == '	' || currentChar == '\r' || currentChar == '\n' || currentChar == ' ') {
				// if space or tab or newline, skip
				continue;
			} else if (currentChar == COMMENT_DELINEATOR) {
				if (source[pos + 1] != COMMENT_DELINEATOR) { // double slashes is comment
					throw(string) "Syntax error.";
				} else {
					while (source[pos] != '\n') { // ignore chars until new line
						pos++;
					}
				}
			} else {
				string charString(1, currentChar);
				Token::Type type = StringToToken(charString);
				if (type == Token::NONTOKEN) {
					throw(string) "Invalid character encountered.";
				} else {
					tokens.push_back(Token(charString, type));
				}
			}
		}
	}

	return tokens;
}

Token::Type Tokenizer::StringToToken(string str) {
	if (str == Token::StartOfStmtList) {
		return Token::START_OF_STMT_LIST;
	} else if (str == Token::EndOfStmtList) {
		return Token::END_OF_STMT_LIST;
	} else if (str == Token::OpenBrace) {
		return Token::OPEN_BRACE;
	} else if (str == Token::CloseBrace) {
		return Token::CLOSE_BRACE;
	} else if (str == Token::EndOfStmt) {
		return Token::END_OF_STMT;

	} else if (str == Token::OperatorPlus) {
		return Token::PLUS;
	} else if (str == Token::OperatorMinus) {
		return Token::MINUS;
	} else if (str == Token::OperatorMultiply) {
		return Token::MULTIPLY;
	} else if (str == Token::OperatorAssign) {
		return Token::ASSIGN;

	} else if (str == Token::KeywordWhile) {
		return Token::WHILE;
	} else if (str == Token::KeywordProcedure) {
		return Token::PROCEDURE;
	} else if (str == Token::KeywordCall) {
		return Token::CALL;
	} else if (str == Token::KeywordIf) {
		return Token::IF;
	} else if (str == Token::KeywordThen) {
		return Token::THEN;
	} else if (str == Token::KeywordElse) {
		return Token::ELSE;
	} else {
		return Token::NONTOKEN;
	}
}