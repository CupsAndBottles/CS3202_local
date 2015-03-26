#pragma once

#include <cppunit/extensions/HelperMacros.h>

class QueryDataTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( QueryDataTest );
	//CPPUNIT_TEST( testInsertQueryData );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testInsertQueryData();
};
