#pragma once

#include <cppunit\extensions\HelperMacros.h>

class ModifiesTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(ModifiesTest);
	/*CPPUNIT_TEST(TestNoStmt);
	CPPUNIT_TEST(TestSingleStmt);
	CPPUNIT_TEST(TestMultipleStmts);
	CPPUNIT_TEST(TestMultipleStmtsModifyingSingleVar);
	CPPUNIT_TEST(TestSingleStmtModifyingMultipleVars);
	CPPUNIT_TEST(TestModifies);

	CPPUNIT_TEST(TestNoProc);
	CPPUNIT_TEST(TestSingleProc);
	CPPUNIT_TEST(TestMultipleProcs);
	CPPUNIT_TEST(TestMultipleProcsModifyingSingleVar);
	CPPUNIT_TEST(TestSingleProcModifyingMultipleVars);*/

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	ModifiesTest();

	void TestNoStmt();
	void TestSingleStmt();
	void TestMultipleStmts();
	void TestMultipleStmtsModifyingSingleVar();
	void TestSingleStmtModifyingMultipleVars();
	void TestModifies();

	void TestNoProc();
	void TestSingleProc();
	void TestMultipleProcs();
	void TestMultipleProcsModifyingSingleVar();
	void TestSingleProcModifyingMultipleVars();
};

