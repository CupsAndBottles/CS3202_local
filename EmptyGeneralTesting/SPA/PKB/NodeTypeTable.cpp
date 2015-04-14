#include "NodeTypeTable.h"
#include "VarTable.h"
#include "ConstTable.h"

map<NodeType, vector<int>> NodeTypeTable::nodeTable;
map<int, vector<int>> NodeTypeTable::varNodeTable;
map<int, vector<int>> NodeTypeTable::constNodeTable;
map<int, TNode*> NodeTypeTable::nodeIndexToPtrMap;
vector<NodeType> NodeTypeTable::iterableEnumSet;

void initialiseEnumSet() {
	NodeTypeTable::iterableEnumSet.clear();
	NodeTypeTable::iterableEnumSet.push_back(ASSIGN_NODE);
	NodeTypeTable::iterableEnumSet.push_back(WHILE_NODE);
	NodeTypeTable::iterableEnumSet.push_back(WHILEBODY_NODE);
	NodeTypeTable::iterableEnumSet.push_back(IF_NODE);
	NodeTypeTable::iterableEnumSet.push_back(THENBODY_NODE);
	NodeTypeTable::iterableEnumSet.push_back(ELSEBODY_NODE);
	NodeTypeTable::iterableEnumSet.push_back(VARIABLE_NODE);
	NodeTypeTable::iterableEnumSet.push_back(CONSTANT_NODE);
	NodeTypeTable::iterableEnumSet.push_back(PLUS_NODE);
	NodeTypeTable::iterableEnumSet.push_back(MINUS_NODE);
	NodeTypeTable::iterableEnumSet.push_back(MULTIPLY_NODE);
	NodeTypeTable::iterableEnumSet.push_back(PROCEDURE_NODE);
	NodeTypeTable::iterableEnumSet.push_back(PROGRAM_NODE);
	NodeTypeTable::iterableEnumSet.push_back(CALL_NODE);
	NodeTypeTable::iterableEnumSet.push_back(PROCEDUREBODY_NODE);
}

NodeTypeTable::NodeTypeTable() {
	ClearData();
	initialiseEnumSet();
}

void NodeTypeTable::Insert(TNode* nodeptr) {
	int nodeIndex = nodeptr->GetNodeIndex();
	nodeIndexToPtrMap[nodeIndex] = nodeptr;
	NodeType type = GetNodeTypeOf(nodeIndex);
	nodeTable[type].push_back(nodeIndex);
	if (type == VARIABLE_NODE) {
		int varIndex = VarTable::GetIndexOfVar(nodeptr->GetContent());
		varNodeTable[varIndex].push_back(nodeIndex);
	} else if (type == CONSTANT_NODE) {
		int constant = stoi(nodeptr->GetContent());
		constNodeTable[constant].push_back(nodeIndex);
	}
}

vector<int> NodeTypeTable::GetAllNodesOfType(NodeType type) {
	return nodeTable[type];
}

vector<int> NodeTypeTable::GetAllNodesOfVar(int varTableIndex) {
	return varNodeTable[varTableIndex];
}

vector<int> NodeTypeTable::GetAllNodesOfConst(int constTableIndex) {
	return constNodeTable[constTableIndex];
}

NodeType NodeTypeTable::GetNodeTypeOf(int nodeIndex) {
	TNode* nodeptr = GetPtrToNode(nodeIndex);
	TNode::Type nodeType = nodeptr->GetType();
	switch (nodeType) {
		case TNode::ASSIGNMENT:
			return ASSIGN_NODE;
		case TNode::CALL:
			return CALL_NODE;
		case TNode::CONSTANT:
			return CONSTANT_NODE;
		case TNode::IF:
			return IF_NODE;
		case TNode::OPERATOR:
			if (nodeptr->GetContent() == "+") {
				return PLUS_NODE;
			} else if (nodeptr->GetContent() == "-") {
				return MINUS_NODE;
			} else if (nodeptr->GetContent() == "*") {
				return MULTIPLY_NODE;
			} else {
				// error, fallthrough
			}
		case TNode::PROCEDURE:
			return PROCEDURE_NODE;
		case TNode::PROGRAM:
			return PROGRAM_NODE;
		case TNode::STMT_LIST:
			if (nodeptr->GetDirectParent()->GetType() == TNode::PROCEDURE) {
				return PROCEDUREBODY_NODE;
			} else if (nodeptr->GetDirectParent()->GetType() == TNode::IF) {
				if (nodeptr->GetName().substr(0, 4) == "then") {
					return THENBODY_NODE;
				} else if (nodeptr->GetName().substr(0, 4) == "else") {
					return ELSEBODY_NODE;
				} else {
					// error, fallthrough
				}
			} else if (nodeptr->GetDirectParent()->GetType() == TNode::WHILE) {
				return WHILEBODY_NODE;
			} else {
				// error, fallthrough
			}
		case TNode::VARIABLE:
			return VARIABLE_NODE;
		case TNode::WHILE:
			return WHILE_NODE;
		default:
			// never will come here
			;
	}
}

bool NodeTypeTable::CheckIfNodeOfType(int nodeIndex, NodeType type) {
	return (GetNodeTypeOf(nodeIndex) == type);
}

TNode* NodeTypeTable::GetPtrToNode(int nodeIndex) {
	return nodeIndexToPtrMap[nodeIndex];
}

int NodeTypeTable::GetNoOfNodes() {
	return nodeIndexToPtrMap.size();
}

int NodeTypeTable::GetNoOfNodesOfType(NodeType type) {
	return nodeTable[type].size();
}

int NodeTypeTable::GetMaxNodeIndex() {
	return GetNoOfNodes() - 1;
}


void NodeTypeTable::ClearData() {
	nodeTable.clear();
	varNodeTable.clear();
	constNodeTable.clear();
	nodeIndexToPtrMap.clear();
}

void NodeTypeTable::Initialise() {
	// only use this method after parse complete
	for each (NodeType nodetype in iterableEnumSet) {
		nodeTable[nodetype] = vector<int>();
	}

	int noOfVars = VarTable::GetNoOfVars();
	for (int i = 0; i < noOfVars; i++) {
		varNodeTable[i] = vector<int>();
	}

	for each (int constant in ConstTable::GetAllConst()) {
		varNodeTable[constant] = vector<int>();
	}


}