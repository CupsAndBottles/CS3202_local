#pragma once

#include <cppunit\extensions\HelperMacros.h>

class StmtTypeTableTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(StmtTypeTableTest);
	//CPPUNIT_TEST(TestGetAllStmtsOfType);
	//CPPUNIT_TEST(TestCheckIfStmtOfType);
	CPPUNIT_TEST(TestInsert);

	//add other methods to test here
	CPPUNIT_TEST_SUITE_END();
	
public:
	void setUp();
	void tearDown();

	void TestGetAllStmtsOfType();
	void TestCheckIfStmtOfType();
	void TestInsert();
	//add other methods to test here
};