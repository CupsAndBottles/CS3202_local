#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include "Grammar.h"
#include <stdio.h>

typedef std::map<std::string,std::vector<int>>::iterator it_synonymList_INT;
typedef std::map<std::string,std::vector<std::string>>::iterator it_synonymList_STR;

class IntermediateResult
{
private:

	struct Element {
		std::string synonym;
		int index;
		std::map<std::string,std::vector<int>> links;	//synonym and and list of link INDEX

		Element() : synonym() , index() , links() {};
		Element(std::string s , int i) : synonym(s) , index(i) , links() {};
	};

	std::map<std::string,std::vector<Element>> linkMap ;	//synonym and its element and links
	std::map<std::string,std::vector<int>> synonymList_INT;					//map for for a,s,w,if,n and corresponding elements
	std::map<std::string,std::vector<std::string>> synonymList_STR;			//map for p,v and corresponding elements
	std::vector<Synonym> synonyms;	//just a list of all declared synonyms

public:

	IntermediateResult(void);
	void Initialize(const std::vector<Declaration> declarations);	//initialize with list of pair(synonymtype, synonym)
	void Insert(std::string synonym , int value);
	void Insert(std::string synonym , std::string value);
	//void Remove(std::string synonym , std::string value);
	void MakeLink(std::string synonym_1, int index_1, std::string synonym_2, int index_2);	//use index to avoid overload function
	void GetList(std::string synonym, std::vector<int>& list);
	void GetList(std::string synonym, std::vector<std::string>& list);
	bool IsListEmpty(Synonym synonym);
	std::vector<Synonym> GetAllSynonyms();
	void Print();

	
	template<typename T , typename U> void InsertPair(std::string synonym_1 , T value_1, std::string synonym_2 , U value_2)
	{
		int index_1 = INT_MAX, index_2 = INT_MAX;
		bool exist_1 = false, exist_2 = false;

		GetSynonymListIndex(synonym_1 , value_1 , exist_1 , index_1 , true);
		GetSynonymListIndex(synonym_2 , value_2 , exist_2 , index_2 , true);

		MakeLink(synonym_1, index_1, synonym_2, index_2);
	
		Print();

		return;
	}

	template<typename T> void GetSynonymListIndex(std::string synonym , T value , bool &exist , int &index , bool wantToInsert)
	{}
	
	//Get index of an element in synonym list, so you can use it with linkMap
	template<> void GetSynonymListIndex<int>(std::string synonym , int value , bool &exist , int &index, bool wantToInsert)
	{
		std::cout << "GetSynonymListIndex<int>, " << synonym << " " << value << "\n";
		it_synonymList_INT it = synonymList_INT.find(synonym);
		if(it != synonymList_INT.end())	//synonym found
		{
			std::vector<int>::iterator it2 = std::find(it->second.begin(),it->second.end() , value);
			if(it2 != it->second.end())	//value found
			{
				exist = true;
				index = std::distance(it->second.begin() , it2);
				std::cout << "found " << *it2 << " at index " << index << "\n";
			}

			else //if not found insert it
			{
				if(wantToInsert)
				{
					index = it->second.size();
					it->second.push_back(value);
					linkMap[synonym].push_back(Element(synonym,index));
					std::cout << "insert " << value << " at index " << linkMap[synonym][index].index << "\n";
				}
			}
		}

		else std::cout << "something wrong\n";

		return;
	}

	template<> void GetSynonymListIndex<const char*>(std::string synonym , const char* value , bool &exist , int &index , bool wantToInsert)
	{
		std::cout << "GetSynonymListIndex<str>, " << synonym << " " << value << "\n";
		it_synonymList_STR it = synonymList_STR.find(synonym);

		if(it != synonymList_STR.end())	//synonym found
		{
			std::vector<std::string>::iterator it2 = std::find(it->second.begin(),it->second.end() , value);
			if(it2 != it->second.end())	//value found
			{
				exist = true;
				index = std::distance(it->second.begin() , it2);
			}

			else //if not found insert it?
			{
				if(wantToInsert)
				{
					index = it->second.size();
					it->second.push_back(value);
					linkMap[synonym].push_back(Element(synonym,index));
				}
			}
		}
		return;
	}

	template<typename T> void Remove(std::string synonym , T value) {}
	
