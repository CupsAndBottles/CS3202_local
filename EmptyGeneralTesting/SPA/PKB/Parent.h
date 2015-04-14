#ifndef PARENT_H
#define PARENT_H

#include <utility>
#include <vector>
#include <map>

using namespace std;

class Parent {
public:
	// Default constructor
	Parent();

    // API
	static void SetParent(int parentStmtIndex, int childStmtIndex);
	static bool IsParent(int parentStmtIndex, int childStmtIndex);
	static int GetParentOf(int childStmtIndex);
	static vector<int> GetChildrenOf(int parentStmtIndex);
	
	static bool IsParentT(int parentStmtIndex, int childStmtIndex);
	static bool IsParentTBV(int parentStmtIndex, int childStmtIndex);
	static vector<int> GetParentTOf(int childStmtIndex);
	static vector<int> GetChildrenTOf(int parentStmtIndex);
	static vector<int> GetStoredParentTOf(int childStmtIndex);
	static vector<int> GetStoredChildrenTOf(int parentStmtIndex);

	static void CreateParentToChildrenTBV();
	static void CreateParentToChildrenTTable();
	static void CreateChildrenToParentTTable();
	static bool HasAnyParents();

	// helper methods for testing
	static int SizeOfParent();
	static void ClearData();

private:
	static map <int, vector<int>> parentToChildrenTable;
	static map <int, int> childToParentTable;

	static vector<vector<bool>> parentToChildrenTBV;
	static vector<vector<int>> parentToChildrenTTable;
	static vector<vector<int>> childToParentTTable;
	
	static bool AlreadyInserted(int parentStmtIndex, int childStmtIndex);
	static bool HasNoParent(int childStmtIndex);
	
};
#endif