// Closure.h
#ifndef UIND_H
#define UIND_H
#include "Bitset.h"
#include "Util.h"

class UIND{	

	public:
		UIND(){};
		vector<Bitset> computeClosure(vector<Bitset>& tab);
		vector<vector<double> > computeSupports(vector<Bitset>& tab);
		vector<vector<double> > computeSupports(vector<Bitset>& tab, double error);
};
#endif // UIND_H