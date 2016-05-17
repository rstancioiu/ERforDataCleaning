#include <fstream>
#include <iostream>
#define NR 100000
#define NS 1000000
#define MR 10
#define MS 15

using namespace std;

// Generates a random database with N tuples and M attributes
// the elements of the database are integers
void generate(const char* str,unsigned int N,unsigned int M) {
	ofstream output;
	output.open(str);
	for(unsigned int i=0;i<N;++i){
		for(unsigned int j=0;j<M;++j){
			output<<rand()%N<<" ";
		}
		output<<endl;
	}
	output.close();
	output.clear();
}


int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	generate("r-large.txt",NR,MR);
	generate("s-large.txt",NS,MS);
	return 0;
}