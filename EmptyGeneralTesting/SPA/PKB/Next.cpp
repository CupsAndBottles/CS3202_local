#include <iostream>

#include "Next.h"
#include "StmtTypeTable.h"

map <int, vector<int>> Next::beforeToAfterTable;
map <int, vector<int>> Next::afterToBeforeTable;
int Next::sizeOfNext;
int Next::maxNoOfLines;
bool Next::bitVectorIsBuilt;
vector<vector<bool>> Next::bitVector;

Next::Next(void) {
	sizeOfNext = 0;
	maxNoOfLines = 0;
	bitVectorIsBuilt = false;
}

// API
void Next::SetNext(int progLineBefore, int progLineAfter) {
	// check availability
	if (!IsNext(progLineBefore, progLineAfter) &&
		progLineBefore != progLineAfter && 
			NoRelationshipConflicts(progLineBefore, progLineAfter)) {
		beforeToAfterTable[progLineBefore].push_back(progLineAfter);
		afterToBeforeTable[progLineAfter].push_back(progLineBefore);
		
		sizeOfNext++;

		maxNoOfLines = maxNoOfLines > progLineBefore ? maxNoOfLines : progLineBefore;
		maxNoOfLines = maxNoOfLines > progLineAfter ? maxNoOfLines : progLineAfter;

	}

}
void Next::CreateBitVector() {
	// this method transfers the r'nships in tables to bitvectors	
	std::vector <bool> a (maxNoOfLines, false);

	for (int i=0;i<maxNoOfLines;i++) {
		bitVector.push_back(a);
	}
	typedef map<int, vector<int>>::iterator map_it;

	for (map_it nextIt = beforeToAfterTable.begin(); nextIt != beforeToAfterTable.end(); nextIt++) {
		if (!nextIt->second.empty()) {
			int size2 = nextIt->second.size();
			for (int j = 0; j < size2; j++) {
				int x = nextIt->second.at(j);
				bitVector[nextIt->first][x]=true;
				bitVector[x][nextIt->first]=true;
			}
		}
	}
}

bool Next::IsNext(int progLineBefore, int progLineAfter) {
	// checking for Next(progLineBefore, progLineAfter)
	if (beforeToAfterTable.count(progLineBefore) != 0) {
		vector<int> childrenOfBefore = beforeToAfterTable[progLineBefore];
		if (childrenOfBefore.size() == 1) {
			return childrenOfBefore.at(0) == progLineAfter;
		} else {	// assuming size is always 2
			return childrenOfBefore.at(0) == progLineAfter ||
				childrenOfBefore.at(1) == progLineAfter;
		}
	}
	return false;
	
}
bool Next::IsNextBV(int progLineBefore, int progLineAfter) {
	if ((progLineBefore >= 0 && progLineBefore <= maxNoOfLines) && (progLineAfter >= 0 && progLineAfter <= maxNoOfLines)) 
	
		return bitVector[progLineBefore][progLineAfter];
	else return false;
}
vector<int> Next::GetNextAfter(int progLineBefore) {
	if (beforeToAfterTable.count(progLineBefore) == 0) {
		vector<int> emptyVector;
		return emptyVector;

	} else {
		return beforeToAfterTable.at(progLineBefore);
	}

}

vector<int> Next::GetNextBefore(int progLineAfter) {
	if (afterToBeforeTable.count(progLineAfter) == 0) {
		vector<int> emptyVector;
		return emptyVector;

	} else {
		return afterToBeforeTable.at(progLineAfter);
	}

}

bool Next::IsNextT(int progLineBefore, int progLineAfter) {

	queue<int> linesToCheck;
		
	int maxNoOfLinesSoFar = maxNoOfLines > StmtTypeTable::GetNoOfStmts() ? maxNoOfLines : StmtTypeTable::GetNoOfStmts();
	vector<bool> checkedLines (maxNoOfLinesSoFar + 1, false);
		
	int currLine;

	linesToCheck.push(progLineBefore);

	while (!linesToCheck.empty()) {
		currLine = linesToCheck.front();

		if (!checkedLines.at(currLine)) {
			if (IsNext(currLine, progLineAfter)) {
				return true;
			}
			checkedLines[currLine] = true;
			linesToCheck = AddToQueue(linesToCheck, GetNextAfter(currLine));
		}
		linesToCheck.pop();
	}
	return false;
}

