#include "Sibling.h"
#include "NodeTypeTable.h"

Sibling::Sibling() {
}

bool Sibling::IsSibling(int nodeOne, int nodeTwo) {
	if (nodeOne == nodeTwo) throw (string) "Nodes must be different!";
	TNode* parentNode = NodeTypeTable::GetPtrToNode(nodeOne)->GetDirectParent();
	if (parentNode != nullptr) {
		for each (TNode* child in parentNode->GetChildren()) {
			if (child->GetNodeIndex() == nodeTwo) {
				return true;
			}
		}
	}
	return false;
}

vector<int> Sibling::GetSiblings(int node) {
	vector<int> results;
	TNode* parentNode = NodeTypeTable::GetPtrToNode(node)->GetDirectParent();
	if (parentNode != nullptr) {
		for each (TNode* child in parentNode->GetChildren()) {
			int childNodeIndex = child->GetNodeIndex();
			if (childNodeIndex != node) {
				results.push_back(childNodeIndex);
			}
		}
	}

	return results;
}
