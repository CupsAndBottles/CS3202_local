#include "RelTableTest.h"
#include <cppunit/config/SourcePrefix.h>
#include "..\SPA\QueryProcessor/RelTable.h"

using namespace std;

void RelTableTest::setUp() {};
void RelTableTest::tearDown() {};

CPPUNIT_TEST_SUITE_REGISTRATION(RelTableTest);

RelTableTest::RelTableTest() {}

void RelTableTest::TestGetRelTableData()
{
	int size = RelTable::GetTableSize();
	CPPUNIT_ASSERT(size == 3);
	
	/*std::cout << "table size : " << RelTable::GetTableSize() << "\n";
	std::cout << "modifies arg1 size : " << RelTable::GetRelArgType(MODIFIES, ARG1).size() << "\n";
	std::vector<ArgumentType> argType = RelTable::GetRelArgType(MODIFIES, ARG1);
	for(std::vector<ArgumentType>::iterator it = argType.begin(); it != argType.end(); ++it)
		std::cout << *it << " ";
	std::cout << "\n";

	std::cout << "modifies arg2 size : " << RelTable::GetRelArgType(MODIFIES, ARG2).size() << "\n";
	argType = RelTable::GetRelArgType(MODIFIES, ARG2);
	for(std::vector<ArgumentType>::iterator it = argType.begin(); it != argType.end(); ++it)
		std::cout << *it << " ";
	std::cout << "\n";

	std::cout << "modifies arg1 synonym size : " << RelTable::GetRelArgSynonymType(MODIFIES, ARG1).size() << "\n";
	std::vector<SynonymType> argSynType = RelTable::GetRelArgSynonymType(MODIFIES, ARG1);
	for(std::vector<SynonymType>::iterator it = argSynType.begin(); it != argSynType.end(); ++it)
		std::cout << *it << " ";
	std::cout << "\n";

	std::cout << "modifies arg2 synonym size : " << RelTable::GetRelArgSynonymType(MODIFIES, ARG2).size() << "\n";
	argSynType = RelTable::GetRelArgSynonymType(MODIFIES, ARG2);
	for(std::vector<SynonymType>::iterator it = argSynType.begin(); it != argSynType.end(); ++it)
		std::cout << *it << " ";
	std::cout << "\n";*/
	
	//MODIFIES table
	std::vector<ArgumentType> argType = RelTable::GetRelArgType(MODIFIES, ARG1);
	CPPUNIT_ASSERT(argType.size() == 2);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == INTEGER);

	std::vector<SynonymType> argSynType = RelTable::GetRelArgSynonymType(MODIFIES, ARG1);
	CPPUNIT_ASSERT(argSynType.size() == 4);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == ASSIGN);
	CPPUNIT_ASSERT(argSynType.at(2) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(3) == PROG_LINE);

	argType = RelTable::GetRelArgType(MODIFIES, ARG2);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == IDENT);

	argSynType = RelTable::GetRelArgSynonymType(MODIFIES, ARG2);
	CPPUNIT_ASSERT(argSynType.size() == 1);
	CPPUNIT_ASSERT(argSynType.at(0) == VARIABLE);
	
	//USES table
	argType = RelTable::GetRelArgType(USES, ARG1);
	CPPUNIT_ASSERT(argType.size() == 2);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == INTEGER);

	argSynType = RelTable::GetRelArgSynonymType(USES, ARG1);
	CPPUNIT_ASSERT(argSynType.size() == 4);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == ASSIGN);
	CPPUNIT_ASSERT(argSynType.at(2) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(3) == PROG_LINE);

	argType = RelTable::GetRelArgType(USES, ARG2);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == IDENT);

	argSynType = RelTable::GetRelArgSynonymType(USES, ARG2);
	CPPUNIT_ASSERT(argSynType.size() == 1);
	CPPUNIT_ASSERT(argSynType.at(0) == VARIABLE);
	
	//PARENT table
	argType = RelTable::GetRelArgType(PARENT, ARG1);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == INTEGER);

	argSynType = RelTable::GetRelArgSynonymType(PARENT, ARG1);
	CPPUNIT_ASSERT(argSynType.size() == 3);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(2) == PROG_LINE);

	argType = RelTable::GetRelArgType(PARENT, ARG2);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == INTEGER);

	argSynType = RelTable::GetRelArgSynonymType(PARENT, ARG2);
	CPPUNIT_ASSERT(argSynType.size() == 4);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == ASSIGN);
	CPPUNIT_ASSERT(argSynType.at(2) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(3) == PROG_LINE);

	//PARENT* table
	argType = RelTable::GetRelArgType(PARENTT, ARG1);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == INTEGER);

	argSynType = RelTable::GetRelArgSynonymType(PARENTT, ARG1);
	CPPUNIT_ASSERT(argSynType.size() == 3);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(2) == PROG_LINE);

	argType = RelTable::GetRelArgType(PARENTT, ARG2);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == INTEGER);

	argSynType = RelTable::GetRelArgSynonymType(PARENTT, ARG2);
	CPPUNIT_ASSERT(argSynType.size() == 4);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == ASSIGN);
	CPPUNIT_ASSERT(argSynType.at(2) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(3) == PROG_LINE);

	//FOLLOWS table
	argType = RelTable::GetRelArgType(FOLLOWS, ARG1);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == INTEGER);

	argSynType = RelTable::GetRelArgSynonymType(FOLLOWS, ARG1);
	CPPUNIT_ASSERT(argSynType.size() == 4);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == ASSIGN);
	CPPUNIT_ASSERT(argSynType.at(2) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(3) == PROG_LINE);

	argType = RelTable::GetRelArgType(FOLLOWS, ARG2);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == INTEGER);

	argSynType = RelTable::GetRelArgSynonymType(FOLLOWS, ARG2);
	CPPUNIT_ASSERT(argSynType.size() == 4);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == ASSIGN);
	CPPUNIT_ASSERT(argSynType.at(2) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(3) == PROG_LINE);

	//FOLLOWS* table
	argType = RelTable::GetRelArgType(FOLLOWST, ARG1);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == INTEGER);

	argSynType = RelTable::GetRelArgSynonymType(FOLLOWST, ARG1);
	CPPUNIT_ASSERT(argSynType.size() == 4);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == ASSIGN);
	CPPUNIT_ASSERT(argSynType.at(2) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(3) == PROG_LINE);

	argType = RelTable::GetRelArgType(FOLLOWST, ARG2);
	CPPUNIT_ASSERT(argType.size() == 3);
	CPPUNIT_ASSERT(argType.at(0) == SYNONYM);
	CPPUNIT_ASSERT(argType.at(1) == UNDERSCORE);
	CPPUNIT_ASSERT(argType.at(2) == INTEGER);

	argSynType = RelTable::GetRelArgSynonymType(FOLLOWST, ARG2);
	CPPUNIT_ASSERT(argSynType.size() == 4);
	CPPUNIT_ASSERT(argSynType.at(0) == STMT);
	CPPUNIT_ASSERT(argSynType.at(1) == ASSIGN);
	CPPUNIT_ASSERT(argSynType.at(2) == WHILE);
	CPPUNIT_ASSERT(argSynType.at(3) == PROG_LINE);

	//ERROR
	try {
		argType = RelTable::GetRelArgType(INVALID_RELATIONSHIP_TYPE, ARG1);
	} catch (const std::invalid_argument &e) {
		std::string s = e.what();
		CPPUNIT_ASSERT(s == "Invalid argument : RelationshipType");
	}
}