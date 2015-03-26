#pragma once

#include "cppunit/extensions/HelperMacros.h"

class TokenizerTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE(TokenizerTest);
	/*CPPUNIT_TEST(TestSymbolTokens);
	CPPUNIT_TEST(TestKeywordTokens);
	CPPUNIT_TEST(TestNumberTokens);
	CPPUNIT_TEST(TestWordTokens);
	CPPUNIT_TEST(TestMixedTokens);*/
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void TestSymbolTokens();
	void TestKeywordTokens();
	void TestNumberTokens();
	void TestWordTokens();
	void TestMixedTokens();
};

