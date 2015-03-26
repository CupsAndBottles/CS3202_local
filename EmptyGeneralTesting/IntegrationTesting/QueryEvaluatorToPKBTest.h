#pragma once

#include <cppunit\extensions\HelperMacros.h>

#include "..\SPA\Parser\Parser.h"
#include "..\SPA\PKB\VarTable.h"
#include "..\SPA\PKB\StmtTypeTable.h"
#include "..\SPA\PKB\Follows.h"
#include "..\SPA\PKB\Parent.h"
#include "..\SPA\PKB\Modifies.h"
#include "..\SPA\PKB\Uses.h"
#include "..\SPA\Program\Program.h"
#include "..\SPA\QueryProcessor\QueryData.h"
#include "..\SPA\QueryProcessor\QueryPreProcessor.h"
#include "..\SPA\QueryProcessor\QueryEvaluator.h"

class QueryEvaluatorToPKBTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(QueryEvaluatorToPKBTest);
	//CPPUNIT_TEST(TestSelectAll);
	//CPPUNIT_TEST(TestParent);
	//CPPUNIT_TEST(TestParentT);
	//CPPUNIT_TEST(TestFollows);
	//CPPUNIT_TEST(TestUses);
	//CPPUNIT_TEST(TestModifies);
	//CPPUNIT_TEST(TestPattern);
	//CPPUNIT_TEST(TestSuchThatPattern);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void TestSelectAll();
	void TestParent();
	void TestParentT();
	void TestFollows();
	void TestFollowsT();
	void TestModifies();
	void TestUses();

	void TestPattern();

	void TestSuchThatPattern();
};

