#pragma once

#include "VarTable.h"
#include "..\Exception\IndexNotFoundException.h"
#include <algorithm>
#include <vector>

vector<string>  VarTable::varNames;

/* Public Methods*/
int VarTable::InsertVar(string varName) {
	vector<string>::iterator searchResult = SearchFor(varName);

	if (searchResult != varNames.end()) {
		return distance(varNames.begin(), searchResult);

	} else {
		varNames.push_back(varName);
		return varNames.size() - 1;

	}

}

int VarTable::GetIndexOfVar(string varName) {
	vector<string>::iterator searchResult = SearchFor(varName);

	if (searchResult != varNames.end()) {
		return distance(varNames.begin(), searchResult);

	} else {
		return -1;

	}

}

string VarTable::GetVarName(int varIndex) {
	if (varIndex >= 0 && (unsigned int)varIndex < varNames.size()) {
		return varNames[varIndex];
	} else {
		throw IndexNotFoundException();
	}

}

vector<string> VarTable::GetAllVarNames() {
	return varNames;
}

int VarTable::GetNoOfVars() {
	return varNames.size();

}

// method(s) to aid testing
void VarTable::ClearData() {
	varNames.clear();

}

/*Private Methods*/
vector<string>::iterator VarTable::SearchFor(string varName) {
	return find(varNames.begin(), varNames.end(), varName);

}