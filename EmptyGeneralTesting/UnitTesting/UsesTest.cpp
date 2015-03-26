#include "UsesTest.h"
#include "..\SPA\PKB\Uses.h"
#include "..\SPA\PKB\Parent.h"

void UsesTest::setUp() { 
	Uses::ClearData();
	Parent::ClearData();
}

void UsesTest::tearDown() {
	Uses::ClearData();
}

CPPUNIT_TEST_SUITE_REGISTRATION(UsesTest);

UsesTest::UsesTest() {
}

void UsesTest::TestSingleStmt() {
	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);

	Uses::SetStmtUsesVar(1, 0);

	CPPUNIT_ASSERT(Uses::HasAnyUses() == true);
	CPPUNIT_ASSERT(Uses::SizeOfUses() == 1);
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 0));

	vector<int> stmtsUsingVar0 = Uses::GetStmtUsingVar(0);
	CPPUNIT_ASSERT(stmtsUsingVar0.size() == 1);
	CPPUNIT_ASSERT(stmtsUsingVar0[0] == 1);

	vector<int> varsUsedByStmt1 = Uses::GetVarUsedByStmt(1);
	CPPUNIT_ASSERT(varsUsedByStmt1.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt1[0] == 0);

	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);
	
}

void UsesTest::TestMultipleStmts() {
	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);

	Uses::SetStmtUsesVar(1, 0);
	Uses::SetStmtUsesVar(2, 1);
	Uses::SetStmtUsesVar(3, 2);
	Uses::SetStmtUsesVar(4, 3);

	CPPUNIT_ASSERT(Uses::HasAnyUses() == true);
	CPPUNIT_ASSERT(Uses::SizeOfUses() == 4);
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 0));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(2, 1));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(3, 2));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(4, 3));

	vector<int> stmtsUsingVar0 = Uses::GetStmtUsingVar(0);

	CPPUNIT_ASSERT_EQUAL(1, (int)stmtsUsingVar0.size());
	CPPUNIT_ASSERT_EQUAL(1, stmtsUsingVar0[0]);

	vector<int> stmtsUsingVar1 = Uses::GetStmtUsingVar(1);
	CPPUNIT_ASSERT_EQUAL(1, (int)stmtsUsingVar1.size());
	CPPUNIT_ASSERT(stmtsUsingVar1[0] == 2);

	vector<int> stmtsUsingVar2 = Uses::GetStmtUsingVar(2);
	CPPUNIT_ASSERT(stmtsUsingVar2.size() == 1);
	CPPUNIT_ASSERT(stmtsUsingVar2[0] == 3);

	vector<int> stmtsUsingVar3 = Uses::GetStmtUsingVar(3);
	CPPUNIT_ASSERT(stmtsUsingVar3.size() == 1);
	CPPUNIT_ASSERT(stmtsUsingVar3[0] == 4);

	vector<int> varsUsedByStmt1 = Uses::GetVarUsedByStmt(1);
	CPPUNIT_ASSERT(varsUsedByStmt1.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt1[0] == 0);

	vector<int> varsUsedByStmt2 = Uses::GetVarUsedByStmt(2);
	CPPUNIT_ASSERT(varsUsedByStmt2.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt2[0] == 1);

	vector<int> varsUsedByStmt3 = Uses::GetVarUsedByStmt(3);
	CPPUNIT_ASSERT(varsUsedByStmt3.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt3[0] == 2);

	vector<int> varsUsedByStmt4 = Uses::GetVarUsedByStmt(4);
	CPPUNIT_ASSERT(varsUsedByStmt4.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt4[0] == 3);

	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);

}

