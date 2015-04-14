#include "AffectsBip.h"
#include "ProcTable.h"
#include "StmtTypeTable.h"
#include "NextBip.h"
#include "Modifies.h"
#include "Uses.h"
#include "Follows.h"
#include "..\Program\Program.h"

AffectsBip::AffectsBip() {
}

bool AffectsBip::IsAffectsBip(int stmtAffecting, int stmtAffected) {
	if (1 <= stmtAffecting && stmtAffecting <= StmtTypeTable::GetMaxStmtIndex() 
		&& 1 <= stmtAffected && stmtAffected <= StmtTypeTable::GetMaxStmtIndex()) {

		if (StmtTypeTable::CheckIfStmtOfType(stmtAffecting, ASSIGN) 
			&& StmtTypeTable::CheckIfStmtOfType(stmtAffected, ASSIGN)) {

			vector<int> modifiedVars = Modifies::GetVarModifiedByStmt(stmtAffecting);
			if (!modifiedVars.empty()) {
				int varModified = modifiedVars.at(0);
				if (Uses::IsStmtUsingVar(stmtAffected, varModified)) {
					return CheckBipCFG(stmtAffecting, stmtAffected, varModified);
				}
			}

		}

	}

	return false;

}

vector<int> AffectsBip::GetStmtsAffectedBipBy(int stmtAffecting) {
	if(1 <= stmtAffecting && stmtAffecting <= StmtTypeTable::GetNoOfStmts()) {
		if (StmtTypeTable::CheckIfStmtOfType(stmtAffecting, ASSIGN)) {
			int varModified = Modifies::GetVarModifiedByStmt(stmtAffecting).at(0);
			return TraverseDownBipCFG(stmtAffecting, varModified);
		}	
	}

	vector<int> emptyVector;
	return emptyVector;

}

vector<int> AffectsBip::GetStmtsAffectingBip(int stmtAffected) {
	if(1 <= stmtAffected && stmtAffected <= StmtTypeTable::GetNoOfStmts()) {
		if (StmtTypeTable::CheckIfStmtOfType(stmtAffected, ASSIGN)) {
			vector<int> varsUsed = Uses::GetVarUsedByStmt(stmtAffected);
			return TraverseUpBipCFG(stmtAffected, varsUsed);
		}	
	}

	vector<int> emptyVector;
	return emptyVector;

}

bool AffectsBip::CheckBipCFG(int stmtAffecting, int stmtAffected, int varModified) {
	queue<int> uncheckedStmts;
	vector<bool> stmtsIsChecked(StmtTypeTable::GetNoOfStmts() + 1, false);
	
	vector<int> nextOfCurrStmt = NextBip::GetNextAfter(stmtAffecting);
	uncheckedStmts.push(nextOfCurrStmt.at(0));

	int currStmt;
	
	while (!uncheckedStmts.empty()) {
		currStmt = uncheckedStmts.front();

		if (!stmtsIsChecked.at(currStmt)) {
			switch (StmtTypeTable::GetStmtTypeOf(currStmt)) {
			case ASSIGN:
				if (currStmt == stmtAffected) {
					return true;
				}
				
				if (Modifies::IsStmtModifyingVar(currStmt, varModified)) {
					// don't check for anything after this stmt
				} else {
					nextOfCurrStmt = NextBip::GetNextAfter(currStmt);
					uncheckedStmts.push(nextOfCurrStmt.at(0));
				}	
				break;
			case WHILE:
				nextOfCurrStmt = NextBip::GetNextAfter(currStmt);
				if ((int) nextOfCurrStmt.size() > 1 && stmtAffected >= nextOfCurrStmt.at(1)) {
					uncheckedStmts.push(nextOfCurrStmt.at(1));
				} else if ((int) nextOfCurrStmt.size() > 1 && !Modifies::IsStmtModifyingVar(currStmt, varModified)) {
					return true;
				} else {
					uncheckedStmts.push(nextOfCurrStmt.at(0));
				}
				break;
			case IF: {
				nextOfCurrStmt = NextBip::GetNextAfter(currStmt);
				int endOfIfElse = Follows::GetFollowsAfter(currStmt);
				
				if (stmtAffected < nextOfCurrStmt.at(1)) {
					// modification in 1st block
					if (!Modifies::IsStmtModifyingVar(currStmt, varModified)) {
						return true;
					} else {
						uncheckedStmts.push(nextOfCurrStmt.at(0));	
					}
				} else if (endOfIfElse > 0 && stmtAffected >= nextOfCurrStmt.at(1) && stmtAffected < endOfIfElse) {
					// modification in 2nd block
					if (!Modifies::IsStmtModifyingVar(currStmt, varModified)) {
						return true;
					} else {
						uncheckedStmts.push(nextOfCurrStmt.at(1));	
					}
				} else if (endOfIfElse > 0 && stmtAffected >= endOfIfElse && !Modifies::IsStmtModifyingVar(currStmt, varModified)) {
					// if-else does not modify at all, skip all if-else
					uncheckedStmts.push(Follows::GetFollowsAfter(currStmt));
				} else {
					uncheckedStmts.push(nextOfCurrStmt.at(0));
					uncheckedStmts.push(nextOfCurrStmt.at(1));
				}
			} break;
			case CALL: {
				int procCalled = ProcTable::GetIndexOfProc(Program::GetStmtFromNumber(currStmt).GetContent());
				if (Modifies::IsProcModifyingVar(procCalled, varModified)) {
					// don't check for anything after this stmt
				} else {
					nextOfCurrStmt = NextBip::GetNextAfter(currStmt);
					uncheckedStmts.push(nextOfCurrStmt.at(0));
				}
			} break;
			default:
				return false;
			}

			stmtsIsChecked.at(currStmt) = true;
		}
		
		uncheckedStmts.pop();

	}

	return false;

}

