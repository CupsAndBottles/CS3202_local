#define NextTest_h

#include <cppunit\extensions\HelperMacros.h>

class NextTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(NextTest);
	
	/*CPPUNIT_TEST(TestSetNext);
	CPPUNIT_TEST(TestIsNextBeforeBitVector);
	CPPUNIT_TEST(TestGetNextAfter);
	CPPUNIT_TEST(TestGetNextBefore);
	CPPUNIT_TEST(TestIsNextTBeforeBitVector1);
	CPPUNIT_TEST(TestIsNextTBeforeBitVector2);
	CPPUNIT_TEST(TestGetNextTAfter1);
	CPPUNIT_TEST(TestGetNextTAfter2);
	CPPUNIT_TEST(TestGetNextTBefore1);
	CPPUNIT_TEST(TestGetNextTBefore2);*/
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void TestSetNext();
	void TestIsNextBeforeBitVector();
	void TestGetNextAfter();
	void TestGetNextBefore();
	void TestIsNextTBeforeBitVector1();
	void TestIsNextTBeforeBitVector2();
	void TestGetNextTAfter1();
	void TestGetNextTAfter2();
	void TestGetNextTBefore1();
	void TestGetNextTBefore2();
	
};