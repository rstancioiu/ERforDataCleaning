#include "UIND.h"

// Given an array of bitsets it computes the closure for each
// attribute.
vector<Bitset> UIND::computeClosure(vector<Bitset>& tab){
	uint32_t size = tab[0].size();
	vector<Bitset> ret;
	vector<bool> array(size,true);
	for(uint32_t i=0;i<size;++i){
		Bitset bitset(array);
		for(uint32_t j=0;j<tab.size();++j){
			if(tab[j].check(i)){
				bitset = bitset&tab[j];
			}
		}
		ret.push_back(bitset);
	}
	return ret;
}

// Given an array of bitsets it computes the support of each 
// combination of two attributes. 
// eg. sup(AB)/sup(A)
vector<vector<double> > UIND::computeSupports(vector<Bitset>& tab){
	vector<vector<double> > ret;
	uint32_t size = tab[0].size();
	for(uint32_t i=0;i<size;++i){
		vector<double> array(size,0);
		vector<int> count(size,0);
		for(uint32_t j=0;j<tab.size();++j){
			if(tab[j].check(i)){
				for(uint32_t k=0;k<size;++k){
					if(tab[j].check(k)){
						count[k]++;
					}
				}
			}
		}
		for(uint32_t j=0;j<array.size();++j){
			array[j] = (double)count[j]/count[i];
		}
		ret.push_back(array);
	}
	return ret;
}