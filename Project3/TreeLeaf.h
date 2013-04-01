#ifndef __treeleaf__
#define __treeleaf__

class TreeLeaf;
#include "ArrayList.h"

using namespace std;


class TreeLeaf{

public:
	TreeLeaf();
	TreeLeaf* parent;
	TreeLeaf(TreeLeaf *parent);
	ArrayList* children;
	void addChild(TreeLeaf *child);
	TreeLeaf* getChild(int index);
	int numChildren();
	TreeLeaf* search(string search);
	ArrayList* getPath();
	string sciName;
	string comName;
	string rank;
};


#endif