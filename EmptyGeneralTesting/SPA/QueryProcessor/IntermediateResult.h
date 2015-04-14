#pragma once
#include <map>
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
	void MakeLink(string synonym_1, string value_1, string synonym_2, string value_2);	
	void GetList(string synonym, vector<int>& list);
	void GetList(string synonym, vector<string>& list);
	bool Exist(string synonym , string value); //check if a value of synonym exist in the list
	bool IsListEmpty(Synonym synonym);			//check is a particular synonym column is empty
	std::vector<Synonym> GetAllSynonyms();		//get all synonym columns
	SynonymType GetSynonymType(std::string val);	//get SynonymType of a synonym
	void Print();

	void Remove(string synonym , string value);
	void Remove(string synonym , int value);
	
	void UpdateTable(string synonym_1, string synonym_2); //remove any element without link between this 2 column, used after evaluate modifies(a,v) where a and v both have elements
	
	
	
	
	
	/*
	template<typename T , typename U> bool HasLink(std::string synonym_1 , T value_1, std::string synonym_2 , U value_2)
	{	
		//only need to check for one direction
		int index_1 = INT_MAX, index_2 = INT_MAX;
		bool exist_1 = false, exist_2 = false;

		GetSynonymListIndex(synonym_1 , value_1 , exist_1 , index_1 , false);
		GetSynonymListIndex(synonym_2 , value_2 , exist_2 , index_2 , false);

		//if either one doesnt exist
		if(!exist_1 || !exist_2)
		{
			std::cout << "In HasLink, something wrong\n";
			return false;
		}

		std::vector<int> links = linkMap[synonym_1][index_1].links[synonym_2];

		if(std::find(links.begin(),links.end(),index_2) != links.end())
			return true;

		return false;
	}


	template<typename T , typename U> void RemovePair(std::string synonym_1 , T value_1, std::string synonym_2 , U value_2)
	{
		int index_1 = INT_MAX, index_2 = INT_MAX;
		bool exist_1 = false, exist_2 = false;

		//No need check exist, remove already check

		Remove(synonym_1 , value_1);
		Remove(synonym_2 , value_2);
	}
	*/
};