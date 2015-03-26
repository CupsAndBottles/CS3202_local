#pragma once
#define ConstTableTest_h

#include <cppunit\extensions\HelperMacros.h>


class ConstTableTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(ConstTableTest);

	/*CPPUNIT_TEST(testSetStmtUsesConst);
	CPPUNIT_TEST(testIsStmtUsingConst);
	CPPUNIT_TEST(testGetStmtUsingConst);
	CPPUNIT_TEST(testGetConstUsedByStmt);

	CPPUNIT_TEST(testHasAnyConst);
	CPPUNIT_TEST(testGetAllConst);
	CPPUNIT_TEST(testSizeOfConstTable);
	CPPUNIT_TEST(testClearData);*/
	// add other methods to test here
	CPPUNIT_TEST_SUITE_END();

public:
	ConstTableTest(void);
	void setUp();
	void tearDown();

	void testSetStmtUsesConst();
	void testIsStmtUsingConst();
	void testGetStmtUsingConst();
	void testGetConstUsedByStmt();

	void testHasAnyConst();
	void testGetAllConst();
	void testSizeOfConstTable();
	void testClearData();
	// add other methods to test here

	
};