void UsesTest::TestMultipleStmtsUsingSingleVar() {
	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);

	Uses::SetStmtUsesVar(1, 0);
	Uses::SetStmtUsesVar(2, 0);
	Uses::SetStmtUsesVar(3, 0);
	Uses::SetStmtUsesVar(4, 0);

	CPPUNIT_ASSERT(Uses::HasAnyUses() == true);
	CPPUNIT_ASSERT(Uses::SizeOfUses() == 4);
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 0));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(2, 0));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(3, 0));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(4, 0));

	vector<int> stmtsUsingVar0 = Uses::GetStmtUsingVar(0);
	CPPUNIT_ASSERT(stmtsUsingVar0.size() == 4);
	CPPUNIT_ASSERT(stmtsUsingVar0[0] == 1);
	CPPUNIT_ASSERT(stmtsUsingVar0[1] == 2);
	CPPUNIT_ASSERT(stmtsUsingVar0[2] == 3);
	CPPUNIT_ASSERT(stmtsUsingVar0[3] == 4);

	vector<int> varsUsedByStmt1 = Uses::GetVarUsedByStmt(1);
	CPPUNIT_ASSERT(varsUsedByStmt1.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt1[0] == 0);

	vector<int> varsUsedByStmt2 = Uses::GetVarUsedByStmt(2);
	CPPUNIT_ASSERT(varsUsedByStmt2.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt2[0] == 0);

	vector<int> varsUsedByStmt3 = Uses::GetVarUsedByStmt(3);
	CPPUNIT_ASSERT(varsUsedByStmt3.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt3[0] == 0);

	vector<int> varsUsedByStmt4 = Uses::GetVarUsedByStmt(4);
	CPPUNIT_ASSERT(varsUsedByStmt4.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt4[0] == 0);

	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);
}

void UsesTest::TestSingleStmtUsingMultipleVars() {
	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);

	Uses::SetStmtUsesVar(1, 0);
	Uses::SetStmtUsesVar(1, 1);
	Uses::SetStmtUsesVar(1, 2);
	Uses::SetStmtUsesVar(1, 3);

	CPPUNIT_ASSERT(Uses::HasAnyUses() == true);
	CPPUNIT_ASSERT(Uses::SizeOfUses() == 4);
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 0));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 1));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 2));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 3));

	vector<int> stmtsUsingVar0 = Uses::GetStmtUsingVar(0);
	CPPUNIT_ASSERT(stmtsUsingVar0.size() == 1);
	CPPUNIT_ASSERT(stmtsUsingVar0[0] == 1);

	vector<int> stmtsUsingVar1 = Uses::GetStmtUsingVar(1);
	CPPUNIT_ASSERT(stmtsUsingVar1.size() == 1);
	CPPUNIT_ASSERT(stmtsUsingVar1[0] == 1);

	vector<int> stmtsUsingVar2 = Uses::GetStmtUsingVar(2);
	CPPUNIT_ASSERT(stmtsUsingVar2.size() == 1);
	CPPUNIT_ASSERT(stmtsUsingVar2[0] == 1);

	vector<int> stmtsUsingVar3 = Uses::GetStmtUsingVar(3);
	CPPUNIT_ASSERT(stmtsUsingVar3.size() == 1);
	CPPUNIT_ASSERT(stmtsUsingVar3[0] == 1);

	vector<int> varsUsedByStmt1 = Uses::GetVarUsedByStmt(1);
	CPPUNIT_ASSERT(varsUsedByStmt1.size() == 4);
	CPPUNIT_ASSERT(varsUsedByStmt1[0] == 0);
	CPPUNIT_ASSERT(varsUsedByStmt1[1] == 1);
	CPPUNIT_ASSERT(varsUsedByStmt1[2] == 2);
	CPPUNIT_ASSERT(varsUsedByStmt1[3] == 3);

	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);
}

