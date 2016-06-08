#include "Reader.h"
#include "Bitset.h"
#include "Mapping.h"
#include "Util.h"
#include "UIND.h"
#include "AVL.h"
#include "AlgoCFUN.h"
#include "PartitionCFUN.h"
#include "Relation.h"
#include "Hopcroft_Karp.h"
#include "CFD.h"
#include "ER.h"

using namespace std;

const char* data_r;
const char* data_s;
vvi r,s;
uint32_t n,m;
void computeCFDs();
void computeMapping();
void computeERs();
void printERs();

map<int, int> finv;
vector<CFD> cfds;
vector<ER> eRs;
vs colr,cols;

int main(int argc, char* argv[] ){
	data_r = argv[1];
	data_s = argv[2];
	computeMapping();
	computeCFDs();
	computeERs();
	return 0;
}


void computeCFDs(){
	PartitionCFUN::m_uMinSup = 1;
    LevelCFUN::m_uMinSupCFD  = 1;

    char _cDelim       = '\0';
	
	bool _bPrune       = false;

	Relation relation;
	if ( ! relation.loadRelation( data_s, _cDelim ) ) {
		cerr << "Open file error: " << data_s << endl;
	}

    cout << "CFun on " << data_s
            << "\n\t#Tuples    : " << relation.getNbTuples()
            << "\n\t#Attributes: " << relation.getNbCol()
            << "\n\tminSup     : " << PartitionCFUN::m_uMinSup
			<< "\n\tminCFD     : " << LevelCFUN::m_uMinSupCFD
            << "\n\tPruning    : " << (_bPrune ? "ON" : "OFF")
            << endl << endl;
            
    cfds = cfun ( relation, 2, _bPrune);

    cout<<endl<<endl<<endl;
    cout<<"--------------------------------------------------"<<endl;
    cout<<"        CONDITIONAL FUNCTIONAL DEPENDENCIES       "<<endl;

    for(uint32_t i=0;i<cfds.size();++i)
	{
		cout<<"--------------------------------------------------"<<endl;
		for(uint32_t j = 0; j < cfds[i].left_side.size(); ++j)
		{
			if(cfds[i].left_side[j]) cout<<cols[j];
		}
		cout<<" -> ";
		for(uint32_t j = 0; j < cfds[i].right_side.size(); ++j)
		{
			if(cfds[i].right_side[j]) cout<<cols[j];
		}
		cout<<endl;
		cout<<"Patterns: "<<endl;
		for(uint32_t j = 0; j < cfds[i].pattern_left_side.size(); ++j)
		{
		    for(uint32_t k = 0; k < cfds[i].pattern_left_side[j].size(); ++k)
	        {
	            cout<< cfds[i].pattern_left_side[j][k]<<" ";
	        }
	        cout<<"----> ";
	        for(uint32_t k = 0; k < cfds[i].pattern_right_side[j].size(); ++k)
	        {
	           cout<<cfds[i].pattern_right_side[j][k]<<" ";
	       	}
		    cout<<endl;
		}
	}
	cout<<"--------------------------------------------------"<<endl;
}

