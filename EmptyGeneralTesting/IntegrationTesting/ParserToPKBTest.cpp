#include <cppunit\config\SourcePrefix.h>

#include "ParserToPKBTest.h"

#include "..\SPA\Parser\Parser.h"
#include "..\SPA\PKB\VarTable.h"
#include "..\SPA\PKB\StmtTypeTable.h"
#include "..\SPA\PKB\ConstTable.h"
#include "..\SPA\PKB\Follows.h"
#include "..\SPA\PKB\Parent.h"
#include "..\SPA\PKB\Modifies.h"
#include "..\SPA\PKB\Uses.h"
#include "..\SPA\PKB\Next.h"
#include "..\SPA\PKB\ProcTable.h"
#include "..\SPA\Program\Program.h"

#include <iostream>
#include <string>
#include <stdint.h>

CPPUNIT_TEST_SUITE_REGISTRATION(ParserToPKBTest);

const string TESTFILE_DIRECTORY = "ParserTestFiles/";

ParserToPKBTest::ParserToPKBTest() {
	Program::ClearData();
	Parent::ClearData();
	Uses::ClearData();
	Modifies::ClearData();
	Follows::ClearData();
	VarTable::ClearData();
	ConstTable::ClearData();
}

void ParseSource(string filename) {
	Parser::Parse(string(TESTFILE_DIRECTORY).append(filename));
}

void ParserToPKBTest::setUp() { 
	ParseSource("pkbTest.txt");
}

void ParserToPKBTest::tearDown() {
	// empty body
}



void ParserToPKBTest::TestNothing() {
	CPPUNIT_ASSERT_EQUAL_MESSAGE("simple dummy test", 0, 0);
}

// testTwo uses the SIMPLE source code in whileTest.txt file
// testTwo uses the SIMPLE source code in nestedWhileTest.txt file
// testTwo uses the SIMPLE source code in additionTest.txt file

// insert other methods below here
void ParserToPKBTest::TestVarTable() {
	// check if VarTable has correct number of variables
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of variables in VarTable", 8, VarTable::GetNoOfVars());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("1st variable of VarTable", 0, VarTable::GetIndexOfVar("x"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("2nd variable of VarTable", 1, VarTable::GetIndexOfVar("y"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("3rd variable of VarTable", 2, VarTable::GetIndexOfVar("z"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("4th variable of VarTable", 3, VarTable::GetIndexOfVar("tEst"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("5th variable of VarTable", 4, VarTable::GetIndexOfVar("newVar"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("6th variable of VarTable", 5, VarTable::GetIndexOfVar("a"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("7th variable of VarTable", 6, VarTable::GetIndexOfVar("b"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("8th variable of VarTable", 7, VarTable::GetIndexOfVar("c"));
}

void ParserToPKBTest::TestModifies() {
	// check if modifiesTable is updated
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of variables in Modifies", 11, Modifies::SizeOfModifies());
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 1st variable", Modifies::IsStmtModifyingVar(1, 0));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 2nd variable", Modifies::IsStmtModifyingVar(2, 1));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 3rd variable", Modifies::IsStmtModifyingVar(4, 3));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 4th variable", Modifies::IsStmtModifyingVar(6, 1));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 5th variable", Modifies::IsStmtModifyingVar(8, 3));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 6th variable", Modifies::IsStmtModifyingVar(9, 5));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 7th variable", Modifies::IsStmtModifyingVar(3, 3));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 8th variable", Modifies::IsStmtModifyingVar(5, 1));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 9th variable", Modifies::IsStmtModifyingVar(3, 1));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 10th variable", Modifies::IsStmtModifyingVar(7, 3));
	CPPUNIT_ASSERT_MESSAGE("test correct Modifies relationship for 11th variable", Modifies::IsStmtModifyingVar(3, 5));

}

void ParserToPKBTest::TestUses() {
	// check if usesTable is updated
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of variables in Uses", 17, Uses::SizeOfUses());
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 1st variable", Uses::IsStmtUsingVar(2, 0));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 2nd variable", Uses::IsStmtUsingVar(3, 2));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 3rd variable", Uses::IsStmtUsingVar(4, 2));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 4th variable", Uses::IsStmtUsingVar(4, 0));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 5th variable", Uses::IsStmtUsingVar(5, 0));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 6th variable", Uses::IsStmtUsingVar(6, 4));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 7th variable", Uses::IsStmtUsingVar(6, 3));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 8th variable", Uses::IsStmtUsingVar(7, 3));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 9th variable", Uses::IsStmtUsingVar(9, 6));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 10th variable", Uses::IsStmtUsingVar(9, 7));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 11th variable", Uses::IsStmtUsingVar(3, 0));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 12th variable", Uses::IsStmtUsingVar(3, 4));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 13th variable", Uses::IsStmtUsingVar(3, 3));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 14th variable", Uses::IsStmtUsingVar(3, 6));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 15th variable", Uses::IsStmtUsingVar(3, 7));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 16th variable", Uses::IsStmtUsingVar(5, 3));
	CPPUNIT_ASSERT_MESSAGE("test correct Uses relationship for 17th variable", Uses::IsStmtUsingVar(5, 4));
}