void UsesTest::TestUses() {
	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);

	Uses::SetStmtUsesVar(1, 0);
	Uses::SetStmtUsesVar(2, 0);
	Uses::SetStmtUsesVar(3, 0);
	Uses::SetStmtUsesVar(4, 0);

	Uses::SetStmtUsesVar(1, 1);
	Uses::SetStmtUsesVar(2, 1);
	Uses::SetStmtUsesVar(3, 1);
	Uses::SetStmtUsesVar(4, 1);

	Uses::SetStmtUsesVar(1, 2);
	Uses::SetStmtUsesVar(2, 2);
	Uses::SetStmtUsesVar(3, 2);
	Uses::SetStmtUsesVar(4, 2);

	Uses::SetStmtUsesVar(1, 3);
	Uses::SetStmtUsesVar(2, 3);
	Uses::SetStmtUsesVar(3, 3);
	Uses::SetStmtUsesVar(4, 3);

	Uses::SetStmtUsesVar(5, 4);
	Uses::SetStmtUsesVar(10, 15);

	CPPUNIT_ASSERT(Uses::HasAnyUses() == true);
	CPPUNIT_ASSERT(Uses::SizeOfUses() == 18);

	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 0));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 1));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 2));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(1, 3));

	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(2, 0));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(2, 1));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(2, 2));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(2, 3));

	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(3, 0));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(3, 1));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(3, 2));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(3, 3));

	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(4, 0));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(4, 1));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(4, 2));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(4, 3));

	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(5, 4));
	CPPUNIT_ASSERT(Uses::IsStmtUsingVar(10, 15));

	vector<int> stmtsUsingVar0 = Uses::GetStmtUsingVar(0);
	CPPUNIT_ASSERT(stmtsUsingVar0.size() == 4);
	CPPUNIT_ASSERT(stmtsUsingVar0[0] == 1);
	CPPUNIT_ASSERT(stmtsUsingVar0[1] == 2);
	CPPUNIT_ASSERT(stmtsUsingVar0[2] == 3);
	CPPUNIT_ASSERT(stmtsUsingVar0[3] == 4);

	vector<int> stmtsUsingVar1 = Uses::GetStmtUsingVar(1);
	CPPUNIT_ASSERT(stmtsUsingVar1.size() == 4);
	CPPUNIT_ASSERT(stmtsUsingVar1[0] == 1);
	CPPUNIT_ASSERT(stmtsUsingVar1[1] == 2);
	CPPUNIT_ASSERT(stmtsUsingVar1[2] == 3);
	CPPUNIT_ASSERT(stmtsUsingVar1[3] == 4);

	vector<int> stmtsUsingVar2 = Uses::GetStmtUsingVar(2);
	CPPUNIT_ASSERT(stmtsUsingVar2.size() == 4);
	CPPUNIT_ASSERT(stmtsUsingVar2[0] == 1);
	CPPUNIT_ASSERT(stmtsUsingVar2[1] == 2);
	CPPUNIT_ASSERT(stmtsUsingVar2[2] == 3);
	CPPUNIT_ASSERT(stmtsUsingVar2[3] == 4);

	vector<int> stmtsUsingVar3 = Uses::GetStmtUsingVar(3);
	CPPUNIT_ASSERT(stmtsUsingVar3.size() == 4);
	CPPUNIT_ASSERT(stmtsUsingVar3[0] == 1);
	CPPUNIT_ASSERT(stmtsUsingVar3[1] == 2);
	CPPUNIT_ASSERT(stmtsUsingVar3[2] == 3);
	CPPUNIT_ASSERT(stmtsUsingVar3[3] == 4);

	vector<int> stmtsUsingVar4 = Uses::GetStmtUsingVar(4);
	CPPUNIT_ASSERT(stmtsUsingVar4.size() == 1);
	CPPUNIT_ASSERT(stmtsUsingVar4[0] == 5);

	vector<int> stmtsUsingVar15 = Uses::GetStmtUsingVar(15);
	CPPUNIT_ASSERT(stmtsUsingVar15.size() == 1);
	CPPUNIT_ASSERT(stmtsUsingVar15[0] == 10);

	vector<int> varsUsedByStmt1 = Uses::GetVarUsedByStmt(1);
	CPPUNIT_ASSERT(varsUsedByStmt1.size() == 4);
	CPPUNIT_ASSERT(varsUsedByStmt1[0] == 0);
	CPPUNIT_ASSERT(varsUsedByStmt1[1] == 1);
	CPPUNIT_ASSERT(varsUsedByStmt1[2] == 2);
	CPPUNIT_ASSERT(varsUsedByStmt1[3] == 3);

	vector<int> varsUsedByStmt2 = Uses::GetVarUsedByStmt(2);
	CPPUNIT_ASSERT(varsUsedByStmt2.size() == 4);
	CPPUNIT_ASSERT(varsUsedByStmt2[0] == 0);
	CPPUNIT_ASSERT(varsUsedByStmt2[1] == 1);
	CPPUNIT_ASSERT(varsUsedByStmt2[2] == 2);
	CPPUNIT_ASSERT(varsUsedByStmt2[3] == 3);

	vector<int> varsUsedByStmt3 = Uses::GetVarUsedByStmt(3);
	CPPUNIT_ASSERT(varsUsedByStmt3.size() == 4);
	CPPUNIT_ASSERT(varsUsedByStmt3[0] == 0);
	CPPUNIT_ASSERT(varsUsedByStmt3[1] == 1);
	CPPUNIT_ASSERT(varsUsedByStmt3[2] == 2);
	CPPUNIT_ASSERT(varsUsedByStmt3[3] == 3);

	vector<int> varsUsedByStmt4 = Uses::GetVarUsedByStmt(4);
	CPPUNIT_ASSERT(varsUsedByStmt4.size() == 4);
	CPPUNIT_ASSERT(varsUsedByStmt4[0] == 0);
	CPPUNIT_ASSERT(varsUsedByStmt4[1] == 1);
	CPPUNIT_ASSERT(varsUsedByStmt4[2] == 2);
	CPPUNIT_ASSERT(varsUsedByStmt4[3] == 3);

	vector<int> varsUsedByStmt5 = Uses::GetVarUsedByStmt(5);
	CPPUNIT_ASSERT(varsUsedByStmt5.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt5[0] == 4);

	vector<int> varsUsedByStmt10 = Uses::GetVarUsedByStmt(10);
	CPPUNIT_ASSERT(varsUsedByStmt10.size() == 1);
	CPPUNIT_ASSERT(varsUsedByStmt10[0] == 15);

	Uses::ClearData();
	CPPUNIT_ASSERT(Uses::HasAnyUses() == false);

}

