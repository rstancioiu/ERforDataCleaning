#include "Bitset.h"

// Constructor of the class Bitset
Bitset::Bitset(vector<bool>& new_bitset){
	tab = new_bitset;
}

// Constructor copy of the class Bitset
Bitset::Bitset(const Bitset& b){
	tab = b.tab;
}

// Returns the result of the AND operation between two bitsets.
Bitset Bitset::operator&(const Bitset& b){
	Bitset res;
	res.updateSize(tab.size());
	for(uint32_t i=0;i<tab.size();++i){
		res.tab[i] = tab[i]&b.tab[i];
	}
	return res;
}

// Returns the result of the OR operation between two bitsets.
Bitset Bitset::operator|(const Bitset& b){
	Bitset res;
	res.updateSize(tab.size());
	for(uint32_t i=0;i<tab.size();++i){
		res.tab[i] = tab[i]|b.tab[i];
	}
	return res;
}

// Updates the size of a Bitset.
void Bitset::updateSize(int n){
	tab.resize(n);
}

// Transforms a Bitset into a string.
// Used to print the results of an operation.
string Bitset::toString(){
	string s="";
	for(uint32_t i=0;i<tab.size();++i){
		if(tab[i]) s+='1';
		else s+='0';
	}
	return s;
}

// Checks the value of the bit found at the position n
// in the bitset.
bool Bitset::check(uint32_t n){
	if(n>=tab.size()) return false;
	return tab[n];
}

// Returns the size of the bitset.
int Bitset::size(){
	return tab.size();
}