vector<int> Next::GetNextTAfter(int progLineBefore) {
	queue<int> linesToCheck;
	vector<int> linesAfter;
	
	int maxNoOfLinesSoFar = maxNoOfLines > StmtTypeTable::GetNoOfStmts() ? maxNoOfLines : StmtTypeTable::GetNoOfStmts();
	vector<bool> checkedLines (maxNoOfLinesSoFar + 1, false);
	
	int currLine;

	linesToCheck = AddToQueue(linesToCheck, GetNextAfter(progLineBefore));
	
	while (!linesToCheck.empty()){
		currLine = linesToCheck.front();
		if (!checkedLines.at(currLine)) {
			linesAfter.push_back(currLine);
			linesToCheck = AddToQueue(linesToCheck, GetNextAfter(currLine));
			
			checkedLines[currLine] = true;

		}
		linesToCheck.pop();

	}

	return linesAfter;

}

vector<int> Next::GetNextTBefore(int progLineAfter) {
	queue<int> linesToCheck;
	vector<int> linesBefore;
	
	int maxNoOfLinesSoFar = maxNoOfLines > StmtTypeTable::GetNoOfStmts() ? maxNoOfLines : StmtTypeTable::GetNoOfStmts();
	vector<bool> checkedLines (maxNoOfLinesSoFar + 1, false);
	
	int currLine;

	linesToCheck = AddToQueue(linesToCheck, GetNextBefore(progLineAfter));
	
	while (!linesToCheck.empty()){
		currLine = linesToCheck.front();
		if (!checkedLines.at(currLine)) {
			linesBefore.push_back(currLine);
			linesToCheck = AddToQueue(linesToCheck, GetNextBefore(currLine));

			checkedLines[currLine] = true;
		}
		linesToCheck.pop();
	}
	return linesBefore;

}

queue<int> Next::AddToQueue(queue<int> linesToCheck, vector<int> additions) {
	for (unsigned int i = 0; i < additions.size(); i++) {
		linesToCheck.push(additions.at(i));
	}

	return linesToCheck;

}

bool Next::HasAnyNext() {
	return sizeOfNext > 0;
}

// helper methods for testing
int Next::SizeOfNext() {
	return sizeOfNext;
}

void Next::ClearData() {
	beforeToAfterTable.clear();
	afterToBeforeTable.clear();
	sizeOfNext = 0;
	bitVector.clear();
	bitVectorIsBuilt = false;
}

// private methods
bool Next::NoRelationshipConflicts(int progLineBefore, int progLineAfter) {
	// checking for relationship conflicts: a node can only have max 2 children but unlimited number of parents
	return BeforeNodeNotFull(progLineBefore, progLineAfter) 
		&& AfterNodeIsValid(progLineBefore, progLineAfter);

}

// each node can only have max 2 children, but unlimited number of parents
bool Next::BeforeNodeNotFull(int progLineBefore, int progLineAfter) {
	if (beforeToAfterTable.count(progLineBefore) != 0) {
		if (beforeToAfterTable[progLineBefore].size() >= 2) {
			return false;
		}

	}

	return true;
}

bool Next::AfterNodeIsValid(int progLineBefore, int progLineAfter) {
	// progLineAfter already has a parent
	if (afterToBeforeTable.count(progLineAfter) != 0) {
		// progLineAfter shares the same immediate parent as progLineBefore
		if (afterToBeforeTable.count(progLineBefore) != 0) {
			vector <int> parentsOfBefore = afterToBeforeTable.at(progLineBefore);
			for (unsigned int i = 0; i < parentsOfBefore.size(); i ++) {
				vector <int> bastardSiblingsOfBefore = beforeToAfterTable.at(parentsOfBefore.at(i));
				if (bastardSiblingsOfBefore.size() > 1) {
					if (bastardSiblingsOfBefore.at(0) == progLineAfter ||
						bastardSiblingsOfBefore.at(1) == progLineAfter)
						return false;
				}
			}

		}

	}

	return true;
}