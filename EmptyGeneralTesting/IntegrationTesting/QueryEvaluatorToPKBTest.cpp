#include "QueryEvaluatorToPKBTest.h"
#include <cppunit\config\SourcePrefix.h>

#include <iostream>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(QueryEvaluatorToPKBTest);

const string TESTFILE_DIRECTORY = "ParserTestFiles/";

void QueryEvaluatorToPKBTest::setUp() { 
	VarTable::ClearData();
	StmtTypeTable::ClearData();
	Parent::ClearData();
	Follows::ClearData();
	Uses::ClearData();
	Modifies::ClearData();

	Parser::Parse(string(TESTFILE_DIRECTORY).append("pkbTest.txt"));
}

void QueryEvaluatorToPKBTest::tearDown() {}


void QueryEvaluatorToPKBTest::TestSelectAll()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "assign a;while w;Select a";

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


	resultList.clear();
	qd.ClearData();

	query = "constant c;Select c";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("0");
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	resultList.clear();
	qd.ClearData();

	query = "assign a;while w;Select w";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	resultList.clear();
	qd.ClearData();

	query = "assign a;while w;stmt s;Select s";

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



	resultList.clear();
	qd.ClearData();

	query = "assign a;while w;prog_line p;Select p";

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



	resultList.clear();
	qd.ClearData();

	query = "variable v;Select v";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 8, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("x");
	actualResultList.push_back("y");
	actualResultList.push_back("z");
	actualResultList.push_back("tEst");
	actualResultList.push_back("newVar");
	actualResultList.push_back("a");
	actualResultList.push_back("b");
	actualResultList.push_back("c");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);
}

void QueryEvaluatorToPKBTest::TestParent()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "assign a;while w;Select a such that Parent(w,a)";
	std::cout << "Test query : " << query << "\n";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));	
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 4, int(resultList.size()));

	std::list<std::string> actualResultList;  
	actualResultList.push_back("4");
	actualResultList.push_back("6");
	actualResultList.push_back("8");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "assign a;while w;Select w such that Parent(w,w)";
	//std::cout << "Test query : " << query << "\n";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);

	qd.ClearData();
	resultList.clear();

	query = "assign a;stmt s;Select s such that Parent(s,a)";
	//std::cout << "Test query : " << query << "\n";

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

	query = "assign a;while w;stmt s;Select a such that Parent(w,s)";

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

	query = "stmt s;Select s such that Parent(s,_)";

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

	query = "stmt s;Select s such that Parent(_,s)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	actualResultList.clear();
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

	query = "while w;Select w such that Parent(w,5)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "stmt s;Select s such that Parent(3,s)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 4, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "prog_line n;Select n such that Parent(_,9)";

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

	query = "prog_line n;Select n such that Parent(1,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);
	


	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Parent(5,6)";

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

	query = "while w;Select w such that Parent(_,_)";

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

