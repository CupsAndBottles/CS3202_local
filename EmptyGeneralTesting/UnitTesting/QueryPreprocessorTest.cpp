#include "QueryPreProcessorTest.h"
#include <cppunit/config/SourcePrefix.h>
#include "..\SPA\QueryProcessor\QueryPreProcessor.h"

using namespace std;

void QueryPreProcessorTest::setUp() {};
void QueryPreProcessorTest::tearDown() {};

CPPUNIT_TEST_SUITE_REGISTRATION(QueryPreProcessorTest);

QueryPreProcessorTest::QueryPreProcessorTest(void) {}

void QueryPreProcessorTest::ActualValidationTest()
{
	QueryPreProcessor qv;
	QueryData qd;

	std::string query = "assign a;while w;Select a";
	//std::string query = "assign a;while w;Select a such that Parent(w,a)";
	qd.ClearData();

	bool valid = qv.ValidateQuery(query, qd);
	CPPUNIT_ASSERT(valid == true);
	
	int size = qd.GetDeclarations().size();
	CPPUNIT_ASSERT(size == 2);

	CPPUNIT_ASSERT(qd.GetDeclarations().at(0).synonym.value == "a");
	CPPUNIT_ASSERT(qd.GetDeclarations().at(0).synonym.type == ASSIGN);
	CPPUNIT_ASSERT(qd.GetDeclarations().at(1).synonym.value == "w");
	CPPUNIT_ASSERT(qd.GetDeclarations().at(1).synonym.type == WHILE);

	size = qd.GetSelects().size();
	CPPUNIT_ASSERT(size == 1);

	CPPUNIT_ASSERT(qd.GetSelects().at(0).synonym.value == "a");
	CPPUNIT_ASSERT(qd.GetSelects().at(0).synonym.type == ASSIGN);
	
	size = qd.GetSuchThats().size();
	CPPUNIT_ASSERT(size == 0);

	size = qd.GetPatterns().size();
	CPPUNIT_ASSERT(size == 0);



	query = "stmt s;variable v;Select s such that Modifies(s,v)";
	qd.ClearData();

	valid = qv.ValidateQuery(query, qd);
	CPPUNIT_ASSERT(valid == true);
	
	size = qd.GetDeclarations().size();
	CPPUNIT_ASSERT(size == 2);

	CPPUNIT_ASSERT(qd.GetDeclarations().at(0).synonym.value == "s");
	CPPUNIT_ASSERT(qd.GetDeclarations().at(0).synonym.type == STMT);
	CPPUNIT_ASSERT(qd.GetDeclarations().at(1).synonym.value == "v");
	CPPUNIT_ASSERT(qd.GetDeclarations().at(1).synonym.type == VARIABLE);

	size = qd.GetSelects().size();
	CPPUNIT_ASSERT(size == 1);

	CPPUNIT_ASSERT(qd.GetSelects().at(0).synonym.value == "s");
	CPPUNIT_ASSERT(qd.GetSelects().at(0).synonym.type == STMT);

	size = qd.GetSuchThats().size();
	CPPUNIT_ASSERT(size == 1);

	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).relationship == MODIFIES);
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg1.value == "s");
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg1.type == SYNONYM);
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg1.syn.value == "s");
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg1.syn.type == STMT);
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg2.value == "v");
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg2.type == SYNONYM);
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg2.syn.value == "v");
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg2.syn.type == VARIABLE);
	
	size = qd.GetPatterns().size();
	CPPUNIT_ASSERT(size == 0);


	query = "while w; assign a; Select w pattern a(\" g3 \" , _\" x\" _)";
	qd.ClearData();

	valid = qv.ValidateQuery(query, qd);
	CPPUNIT_ASSERT(valid == true);
	
	size = qd.GetDeclarations().size();
	CPPUNIT_ASSERT(size == 2);

	CPPUNIT_ASSERT(qd.GetDeclarations().at(0).synonym.value == "w");
	CPPUNIT_ASSERT(qd.GetDeclarations().at(0).synonym.type == WHILE);
	CPPUNIT_ASSERT(qd.GetDeclarations().at(1).synonym.value == "a");
	CPPUNIT_ASSERT(qd.GetDeclarations().at(1).synonym.type == ASSIGN);

	size = qd.GetSelects().size();
	CPPUNIT_ASSERT(size == 1);

	CPPUNIT_ASSERT(qd.GetSelects().at(0).synonym.value == "w");
	CPPUNIT_ASSERT(qd.GetSelects().at(0).synonym.type == WHILE);

	size = qd.GetSuchThats().size();
	CPPUNIT_ASSERT(size == 0);
	
	size = qd.GetPatterns().size();
	CPPUNIT_ASSERT(size == 1);

	CPPUNIT_ASSERT(qd.GetPatterns().at(0).synonym.value == "a");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).synonym.type == ASSIGN);
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg1.value == "\"g3\"");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg1.type == IDENT);
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg1.syn.value == "");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg2.value == "_\"x\"_");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg2.type == EXPRESSION);
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg2.syn.value == "");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg2.syn.type == INVALID_SYNONYM_TYPE);



	query = "assign a ; Select a such that Modifies(a,\"jj\") pattern a(_ , _\" x+ y\" _)";
	qd.ClearData();

	valid = qv.ValidateQuery(query, qd);
	CPPUNIT_ASSERT(valid == true);
	
	size = qd.GetDeclarations().size();
	CPPUNIT_ASSERT(size == 1);

	CPPUNIT_ASSERT(qd.GetDeclarations().at(0).synonym.value == "a");
	CPPUNIT_ASSERT(qd.GetDeclarations().at(0).synonym.type == ASSIGN);

	size = qd.GetSelects().size();
	CPPUNIT_ASSERT(size == 1);

	CPPUNIT_ASSERT(qd.GetSelects().at(0).synonym.value == "a");
	CPPUNIT_ASSERT(qd.GetSelects().at(0).synonym.type == ASSIGN);

	size = qd.GetSuchThats().size();
	CPPUNIT_ASSERT(size == 1);

	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).relationship == MODIFIES);
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg1.value == "a");
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg1.type == SYNONYM);
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg1.syn.value == "a");
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg1.syn.type == ASSIGN);
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg2.value == "\"jj\"");
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg2.type == IDENT);
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg2.syn.value == "");
	CPPUNIT_ASSERT(qd.GetSuchThats().at(0).arg2.syn.type == INVALID_SYNONYM_TYPE);
	
	size = qd.GetPatterns().size();
	CPPUNIT_ASSERT(size == 1);

	CPPUNIT_ASSERT(qd.GetPatterns().at(0).synonym.value == "a");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).synonym.type == ASSIGN);
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg1.value == "_");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg1.type == UNDERSCORE);
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg1.syn.value == "");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg2.value == "_\"x+y\"_");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg2.type == EXPRESSION);
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg2.syn.value == "");
	CPPUNIT_ASSERT(qd.GetPatterns().at(0).arg2.syn.type == INVALID_SYNONYM_TYPE); 
}

