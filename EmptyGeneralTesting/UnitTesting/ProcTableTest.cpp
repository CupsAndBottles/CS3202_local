#include <cppunit\config\SourcePrefix.h>
#include "..\SPA\PKB\ProcTable.h"
#include "..\SPA\Exception\IndexNotFoundException.h"

#include "ProcTableTest.h"

#include <iostream>
#include <string>

void ProcTableTest::setUp() {
	ProcTable::ClearData();
}

void ProcTableTest::tearDown() {
	ProcTable::ClearData();
}

CPPUNIT_TEST_SUITE_REGISTRATION(ProcTableTest);

void ProcTableTest::TestInsertProc() {
	CPPUNIT_ASSERT_EQUAL(0, ProcTable::GetNoOfProcs());

	// regular insertions
	CPPUNIT_ASSERT_EQUAL(0, ProcTable::InsertProc("First"));
	CPPUNIT_ASSERT_EQUAL(1, ProcTable::GetNoOfProcs());

	CPPUNIT_ASSERT_EQUAL(1, ProcTable::InsertProc("Second"));
	CPPUNIT_ASSERT_EQUAL(2, ProcTable::GetNoOfProcs());
	
	CPPUNIT_ASSERT_EQUAL(2, ProcTable::InsertProc("Third1223"));
	CPPUNIT_ASSERT_EQUAL(3, ProcTable::GetNoOfProcs());
	
	// repeated insertions
	CPPUNIT_ASSERT_EQUAL(0, ProcTable::InsertProc("First"));
	CPPUNIT_ASSERT_EQUAL(2, ProcTable::InsertProc("Third1223"));
	CPPUNIT_ASSERT_EQUAL(3, ProcTable::GetNoOfProcs());

	// do we consider invalid names? (white spaces, non-alphanumeric...)
}

void ProcTableTest::TestGetIndexOfProc() {
	CPPUNIT_ASSERT_EQUAL(0, ProcTable::GetNoOfProcs());

	ProcTable::InsertProc("First");
	ProcTable::InsertProc("Second");
	ProcTable::InsertProc("Third");

	// regular get index of
	CPPUNIT_ASSERT_EQUAL(0, ProcTable::GetIndexOfProc("First"));
	CPPUNIT_ASSERT_EQUAL(2, ProcTable::GetIndexOfProc("Third"));

	// test for variables that do not exist
	CPPUNIT_ASSERT_EQUAL(-1, ProcTable::GetIndexOfProc("Fourth"));

}

void ProcTableTest::TestGetProcName() {
	CPPUNIT_ASSERT_EQUAL(0, ProcTable::GetNoOfProcs());

	ProcTable::InsertProc("Bo00YeaH");
	ProcTable::InsertProc("Yaemiah");

	CPPUNIT_ASSERT(ProcTable::GetProcName(1) == "Yaemiah");
	CPPUNIT_ASSERT(ProcTable::GetProcName(1) != " Yaemiah");
	CPPUNIT_ASSERT(ProcTable::GetProcName(1) != "Yaemiah ");
	
	CPPUNIT_ASSERT(ProcTable::GetProcName(0) == "Bo00YeaH");

	CPPUNIT_ASSERT_THROW(ProcTable::GetProcName(2), IndexNotFoundException);
}

void ProcTableTest::TestGetAllProcNames() {
	CPPUNIT_ASSERT_EQUAL(0, ProcTable::GetNoOfProcs());

	// test empty list
	CPPUNIT_ASSERT_EQUAL(0, (int) ProcTable::GetAllProcNames().size());

	ProcTable::InsertProc("This");
	ProcTable::InsertProc("is");
	ProcTable::InsertProc("a");
	ProcTable::InsertProc("list");
	ProcTable::InsertProc("of");
	ProcTable::InsertProc("proc");
	ProcTable::InsertProc("names");

	// test regular proc names
	vector<string> allProcNames = ProcTable::GetAllProcNames();
	CPPUNIT_ASSERT_EQUAL(7, (int) allProcNames.size());
	CPPUNIT_ASSERT("This" == allProcNames.at(0));
	CPPUNIT_ASSERT("is" == allProcNames.at(1));
	CPPUNIT_ASSERT("a" == allProcNames.at(2));
	CPPUNIT_ASSERT("list" == allProcNames.at(3));
	CPPUNIT_ASSERT("of" == allProcNames.at(4));
	CPPUNIT_ASSERT("proc" == allProcNames.at(5));
	CPPUNIT_ASSERT("names" == allProcNames.at(6));

}

