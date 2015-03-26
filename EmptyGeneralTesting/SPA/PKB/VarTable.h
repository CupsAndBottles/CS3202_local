#pragma once

#include<string>
#include<vector>

using namespace std;


/** To store a mapping of all variables
	and their indexes. **/

class VarTable {
public:
	// Default constructor
	VarTable();

	// Methods
	static int InsertVar(string varName);
	static int GetIndexOfVar(string varName); 
	static string GetVarName(int varIndex);
	static vector<string> GetAllVarNames();
	static int GetNoOfVars();
	
	// method(s) to aid testing
	static void ClearData();

private:
	static vector<string>  varNames;	// List of variable strings
	// insert subsequent columns of the VarTable here

	static vector<string>::iterator SearchFor(string varName);

};