void QueryPreProcessorTest::TokenizeTest()
{
	QueryPreProcessor qv;
	std::vector<std::string> tokenList;
	std::string query = "assign a;while w;Select a such that Parent(w,a)";

	//tokenize query
	qv.Tokenize(query,tokenList);

	int size = tokenList.size();

	/*cout << "\n" << size << "\n";
	for(std::vector<std::string>::iterator it = tokenList.begin(); it!=tokenList.end(); ++it)
		cout << *it << "\n";*/

	CPPUNIT_ASSERT(size == 13);
	CPPUNIT_ASSERT(tokenList.at(0) == "assign");
	CPPUNIT_ASSERT(tokenList.at(1) == "a");
	CPPUNIT_ASSERT(tokenList.at(2) == ";");
	CPPUNIT_ASSERT(tokenList.at(3) == "while");
	CPPUNIT_ASSERT(tokenList.at(4) == "w");
	CPPUNIT_ASSERT(tokenList.at(5) == ";");
	CPPUNIT_ASSERT(tokenList.at(6) == "Select");
	CPPUNIT_ASSERT(tokenList.at(7) == "a");
	CPPUNIT_ASSERT(tokenList.at(8) == "such");
	CPPUNIT_ASSERT(tokenList.at(9) == "that");
	CPPUNIT_ASSERT(tokenList.at(10) == "Parent");
	CPPUNIT_ASSERT(tokenList.at(11) == "w");
	CPPUNIT_ASSERT(tokenList.at(12) == "a");


	tokenList.clear();
	query = "stmt s;Select s such that Parent(s,_ )";

	qv.Tokenize(query,tokenList);

	size = tokenList.size();
	/*cout << "\n" << size << "\n";
	for(std::vector<std::string>::iterator it = tokenList.begin(); it!=tokenList.end(); ++it)
		cout << *it << "\n";*/

	CPPUNIT_ASSERT(size == 10);
	CPPUNIT_ASSERT(tokenList.at(0) == "stmt");
	CPPUNIT_ASSERT(tokenList.at(1) == "s");
	CPPUNIT_ASSERT(tokenList.at(2) == ";");
	CPPUNIT_ASSERT(tokenList.at(3) == "Select");
	CPPUNIT_ASSERT(tokenList.at(4) == "s");
	CPPUNIT_ASSERT(tokenList.at(5) == "such");
	CPPUNIT_ASSERT(tokenList.at(6) == "that");
	CPPUNIT_ASSERT(tokenList.at(7) == "Parent");
	CPPUNIT_ASSERT(tokenList.at(8) == "s");
	CPPUNIT_ASSERT(tokenList.at(9) == "_");

	tokenList.clear();
	query = "stmt s;Select s such that Parent(_,_ ) pattern a(\"x\" , \" (x + y)* z\")";

	qv.Tokenize(query,tokenList);

	size = tokenList.size();
	/*cout << "\n" << size << "\n";
	for(std::vector<std::string>::iterator it = tokenList.begin(); it!=tokenList.end(); ++it)
		cout << *it << "\n";*/

	CPPUNIT_ASSERT(size == 14);
	CPPUNIT_ASSERT(tokenList.at(0) == "stmt");
	CPPUNIT_ASSERT(tokenList.at(1) == "s");
	CPPUNIT_ASSERT(tokenList.at(2) == ";");
	CPPUNIT_ASSERT(tokenList.at(3) == "Select");
	CPPUNIT_ASSERT(tokenList.at(4) == "s");
	CPPUNIT_ASSERT(tokenList.at(5) == "such");
	CPPUNIT_ASSERT(tokenList.at(6) == "that");
	CPPUNIT_ASSERT(tokenList.at(7) == "Parent");
	CPPUNIT_ASSERT(tokenList.at(8) == "_");
	CPPUNIT_ASSERT(tokenList.at(9) == "_");
	CPPUNIT_ASSERT(tokenList.at(10) == "pattern");
	CPPUNIT_ASSERT(tokenList.at(11) == "a");
	CPPUNIT_ASSERT(tokenList.at(12) == "\"x\"");
	CPPUNIT_ASSERT(tokenList.at(13) == "\" (x + y)* z\"");

	tokenList.clear();
	query = "stmt s###; Select s pattern a44 (\"x123\" , _ \" (f - ( x + y )) * z\") _";

	qv.Tokenize(query,tokenList);

	size = tokenList.size();
	/*cout << "\n" << size << "\n";
	for(std::vector<std::string>::iterator it = tokenList.begin(); it!=tokenList.end(); ++it)
		cout << *it << "\n";*/

	CPPUNIT_ASSERT(size == 9);
	CPPUNIT_ASSERT(tokenList.at(0) == "stmt");
	CPPUNIT_ASSERT(tokenList.at(1) == "s###");
	CPPUNIT_ASSERT(tokenList.at(2) == ";");
	CPPUNIT_ASSERT(tokenList.at(3) == "Select");
	CPPUNIT_ASSERT(tokenList.at(4) == "s");
	CPPUNIT_ASSERT(tokenList.at(5) == "pattern");
	CPPUNIT_ASSERT(tokenList.at(6) == "a44");
	CPPUNIT_ASSERT(tokenList.at(7) == "\"x123\"");
	CPPUNIT_ASSERT(tokenList.at(8) == "_ \" (f - ( x + y )) * z\") _");



	tokenList.clear();
	query = "assign a1,a2;Select a1 such that Follows(a1,a2)";

	qv.Tokenize(query,tokenList);

	size = tokenList.size();
	/*cout << "\n" << size << "\n";
	for(std::vector<std::string>::iterator it = tokenList.begin(); it!=tokenList.end(); ++it)
		cout << *it << "\n";*/

	CPPUNIT_ASSERT(size == 11);
	CPPUNIT_ASSERT(tokenList.at(0) == "assign");
	CPPUNIT_ASSERT(tokenList.at(1) == "a1");
	CPPUNIT_ASSERT(tokenList.at(2) == "a2");
	CPPUNIT_ASSERT(tokenList.at(3) == ";");
	CPPUNIT_ASSERT(tokenList.at(4) == "Select");
	CPPUNIT_ASSERT(tokenList.at(5) == "a1");
	CPPUNIT_ASSERT(tokenList.at(6) == "such");
	CPPUNIT_ASSERT(tokenList.at(7) == "that");
	CPPUNIT_ASSERT(tokenList.at(8) == "Follows");
	CPPUNIT_ASSERT(tokenList.at(9) == "a1");
	CPPUNIT_ASSERT(tokenList.at(10) == "a2");


	tokenList.clear();
	query = "assign  a  ;    Select a pattern a ( \" x \" , _ \" y + 5 \" _  )";

	qv.Tokenize(query,tokenList);

	size = tokenList.size();
	/*cout << "\n" << size << "\n";
	for(std::vector<std::string>::iterator it = tokenList.begin(); it!=tokenList.end(); ++it)
		cout << *it << "\n";*/

	CPPUNIT_ASSERT(size == 9);
	CPPUNIT_ASSERT(tokenList.at(0) == "assign");
	CPPUNIT_ASSERT(tokenList.at(1) == "a");
	CPPUNIT_ASSERT(tokenList.at(2) == ";");
	CPPUNIT_ASSERT(tokenList.at(3) == "Select");
	CPPUNIT_ASSERT(tokenList.at(4) == "a");
	CPPUNIT_ASSERT(tokenList.at(5) == "pattern");
	CPPUNIT_ASSERT(tokenList.at(6) == "a");
	CPPUNIT_ASSERT(tokenList.at(7) == "\" x \"");
	CPPUNIT_ASSERT(tokenList.at(8) == "_ \" y + 5 \" _");

	
	tokenList.clear();
	query = "assign a;while w;Select a such that Parent(w,a) pattern a(_,_\"y+5\"_)";

	//tokenize query
	qv.Tokenize(query,tokenList);

	size = tokenList.size();
	CPPUNIT_ASSERT(size == 17);
	CPPUNIT_ASSERT(tokenList.at(0) == "assign");
	CPPUNIT_ASSERT(tokenList.at(1) == "a");
	CPPUNIT_ASSERT(tokenList.at(2) == ";");
	CPPUNIT_ASSERT(tokenList.at(3) == "while");
	CPPUNIT_ASSERT(tokenList.at(4) == "w");
	CPPUNIT_ASSERT(tokenList.at(5) == ";");
	CPPUNIT_ASSERT(tokenList.at(6) == "Select");
	CPPUNIT_ASSERT(tokenList.at(7) == "a");
	CPPUNIT_ASSERT(tokenList.at(8) == "such");
	CPPUNIT_ASSERT(tokenList.at(9) == "that");
	CPPUNIT_ASSERT(tokenList.at(10) == "Parent");
	CPPUNIT_ASSERT(tokenList.at(11) == "w");
	CPPUNIT_ASSERT(tokenList.at(12) == "a");
	CPPUNIT_ASSERT(tokenList.at(13) == "pattern");
	CPPUNIT_ASSERT(tokenList.at(14) == "a");
	CPPUNIT_ASSERT(tokenList.at(15) == "_");
	CPPUNIT_ASSERT(tokenList.at(16) == "_\"y+5\"_");


		
	tokenList.clear();
	query = "assign a;Select a";

	//tokenize query
	qv.Tokenize(query,tokenList);

	size = tokenList.size();
	CPPUNIT_ASSERT(size == 5);
	CPPUNIT_ASSERT(tokenList.at(0) == "assign");
	CPPUNIT_ASSERT(tokenList.at(1) == "a");
	CPPUNIT_ASSERT(tokenList.at(2) == ";");
	CPPUNIT_ASSERT(tokenList.at(3) == "Select");
	CPPUNIT_ASSERT(tokenList.at(4) == "a");


	tokenList.clear();
	query = "assign a1,a2,a3;Select <a1,a2,a3>";

	//tokenize query
	qv.Tokenize(query,tokenList);

	size = tokenList.size();
	CPPUNIT_ASSERT(size == 11);
	CPPUNIT_ASSERT(tokenList.at(0) == "assign");
	CPPUNIT_ASSERT(tokenList.at(1) == "a1");
	CPPUNIT_ASSERT(tokenList.at(2) == "a2");
	CPPUNIT_ASSERT(tokenList.at(3) == "a3");
	CPPUNIT_ASSERT(tokenList.at(4) == ";");
	CPPUNIT_ASSERT(tokenList.at(5) == "Select");
	CPPUNIT_ASSERT(tokenList.at(6) == "<");
	CPPUNIT_ASSERT(tokenList.at(7) == "a1");
	CPPUNIT_ASSERT(tokenList.at(8) == "a2");
	CPPUNIT_ASSERT(tokenList.at(9) == "a3");
	CPPUNIT_ASSERT(tokenList.at(10) == ">");


	tokenList.clear();
	query = "assign a  ; while w , w1 ;  variable v  ;  Select w such that Modifies( 3 , w1 ) pattern a( \"  y  \"  , _ \" y + 5  \" _)";

	qv.Tokenize(query,tokenList);

	size = tokenList.size();

	CPPUNIT_ASSERT(size == 21);
	CPPUNIT_ASSERT(tokenList.at(0) == "assign");
	CPPUNIT_ASSERT(tokenList.at(1) == "a");
	CPPUNIT_ASSERT(tokenList.at(2) == ";");
	CPPUNIT_ASSERT(tokenList.at(3) == "while");
	CPPUNIT_ASSERT(tokenList.at(4) == "w");
	CPPUNIT_ASSERT(tokenList.at(5) == "w1");
	CPPUNIT_ASSERT(tokenList.at(6) == ";");
	CPPUNIT_ASSERT(tokenList.at(7) == "variable");
	CPPUNIT_ASSERT(tokenList.at(8) == "v");
	CPPUNIT_ASSERT(tokenList.at(9) == ";");
	CPPUNIT_ASSERT(tokenList.at(10) == "Select");
	CPPUNIT_ASSERT(tokenList.at(11) == "w");
	CPPUNIT_ASSERT(tokenList.at(12) == "such");
	CPPUNIT_ASSERT(tokenList.at(13) == "that");
	CPPUNIT_ASSERT(tokenList.at(14) == "Modifies");
	CPPUNIT_ASSERT(tokenList.at(15) == "3");
	CPPUNIT_ASSERT(tokenList.at(16) == "w1");
	CPPUNIT_ASSERT(tokenList.at(17) == "pattern");
	CPPUNIT_ASSERT(tokenList.at(18) == "a");
	CPPUNIT_ASSERT(tokenList.at(19) == "\"  y  \"");
	CPPUNIT_ASSERT(tokenList.at(20) == "_ \" y + 5  \" _");

	tokenList.clear();
	std::stringstream ss;
	ss << "assign a  ,a2; while w , w1 ;  variable v  ; if i ";
	ss << "Select <a, w,v> such that Modifies( 3 , \"x\" ) and Affect*( n ,a2) ";
	ss << "pattern a( \"  y  \"  , _ \" y *5 -7 \" _) and pattern i(\"y\",_,_) and pattern w(_,_) ";
	ss << "with a.stmt# = a2.stmt# and c.value = 5 and v.varName = \"lol\" p.procName = v.varName and n = 5;";
	std::cout << ss;
	query = ss.str();

	qv.Tokenize(query,tokenList);

	std::vector<std::string> correctList;
	correctList.push_back("assign"); 
	correctList.push_back("a"); 
	correctList.push_back("a2"); 
	correctList.push_back(";"); 
	correctList.push_back("while"); 
	correctList.push_back("w");  
	correctList.push_back("w1"); 
	correctList.push_back(";"); 
	correctList.push_back("variable"); 
	correctList.push_back("v"); 
	correctList.push_back(";"); 
	correctList.push_back("if"); 
	correctList.push_back("i"); 

	//ss << "Select <a, w,v> such that Modifies( 3 , \"x\" ) and Affect*( n ,a2) ";
	correctList.push_back("Select"); 
	correctList.push_back("<"); 
	correctList.push_back("a"); 
	correctList.push_back("w"); 
	correctList.push_back("v"); 
	correctList.push_back(">"); 
	correctList.push_back("such"); 
	correctList.push_back("that"); 
	correctList.push_back("Modifies"); 
	correctList.push_back("3"); 
	correctList.push_back("\"x\""); 
	correctList.push_back("and"); 
	correctList.push_back("Affect*"); 
	correctList.push_back("n"); 
	correctList.push_back("a2"); 
	
	//ss << "pattern a( \"  y  \"  , _ \" y + 5  \" _) and pattern i(\"y\",_,_) and pattern w(_,_) ";
	correctList.push_back("pattern"); 
	correctList.push_back("a"); 
	correctList.push_back("\"  y  \""); 
	correctList.push_back("_ \" y *5 -7 \" _"); 
	correctList.push_back("and"); 
	correctList.push_back("pattern"); 
	correctList.push_back("i"); 
	correctList.push_back("\"y\""); 
	correctList.push_back("_"); 
	correctList.push_back("_"); 
	correctList.push_back("and"); 
	correctList.push_back("pattern"); 
	correctList.push_back("w"); 
	correctList.push_back("_"); 
	correctList.push_back("_"); 

	//ss << "with a.stmt# = a2.stmt# and c.value = 5 and v.varName = \"lol\" p.procName = v.varName and n = 5;";
	correctList.push_back("with"); 
	correctList.push_back("a.stmt#"); 
	correctList.push_back("="); 
	correctList.push_back("a2.stmt#"); 
	correctList.push_back("and"); 
	correctList.push_back("c.value"); 
	correctList.push_back("="); 
	correctList.push_back("5"); 
	correctList.push_back("and"); 
	correctList.push_back("v.varName"); 
	correctList.push_back("="); 
	correctList.push_back("\"lol\""); 
	correctList.push_back("p.procName"); 
	correctList.push_back("=");
	correctList.push_back("v.varName"); 
	correctList.push_back("and"); 
	correctList.push_back("n");
	correctList.push_back("="); 
	correctList.push_back("5"); 
	correctList.push_back(";"); 

	//std::cout << "tokenList size: " << tokenList.size() << "\n";
	//std::cout << "correctList size: " << correctList.size() << "\n";


/*	for(int i=0; i<tokenList.size(); ++i) {
		//std::cout << "Testing " << tokenList[i] << " and " << correctList[i] << "\n";
		std::cout << tokenList[i] << "\n";
	}*/

	CPPUNIT_ASSERT(tokenList.size() == correctList.size());

	for(int i=0; i<tokenList.size(); ++i) {
		//std::cout << "Testing " << tokenList[i] << " and " << correctList[i] << "\n";
		CPPUNIT_ASSERT(tokenList[i] == correctList[i]);
	}
}

