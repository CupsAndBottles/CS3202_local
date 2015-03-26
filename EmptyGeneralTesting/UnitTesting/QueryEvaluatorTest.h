#pragma once

#include <cppunit/extensions/HelperMacros.h>

class QueryEvaluatorTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( QueryEvaluatorTest );
	//CPPUNIT_TEST(TestHelperFunction);
	//CPPUNIT_TEST(TestMergeResult);
	CPPUNIT_TEST_SUITE_END();

public:
	QueryEvaluatorTest(void);

	void setUp();
	void tearDown();

	void TestHelperFunction();
	void TestMergeResult();
};

