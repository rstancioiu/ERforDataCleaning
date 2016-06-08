// CFD.h
#ifndef CFD_H
#define CFD_H
#include "Util.h"

class CFD{	
	public:
		vector<bool> left_side;
		vector<bool> right_side;
		vector<vector<string> > pattern_left_side;
		vector<vector<string> > pattern_right_side;
	
	public:
		CFD(){};
};
#endif // CFD_H