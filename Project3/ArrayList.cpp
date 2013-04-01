#include <string.h>
#include <stdexcept>
#include <iostream>
#include <streambuf>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <ctime>
#include "ArrayList.h"
#include "TreeLeaf.h"

using namespace std;

ArrayList::ArrayList(){
this->size = 0;
this->items = new TreeLeaf*[0];
};

void ArrayList::add(TreeLeaf* item){
	TreeLeaf** newItems = new TreeLeaf*[size+1];
	for(int i = 0 ; i < size ; i++){
		newItems[i] = items[i];
	}
	newItems[size] = item;
	items = newItems;
	size++;
}

void ArrayList::insert(int index, TreeLeaf* item){
	TreeLeaf** newItems = new TreeLeaf*[size+1];

	for(int i = 0 ; i < index ; i++){
		newItems[i] = items[i];
	}
	newItems[index] = item;
	for(int i = index ; i < size ; i++){
		newItems[i+1] = items[i];
	}
	items = newItems;
	size++;
}

void ArrayList::set(int index, TreeLeaf* item){
	items[index] = item;
}

void ArrayList::remove(int index, TreeLeaf* item){
	TreeLeaf** newItems = new TreeLeaf*[size-1];

	for(int i = 0 ; i < size ; i++){
		if(i < index)
			newItems[i] = items[i];
		else if(i > index)
			newItems[i-1] = items[i];
	}
	items = newItems;
	size--;
}

TreeLeaf* ArrayList::get(int index){
	return items[index];
}

void ArrayList::reverse(){
	numAccesses = 0;
	clock_t startClock = clock();
	TreeLeaf** newList = new TreeLeaf*[size];

	for(int i = 0; i < size ; i++){
		newList[i] = items[size-i-1];
		numAccesses++;
	}
	items = newList;
	numAccesses++;
	clock_t stopClock = clock();
	execTime = (double)(stopClock - startClock)/(double)CLOCKS_PER_SEC;
}

//void ArrayList::inSort(int criteria){
//	numAccesses = 0;
//	clock_t startClock = clock();	
//	int j = 0;
//	TreeLeaf* key;
//		
//	for (int i = 1 ; i < size ; ++i){
//		numAccesses++;
//		key = items[i];
//		j = i - 1;
//		while(j >= 0 && items[j]->compareTo(criteria, key) == 1){
//			numAccesses++;
//			items[j+1] = items[j];
//			j--;
//		} 
//		items[j+1] = key;
//		numAccesses++;
//	}
//	clock_t stopClock = clock();
//	execTime = (double)(stopClock - startClock)/(double)CLOCKS_PER_SEC;
//}

//void ArrayList::print(){
//	clock_t startClock = clock();
//	string output = "";
//	for(int i = 0 ; i < size ; i++){
//		numAccesses++;
//		output = items[i]->fname;
//		cout << items[i]->toString() << endl;
//	}
//	clock_t stopClock = clock();
//	execTime = (double)(stopClock - startClock)/(double)CLOCKS_PER_SEC;
//}

void ArrayList::print(int start){
	string output = "";
	string com = "] ";
	bool reg = true;
	if(start > 0)
		reg = false;
	for(start ; start < size ; start++){
		output = items[start]->comName + " (" + items[start]->sciName + ", " + items[start]->rank + ")";
		if(reg)
			output.insert(0,start*2,'>');
		else{
			if(start == size-1)
				output = "Level[" + std::to_string(start) + com + "Species of interest: " + output;
			else
				output = "Level[" + std::to_string(start) + com + output;
		}
		cout << output << endl;
	}
}

double ArrayList::getExecTime(){
	return execTime;
}

int ArrayList::getNumAccess(){
	return numAccesses;
}