#pragma once

#include "NodeTypeTable.h"
#include <vector>

using namespace std;

class Contains {
public:
	Contains();

	// General API
	// isContains
	// bool IsContains(int/nodetype, int/nodetype)
	static bool IsContains(int nodeContaining, int nodeContained);
	static bool IsContains(NodeType nodetypeContaining, NodeType nodetypeContained);
	static bool IsContains(int nodeContaining, NodeType nodetypeContained);
	static bool IsContains(NodeType nodetypeContaining, int nodeContained);

	// bool IsContainsVar(int/nodetype, int)
	static bool IsContainsVar(int nodeContaining, int varIndex);
	static bool IsContainsVar(NodeType nodetypeContaining, int varIndex);
	// bool IsContainsConst(int/nodetype, int)
	static bool IsContainsConst(int nodeContaining, int constant);
	static bool IsContainsConst(NodeType nodetypeContaining, int constant);

	// vector<int> GetNodesContained(int/nodetype)
	static vector<int> GetNodesContained(int nodeContaining);
	static vector<int> GetNodesContained(NodeType nodetypeContaining);
	// non-overloaded methods
	static int GetNodeContaining(int nodeContained);
	static vector<int> GetNodesContaining(NodeType nodetypeContained);
	static vector<int> GetNodesContainingVar(int varIndex);
	static vector<int> GetNodesContainingConst(int constant);


	// isContainsT
	// bool IsContainsT(int/nodetype, int/nodetype)
	static bool IsContainsT(int nodeContaining, int nodeContained);
	static bool IsContainsT(NodeType nodetypeContaining, NodeType nodetypeContained);
	static bool IsContainsT(int nodeContaining, NodeType nodetypeContained);
	static bool IsContainsT(NodeType nodetypeContaining, int nodeContained);

	// bool IsContainsTVar(int/nodetype, int)
	static bool IsContainsTVar(int nodeContaining, int varIndex);
	static bool IsContainsTVar(NodeType nodetypeContaining, int varIndex);
	// bool IsContainsTConst(int/nodetype, int)
	static bool IsContainsTConst(int nodeContaining, int constant);
	static bool IsContainsTConst(NodeType nodetypeContaining, int constant);

	// vector<int> GetNodesContainingT(int/nodetype)
	static vector<int> GetNodesContainingT(int nodeContained);
	static vector<int> GetNodesContainingT(NodeType nodetypeContained);
	// vector<int> GetNodesContainedT(int/nodetype)
	static vector<int> GetNodesContainedT(int nodeContaining);
	static vector<int> GetNodesContainedT(NodeType nodetypeContaining);
	// non-overloaded methods
	static vector<int> GetNodesContainingTVar(int varIndex);
	static vector<int> GetNodesContainingTConst(int constant);

	// helpers
	static vector<int> FilterNodesBy(vector<int> nodes, NodeType filterType);

	// specialisations, feel free to add your own or make them
	static bool IsStmtContains(int stmtNumber, NodeType nodetypeContained);
	static bool IsStmtContains(int stmtNumber, int nodeContained);
	static bool IsContainsStmt(NodeType nodetypeContaining, int stmtNumber);
	static bool IsContainsStmt(int nodeContained, int stmtNumber);

	static bool IsStmtContainsVar(int stmtNumber, int varIndex);
	static bool IsStmtContainsConst(int stmtNumber, int constant);

	static int GetNodeContainingStmt(int stmtNumber);
	static vector<int> GetNodesContainedStmt(int stmtNumber);

	static bool IsStmtContainsT(int stmtNumber, NodeType nodetypeContained);
	static bool IsStmtContainsT(int stmtNumber, int nodeContained);

	static bool IsContainsStmtT(NodeType nodetypeContaining, int stmtNumber);
	static bool IsContainsStmtT(int nodeContained, int stmtNumber);

	static bool IsStmtContainsTVar(int stmtNumber, int varIndex);
	static bool IsStmtContainsTConst(int stmtNumber, int constant);

	static vector<int> GetNodesContainingStmtT(int stmtNumber);
	static vector<int> GetNodesContainedStmtT(int stmtNumber);
};

