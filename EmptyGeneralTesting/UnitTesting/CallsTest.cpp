#include <cppunit\config\SourcePrefix.h>

#include "..\SPA\PKB\Calls.h"
#include "CallsTest.h"

#include <string>
#include <stdint.h>
#include <algorithm>

void CallsTest::setUp() {
	Calls::ClearData();
}

void CallsTest::tearDown() {
	Calls::ClearData();
}

CPPUNIT_TEST_SUITE_REGISTRATION(CallsTest);

void CallsTest::testSetCalls() {
	Calls::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Calls::SizeOfCalls());

	Calls::SetCalls(1, 0);
	CPPUNIT_ASSERT_EQUAL(1, Calls::SizeOfCalls());
	Calls::SetCalls(1, 0);
	CPPUNIT_ASSERT_EQUAL(1, Calls::SizeOfCalls());
	Calls::SetCalls(1, 1);
	CPPUNIT_ASSERT_EQUAL(2, Calls::SizeOfCalls());
	Calls::SetCalls(2, 0);
	CPPUNIT_ASSERT_EQUAL(3, Calls::SizeOfCalls());

	Calls::SetCalls(2, 1);
	CPPUNIT_ASSERT_EQUAL(4, Calls::SizeOfCalls());
	// set recurssive call (illegal)
	Calls::SetCalls(1, 2);
	CPPUNIT_ASSERT_EQUAL(4, Calls::SizeOfCalls());
	
}

void CallsTest::testIsCalls() {
	Calls::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Calls::SizeOfCalls());

	Calls::SetCalls(0, 0);
	Calls::SetCalls(0, 1);
	Calls::SetCalls(0, 2);

	Calls::SetCalls(1, 0);	// illegal
	Calls::SetCalls(1, 1);
	Calls::SetCalls(1, 2);

	Calls::SetCalls(2, 0);	// illegal
	Calls::SetCalls(2, 1);	// illegal
	Calls::SetCalls(2, 2);

	CPPUNIT_ASSERT_EQUAL(6, Calls::SizeOfCalls());

	CPPUNIT_ASSERT(Calls::IsCalls(0, 0));
	CPPUNIT_ASSERT(Calls::IsCalls(0, 1));
	CPPUNIT_ASSERT(Calls::IsCalls(0, 2));
	CPPUNIT_ASSERT(Calls::IsCalls(2, 2));

	CPPUNIT_ASSERT(!Calls::IsCalls(1, 0));
	CPPUNIT_ASSERT(!Calls::IsCalls(2, 0));
	CPPUNIT_ASSERT(!Calls::IsCalls(2, 1));
}

void CallsTest::testGetProcsCalledBy() {
	Calls::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Calls::SizeOfCalls());

	Calls::SetCalls(1, 0);
	Calls::SetCalls(1, 1);
	Calls::SetCalls(1, 2);

	Calls::SetCalls(2, 0);
	Calls::SetCalls(2, 1);	// illegal

	Calls::SetCalls(3, 2);

	CPPUNIT_ASSERT_EQUAL(5, Calls::SizeOfCalls());
	
	vector<int> procsCalledBy1 = Calls::GetProcsCalledBy(1);
	CPPUNIT_ASSERT_EQUAL(3, (int) procsCalledBy1.size());
	CPPUNIT_ASSERT_EQUAL(0, procsCalledBy1[0]);
	CPPUNIT_ASSERT_EQUAL(1, procsCalledBy1[1]);
	CPPUNIT_ASSERT_EQUAL(2, procsCalledBy1[2]);

	vector<int> procsCalledBy2 = Calls::GetProcsCalledBy(2);
	CPPUNIT_ASSERT_EQUAL(1, (int) procsCalledBy2.size());
	CPPUNIT_ASSERT_EQUAL(0, procsCalledBy2[0]);
	
	vector<int> procsCalledBy3 = Calls::GetProcsCalledBy(3);
	CPPUNIT_ASSERT_EQUAL(1, (int) procsCalledBy3.size());
	CPPUNIT_ASSERT_EQUAL(2, procsCalledBy3[0]);

	// for a non-existing stmt#
	vector<int> procsCalledBy15 = Calls::GetProcsCalledBy(15);
	CPPUNIT_ASSERT_EQUAL(0, (int) procsCalledBy15.size());
}