void QueryEvaluatorToPKBTest::TestParentT()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "assign a;while w;Select a such that Parent*(w,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 4, int(resultList.size()));

	std::list<std::string> actualResultList;  
	actualResultList.push_back("4");
	actualResultList.push_back("6");
	actualResultList.push_back("8");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "assign a;while w;Select w such that Parent*(w,w)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "assign a;stmt s;Select s such that Parent*(s,a)";

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

	query = "assign a;while w;stmt s;Select s such that Parent*(w,s)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	actualResultList.clear();
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

	query = "stmt s;Select s such that Parent*(s,_)";

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

	query = "stmt s;Select s such that Parent*(_,s)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	actualResultList.clear();
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

	query = "while w;Select w such that Parent*(w,6)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	
	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a such that Parent*(3,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 4, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("4");
	actualResultList.push_back("6");
	actualResultList.push_back("8");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	
	qd.ClearData();
	resultList.clear();

	query = "prog_line n;Select n such that Parent*(_,9)";

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

	query = "prog_line n;Select n such that Parent*(1,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);
	


	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Parent*(3,8)";

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

	query = "while w;Select w such that Parent*(_,_)";

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
/*
void QueryEvaluatorToPKBTest::TestFollows()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "stmt s;while w;Select s such that Follows(w,s)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	std::list<std::string> actualResultList;
	actualResultList.push_back("7");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "stmt s;Select s such that Follows(s,s)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "assign a1, a2;Select a1 such that Follows(a1,a2)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList;
	actualResultList.push_back("1");

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "assign a;while w1,w2;Select a such that Follows(w1,w2)";

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

	query = "prog_line n;Select n such that Follows(n,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 5, int(resultList.size()));

	actualResultList.clear();  
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "prog_line n;Select n such that Follows(_,n)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 5, int(resultList.size()));

	actualResultList.clear();  
	actualResultList.push_back("2");
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "stmt s;Select s such that Follows(s,2)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();  
	actualResultList.push_back("1");

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "while w;prog_line n;Select w such that Follows(3,n)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();  

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "prog_line n;Select n such that Follows(_,1)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();  

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Follows(1,_)";

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

	query = "while w;Select w such that Follows(3,4)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();  

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Follows(_,_)";

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
*/
void QueryEvaluatorToPKBTest::TestModifies()
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

void QueryEvaluatorToPKBTest::TestUses()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "assign a;variable v;Select a such that Uses(a,v)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 4, int(resultList.size()));

	std::list<std::string> actualResultList; 
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.push_back("6");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);

	
	qd.ClearData();
	resultList.clear();

	query = "prog_line n;variable v;Select v such that Uses(n,v)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("x");
	actualResultList.push_back("z");
	actualResultList.push_back("tEst");
	actualResultList.push_back("newVar");
	actualResultList.push_back("b");
	actualResultList.push_back("c");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	
	qd.ClearData();
	resultList.clear();

	query = "prog_line n1,n2;variable v;Select n1 such that Uses(n2,v)";

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

	query = "stmt s;Select s such that Uses(s,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 7, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("2");
	actualResultList.push_back("3");
	actualResultList.push_back("4");
	actualResultList.push_back("5");
	actualResultList.push_back("6");
	actualResultList.push_back("7");
	actualResultList.push_back("9");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a such that Uses(a,\"x\")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);

	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a such that Uses(a,\"b\")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("9");

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Uses(w,\"newVar\")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.sort();

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "variable v;Select v such that Uses(8,v)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();
	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "while w;Select w such that Uses(3,\"tEst\")";

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

	query = "while w;Select w such that Uses(1,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);
}

void QueryEvaluatorToPKBTest::TestPattern()
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

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.push_back("6");
	actualResultList.push_back("8");
	actualResultList.push_back("9");

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

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 6, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("2");
	actualResultList.push_back("4");
	actualResultList.push_back("6");
	actualResultList.push_back("8");
	actualResultList.push_back("9");

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

void QueryEvaluatorToPKBTest::TestSuchThatPattern()
{
	QueryData qd;
	QueryPreProcessor qv;
	QueryEvaluator qe;
	std::list<std::string> resultList; 

	qd.ClearData();

	std::string query = "assign a;Select a pattern a(_,_\"1\"_) such that Modifies(a,\"  tEst\")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd)); 
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	std::list<std::string> actualResultList; 
	actualResultList.push_back("4");

	CPPUNIT_ASSERT(resultList == actualResultList);

	
	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a pattern a(_,_\"0\"_) such that Modifies(a,\"  tEst\")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("8");

	CPPUNIT_ASSERT(resultList == actualResultList);



	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select a such that Modifies(a,\"tEst\") pattern a(v,_\"1\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("4");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select v such that Modifies(a,\"tEst\") pattern a(v,_\"1\"_)";

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

	query = "assign a;variable v;Select v such that Modifies(a,v) pattern a(v,_\"1\"_)";

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

	query = "assign a;Select a pattern a(\" y \",_) such that Uses(a,\" tEst \")";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd)); 
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList)); 

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("6");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select v such that Uses(a,v) pattern a(v,_\"1\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("x");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v1,v2;Select v1 such that Uses(a,v1) pattern a(v2,_\"1\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("x");
	actualResultList.push_back("z");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select v such that Uses(a,v) pattern a(v,_\"x\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select v such that Uses(a,v) pattern a(v,_\"1\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("x");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;variable v;Select v such that Uses(a,v) pattern a(v,_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("tEst");
	actualResultList.push_back("x");

	CPPUNIT_ASSERT(resultList == actualResultList);
	

	qd.ClearData();
	resultList.clear();

	query = "while w;assign a;Select w such that Parent(w,a) pattern a(_,_\"z+x\"_)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "while w;assign a;variable v;Select a pattern a(v,_\"x + 1\"_) such that Parent(w,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();
	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "while w;assign a;Select w pattern a(_,_\"tEst\"_) such that Parent(w,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("5");
	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "stmt s;assign a;Select s pattern a(\"y\",_) such that Parent*(s,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "while w;assign a;Select w pattern a(_,_\"tEst\"_) such that Parent(w,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("5");
	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;Select a pattern a(_,_\"0\"_) such that Parent*(3,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd)); 
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("8");

	CPPUNIT_ASSERT(resultList == actualResultList);

	qd.ClearData();
	resultList.clear();

	query = "assign a;while w;Select w pattern a(_,_\"0\"_) such that Parent(w,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd)); 
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList)); 

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("7");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;stmt s;Select s pattern a(_,_\"x+1\"_) such that Parent(_,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 0, int(resultList.size()));

	actualResultList.clear();

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a;while w;Select w pattern a(_,_\"z+x\"_) such that Parent(_,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 3, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.push_back("7");

	CPPUNIT_ASSERT(resultList == actualResultList);
	
	
	qd.ClearData();
	resultList.clear();

	query = "while w;assign a;Select a pattern a(_,_\"b+2\"_) such that Follows(w,a)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("9");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "while w;assign a1,a2;Select a1 pattern a1(_,_\"z+x\"_) such that Follows*(a1,a2)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("4");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "while w;assign a1,a2;Select a1 pattern a1(_,_\"1\"_) such that Follows*(a1,a2)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 2, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");
	actualResultList.push_back("4");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "stmt s;assign a;Select s pattern a(_,_\"1\"_) such that Follows*(a,s)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 5, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("2");
	actualResultList.push_back("3");
	actualResultList.push_back("5");
	actualResultList.push_back("7");
	actualResultList.push_back("9");

	CPPUNIT_ASSERT(resultList == actualResultList);


	qd.ClearData();
	resultList.clear();

	query = "assign a1,a2;Select a2 pattern a1(_,_\"x+1\"_) such that Follows(a2,a1)";

	CPPUNIT_ASSERT_MESSAGE("Query is valid", qv.ValidateQuery(query, qd));
	CPPUNIT_ASSERT_MESSAGE("Query is successfully evaluated", qe.EvaluateQuery(qd, resultList));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Number of results is correct", 1, int(resultList.size()));

	actualResultList.clear();
	actualResultList.push_back("1");

	CPPUNIT_ASSERT(resultList == actualResultList);
}
