#pragma once

#include "PatternMatcherTest.h"
#include "..\SPA\Parser\Parser.h"
#include "..\SPA\Parser\Tokenizer.h"
#include "..\SPA\PKB\Uses.h"
#include "..\SPA\PKB\VarTable.h"
#include "..\SPA\PKB\ConstTable.h"
#include "..\SPA\QueryProcessor\PatternMatcher.h"

TNode* PatternMatcherTest::ParseExpr(string expr) {
	// use parser to construct a tree to test with
	// kinda hacky
	Parser parser(Tokenizer::Tokenize(expr.append(";"))); // need to append end of stmt
	parser.currentLineNumber = 1;
	TNode* exprTree = parser.ParseExpr(false);
	Uses::ClearData();
	VarTable::ClearData();
	ConstTable::ClearData();
	return exprTree;
}

void PatternMatcherTest::setUp() {
	Program::ClearData();
}

void PatternMatcherTest::tearDown() {
	Program::ClearData();
}

CPPUNIT_TEST_SUITE_REGISTRATION(PatternMatcherTest);

PatternMatcherTest::PatternMatcherTest() {
	// empty body
}

void PatternMatcherTest::matchSingleVariable() {
	string expr = "x";
	TNode* exprTree = ParseExpr(expr);

	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("x", nullptr, nullptr), false));
	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("x", nullptr, nullptr), true));
	CPPUNIT_ASSERT_EQUAL(false, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("y", nullptr, nullptr), true));
}

void PatternMatcherTest::matchExprsWithTwoVariables() {
	string expr = "x + y";
	TNode* exprTree = ParseExpr(expr);
	
	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("x", nullptr, nullptr), true));
	CPPUNIT_ASSERT_EQUAL(false, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("x", nullptr, nullptr), false));
	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("y", nullptr, nullptr), true));
	CPPUNIT_ASSERT_EQUAL(false, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("y", nullptr, nullptr), false));

	Pattern xPlusY("+", new Pattern("x", nullptr, nullptr), new Pattern("y", nullptr, nullptr));

	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, xPlusY, true));
	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, xPlusY, false));
}

void PatternMatcherTest::matchExprsWithMultipleVariables() {
	string expr = "x + 1 + y";
	TNode* exprTree = ParseExpr(expr);

	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("x", nullptr, nullptr), true));
	CPPUNIT_ASSERT_EQUAL(false, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("x", nullptr, nullptr), false));
	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("y", nullptr, nullptr), true));
	CPPUNIT_ASSERT_EQUAL(false, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("y", nullptr, nullptr), false));
	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("1", nullptr, nullptr), true));
	CPPUNIT_ASSERT_EQUAL(false, PatternMatcher::MatchPatternAtLeaves(exprTree, Pattern("1", nullptr, nullptr), false));

	Pattern xPlus1("+", new Pattern("x", nullptr, nullptr), new Pattern("1", nullptr, nullptr));
	Pattern xPlusY("+", new Pattern("x", nullptr, nullptr), new Pattern("y", nullptr, nullptr));
	Pattern yPlus1("+", new Pattern("y", nullptr, nullptr), new Pattern("1", nullptr, nullptr));

	CPPUNIT_ASSERT_EQUAL(true, PatternMatcher::MatchPatternAtLeaves(exprTree, xPlus1, true));
	CPPUNIT_ASSERT_EQUAL(false, PatternMatcher::MatchPatternAtLeaves(exprTree, xPlusY, true));
	CPPUNIT_ASSERT_EQUAL(false, PatternMatcher::MatchPatternAtLeaves(exprTree, yPlus1, true));
}

