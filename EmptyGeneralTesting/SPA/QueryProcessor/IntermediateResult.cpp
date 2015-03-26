#include "IntermediateResult.h"
#include <iostream>
#include <algorithm>

IntermediateResult::IntermediateResult(void) {}

/*
Initialize the intermediate result table
Takes in a list of pair of (SynonymType , Synonym)	e.g ((ASSIGN,"a1") , ("VARIABLE,"v"), (PROCEDURE "p"))
If synonym type = var/proc, insert into synonymList_STR, everything else goes into synonymList_INT
e.g.
synonymList_INT
[a1] - empty vector 

synonymList_STR	
[v] - empty vector 
[p] - empty vector 
*/
void IntermediateResult::Initialize(const std::vector<Declaration> declarations
{
	for(std::vector<Declaration>::const_iterator it = declarations.begin(); it != declarations.end(); ++it)
	{
		SynonymType type = it->synonym.type;
		std::string value = it->synonym.value;

		if(type == PROCEDURE || type == VARIABLE) {
			synonymList_STR[value] = std::vector<std::string>();
		}
		else
		{
			synonymList_INT[value] = std::vector<int>();
		}
	}

	return;
}

void IntermediateResult::Insert(std::string synonym , int value)
{	
	int index = 0;

	for(it_synonymList_INT it = synonymList_INT.begin(); it != synonymList_INT.end(); ++it)
	{
		if(it->first == synonym) {
			index = it->second.size();
			it->second.push_back(value);
			linkMap[synonym].push_back(Element(synonym,index));
			Print(); 
			return;
		}
	}
	
	std::cout << "In IntermediateResult::InsertElement, " << synonym << " " << value << " not found in synonymList_INT \n";

	return;
}

void IntermediateResult::Insert(std::string synonym , std::string value)
{	
	int index = 0;

	for(it_synonymList_STR it = synonymList_STR.begin(); it != synonymList_STR.end(); ++it)
	{
		if(it->first == synonym) {
			index = it->second.size();
			it->second.push_back(value);
			linkMap[synonym].push_back(Element(synonym,index));
			return;
		}
	}

	std::cout << "In IntermediateResult::InsertElement, " << synonym << " " << value << " not found in synonymList_STR \n";

	return;
}

void IntermediateResult::MakeLink(std::string synonym_1, int index_1, std::string synonym_2, int index_2)
{
	//check if index_2 already exist in linkMap[Synonym_1][index_1][synonym2]
	//if yes do nothing, if no push back, sort it?
	//then check if index_1 already exist in linkMap[Synonym_2][index_2][synonym1], same thing

	//maybe use set for link? cannot contain duplicate and its ordered
	std::vector<int>* link = &linkMap[synonym_1][index_1].links[synonym_2];	//use pointer so that changes will be reflected
	if(std::find(link->begin(),link->end(),index_2) == link->end())  //1 is not linked to 2
	{
		link->push_back(index_2);
		std::sort(link->begin(),link->end());
	}
	std::cout << synonym_1 << " " << index_1 << " " << synonym_2 << " " << index_2;
	link = &linkMap[synonym_2][index_2].links[synonym_1];	//use pointer so that changes will be reflected
	std::cout << "here\n";
	if(std::find(link->begin(),link->end(),index_1) == link->end())  //1 is not linked to 2
	{
		//std::cout << "here2\n";
		link->push_back(index_1);
		std::sort(link->begin(),link->end());
	}
	//std::cout << "here3\n";
}




void IntermediateResult::GetList(std::string synonym, std::vector<int>& list)
{
	for(it_synonymList_INT it = synonymList_INT.begin(); it != synonymList_INT.end(); ++it)
	{
		if(it->first == synonym) {
			list = it->second;
			return;
		}
	}

	std::cout << "In IntermediateResult::GetList, " << synonym << " not found in synonymList_INT\n";

	return;
}

void IntermediateResult::GetList(std::string synonym, std::vector<std::string>& list)
{
	for(it_synonymList_STR it = synonymList_STR.begin(); it != synonymList_STR.end(); ++it)
	{
		if(it->first == synonym) {
			list = it->second;
			return;
		}
	}

	std::cout << "In IntermediateResult::GetList, " << synonym << " not found in synonymList_STR\n";

	return;
}

void IntermediateResult::Print()
{
	for(it_synonymList_INT it = synonymList_INT.begin(); it != synonymList_INT.end(); ++it)
	{
		std::cout << it->first << " : ";
		for(std::vector<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			std::cout << *it2 << " ";
		std::cout << "\n";
	}

	for(it_synonymList_STR it = synonymList_STR.begin(); it != synonymList_STR.end(); ++it)
	{
		std::cout << it->first << " : ";
		for(std::vector<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			std::cout << *it2 << " ";
		std::cout << "\n";
	}

	std::cout << "\n";
	
	
	for(std::map<std::string,std::vector<Element>>::iterator it = linkMap.begin(); it != linkMap.end(); ++it)
	{
		std::cout << it->first << "\n";
		for(std::vector<Element>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			std::cout << (*it2).index << " - ";

			for(std::map<std::string,std::vector<int>>::iterator it3 = (*it2).links.begin(); it3 != (*it2).links.end(); ++it3)
			{
				std::cout << "(" << it3->first << ")";
				for(std::vector<int>::iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4)
					std::cout << *it4 << " ";
				std::cout << "  ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	return;
}