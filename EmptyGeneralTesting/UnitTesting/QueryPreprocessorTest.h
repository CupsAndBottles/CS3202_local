#pragma once

#include <cppunit/extensions/HelperMacros.h>

class QueryPreProcessorTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( QueryPreProcessorTest );
	/*CPPUNIT_TEST( StringMatchingFunctionTest );
	CPPUNIT_TEST( HelperFunctionTest );
	CPPUNIT_TEST( ArgumentValidationTest );
	CPPUNIT_TEST( ClauseValidationTest );
	CPPUNIT_TEST( TokenizeTest );
	CPPUNIT_TEST( ActualValidationTest );*/
	CPPUNIT_TEST_SUITE_END();

public:
	QueryPreProcessorTest(void);
		
	void setUp();
	void tearDown();

	void StringMatchingFunctionTest();
	void HelperFunctionTest();
	void ArgumentValidationTest();
	void ClauseValidationTest();
	void TokenizeTest();
	void ActualValidationTest();
};