void computeMapping(){
	Reader* reader = new Reader();
	pair< pair<vvi,vs> , pair<vvi,vs> > prs = reader->readFiles(data_r,data_s);
	delete reader;
	colr = prs.first.second;
	cols = prs.second.second;

	Mapping* mapping = new Mapping();
	vector<Bitset> bitsets = mapping->mapDataToBitsets(prs.first.first,prs.second.first);
	sort(bitsets.begin(),bitsets.end());
	delete mapping;

	cout<<endl<<endl<<endl;
	cout<<"--------------------------------------------------"<<endl;
    cout<<"        BITSETS BETWEEN SOURCE AND MASTER         "<<endl;
    cout<<"--------------------------------------------------"<<endl;
	for(uint32_t i=0;i<bitsets.size();++i){
		cout<<bitsets[i].toString()<<endl;
	}
	cout<<endl;
	cout<<"--------------------------------------------------"<<endl;
	n = prs.first.first[0].size();
	m = prs.second.first[0].size();
	UIND* uind = new UIND();
	vector<vector<double> > supports = uind->computeSupports(bitsets, n, m);
	cout<<endl<<endl<<endl;
	cout<<"--------------------------------------------------"<<endl;
    cout<<"        MATRIX OF SUPPORTS BETWEEN R AND S        "<<endl;
    cout<<"--------------------------------------------------"<<endl;
	for(uint32_t i=0; i<supports.size(); ++i){
		for(uint32_t j=0; j< supports[i].size();++j){
			cout<<fixed<<setprecision(2)<<supports[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	cout<<"--------------------------------------------------"<<endl;
	delete uind;

	Hopcroft_Karp* match = new Hopcroft_Karp(n, m);
	for(uint32_t i=0; i<supports.size(); ++i){
		for(uint32_t j=0; j<supports[i].size();++j){
			if(supports[i][j]>=0.8)
			{
				match->add_edge(i+1,j+1);
			}

		}
	}
	vector<pair<int,int> > f = match->matching(); 
	cout<<endl<<endl<<endl;
	cout<<"--------------------------------------------------"<<endl;
    cout<<"        RESULTS OF THE MATCHING BETWEEN R AND S   "<<endl;
    cout<<"--------------------------------------------------"<<endl;
	for(uint32_t i=0;i<f.size();++i){
		int x = f[i].first-1;
		int y = f[i].second-1;
		cout<<"f["<<prs.second.second[y]<<"] = "<<prs.first.second[x]<<endl;
		finv.insert(make_pair(y,x));
	}
	cout<<"--------------------------------------------------"<<endl;
}


void computeERs(){
	for(uint32_t i = 0; i < cfds.size(); ++i) {
		ER er;
		er.pattern_left_side = cfds[i].pattern_left_side;
		er.pattern_right_side = cfds[i].pattern_right_side;
		bool problem = false;
		for(uint32_t j=0;j<cfds[i].left_side.size();++j){
			if(!cfds[i].left_side[j])
				continue;
			if(finv.find(j)==finv.end())
			{
				problem = true;
				break;
			}
			er.left_side.second.push_back(cols[j]);
			er.left_side.first.push_back(colr[finv[j]]);
		}
		for(uint32_t j=0;j<cfds[i].right_side.size();++j){
			if(!cfds[i].right_side[j])
				continue;
			if(finv.find(j)==finv.end()) 
			{
				problem = true;
				break;
			}
			er.right_side.second.push_back(cols[j]);
			er.right_side.first.push_back(colr[finv[j]]);
		}
		if(!problem) eRs.push_back(er);
	}
	printERs();
}

void printERs(){
	cout<<endl<<endl<<endl;
	cout<<"--------------------------------------------------"<<endl;
    cout<<"        EDITING RULES FOUND                       "<<endl;
    cout<<"--------------------------------------------------"<<endl;
	for(uint32_t i = 0;i < eRs.size(); ++i)
	{
		cout<<"(";
		for(uint32_t j = 0 ; j < eRs[i].left_side.first.size(); ++j )
			cout<<eRs[i].left_side.first[j];
		cout<<",";
		for(uint32_t j = 0 ; j < eRs[i].left_side.second.size(); ++j )
			cout<<eRs[i].left_side.second[j];
		cout<<") -> (";
		for(uint32_t j = 0; j < eRs[i].right_side.first.size(); ++j)
			cout<<eRs[i].right_side.first[j];
		cout<<",";
		for(uint32_t j = 0; j < eRs[i].right_side.second.size(); ++j)
			cout<<eRs[i].right_side.second[j];
		cout<<")"<<endl;
		cout<<"Patterns: "<<endl;
		for(uint32_t j = 0; j < eRs[i].pattern_left_side.size(); ++j)
		{
		    for(uint32_t k = 0; k<eRs[i].pattern_left_side[j].size(); ++k)
	        {
	            cout<< eRs[i].pattern_left_side[j][k]<<" ";
	        }
	        cout<<"----> ";
	        for(uint32_t k = 0; k< eRs[i].pattern_right_side[j].size(); ++k)
	        {
	           cout<<eRs[i].pattern_right_side[j][k]<<" ";
	       	}
		    cout<<endl;
		}
	}
	cout<<"--------------------------------------------------"<<endl<<endl<<endl;
}