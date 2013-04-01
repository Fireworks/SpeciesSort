#ifndef __arraylist__
#define __arraylist__

class ArrayList;
#include <iostream>
#include "TreeLeaf.h"

using namespace std;

class ArrayList {
public:
	ArrayList();
	int size;
	double execTime;
	int numAccesses;
	void add(TreeLeaf* item);
	void insert(int index, TreeLeaf* item);
	void set(int index, TreeLeaf* item);
	void remove(int index, TreeLeaf* item);
	TreeLeaf* get(int index);
	void reverse();
	void inSort(int criteria);
	void print(int start);
	double getExecTime();
	int getNumAccess();
private:
	TreeLeaf** items;

};


#endif