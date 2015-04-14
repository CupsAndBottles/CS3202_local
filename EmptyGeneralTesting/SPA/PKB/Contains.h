#pragma once

#include "NodeTypeTable.h"
#include <vector>

using namespace std;

class Contains {
public:
	Contains();

	// General API
	bool IsContains(int nodeContaining, int nodeContained);
	bool IsContains(NodeType nodetypeContaining, NodeType nodetypeContained);
	bool IsContains(int nodeContaining, NodeType nodetypeContained);
	bool IsContains(NodeType nodetypeContaining, int nodeContained);

	vector<int> GetNodesContaining(int nodeContained);
	vector<int> GetNodesContained(int nodeContaining);
	vector<int> GetNodesContaining(NodeType nodetypeContained);
	vector<int> GetNodesContained(NodeType nodetypeContaining);


	bool IsContainsT(int nodeContaining, int nodeContained);
	bool IsContainsT(NodeType nodetypeContaining, NodeType nodetypeContained);
	bool IsContainsT(int nodeContaining, NodeType nodetypeContained);
	bool IsContainsT(NodeType nodetypeContaining, int nodeContained);

	vector<int> GetNodesContainingT(int nodeContained);
	vector<int> GetNodesContainedT(int nodeContaining);
	vector<int> GetNodesContainingT(NodeType nodetypeContained);
	vector<int> GetNodesContainedT(NodeType nodetypeContaining);

	// helpers
	vector<int> FilterNodesBy(vector<int> nodes, NodeType filterType);
	bool DoesNodeNumberExistIn(vector<int> nodes, int nodeIndex);

	// specialisations, feel free to add your own or make them
	bool IsContainsStmt(NodeType nodetypeContaining, int stmtNumber);
	bool IsStmtContains(int stmtNumber, NodeType nodetypeContained);

};

