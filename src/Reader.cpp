#include "Reader.h"
#include "ReadTextFile.h"
#include <sstream>
#include <iostream>

// Reades two files r and s
pair< pair<vvi, vs>,pair<vvi,vs> > Reader::readFiles(const char* data_r,const char* data_s){
	vvi r,s;
	vs cr, cs;
	read(data_r, r, cr);
	read(data_s, s, cs);
	return make_pair(make_pair(r,cr), make_pair(s,cs));
}

// Reades an input file and writes the results into an 2d array
void Reader::read(const char* data, vvi& tab, vs& colName){
	char _cDelim = '\0';
	ReadTextFile textFile(_cDelim);
	if ( ! textFile.openTextFile(data, colName) )
		return;

	vector<string> vstring;
	while(textFile.getline(vstring)){
		vector<int> new_tab;
		for(uint32_t i=0;i<vstring.size();++i)
		{
			int value = atoi(vstring[i].c_str());
			new_tab.push_back(value);
		}
		tab.push_back(new_tab);
	}
}