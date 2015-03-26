#define CallsTest_h

#include <cppunit\extensions\HelperMacros.h>

class CallsTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(CallsTest);

	/*CPPUNIT_TEST(testSetCalls);
	CPPUNIT_TEST(testIsCalls);
	CPPUNIT_TEST(testGetProcsCalledBy);
	CPPUNIT_TEST(testGetProcsCalling);
	CPPUNIT_TEST(testIsCallsT);
	CPPUNIT_TEST(testGetProcsCalledTBy);
	CPPUNIT_TEST(testGetProcsCallingT);*/

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testSetCalls();
	void testIsCalls();
	void testGetProcsCalledBy();
	void testGetProcsCalling();
	void testIsCallsT();
	void testGetProcsCalledTBy();
	void testGetProcsCallingT();
};