void QueryPreProcessorTest::ClauseValidationTest()
{
	QueryPreProcessor qv;
	QueryData qd;
	Synonym syn;
	syn.value = "a";

	qd.ClearData();

	//bool QueryPreProcessor::ValidateDeclaration(Synonym &synonym, std::string type)
	bool valid = qv.ValidateDeclaration(syn, "assign");
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(syn.type == ASSIGN);

	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateDeclaration(syn, "stmt");
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(syn.type == STMT);

	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateDeclaration(syn, "while");
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(syn.type == WHILE);

	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateDeclaration(syn, "variable");
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(syn.type == VARIABLE);

	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateDeclaration(syn, "constant");
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(syn.type == CONSTANT);

	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateDeclaration(syn, "prog_line");
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(syn.type == PROG_LINE);

	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateDeclaration(syn, "LOL");
	CPPUNIT_ASSERT(valid == false);
	CPPUNIT_ASSERT(syn.type == INVALID_SYNONYM_TYPE);

	//duplicated declaration
	qd.InsertDeclaration(Synonym("a",ASSIGN));
	qd.InsertDeclaration(Synonym("w",WHILE));

	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateDeclaration(syn, "assign");
	CPPUNIT_ASSERT(valid == false);
	CPPUNIT_ASSERT(syn.type == INVALID_SYNONYM_TYPE);

	syn.value = "w";
	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateDeclaration(syn, "while");
	CPPUNIT_ASSERT(valid == false);
	CPPUNIT_ASSERT(syn.type == INVALID_SYNONYM_TYPE);

	
	//bool QueryPreProcessor::ValidateSelect(Synonym &synonym)
	qd.ClearData();
	qd.InsertDeclaration(Synonym("a",ASSIGN));
	qd.InsertDeclaration(Synonym("w",WHILE));

	syn.value = "a";
	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateSelect(syn);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(syn.type == ASSIGN);

	syn.value = "w";
	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateSelect(syn);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(syn.type == WHILE);

	syn.value = "s";
	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateSelect(syn);
	CPPUNIT_ASSERT(valid == false);
	CPPUNIT_ASSERT(syn.type == INVALID_SYNONYM_TYPE);

	//empty declaration
	qd.ClearData();
	syn.value = "a";
	syn.type = INVALID_SYNONYM_TYPE;
	valid = qv.ValidateSelect(syn);
	CPPUNIT_ASSERT(valid == false);
	CPPUNIT_ASSERT(syn.type == INVALID_SYNONYM_TYPE);


	//bool QueryPreProcessor::ValidatePattern(Synonym synonym, Argument &arg1, Argument &arg2)
	qd.ClearData();
	qd.InsertDeclaration(Synonym("a",ASSIGN));
	qd.InsertDeclaration(Synonym("w",WHILE));
	qd.InsertDeclaration(Synonym("v",VARIABLE));

	//pattern a( Synonym | _ | "Ident" , _ | _"Expression"_ )

	syn.value = "a";
	syn.type = ASSIGN;
	Argument arg1, arg2;
	arg1.value = "v";
	arg2.value = "_";
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.syn.value == "v");
	CPPUNIT_ASSERT(arg1.syn.type == VARIABLE);
	CPPUNIT_ASSERT(arg2.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	//argument 1 synonym is not a variable
	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "a";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);
	CPPUNIT_ASSERT(arg1.type == INVALID_ARGUMENT_TYPE);
	CPPUNIT_ASSERT(arg1.syn.value == "");
	CPPUNIT_ASSERT(arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(arg2.type == INVALID_ARGUMENT_TYPE);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "v";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_\"x+y\"_";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.syn.value == "v");
	CPPUNIT_ASSERT(arg1.syn.type == VARIABLE);
	CPPUNIT_ASSERT(arg2.type == EXPRESSION);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	//argument 2 expression invalid
	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "v";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_\"x+y\"";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.syn.value == "v");
	CPPUNIT_ASSERT(arg1.syn.type == VARIABLE);
	CPPUNIT_ASSERT(arg2.type == INVALID_ARGUMENT_TYPE);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);


	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "_";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(arg1.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg1.syn.value == "");
	CPPUNIT_ASSERT(arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(arg2.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	//invalid underscore
	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "__";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);

	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "_";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_ \" x + y \"   _";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(arg1.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg1.syn.value == "");
	CPPUNIT_ASSERT(arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(arg2.type == EXPRESSION);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "\"x\"";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(arg1.type == IDENT);
	CPPUNIT_ASSERT(arg1.syn.value == "");
	CPPUNIT_ASSERT(arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(arg2.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "\"x3\"";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_ \" x \"   _";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(arg1.type == IDENT);
	CPPUNIT_ASSERT(arg1.syn.value == "");
	CPPUNIT_ASSERT(arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(arg2.type == EXPRESSION);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	//invalid ident
	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "\"1x\"";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_ \" x \"   _";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidatePattern(syn, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);


	//bool QueryPreProcessor::ValidateRelationship(std::string rel, RelationshipType &rel_enum, Argument &arg1, Argument &arg2)

	/*
	Modifies(Synonym | Integer , Synonym | _ | "Ident")
	Uses	(Synonym | Integer , Synonym | _ | "Ident")
	Argument 1 synonym - stmt, assign, while, prog_line
	Argument 2 synonym - variable
	*/

	qd.ClearData();
	qd.InsertDeclaration(Synonym("a",ASSIGN));
	qd.InsertDeclaration(Synonym("w",WHILE));
	qd.InsertDeclaration(Synonym("v",VARIABLE));
	
	std::string rel = "Modifies";
	RelationshipType rel_enum;
	arg1.value = "a";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "v";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(rel_enum == MODIFIES);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.syn.value == "a");
	CPPUNIT_ASSERT(arg1.syn.type == ASSIGN);
	CPPUNIT_ASSERT(arg2.type == SYNONYM);
	CPPUNIT_ASSERT(arg2.syn.value == "v");
	CPPUNIT_ASSERT(arg2.syn.type == VARIABLE);

	//invalid argument 1 synonym type
	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "v";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "v";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);

	//invalid argument 1 value
	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "k";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "v";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);

	//invalid argument 2 synonym type
	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "s";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "a";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);

	//invalid relationship name
	rel = "Modifie";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "w";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "v";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);

	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "a";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(rel_enum == MODIFIES);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.syn.value == "a");
	CPPUNIT_ASSERT(arg1.syn.type == ASSIGN);
	CPPUNIT_ASSERT(arg2.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "a";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "\"x\"";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(rel_enum == MODIFIES);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.syn.value == "a");
	CPPUNIT_ASSERT(arg1.syn.type == ASSIGN);
	CPPUNIT_ASSERT(arg2.type == IDENT);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	//invalid ident
	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "a";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "\"3x\"";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);

	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "5";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "v";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(rel_enum == MODIFIES);
	CPPUNIT_ASSERT(arg1.type == INTEGER);
	CPPUNIT_ASSERT(arg1.syn.value == "");
	CPPUNIT_ASSERT(arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(arg2.type == SYNONYM);
	CPPUNIT_ASSERT(arg2.syn.value == "v");
	CPPUNIT_ASSERT(arg2.syn.type == VARIABLE);

	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "5";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "_";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(rel_enum == MODIFIES);
	CPPUNIT_ASSERT(arg1.type == INTEGER);
	CPPUNIT_ASSERT(arg1.syn.value == "");
	CPPUNIT_ASSERT(arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(arg2.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);

	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "5";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "\"xyz\"";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == true);
	CPPUNIT_ASSERT(rel_enum == MODIFIES);
	CPPUNIT_ASSERT(arg1.type == INTEGER);
	CPPUNIT_ASSERT(arg1.syn.value == "");
	CPPUNIT_ASSERT(arg1.syn.type == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(arg2.type == IDENT);
	CPPUNIT_ASSERT(arg2.syn.value == "");
	CPPUNIT_ASSERT(arg2.syn.type == INVALID_SYNONYM_TYPE);


	//invalid arg1 type
	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "_";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "\"xyz\"";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);

	//invalid arg2 type
	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "a";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "5";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);

	//invalid arg2 type
	rel = "Modifies";
	rel_enum = INVALID_RELATIONSHIP_TYPE;
	arg1.value = "a";
	arg1.type = INVALID_ARGUMENT_TYPE;
	arg1.syn = Synonym();
	arg2.value = "\"x+y\"";
	arg2.type = INVALID_ARGUMENT_TYPE;
	arg2.syn = Synonym();
	valid = qv.ValidateRelationship(rel, rel_enum, arg1, arg2);
	CPPUNIT_ASSERT(valid == false);
}

