// Mapping.h
#ifndef MAPPING_H
#define MAPPING_H
#include <algorithm>
#include <unordered_map>
#include "Bitset.h"
#include "Util.h"

class Mapping{	
	
	public:
		Mapping(){};
		vector<Bitset> mapDataToBitsets(vvi& r, vvi& s);
		vector<vector<int> > mapDataToVectors(vvi& r,vvi& s);
};
#endif // MAPPING_H