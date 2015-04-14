#include "IntermediateResult.h"
#include <iostream>
#include <algorithm>
#include <sstream>   

using namespace std;

IntermediateResult::IntermediateResult(void) {}

void IntermediateResult::Initialize(vector<Declaration> declarations)
{
	for(std::vector<Declaration>::iterator it = declarations.begin(); it != declarations.end(); ++it)
	{
		string value = it->synonym.value;
		synonymList[value] = vector<Element>();
		synonyms.push_back(it->synonym);
	}

	return;
}

void IntermediateResult::Insert(string synonym , int value)
{	
	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		if(it->first == synonym) 
		{
			it->second.push_back(Element(synonym , ITOS(value)));
			return;
		}
	}
	
	std::cout << "In IntermediateResult::InsertElement, " << synonym << " " << value << " not found in synonymList_INT \n";

	return;
}

void IntermediateResult::Insert(string synonym , string value)
{	
	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		if(it->first == synonym) 
		{
			it->second.push_back(Element(synonym , value));
				return;
		}
	}

	std::cout << "In IntermediateResult::InsertElement, " << synonym << " " << value << " not found in synonymList_STR \n";

	return;
}

void IntermediateResult::InsertPair(string synonym_1 , int value_1_int, string synonym_2 , int value_2_int)
{
	if(synonym_1 == synonym_2)	return;

	string value_1 = ITOS(value_1_int);
	string value_2 = ITOS(value_2_int);
	InsertPair(synonym_1 , value_1, synonym_2 , value_2);
}


void IntermediateResult::InsertPair(string synonym_1 , int value_1_int, string synonym_2 , string value_2)
{
	if(synonym_1 == synonym_2)	return;

	string value_1 = ITOS(value_1_int);
	InsertPair(synonym_1 , value_1, synonym_2 , value_2);
}


void IntermediateResult::InsertPair(string synonym_1 , string value_1, string synonym_2 , int value_2_int)
{
	if(synonym_1 == synonym_2)	return;

	string value_2 = ITOS(value_2_int);
	InsertPair(synonym_1 , value_1, synonym_2 , value_2);
}


void IntermediateResult::InsertPair(string synonym_1 , string value_1, string synonym_2 , string value_2)
{
	//find value_1, if does not exist, add. else do nothing
	//find value_2, if does not exist, add. else do nothing
	//call make link to put value1 in value2, and value2 in value1, 
	if(synonym_1 == synonym_2)	return;

	if(!Exist(synonym_1 , value_1)) Insert(synonym_1 , value_1);
	if(!Exist(synonym_2 , value_2)) Insert(synonym_2 , value_2);

	MakeLink(synonym_1 , value_1, synonym_2 , value_2);
}

