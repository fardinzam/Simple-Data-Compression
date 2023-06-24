#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include "HashTable.h"
using namespace std;

//adds words to hastahble and increments frequency
HashTable countTokens(string filename){
    ifstream inFS(filename);
    if (inFS.fail()) {
	    cout << "could not open file" << endl;
	    exit(1);
	}
    string line;
    HashTable counterHashTable(10000); // declare the hashtable [word -> count]
    while(!inFS.eof()) {
        getline(inFS, line); // read one line from the file
        // go through the line, build up each token char by char, then append each token to a list 
        string buffer = "";
        for(int i=0; i<line.length(); i++) {
            if(!isalpha(line[i])) { // encountered a non-letter, which means we're at the end of a token
                if(buffer.length()) { // buffer contains a word, insert buffer into hashtable
                    counterHashTable.put(buffer);
                    buffer = "";
                }
                if(line[i] != ' ') {  // insert line[i] into hashtable
                    counterHashTable.put(string(1, line[i])); //converts char to string and pushes into hashtable
                }
            }
            else {
                buffer.append(string(1, line[i]));//converts char to string and appends to buffer
            }
            // if we're at the end of the line and we have something in the
            //   buffer, we need to insert it ito the counterHashTable
            if(i == line.length() - 1 && buffer.length()) {
                counterHashTable.put(buffer); 
            }
        }
    }
    inFS.close();
    return counterHashTable;
}

//helps another function order the words from greatest to least
bool comparator(const pair<string, int>& left, const pair<string, int>& right) {
    return left.second > right.second;
}

//converts hashtable to list
list<pair<string, int> > convertCounterHashTableToSortedList(HashTable counterHashTable) {
    list<pair<string, int> > tokenList = counterHashTable.getList();  // convert hashtable to list
    tokenList.sort(comparator);  // sort using custom comparator
    return tokenList; //uses this list to get encoded/decoded hashtable
}

HashTable getEncoder(list<pair<string, int> >& tokenList) {
    HashTable encoder(10000);
    list<pair<string, int> >::iterator it = tokenList.begin();
    int tokenCode = 1;
    while(it != tokenList.end()) {
        encoder.put(it->first, tokenCode);//finds word at iterator, tokenCode is the frequency that is updated in the loop
        tokenCode++;//increments frequency
        it++;//iterator
    }
    return encoder; //returns encoded file
}

void writeDecoderToFile(string decoderFilename, list<pair<string, int> >& tokenList) {
    // open the output file
    ofstream ofile(decoderFilename);
    if(ofile.fail()) {
        cout << "ERROR OPENING OUTPUT FILE " + decoderFilename << endl;
        exit(1);
    }
    // iterate through the sorted list of tokens and write them to file
    list<pair<string, int> >::iterator it = tokenList.begin();
    int tokenCode = 1;
    ofile << "Code -> Token" << endl; //first line of text of output file
    while(it != tokenList.end()) {
        string token = it->first;
        // write token and tokenCode to file
        ofile << tokenCode << " -> " << token << endl;
        tokenCode++; // increment tokenCode for the next token
        it++; // increment iterator
    }
    ofile.close();
}

void writeEncodedFile(string originalFilename, string encodedFilename, HashTable encoder) {
    // in a loop, getline from original file, encode the line, then write encoded line to outputFile
    // open both files
    ifstream ogFS(originalFilename);
    ofstream enFS(encodedFilename);
    if (ogFS.fail()) {
	    cout << "could not open file original File: " << originalFilename << endl;
	    exit(1);
	}
    if (enFS.fail()) {
	    cout << "could not open file encoded File: " << encodedFilename << endl;
	    exit(1);
	}
    // go through ogFS line by line, extract each token, 
    //   convert the token to the code, write the encoded line to enFS
    string line;
    while(!ogFS.eof()) {
        getline(ogFS, line);
        string buffer = "";
        for(int i=0; i<line.length(); i++) {
            if(!isalpha(line[i])) { // encountered a non-letter, which means we're at the end of a token
                if(buffer.length()) { // buffer contains a word, insert buffer into hashtable
                    // found a word!! Write its encoded representation to enFS:
                    enFS << encoder.getAppearances(buffer);
                    buffer = "";
                }
                // insert line[i] into hashtable
                if(line[i] != ' ') { // found a non-alpha char! Write the char's code to enFS
                    enFS << encoder.getAppearances(string(1, line[i]));
                }
                else {
                    enFS << " "; //adds a space inbetween each "word"
                }

            }
            else {
                buffer.append(string(1, line[i])); //converts char to string and appends to buffer
            }
            // if we're at the end of the line and we have something in the 
            //   buffer, we need to insert it ito the counterHashTable
            if(i == line.length() - 1 && buffer.length()) {
                // word found at end of line! Write its encoded representation to enFS:
                enFS << encoder.getAppearances(buffer);
            }
        }
    }
    ogFS.close();
    enFS.close();
}


int main() {
    string filename = "TESTING_mobydick.txt";
    string encodedFilename = "TESTING_mobydick_ENCODED.txt";
    string decoderFilename = "decoder.txt";
    // create hashtable [word -> count]
    HashTable counterHashTable = countTokens(filename);
    // convert hashtable to list<pair<string, int>> and sort descending by count
    list<pair<string, int> > tokenList = convertCounterHashTableToSortedList(counterHashTable);
    // create the encoder hashtables by iterating through the sorted list
    HashTable encoder = getEncoder(tokenList);
    // write decoder and encoded output file
    writeDecoderToFile(decoderFilename, tokenList);
    writeEncodedFile(filename, encodedFilename, encoder);
    return 0;
}
