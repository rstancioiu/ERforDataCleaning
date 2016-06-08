// Reader.h
#ifndef READER_H
#define READER_H
#include "Util.h"
#include <fstream>



class Reader{	

	private:
		void read(const char* data, vvi& tab, vs& colName);

	public:
		Reader(){};
		pair< pair<vvi, vs>, pair<vvi,vs> > readFiles(const char* data_r,const char* data_s);
};
#endif // READER_H