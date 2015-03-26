#pragma once

#include "Token.h"

#include <vector>

class Tokenizer {
public:
	static const char COMMENT_DELINEATOR;
	static vector<Token> Tokenize(string);
private:
	static Token::Type StringToToken(string);
};