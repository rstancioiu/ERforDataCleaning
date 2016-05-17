#include "Mapping.h"

// Maps the tables r and s into an array of Bitsets
vector<Bitset> Mapping::mapDataToBitsets(vvi& r, vvi& s){
	vvi itemsets = mapDataToVectors(r,s);
	vector<Bitset> ret;
	int nbAttributes = r[0].size()+s[0].size(); 
	for(uint32_t i=0;i<itemsets.size();++i){
		vector<bool> array(nbAttributes, false);
		for(uint32_t j=0;j<itemsets[i].size();++j){

			array[itemsets[i][j]]=true;
		}
		Bitset set(array);
		ret.push_back(set);
	}	
	return ret;
}

// Maps the tables r and s into a table of integers
vvi Mapping::mapDataToVectors(vvi &r, vvi &s){
	vvi itemsets;
	for(uint32_t i=0;i<r.size();++i) {
		for(uint32_t j=0;j<r[i].size();++j) {
			if(r[i][j]==0)
				continue;

			it = objectToId.find(r[i][j]);
			if(it==objectToId.end()) {
				int size = itemsets.size();
				vector<int> new_itemset;
				new_itemset.push_back(j);
				itemsets.push_back(new_itemset);
				objectToId.insert(make_pair(r[i][j],size));
			} else {
				itemsets[it->second].push_back(j);
			}
		}
	}
	int cnt = r[0].size();
	for(uint32_t i=0;i<s.size();++i) {
		for(uint32_t j=0;j<s[i].size();++j) {
			it = objectToId.find(s[i][j]);
			if(it==objectToId.end() || s[i][j]==0)
				continue;
			int p = it->second;
			int l = j+cnt;
			itemsets[p].push_back(l);
		}
	}
	for(uint32_t i=0;i<itemsets.size();++i){	
		sort(itemsets[i].begin(),itemsets[i].end());
		int p=1;
		for(uint32_t j=1;j<itemsets[i].size();++j){
			if(itemsets[i][j]!=itemsets[i][j-1])
				itemsets[i][p++]=itemsets[i][j];
		}
		itemsets[i].resize(p);
	}
	objectToId.clear();
	return itemsets;
}