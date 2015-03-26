#include "Answers.h"


vector<Answers*> Answers::getLinksFromList(int listIndex){
	return this->links[listIndex];
}

void Answers::addLink(Answers answer1, Answers answer2){
	answer1.links[answer2.listIndex].push_back(&answer2);
	answer2.links[answer1.listIndex].push_back(&answer1);
}

bool Answers::hasLinksWith(int index){
	return !(this->links[index].empty());
}

vector<Answers*> Answers::cleanLinks(){

	vector<Answers*> needDelete;

	for(int i=0;i<(int)this->links.size();i++)
		for(int j=0;j<(int)this->links[i].size();j++){
			if(!(*(this->links[i][j])).links[listIndex].empty()){
				vector<Answers*> *tempList = &((*(this->links[i][j])).links[listIndex]);

				vector<Answers*>::iterator iter = (*tempList).begin();

				while(iter!=(*tempList).end()){
					Answers* temp = *iter; 
					if(temp == this)  
						iter = (*tempList).erase(iter);
					else iter++;
				}

				if((*(this->links[i][j])).links[listIndex].empty()){
					needDelete.push_back(this->links[i][j]);
				}
			}
		}

	return needDelete;
}

bool Answers::operator==(Answers answer2){
	if((this->indexNumber==answer2.indexNumber)&&(this->listIndex==answer2.listIndex))
		return true;
	else 
		return false;
}