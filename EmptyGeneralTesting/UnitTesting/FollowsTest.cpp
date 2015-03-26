#include <cppunit\config\SourcePrefix.h>

#include "..\SPA\PKB\Follows.h"
#include "FollowsTest.h"

#include <iostream>
#include <string>
#include <stdint.h>

void FollowsTest::setUp() {
	Follows::ClearData();
}

void FollowsTest::tearDown() {
	Follows::ClearData();
}

CPPUNIT_TEST_SUITE_REGISTRATION(FollowsTest);

/*void FollowsTest::testConstructor() {
	CPPUNIT_ASSERT_EQUAL(0, Follows::SizeOfFollows());

}*/ // not required in static class

void FollowsTest::testSetFollows() {
	Follows::ClearData();

	Follows::SetFollows(1, 2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular insert, size of table should increase by 1", 1, Follows::SizeOfFollows());

	Follows::SetFollows(1, 2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("inserting a repeated relationship, size should remain the same", 1, Follows::SizeOfFollows());

	// throws exception if > 1 stmt follows after/before

}

void FollowsTest::testIsFollows() {
	Follows::ClearData();

	Follows::SetFollows(3, 4);
	CPPUNIT_ASSERT_MESSAGE("regular check that's true", Follows::IsFollows(3, 4));
	CPPUNIT_ASSERT_MESSAGE("regular check that's false", !Follows::IsFollows(4, 3));
	CPPUNIT_ASSERT_MESSAGE("checking for a non-existent stmt", !Follows::IsFollows(5, 3));

}

void FollowsTest::testGetFollowsBefore() {
	Follows::ClearData();

	Follows::SetFollows(3, 4);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular GetFollowsBefore", 3, Follows::GetFollowsBefore(4));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular no stmt before", -1, Follows::GetFollowsBefore(3));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("finding stmt before an element that doesn't exist", -1, Follows::GetFollowsBefore(2));

}

void FollowsTest::testGetFollowsAfter() {
	Follows::ClearData();

	Follows::SetFollows(3, 4);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular GetFollowsAfter", 4, Follows::GetFollowsAfter(3));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular no stmt after", -1, Follows::GetFollowsAfter(4));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("finding stmt after an element that doesn't exist", -1, Follows::GetFollowsAfter(2));

}

void FollowsTest::testIsFollowsT() {
	Follows::ClearData();

	Follows::SetFollows(3, 5);
	Follows::SetFollows(5, 7);
	Follows::SetFollows(2, 4);

	CPPUNIT_ASSERT_MESSAGE("regular check that's true for 1 layer", Follows::IsFollowsT(3, 5));
	CPPUNIT_ASSERT_MESSAGE("regular check that's true for more than 1 layers", Follows::IsFollowsT(3, 7));
	CPPUNIT_ASSERT_MESSAGE("regular check that's false", !Follows::IsFollowsT(2, 3));

	// throw exception for non-existent stmt#?

}

void FollowsTest::testGetFollowsTBefore() {
	Follows::ClearData();
	vector<int> resultTestFollows;
	int resultSize;

	Follows::SetFollows(3, 4);
	Follows::SetFollows(4, 6);

	resultTestFollows = Follows::GetFollowsTBefore(4);
	resultSize = (int16_t) resultTestFollows.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows before for 1 layer", 1, resultSize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows beforefor 1 layer", 3, resultTestFollows.at(0));

	resultTestFollows = Follows::GetFollowsTBefore(6);
	resultSize = (int16_t) resultTestFollows.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows before for more than 1 layers", 2, resultSize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows before for more than 1 layers", 4, resultTestFollows.at(0));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows before for more than 1 layers", 3, resultTestFollows.at(1));

	resultTestFollows = Follows::GetFollowsTBefore(3);
	resultSize = (int16_t) resultTestFollows.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("no stmt following before", 0, resultSize);

	resultTestFollows = Follows::GetFollowsTBefore(2);
	resultSize = (int16_t) resultTestFollows.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("get stmt following before an element that doesn't exist", 0, resultSize);
	
}

void FollowsTest::testGetFollowsTAfter() {
	Follows::ClearData();
	vector<int> resultTestFollows;
	int resultSize;

	Follows::SetFollows(3, 5);
	Follows::SetFollows(5, 7);
	Follows::SetFollows(2, 4);

	resultTestFollows = Follows::GetFollowsTAfter(2);
	resultSize = (int16_t) resultTestFollows.size();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows after for 1 layer", 1, resultSize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows after for 1 layer", 4, resultTestFollows.at(0));

	resultTestFollows = Follows::GetFollowsTAfter(3);
	resultSize = (int16_t) resultTestFollows.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows after for more than 1 layers", 2, resultSize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows after for more than 1 layers", 5, resultTestFollows.at(0));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular follows after for more than 1 layers", 7, resultTestFollows.at(1));

	resultTestFollows = Follows::GetFollowsTAfter(7);
	resultSize = (int16_t) resultTestFollows.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("no stmt following after", 0, resultSize);

	resultTestFollows = Follows::GetFollowsTAfter(6);
	resultSize = (int16_t) resultTestFollows.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("get stmt following after an element that doesn't exist", 0, resultSize);

}

void FollowsTest::testHasAnyFollows() {
	Follows::ClearData();

	CPPUNIT_ASSERT_MESSAGE("regular has no follows", !Follows::HasAnyFollows());
	
	Follows::SetFollows(3, 4);
	CPPUNIT_ASSERT_MESSAGE("regular has follows", Follows::HasAnyFollows());

}
// insert other methods below here
// add a comment before each method to indicate its purpose