void CallsTest::testGetProcsCalling() {
	Calls::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Calls::SizeOfCalls());

	Calls::SetCalls(0, 4);
	Calls::SetCalls(1, 4);
	Calls::SetCalls(2, 4);
	Calls::SetCalls(3, 4);

	Calls::SetCalls(0, 5);
	Calls::SetCalls(1, 5);
	Calls::SetCalls(4, 5);
	Calls::SetCalls(4, 6);

	vector<int> procCalling4 = Calls::GetProcsCalling(4);
	CPPUNIT_ASSERT_EQUAL(4, (int) procCalling4.size());
	CPPUNIT_ASSERT_EQUAL(0, procCalling4[0]);
	CPPUNIT_ASSERT_EQUAL(1, procCalling4[1]);
	CPPUNIT_ASSERT_EQUAL(2, procCalling4[2]);
	CPPUNIT_ASSERT_EQUAL(3, procCalling4[3]);

	vector<int> procCalling5 = Calls::GetProcsCalling(5);
	CPPUNIT_ASSERT_EQUAL(3, (int) procCalling5.size());
	CPPUNIT_ASSERT_EQUAL(0, procCalling5[0]);
	CPPUNIT_ASSERT_EQUAL(1, procCalling5[1]);
	CPPUNIT_ASSERT_EQUAL(4, procCalling5[2]);

	vector<int> procCalling6 = Calls::GetProcsCalling(6);
	CPPUNIT_ASSERT_EQUAL(1, (int) procCalling6.size());
	CPPUNIT_ASSERT_EQUAL(4, procCalling6[0]);

}

void CallsTest::testIsCallsT() {
	Calls::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Calls::SizeOfCalls());

	Calls::SetCalls(0, 1);
	Calls::SetCalls(1, 3);
	Calls::SetCalls(3, 6);
	
	Calls::SetCalls(0, 2);
	Calls::SetCalls(2, 4); Calls::SetCalls(4, 5);
	Calls::SetCalls(4, 1);

	CPPUNIT_ASSERT(Calls::IsCallsT(0, 1));
	CPPUNIT_ASSERT(Calls::IsCallsT(0, 3));
	CPPUNIT_ASSERT(Calls::IsCallsT(4, 6));
	CPPUNIT_ASSERT(Calls::IsCallsT(2, 6));

	CPPUNIT_ASSERT(Calls::IsCallsT(4, 1));
	CPPUNIT_ASSERT(Calls::IsCallsT(4, 3));

	CPPUNIT_ASSERT(!Calls::IsCallsT(1, 4));
	CPPUNIT_ASSERT(!Calls::IsCallsT(1, 5));
	CPPUNIT_ASSERT(!Calls::IsCallsT(5, 6));

}

