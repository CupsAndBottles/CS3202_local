#include <cppunit\config\SourcePrefix.h>

#include "..\SPA\PKB\Next.h"
#include "NextTest.h"

#include <iostream>
#include <string>
#include <stdint.h>
#include <algorithm>

void NextTest::setUp() {
	Next::ClearData();
}

void NextTest::tearDown() {
	Next::ClearData();
}

CPPUNIT_TEST_SUITE_REGISTRATION(NextTest);

void NextTest::TestSetNext() {
	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// regular corerct insert
	Next::SetNext(1, 2);
	CPPUNIT_ASSERT_EQUAL(1, Next::SizeOfNext());
	// repeated insertion, not allowed
	Next::SetNext(1, 2);
	CPPUNIT_ASSERT_EQUAL(1, Next::SizeOfNext());
	Next::SetNext(1, 1);
	CPPUNIT_ASSERT_EQUAL(1, Next::SizeOfNext());

	// inserting more than 2 children
	Next::SetNext(1, 3);
	CPPUNIT_ASSERT_EQUAL(2, Next::SizeOfNext());
	Next::SetNext(1, 4);
	CPPUNIT_ASSERT_EQUAL(2, Next::SizeOfNext());
	
	vector<int> linesAfter1 = Next::GetNextAfter(1);
	CPPUNIT_ASSERT_EQUAL(2, (int) linesAfter1.size());
	CPPUNIT_ASSERT_EQUAL(2, linesAfter1.at(0));
	CPPUNIT_ASSERT_EQUAL(3, linesAfter1.at(1));

	// inserting more than 1 parent, legal operations
	Next::SetNext(4, 2);
	Next::SetNext(5, 2);
	CPPUNIT_ASSERT_EQUAL(4, Next::SizeOfNext());
	
	vector<int> linesBefore2 = Next::GetNextBefore(2);
	CPPUNIT_ASSERT_EQUAL(3, (int) linesBefore2.size());
	CPPUNIT_ASSERT_EQUAL(1, linesBefore2.at(0));
	CPPUNIT_ASSERT_EQUAL(4, linesBefore2.at(1));
	CPPUNIT_ASSERT_EQUAL(5, linesBefore2.at(2));

	// inserting more than 1 parent, illegal operations
	
	// before and after share the same parent
	Next::SetNext(2, 3);
	CPPUNIT_ASSERT_EQUAL(4, Next::SizeOfNext());
	Next::SetNext(3, 2);
	CPPUNIT_ASSERT_EQUAL(4, Next::SizeOfNext());
	
}

void NextTest::TestGetNextAfter() {
	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// lines before and after don't exist
	vector <int> linesAfterNothing = Next::GetNextAfter(1);
	CPPUNIT_ASSERT_EQUAL(0, (int) linesAfterNothing.size());

	// lines after don't exist
	Next::SetNext(1, 2);
	CPPUNIT_ASSERT_EQUAL(1, Next::SizeOfNext());
	vector<int> linesAfter2 = Next::GetNextAfter(2);
	CPPUNIT_ASSERT_EQUAL(0, (int) linesAfter2.size());
	
	// only one line after
	vector<int> linesAfter1 = Next::GetNextAfter(1);
	CPPUNIT_ASSERT_EQUAL(1, (int) linesAfter1.size());
	CPPUNIT_ASSERT_EQUAL(2, linesAfter1.at(0));

	// only 2 lines after
	Next::SetNext(1, 3);
	CPPUNIT_ASSERT_EQUAL(2, Next::SizeOfNext());
	Next::SetNext(1, 4);
	CPPUNIT_ASSERT_EQUAL(2, Next::SizeOfNext());
	linesAfter1 = Next::GetNextAfter(1);
	CPPUNIT_ASSERT_EQUAL(2, (int) linesAfter1.size());
	CPPUNIT_ASSERT_EQUAL(2, linesAfter1.at(0));
	CPPUNIT_ASSERT_EQUAL(3, linesAfter1.at(1));
}