void QueryPreProcessorTest::ArgumentValidationTest()
{
	QueryPreProcessor qd;

	//Function in test : bool IsExpression(std::string)
	bool matched = qd.IsExpression("\"x\"");
	CPPUNIT_ASSERT(matched == true);
	
	matched = qd.IsExpression("\"xyz\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("\"x50\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("\"x388x3fg\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("\"45\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("\"0\"");
	CPPUNIT_ASSERT(matched == true);

	//matched = qd.IsExpression("\"25jj\"");
	//CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("\"=\"");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("\"ss3=\"");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("\"-58\"");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("\"x+y\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("\" x + y \"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("\" 5 + 7\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("\"4+a1\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("\" w +2\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("_");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("_\"\"_");		//_""_
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("_\" \"_");		//_" "_
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("_\"  _");		//_"  _
	CPPUNIT_ASSERT(matched == false);
	
	matched = qd.IsExpression("__");			//__
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("_\"x\"_");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("_\"50\"_");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("_\"a50\"_");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("_\"x+y\"_");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("_ \" x + y \" _");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("_\"x*y\"_");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsExpression("_\"x+y\"");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("\"x+y\"_");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("\"x-y\"_");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("\"x - y\"_");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("\"(x-y)*z\"_");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsExpression("_\"=,-9,v=0,5\"");
	CPPUNIT_ASSERT(matched == false);

	//Function in test : IsIdent(std::string)
	matched = qd.IsIdent("\"x\"");
	CPPUNIT_ASSERT(matched == true);
	
	matched = qd.IsIdent("\"xyz\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsIdent("\"x50\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsIdent("\"x388x3fg\"");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsIdent("\"45\"");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsIdent("\"25jj\"");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsIdent("\"=\"");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsIdent("\"ss3=\"");
	CPPUNIT_ASSERT(matched == false);
	
	
	//Function in test : bool IsName(std::string)
	matched = qd.IsName("x");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsName("xyz");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsName("x50");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsName("x388x3fg");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsName("45");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsName("25jj");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsName("=");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsName("ss3=");
	CPPUNIT_ASSERT(matched == false);


	//Function in test : bool IsInteger(const std::string&);

	matched = qd.IsInteger("5");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsInteger("0");
	CPPUNIT_ASSERT(matched == true);

	matched = qd.IsInteger("25jj");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsInteger("50x");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsInteger("x");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsInteger("=");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsInteger("-24");
	CPPUNIT_ASSERT(matched == false);

	matched = qd.IsInteger("0.4");
	CPPUNIT_ASSERT(matched == false);
}

