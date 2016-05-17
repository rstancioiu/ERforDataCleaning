// Reader.h
#ifndef READER_H
#define READER_H
#include "Util.h"
#include <fstream>



class Reader{	

	private:
		void read(ifstream& input, vector<vector<int> >& tab);

	public:
		Reader(){
			ios_base::sync_with_stdio();
			cin.tie(0);
		};
		pair<vvi,vvi> readFiles(const char* data_r,const char* data_s);
};
#endif // READER_H