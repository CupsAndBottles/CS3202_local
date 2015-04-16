#pragma once
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include "Grammar.h"
#include <stdio.h>

using namespace std;


typedef map<string,vector<string>> links_map;


class IntermediateResult
{
public:
	IntermediateResult(void);

private:

	struct Element {
		string synonym;
		string value;
		map<string,vector<string>> links;	//synonym and and list of link 

		Element() : synonym() , value() , links() {};
		Element(string s , string val) : synonym(s) , value(val) , links() {};
	};

	typedef map<string,vector<Element>>::iterator it_synonymList;
	typedef map<string,vector<string>>::iterator links_iterator;
	typedef map<string,vector<Element>>::iterator synonymList_iterator;

	map<string,vector<Element>> synonymList;				
	std::vector<Synonym> synonyms;	//just a list of all declared synonyms

	string ITOS(int num);
	vector<string> ITOS(vector<int> intList);
	int STOI(string s);
	vector<int> STOI(vector<string> strList);

	links_map GetLinks(string synonym, string value); //convenient method to get links of a value
	int GetElementIndex(string synonym, string value);

public:
	void Initialize(vector<Declaration> declarations);	

	void Insert(string synonym , int value);
	void Insert(string synonym , string value);
	
	void InsertPair(string synonym_1 , int value_1, string synonym_2 , int value_2);
	void InsertPair(string synonym_1 , int value_1, string synonym_2 , string value_2);
	void InsertPair(string synonym_1 , string value_1, string synonym_2 , int value_2);
	void InsertPair(string synonym_1 , string value_1, string synonym_2 , string value_2);

	void InsertList(string synonym , vector<int> list_int);
	void InsertList(string synonym , vector<string> list);
	
	void MakeLink(string synonym_1, int value_1_int, string synonym_2, int value_2_int);
	void MakeLink(string synonym_1, int value_1_int, string synonym_2, string value_2);
	void MakeLink(string synonym_1, string value_1, string synonym_2, int value_2_int);
	void MakeLink(string synonym_1, string value_1, string synonym_2, string value_2);	

	void Unlink(string synonym_1, int value_1_int, string synonym_2, int value_2_int);
	void Unlink(string synonym_1, int value_1_int, string synonym_2, string value_2);
	void Unlink(string synonym_1, string value_1, string synonym_2, int value_2_int);
	void Unlink(string synonym_1, string value_1, string synonym_2, string value_2);	

	bool HasLinkBetweenColumns(string synonym_1, int value_1_int, string synonym_2, int value_2_int, bool& IsDirectLink , string skipSynonym = "");
	bool HasLinkBetweenColumns(string synonym_1, int value_1_int, string synonym_2, string value_2, bool& IsDirectLink , string skipSynonym = "");
	bool HasLinkBetweenColumns(string synonym_1, string value_1, string synonym_2, int value_2_int, bool& IsDirectLink , string skipSynonym = "");
	bool HasLinkBetweenColumns(string synonym_1, string value_1, string synonym_2, string value_2, bool& IsDirectLink , string skipSynonym = "");
	
	bool HasLinkToColumn(string synonym_1, int value_1_int, string synonym_2);
	bool HasLinkToColumn(string synonym_1, string value_1, string synonym_2);  //check whether value1 has link to synonym2

	bool HasLink(string synonym_1, int value_1_int);
	bool HasLink(string synonym_1, string value_1);


	void Remove(string synonym , string value);			
	void Remove(string synonym , int value);
	
	void GetList(string synonym, vector<int>& list);			//get intermediate result of a synonym
	void GetList(string synonym, vector<string>& list);			//get intermediate result of a synonym
	
	void RemoveElementsWithoutLink(string synonym_1, string synonym_2);		//remove any element that doesnt have link between this 2 synonyms

	void GetResultSingle(string synonym , list<string>& result);				//get final result for select a
	void GetResultTuple(vector<string> synonym , list<string>& result);			//get final result for select <a,w,v>

	bool Exist(string synonym , string value);				//check if a value of synonym exist in the list
	bool IsListEmpty(Synonym synonym);						//check is a particular synonym column is empty
	std::vector<Synonym> GetAllSynonyms();					//get all synonym
	SynonymType GetSynonymType(std::string val);			//get SynonymType of a synonym
	void UpdateTable(string synonym_1, string synonym_2);	//remove any element without link between this 2 column, used after evaluate modifies(a,v) where a and v both have elements
	void Print();
};