void NextTest::TestIsNextBeforeBitVector() {
	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	Next::SetNext(11, 12);
	Next::SetNext(11, 12); //illegal
	Next::SetNext(11, 13);
	CPPUNIT_ASSERT_EQUAL(2, Next::SizeOfNext());

	Next::SetNext(12, 13); //illegal
	Next::SetNext(12, 14); 
	Next::SetNext(13, 15);
	CPPUNIT_ASSERT_EQUAL(4, Next::SizeOfNext());
	
	CPPUNIT_ASSERT_EQUAL(4, Next::SizeOfNext());
	CPPUNIT_ASSERT(Next::IsNext(11, 12));
	CPPUNIT_ASSERT(Next::IsNext(11, 13));
	CPPUNIT_ASSERT(Next::IsNext(12, 14));
	CPPUNIT_ASSERT(Next::IsNext(13, 15));

	CPPUNIT_ASSERT(!Next::IsNext(12, 13));
	CPPUNIT_ASSERT(!Next::IsNext(13, 14));
	CPPUNIT_ASSERT(!Next::IsNext(12, 15));
	CPPUNIT_ASSERT(!Next::IsNext(1, 2));
	CPPUNIT_ASSERT(!Next::IsNext(1, 12));
	CPPUNIT_ASSERT(!Next::IsNext(12, 1));
}

void NextTest::TestGetNextBefore() {
	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// lines before and after don't exist
	vector <int> linesBeforeNothing = Next::GetNextBefore(1);
	CPPUNIT_ASSERT_EQUAL(0, (int) linesBeforeNothing.size());

	// lines before don't exist
	Next::SetNext(1, 2);
	CPPUNIT_ASSERT_EQUAL(1, Next::SizeOfNext());
	vector<int> linesBefore1 = Next::GetNextBefore(1);
	CPPUNIT_ASSERT_EQUAL(0, (int) linesBefore1.size());

	// many lines before
	Next::SetNext(3, 2);
	CPPUNIT_ASSERT_EQUAL(2, Next::SizeOfNext());
	Next::SetNext(4, 2);
	CPPUNIT_ASSERT_EQUAL(3, Next::SizeOfNext());
	Next::SetNext(5, 2);
	CPPUNIT_ASSERT_EQUAL(4, Next::SizeOfNext());
	Next::SetNext(3, 2);	// illegal
	CPPUNIT_ASSERT_EQUAL(4, Next::SizeOfNext());

	vector<int> linesBefore2 = Next::GetNextBefore(2);
	CPPUNIT_ASSERT_EQUAL(4, (int) linesBefore2.size());
	CPPUNIT_ASSERT_EQUAL(1, linesBefore2.at(0));
	CPPUNIT_ASSERT_EQUAL(3, linesBefore2.at(1));
	CPPUNIT_ASSERT_EQUAL(4, linesBefore2.at(2));
	CPPUNIT_ASSERT_EQUAL(5, linesBefore2.at(3));
}

void NextTest::TestIsNextTBeforeBitVector1() {
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// test single linear
	Next::SetNext(1, 2);
	Next::SetNext(2, 3);
	CPPUNIT_ASSERT_EQUAL(2, Next::SizeOfNext());

	CPPUNIT_ASSERT(Next::IsNextT(1, 2));
	CPPUNIT_ASSERT(Next::IsNextT(2, 3));

	// test single if-else
	Next::SetNext(3, 4);
	Next::SetNext(4, 6);
	Next::SetNext(3, 5);
	Next::SetNext(5, 7);
	CPPUNIT_ASSERT_EQUAL(6, Next::SizeOfNext());
	CPPUNIT_ASSERT(Next::IsNextT(3, 6));
	CPPUNIT_ASSERT(Next::IsNextT(3, 7));
	CPPUNIT_ASSERT(Next::IsNextT(1, 6));
	CPPUNIT_ASSERT(Next::IsNextT(1, 7));
	CPPUNIT_ASSERT(!Next::IsNextT(4, 7));
	CPPUNIT_ASSERT(!Next::IsNextT(5, 6));

	Next::SetNext(6, 8);
	Next::SetNext(7, 8);
	CPPUNIT_ASSERT_EQUAL(8, Next::SizeOfNext());
	CPPUNIT_ASSERT(Next::IsNextT(3, 8));
	CPPUNIT_ASSERT(Next::IsNextT(4, 8));
	CPPUNIT_ASSERT(Next::IsNextT(5, 8));

	// test single while
	Next::SetNext(8, 9);
	Next::SetNext(9, 8);
	Next::SetNext(8, 10);
	CPPUNIT_ASSERT_EQUAL(11, Next::SizeOfNext());
	CPPUNIT_ASSERT(Next::IsNextT(8, 9));
	CPPUNIT_ASSERT(Next::IsNextT(9, 8));
	CPPUNIT_ASSERT(Next::IsNextT(8, 8));
	CPPUNIT_ASSERT(Next::IsNextT(9, 9));
	CPPUNIT_ASSERT(Next::IsNextT(9, 10));

}

