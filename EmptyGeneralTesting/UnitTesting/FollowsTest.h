#define FollowsTest_h

#include <cppunit\extensions\HelperMacros.h>

class FollowsTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(FollowsTest);
	
	//CPPUNIT_TEST(testConstructor);
	/*CPPUNIT_TEST(testSetFollows);
	CPPUNIT_TEST(testIsFollows);
	CPPUNIT_TEST(testGetFollowsBefore);
	CPPUNIT_TEST(testGetFollowsAfter);
	
	CPPUNIT_TEST(testIsFollowsT);
	CPPUNIT_TEST(testGetFollowsTBefore);
	CPPUNIT_TEST(testGetFollowsTAfter);
	CPPUNIT_TEST(testHasAnyFollows);*/
	// add other methods to test here
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	//void testConstructor();
	void testSetFollows();
	void testIsFollows();
	void testGetFollowsBefore();
	void testGetFollowsAfter();

	void testIsFollowsT();
	void testGetFollowsTBefore();
	void testGetFollowsTAfter();
	void testHasAnyFollows();
	
	// add other methods to test here

};