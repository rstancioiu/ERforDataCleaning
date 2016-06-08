#include "Reader.h"
#include "Bitset.h"
#include "Mapping.h"
#include "Util.h"
#include "UIND.h"
#include "AVL.h"

const char* data_r;
const char* data_s;

void Test();
vvi r,s;

int main(int args, char* argv[]){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	data_r = argv[1];
	data_s = argv[2];
	Test();

	return 0;
}

void Test(){
	Reader* reader = new Reader();
	pair<vvi,vvi> prs = reader->readFiles(data_r,data_s);
	delete reader;
	Mapping* mapping = new Mapping();
	vector<Bitset> bitsets = mapping->mapDataToBitsets(prs.first,prs.second);
	sort(bitsets.begin(),bitsets.end());
	delete mapping;
	for(uint32_t i=0;i<bitsets.size();++i){
		cout<<bitsets[i].toString()<<endl;
	}
	cout<<endl;

	UIND* uind = new UIND();
	vector<vector<double> > supports = uind->computeSupports(bitsets);
	for(uint32_t i=0; i<supports.size(); ++i){
		for(uint32_t j=0; j< supports[i].size();++j){
			cout<<fixed<<setprecision(2)<<supports[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	delete uind;
}




