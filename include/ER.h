// ER.h
#ifndef ER_H
#define ER_H
#include "Util.h"

class ER{	
	public:
		pair<vs, vs> left_side;
		pair<vs, vs> right_side;
		vector<vector<string> > pattern_left_side;
		vector<vector<string> > pattern_right_side;
	
	public:
		ER(){};
};
#endif // ER_H