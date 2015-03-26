#pragma once

#define ProcTableTest_h

#include <cppunit\extensions\HelperMacros.h>

class ProcTableTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(ProcTableTest);
	/*CPPUNIT_TEST(TestInsertProc);
	CPPUNIT_TEST(TestGetIndexOfProc);
	CPPUNIT_TEST(TestGetProcName);
	CPPUNIT_TEST(TestGetAllProcNames);
	CPPUNIT_TEST(TestFirstLastStmtNos);
	CPPUNIT_TEST(TestGetProcOfStmt);*/
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void TestInsertProc();
	void TestGetIndexOfProc();
	void TestGetProcName();
	void TestGetAllProcNames();
	void TestFirstLastStmtNos();
	void TestGetProcOfStmt();
	
};