bool IntermediateResult::Exist(string synonym , string value)
{
	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		//synonym matched
		if(it->first == synonym)
		{
			for(vector<Element>::iterator it_elements = it->second.begin(); it_elements != it->second.end(); ++it_elements)
			{
				//value matched
				if(it_elements->value == value)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void IntermediateResult::MakeLink(string synonym_1, string value_1, string synonym_2, string value_2)
{
	//find value1, find synonym2 in element, if not exist, create
	//push value2 to the correct link
	//repeat for value2

	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		//synonym_1 matched
		if(it->first == synonym_1)
		{
			for(vector<Element>::iterator it_elements = it->second.begin(); it_elements != it->second.end(); ++it_elements)
			{
				//value_1 matched
				if(it_elements->value == value_1)
				{
					links_iterator it_synonym2_found = it_elements->links.find(synonym_2);

					if(it_synonym2_found != it_elements->links.end())
					{
						std::vector<string>::iterator it_value2_found = find(it_synonym2_found->second.begin() , it_synonym2_found->second.end() , value_2);

						if(it_value2_found != it_synonym2_found->second.end())
						{
							//alread has link, do nothing
						}

						else
						{
							//add value2
							it_synonym2_found->second.push_back(value_2);
						}
					}

					else
					{
						//add synonym2 and add value 2
						it_elements->links[synonym_2] = vector<string>();
						it_elements->links[synonym_2].push_back(value_2);
					}
				}
			}
		}

		//synonym_2 matched
		if(it->first == synonym_2)
		{
			for(vector<Element>::iterator it_elements = it->second.begin(); it_elements != it->second.end(); ++it_elements)
			{
				//value_2 matched
				if(it_elements->value == value_2)
				{
					links_iterator it_synonym1_found = it_elements->links.find(synonym_1);

					if(it_synonym1_found != it_elements->links.end())
					{
						std::vector<string>::iterator it_value1_found = find(it_synonym1_found->second.begin() , it_synonym1_found->second.end() , value_1);

						if(it_value1_found != it_synonym1_found->second.end())
						{
							//alread has link, do nothing
						}

						else
						{
							//add value1
							it_synonym1_found->second.push_back(value_1);
						}
					}

					else
					{
						//add synonym 1 and add value 1
						it_elements->links[synonym_1] = vector<string>();
						it_elements->links[synonym_1].push_back(value_1);
					}
				}
			}
		}
	}
}


void IntermediateResult::Remove(string synonym , string value)
{
	//if value has no link to anything, just remove it

	//if value has link , delete the link between value and linkvalue, then check if linkvalue is empty, if yes, REmove it also
	//if it is not empty, do nothing
	//check if synonym column has any other value that link back to link synonym column,
	//if has do nothing
	//if not, call Remove on all the element u removed just now
	//remove value in synonym column
	cout << "Removing " << synonym << " " << value << "\n";
	vector<string> linkBackColumns;	//to check which column need to link back to 

	//must check value is in synonym or not, if not just return , output some message !!!!!!!!!!!

	links_map links = GetLinks(synonym , value);
	if(links.empty())
	{
		cout << "links is empty\n";
		int index = GetElementIndex(synonym , value);
		if(index != -1)
			synonymList[synonym].erase(synonymList[synonym].begin() + index);
	}

	else
	{
		cout << "links is not empty\n";
		//remove value from link value
		//Duo - v(m x y z)
		for(links_iterator it_links = links.begin(); it_links != links.end(); ++it_links)
		{
			
			string linkSynonym = it_links->first;
			vector<string> linkValueList = it_links->second;

			cout << "Go to " << linkSynonym << " ";

			linkBackColumns.push_back(linkSynonym);	

			//x - a(9 19 29) - q(Duo)
			for(vector<string>::iterator it_links_value = linkValueList.begin(); it_links_value != linkValueList.end(); ++it_links_value)
			{
				string linkValue = *it_links_value;
				int index = GetElementIndex(linkSynonym , linkValue);

				cout << linkValue << "\n";
				
				vector<string> *temp = &synonymList[linkSynonym][index].links[synonym];
				vector<string>::iterator found = find(temp->begin() , temp->end() , value);

				if(found != temp->end())
				{
					int i = distance(temp->begin() , found);
					temp->erase(temp->begin() + i);

					//if x - a(9 19 29) - q()
					//remove the synonym
					//else do nothing
					if(temp->empty())
					{
						cout << "here is empty\n";
						synonymList[linkSynonym][index].links.erase(synonym);
					}

					//x - empty
					//call Remove on x
					//else do nothing
					if(synonymList[linkSynonym][index].links.empty())
					{
						cout << "no more link\n";
						Remove(linkSynonym , linkValue);
					}
				}
			}
		}
		cout << "here1\n";
		//check if synonym column has any other value that link back to link synonym column,
		//if has do nothing
		//if not, call Remove on all the element u removed just now
		vector<Element> elements = synonymList[synonym];

		for(vector<Element>::iterator it_elements = elements.begin(); it_elements != elements.end(); ++it_elements)
		{
			if(it_elements->value != value)
			{
				for(links_iterator it_links = it_elements->links.begin(); it_links != it_elements->links.end(); ++it_links)
				{
					string linkSynonym = it_links->first;
					
					vector<string>::iterator found = find(linkBackColumns.begin() , linkBackColumns.end() , linkSynonym);

					if(found != linkBackColumns.end())
					{
						linkBackColumns.erase(found);
					}

					if(linkBackColumns.empty()) break;
				}
			}
			if(linkBackColumns.empty()) break;
		}
		cout << "here2\n";
		//not empty means some columns has no links back
		if(!linkBackColumns.empty())
		{
			for(vector<string>::iterator columnToDelete = linkBackColumns.begin(); columnToDelete != linkBackColumns.end(); ++columnToDelete)
			{
				for(links_iterator linksToDelete = links.begin(); linksToDelete != links.end(); ++linksToDelete)
				{
					if(linksToDelete->first == *columnToDelete)
					{
						for(vector<string>::iterator linkValueToDelete = linksToDelete->second.begin(); linkValueToDelete != linksToDelete->second.end(); ++linkValueToDelete)
						{
							Remove(linksToDelete->first , *linkValueToDelete);
						}
					}
				}
			}
		}
		cout << "here3\n";
		//remove value in synonym column
		int index = GetElementIndex(synonym , value);
		synonymList[synonym].erase(synonymList[synonym].begin() + index);
	}
}

void IntermediateResult::Remove(string synonym , int value_int)
{
	//convert int to string and call the above remove
	string value = ITOS(value_int);
	Remove(synonym , value);
}

links_map IntermediateResult::GetLinks(string synonym, string value)
{
	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		if(it->first == synonym)
		{
			for(vector<Element>::iterator it_elements = it->second.begin(); it_elements != it->second.end(); ++it_elements)
			{
				if(it_elements->value == value)
					return it_elements->links;
			}
		}
	}

	return links_map();
}

//given synonym and value, find the index of the elements of the synonym column that match the value
int IntermediateResult::GetElementIndex(string synonym, string value)
{
	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		if(it->first == synonym)
		{
			for(vector<Element>::iterator it_elements = it->second.begin(); it_elements != it->second.end(); ++it_elements)
			{
				if(it_elements->value == value)
					return distance(it->second.begin() , it_elements);
			}
		}
	}

	return -1;
}