void NextTest::TestIsNextTBeforeBitVector2() {
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// test nested while stmts
	// 1. while x {
	// 2.	while y {
	// 3.		while z {
	// 4.			x = y + z}}}
	
	Next::SetNext(1, 2);
	Next::SetNext(2, 3);
	Next::SetNext(3, 4);
	Next::SetNext(4, 3);

	Next::SetNext(3, 2);

	Next::SetNext(2, 1);

	CPPUNIT_ASSERT_EQUAL(6, Next::SizeOfNext());
	CPPUNIT_ASSERT(Next::IsNextT(1, 1));
	CPPUNIT_ASSERT(Next::IsNextT(4, 1));

	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// test nested if stmts
	// 1.	if a then {
	// 2.		a = a }
	//		else {
	// 3.		if b then {
	// 4.			b = b }
	//			else {
	// 5.			c = c}}
	// 6.	a = b + c
	
	Next::SetNext(1, 2);
	Next::SetNext(1, 3);
	Next::SetNext(3, 4);
	Next::SetNext(3, 5);

	Next::SetNext(4, 6);
	Next::SetNext(5, 6);
	Next::SetNext(2, 6);

	CPPUNIT_ASSERT_EQUAL(7, Next::SizeOfNext());
	CPPUNIT_ASSERT(Next::IsNextT(1, 6));
	CPPUNIT_ASSERT(Next::IsNextT(2, 6));
	CPPUNIT_ASSERT(!Next::IsNextT(2, 5));
	CPPUNIT_ASSERT(!Next::IsNextT(4, 5));

	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// test mixture of while and nested if stmts
	// 1. while x {
	// 2.	if a then {
	// 3.		a = a }
	//		else {
	// 4.		if b then {
	// 5.			b = b }
	//			else {
	// 6.			if c then {
	// 7.				c = c }
	//				else {
	// 8.				c = x}}}
	// 9.	x = x }
	// 10. x = a + b + c

	Next::SetNext(1, 2);
	Next::SetNext(2, 3);
	Next::SetNext(2, 4);

	Next::SetNext(4, 5);
	Next::SetNext(4, 6);
	
	Next::SetNext(6, 7);
	Next::SetNext(6, 8);
	
	Next::SetNext(7, 9);
	Next::SetNext(8, 9);
	Next::SetNext(5, 9);
	Next::SetNext(3, 9);

	Next::SetNext(9, 1);
	Next::SetNext(1, 10);

	CPPUNIT_ASSERT_EQUAL(13, Next::SizeOfNext());

	// "backwards" of nested if
	CPPUNIT_ASSERT(Next::IsNextT(7, 5));
	CPPUNIT_ASSERT(Next::IsNextT(5, 3));
	// siblings with same parent
	CPPUNIT_ASSERT(Next::IsNextT(3, 4));
	CPPUNIT_ASSERT(Next::IsNextT(4, 3));
	CPPUNIT_ASSERT(Next::IsNextT(1, 1));
	// the only false in this case
	CPPUNIT_ASSERT(!Next::IsNextT(10, 1));

}

