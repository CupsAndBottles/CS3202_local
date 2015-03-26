#pragma once

#include "cppunit\extensions\HelperMacros.h"
#include "..\SPA\Program\TNode\TNode.h"

class PatternMatcherTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(PatternMatcherTest);
	/*CPPUNIT_TEST(matchSingleVariable);
	CPPUNIT_TEST(matchExprsWithTwoVariables);
	CPPUNIT_TEST(matchExprsWithMultipleVariables);*/
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	PatternMatcherTest();

	TNode* ParseExpr(string expr);

	void matchSingleVariable();
	void matchExprsWithTwoVariables();
	void matchExprsWithMultipleVariables();

};