void UsesTest::TestNoProc() {
	Uses::ClearData();
	CPPUNIT_ASSERT(!Uses::HasAnyUses());
	CPPUNIT_ASSERT(!Uses::IsProcUsingVar(4, 3));
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::SizeOfUses());

}

void UsesTest::TestSingleProc() {
	Uses::ClearData();
	CPPUNIT_ASSERT(!Uses::HasAnyUses());

	//Uses::SetProcUsesVar*/
	Uses::SetProcUsesVar(2, 3);
	Uses::SetProcUsesVar(2, 3); // test repeated additions
	CPPUNIT_ASSERT_EQUAL(1, (int) Uses::SizeOfUses());
	
	CPPUNIT_ASSERT(Uses::IsProcUsingVar(2, 3));
	CPPUNIT_ASSERT(!Uses::IsStmtUsingVar(2, 3)); // test correct proc tables used not stmt tables

	//Uses::GetProcUsingVar*/
	vector<int> procsUsingVar3 = Uses::GetProcUsingVar(3);
	CPPUNIT_ASSERT_EQUAL(1, (int) procsUsingVar3.size());
	CPPUNIT_ASSERT(procsUsingVar3[0] == 2);
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(3).size());
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetProcUsingVar(0).size());

	//Uses::GetVarUsedByProc*/
	vector<int> varsUsedByProc2 = Uses::GetVarUsedByProc(2);
	CPPUNIT_ASSERT_EQUAL(1, (int) varsUsedByProc2.size());
	CPPUNIT_ASSERT(varsUsedByProc2[0] == 3);
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetVarUsedByStmt(2).size());
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetVarUsedByProc(0).size());
}