void ParserToPKBTest::TestParent() {
	// check if parentTable is updated
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of variables in Parent", 6, Parent::SizeOfParent());
	CPPUNIT_ASSERT_MESSAGE("test correct Parent relationship for 1st variable", Parent::IsParent(3, 4));
	CPPUNIT_ASSERT_MESSAGE("test correct Parent relationship for 2nd variable", Parent::IsParent(3, 5));
	CPPUNIT_ASSERT_MESSAGE("test correct Parent relationship for 3rd variable", Parent::IsParent(3, 7));
	CPPUNIT_ASSERT_MESSAGE("test correct Parent relationship for 4th variable", Parent::IsParent(3, 9));
	CPPUNIT_ASSERT_MESSAGE("test correct Parent relationship for 5th variable", Parent::IsParent(5, 6));
	CPPUNIT_ASSERT_MESSAGE("test correct Parent relationship for 6th variable", Parent::IsParent(7, 8));

	CPPUNIT_ASSERT_MESSAGE("test correct ParentT relationship for 3th variable", Parent::IsParentT(3, 6));
	CPPUNIT_ASSERT_MESSAGE("test correct ParentT relationship for 3th variable", Parent::IsParentT(3, 7));
	CPPUNIT_ASSERT_MESSAGE("test correct ParentT relationship for 3th variable", Parent::IsParentT(3, 8));
	//CPPUNIT_ASSERT_MESSAGE("test correct ParentT relationship for 3th variable", Parent::IsParentT(2, 9));

}

void ParserToPKBTest::TestFollows() {
	// check if followsTable is updated
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of variables in Follows", 5, Follows::SizeOfFollows());
	CPPUNIT_ASSERT_MESSAGE("test correct Follows relationship for 1st variable", Follows::IsFollows(1, 2));
	CPPUNIT_ASSERT_MESSAGE("test correct Follows relationship for 2nd variable", Follows::IsFollows(2, 3));
	CPPUNIT_ASSERT_MESSAGE("test correct Follows relationship for 3rd variable", Follows::IsFollows(4, 5));
	CPPUNIT_ASSERT_MESSAGE("test correct Follows relationship for 4th variable", Follows::IsFollows(5, 7));
	CPPUNIT_ASSERT_MESSAGE("test correct Follows relationship for 5th variable", Follows::IsFollows(7, 9));

	CPPUNIT_ASSERT(Follows::IsFollowsT(1, 3));
	CPPUNIT_ASSERT(Follows::IsFollowsT(1, 2));
	CPPUNIT_ASSERT(Follows::IsFollowsT(4, 9));

	//CPPUNIT_ASSERT(Follows::IsFollowsT(4, 6));
	//CPPUNIT_ASSERT(Follows::IsFollowsT(9, 4));

}

void ParserToPKBTest::TestNext() {
	// check if next is working in DE
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of variables in Next", 11 ,Next::SizeOfNext());
	CPPUNIT_ASSERT_MESSAGE("1st variable of Next", Next::IsNext(1, 2));
	CPPUNIT_ASSERT_MESSAGE("2nd variable of Next", Next::IsNext(2, 3));
	CPPUNIT_ASSERT_MESSAGE("3rd variable of Next", Next::IsNext(3, 4));
	CPPUNIT_ASSERT_MESSAGE("4th variable of Next", Next::IsNext(4, 5));
	CPPUNIT_ASSERT_MESSAGE("5th variable of Next", Next::IsNext(5, 7));
	CPPUNIT_ASSERT_MESSAGE("6th variable of Next", Next::IsNext(7, 9));
	CPPUNIT_ASSERT_MESSAGE("7th variable of Next", Next::IsNext(5, 6));
	CPPUNIT_ASSERT_MESSAGE("8th variable of Next", Next::IsNext(6, 5));
	CPPUNIT_ASSERT_MESSAGE("9th variable of Next", Next::IsNext(7, 8));
	CPPUNIT_ASSERT_MESSAGE("10th variable of Next", Next::IsNext(8, 7));
	CPPUNIT_ASSERT_MESSAGE("11th variable of Next", Next::IsNext(9, 3));
}

void ParserToPKBTest::TestModifiesForProcs() {
	// check if modifies for procs is computed correctly
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of variables in Modifies for Procedures", 
		5, (int)Modifies::GetVarModifiedByProc(ProcTable::GetIndexOfProc("test")).size());
}

void ParserToPKBTest::TestUsesForProcs() {
	// check if uses for procs is computed correctly
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of variables in Uses for Procedures", 
		6, (int)Uses::GetVarUsedByProc(ProcTable::GetIndexOfProc("test")).size());
}

void ParserToPKBTest::TestStmtTypeTable() {
	// check if StmtTypeTable is updated
	CPPUNIT_ASSERT_MESSAGE("test correct typing for 1st stmt", StmtTypeTable::CheckIfStmtOfType(1, ASSIGN));
	CPPUNIT_ASSERT_MESSAGE("test correct typing for 2nd stmt", StmtTypeTable::CheckIfStmtOfType(2, ASSIGN));
	CPPUNIT_ASSERT_MESSAGE("test correct typing for 3rd stmt", StmtTypeTable::CheckIfStmtOfType(3, WHILE));
	CPPUNIT_ASSERT_MESSAGE("test correct typing for 4th stmt", StmtTypeTable::CheckIfStmtOfType(4, ASSIGN));
	CPPUNIT_ASSERT_MESSAGE("test correct typing for 5th stmt", StmtTypeTable::CheckIfStmtOfType(5, WHILE));
	CPPUNIT_ASSERT_MESSAGE("test correct typing for 6th stmt", StmtTypeTable::CheckIfStmtOfType(6, ASSIGN));
	CPPUNIT_ASSERT_MESSAGE("test correct typing for 7th stmt", StmtTypeTable::CheckIfStmtOfType(7, WHILE));
	CPPUNIT_ASSERT_MESSAGE("test correct typing for 8th stmt", StmtTypeTable::CheckIfStmtOfType(8, ASSIGN));
	CPPUNIT_ASSERT_MESSAGE("test correct typing for 9th stmt", StmtTypeTable::CheckIfStmtOfType(9, ASSIGN));
}

// add a comment before each method to indicate its purpose