void IntermediateResult::GetList(std::string synonym, std::vector<int>& list)
{
	vector<string> temp;
	vector<int> tempInt;

	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		if(it->first == synonym) 
		{
			for(int i=0; i < it->second.size(); ++i)
				temp.push_back(it->second[i].value);	

			list = STOI(temp);

			return;
		}
	}

	std::cout << "In IntermediateResult::GetList, " << synonym << " not found in synonymList_INT\n";

	return;
}

void IntermediateResult::GetList(std::string synonym, std::vector<std::string>& list)
{
	vector<string> temp;

	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		if(it->first == synonym) 
		{
			for(int i=0; i < it->second.size(); ++i)
				temp.push_back(it->second[i].value);	

			list = temp;

			return;
		}
	}

	std::cout << "In IntermediateResult::GetList, " << synonym << " not found in synonymList\n";

	return;
}


bool IntermediateResult::IsListEmpty(Synonym synonym)
{
	bool empty = true;

	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		if(it->first == synonym.value) {
			if(!it->second.empty())
				empty = false;
			return empty;
		}
	}
	

	std::cout << "In IntermediateResult::IsListEmpty, " << synonym.value << " not found in both synonymList\n";

	return empty;
}


void IntermediateResult::Print()
{
	cout << "\n\nPrint Intermediate Result\n";
	cout << "----------------------------------------------\n";

	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		cout << it->first << " : ";
		for(vector<Element>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			cout << it2->value << " ";
		cout << "\n";
	}


	std::cout << "\n";
	
	for(it_synonymList it = synonymList.begin(); it != synonymList.end(); ++it)
	{
		cout << it->first << "\n";
		cout << "-----------------------\n";

		for(vector<Element>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			cout << it2->value << " - ";

			for(map<string,vector<string>>::iterator it3 = it2->links.begin(); it3 != it2->links.end(); ++it3)
			{
				cout << it3->first << "( ";

				for(vector<string>::iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4)
					cout << *it4 << " ";

				if(distance(it3 , it2->links.end()) == 1)	cout << ")";
				else										cout << ")  -  ";
			}
			cout << "\n";
		}
		cout << "\n";
	}
	cout << "----------------------------------------------\n";

	return;
}

std::vector<Synonym> IntermediateResult::GetAllSynonyms()
{
	return synonyms;
}

SynonymType IntermediateResult::GetSynonymType(std::string val)
{
	for(std::vector<Synonym>::iterator it = synonyms.begin(); it != synonyms.end(); ++it)
	{
		if(it->value == val)
			return it->type;
	}


	return INVALID_SYNONYM_TYPE;
}



string IntermediateResult::ITOS(int num)
{
	return to_string(long long(num));
}

vector<string> IntermediateResult::ITOS(vector<int> intList)
{
	vector<string> strList;

	for(int i=0; i < intList.size(); ++i)
		strList.push_back(ITOS(intList[i]));
	
	return strList;
}

int IntermediateResult::STOI(string s)
{
	int i;
    istringstream ss(s);
    ss >> i;
    
	return i;
}

vector<int> IntermediateResult::STOI(vector<string> strList)
{
	vector<int> intList;

	for(int i=0; i < strList.size(); ++i)
		intList.push_back(STOI(strList[i]));

	return intList;
}
