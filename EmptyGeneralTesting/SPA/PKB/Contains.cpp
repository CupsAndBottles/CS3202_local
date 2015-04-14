#include "Contains.h"
#include "NodeTypeTable.h"
#include "VarTable.h"
#include "..\Program\Program.h"

#include <algorithm>
#include <set>

Contains::Contains() {
}

bool Contains::IsContains(int nodeContaining, int nodeContained) {
	vector<TNode*> children = NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren();
	TNode* containedNode = NodeTypeTable::GetPtrToNode(nodeContained);

	return (find(children.begin(), children.end(), containedNode) != children.end());
}

bool Contains::IsContains(int nodeContaining, NodeType nodetypeContained) {
	for each (TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
		if (NodeTypeTable::GetNodeTypeOf(child->GetNodeIndex()) == nodetypeContained) {
			return true;
		}
	}
	return false;
}

bool Contains::IsContains(NodeType nodetypeContaining, NodeType nodetypeContained) {
	for each (int node in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		if (IsContains(node, nodetypeContained)) {
			return true;
		}
	}
	return false;
}

bool Contains::IsContains(NodeType nodetypeContaining, int nodeContained) {
	for each (int node in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		if (IsContains(node, nodeContained)) {
			return true;
		}
	}
	return false;
}


bool Contains::IsContainsVar(int nodeContaining, int varIndex) {
	for each (TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
		if (child->GetType() == VARIABLE) {
			if (VarTable::GetIndexOfVar(child->GetContent()) == varIndex) {
				return true;
			}
		}
	}
	return false;
}

bool Contains::IsContainsConst(int nodeContaining, int constant) {
	for each (TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
		if (child->GetType() == CONSTANT) {
			if (stoi(child->GetContent()) == constant) {
				return true;
			}
		}
	}
	return false;
}

bool Contains::IsContainsVar(NodeType nodetypeContaining, int varIndex) {
	for each (int nodeContaining in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		for each (TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
			if (child->GetType() == VARIABLE) {
				if (VarTable::GetIndexOfVar(child->GetContent()) == varIndex) {
					return true;
				}
			}
		}
	}

	return false;
}

bool Contains::IsContainsConst(NodeType nodetypeContaining, int constant) {
	for each (int nodeContaining in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		for each (TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
			if (child->GetType() == CONSTANT) {
				if (stoi(child->GetContent()) == constant) {
					return true;
				}
			}
		}
	}

	return false;
}

int Contains::GetNodeContaining(int nodeContained) {
	return NodeTypeTable::GetPtrToNode(nodeContained)->GetDirectParent()->GetNodeIndex();
}

vector<int> Contains::GetNodesContained(int nodeContaining) {
	vector<int> result;
	for each(TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
		result.push_back(child->GetNodeIndex());
	}
	return result;
}

vector<int> Contains::GetNodesContaining(NodeType nodetypeContained) {
	set<int> result;
	for each(int node in NodeTypeTable::GetAllNodesOfType(nodetypeContained)) {
		result.insert(NodeTypeTable::GetPtrToNode(node)->GetDirectParent()->GetNodeIndex());
	}
	return vector<int>(result.begin(), result.end());
}

vector<int> Contains::GetNodesContained(NodeType nodetypeContaining) {
	set<int> result;
	for each(int node in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		for each(TNode* child in NodeTypeTable::GetPtrToNode(node)->GetChildren()) {
			result.insert(child->GetNodeIndex());
		}
	}
	return vector<int>(result.begin(), result.end());
}

vector<int> Contains::GetNodesContainingVar(int varIndex) {
	set<int> result;
	for each(int node in NodeTypeTable::GetAllNodesOfVar(varIndex)) {
		result.insert(NodeTypeTable::GetPtrToNode(node)->GetDirectParent()->GetNodeIndex());
	}
	return vector<int>(result.begin(), result.end());
}

vector<int> Contains::GetNodesContainingConst(int constant) {
	set<int> result;
	for each(int node in NodeTypeTable::GetAllNodesOfVar(constant)) {
		result.insert(NodeTypeTable::GetPtrToNode(node)->GetDirectParent()->GetNodeIndex());
	}
	return vector<int>(result.begin(), result.end());
}

bool Contains::IsContainsT(int nodeContaining, int nodeContained) {
	if (IsContains(nodeContaining, nodeContained)) {
		return true;
	} else {
		for each (TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
			if (IsContainsT(child->GetNodeIndex(), nodeContained)) return true;
		}
		return false;
	}
}

bool Contains::IsContainsT(int nodeContaining, NodeType nodetypeContained) {
	if (IsContains(nodeContaining, nodetypeContained)) {
		return true;
	} else {
		for each (TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
			if (IsContainsT(child->GetNodeIndex(), nodetypeContained)) return true;
		}
		return false;
	}
}

bool Contains::IsContainsT(NodeType nodetypeContaining, NodeType nodetypeContained) {
	for each (int node in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		if (IsContainsT(node, nodetypeContained)) {
			return true;
		}
	}
	return false;
}

bool Contains::IsContainsT(NodeType nodetypeContaining, int nodeContained) {
	for each (int node in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		if (IsContainsT(node, nodeContained)) {
			return true;
		}
	}
	return false;
}

bool Contains::IsContainsTVar(int nodeContaining, int varIndex) {
	if (IsContainsVar(nodeContaining, varIndex)) {
		return true;
	} else {
		for each (TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
			if (IsContainsTVar(child->GetNodeIndex(), varIndex)) return true;
		}
		return false;
	}
}

bool Contains::IsContainsTVar(NodeType nodetypeContaining, int varIndex) {
	for each (int node in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		if (IsContainsTVar(node, varIndex)) {
			return true;
		}
	}
	return false;
}