void QueryPreProcessorTest::StringMatchingFunctionTest()
{
	QueryPreProcessor qv;

	//Function in test : bool IsDeclaration(std::string);
	
	bool matched = qv.IsDeclaration("stmt");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsDeclaration("assign");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsDeclaration("while");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsDeclaration("variable");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsDeclaration("prog_line");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsDeclaration("constant");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsDeclaration("lol");
	CPPUNIT_ASSERT(matched == false);

	
	//Function in test : bool IsDeclaration(std::string);
	
	matched = qv.IsRelationship("Modifies");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsRelationship("Uses");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsRelationship("Parent");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsRelationship("Parent*");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsRelationship("Follows");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsRelationship("Follows*");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsDeclaration("lol");
	CPPUNIT_ASSERT(matched == false);


	//Function in test : bool IsSelect(std::string);
	
	matched = qv.IsSelect("Select");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsSelect("lol");
	CPPUNIT_ASSERT(matched == false);


	//Function in test : bool IsSemiColon(std::string);
	
	matched = qv.IsSemiColon(";");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsSemiColon("lol");
	CPPUNIT_ASSERT(matched == false);


	//Function in test : bool IsPattern(std::string);
	
	matched = qv.IsPattern("pattern");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsPattern("lol");
	CPPUNIT_ASSERT(matched == false);


	//Function in test : bool IsSuchThat(std::string);
	
	matched = qv.IsSuchThat("such");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsSuchThat("lol");
	CPPUNIT_ASSERT(matched == false);

	
	//Function in test : bool IsUnderscore(std::string);
	
	matched = qv.IsUnderscore("_");
	CPPUNIT_ASSERT(matched == true);

	matched = qv.IsUnderscore("lol");
	CPPUNIT_ASSERT(matched == false);
}