/* 'basic'
	1,2
	 |
	 3
	/  \
   4  - 5--
   |  | |   |
   |  | 6   |
   |  | |   |
   |  | 7----
   |  |
   |  |_8
   |    |
   |	9
   |   / \
   |  10  11
   \   \  /
	\ __12*/

void NextTest::TestGetNextTAfter1() {
	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// 'basic'
	Next::SetNext(1, 2);
	Next::SetNext(2, 3);
	
	Next::SetNext(3, 4);
	Next::SetNext(3, 5);

	Next::SetNext(5, 6);
	Next::SetNext(6, 7);
	Next::SetNext(7, 5);
	Next::SetNext(5, 8);

	Next::SetNext(8, 9);

	Next::SetNext(9, 10);
	Next::SetNext(9, 11);

	Next::SetNext(10, 12);
	Next::SetNext(11, 12);
	Next::SetNext(4, 12);

	CPPUNIT_ASSERT_EQUAL(14, Next::SizeOfNext());

	vector<int> linesAfter1 = Next::GetNextTAfter(1);
	CPPUNIT_ASSERT_EQUAL(11, (int) linesAfter1.size());

	vector<int> linesAfter4 = Next::GetNextTAfter(4);
	CPPUNIT_ASSERT_EQUAL(1, (int) linesAfter4.size());
	CPPUNIT_ASSERT_EQUAL(12, linesAfter4.at(0));

	vector<int> linesAfter6 = Next::GetNextTAfter(6);
	CPPUNIT_ASSERT_EQUAL(8, (int) linesAfter6.size());
	sort(linesAfter6.begin(), linesAfter6.end());
	CPPUNIT_ASSERT_EQUAL(5, linesAfter6.at(0));
	CPPUNIT_ASSERT_EQUAL(6, linesAfter6.at(1));
	CPPUNIT_ASSERT_EQUAL(7, linesAfter6.at(2));
	CPPUNIT_ASSERT_EQUAL(10, linesAfter6.at(5));
	CPPUNIT_ASSERT_EQUAL(11, linesAfter6.at(6));
	CPPUNIT_ASSERT_EQUAL(12, linesAfter6.at(7));

}