void ProcTableTest::TestFirstLastStmtNos() {
	CPPUNIT_ASSERT_EQUAL(0, ProcTable::GetNoOfProcs());

	// regular insert and set
	int procIndexOfFirst = ProcTable::InsertProc("First");
	ProcTable::SetFirstStmtNoOfProc(procIndexOfFirst, 1);
	ProcTable::SetLastStmtNoOfProc(procIndexOfFirst, 30);

	int procIndexOfSecond = ProcTable::InsertProc("Second");
	ProcTable::SetFirstStmtNoOfProc(procIndexOfSecond, 31);
	ProcTable::SetLastStmtNoOfProc(procIndexOfSecond, 40);

	CPPUNIT_ASSERT_EQUAL(1, ProcTable::GetFirstStmtNoOfProc(procIndexOfFirst));
	CPPUNIT_ASSERT_EQUAL(30, ProcTable::GetLastStmtNoOfProc(procIndexOfFirst));
	CPPUNIT_ASSERT_EQUAL(31, ProcTable::GetFirstStmtNoOfProc(procIndexOfSecond));
	CPPUNIT_ASSERT_EQUAL(40, ProcTable::GetLastStmtNoOfProc(procIndexOfSecond));

	// non existent procIndex used
	CPPUNIT_ASSERT_THROW(ProcTable::SetFirstStmtNoOfProc(2, 1), IndexNotFoundException);
	CPPUNIT_ASSERT_THROW(ProcTable::SetLastStmtNoOfProc(2, 30), IndexNotFoundException);
	CPPUNIT_ASSERT_THROW(ProcTable::GetFirstStmtNoOfProc(2), IndexNotFoundException);
	CPPUNIT_ASSERT_THROW(ProcTable::GetLastStmtNoOfProc(2), IndexNotFoundException);

	// wrong index settings, ignore and remove
	ProcTable::InsertProc("Third");	// valid proc insertion
	ProcTable::SetFirstStmtNoOfProc(2, 30);
	CPPUNIT_ASSERT_EQUAL(-1, ProcTable::GetFirstStmtNoOfProc(2)); // not set
	ProcTable::SetFirstStmtNoOfProc(2, 40);
	CPPUNIT_ASSERT_EQUAL(-1, ProcTable::GetFirstStmtNoOfProc(2)); // not set
	ProcTable::SetLastStmtNoOfProc(2, 40);
	CPPUNIT_ASSERT_EQUAL(-1, ProcTable::GetLastStmtNoOfProc(2));

	// repeated insertions, should ignore the 2nd one onwards
	ProcTable::SetFirstStmtNoOfProc(procIndexOfSecond, 32);
	ProcTable::SetLastStmtNoOfProc(procIndexOfSecond, 41);
	CPPUNIT_ASSERT_EQUAL(31, ProcTable::GetFirstStmtNoOfProc(procIndexOfSecond));
	CPPUNIT_ASSERT_EQUAL(40, ProcTable::GetLastStmtNoOfProc(procIndexOfSecond));
}

void ProcTableTest::TestGetProcOfStmt() {
	CPPUNIT_ASSERT_EQUAL(0, ProcTable::GetNoOfProcs());

	// no conflicts
	ProcTable::InsertProc("First");
	ProcTable::SetFirstStmtNoOfProc(0, 1);
	ProcTable::SetLastStmtNoOfProc(0, 30);
	ProcTable::InsertProc("Second");
	ProcTable::SetFirstStmtNoOfProc(1, 31);
	ProcTable::SetLastStmtNoOfProc(1, 40);
	ProcTable::InsertProc("Third");
	ProcTable::SetFirstStmtNoOfProc(2, 41);
	ProcTable::SetLastStmtNoOfProc(2, 45);

	CPPUNIT_ASSERT_EQUAL(0, ProcTable::GetProcOfStmt(30));
	CPPUNIT_ASSERT_EQUAL(1, ProcTable::GetProcOfStmt(31));
	CPPUNIT_ASSERT_EQUAL(1, ProcTable::GetProcOfStmt(35));
	CPPUNIT_ASSERT_EQUAL(1, ProcTable::GetProcOfStmt(40));
	CPPUNIT_ASSERT_EQUAL(2, ProcTable::GetProcOfStmt(41));

	int procIndexOfFourth = ProcTable::InsertProc("Fourth");
	ProcTable::SetFirstStmtNoOfProc(procIndexOfFourth, 45);	// conflicts with Third
	ProcTable::SetLastStmtNoOfProc(procIndexOfFourth, 48);

	CPPUNIT_ASSERT_EQUAL(2, ProcTable::GetProcOfStmt(45));
	CPPUNIT_ASSERT_EQUAL(-1, ProcTable::GetProcOfStmt(46));

}