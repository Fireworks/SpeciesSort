#include <string.h>
#include <stdexcept>
#include <iostream>
#include <streambuf>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include "TreeLeaf.h"
#include "ArrayList.h"


using namespace std;

//Parsing of individual leaf and recursion until all children of the leaf are parsed
void parseLeaf(TreeLeaf* leaf, rapidxml::xml_node<> *node){
	if(node->first_node("taxonomy") == NULL){ //if the node is not a taxonomy, get to the next taxonomy
		rapidxml::xml_node<> *firstChild = node->first_node();
		while(firstChild != NULL){
			parseLeaf(leaf,firstChild);
			firstChild = firstChild->next_sibling();
		}
	} else { //get all the properties of the node
		rapidxml::xml_node<> *taxonomy = node->first_node("taxonomy");
		TreeLeaf *taxLeaf = new TreeLeaf(leaf);
		rapidxml::xml_node<> *sciName = taxonomy->first_node("scientific_name");
		if(sciName == NULL)
			taxLeaf->sciName = "";
		else
			taxLeaf->sciName = string(sciName->first_node()->value(),sciName->first_node()->value_size());
		rapidxml::xml_node<> *comName = taxonomy->first_node("common_name");
		if(comName == NULL)
			taxLeaf->comName = "";
		else
			taxLeaf->comName = string(comName->first_node()->value(),comName->first_node()->value_size());
		rapidxml::xml_node<> *rank = taxonomy->first_node("rank");
		if(rank == NULL)
			taxLeaf->rank = "";
		else
			taxLeaf->rank = string(rank->first_node()->value(),rank->first_node()->value_size());
		rapidxml::xml_node<> *firstChild = node->first_node("clade");
		//While there are still children, recursively parse
		while(firstChild != NULL){
			parseLeaf(taxLeaf,firstChild);
			firstChild = firstChild->next_sibling();
		}
	}
}

//void print(vector<char> printing){
//for( std::vector<char>::const_iterator i = printing.begin(); i != printing.end(); ++i)
//    std::cout << *i << ' ';
//}

TreeLeaf* parse(string filename){
	ifstream t("species.xml");	
	vector<char> buffer((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	buffer.push_back('\0');
    //string text(std::istreambuf_iterator<char>(t), (std::istreambuf_iterator<char>()));
	//text = text.append('\0');
	//print(buffer);
	rapidxml::xml_document<> doc;
	doc.parse<0>(&buffer[0]);
	rapidxml::xml_node<> * root_node = doc.first_node(); // This will give you the phylogeny node.

	TreeLeaf* root = new TreeLeaf(NULL); // This is the actual root node
	root->sciName = "ROOT";
	root->comName = "ROOT";
	root->rank = "ROOT";
	parseLeaf(root,root_node);
	return root;
}

//Get the lineage of a single species
void getLineage(TreeLeaf* tree){
	cout << "Please enter the name of the species of interest:" << endl;
	string input;
	getline(cin, input);
	TreeLeaf* result = tree->search(input);
	ArrayList* path = result->getPath();
	path->print(0);
	return;
}

//Get the common lineage of two species
void getCommon(TreeLeaf* tree){
	cout << "Please enter the name of the first species" << endl;
	string input;
	getline(cin, input);
	cout << "Please enter the name of the second species" << endl;
	string input2;
	getline(cin, input2);

	TreeLeaf* result = tree->search(input);
	TreeLeaf* result2 = tree->search(input2);

	//Get each path and reverse so it is in correct order
	ArrayList* path = result->getPath();
	ArrayList* path2 = result2->getPath();
	path->reverse();
	path2->reverse();

	//Check when the paths are equal, and if so then print the common level
	//Otherwise print the rest of the path for each species
	for(int i = 0 ; i < max(path->size,path2->size) ; i++){
		string output = "";
		string com = "] Common Ancestor: ";
		if(path->get(i) == path2->get(i)){
			output = "Level[" + std::to_string(i) + com + path->get(i)->comName + " (" + path->get(i)->sciName + ", " + path->get(i)->rank + ")";
			cout << output << endl;
		} else {
			if(i == 0){
				cout << "One of your species was not found. Please check your spelling and try again." << endl;
				return;
			}
			cout << "Ancestry unique to " + input << endl;
			path->print(i);//This print method prints starting from current index
			cout << "Ancestry unique to " + input2 << endl;
			path2->print(i);
			return;
		}
	}
	cout << "Both of your species were not found.  Please check your spelling and try again." << endl;
}

int main(int argc, char *argv[]){
	TreeLeaf* treeOfLife = parse("species.xml");
	
	//Run through console mode
	if(argv[1] != NULL){
		int option =  atoi(argv[1]);

		if(option == 1){
			TreeLeaf* result = treeOfLife->search(argv[2]);
			ArrayList* path = result->getPath();
			path->print(0);
			cin.get();
			return 0;
		} else if (option == 2){
			string one = argv[2];
			string two = argv[3];
			TreeLeaf* result = treeOfLife->search(one);
			TreeLeaf* result2 = treeOfLife->search(two);

			ArrayList* path = result->getPath();
			ArrayList* path2 = result2->getPath();
			path->reverse();
			path2->reverse();

			for(int i = 0 ; i < max(path->size,path2->size) ; i++){
				string output = "";
				string com = "] Common Ancestor: ";
				if(path->get(i) == path2->get(i)){
					output = "Level[" + std::to_string(i) + com + path->get(i)->comName + " (" + path->get(i)->sciName + ", " + path->get(i)->rank + ")";
					cout << output << endl;
				} else {
					cout << "Ancestry unique to " + one << endl;
					path->print(i);
					cout << "Ancestry unique to " +  two << endl;
					path2->print(i);
					cin.get();
					return 0;
				}
			}
		}
	}
	
	//UI mode
	cout << "Welcome to my Tree of Life program!" << endl;
	start:
	cout << "Please choose from the following options:" << endl;
	cout << "     1.Get the lineage of a species" << endl;
	cout << "     2.Get the commmon lineage of two species" << endl;
	cout << "     3.Exit program" << endl;
	string input;
	getline(cin, input);
	int option = atoi(input.c_str());
	
	switch(option){
	case 1:
		getLineage(treeOfLife);
		cout << "Press any key to continue" << endl;
		cin.get();
		goto start;
	case 2:
		getCommon(treeOfLife);
		cout << "Press any key to continue" << endl;
		cin.get();
		goto start;
	case 3:
		exit(0);
	default:
		cout << "Not a valid option. Try again. ";
		goto start;
	}
}