	template<> void Remove<int>(std::string synonym_deletedItem , int value)
	{
		//get the index from synonymList_INT
		//get all the linked element from linkMap[syn][index].links
		//for each linked element,
		//	remove the index from its link, and for all link above the index, minus 1
		//	check if the LE has any other link, if no, just delete it
		//	if still has link, check it has any link to the column
		//	if no, call Remove() again, otherwise continue 

		std::cout << "Removing " << synonym_deletedItem << " " << value << "\n";

		bool exist = false;
		int index_deletedItem = INT_MAX;
		GetSynonymListIndex(synonym_deletedItem , value , exist , index_deletedItem, false);

		//if the item to be deleted does not exist, do nothing
		if(!exist) {
			std::cout << "Something wrong in Remove";
			return;
		}

		//get the all the elements that are linked to the deleted element 
		std::map<std::string,std::vector<int>> link_DeletedElement = linkMap[synonym_deletedItem][index_deletedItem].links;

		/********************Debug Message********************/
		std::cout << "Need to delete all these links: ";

		if(link_DeletedElement.empty())
			std::cout << "-\n";

		else{
			for(std::map<std::string,std::vector<int>>::iterator it_map = link_DeletedElement.begin(); it_map != link_DeletedElement.end(); ++it_map) 
			{
				std::cout << "(" << it_map->first << ") ";
				for(std::vector<int>::iterator it_vec = it_map->second.begin(); it_vec != it_map->second.end(); ++it_vec)
				{
					std::cout << it_map->first << " ";
				}
				std::cout << "\n";
			}
			std::cout << "\n";
		}
		/****************************************************/

		//for each linked element, remove index for the deleted item, and update index of other element
		for(std::map<std::string,std::vector<int>>::iterator it_map = link_DeletedElement.begin(); it_map != link_DeletedElement.end(); ++it_map)
		{
			std::string synonym_linkedElement = it_map->first;
			for(std::vector<int>::iterator it_vec = it_map->second.begin(); it_vec != it_map->second.end(); ++it_vec)
			{
				int index_linkedElement = *it_vec;

				//remove the index from its link, and for all link above the index, minus 1
				std::vector<int>* link_linkedElement = &linkMap[synonym_linkedElement][index_linkedElement].links[synonym_deletedItem];
				std::vector<int>::iterator it_indexDeletedItem = std::find(link_linkedElement->begin(),link_linkedElement->end(),index_deletedItem);
				int index = INT_MAX;

				if(it_indexDeletedItem != link_linkedElement->end())	//found
					index = std::distance(link_linkedElement->begin(),it_indexDeletedItem);

				for(size_t j=0; j<link_linkedElement->size(); ++j)
				{
					if(j > index) {
						(*link_linkedElement)[j] -= 1;
					}
				}
				//remove index
				link_linkedElement->erase(it_indexDeletedItem);

				bool noLinkBackToColumn = false;

				if(link_linkedElement->empty())	//no more element for this synonym, remove this synonym from the link
				{
					noLinkBackToColumn = true;
					linkMap[synonym_linkedElement][index_linkedElement].links.erase(linkMap[synonym_linkedElement][index_linkedElement].links.find(synonym_deletedItem));
				}

				//	check if the LE has any other link, if no, just delete it
				if(linkMap[synonym_linkedElement][index_linkedElement].links.empty())
				{
					// linkMap[synonym_linkedElement].erase(linkMap[synonym_linkedElement].begin() + index_linkedElement);
					//int val = synonymList_INT[synonym_linkedElement][index_linkedElement];
					RemoveElementFromList(synonym_linkedElement , index_linkedElement);
				}

				//	if still has link, check it has any link back to the column
				else {
					if(noLinkBackToColumn)
					{
						//has link back, so don't delete, continue;
					}

					else {
						//no link back, delete recursively
						Remove(synonym_linkedElement , synonymList_INT[synonym_linkedElement][index_linkedElement]);
					}
				}
			}
		}

		//delete the deleted element here?
		RemoveElementFromList(synonym_deletedItem,index_deletedItem);
	}

	template<> void Remove<const char*>(std::string synonym , const char* value)
	{
	}


	template<typename T> void RemoveElementFromList(std::string synonym , T index_deletedElement) {}
	template<> void RemoveElementFromList<int>(std::string synonym_deletedElement , int index_deletedElement)
	{
		//when this function is called, that means that element has no more link, so no need to check its link
		//get index of value from synonymlist
		//check if there is any element above the index
		//if no, just delete it from synonymList and linkMap
		//if yes, for each element above the index
		//	get the link, for each link
		//		go to linkMap and find it, and minus 1
		//
		//delete the element from linkMap, and delete it from vec


		std::cout << "RemoveElementFromList " << synonym_deletedElement << " " << index_deletedElement << "\n";

		if(index_deletedElement == synonymList_INT[synonym_deletedElement].size() - 1)	//if it is last element
		{
			std::cout << synonym_deletedElement << " is last element, just pop it\n";

			synonymList_INT[synonym_deletedElement].pop_back();
			linkMap[synonym_deletedElement].pop_back();
			if(linkMap[synonym_deletedElement].empty())	//remove the synonym also if no more element
			{
				linkMap.erase(linkMap.find(synonym_deletedElement));
			}

			Print();
			std::cout << "After popping\n";
		}

		else {	//there are element above the index
			int index = index_deletedElement + 1; 
			int size = synonymList_INT[synonym_deletedElement].size();

			for(index ; index < size; ++ index)
			{
				std::map<std::string,std::vector<int>> elements = linkMap[synonym_deletedElement][index].links;

				for(std::map<std::string,std::vector<int>>::iterator it_map = elements.begin(); it_map != elements.end(); ++it_map)
				{
					std::string synonym_linkedElement = it_map->first;
					for(std::vector<int>::iterator it_vec = it_map->second.begin(); it_vec != it_map->second.end(); ++it_vec)
					{
						int index_linkedElement = *it_vec;

						//minus 1 at the index
						linkMap[synonym_linkedElement][index_linkedElement].links[synonym_deletedElement][index_deletedElement] -= 1;
					}
				}
			}
		
			synonymList_INT[synonym_deletedElement].erase(synonymList_INT[synonym_deletedElement].begin() + index_deletedElement);
			linkMap[synonym_deletedElement].erase(linkMap[synonym_deletedElement].begin() + index_deletedElement);
			if(linkMap[synonym_deletedElement].empty())	//remove the synonym also if no more element
			{
				linkMap.erase(linkMap.find(synonym_deletedElement));
			}
		}
	}

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






};