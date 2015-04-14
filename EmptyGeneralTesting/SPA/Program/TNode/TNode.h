#pragma once

#include <vector>

using namespace std;

class TNode {
public:
	enum Type {
		PROCEDURE = 0,
		PROGRAM,
		ASSIGNMENT,
		VARIABLE,
		CONSTANT,
		OPERATOR,
		STMT_LIST,
		WHILE,
		CALL,
		IF
	};

	static const string enumStringDeclarations[];
	static int nodeCounter;

	TNode* GetDirectParent();
	TNode* GetLogicalParent();
	TNode* GetRightSibling();
	vector<TNode*> GetChildren();
	TNode& GetChild(int);
	Type GetType();
	string GetName();
	string GetContent();
	int GetLineNumber();
	bool IsType(Type);
	bool HasChildren();
	int GetNodeIndex();
	
	void SetRightSibling(TNode*);
	void SetLogicalParent(TNode*);
	void AddChild(TNode*);

	static string EnumToString(Type);

	static TNode ConstructIfTNode(int lineNumber);
	static TNode ConstructWhileTNode(int lineNumber);
	static TNode ConstructCallTNode(int lineNumber, string procName);
	static TNode ConstructAssignmentTNode(int lineNumber);
	static TNode ConstructConstTNode(string value);
	static TNode ConstructVarTNode(string symbol);
	static TNode ConstructOperatorTNode(string symbol);
	static TNode ConstructStmtListTNode(string name);
	static TNode ConstructProcedureTNode(string name);
	static TNode ConstructProgramTNode(string name);
	static TNode ConstructStmtTNode(Type type, int lineNumber);
	static void resetNodeCounter();

private:
	TNode(Type type, string name);

	Type type;
	TNode* directParent;
	vector<TNode*> children;
	TNode* rightSibling;
	TNode* logicalParent;
	string name;
	string content;
	int lineNumber;
	int nodeIndex;

	void ThrowUnsupportedOperationException();

	virtual void SetDirectParent(TNode*);

};