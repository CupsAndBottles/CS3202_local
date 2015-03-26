#include "UltimateTest.h"
#include <cppunit\config\SourcePrefix.h>

#include <iostream>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(UltimateTest);

const string TESTFILE_DIRECTORY = "ParserTestFiles/";

void UltimateTest::setUp() { 
	VarTable::ClearData();
	StmtTypeTable::ClearData();
	Parent::ClearData();
	Follows::ClearData();
	Uses::ClearData();
	Modifies::ClearData();

	Parser::Parse(string(TESTFILE_DIRECTORY).append("ultimate-source.txt"));
}

void UltimateTest::tearDown() {}


void UltimateTest::TestSelectAll()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "assign a;while w;Select a";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	std::list<std::string> actualResultList;  
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 21, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("3");
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.push_back("8");
	actualResultList.push_back("10");
	actualResultList.push_back("12");
	actualResultList.push_back("13");
	actualResultList.push_back("14");
	actualResultList.push_back("16");
	actualResultList.push_back("17");
	actualResultList.push_back("18");
	actualResultList.push_back("19");
	actualResultList.push_back("20");
	actualResultList.push_back("22");
	actualResultList.push_back("23");
	actualResultList.push_back("25");
	actualResultList.push_back("26");
	actualResultList.push_back("27");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	resultList.clear();
	qd.ClearData();

	query = "constant c;Select c";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 7, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("0");
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("8");
	actualResultList.push_back("10");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	resultList.clear();
	qd.ClearData();

	query = "assign a;while w;Select w";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("6");
	actualResultList.push_back("9");
	actualResultList.push_back("11");
	actualResultList.push_back("15");
	actualResultList.push_back("21");
	actualResultList.push_back("24");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	
	resultList.clear();
	qd.ClearData();

	query = "assign a;while w;stmt s;Select s";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 27, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("3");
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("6");
	actualResultList.push_back("7");
	actualResultList.push_back("8");
	actualResultList.push_back("9");
	actualResultList.push_back("10");
	actualResultList.push_back("11");
	actualResultList.push_back("12");
	actualResultList.push_back("13");
	actualResultList.push_back("14");
	actualResultList.push_back("15");
	actualResultList.push_back("16");
	actualResultList.push_back("17");
	actualResultList.push_back("18");
	actualResultList.push_back("19");
	actualResultList.push_back("20");
	actualResultList.push_back("21");
	actualResultList.push_back("22");
	actualResultList.push_back("23");
	actualResultList.push_back("24");
	actualResultList.push_back("25");
	actualResultList.push_back("26");
	actualResultList.push_back("27");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	resultList.clear();
	qd.ClearData();

	query = "assign a;while w;prog_line p;Select p";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 27, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("3");
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("6");
	actualResultList.push_back("7");
	actualResultList.push_back("8");
	actualResultList.push_back("9");
	actualResultList.push_back("10");
	actualResultList.push_back("11");
	actualResultList.push_back("12");
	actualResultList.push_back("13");
	actualResultList.push_back("14");
	actualResultList.push_back("15");
	actualResultList.push_back("16");
	actualResultList.push_back("17");
	actualResultList.push_back("18");
	actualResultList.push_back("19");
	actualResultList.push_back("20");
	actualResultList.push_back("21");
	actualResultList.push_back("22");
	actualResultList.push_back("23");
	actualResultList.push_back("24");
	actualResultList.push_back("25");
	actualResultList.push_back("26");
	actualResultList.push_back("27");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);
	


	resultList.clear();
	qd.ClearData();

	query = "variable v;Select v";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 10, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("x");
	actualResultList.push_back("y");
	actualResultList.push_back("z");
	actualResultList.push_back("tEst");
	actualResultList.push_back("newVar");
	actualResultList.push_back("a");
	actualResultList.push_back("b");
	actualResultList.push_back("c");
	actualResultList.push_back("d");
	actualResultList.push_back("s");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);
}

