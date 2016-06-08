#include "Hopcroft_Karp.h"

// generate g;

Hopcroft_Karp::Hopcroft_Karp(int nsize, int msize)
{
	n = nsize;
	m = msize;
	g.resize(n+1);
	l = new int(n+1);
	r = new int(m+1);
	vis = new bool(n+1);
	fill(l,l+n+1,0);
	fill(r,r+m+1,0);
	e = 0;
}

void Hopcroft_Karp::add_edge(int a, int b)
{
	g[a].push_back(b);
}

bool Hopcroft_Karp::dfs(int v)
{
	if(vis[v]) return false;
	vis[v] = true;
	for(uint32_t u=0; u<g[v].size(); ++u)
	{
		if(!r[g[v][u]])
		{
			l[v]=g[v][u];
			r[g[v][u]]=v;
			return true;
		}
	}
	for(uint32_t u=0; u<g[v].size(); ++u)
	{
		if(dfs(r[g[v][u]]))
		{
			l[v]=g[v][u];
			r[g[v][u]]=v;
			return true;
		}
	}
	return false;
}

vector<pair<int,int> > Hopcroft_Karp::matching()
{ 
	bool change = true;
	while(change)
	{
		change = false;
		fill(vis, vis+n+1, false);
		for(int i=1; i<=n; ++i)
			if(!l[i])
				change|= dfs(i);
	}
	vector<pair<int,int> > ret;
	for(int i=1;i<=n;++i)
	{
		if(l[i]) ret.push_back(make_pair(i,l[i]));
	}	
	return ret;
}

Hopcroft_Karp::~Hopcroft_Karp()
{
	delete r;
	delete l;
	delete vis;
	g.clear();
}