void QueryPreProcessorTest::HelperFunctionTest()
{
	QueryPreProcessor qv;
	
	//Function in test : bool GetEnumRelationshipType(std::string, RelationshipType&);
	
	RelationshipType rt = INVALID_RELATIONSHIP_TYPE;
	bool exist = qv.GetEnumRelationshipType("Modifies", rt);
	CPPUNIT_ASSERT(rt == MODIFIES);
	CPPUNIT_ASSERT(exist == true);

	rt = INVALID_RELATIONSHIP_TYPE;
	exist = qv.GetEnumRelationshipType("Uses", rt);
	CPPUNIT_ASSERT(rt == USES);
	CPPUNIT_ASSERT(exist == true);

	rt = INVALID_RELATIONSHIP_TYPE;
	exist = qv.GetEnumRelationshipType("Parent", rt);
	CPPUNIT_ASSERT(rt == PARENT);
	CPPUNIT_ASSERT(exist == true);

	rt = INVALID_RELATIONSHIP_TYPE;
	exist = qv.GetEnumRelationshipType("Parent*", rt);
	CPPUNIT_ASSERT(rt == PARENTT);
	CPPUNIT_ASSERT(exist == true);

	rt = INVALID_RELATIONSHIP_TYPE;
	exist = qv.GetEnumRelationshipType("Follows", rt);
	CPPUNIT_ASSERT(rt == FOLLOWS);
	CPPUNIT_ASSERT(exist == true);

	rt = INVALID_RELATIONSHIP_TYPE;
	exist = qv.GetEnumRelationshipType("Follows*", rt);
	CPPUNIT_ASSERT(rt == FOLLOWST);
	CPPUNIT_ASSERT(exist == true);

	//Invalid
	rt = INVALID_RELATIONSHIP_TYPE;
	exist = qv.GetEnumRelationshipType("LOL", rt);
	CPPUNIT_ASSERT(rt == INVALID_RELATIONSHIP_TYPE);
	CPPUNIT_ASSERT(exist == false);


	//Function in test : bool GetEnumSynonymType(std::string, SynonymType&)
	
	SynonymType st = INVALID_SYNONYM_TYPE;
	exist = qv.GetEnumSynonymType("assign", st);
	CPPUNIT_ASSERT(st == ASSIGN);
	CPPUNIT_ASSERT(exist == true);

	st = INVALID_SYNONYM_TYPE;
	exist = qv.GetEnumSynonymType("stmt", st);
	CPPUNIT_ASSERT(st == STMT);
	CPPUNIT_ASSERT(exist == true);

	st = INVALID_SYNONYM_TYPE;
	exist = qv.GetEnumSynonymType("while", st);
	CPPUNIT_ASSERT(st == WHILE);
	CPPUNIT_ASSERT(exist == true);

	st = INVALID_SYNONYM_TYPE;
	exist = qv.GetEnumSynonymType("variable", st);
	CPPUNIT_ASSERT(st == VARIABLE);
	CPPUNIT_ASSERT(exist == true);

	st = INVALID_SYNONYM_TYPE;
	exist = qv.GetEnumSynonymType("prog_line", st);
	CPPUNIT_ASSERT(st == PROG_LINE);
	CPPUNIT_ASSERT(exist == true);

	st = INVALID_SYNONYM_TYPE;
	exist = qv.GetEnumSynonymType("constant", st);
	CPPUNIT_ASSERT(st == CONSTANT);
	CPPUNIT_ASSERT(exist == true);

	//Invalid
	st = INVALID_SYNONYM_TYPE;
	exist = qv.GetEnumSynonymType("lol", st);
	CPPUNIT_ASSERT(st == INVALID_SYNONYM_TYPE);
	CPPUNIT_ASSERT(exist == false);






	QueryData qd;
	qd.ClearData();
	qd.InsertDeclaration(Synonym("a",ASSIGN));
	qd.InsertDeclaration(Synonym("w",WHILE));
	qd.InsertDeclaration(Synonym("s",STMT));
	qd.InsertDeclaration(Synonym("ifstat",IF));
	qd.InsertDeclaration(Synonym("v",VARIABLE));
	qd.InsertDeclaration(Synonym("p",PROCEDURE));
	qd.InsertDeclaration(Synonym("c",CONSTANT));

	//valid
	std::string attrRef = "a.stmt#";
	Synonym syn;
	AttrNameType attrName;
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == true);
	CPPUNIT_ASSERT(syn.value == "a");
	CPPUNIT_ASSERT(syn.type == ASSIGN);
	CPPUNIT_ASSERT(attrName == STMTNUM);

	attrRef = "s.stmt#";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == true);
	CPPUNIT_ASSERT(syn.value == "s");
	CPPUNIT_ASSERT(syn.type == STMT);
	CPPUNIT_ASSERT(attrName == STMTNUM);

	attrRef = "w.stmt#";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == true);
	CPPUNIT_ASSERT(syn.value == "w");
	CPPUNIT_ASSERT(syn.type == WHILE);
	CPPUNIT_ASSERT(attrName == STMTNUM);

	attrRef = "ifstat.stmt#";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == true);
	CPPUNIT_ASSERT(syn.value == "ifstat");
	CPPUNIT_ASSERT(syn.type == IF);
	CPPUNIT_ASSERT(attrName == STMTNUM);

	attrRef = "v.varName";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == true);
	CPPUNIT_ASSERT(syn.value == "v");
	CPPUNIT_ASSERT(syn.type == VARIABLE);
	CPPUNIT_ASSERT(attrName == VARNAME);

	attrRef = "p.procName";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == true);
	CPPUNIT_ASSERT(syn.value == "p");
	CPPUNIT_ASSERT(syn.type == PROCEDURE);
	CPPUNIT_ASSERT(attrName == PROCNAME);

	attrRef = "c.value";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == true);
	CPPUNIT_ASSERT(syn.value == "c");
	CPPUNIT_ASSERT(syn.type == CONSTANT);
	CPPUNIT_ASSERT(attrName == VALUE);

	//invalid 
	//undeclared synonym
	attrRef = "a1.stmt#";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	//no dot
	attrRef = "astmt#";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	//not a attrRef
	attrRef = "n";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	//incorrect attrName 
	attrRef = "a.value";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "a.varName";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "a.procName";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);


	attrRef = "v.value";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "v.stmt#";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "v.procName";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);
	

	attrRef = "p.value";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "p.stmt#";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "p.varName";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);


	attrRef = "c.stmt#";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "c.varName";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "c.procName";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);


	//invalid attrName
	attrRef = "s.stmt";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "w.Stmt#";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "c.val";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);

	attrRef = "p.procname";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);


	attrRef = "v.varname";
	CPPUNIT_ASSERT(qv.IsValidAttrRef(attrRef , syn , attrName) == false);
}

void QueryPreProcessorTest::ValidateRelationshipTest()
{
	QueryPreProcessor qp;
	QueryData qd;
	qd.ClearData();
	qd.InsertDeclaration(Synonym("a",ASSIGN));
	qd.InsertDeclaration(Synonym("a1",ASSIGN));
	qd.InsertDeclaration(Synonym("a2",ASSIGN));
	qd.InsertDeclaration(Synonym("w",WHILE));
	qd.InsertDeclaration(Synonym("w1",WHILE));
	qd.InsertDeclaration(Synonym("w2",WHILE));
	qd.InsertDeclaration(Synonym("s",STMT));
	qd.InsertDeclaration(Synonym("ifstat",IF));
	qd.InsertDeclaration(Synonym("v",VARIABLE));
	qd.InsertDeclaration(Synonym("p",PROCEDURE));
	qd.InsertDeclaration(Synonym("c",CONSTANT));
	qd.InsertDeclaration(Synonym("n",PROG_LINE));
	
	//*****************************VALID*********************************
	std::string rel;
	Argument arg1, arg2;;
	RelationshipType rel_enum = INVALID_RELATIONSHIP_TYPE;

	//Affects with arg1 SYNONYM , arg2 SYNONYM
	rel = "Affects";
	arg1.value = "a1";
	arg2.value = "a2";
	CPPUNIT_ASSERT(qp.ValidateRelationship(rel , rel_enum , arg1 , arg2) == true);
	CPPUNIT_ASSERT(rel_enum == AFFECTS);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.value == "a1");
	CPPUNIT_ASSERT(arg1.syn.type == ASSIGN);
	CPPUNIT_ASSERT(arg1.syn.value == "a1");
	CPPUNIT_ASSERT(arg2.type == SYNONYM);
	CPPUNIT_ASSERT(arg2.value == "a2");
	CPPUNIT_ASSERT(arg2.syn.type == ASSIGN);
	CPPUNIT_ASSERT(arg2.syn.value == "a2");

	//Affects with arg1 INTEGER , arg2 SYNONYM
	rel = "Affects";
	arg1.value = "100";
	arg2.value = "a";
	CPPUNIT_ASSERT(qp.ValidateRelationship(rel , rel_enum , arg1 , arg2) == true);
	CPPUNIT_ASSERT(rel_enum == AFFECTS);
	CPPUNIT_ASSERT(arg1.type == INTEGER);
	CPPUNIT_ASSERT(arg1.value == "100");
	CPPUNIT_ASSERT(arg2.type == SYNONYM);
	CPPUNIT_ASSERT(arg2.value == "a");
	CPPUNIT_ASSERT(arg2.syn.type == ASSIGN);
	CPPUNIT_ASSERT(arg2.syn.value == "a");

	//Affects with arg1 SYNONYM , arg2 INTEGER
	rel = "Affects";
	arg1.value = "a";
	arg2.value = "5";
	CPPUNIT_ASSERT(qp.ValidateRelationship(rel , rel_enum , arg1 , arg2) == true);
	CPPUNIT_ASSERT(rel_enum == AFFECTS);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.value == "a");
	CPPUNIT_ASSERT(arg1.syn.type == ASSIGN);
	CPPUNIT_ASSERT(arg1.syn.value == "a");
	CPPUNIT_ASSERT(arg2.type == INTEGER);
	CPPUNIT_ASSERT(arg2.value == "5");

	//Affects with arg1 INTEGER , arg2 INTEGER
	rel = "Affects";
	arg1.value = "4";
	arg2.value = "5";
	CPPUNIT_ASSERT(qp.ValidateRelationship(rel , rel_enum , arg1 , arg2) == true);
	CPPUNIT_ASSERT(rel_enum == AFFECTS);
	CPPUNIT_ASSERT(arg1.type == INTEGER);
	CPPUNIT_ASSERT(arg1.value == "4");
	CPPUNIT_ASSERT(arg2.type == INTEGER);
	CPPUNIT_ASSERT(arg2.value == "5");

	//Affects*

	//Next

	//Next*

	//...



	//*******************************************************************


	//*****************************INVALID*********************************



	//*********************************************************************

}

