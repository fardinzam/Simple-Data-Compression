#include "HashTable.h"
#include "WordEntry.h"
#include <list>
#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
#include <cstdlib>

using namespace std;

HashTable::HashTable (int s) {
	this->size = s;
	hashTable = new list<WordEntry>[s]; //dynamically allocate an array of lists
}

int HashTable::computeHash(const string &s) { //finds which list to look at
    int val = 0;
    int len = s.size();
    for (int i=0; i<len; ++i) {
        val += s[i];
    }
    return val % size;
}

void HashTable::put(const string &s) {//if word is not found it sis inserted
	int index = computeHash(s);
    list<WordEntry>::iterator it = hashTable[index].begin();
    while(it != hashTable[index].end()){
		if(it->getWord() == s){ //check if indexed word is the parameter
			it->addNewAppearance();
			return;
		}
		++it;
	}
    WordEntry newW(s); //if word doesnt exist, create a new entry
	hashTable[index].push_back(newW); // and push it into hashtable (append to indexed list)
}

void HashTable::put(const string &s, const int& value) {
	int index = computeHash(s);
    list<WordEntry>::iterator it = hashTable[index].begin();
    while(it != hashTable[index].end()){
		if(it->getWord() == s){
			cout << "ERROR: TRYING TO PUT WITH DEFINED VALUE" << endl;
			return;
		}
		++it;
	}
    WordEntry newW(s, value); //if word doesnt exist, create a new entry
	hashTable[index].push_back(newW); // and push it into hashtable (append to indexed list)
}

int HashTable::getAppearances(const string &s) {
    int index = computeHash(s);
    list<WordEntry>::iterator it = hashTable[index].begin();
    while(it != hashTable[index].end()){
		if(it->getWord() == s)//if word is found in hashtable
	        return it->getTotalAppearances();
	    ++it;
	}
    return -1;//if not found
}


bool HashTable::contains(const string &s) { //returns true if exists in hashtable, false if not
    int index = computeHash(s);
    list<WordEntry>::iterator it = hashTable[index].begin(); //create iterator to traverse through hashtable
    while(it != hashTable[index].end()){
		if(it->getWord() == s)
			return true;
		++it;
    }
	return false;
}

list<pair<string, int> > HashTable::getList() {//gets word at index, calculates total num of appearances, and pushes into list
    list<pair<string, int> > allItems;
    for(int i=0; i<this->size; i++) {
        list<WordEntry>::iterator it = hashTable[i].begin();
        while(it != hashTable[i].end()){
            string word = it->getWord();
            int count = it->getTotalAppearances();
            pair<string, int> p(word, count);
            allItems.push_back(p);
            ++it;
        }
    }
    return allItems;
}