#pragma once

#include "..\Program\TNode\TNode.h"
#include "..\QueryProcessor\grammar.h"

#include <vector>
#include <map>

using namespace std;


class NodeTypeTable {
public:
	NodeTypeTable();

	static vector<NodeType> iterableEnumSet;

	//API-Query
	static vector<int> GetAllNodesOfType(NodeType type);
	static vector<int> GetAllNodesOfVar(int varTableIndex);
	static vector<int> GetAllNodesOfConst(int constTableIndex);
	static NodeType GetNodeTypeOf(int nodeIndex);
	static bool CheckIfNodeOfType(int nodeIndex, NodeType type);
	static bool CheckIfNodeOfVar(int nodeIndex, int varTableIndex);
	static bool CheckIfNodeOfConst(int nodeIndex, int constTableIndex);
	static TNode* GetPtrToNode(int nodeIndex);

	//API-PKB and DE
	static void Insert(TNode* nodeptr); // automagically filter the var and const types

	static int GetNoOfNodes();
	static int GetNoOfNodesOfType(NodeType type);
	static int GetMaxNodeIndex();

	static void ClearData();
	static void Initialise();

private:
	static map<NodeType, vector<int>> nodeTable;
	static map<int, vector<int>> varNodeTable;
	static map<int, vector<int>> constNodeTable;
	static map<int, TNode*> nodeIndexToPtrMap;
};