void CallsTest::testGetProcsCalledTBy() {
	Calls::ClearData();
	CPPUNIT_ASSERT_EQUAL(0,  Calls::SizeOfCalls());

	Calls::SetCalls(1, 2);
	Calls::SetCalls(1, 3);

	Calls::SetCalls(2, 4);
	Calls::SetCalls(2, 5);
	Calls::SetCalls(2, 6);

	Calls::SetCalls(3, 5);
	Calls::SetCalls(3, 6);
	Calls::SetCalls(3, 7);

	Calls::SetCalls(4, 8);
	Calls::SetCalls(5, 8);

	Calls::SetCalls(6, 9);
	Calls::SetCalls(7, 9);

	CPPUNIT_ASSERT_EQUAL(12,  Calls::SizeOfCalls());
	
	vector<int> procsCalledTBy1 = Calls::GetProcsCalledTBy(1);
	CPPUNIT_ASSERT_EQUAL(8, (int) procsCalledTBy1.size());
	sort(procsCalledTBy1.begin(), procsCalledTBy1.end());
	CPPUNIT_ASSERT_EQUAL(2, procsCalledTBy1[0]);
	CPPUNIT_ASSERT_EQUAL(3, procsCalledTBy1[1]);
	CPPUNIT_ASSERT_EQUAL(4, procsCalledTBy1[2]);
	CPPUNIT_ASSERT_EQUAL(5, procsCalledTBy1[3]);
	CPPUNIT_ASSERT_EQUAL(6, procsCalledTBy1[4]);
	CPPUNIT_ASSERT_EQUAL(7, procsCalledTBy1[5]);
	CPPUNIT_ASSERT_EQUAL(8, procsCalledTBy1[6]);
	CPPUNIT_ASSERT_EQUAL(9, procsCalledTBy1[7]);

	vector<int> procsCalledTBy3 = Calls::GetProcsCalledTBy(3);
	sort(procsCalledTBy3.begin(), procsCalledTBy3.end());
	CPPUNIT_ASSERT_EQUAL(5, (int) procsCalledTBy3.size());
	CPPUNIT_ASSERT_EQUAL(5, procsCalledTBy3[0]);
	CPPUNIT_ASSERT_EQUAL(6, procsCalledTBy3[1]);
	CPPUNIT_ASSERT_EQUAL(7, procsCalledTBy3[2]);
	CPPUNIT_ASSERT_EQUAL(8, procsCalledTBy3[3]);
	CPPUNIT_ASSERT_EQUAL(9, procsCalledTBy3[4]);
}

void CallsTest::testGetProcsCallingT() {
	Calls::ClearData();
	CPPUNIT_ASSERT_EQUAL(0,  Calls::SizeOfCalls());

	Calls::SetCalls(1, 2);
	Calls::SetCalls(1, 3);

	Calls::SetCalls(2, 4);
	Calls::SetCalls(2, 5);
	Calls::SetCalls(2, 6);

	Calls::SetCalls(3, 5);
	Calls::SetCalls(3, 6);
	Calls::SetCalls(3, 7);

	Calls::SetCalls(4, 8);
	Calls::SetCalls(5, 8);

	Calls::SetCalls(6, 9);
	Calls::SetCalls(7, 9);

	CPPUNIT_ASSERT_EQUAL(12,  Calls::SizeOfCalls());
	
	vector<int> procsCallingT1 = Calls::GetProcsCallingT(1);
	CPPUNIT_ASSERT_EQUAL(0, (int) procsCallingT1.size());

	vector<int> procsCallingT5 = Calls::GetProcsCallingT(5);
	sort(procsCallingT5.begin(), procsCallingT5.end());
	CPPUNIT_ASSERT_EQUAL(3, (int) procsCallingT5.size());
	CPPUNIT_ASSERT_EQUAL(1, procsCallingT5[0]);
	CPPUNIT_ASSERT_EQUAL(2, procsCallingT5[1]);
	CPPUNIT_ASSERT_EQUAL(3, procsCallingT5[2]);

	vector<int> procsCallingT9 = Calls::GetProcsCallingT(9);
	sort(procsCallingT9.begin(), procsCallingT9.end());
	CPPUNIT_ASSERT_EQUAL(5, (int) procsCallingT9.size());
	CPPUNIT_ASSERT_EQUAL(1, procsCallingT9[0]);
	CPPUNIT_ASSERT_EQUAL(2, procsCallingT9[1]);
	CPPUNIT_ASSERT_EQUAL(3, procsCallingT9[2]);
	CPPUNIT_ASSERT_EQUAL(6, procsCallingT9[3]);
	CPPUNIT_ASSERT_EQUAL(7, procsCallingT9[4]);

}