void NextTest::TestGetNextTAfter2() {
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// test nested while stmts
	// 1. x = x
	// 2. while y {
	// 3.	while z {
	// 4.		x = y + z}}
	
	Next::SetNext(1, 2);
	Next::SetNext(2, 3);
	Next::SetNext(3, 4);
	Next::SetNext(4, 3);

	Next::SetNext(3, 2);

	CPPUNIT_ASSERT_EQUAL(5, Next::SizeOfNext());

	vector<int> linesAfter1 = Next::GetNextTAfter(1);
	CPPUNIT_ASSERT_EQUAL(3, (int) linesAfter1.size());
	sort(linesAfter1.begin(), linesAfter1.end());
	CPPUNIT_ASSERT_EQUAL(2, linesAfter1.at(0));
	CPPUNIT_ASSERT_EQUAL(3, linesAfter1.at(1));
	CPPUNIT_ASSERT_EQUAL(4, linesAfter1.at(2));

	vector<int> linesAfter3 = Next::GetNextTAfter(3);
	CPPUNIT_ASSERT_EQUAL(3, (int) linesAfter3.size());
	sort(linesAfter3.begin(), linesAfter3.end());
	CPPUNIT_ASSERT_EQUAL(2, linesAfter3.at(0));
	CPPUNIT_ASSERT_EQUAL(3, linesAfter3.at(1));
	CPPUNIT_ASSERT_EQUAL(4, linesAfter3.at(2));
	
	vector<int> linesAfter4 = Next::GetNextTAfter(4);
	CPPUNIT_ASSERT_EQUAL(3, (int) linesAfter4.size());
	sort(linesAfter4.begin(), linesAfter4.end());
	CPPUNIT_ASSERT_EQUAL(2, linesAfter4.at(0));
	CPPUNIT_ASSERT_EQUAL(3, linesAfter4.at(1));
	CPPUNIT_ASSERT_EQUAL(4, linesAfter4.at(2));

	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// test nested if stmts
	// 1.	if a then {
	// 2.		a = a }
	//		else {
	// 3.		if b then {
	// 4.			b = b }
	//			else {
	// 5.			if c then {
	// 6.				c = c }
	//				else {
	// 7.				c = x}}}
	// 8.	a = b + c
	
	Next::SetNext(1, 2);
	Next::SetNext(1, 23);
	
	Next::SetNext(23, 14);
	Next::SetNext(23, 5);

	Next::SetNext(5, 6);
	Next::SetNext(5, 7);
	
	Next::SetNext(2, 8);
	Next::SetNext(14, 8);
	Next::SetNext(6, 8);
	Next::SetNext(7, 8);

	CPPUNIT_ASSERT_EQUAL(10, Next::SizeOfNext());

	vector<int> linesAfter14 = Next::GetNextTAfter(14);
	CPPUNIT_ASSERT_EQUAL(1, (int) linesAfter14.size());
	CPPUNIT_ASSERT_EQUAL(8, linesAfter14.at(0));

	vector<int> linesAfter23 = Next::GetNextTAfter(23);
	CPPUNIT_ASSERT_EQUAL(5, (int) linesAfter23.size());
	sort(linesAfter23.begin(), linesAfter23.end());
	CPPUNIT_ASSERT_EQUAL(5, linesAfter23.at(0));
	CPPUNIT_ASSERT_EQUAL(6, linesAfter23.at(1));
	CPPUNIT_ASSERT_EQUAL(7, linesAfter23.at(2));
	CPPUNIT_ASSERT_EQUAL(8, linesAfter23.at(3));
	CPPUNIT_ASSERT_EQUAL(14, linesAfter23.at(4));
}

void NextTest::TestGetNextTBefore1() {
	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// 'basic'
	Next::SetNext(1, 2);
	Next::SetNext(2, 3);
	
	Next::SetNext(3, 4);
	Next::SetNext(3, 5);

	Next::SetNext(5, 6);
	Next::SetNext(6, 7);
	Next::SetNext(7, 5);
	Next::SetNext(5, 8);

	Next::SetNext(8, 9);

	Next::SetNext(9, 10);
	Next::SetNext(9, 11);

	Next::SetNext(10, 12);
	Next::SetNext(11, 12);
	Next::SetNext(4, 12);

	CPPUNIT_ASSERT_EQUAL(14, Next::SizeOfNext());

	vector<int> linesBefore12 = Next::GetNextTBefore(12);
	CPPUNIT_ASSERT_EQUAL(11, (int) linesBefore12.size());

	vector<int> linesBefore6 = Next::GetNextTBefore(6);
	CPPUNIT_ASSERT_EQUAL(6, (int) linesBefore6.size());
	sort(linesBefore6.begin(), linesBefore6.end());
	CPPUNIT_ASSERT_EQUAL(1, linesBefore6.at(0));
	CPPUNIT_ASSERT_EQUAL(2, linesBefore6.at(1));
	CPPUNIT_ASSERT_EQUAL(6, linesBefore6.at(4));
	CPPUNIT_ASSERT_EQUAL(7, linesBefore6.at(5));

	vector<int> linesBefore4 = Next::GetNextTBefore(4);
	CPPUNIT_ASSERT_EQUAL(3, (int) linesBefore4.size());
	sort(linesBefore4.begin(), linesBefore4.end());
	CPPUNIT_ASSERT_EQUAL(1, linesBefore4.at(0));
	CPPUNIT_ASSERT_EQUAL(2, linesBefore4.at(1));
	CPPUNIT_ASSERT_EQUAL(3, linesBefore4.at(2));


}

