
#ifndef HOPCROFT_KARP_H
#define HOPCROFT_KARP_H
#include "Util.h"

class Hopcroft_Karp{	
	private:
		bool dfs(int v);
	
	public:
		Hopcroft_Karp(int nsize, int msize);
		~Hopcroft_Karp();
		vector<pair<int,int> > matching();
		void add_edge(int a,int b);

	public:
		vector<vector<int> > g;
		int* r;
		int* l;
		bool* vis;
		int n,m,e;
};
#endif // HOPCROFT_KARP_H
