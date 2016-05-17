#include "Reader.h"
#include "Bitset.h"
#include "Mapping.h"
#include "Util.h"
#include "UIND.h"
#include "AVL.h"

const char* data_r = "test/r-large.txt";
const char* data_s = "test/s-large.txt";

void read();
void printVectors();
void mapData();
void computeUIND();
vvi r,s;

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	read();
	mapData();
	computeUIND();

	return 0;
}



// Reads two files r and s by calling the class Reader
// and prints out the time spent for reading.
void read(){
	clock_t start,end;
	double time;
	start = clock();
	Reader* reader = new Reader();
	pair<vvi,vvi> prs = reader->readFiles(data_r,data_s);
	r = prs.first;
	s = prs.second;
	delete reader;
	end = clock();
	time = (double)(end-start)/CLOCKS_PER_SEC*1000.0;
	cout<<"Time for reading: "<<time<<endl;
}

vector<Bitset> bitsets;

// Maps the data read previously into an array of Bitsets
void mapData(){
	clock_t start,end;
	double time;
	start = clock();
	Mapping* mapping = new Mapping();
 	bitsets = mapping->mapDataToBitsets(r,s);
	delete mapping;
	end = clock();
	time = (double)(end-start)/CLOCKS_PER_SEC*1000.0;
	cout<<"Time for mapping: "<<time<<endl;
}

// Given an array of bitsets, it computes the closure or 
// the support for each attribute.
void computeUIND(){
	clock_t start,end;
	double time;
	start = clock();
	UIND* uind = new UIND();
	//uind->computeClosure(bitsets);
	uind->computeSupports(bitsets,0.9);
	delete uind;
	end = clock();
	time = (double)(end-start)/CLOCKS_PER_SEC*1000.0;
	cout<<"Time for UIND: "<<time<<endl;
}