#include <cppunit\config\SourcePrefix.h>

#include "..\SPA\PKB\Parent.h"
#include "ParentTest.h"

#include <iostream>
#include <string>
#include <stdint.h>

void ParentTest::setUp() {
	Parent::ClearData();
}

void ParentTest::tearDown() {
	Parent::ClearData();
}

CPPUNIT_TEST_SUITE_REGISTRATION(ParentTest);

void ParentTest::testConstructor() {
	Parent::ClearData();

	CPPUNIT_ASSERT_EQUAL(0, Parent::SizeOfParent());

}

void ParentTest::testSetParent() {
	Parent::ClearData();

	Parent::SetParent(1, 2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular insert, size of table should increase by 1", 1, Parent::SizeOfParent());

	Parent::SetParent(1, 2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("inserting a repeated relationship, size should remain the same", 1, Parent::SizeOfParent());

	Parent::SetParent(3, 2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("inserting a false relationship with one child having > 1 parent, size should remain the same", 1, Parent::SizeOfParent());

}

void ParentTest::testIsParent() {
	Parent::ClearData();

	Parent::SetParent(3, 4);
	CPPUNIT_ASSERT_MESSAGE("regular check that's true", Parent::IsParent(3, 4));
	CPPUNIT_ASSERT_MESSAGE("regular check that's false", !Parent::IsParent(4, 3));
	CPPUNIT_ASSERT_MESSAGE("checking for a non-existent stmt", !Parent::IsParent(5, 3));

}

void ParentTest::testGetParentOf() {
	Parent::ClearData();

	Parent::SetParent(3, 4);
	Parent::SetParent(3, 5);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular GetParent", 3, Parent::GetParentOf(4));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular GetParent", 3, Parent::GetParentOf(5));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular no Parent", -1, Parent::GetParentOf(3));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("finding parent of an element that doesn't exist", -1, Parent::GetParentOf(2));

}

void ParentTest::testGetChildrenOf() {
	Parent::ClearData();
	vector<int> resultTestParent;
	int resultSize;

	Parent::SetParent(3, 4);
	Parent::SetParent(3, 5);
	resultTestParent = Parent::GetChildrenOf(3);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular GetChildren", 2, resultSize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular GetChildren", 4, resultTestParent.at(0));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular GetChildren", 5, resultTestParent.at(1));

	resultTestParent = Parent::GetChildrenOf(4);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular GetChildren from an element with not children", 0, resultSize);

	resultTestParent = Parent::GetChildrenOf(2);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular GetChildren from an element that doesn't exist", 0, resultSize);

}

void ParentTest::testIsParentT() {
	Parent::ClearData();

	Parent::SetParent(3, 4);
	Parent::SetParent(3, 5);
	Parent::SetParent(4, 6);
	Parent::SetParent(5, 7);

	CPPUNIT_ASSERT_MESSAGE("regular check that's true for 1 generation", Parent::IsParentT(3, 5));
	CPPUNIT_ASSERT_MESSAGE("regular check that's true for more than 1 generation", Parent::IsParentT(3, 6));
	CPPUNIT_ASSERT_MESSAGE("regular check that's false", !Parent::IsParentT(5, 6));

}

void ParentTest::testGetParentTOf() {
	Parent::ClearData();
	vector<int> resultTestParent;
	int resultSize;

	Parent::SetParent(3, 4);
	Parent::SetParent(3, 5);
	Parent::SetParent(4, 6);
	Parent::SetParent(5, 7);
	Parent::SetParent(7, 8);

	resultTestParent = Parent::GetParentTOf(4);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular parent for 1 generation", 1, resultSize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular parent for 1 generation", 3, resultTestParent.at(0));

	resultTestParent = Parent::GetParentTOf(8);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular parent for more than 1 generation", 3, resultSize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular parent for more than 1 generation", 7, resultTestParent.at(0));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular parent for more than 1 generation", 5, resultTestParent.at(1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular parent for more than 1 generation", 3, resultTestParent.at(2));

	resultTestParent = Parent::GetParentTOf(3);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("no parent exists", 0, resultSize);

	resultTestParent = Parent::GetParentTOf(2);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("get parent of an element that doesn't exist", 0, resultSize);

}

void ParentTest::testGetChildrenTOf() {
	Parent::ClearData();
	vector<int> resultTestParent;
	int resultSize;

	Parent::SetParent(3, 4);
	Parent::SetParent(3, 5);
	Parent::SetParent(4, 6);
	Parent::SetParent(5, 7);
	Parent::SetParent(7, 8);

	resultTestParent = Parent::GetChildrenTOf(4);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular children for 1 generation", 1, resultSize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular children for 1 generation", 6, resultTestParent.at(0));
	
	resultTestParent = Parent::GetChildrenTOf(5);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular children for more than 1 generation", 2, resultSize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular children for more than 1 generation", 7, resultTestParent.at(0));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("regular children for more than 1 generation", 8, resultTestParent.at(1));

	resultTestParent = Parent::GetChildrenTOf(6);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("no children exists", 0, resultSize);

	resultTestParent = Parent::GetChildrenTOf(2);
	resultSize = (int16_t) resultTestParent.size();	// assumes 16-bit int
	CPPUNIT_ASSERT_EQUAL_MESSAGE("get children of an element that doesn't exist", 0, resultSize);
	
}

void ParentTest::testHasAnyParents() {
	Parent::ClearData();

	CPPUNIT_ASSERT_MESSAGE("regular has no parent", !Parent::HasAnyParents());
	
	Parent::SetParent(3, 4);
	CPPUNIT_ASSERT_MESSAGE("regular has parent", Parent::HasAnyParents());

}

// insert other methods below here
// add a comment before each method to indicate its purpose