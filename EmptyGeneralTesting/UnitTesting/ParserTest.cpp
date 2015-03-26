#include "ParserTest.h"
#include "..\SPA\Program\Program.h"


#include <fstream>

const string TESTFILE_DIRECTORY = "ParserTestFiles/";

void ParserTest::setUp() {}
void ParserTest::tearDown() {}

CPPUNIT_TEST_SUITE_REGISTRATION(ParserTest);

ParserTest::ParserTest() {
}

void ParseSource(string filename) {
	Parser::Parse(string(TESTFILE_DIRECTORY).append(filename));
}

TNode& GetLHS(TNode& node) {
	return node.GetChild(0);
}

TNode& GetRHS(TNode& node) {
	return node.GetChild(1);
}

void ParserTest::TestSimpleAssignmentParsing() {
	Program::ClearAll();
	ParseSource("simpleAssignmentTest.txt");
	TNode program = Program::GetASTRootNode();
	TNode procedure = program.GetChild(0);
	TNode firstStmt = procedure.GetChild(0).GetChild(0);
	TNode secondStmt = procedure.GetChild(0).GetChild(1);

	CPPUNIT_ASSERT(procedure.GetContent() == "simpleAssignmentTest");

	CPPUNIT_ASSERT(firstStmt.GetLineNumber() == 1);
	CPPUNIT_ASSERT(firstStmt.GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(firstStmt).GetContent() == "x");
	CPPUNIT_ASSERT(GetRHS(firstStmt).GetContent() == "1");

	CPPUNIT_ASSERT(secondStmt.GetLineNumber() == 2);
	CPPUNIT_ASSERT(secondStmt.GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(secondStmt).GetContent() == "y");
	CPPUNIT_ASSERT(GetRHS(secondStmt).GetContent() == "x");

}

void ParserTest::TestAdditionParsing() {
	Program::ClearAll();
	ParseSource("additionTest.txt");
	TNode program = Program::GetASTRootNode();
	TNode procedure = program.GetChild(0);
	TNode firstStmt = procedure.GetChild(0).GetChild(0);
	TNode secondStmt = procedure.GetChild(0).GetChild(1);
	TNode thirdStmt = procedure.GetChild(0).GetChild(2);
	TNode fourthStmt = procedure.GetChild(0).GetChild(3);

	CPPUNIT_ASSERT(procedure.GetContent() == "additionTest");

	CPPUNIT_ASSERT(firstStmt.GetLineNumber() == 1);
	CPPUNIT_ASSERT(firstStmt.GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(firstStmt).GetContent() == "x");
	CPPUNIT_ASSERT(GetRHS(firstStmt).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(firstStmt)).GetContent() == "y");
	CPPUNIT_ASSERT(GetRHS(GetRHS(firstStmt)).GetContent() == "z");

	CPPUNIT_ASSERT(secondStmt.GetLineNumber() == 2);
	CPPUNIT_ASSERT(secondStmt.GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(secondStmt).GetContent() == "y");
	CPPUNIT_ASSERT(GetRHS(secondStmt).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(secondStmt)).GetContent() == "z");
	CPPUNIT_ASSERT(GetRHS(GetRHS(secondStmt)).GetContent() == "1");

	CPPUNIT_ASSERT(thirdStmt.GetLineNumber() == 3);
	CPPUNIT_ASSERT(thirdStmt.GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(thirdStmt).GetContent() == "z");
	CPPUNIT_ASSERT(GetRHS(thirdStmt).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(thirdStmt)).GetContent() == "2");
	CPPUNIT_ASSERT(GetRHS(GetRHS(thirdStmt)).GetContent() == "3");

	CPPUNIT_ASSERT(fourthStmt.GetLineNumber() == 4);
	CPPUNIT_ASSERT(fourthStmt.GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(fourthStmt).GetContent() == "a");
	CPPUNIT_ASSERT(GetRHS(fourthStmt).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(fourthStmt)).GetContent() == "+");
	CPPUNIT_ASSERT(GetRHS(GetRHS(fourthStmt)).GetContent() == "y");
	CPPUNIT_ASSERT(GetLHS(GetLHS(GetRHS(fourthStmt))).GetContent() == "x");
	CPPUNIT_ASSERT(GetRHS(GetLHS(GetRHS(fourthStmt))).GetContent() == "2");
}

