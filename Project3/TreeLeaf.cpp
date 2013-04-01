#include "TreeLeaf.h"
#include <stdlib.h>
#include <sstream>

using namespace std;

TreeLeaf::TreeLeaf(TreeLeaf *parent){
	this->children = new ArrayList;
	this->parent = parent;

	if(parent != NULL)
		parent->addChild(this);
}

void TreeLeaf::addChild(TreeLeaf *child){
	this->children->add(child);
}

TreeLeaf* TreeLeaf::getChild(int index){
	return this->children->get(index);
}

int TreeLeaf::numChildren(){
	return this->children->size;
}

//Iterate through parents of the current node
//Add all nodes leading up to root to ArrayList
ArrayList* TreeLeaf::getPath(){
	ArrayList* path = new ArrayList();
	TreeLeaf* current = this;

	while(current != NULL){
		path->add(current);
		current = current->parent;
	}
	return path;
}

//Search for the species in the tree by iterating
//recursively through children of node
TreeLeaf* TreeLeaf::search(string search){
	if(comName.compare(search) == 0 || sciName.compare(search) == 0)
		return this;
	else{
		for(int i = 0 ; i < this->numChildren() ; i++){
			TreeLeaf* temp = getChild(i)->search(search);
			if(temp != NULL)
				return temp;
		}
	}
	return NULL;
}