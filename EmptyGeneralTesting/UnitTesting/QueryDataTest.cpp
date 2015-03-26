#pragma once

#include <cppunit/config/SourcePrefix.h>

#include "QueryDataTest.h"
#include "..\SPA\QueryProcessor\QueryData.h"

#include <vector>

using namespace std;

void QueryDataTest::setUp() {};
void QueryDataTest::tearDown() {};

CPPUNIT_TEST_SUITE_REGISTRATION(QueryDataTest);

void QueryDataTest::testInsertQueryData() 
{
	//assign a; while w; Select a such that Parent(w,a) pattern a("x",_"x+y"_)

	QueryData qd;

	Synonym a("a", ASSIGN);
	Synonym w("w", WHILE);
	Argument arg1_suchthat("w", SYNONYM, w);
	Argument arg2_suchthat("a", SYNONYM, a);
	Argument arg1_pattern("x", IDENT);
	Argument arg2_pattern("_\"x+y\"_", EXPRESSION);

	qd.InsertDeclaration(a);
	qd.InsertDeclaration(w);
	qd.InsertSelect(a);
	qd.InsertPattern(a, arg1_pattern, arg2_pattern);
	qd.InsertSuchThat(PARENT, arg1_suchthat, arg2_suchthat);


	std::vector<Declaration> declarations = qd.GetDeclarations();
	CPPUNIT_ASSERT(declarations.size() == 2);
	CPPUNIT_ASSERT(declarations.at(0).synonym.value == "a");
	CPPUNIT_ASSERT(declarations.at(0).synonym.type == ASSIGN);
	CPPUNIT_ASSERT(declarations.at(1).synonym.value == "w");
	CPPUNIT_ASSERT(declarations.at(1).synonym.type == WHILE);

	std::vector<SelectClause> selects = qd.GetSelects();
	CPPUNIT_ASSERT(selects.size() == 1);
	CPPUNIT_ASSERT(selects.at(0).synonym.value == "a");
	CPPUNIT_ASSERT(selects.at(0).synonym.type == ASSIGN);

	std::vector<PatternClause> patterns = qd.GetPatterns();
	CPPUNIT_ASSERT(patterns.size() == 1);
	CPPUNIT_ASSERT(patterns.at(0).synonym.value == "a");
	CPPUNIT_ASSERT(patterns.at(0).synonym.type == ASSIGN);
	CPPUNIT_ASSERT(patterns.at(0).arg1.value == "x");
	CPPUNIT_ASSERT(patterns.at(0).arg1.type == IDENT);
	CPPUNIT_ASSERT(patterns.at(0).arg1.syn.value == "");
	CPPUNIT_ASSERT(patterns.at(0).arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(patterns.at(0).arg2.value == "_\"x+y\"_");
	CPPUNIT_ASSERT(patterns.at(0).arg2.type == EXPRESSION);
	CPPUNIT_ASSERT(patterns.at(0).arg2.syn.value == "");
	CPPUNIT_ASSERT(patterns.at(0).arg2.syn.type == INVALID_SYNONYM_TYPE);

	std::vector<SuchThatClause> suchThats = qd.GetSuchThats();
	CPPUNIT_ASSERT(suchThats.size() == 1);
	CPPUNIT_ASSERT(suchThats.at(0).relationship == PARENT);
	CPPUNIT_ASSERT(suchThats.at(0).arg1.value == "w");
	CPPUNIT_ASSERT(suchThats.at(0).arg1.type == SYNONYM);
	CPPUNIT_ASSERT(suchThats.at(0).arg1.syn.value == "w");
	CPPUNIT_ASSERT(suchThats.at(0).arg1.syn.type == WHILE);
	CPPUNIT_ASSERT(suchThats.at(0).arg2.value == "a");
	CPPUNIT_ASSERT(suchThats.at(0).arg2.type == SYNONYM);
	CPPUNIT_ASSERT(suchThats.at(0).arg2.syn.value == "a");
	CPPUNIT_ASSERT(suchThats.at(0).arg2.syn.type == ASSIGN);


	//bool IsSynonymExist(std::string, SynonymType);
	bool exist = qd.IsSynonymExist("a", ASSIGN);
	CPPUNIT_ASSERT(exist == true);

	exist = qd.IsSynonymExist("b", ASSIGN);
	CPPUNIT_ASSERT(exist == false);


	//bool IsSynonymExist(std::string, SynonymType*);
	SynonymType type = INVALID_SYNONYM_TYPE;
	exist = qd.IsSynonymExist("a", &type);
	CPPUNIT_ASSERT(exist == true);
	CPPUNIT_ASSERT(type == ASSIGN);

	type = INVALID_SYNONYM_TYPE;
	exist = qd.IsSynonymExist("c", &type);
	CPPUNIT_ASSERT(exist == false);
	CPPUNIT_ASSERT(type == INVALID_SYNONYM_TYPE);


	//bool IsSynonymExist(std::string, std::vector<SynonymType>);
	SynonymType list[] = {STMT, ASSIGN, WHILE, PROG_LINE, CONSTANT, VARIABLE};
	std::vector<SynonymType> typeList(list, list + 6);

	exist = qd.IsSynonymExist("a",list);
	CPPUNIT_ASSERT(exist == true);

	exist = qd.IsSynonymExist("w",list);
	CPPUNIT_ASSERT(exist == true);

	exist = qd.IsSynonymExist("p",list);
	CPPUNIT_ASSERT(exist == false);

	SynonymType list2[] = {STMT, PROG_LINE, CONSTANT, VARIABLE};
	std::vector<SynonymType> typeList2(list2, list2 + 4);

	exist = qd.IsSynonymExist("a",typeList2);
	CPPUNIT_ASSERT(exist == false);

	exist = qd.IsSynonymExist("w",typeList2);
	CPPUNIT_ASSERT(exist == false);
}