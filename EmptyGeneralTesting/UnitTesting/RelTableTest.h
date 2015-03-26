#pragma once

#include <cppunit/extensions/HelperMacros.h>

class RelTableTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE( RelTableTest );
	//CPPUNIT_TEST( TestGetRelTableData );
	CPPUNIT_TEST_SUITE_END();

public:
	RelTableTest();

	void setUp();
	void tearDown();

	void TestGetRelTableData();
};