void UltimateTest::TestModifies()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "assign a;variable v;Select a such that Modifies(a,v)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	std::list<std::string> actualResultList; 
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.push_back("6");
	actualResultList.push_back("8");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);

	
	qd.ClearData();
	resultList.clear();

	query = "stmt s;variable v;Select v such that Modifies(s,v)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 4, int(resultList.size()));

	actualResultList.clear();  
	actualResultList.push_back("a");
	actualResultList.push_back("tEst");
	actualResultList.push_back("x");
	actualResultList.push_back("y");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	
	qd.ClearData();
	resultList.clear();

	query = "prog_line n1,n2;variable v;Select n1 such that Modifies(n2,v)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 9, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("3");
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("6");
	actualResultList.push_back("7");
	actualResultList.push_back("8");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Modifies(w,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a such that Modifies(a,\"x\")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Modifies(w,\"tEst\")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("7");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "variable v;Select v such that Modifies(3,v)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();  
	actualResultList.push_back("a");
	actualResultList.push_back("tEst");
	actualResultList.push_back("y");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Modifies(8,\"tEst\")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Modifies(1,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "variable v;Select v such that Modifies(6,v)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("y");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;while w;Select w such that Modifies(a,\"x\")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);
}

void UltimateTest::TestPattern()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "assign a;Select a pattern a(_,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	std::list<std::string> actualResultList; 
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.push_back("6");
	actualResultList.push_back("8");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);

	
	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select v pattern a(_,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 8, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("a");
	actualResultList.push_back("b");
	actualResultList.push_back("c");
	actualResultList.push_back("newVar");
	actualResultList.push_back("tEst");
	actualResultList.push_back("x");
	actualResultList.push_back("y");
	actualResultList.push_back("z");
	
	CPPUNIT_ASSERT(resultList == actualResultList);

	qd.ClearData();
	resultList.clear();

	query = "assign a1, a2;Select a1 pattern a2(_,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select a pattern a(v,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.push_back("6");
	actualResultList.push_back("8");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select v pattern a(v,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 4, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("a");
	actualResultList.push_back("tEst");
	actualResultList.push_back("x");
	actualResultList.push_back("y");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a1,a2;variable v;Select a1 pattern a2(v,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);
	
	
	qd.ClearData();
	resultList.clear();

	query = "variable v;assign a;Select a pattern a(\"tEst\",_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("4");
	actualResultList.push_back("8");

	CPPUNIT_ASSERT(resultList == actualResultList);
	

	qd.ClearData();
	resultList.clear();

	query = "variable v;assign a;Select a pattern a(\"y\",_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("2");
	actualResultList.push_back("6");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "variable v;assign a;Select v pattern a(\"y\",_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 8, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("a");
	actualResultList.push_back("b");
	actualResultList.push_back("c");
	actualResultList.push_back("newVar");
	actualResultList.push_back("tEst");
	actualResultList.push_back("x");
	actualResultList.push_back("y");
	actualResultList.push_back("z");
	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "variable v;assign a;Select a pattern a(\"newVar\",_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);
	
	
	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a pattern a(_,_\"x + 1\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("2");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a pattern a(_,_\"1\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a pattern a(_,_\"tEst\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("6");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a pattern a(_,_ \" 2  +c\" _)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();
	CPPUNIT_ASSERT(resultList == actualResultList);


	CPPUNIT_ASSERT(resultList == actualResultList);

		qd.ClearData();
	resultList.clear();

	query = "assign a;Select a pattern a(_,_ \" b  +2\" _)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("9");

	CPPUNIT_ASSERT(resultList == actualResultList);

	
	qd.ClearData();
	resultList.clear();

	query = "variable v;assign a;Select a pattern a(v,_ \" 0\" _)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("8");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "variable v;assign a;Select v pattern a(v,_ \" 1\" _)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("tEst");
	actualResultList.push_back("x");
	actualResultList.push_back("y");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "variable v;assign a;Select v pattern a(v,_ \" z+x\" _)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("tEst");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();


	query = "variable v;assign a;Select a pattern a(\"tEst\",_\" z + x\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("4");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "variable v;assign a;Select a pattern a(\"tEst\",_\"  x +  1\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "variable v;assign a;Select a pattern a(\"y\",_\"  tEst\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("6");

	CPPUNIT_ASSERT(resultList == actualResultList);
	
}

void UltimateTest::TestCombination()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "assign a;variable v;Select a such that Modifies(a,v) pattern a(v,_\"z+x\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	std::list<std::string> actualResultList; 
	actualResultList.push_back("7");
	actualResultList.push_back("17");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);

	
	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select a such that Modifies(a,v) pattern a(v,_)";		

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 21, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("3");
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.push_back("8");
	actualResultList.push_back("10");
	actualResultList.push_back("12");
	actualResultList.push_back("13");
	actualResultList.push_back("14");
	actualResultList.push_back("16");
	actualResultList.push_back("17");
	actualResultList.push_back("18");
	actualResultList.push_back("19");
	actualResultList.push_back("20");
	actualResultList.push_back("22");
	actualResultList.push_back("23");
	actualResultList.push_back("25");
	actualResultList.push_back("26");
	actualResultList.push_back("27");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	/*qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select a such that Modifies(a,_) pattern a(\"newVar\",\"b+a+c\")";	

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("23");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);*/


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select a such that Modifies(a,_) pattern a(\"z\",_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("5");
	actualResultList.push_back("19");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select a such that Modifies(a,_) pattern a(_,_\"z+8\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("14");
	actualResultList.push_back("25");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select a such that Modifies(a,_) pattern a(_,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 21, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("3");
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.push_back("8");
	actualResultList.push_back("10");
	actualResultList.push_back("12");
	actualResultList.push_back("13");
	actualResultList.push_back("14");
	actualResultList.push_back("16");
	actualResultList.push_back("17");
	actualResultList.push_back("18");
	actualResultList.push_back("19");
	actualResultList.push_back("20");
	actualResultList.push_back("22");
	actualResultList.push_back("23");
	actualResultList.push_back("25");
	actualResultList.push_back("26");
	actualResultList.push_back("27");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select v such that Modifies(a,v) pattern a(v,_\"newVar\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("newVar");
	actualResultList.push_back("y");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select v such that Modifies(a,v) pattern a(\"b\",_\"c\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("b");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);
}