#pragma once

#include "cppunit\extensions\HelperMacros.h"

class UsesTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(UsesTest);
	/*CPPUNIT_TEST(TestSingleStmt);
	CPPUNIT_TEST(TestMultipleStmts);
	CPPUNIT_TEST(TestMultipleStmtsUsingSingleVar);
	CPPUNIT_TEST(TestSingleStmtUsingMultipleVars);
	CPPUNIT_TEST(TestUses);

	CPPUNIT_TEST(TestNoProc);
	CPPUNIT_TEST(TestSingleProc);
	CPPUNIT_TEST(TestMultipleProcs);
	CPPUNIT_TEST(TestMultipleProcsUsingSingleVar);
	CPPUNIT_TEST(TestSingleProcUsingMultipleVars);*/

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	UsesTest();

	void TestSingleStmt();
	void TestMultipleStmts();
	void TestMultipleStmtsUsingSingleVar();
	void TestSingleStmtUsingMultipleVars();
	void TestUses();

	void TestNoProc();
	void TestSingleProc();
	void TestMultipleProcs();
	void TestMultipleProcsUsingSingleVar();
	void TestSingleProcUsingMultipleVars();

};

