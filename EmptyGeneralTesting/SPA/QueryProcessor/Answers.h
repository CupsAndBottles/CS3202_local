#pragma once
#include "QueryData.h"
#include "PatternMatcher.h"
#include "Grammar.h"
#include <vector>

using namespace std;

class Answers{
private:
	int indexNumber;
	vector<vector<Answers*>> links;
	

public:
	bool useful; //used when delete elements in a list (Biuld a new list)
	int listIndex; //Which list is this synonym in.
	Answers(int value,int index):indexNumber(value), listIndex(index){};

	bool hasLinksWith(int index);	

	bool operator==(Answers answer2);

	vector<Answers*> cleanLinks();
	//Delete all the links with other answers. Then, we can delete this answer. 
	//Return the list of answers that has no links with others (need to be delete) after this operation.

	vector<Answers*> getLinksFromList(int listIndex);
	static void addLink(Answers answer1, Answers answer2);
};