void ParserTest::TestWhileParsing() {
	Program::ClearAll();
	ParseSource("whileTest.txt");
	TNode program = Program::GetASTRootNode();
	TNode procedure = program.GetChild(0);
	TNode whileLoop = procedure.GetChild(0).GetChild(0);
	TNode firstStmt = whileLoop.GetChild(1).GetChild(0);
	TNode secondStmt = whileLoop.GetChild(1).GetChild(1);

	CPPUNIT_ASSERT(procedure.GetContent() == "whileTest");

	//test condition
	CPPUNIT_ASSERT(whileLoop.GetLineNumber() == 1);
	CPPUNIT_ASSERT(whileLoop.GetChild(0).GetContent() == "z");

	CPPUNIT_ASSERT(firstStmt.GetLineNumber() == 2);
	CPPUNIT_ASSERT(firstStmt.GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(firstStmt).GetContent() == "x");
	CPPUNIT_ASSERT(GetRHS(firstStmt).GetContent() == "2");

	CPPUNIT_ASSERT(secondStmt.GetLineNumber() == 3);
	CPPUNIT_ASSERT(secondStmt.GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(secondStmt).GetContent() == "y");
	CPPUNIT_ASSERT(GetRHS(secondStmt).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(secondStmt)).GetContent() == "y");
	CPPUNIT_ASSERT(GetRHS(GetRHS(secondStmt)).GetContent() == "1");
}

void ParserTest::TestNestedWhileParsing() {
	Program::ClearAll();
	ParseSource("nestedWhileTest.txt");
	TNode program = Program::GetASTRootNode();
	TNode procedure = program.GetChild(0);
	TNode firstWhileLoop = procedure.GetChild(0).GetChild(0);
	TNode secondWhileLoop = firstWhileLoop.GetChild(1).GetChild(0);
	TNode thirdWhileLoop = secondWhileLoop.GetChild(1).GetChild(0);
	TNode nestedStmt = thirdWhileLoop.GetChild(1).GetChild(0);

	CPPUNIT_ASSERT(procedure.GetContent() == "nestedWhileParsing");

	CPPUNIT_ASSERT(firstWhileLoop.GetLineNumber() == 1);
	CPPUNIT_ASSERT(firstWhileLoop.GetChild(0).GetContent() == "x");

	CPPUNIT_ASSERT(secondWhileLoop.GetLineNumber() == 2);
	CPPUNIT_ASSERT(secondWhileLoop.GetChild(0).GetContent() == "y");

	CPPUNIT_ASSERT(thirdWhileLoop.GetLineNumber() == 3);
	CPPUNIT_ASSERT(thirdWhileLoop.GetChild(0).GetContent() == "z");

	CPPUNIT_ASSERT(nestedStmt.GetLineNumber() == 4);
	CPPUNIT_ASSERT(nestedStmt.GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(nestedStmt).GetContent() == "x");
	CPPUNIT_ASSERT(GetRHS(nestedStmt).GetContent() == "0");
}

