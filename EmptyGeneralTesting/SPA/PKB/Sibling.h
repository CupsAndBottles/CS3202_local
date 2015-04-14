#pragma once

#include <vector>

using namespace std;

class Sibling {
public:
	Sibling();

	// API 
	static bool IsSibling(int nodeOne, int nodeTwo);
	static vector<int> GetSiblings(int node);
};