void NextTest::TestGetNextTBefore2() {
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// test nested while stmts
	// 1. x = x
	// 2. while y {
	// 3.	while z {
	// 4.		x = y + z}}
	
	Next::SetNext(1, 2);
	Next::SetNext(2, 3);
	Next::SetNext(3, 4);
	Next::SetNext(4, 3);

	Next::SetNext(3, 2);

	CPPUNIT_ASSERT_EQUAL(5, Next::SizeOfNext());

	vector<int> linesBefore1 = Next::GetNextTBefore(1);
	CPPUNIT_ASSERT_EQUAL(0, (int) linesBefore1.size());

	vector<int> linesBefore2 = Next::GetNextTBefore(2);
	CPPUNIT_ASSERT_EQUAL(4, (int) linesBefore2.size());

	sort(linesBefore2.begin(), linesBefore2.end());
	CPPUNIT_ASSERT_EQUAL(1, linesBefore2.at(0));
	CPPUNIT_ASSERT_EQUAL(2, linesBefore2.at(1));
	CPPUNIT_ASSERT_EQUAL(3, linesBefore2.at(2));
	CPPUNIT_ASSERT_EQUAL(4, linesBefore2.at(3));

	vector<int> linesBefore3 = Next::GetNextTBefore(3);
	CPPUNIT_ASSERT_EQUAL(4, (int) linesBefore3.size());
	sort(linesBefore3.begin(), linesBefore3.end());
	CPPUNIT_ASSERT_EQUAL(1, linesBefore3.at(0));
	CPPUNIT_ASSERT_EQUAL(2, linesBefore3.at(1));
	CPPUNIT_ASSERT_EQUAL(3, linesBefore3.at(2));
	CPPUNIT_ASSERT_EQUAL(4, linesBefore3.at(3));

	Next::ClearData();
	CPPUNIT_ASSERT_EQUAL(0, Next::SizeOfNext());

	// test nested if stmts
	// 1.	if a then {
	// 23.		a = a }
	//		else {
	// 3.		if b then {
	// 14.			b = b }
	//			else {
	// 5.			if c then {
	// 6.				c = c }
	//				else {
	// 7.				c = x}}}
	// 8.	a = b + c
	
	Next::SetNext(1, 2);
	Next::SetNext(1, 23);
	
	Next::SetNext(23, 14);
	Next::SetNext(23, 5);

	Next::SetNext(5, 6);
	Next::SetNext(5, 7);
	
	Next::SetNext(2, 8);
	Next::SetNext(14, 8);
	Next::SetNext(6, 8);
	Next::SetNext(7, 8);

	CPPUNIT_ASSERT_EQUAL(10, Next::SizeOfNext());
	
	vector<int> linesBefore5 = Next::GetNextTBefore(5);
	CPPUNIT_ASSERT_EQUAL(2, (int) linesBefore5.size());
	sort(linesBefore5.begin(), linesBefore5.end());
	CPPUNIT_ASSERT_EQUAL(1, linesBefore5.at(0));
	CPPUNIT_ASSERT_EQUAL(23, linesBefore5.at(1));

	vector<int> linesBefore6 = Next::GetNextTBefore(6);
	CPPUNIT_ASSERT_EQUAL(3, (int) linesBefore6.size());
	sort(linesBefore6.begin(), linesBefore6.end());
	CPPUNIT_ASSERT_EQUAL(1, linesBefore6.at(0));
	CPPUNIT_ASSERT_EQUAL(5, linesBefore6.at(1));
	CPPUNIT_ASSERT_EQUAL(23, linesBefore6.at(2));

	vector<int> linesBefore8 = Next::GetNextTBefore(8);
	CPPUNIT_ASSERT_EQUAL(7, (int) linesBefore8.size());
	sort(linesBefore8.begin(), linesBefore8.end());
	CPPUNIT_ASSERT_EQUAL(1, linesBefore8.at(0));
	CPPUNIT_ASSERT_EQUAL(2, linesBefore8.at(1));
	CPPUNIT_ASSERT_EQUAL(14, linesBefore8.at(5));
	CPPUNIT_ASSERT_EQUAL(23, linesBefore8.at(6));
}