void ParserTest::TestParsing() {
	Program::ClearAll();
	ParseSource("sample_SIMPLE_source.txt");
	TNode program = Program::GetASTRootNode();
	TNode procedure = program.GetChild(0);
	vector<TNode*> stmts = procedure.GetChild(0).GetChildren();

	// procedure ABC {
	CPPUNIT_ASSERT(procedure.GetContent() == "ABC");

	//  i=1;
	CPPUNIT_ASSERT(stmts[0]->GetLineNumber() == 1);
	CPPUNIT_ASSERT(stmts[0]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmts[0]).GetContent() == "i");
	CPPUNIT_ASSERT(GetRHS(*stmts[0]).GetContent() == "1");

	// b=200 ;
	CPPUNIT_ASSERT(stmts[1]->GetLineNumber() == 2);
	CPPUNIT_ASSERT(stmts[1]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmts[1]).GetContent() == "b");
	CPPUNIT_ASSERT(GetRHS(*stmts[1]).GetContent() == "200");

	// 	c= a   ;
	CPPUNIT_ASSERT(stmts[2]->GetLineNumber() == 3);
	CPPUNIT_ASSERT(stmts[2]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmts[2]).GetContent() == "c");
	CPPUNIT_ASSERT(GetRHS(*stmts[2]).GetContent() == "a");

	// while a	
	CPPUNIT_ASSERT(stmts[3]->GetLineNumber() == 4);
	CPPUNIT_ASSERT(stmts[3]->GetType() == TNode::WHILE);
	TNode stmt4 = *stmts[3];
	vector<TNode*> stmtList4 = stmt4.GetChild(1).GetChildren();
	CPPUNIT_ASSERT(stmt4.GetChild(0).GetContent() == "a");

	//    while beta {
	CPPUNIT_ASSERT(stmtList4[0]->GetLineNumber() == 5);
	CPPUNIT_ASSERT(stmtList4[0]->GetType() == TNode::WHILE);
	TNode stmt5 = *stmtList4[0];
	vector<TNode*> stmtList5 = stmt5.GetChild(1).GetChildren();
	CPPUNIT_ASSERT(stmt5.GetChild(0).GetContent() == "beta");

	//         oSCar  = 1 + beta + tmp;
	CPPUNIT_ASSERT(stmtList5[0]->GetLineNumber() == 6);
	CPPUNIT_ASSERT(stmtList5[0]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList5[0]).GetContent() == "oSCar");
	CPPUNIT_ASSERT(GetRHS(*stmtList5[0]).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(*stmtList5[0])).GetContent() == "+");
	CPPUNIT_ASSERT(GetRHS(GetRHS(*stmtList5[0])).GetContent() == "tmp");
	CPPUNIT_ASSERT(GetLHS(GetLHS(GetRHS(*stmtList5[0]))).GetContent() == "1");
	CPPUNIT_ASSERT(GetRHS(GetLHS(GetRHS(*stmtList5[0]))).GetContent() == "beta");

	//         while tmp{
	CPPUNIT_ASSERT(stmtList5[1]->GetLineNumber() == 7);
	CPPUNIT_ASSERT(stmtList5[1]->GetType() == TNode::WHILE);
	TNode stmt7 = *stmtList5[1];
	vector<TNode*> stmtList7 = stmt7.GetChild(1).GetChildren();
	CPPUNIT_ASSERT(stmt7.GetChild(0).GetContent() == "tmp");

	//           oSCar = I + k + j1k + chArlie; }
	CPPUNIT_ASSERT(stmtList7[0]->GetLineNumber() == 8);
	CPPUNIT_ASSERT(stmtList7[0]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList7[0]).GetContent() == "oSCar");
	CPPUNIT_ASSERT(GetRHS(*stmtList7[0]).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(*stmtList7[0])).GetContent() == "+");
	CPPUNIT_ASSERT(GetRHS(GetRHS(*stmtList7[0])).GetContent() == "chArlie");
	CPPUNIT_ASSERT(GetLHS(GetLHS(GetRHS(*stmtList7[0]))).GetContent() == "+");
	CPPUNIT_ASSERT(GetRHS(GetLHS(GetRHS(*stmtList7[0]))).GetContent() == "j1k");
	CPPUNIT_ASSERT(GetLHS(GetLHS(GetLHS(GetRHS(*stmtList7[0])))).GetContent() == "I");
	CPPUNIT_ASSERT(GetRHS(GetLHS(GetLHS(GetRHS(*stmtList7[0])))).GetContent() == "k");

	// 	while x 
	CPPUNIT_ASSERT(stmtList5[2]->GetLineNumber() == 9);
	CPPUNIT_ASSERT(stmtList5[2]->GetType() == TNode::WHILE);
	TNode stmt9 = *stmtList5[2];
	vector<TNode*> stmtList9 = stmt9.GetChild(1).GetChildren();
	CPPUNIT_ASSERT(stmt9.GetChild(0).GetContent() == "x");

	//         x = x + 1;
	CPPUNIT_ASSERT(stmtList9[0]->GetLineNumber() == 10);
	CPPUNIT_ASSERT(stmtList9[0]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList9[0]).GetContent() == "x");
	CPPUNIT_ASSERT(GetRHS(*stmtList9[0]).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(*stmtList9[0])).GetContent() == "x");
	CPPUNIT_ASSERT(GetRHS(GetRHS(*stmtList9[0])).GetContent() == "1");

	//        while left {
	CPPUNIT_ASSERT(stmtList9[1]->GetLineNumber() == 11);
	CPPUNIT_ASSERT(stmtList9[1]->GetType() == TNode::WHILE);
	TNode stmt11 = *stmtList9[1];
	vector<TNode*> stmtList11 = stmt11.GetChild(1).GetChildren();
	CPPUNIT_ASSERT(stmt11.GetChild(0).GetContent() == "left");

	//           while right {
	CPPUNIT_ASSERT(stmtList11[0]->GetLineNumber() == 12);
	CPPUNIT_ASSERT(stmtList11[0]->GetType() == TNode::WHILE);
	TNode stmt12 = *stmtList11[0];
	vector<TNode*> stmtList12 = stmt12.GetChild(1).GetChildren();
	CPPUNIT_ASSERT(stmt12.GetChild(0).GetContent() == "right");

	//             Romeo = Romeo + 1;
	CPPUNIT_ASSERT(stmtList12[0]->GetLineNumber() == 13);
	CPPUNIT_ASSERT(stmtList12[0]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList12[0]).GetContent() == "Romeo");
	CPPUNIT_ASSERT(GetRHS(*stmtList12[0]).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(*stmtList12[0])).GetContent() == "Romeo");
	CPPUNIT_ASSERT(GetRHS(GetRHS(*stmtList12[0])).GetContent() == "1");

	//             b = 0;
	CPPUNIT_ASSERT(stmtList12[1]->GetLineNumber() == 14);
	CPPUNIT_ASSERT(stmtList12[1]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList12[1]).GetContent() == "b");
	CPPUNIT_ASSERT(GetRHS(*stmtList12[1]).GetContent() == "0");

	//             c = delta    + l  + width + Romeo ; }
	CPPUNIT_ASSERT(stmtList12[2]->GetLineNumber() == 15);
	CPPUNIT_ASSERT(stmtList12[2]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList12[2]).GetContent() == "c");
	CPPUNIT_ASSERT(GetRHS(*stmtList12[2]).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(*stmtList12[2])).GetContent() == "+");
	CPPUNIT_ASSERT(GetRHS(GetRHS(*stmtList12[2])).GetContent() == "Romeo");
	CPPUNIT_ASSERT(GetLHS(GetLHS(GetRHS(*stmtList12[2]))).GetContent() == "+");
	CPPUNIT_ASSERT(GetRHS(GetLHS(GetRHS(*stmtList12[2]))).GetContent() == "width");
	CPPUNIT_ASSERT(GetLHS(GetLHS(GetLHS(GetRHS(*stmtList12[2])))).GetContent() == "delta");
	CPPUNIT_ASSERT(GetRHS(GetLHS(GetLHS(GetRHS(*stmtList12[2])))).GetContent() == "l");

	//             while c {
	CPPUNIT_ASSERT(stmtList11[1]->GetLineNumber() == 16);
	CPPUNIT_ASSERT(stmtList11[1]->GetType() == TNode::WHILE);
	TNode stmt16 = *stmtList11[1];
	vector<TNode*> stmtList16 = stmt16.GetChild(1).GetChildren();
	CPPUNIT_ASSERT(stmt16.GetChild(0).GetContent() == "c");

	//               c = c +1   	; }
	CPPUNIT_ASSERT(stmtList16[0]->GetLineNumber() == 17);
	CPPUNIT_ASSERT(stmtList16[0]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList16[0]).GetContent() == "c");
	CPPUNIT_ASSERT(GetRHS(*stmtList16[0]).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(*stmtList16[0])).GetContent() == "c");
	CPPUNIT_ASSERT(GetRHS(GetRHS(*stmtList16[0])).GetContent() == "1");

	//             x = x+ 1	; }}
	CPPUNIT_ASSERT(stmtList11[2]->GetLineNumber() == 18);
	CPPUNIT_ASSERT(stmtList11[2]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList11[2]).GetContent() == "x");
	CPPUNIT_ASSERT(GetRHS(*stmtList11[2]).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(*stmtList11[2])).GetContent() == "x");
	CPPUNIT_ASSERT(GetRHS(GetRHS(*stmtList11[2])).GetContent() == "1");

	//           a=   2; }
	CPPUNIT_ASSERT(stmtList5[3]->GetLineNumber() == 19);
	CPPUNIT_ASSERT(stmtList5[3]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList5[3]).GetContent() == "a");
	CPPUNIT_ASSERT(GetRHS(*stmtList5[3]).GetContent() == "2");

	//    w = w+1  ;
	CPPUNIT_ASSERT(stmtList4[1]->GetLineNumber() == 20);
	CPPUNIT_ASSERT(stmtList4[1]->GetContent() == "=");
	CPPUNIT_ASSERT(GetLHS(*stmtList4[1]).GetContent() == "w");
	CPPUNIT_ASSERT(GetRHS(*stmtList4[1]).GetContent() == "+");
	CPPUNIT_ASSERT(GetLHS(GetRHS(*stmtList4[1])).GetContent() == "w");
	CPPUNIT_ASSERT(GetRHS(GetRHS(*stmtList4[1])).GetContent() == "1");

}