bool Contains::IsContainsTConst(int nodeContaining, int constant) {
	if (IsContainsVar(nodeContaining, constant)) {
		return true;
	} else {
		for each (TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
			if (IsContainsTConst(child->GetNodeIndex(), constant)) return true;
		}
		return false;
	}
}

bool Contains::IsContainsTConst(NodeType nodetypeContaining, int constant) {
	for each (int node in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		if (IsContainsTConst(node, constant)) {
			return true;
		}
	}
	return false;
}

vector<int> Contains::GetNodesContainingT(int nodeContained) {
	vector<int> result;
	TNode* parent = NodeTypeTable::GetPtrToNode(nodeContained)->GetDirectParent();
	if (parent != nullptr) {
		result.push_back(parent->GetNodeIndex());
		vector<int> containing = GetNodesContainingT(parent->GetNodeIndex());
		result.insert(result.end(), containing.begin(), containing.end());
	}
	return result;
}

vector<int> Contains::GetNodesContainedT(int nodeContaining) {
	vector<int> result;
	for each(TNode* child in NodeTypeTable::GetPtrToNode(nodeContaining)->GetChildren()) {
		int childNodeIndex = child->GetNodeIndex();
		result.push_back(childNodeIndex);
		vector<int> childResults = GetNodesContainedT(childNodeIndex);
		result.insert(result.end(), childResults.begin(), childResults.end());
	}

	return result;
}

vector<int> Contains::GetNodesContainingT(NodeType nodetypeContained) {
	set<int> result;
	for each (int node in NodeTypeTable::GetAllNodesOfType(nodetypeContained)){
		vector<int> intermediateResult = GetNodesContainingT(node);
		result.insert(intermediateResult.begin(), intermediateResult.end());
	}
	return vector<int>(result.begin(), result.end());
}

vector<int> Contains::GetNodesContainedT(NodeType nodetypeContaining) {
	set<int> result;
	for each (int node in NodeTypeTable::GetAllNodesOfType(nodetypeContaining)) {
		vector<int> intermediateResult = GetNodesContainedT(node);
		result.insert(intermediateResult.begin(), intermediateResult.end());
	}
	return vector<int>(result.begin(), result.end());
}

vector<int> Contains::GetNodesContainingTVar(int varIndex) {
	set<int> result;
	for each (int node in NodeTypeTable::GetAllNodesOfVar(varIndex)) {
		vector<int> intermediateResult = GetNodesContainingT(node);
		result.insert(intermediateResult.begin(), intermediateResult.end());
	}
	return vector<int>(result.begin(), result.end());
}

vector<int> Contains::GetNodesContainingTConst(int constant) {
	set<int> result;
	for each (int node in NodeTypeTable::GetAllNodesOfConst(constant)) {
		vector<int> intermediateResult = GetNodesContainingT(node);
		result.insert(intermediateResult.begin(), intermediateResult.end());
	}
	return vector<int>(result.begin(), result.end());
}

// helpers
vector<int> Contains::FilterNodesBy(vector<int> nodes, NodeType filterType) {
	vector<int> results;
	for each (int node in nodes) {
		if (NodeTypeTable::CheckIfNodeOfType(node, filterType)) results.push_back(node);
	}
	return results;
}


// specialisations, feel free to add your own or make them

bool Contains::IsStmtContains(int stmtNumber, NodeType nodetypeContained){
	return IsContains(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex(), nodetypeContained);
}

bool Contains::IsStmtContains(int stmtNumber, int nodeContained) {
	return IsContains(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex(), nodeContained);
}

bool Contains::IsContainsStmt(NodeType nodetypeContaining, int stmtNumber) {
	return IsContains(nodetypeContaining, Program::GetStmtFromNumber(stmtNumber).GetNodeIndex());
}

bool Contains::IsContainsStmt(int nodeContained, int stmtNumber) {
	return IsContains(nodeContained, Program::GetStmtFromNumber(stmtNumber).GetNodeIndex());
}

bool Contains::IsStmtContainsVar(int stmtNumber, int varIndex) {
	return IsContainsVar(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex(), varIndex);
}

bool Contains::IsStmtContainsConst(int stmtNumber, int constant) {
	return IsContainsConst(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex(), constant);
}

int Contains::GetNodeContainingStmt(int stmtNumber){
	return GetNodeContaining(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex());
}

vector<int> Contains::GetNodesContainedStmt(int stmtNumber){
	return GetNodesContained(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex());
}

bool Contains::IsStmtContainsT(int stmtNumber, NodeType nodetypeContained){
	return IsContainsT(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex(), nodetypeContained);
}

bool Contains::IsStmtContainsT(int stmtNumber, int nodeContained) {
	return IsContainsT(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex(), nodeContained);
}

bool Contains::IsContainsStmtT(NodeType nodetypeContaining, int stmtNumber){
	return IsContainsT(nodetypeContaining, Program::GetStmtFromNumber(stmtNumber).GetNodeIndex());
}

bool Contains::IsContainsStmtT(int nodeContained, int stmtNumber) {
	return IsContainsT(nodeContained, Program::GetStmtFromNumber(stmtNumber).GetNodeIndex());
}

bool Contains::IsStmtContainsTVar(int stmtNumber, int varIndex) {
	return IsContainsTVar(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex(), varIndex);
}

bool Contains::IsStmtContainsTConst(int stmtNumber, int constant) {
	return IsContainsTConst(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex(), constant);
}

vector<int> Contains::GetNodesContainingStmtT(int stmtNumber) {
	return GetNodesContainingT(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex());
}

vector<int> Contains::GetNodesContainedStmtT(int stmtNumber) {
	return GetNodesContainedT(Program::GetStmtFromNumber(stmtNumber).GetNodeIndex());
}