void QueryPreProcessorTest::ValidatePatternTest()
{
	QueryPreProcessor qp;
	QueryData qd;
	qd.ClearData();
	qd.InsertDeclaration(Synonym("a",ASSIGN));
	qd.InsertDeclaration(Synonym("w",WHILE));
	qd.InsertDeclaration(Synonym("s",STMT));
	qd.InsertDeclaration(Synonym("ifstat",IF));
	qd.InsertDeclaration(Synonym("v",VARIABLE));
	qd.InsertDeclaration(Synonym("p",PROCEDURE));
	qd.InsertDeclaration(Synonym("c",CONSTANT));
	qd.InsertDeclaration(Synonym("n",PROG_LINE));

	//valid
	Synonym syn;
	Argument arg1, arg2, arg3;

	syn.value = "ifstat";
	syn.type = IF;
	arg1.value = "\" x \"";
	arg2.value = "_";
	arg3.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 , arg3) == true);
	CPPUNIT_ASSERT(arg1.type == IDENT);
	CPPUNIT_ASSERT(arg1.value == "\"x\"");
	CPPUNIT_ASSERT(arg2.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg2.value == "_");
	CPPUNIT_ASSERT(arg3.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg3.value == "_");

	syn.value = "ifstat";
	syn.type = IF;
	arg1.value = "v";
	arg2.value = "_";
	arg3.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 , arg3) == true);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.value == "v");
	CPPUNIT_ASSERT(arg1.syn.type == VARIABLE);
	CPPUNIT_ASSERT(arg1.syn.value == "v");
	CPPUNIT_ASSERT(arg2.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg2.value == "_");
	CPPUNIT_ASSERT(arg3.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg3.value == "_");

	syn.value = "w";
	syn.type = WHILE;
	arg1.value = "\"x\"";
	arg2.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 ) == true);
	CPPUNIT_ASSERT(arg1.type == IDENT);
	CPPUNIT_ASSERT(arg1.value == "\"x\"");
	CPPUNIT_ASSERT(arg2.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg2.value == "_");

	syn.value = "w";
	syn.type = WHILE;
	arg1.value = "v";
	arg2.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 ) == true);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.value == "v");
	CPPUNIT_ASSERT(arg1.syn.type == VARIABLE);
	CPPUNIT_ASSERT(arg1.syn.value == "v");
	CPPUNIT_ASSERT(arg2.type == UNDERSCORE);
	CPPUNIT_ASSERT(arg2.value == "_");
	
	//invalid
	//synonym is not declared
	syn.value = "ifstat1";
	syn.type = IF;
	arg1.value = "\"x\"";
	arg2.value = "_";
	arg3.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 , arg3) == false);

	//synonym type is not IF
	syn.value = "ifstat";
	syn.type = ASSIGN;
	arg1.value = "\"x\"";
	arg2.value = "_";
	arg3.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 , arg3) == false);
	
	syn.value = "a";
	syn.type = ASSIGN;
	arg1.value = "\"x\"";
	arg2.value = "_";
	arg3.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 , arg3) == false);
	
	//first argument is not IDENT
	syn.value = "ifstat";
	syn.type = IF;
	arg1.value = "a";
	arg2.value = "_";
	arg3.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 , arg3) == false);
	
	//first argument is not IDENT
	syn.value = "ifstat";
	syn.type = IF;
	arg1.value = 5;
	arg2.value = "_";
	arg3.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 , arg3) == false);

	//second argument is not _
	syn.value = "ifstat";
	syn.type = IF;
	arg1.value = "\"abc\"";
	arg2.value = "a";
	arg3.value = "_";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 , arg3) == false);

	//third argument is not _
	syn.value = "ifstat";
	syn.type = IF;
	arg1.value = "\"abc\"";
	arg2.value = "_";
	arg3.value = "a";
	CPPUNIT_ASSERT(qp.ValidatePattern(syn , arg1 , arg2 , arg3) == false);

	//check second and third arg



	//


}


void QueryPreProcessorTest::ValidateWithTest()
{
	QueryPreProcessor qp;
	QueryData qd;
	qd.ClearData();
	qd.InsertDeclaration(Synonym("a",ASSIGN));
	qd.InsertDeclaration(Synonym("w",WHILE));
	qd.InsertDeclaration(Synonym("s",STMT));
	qd.InsertDeclaration(Synonym("ifstat",IF));
	qd.InsertDeclaration(Synonym("v",VARIABLE));
	qd.InsertDeclaration(Synonym("p",PROCEDURE));
	qd.InsertDeclaration(Synonym("c",CONSTANT));
	qd.InsertDeclaration(Synonym("n",PROG_LINE));

	//valid
	std::string lhs = "a.stmt#";
	std::string rhs = "s.stmt#";
	Argument arg1, arg2;
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == true);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.value == "a");
	CPPUNIT_ASSERT(arg1.syn.type == ASSIGN);
	CPPUNIT_ASSERT(arg1.syn.value == "a");

	CPPUNIT_ASSERT(arg2.type == SYNONYM);
	CPPUNIT_ASSERT(arg2.value == "s");
	CPPUNIT_ASSERT(arg2.syn.type == STMT);
	CPPUNIT_ASSERT(arg2.syn.value == "s");

	lhs = "s.stmt#";
	rhs = "5";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == true);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.value == "s");
	CPPUNIT_ASSERT(arg1.syn.type == STMT);
	CPPUNIT_ASSERT(arg1.syn.value == "s");

	CPPUNIT_ASSERT(arg2.type == INTEGER);
	CPPUNIT_ASSERT(arg2.value == "5");

	lhs = "ifstat.stmt#";
	rhs = "c.value";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == true);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.value == "ifstat");
	CPPUNIT_ASSERT(arg1.syn.type == IF);
	CPPUNIT_ASSERT(arg1.syn.value == "ifstat");

	CPPUNIT_ASSERT(arg2.type == SYNONYM);
	CPPUNIT_ASSERT(arg2.value == "c");
	CPPUNIT_ASSERT(arg2.syn.type == CONSTANT);
	CPPUNIT_ASSERT(arg2.syn.value == "c");

	lhs = "n";
	rhs = "10";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == true);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.value == "n");
	CPPUNIT_ASSERT(arg1.syn.type == PROG_LINE);
	CPPUNIT_ASSERT(arg1.syn.value == "n");

	CPPUNIT_ASSERT(arg2.type == INTEGER);
	CPPUNIT_ASSERT(arg2.value == "10");

	lhs = "n";
	rhs = "a.stmt#";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == true);
	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg1.value == "n");
	CPPUNIT_ASSERT(arg1.syn.type == PROG_LINE);
	CPPUNIT_ASSERT(arg1.syn.value == "n");

	CPPUNIT_ASSERT(arg1.type == SYNONYM);
	CPPUNIT_ASSERT(arg2.value == "a");
	CPPUNIT_ASSERT(arg2.syn.type == ASSIGN);
	CPPUNIT_ASSERT(arg2.syn.value == "a");

	//invalid
	lhs = "a.stmt#";
	rhs = "p.procName";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);

	lhs = "w.stmt#";
	rhs = "v.varName";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);

	lhs = "p.procName";
	rhs = "c.value";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);

	lhs = "n";
	rhs = "p.procName";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);

	lhs = "n";
	rhs = "p.procName";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);

	lhs = "p.procName";
	rhs = "100";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);

	lhs = "v.varName";
	rhs = "5";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);
	
	lhs = "ifstat.stmt#";
	rhs = "\"x\"";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);

	lhs = "n";
	rhs = "\"z\"";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);

	lhs = "n.stmt#";
	rhs = "5";
	CPPUNIT_ASSERT(qp.ValidateWith(arg1 , arg2 , lhs , rhs) == false);
}