vector<int> AffectsBip::TraverseDownBipCFG(int stmtAffecting, int varModified) {
	queue<int> uncheckedStmts;
	vector<bool> stmtsIsChecked(StmtTypeTable::GetNoOfStmts() + 1, false);
	
	vector<int> nextOfCurrStmt = NextBip::GetNextAfter(stmtAffecting);
	uncheckedStmts.push(nextOfCurrStmt.at(0));

	int currStmt;

	vector<int> affectedStmts;

	while (!uncheckedStmts.empty()) {
		currStmt = uncheckedStmts.front();
		//cout << "\n currStmt: " << currStmt << "\n";
		if (!stmtsIsChecked.at(currStmt)) {
			switch (StmtTypeTable::GetStmtTypeOf(currStmt)) {
			case ASSIGN:
				if (Uses::IsStmtUsingVar(currStmt, varModified)) {
					affectedStmts.push_back(currStmt);
				}
				
				if (Modifies::IsStmtModifyingVar(currStmt, varModified)) {
					// don't check for anything after this stmt
				} else {
					nextOfCurrStmt = NextBip::GetNextAfter(currStmt);
					if (!nextOfCurrStmt.empty()) {
						uncheckedStmts.push(nextOfCurrStmt.at(0));
					}
				}
				break;
			case WHILE:
				nextOfCurrStmt = NextBip::GetNextAfter(currStmt);
				if (Uses::IsStmtUsingVar(currStmt, varModified)) {
					// find a way to check that the used variable occurs in the stmtLst contained by while, 
					// not used as a control variable in while
					uncheckedStmts.push(nextOfCurrStmt.at(0));
				}
				if ((int) nextOfCurrStmt.size() > 1) {
					uncheckedStmts.push(nextOfCurrStmt.at(1));
				}
				break;
			case IF:
				nextOfCurrStmt = NextBip::GetNextAfter(currStmt);
				
				if (!Modifies::IsStmtModifyingVar(currStmt, varModified) && !Uses::IsStmtUsingVar(currStmt, varModified)) {
					uncheckedStmts.push(Follows::GetFollowsAfter(currStmt));
				} else {
					uncheckedStmts.push(nextOfCurrStmt.at(0));
					uncheckedStmts.push(nextOfCurrStmt.at(1));
				}
				break;
			case CALL: {
				int procCalled = ProcTable::GetIndexOfProc(Program::GetStmtFromNumber(currStmt).GetContent());
				if (Uses::IsProcUsingVar(procCalled, varModified)) {
					affectedStmts.push_back(currStmt);
				}
				
				if (Modifies::IsProcModifyingVar(procCalled, varModified)) {
					// don't check for anything after this stmt
				} else {
					nextOfCurrStmt = NextBip::GetNextAfter(currStmt);
					if (!nextOfCurrStmt.empty()) {
						uncheckedStmts.push(nextOfCurrStmt.at(0));
					}
				}
			} break;
			default:
				//1. "longer method, without using bitvector"
				//2. do bit vector optimisaion of while and if
				break;
			}

			stmtsIsChecked.at(currStmt) = true;
		}
		
		uncheckedStmts.pop();

	}

	return affectedStmts;

}

vector<int> AffectsBip::TraverseUpBipCFG(int stmtAffected, vector<int> varsUsed) {
	vector<bool> stmtsIsChecked(StmtTypeTable::GetNoOfStmts() + 1, false);
	vector<int> nextBeforeCurrStmt = NextBip::GetNextBefore(stmtAffected);
	vector<int> affectedStmts;
	
	if (!varsUsed.empty()) {
		for (vector<int>::iterator it = nextBeforeCurrStmt.begin(); it != nextBeforeCurrStmt.end(); it++) {
			pair<vector<int>, vector<bool>> subAns = RecurTraverseUpBipCFG(*it, varsUsed, stmtsIsChecked);
			vector<int> subAffectedStmts = subAns.first;
			affectedStmts = OrOperationOnVects(affectedStmts, subAffectedStmts);
			vector<bool> subStmtsIsChecked = subAns.second;
			subStmtsIsChecked = OrOperationOnBoolVects(subStmtsIsChecked, stmtsIsChecked);
		}
	
	}

	return affectedStmts;
}