void UsesTest::TestMultipleProcs() {
	Uses::ClearData();
	CPPUNIT_ASSERT(!Uses::HasAnyUses());

	//Uses::SetProcUsesVar*/
	Uses::SetProcUsesVar(1, 0);
	Uses::SetProcUsesVar(2, 1);
	Uses::SetProcUsesVar(3, 2);
	Uses::SetProcUsesVar(4, 3);

	CPPUNIT_ASSERT_EQUAL(4, (int) Uses::SizeOfUses());
	
	//Uses::IsProcModifying*/
	CPPUNIT_ASSERT(Uses::IsProcUsingVar(1, 0));
	CPPUNIT_ASSERT(Uses::IsProcUsingVar(2, 1));
	CPPUNIT_ASSERT(Uses::IsProcUsingVar(3, 2));
	CPPUNIT_ASSERT(Uses::IsProcUsingVar(4, 3));
	// false results
	CPPUNIT_ASSERT(!Uses::IsProcUsingVar(0, 0));
	CPPUNIT_ASSERT(!Uses::IsProcUsingVar(3, 4));
	
	CPPUNIT_ASSERT(!Uses::IsStmtUsingVar(1, 0));
	CPPUNIT_ASSERT(!Uses::IsStmtUsingVar(4, 3));

	//Uses::GetProcModifying*/
	vector<int> procsUsesVar0 = Uses::GetProcUsingVar(0);
	CPPUNIT_ASSERT_EQUAL(1, (int) procsUsesVar0.size());
	CPPUNIT_ASSERT(procsUsesVar0[0] == 1);
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(0).size());

	vector<int> procsUsesVar3 = Uses::GetProcUsingVar(3);
	CPPUNIT_ASSERT_EQUAL(1, (int) procsUsesVar3.size());
	CPPUNIT_ASSERT(procsUsesVar3[0] == 4);
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(3).size());

	// test empty result
	vector<int> procsUsesVar4 = Uses::GetProcUsingVar(4);
	CPPUNIT_ASSERT_EQUAL(0, (int) procsUsesVar4.size());
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(4).size());

	//Uses::GetVarUsedByProc*/
	vector<int> varsUsedByProc1 = Uses::GetVarUsedByProc(1);
	CPPUNIT_ASSERT_EQUAL(1, (int) varsUsedByProc1.size());
	CPPUNIT_ASSERT(varsUsedByProc1[0] == 0);
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(1).size());

	vector<int> varsUsedByProc4 = Uses::GetVarUsedByProc(4);
	CPPUNIT_ASSERT_EQUAL(1, (int) varsUsedByProc4.size());
	CPPUNIT_ASSERT(varsUsedByProc4[0] == 3);
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(4).size());

	// test empty result
	vector<int> varsUsedByProc0 = Uses::GetVarUsedByProc(0);
	CPPUNIT_ASSERT_EQUAL(0, (int) varsUsedByProc0.size());
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetVarUsedByProc(0).size());

}

