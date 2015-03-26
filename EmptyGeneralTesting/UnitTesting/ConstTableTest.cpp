#include <cppunit\config\SourcePrefix.h>

#include "..\SPA\PKB\ConstTable.h"
#include "ConstTableTest.h"

#include <iostream>
#include <string>
#include <stdint.h>

ConstTableTest::ConstTableTest(void) {
}

void ConstTableTest::setUp() {
	ConstTable::ClearData();
}

void ConstTableTest::tearDown() {
	ConstTable::ClearData();
}

CPPUNIT_TEST_SUITE_REGISTRATION(ConstTableTest);

void ConstTableTest::testSetStmtUsesConst() {
	CPPUNIT_ASSERT_EQUAL(0, ConstTable::SizeOfConstTable());

	ConstTable::SetStmtUsesConst(1, 1);
	ConstTable::SetStmtUsesConst(1, 2);
	ConstTable::SetStmtUsesConst(2, 1);

	CPPUNIT_ASSERT_EQUAL(3, ConstTable::SizeOfConstTable());
}

void ConstTableTest::testIsStmtUsingConst() {
	CPPUNIT_ASSERT_EQUAL(0, ConstTable::SizeOfConstTable());

	ConstTable::SetStmtUsesConst(1, 1);
	ConstTable::SetStmtUsesConst(1, 2);
	ConstTable::SetStmtUsesConst(2, 1);

	CPPUNIT_ASSERT(ConstTable::IsStmtUsingConst(1,1));
	CPPUNIT_ASSERT(ConstTable::IsStmtUsingConst(2,1));
	CPPUNIT_ASSERT(ConstTable::IsStmtUsingConst(1,2));
}

void ConstTableTest::testGetStmtUsingConst() {
	CPPUNIT_ASSERT_EQUAL(0, ConstTable::SizeOfConstTable());

	ConstTable::SetStmtUsesConst(1, 1);
	ConstTable::SetStmtUsesConst(1, 2);
	ConstTable::SetStmtUsesConst(2, 1);

	int correctListOfConstants[] = {1, 2};
	vector<int>listOfConstants = ConstTable::GetStmtUsingConst(1);	
	CPPUNIT_ASSERT_EQUAL(correctListOfConstants[0], listOfConstants[0]);
	CPPUNIT_ASSERT_EQUAL(correctListOfConstants[1], listOfConstants[1]);
}

void ConstTableTest::testGetConstUsedByStmt() {
	CPPUNIT_ASSERT_EQUAL(0, ConstTable::SizeOfConstTable());

	ConstTable::SetStmtUsesConst(1, 3);
	ConstTable::SetStmtUsesConst(1, 2);
	ConstTable::SetStmtUsesConst(1, 1);

	CPPUNIT_ASSERT_EQUAL(3, ConstTable::SizeOfConstTable());

	int correctListOfConstants[] = {3, 2, 1};
	vector<int> listOfConstants = ConstTable::GetConstUsedByStmt(1);
	CPPUNIT_ASSERT_EQUAL(correctListOfConstants[0], listOfConstants[0]);
	CPPUNIT_ASSERT_EQUAL(correctListOfConstants[1], listOfConstants[1]);
	CPPUNIT_ASSERT_EQUAL(correctListOfConstants[2], listOfConstants[2]);
}

void ConstTableTest::testHasAnyConst() {
	CPPUNIT_ASSERT_EQUAL(0, ConstTable::SizeOfConstTable());

	ConstTable::SetStmtUsesConst(1, 1);
	ConstTable::SetStmtUsesConst(1, 2);

	CPPUNIT_ASSERT(ConstTable::HasAnyConst());
}

void ConstTableTest::testGetAllConst() {
	CPPUNIT_ASSERT_EQUAL(0, ConstTable::SizeOfConstTable());

	ConstTable::SetStmtUsesConst(1, 1);
	ConstTable::SetStmtUsesConst(1, 2);
	ConstTable::SetStmtUsesConst(2, 1);

	CPPUNIT_ASSERT_EQUAL(3, ConstTable::SizeOfConstTable());
	
	int correctListOfConstants[] = {1, 2};
	vector<int> listOfConstants = ConstTable::GetAllConst();
	CPPUNIT_ASSERT_EQUAL(correctListOfConstants[0], listOfConstants[0]);
	CPPUNIT_ASSERT_EQUAL(correctListOfConstants[1], listOfConstants[1]);
}

void ConstTableTest::testSizeOfConstTable() {
	CPPUNIT_ASSERT_EQUAL(0, ConstTable::SizeOfConstTable());

	ConstTable::SetStmtUsesConst(1, 1);
	ConstTable::SetStmtUsesConst(1, 2);
	ConstTable::SetStmtUsesConst(2, 3);
	
	CPPUNIT_ASSERT_EQUAL(3, ConstTable::SizeOfConstTable());
}

void ConstTableTest::testClearData() {
	CPPUNIT_ASSERT_EQUAL(0, ConstTable::SizeOfConstTable());

	ConstTable::SetStmtUsesConst(1, 1);
	ConstTable::SetStmtUsesConst(1, 2);
	ConstTable::SetStmtUsesConst(2, 3);
	
	CPPUNIT_ASSERT_EQUAL(3, ConstTable::SizeOfConstTable());
	ConstTable::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, ConstTable::SizeOfConstTable());

}