void QueryPreProcessorTest::TokenizeExpressionTest()
{
	QueryPreProcessor qp;

	std::vector<std::string> tokens , correctResult;
	std::string delim = "+-*()";
	std::string exp = "x";
	correctResult.push_back("x");

	qp.TokenizeExpression(exp , tokens , delim);

	for(int i=0; i<tokens.size(); ++i)
	{
		std::cout << tokens[i]  << "\n";
	}

	//qp.TokenizeExpression(exp , tokens , delim);
	CPPUNIT_ASSERT(tokens.size() == 1);
	for(int i=0; i<tokens.size(); ++i)
	{
		//std::cout << tokens[i] << " = " << correctResult[i] << "\n";
		CPPUNIT_ASSERT(tokens[i] == correctResult[i]);
	}


	tokens.clear(); 
	correctResult.clear();
	exp = "x+y";
	correctResult.push_back("x");
	correctResult.push_back("+");
	correctResult.push_back("y");

	qp.TokenizeExpression(exp , tokens , delim);

	/*for(int i=0; i<tokens.size(); ++i)
	{
		std::cout << tokens[i]  << "\n";
	}*/

	//qp.TokenizeExpression(exp , tokens , delim);
	CPPUNIT_ASSERT(tokens.size() == correctResult.size());
	for(int i=0; i<tokens.size(); ++i)
	{
		//std::cout << tokens[i] << " = " << correctResult[i] << "\n";
		CPPUNIT_ASSERT(tokens[i] == correctResult[i]);
	}


	tokens.clear(); 
	correctResult.clear();
	exp = "x+y*z";
	correctResult.push_back("x");
	correctResult.push_back("+");
	correctResult.push_back("y");
	correctResult.push_back("*");
	correctResult.push_back("z");

	qp.TokenizeExpression(exp , tokens , delim);

	/*for(int i=0; i<tokens.size(); ++i)
	{
		std::cout << tokens[i]  << "\n";
	}*/

	//qp.TokenizeExpression(exp , tokens , delim);
	CPPUNIT_ASSERT(tokens.size() == correctResult.size());
	for(int i=0; i<tokens.size(); ++i)
	{
		//std::cout << tokens[i] << " = " << correctResult[i] << "\n";
		CPPUNIT_ASSERT(tokens[i] == correctResult[i]);
	}


	tokens.clear(); 
	correctResult.clear();
	exp = "(x+y)*z";
	correctResult.push_back("(");
	correctResult.push_back("x");
	correctResult.push_back("+");
	correctResult.push_back("y");
	correctResult.push_back(")");
	correctResult.push_back("*");
	correctResult.push_back("z");

	qp.TokenizeExpression(exp , tokens , delim);

	/*for(int i=0; i<tokens.size(); ++i)
	{
		std::cout << tokens[i]  << "\n";
	}*/

	//qp.TokenizeExpression(exp , tokens , delim);
	CPPUNIT_ASSERT(tokens.size() == correctResult.size());
	for(int i=0; i<tokens.size(); ++i)
	{
		//std::cout << tokens[i] << " = " << correctResult[i] << "\n";
		CPPUNIT_ASSERT(tokens[i] == correctResult[i]);
	}



	tokens.clear(); 
	correctResult.clear();
	exp = "x*(y-z)";
	
	correctResult.push_back("x");
	correctResult.push_back("*");
	correctResult.push_back("(");
	correctResult.push_back("y");
	correctResult.push_back("-");
	correctResult.push_back("z");
	correctResult.push_back(")");

	qp.TokenizeExpression(exp , tokens , delim);

	/*for(int i=0; i<tokens.size(); ++i)
	{
		std::cout << tokens[i]  << "\n";
	}*/

	//qp.TokenizeExpression(exp , tokens , delim);
	CPPUNIT_ASSERT(tokens.size() == correctResult.size());
	for(int i=0; i<tokens.size(); ++i)
	{
		//std::cout << tokens[i] << " = " << correctResult[i] << "\n";
		CPPUNIT_ASSERT(tokens[i] == correctResult[i]);
	}

}

void QueryPreProcessorTest::IsValidExpressionTest()
{
	QueryPreProcessor qp;
	std::vector<std::string> exp;
	exp.push_back("x");

	CPPUNIT_ASSERT(qp.IsValidExpression(exp) == true);

	exp.clear();
	exp.push_back("x");
	exp.push_back("+");
	exp.push_back("y");

	CPPUNIT_ASSERT(qp.IsValidExpression(exp) == true);


	exp.clear();
	exp.push_back("x");
	exp.push_back("+");
	exp.push_back("y");
	exp.push_back("*");
	exp.push_back("z");

	CPPUNIT_ASSERT(qp.IsValidExpression(exp) == true);


	//(x+y)*z
	exp.clear();
	exp.push_back("(");
	exp.push_back("x");
	exp.push_back("+");
	exp.push_back("y");
	exp.push_back(")");
	exp.push_back("*");
	exp.push_back("z");

	CPPUNIT_ASSERT(qp.IsValidExpression(exp , 0 , 4) == true);

	exp.clear();
	exp.push_back("x");
	exp.push_back("+");
	exp.push_back("(");
	exp.push_back("y");
	exp.push_back("*");
	exp.push_back("z");
	exp.push_back(")");

	CPPUNIT_ASSERT(qp.IsValidExpression(exp , 2 , 6) == true);


	exp.clear();
	exp.push_back("x");
	exp.push_back("+");
	exp.push_back("(");
	exp.push_back("y");
	exp.push_back("*");
	exp.push_back("z");
	exp.push_back(")");
	exp.push_back("+");
	exp.push_back("a");

	CPPUNIT_ASSERT(qp.IsValidExpression(exp , 2 , 6) == true);


	exp.clear();
	exp.push_back("x");
	exp.push_back("+");
	exp.push_back("(");
	exp.push_back("x");
	exp.push_back("+");
	exp.push_back("(");
	exp.push_back("y");
	exp.push_back("*");
	exp.push_back("z");
	exp.push_back(")");
	exp.push_back(")");
	exp.push_back("+");
	exp.push_back("a");

	CPPUNIT_ASSERT(qp.IsValidExpression(exp , 2 , 10) == true);

}

void QueryPreProcessorTest::IsExpressionTest()
{
	QueryPreProcessor qp;
	std::string exp;

	exp = "\"(x+y)*z\"";
	CPPUNIT_ASSERT(qp.IsExpression(exp) == true);


	exp = "\"(a-b+(x+y))*z\"";
	CPPUNIT_ASSERT(qp.IsExpression(exp) == true);

	exp = "\"x\"";
	CPPUNIT_ASSERT(qp.IsExpression(exp) == true);
}
