#include <algorithm>
#include <utility>
#include <map>
#include <vector>
#include <iostream>

#include "Parent.h"
#include "StmtTypeTable.h"

map <int, vector<int>> Parent::parentToChildrenTable;
map <int, int> Parent::childToParentTable;
vector<vector<bool>> Parent::parentToChildrenTBV;
vector<vector<int>> Parent::parentToChildrenTTable;
vector<vector<int>> Parent::childToParentTTable;

/** public methods **/
Parent::Parent() {
	// empty constructor

}

// Methods
void Parent::SetParent(int parentStmtIndex, int childStmtIndex) {
	if (HasNoParent(childStmtIndex) && !AlreadyInserted(parentStmtIndex, childStmtIndex) ) {
		parentToChildrenTable[parentStmtIndex].push_back(childStmtIndex);
		childToParentTable[childStmtIndex] = parentStmtIndex;
	}
}

bool Parent::IsParent(int parentStmtIndex, int childStmtIndex) {
	return AlreadyInserted(parentStmtIndex, childStmtIndex);

}

int Parent::GetParentOf(int childStmtIndex) {
	if (!HasNoParent(childStmtIndex)) {
		return childToParentTable[childStmtIndex];
	} else {
		return -1;
	}
}

vector<int> Parent::GetChildrenOf(int parentStmtIndex) {
	return parentToChildrenTable[parentStmtIndex];
}

bool Parent::IsParentT(int parentStmtIndex, int childStmtIndex) {
	int currChild = childStmtIndex;

	while (!HasNoParent(currChild)) {
		if (childToParentTable[currChild] == parentStmtIndex) {
			return true;

		}

		currChild = childToParentTable[currChild];

	}

	return false;

}

bool Parent::IsParentTBV(int parentStmtIndex, int childStmtIndex) {
	int totalNoOfStmts = StmtTypeTable::GetMaxStmtIndex();

	if (parentStmtIndex >= 0 && parentStmtIndex <= totalNoOfStmts
		&& childStmtIndex >= 0 && childStmtIndex <= totalNoOfStmts) {
			return parentToChildrenTBV.at(parentStmtIndex).at(childStmtIndex);
	} else {
		return false;
	}

}

vector<int> Parent::GetParentTOf(int childStmtIndex) {
	vector<int> parentList;

	int currParent = GetParentOf(childStmtIndex);

	while (currParent != -1) {
		parentList.push_back(currParent);

		childStmtIndex = currParent;
		currParent = GetParentOf(childStmtIndex);
		
	}

	return parentList;
}

vector<int> Parent::GetStoredParentTOf(int childStmtIndex) {
	int maxStmtIndex = StmtTypeTable::GetMaxStmtIndex();

	if (childStmtIndex >= 0 && childStmtIndex <= maxStmtIndex) {
		if ((int) childToParentTTable.size() >= childStmtIndex) {
			return childToParentTTable.at(childStmtIndex);
		} else {
			cout << "\nwarning: unable to find " << childStmtIndex << " in childToParentTTable.\n";
			return GetParentTOf(childStmtIndex);
		}
	}

	vector<int> emptyList;
	return emptyList;

}

vector<int> Parent::GetChildrenTOf(int parentStmtIndex) {
	vector<int> allDescendants, 
				childrenOfCurrNode, grandChildrenOfCurrChild;

	// add this's children
	childrenOfCurrNode = GetChildrenOf(parentStmtIndex);
	allDescendants.insert(allDescendants.end(), childrenOfCurrNode.begin(), childrenOfCurrNode.end());

	// add this's children's children
	// only enter for loop if parentStmtIndex has at least 1 child
	for (unsigned int i = 0; i < childrenOfCurrNode.size(); i++) {
		grandChildrenOfCurrChild = GetChildrenTOf(childrenOfCurrNode.at(i));
		allDescendants.insert(allDescendants.end(), grandChildrenOfCurrChild.begin(), grandChildrenOfCurrChild.end());

	}
	
	return allDescendants;

	// each recursive iteration inserts the subtree whose root is the currNode into allDescendants

}

vector<int> Parent::GetStoredChildrenTOf(int parentStmtIndex) {
	vector<int> childrenList;
	int maxStmtIndex = StmtTypeTable::GetMaxStmtIndex();

	if (parentStmtIndex >= 0 && parentStmtIndex <= maxStmtIndex) {
		if ((int) parentToChildrenTTable.size() >= 0) {
			return parentToChildrenTTable.at(parentStmtIndex);
		} else {
			cout << "\nwarning: unable to find " << parentStmtIndex << " in parentToChildrenTTable.\n";
			return GetChildrenTOf(parentStmtIndex);
		}
	}

	return childrenList;


}

void Parent::CreateParentToChildrenTBV() {
	int totalNoOfStmts = StmtTypeTable::GetMaxStmtIndex();
	vector<bool> emptyRow(totalNoOfStmts + 1, false);
	parentToChildrenTBV = vector<vector<bool>>(totalNoOfStmts + 1, emptyRow);

	vector<int> allStmts = StmtTypeTable::GetAllStmtsOfType(STMT);
	vector<int> childrenT;
	for (vector<int>::iterator it1 = allStmts.begin(); it1 != allStmts.end(); it1++) {
		childrenT = GetChildrenTOf(*it1);

		for (vector<int>::iterator it2 = childrenT.begin(); it2 != childrenT.end(); it2++) {
			parentToChildrenTBV.at(*it1).at(*it2) = true;

		}
	
	}

}

void Parent::CreateParentToChildrenTTable() {
	int totalNoOfStmts = StmtTypeTable::GetMaxStmtIndex();
	vector<int> emptyRow;
	parentToChildrenTTable = vector<vector<int>>(totalNoOfStmts + 1, emptyRow);

	vector<int> allStmts = StmtTypeTable::GetAllStmtsOfType(STMT);
	vector<int> childrenT;
	for (vector<int>::iterator it = allStmts.begin(); it != allStmts.end(); it++) {
		childrenT = GetChildrenTOf(*it);
		parentToChildrenTTable.at(*it) = childrenT;
	
	}

}

void Parent::CreateChildrenToParentTTable() {
	int totalNoOfStmts = StmtTypeTable::GetMaxStmtIndex();
	vector<int> emptyRow;
	childToParentTTable = vector<vector<int>>(totalNoOfStmts + 1, emptyRow);

	vector<int> allStmts = StmtTypeTable::GetAllStmtsOfType(STMT);
	vector<int> parentT;
	for (vector<int>::iterator it = allStmts.begin(); it != allStmts.end(); it++) {
		parentT = GetParentTOf(*it);
		childToParentTTable.at(*it) = parentT;
	
	}

}

// tells whether any parents relationships are stored
bool Parent::HasAnyParents() {
	return SizeOfParent() > 0;
}

int Parent::SizeOfParent() {
	return childToParentTable.size();
}

void Parent::ClearData() {
	parentToChildrenTable.clear();
	childToParentTable.clear();
	parentToChildrenTBV.clear();
	parentToChildrenTTable.clear();
	childToParentTTable.clear();
}

/** private methods **/
bool Parent::AlreadyInserted(int parentStmtIndex, int childStmtIndex) {
	if (childToParentTable.count(childStmtIndex) != 0)
		return childToParentTable[childStmtIndex] == parentStmtIndex;
	else
		return false;
}

bool Parent::HasNoParent(int childStmtIndex) {
	// map.find() == map.end() was buggy, used count method cos no need to worry about iterator equality
	return childToParentTable.count(childStmtIndex) == 0;
}