pair<vector<int>, vector<bool>> AffectsBip::RecurTraverseUpBipCFG(int currStmt, vector<int> varsUsed, vector<bool> stmtsIsChecked) {
	vector<int> affectedStmts;
	vector<int> nextBeforeCurrStmt = NextBip::GetNextBefore(currStmt);

	//cout << "\n currStmt: " << currStmt << "\n";

	switch(StmtTypeTable::GetStmtTypeOf(currStmt)) {
	case ASSIGN: {
		if (!stmtsIsChecked.at(currStmt)) {
			int varModified = Modifies::GetVarModifiedByStmt(currStmt).at(0);
			vector<int>::iterator it = varsUsed.begin();
			while (it != varsUsed.end()) {
				if (varModified == *it) {
					affectedStmts.push_back(currStmt);
					it = varsUsed.erase(it);
				} else {
					it++;
				}
			}
			stmtsIsChecked.at(currStmt) = true;
		} else {
			nextBeforeCurrStmt.clear();
		}
	} break;
	case WHILE: {
		vector<int>::iterator it = nextBeforeCurrStmt.begin();
		if (!stmtsIsChecked.at(currStmt)) {
			while (it != nextBeforeCurrStmt.end()) {
				if (*it > currStmt) {
					nextBeforeCurrStmt.erase(it, nextBeforeCurrStmt.end());
					it = nextBeforeCurrStmt.end();
				} else {
					it++;
				}
			}
			stmtsIsChecked.at(currStmt) = true;
		} else {
			nextBeforeCurrStmt.clear();
		}

	} break;
	case IF: {
		vector<int> nextAfterIf = NextBip::GetNextAfter(currStmt);
		if ((int) nextAfterIf.size() == 2) {
			if (stmtsIsChecked.at(nextAfterIf.at(0)) && stmtsIsChecked.at(nextAfterIf.at(1))) {
				stmtsIsChecked.at(currStmt) = true;
			} 
		}
	}
	case CALL:{
		int procCalled = ProcTable::GetIndexOfProc(Program::GetStmtFromNumber(currStmt).GetContent());
		if (!stmtsIsChecked.at(currStmt)) {
			int varModified = Modifies::GetVarModifiedByProc(procCalled).at(0);
			vector<int>::iterator it = varsUsed.begin();
			while (it != varsUsed.end()) {
				if (varModified == *it) {
					affectedStmts.push_back(currStmt);
					it = varsUsed.erase(it);
				} else {
					it++;
				}
			}
			stmtsIsChecked.at(currStmt) = true;
		} else {
			nextBeforeCurrStmt.clear();
		}
	} break;
	default:
		cout << "unable to determine stmttype of stmt# " << currStmt;
		break;
	}

	if (!varsUsed.empty()) {
		for (vector<int>::iterator it = nextBeforeCurrStmt.begin(); it != nextBeforeCurrStmt.end(); it++) {
			pair<vector<int>, vector<bool>> subAns = RecurTraverseUpBipCFG(*it, varsUsed, stmtsIsChecked);
			vector<int> subAffectedStmts = subAns.first;
			affectedStmts = OrOperationOnVects(affectedStmts, subAffectedStmts);
			vector<bool> subStmtsIsChecked = subAns.second;
			stmtsIsChecked = OrOperationOnBoolVects(subStmtsIsChecked, stmtsIsChecked);
		}
	
	}

	return make_pair(affectedStmts, stmtsIsChecked);
}

vector<int> AffectsBip::OrOperationOnVects(vector<int> vect1, vector<int> vect2) {
	vector<int> combinedVect;

	sort(vect1.begin(), vect1.end());
	sort(vect2.begin(), vect2.end());
	vector<int>::iterator it1 = vect1.begin(), it2 = vect2.begin();

	while (it1 != vect1.end() || it2 != vect2.end()) {
		if (it1 == vect1.end()) {
			combinedVect.insert(combinedVect.end(), it2, vect2.end());
			return combinedVect;
		} else if (it2 == vect2.end()) {
			combinedVect.insert(combinedVect.end(), it1, vect1.end());
			return combinedVect;
		} else if (*it1 == *it2) {
			combinedVect.push_back(*it1);
			it1++;
			it2++;
		} else if (*it1 > *it2) {
			combinedVect.push_back(*it2);
			it2++;
		} else {
			combinedVect.push_back(*it1);
			it1++;
		}
	
	}

	return combinedVect;
}

vector<bool> AffectsBip::OrOperationOnBoolVects(vector<bool> boolVect1, vector<bool> boolVect2) { 
	if (boolVect1.size() == boolVect2.size()) {
		vector<bool>::iterator it1 = boolVect1.begin(), it2 = boolVect2.begin();
		vector<bool> boolVect3(boolVect1.size() > boolVect2.size() ? boolVect1.size() : boolVect2.size());
		for (vector<bool>::iterator it3 = boolVect3.begin(); it3 != boolVect3.end(); it3++) {
			if (*it1 || *it2) {
				*it3 = true;
			}
			it1++;
			it2++;
		}
		return boolVect3;
	} else {
		cout << "problem: bool vectors different sizes";
		return boolVect2;
	}
}
