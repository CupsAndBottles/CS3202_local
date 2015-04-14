#include "ParserTester.h"

#include "parser.h"

ParserTester::ParserTester(string directoryName) : directoryName(directoryName) {}

void ParserTester::ParseSource(string filename) {
	Parser::Parse(string(directoryName).append(filename));
}