void UsesTest::TestMultipleProcsUsingSingleVar() {
	Uses::ClearData();
	CPPUNIT_ASSERT(!Uses::HasAnyUses());

	//Uses::SetProcUsesVar*/
	Uses::SetProcUsesVar(1, 0);
	Uses::SetProcUsesVar(2, 0);
	Uses::SetProcUsesVar(3, 0);
	Uses::SetProcUsesVar(4, 0);

	CPPUNIT_ASSERT_EQUAL(4, (int) Uses::SizeOfUses());

	//Uses::IsProcUsingVar*/
	CPPUNIT_ASSERT(Uses::IsProcUsingVar(1, 0));
	CPPUNIT_ASSERT(Uses::IsProcUsingVar(4, 0));
	// false results
	CPPUNIT_ASSERT(!Uses::IsProcUsingVar(0, 0));
	CPPUNIT_ASSERT(!Uses::IsProcUsingVar(5, 0));
	CPPUNIT_ASSERT(!Uses::IsProcUsingVar(0, 1));

	//Uses::GetProcUsingVar*/
	vector<int> GetProcUsingVar0 = Uses::GetProcUsingVar(0);
	CPPUNIT_ASSERT_EQUAL(4, (int) GetProcUsingVar0.size());
	CPPUNIT_ASSERT(GetProcUsingVar0[0] == 1);
	CPPUNIT_ASSERT(GetProcUsingVar0[1] == 2);
	CPPUNIT_ASSERT(GetProcUsingVar0[2] == 3);
	CPPUNIT_ASSERT(GetProcUsingVar0[3] == 4);
	//empty results
	vector<int> GetProcUsingVar1 = Uses::GetProcUsingVar(1);
	CPPUNIT_ASSERT_EQUAL(0, (int) GetProcUsingVar1.size());

	//Uses::GetVarUsedByProc*/
	vector<int> varsUsedByProc1 = Uses::GetVarUsedByProc(1);
	CPPUNIT_ASSERT_EQUAL(1, (int) varsUsedByProc1.size());
	CPPUNIT_ASSERT(varsUsedByProc1[0] == 0);

	vector<int> varsUsedByProc4 = Uses::GetVarUsedByProc(4);
	CPPUNIT_ASSERT_EQUAL(1, (int) varsUsedByProc4.size());
	CPPUNIT_ASSERT(varsUsedByProc4[0] == 0);

	//empty results
	vector<int> varsUsedByProc0 = Uses::GetVarUsedByProc(0);
	CPPUNIT_ASSERT_EQUAL(0, (int) varsUsedByProc0.size());

}

void UsesTest::TestSingleProcUsingMultipleVars() {
	Uses::ClearData();
	CPPUNIT_ASSERT(!Uses::HasAnyUses());

	//Uses::SetProcUsesVar*/
	Uses::SetProcUsesVar(1, 0);
	Uses::SetProcUsesVar(1, 1);
	Uses::SetProcUsesVar(1, 2);
	Uses::SetProcUsesVar(1, 3);

	CPPUNIT_ASSERT_EQUAL(4, (int) Uses::SizeOfUses());
	
	//Uses::IsProcUsingVar*/
	CPPUNIT_ASSERT(Uses::IsProcUsingVar(1, 0));
	CPPUNIT_ASSERT(Uses::IsProcUsingVar(1, 3));
	//false results
	CPPUNIT_ASSERT(!Uses::IsProcUsingVar(0, 1));
	CPPUNIT_ASSERT(!Uses::IsStmtUsingVar(1, 3));

	//Uses::GetProcUsingVar*/
	vector<int> procsUsingVar0 = Uses::GetProcUsingVar(0);
	CPPUNIT_ASSERT_EQUAL(1, (int) procsUsingVar0.size());
	CPPUNIT_ASSERT(procsUsingVar0[0] == 1);
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(0).size());

	vector<int> procsUsingVar3 = Uses::GetProcUsingVar(3);
	CPPUNIT_ASSERT_EQUAL(1, (int) procsUsingVar3.size());
	CPPUNIT_ASSERT(procsUsingVar3[0] == 1);
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(3).size());

	// empty results
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(-1).size());
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetStmtUsingVar(4).size());

	//Uses::GetVarUsedByProc*/
	vector<int> varsUsedByProc1 = Uses::GetVarUsedByProc(1);
	CPPUNIT_ASSERT_EQUAL(4, (int) varsUsedByProc1.size());
	CPPUNIT_ASSERT(varsUsedByProc1[0] == 0);
	CPPUNIT_ASSERT(varsUsedByProc1[3] == 3);
	
	// empty results
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetVarUsedByProc(0).size());
	CPPUNIT_ASSERT_EQUAL(0, (int) Uses::GetVarUsedByStmt(0).size());
}