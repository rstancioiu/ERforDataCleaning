// Bitset.h
#ifndef BITSET_H
#define BITSET_H
#include "Util.h"


class Bitset{	
	vector<bool> tab;

	public:
		Bitset(){};
		Bitset(vector<bool>& new_bitset);
		Bitset(const Bitset &obj);
		Bitset operator&(const Bitset& b);
		Bitset operator|(const Bitset& b);
		bool check(uint32_t n);
		int size();
		void updateSize(int n);
		string toString();
};
#endif // BITSET_H