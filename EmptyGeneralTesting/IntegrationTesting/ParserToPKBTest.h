#define ParserToPKBTest_h

#include <cppunit\extensions\HelperMacros.h>

class ParserToPKBTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(ParserToPKBTest);

	/*CPPUNIT_TEST(TestNothing);
	CPPUNIT_TEST(TestVarTable);
	CPPUNIT_TEST(TestModifies);
	CPPUNIT_TEST(TestUses);
	CPPUNIT_TEST(TestParent);
	CPPUNIT_TEST(TestFollows);
	//CPPUNIT_TEST(TestNext);
	CPPUNIT_TEST(TestModifiesForProcs);
	CPPUNIT_TEST(TestUsesForProcs);
	CPPUNIT_TEST(TestStmtTypeTable);*/
	// insert methods in here
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	ParserToPKBTest();
	
	void TestNothing();
	void TestVarTable();
	void TestModifies();
	void TestUses();
	void TestParent();
	void TestFollows();
	void TestNext();
	void TestModifiesForProcs();
	void TestUsesForProcs();
	void TestStmtTypeTable();

	// add other methods to test here

};