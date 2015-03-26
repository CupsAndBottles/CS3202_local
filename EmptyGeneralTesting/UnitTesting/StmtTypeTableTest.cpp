#include <cppunit\config\SourcePrefix.h>

#include "..\SPA\PKB\StmtTypeTable.h"
#include "StmtTypeTableTest.h"

#include <iostream>
#include <string>
#include <stdint.h>
#include <algorithm>

void StmtTypeTableTest::setUp() {
	StmtTypeTable::ClearData();
}

void StmtTypeTableTest::tearDown() {
	StmtTypeTable::ClearData();
}

CPPUNIT_TEST_SUITE_REGISTRATION(StmtTypeTableTest);

void StmtTypeTableTest::TestGetAllStmtsOfType() {
	StmtTypeTable::ClearData();

	StmtTypeTable::Insert(1, ASSIGN);
	StmtTypeTable::Insert(2, WHILE);
	StmtTypeTable::Insert(3, IF);
	StmtTypeTable::Insert(4, CALL);
	StmtTypeTable::Insert(4, CALL); // repetition won't get added
	StmtTypeTable::Insert(6, STMT); // illegal operation

	CPPUNIT_ASSERT_EQUAL(4, StmtTypeTable::GetNoOfStmts());
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(ASSIGN));
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(WHILE));
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(CALL));
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(IF));
	
	vector<int> resultAssignStmts = StmtTypeTable::GetAllStmtsOfType(ASSIGN);
	CPPUNIT_ASSERT_EQUAL(1, resultAssignStmts.at(0));

	vector<int> resultWhileStmts = StmtTypeTable::GetAllStmtsOfType(WHILE);
	CPPUNIT_ASSERT_EQUAL(2, resultWhileStmts.at(0));

	vector<int> resultCallStmts = StmtTypeTable::GetAllStmtsOfType(CALL);
	CPPUNIT_ASSERT_EQUAL(4, resultCallStmts.at(0));

	vector<int> resultIfStmts = StmtTypeTable::GetAllStmtsOfType(IF);
	CPPUNIT_ASSERT_EQUAL(3, resultIfStmts.at(0));

	vector<int> resultAllStmts = StmtTypeTable::GetAllStmtsOfType(STMT);
	CPPUNIT_ASSERT_EQUAL(4, (int) resultAllStmts.size());
	sort(resultAllStmts.begin(), resultAllStmts.end());
	CPPUNIT_ASSERT_EQUAL(1, resultAllStmts.at(0));
	CPPUNIT_ASSERT_EQUAL(2, resultAllStmts.at(1));
	CPPUNIT_ASSERT_EQUAL(3, resultAllStmts.at(2));
	CPPUNIT_ASSERT_EQUAL(4, resultAllStmts.at(3));
}

void StmtTypeTableTest::TestCheckIfStmtOfType() {
	StmtTypeTable::ClearData();

	StmtTypeTable::Insert(1, ASSIGN);
	StmtTypeTable::Insert(2, WHILE);

	CPPUNIT_ASSERT(StmtTypeTable::CheckIfStmtOfType(1, ASSIGN)); 
	CPPUNIT_ASSERT(StmtTypeTable::CheckIfStmtOfType(2, WHILE)); 
}

void StmtTypeTableTest::TestInsert() {
	StmtTypeTable::ClearData();
	
	StmtTypeTable::Insert(1, ASSIGN);
	StmtTypeTable::Insert(1, ASSIGN);

	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmts());
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(ASSIGN));
	CPPUNIT_ASSERT_EQUAL(1, (int) StmtTypeTable::GetAllStmtsOfType(ASSIGN).size());
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetAllStmtsOfType(ASSIGN).at(0));

	CPPUNIT_ASSERT_EQUAL(0, StmtTypeTable::GetNoOfStmtsOfType(WHILE));
	CPPUNIT_ASSERT_EQUAL(0, StmtTypeTable::GetNoOfStmtsOfType(IF));
	CPPUNIT_ASSERT_EQUAL(0, StmtTypeTable::GetNoOfStmtsOfType(CALL));

	StmtTypeTable::Insert(59, WHILE);
	CPPUNIT_ASSERT_EQUAL(2, StmtTypeTable::GetNoOfStmts());
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(ASSIGN));
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(WHILE));
	CPPUNIT_ASSERT_EQUAL(1, (int) StmtTypeTable::GetAllStmtsOfType(WHILE).size());
	CPPUNIT_ASSERT_EQUAL(59, StmtTypeTable::GetAllStmtsOfType(WHILE).at(0));

	StmtTypeTable::Insert(59, CALL);
	CPPUNIT_ASSERT_EQUAL(2, StmtTypeTable::GetNoOfStmts());
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(ASSIGN));
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(WHILE));
	CPPUNIT_ASSERT_EQUAL(0, StmtTypeTable::GetNoOfStmtsOfType(CALL));
	
	StmtTypeTable::Insert(3, PROG_LINE);
	CPPUNIT_ASSERT_EQUAL(2, StmtTypeTable::GetNoOfStmts());
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(ASSIGN));
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(WHILE));
	CPPUNIT_ASSERT_EQUAL(0, StmtTypeTable::GetNoOfStmtsOfType(CALL));

	StmtTypeTable::Insert(60, CALL);
	StmtTypeTable::Insert(61, ASSIGN);
	StmtTypeTable::Insert(62, WHILE);
	StmtTypeTable::Insert(63, IF);
	StmtTypeTable::Insert(64, CALL);

	CPPUNIT_ASSERT_EQUAL(7, StmtTypeTable::GetNoOfStmts());
	CPPUNIT_ASSERT_EQUAL(2, StmtTypeTable::GetNoOfStmtsOfType(ASSIGN));
	CPPUNIT_ASSERT_EQUAL(2, StmtTypeTable::GetNoOfStmtsOfType(WHILE));
	CPPUNIT_ASSERT_EQUAL(2, StmtTypeTable::GetNoOfStmtsOfType(CALL));
	CPPUNIT_ASSERT_EQUAL(1, StmtTypeTable::GetNoOfStmtsOfType(IF));
}
