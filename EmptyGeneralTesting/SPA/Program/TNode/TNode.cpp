#include "TNode.h"
#include "../../Parser/Token.h"
#include <sstream>

const string TNode::enumStringDeclarations[] = {
	"Procedure",
	"Program",
	"Assignment",
	"Variable",
	"Constant",
	"Operator",
	"StmtList",
	"While",
	"Call",
	"If"
};

int TNode::nodeCounter;

TNode::TNode(Type type, string name)
	: type(type), name(name), rightSibling(nullptr)
	, logicalParent(nullptr), content(""), lineNumber(0) {
	nodeIndex = nodeCounter;
	nodeCounter++;
}

void TNode::ThrowUnsupportedOperationException() {
	throw(string) "Unsupported Operation";
}

TNode* TNode::GetDirectParent() {
	return directParent;
}

TNode* TNode::GetRightSibling() {
	return rightSibling;
}

vector<TNode*> TNode::GetChildren() {
	return children;
}

TNode& TNode::GetChild(int index) {
	return *children[index];
}

TNode::Type TNode::GetType() {
	return type;
}

bool TNode::IsType(Type testType) {
	return (type == testType);
}

bool TNode::HasChildren() {
	return (children.size() > 0);
}

void TNode::SetDirectParent(TNode* parent) {
	directParent = parent;
}

void TNode::SetRightSibling(TNode* rightSibling) {
	rightSibling = rightSibling;
}

void TNode::SetLogicalParent(TNode* logicalParent) {
	logicalParent = logicalParent;
}

void TNode::AddChild(TNode* child) {
	if (child == nullptr) {
		throw(string) "Null pointer!";
	}
	if (children.size() > 0) {
		children.back()->SetRightSibling(child);
	}
	child->SetDirectParent(this);
	children.push_back(child);
}

string TNode::GetName() {
	return name;
}

string TNode::GetContent() {
	return content;
}

int TNode::GetLineNumber() {
	return lineNumber;
}

int TNode::GetNodeIndex() {
	return nodeIndex;
}

string TNode::EnumToString(TNode::Type type) {
	return enumStringDeclarations[type];
}

TNode TNode::ConstructIfTNode(int lineNumber) {
	return ConstructStmtTNode(TNode::IF, lineNumber);
}

TNode TNode::ConstructWhileTNode(int lineNumber) {
	return ConstructStmtTNode(TNode::WHILE, lineNumber);
}

TNode TNode::ConstructCallTNode(int lineNumber, string procName) {
	TNode result = ConstructStmtTNode(TNode::CALL, lineNumber);
	result.content = procName;
	result.name = procName.append(result.name);
	return result;
}

TNode TNode::ConstructAssignmentTNode(int lineNumber) {
	TNode result = ConstructStmtTNode(TNode::ASSIGNMENT, lineNumber);
	result.content = Token::OperatorAssign;
	return result;
}

TNode TNode::ConstructConstTNode(string value) {
	TNode result(TNode::CONSTANT, value + ":" + EnumToString(TNode::CONSTANT));
	result.content = value;
	return result;
}

TNode TNode::ConstructVarTNode(string symbol) {
	TNode result(TNode::VARIABLE, symbol + ":" + EnumToString(TNode::VARIABLE));
	result.content = symbol;
	return result;
}

TNode TNode::ConstructOperatorTNode(string symbol) {
	TNode result(TNode::OPERATOR, symbol + ":" + EnumToString(TNode::OPERATOR));
	result.content = symbol;
	return result;
}

TNode TNode::ConstructStmtListTNode(string name) {
	return TNode(TNode::STMT_LIST, name +":" + EnumToString(TNode::STMT_LIST));
}

TNode TNode::ConstructProcedureTNode(string name) {
	TNode result(TNode::PROCEDURE, ":" + EnumToString(TNode::PROCEDURE));
	result.content = name;
	return result;
}

TNode TNode::ConstructProgramTNode(string name) {
	return TNode(TNode::PROGRAM, ":" + EnumToString(TNode::PROGRAM));
}

TNode TNode::ConstructStmtTNode(Type type, int lineNumber) {
	TNode result(type, ":" + EnumToString(type));
	result.lineNumber = lineNumber;
	return result;
}

void TNode::resetNodeCounter() {
	nodeCounter = 0;
}
