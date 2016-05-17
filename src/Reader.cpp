#include "Reader.h"
#include <sstream>
#include <iostream>

// Reades two files r and s
pair<vvi, vvi> Reader::readFiles(const char* data_r,const char* data_s){
	vvi r,s;
	ifstream input;
	input.open(data_r);
	read(input,r);
	input.close();
	input.clear();
	input.open(data_s);
	read(input,s);
	input.close();
	input.clear();
	return make_pair(r,s);
}

// Reades an input file and writes the results into an 2d array
void Reader::read(ifstream& input, vector<vector<int> >& tab){
	int number;
	for(string line; getline(input,line);){
		istringstream iss(line);
		vector<int> tuple;
		while(iss>>number){
			tuple.push_back(number);
		}
		tab.push_back(tuple);
	}
}