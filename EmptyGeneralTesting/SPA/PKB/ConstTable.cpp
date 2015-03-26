#include <utility>
#include <map>
#include <vector>
#include <algorithm>
#include "ConstTable.h"
#include "Parent.h"

using namespace std;

map <int, vector<int> > ConstTable::stmtToConstTable;
map <int, vector<int> > ConstTable::constToStmtTable;

// empty constructor
ConstTable::ConstTable() {}

// API
void ConstTable::SetStmtUsesConst(int stmtUsing, int constUsed) {
    if (!IsStmtUsingConst(stmtUsing, constUsed)) {
        stmtToConstTable[stmtUsing].push_back(constUsed);
        constToStmtTable[constUsed].push_back(stmtUsing);
    }

    if (Parent::GetParentOf(stmtUsing) != -1)
        SetStmtUsesConst(Parent::GetParentOf(stmtUsing), constUsed);
}

bool ConstTable::IsStmtUsingConst(int stmtUsing, int constUsed) {
    if (stmtToConstTable.count(stmtUsing)!=0)
		for (vector<int>::iterator it = stmtToConstTable.at(stmtUsing).begin(); it != stmtToConstTable.at(stmtUsing).end(); it++)
            if (*it == constUsed)
                return true;
    return false;
}

vector<int> ConstTable::GetStmtUsingConst(int constUsed) {
    vector<int> ret;
    if (constToStmtTable.count(constUsed)==0)
        return ret;
    else return constToStmtTable.at(constUsed);
}

vector<int> ConstTable::GetConstUsedByStmt(int stmtUsing) {
    vector<int> ret;
    if (stmtToConstTable.count(stmtUsing)==0)
        return ret;
    else return stmtToConstTable.at(stmtUsing);
}


bool ConstTable::HasAnyConst() {
    return !stmtToConstTable.empty();

}

vector<int> ConstTable::GetAllConst() {
    vector<int> listOfConstants;

    for(map<int, vector<int> >::iterator it = stmtToConstTable.begin(); it != stmtToConstTable.end(); it++) {
        for(unsigned int i = 0; i < it->second.size(); i++) {
            listOfConstants.push_back(it->second.at(i));
		}
	}

	sort(listOfConstants.begin(), listOfConstants.end());
	
	vector<int>::iterator it;
	it = unique (listOfConstants.begin(), listOfConstants.end()); 
	listOfConstants.resize(distance(listOfConstants.begin(),it) ); // trims excess spaces in vector

    return listOfConstants;
}

int ConstTable::SizeOfConstTable() {
    int sum = 0;

    for(map<int, vector<int> >::iterator it=stmtToConstTable.begin(); it!=stmtToConstTable.end();        it++)
    sum += it->second.size();
    
	return sum;
}

void ConstTable::ClearData() {
    stmtToConstTable.clear();
    constToStmtTable.clear();
}
