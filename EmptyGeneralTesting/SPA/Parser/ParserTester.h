#pragma once

#include <iostream>

using namespace std;

class ParserTester {
public:
	string directoryName;
	ParserTester(string directoryName);
	void ParseSource(string filename);
};

