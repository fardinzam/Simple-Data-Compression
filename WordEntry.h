#ifndef WORDENTRY_H
#define WORDENTRY_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class WordEntry {
 private:
	string word;
	int numAppearances;
 public:
	WordEntry(const string&);
	WordEntry(const string&, const int&);
	void addNewAppearance();
	const string& getWord();
	int getTotalAppearances();
};
#endif