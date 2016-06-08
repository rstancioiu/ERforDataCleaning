#include "Bitset.h"

Bitset::Bitset(){
	tab.resize(256);
	fill(tab.begin(),tab.end(),false);
}

Bitset::Bitset(uint32_t V){
	tab.resize(256);
	fill(tab.begin(),tab.end(),false);
	tab[V]=1;
}

Bitset::~Bitset() {
	tab.clear();
}

// Constructor of the class Bitset
Bitset::Bitset(vector<bool>& new_bitset) {
	tab = new_bitset;
}

// Constructor copy of the class Bitset
Bitset::Bitset(const Bitset& b) {
	tab = b.tab;
}

void Bitset::init(){
	tab.resize(GetMaxAttribute());
	fill(tab.begin(),tab.end(),0);
}

void Bitset::clear(){
	fill(tab.begin(),tab.end(),0);	
}

bool Bitset::empty() const {
	for(uint32_t i=0;i<tab.size();++i)
		if(tab[i]) return false;
	return true;
}

bool Bitset::contain(uint32_t x) const {
	return tab[x];
} 

void Bitset::erase(uint32_t x) {
	tab[x]=0;
}

void Bitset::insert(uint32_t x) {
	tab[x]=1;
}

uint32_t Bitset::size() const{
	uint32_t cnt=0;
	for(uint32_t i=0; i<tab.size();++i)
		if(tab[i])
			cnt++;
	return cnt;
}

Bitset& Bitset::operator&=(const Bitset& b){
	for(uint32_t i=0;i<GetMaxAttribute();++i){
		tab[i] =tab[i]&b.tab[i];
	}
	return *this;
}

Bitset& Bitset::operator|=(const Bitset& b){
	for(uint32_t i=0;i<GetMaxAttribute();++i){
		tab[i] = tab[i]|b.tab[i];
	}
	return *this;
}

Bitset& Bitset::operator-=(const Bitset& b){
	for(uint32_t i=0;i<tab.size();++i){
		tab[i] = tab[i]&(~b.tab[i]);
	}

	return *this;
}

// Returns the result of the AND operation between two bitsets.
Bitset Bitset::operator&(const Bitset& b) const{
	Bitset res(*this);
	res&=b;
	return res;
}

// Returns the result of the OR operation between two bitsets.
Bitset Bitset::operator|(const Bitset& b) const{
	Bitset res(*this);
	res|=b;
	return res;
}

Bitset Bitset::operator-(const Bitset& b) const{
	Bitset res(*this);
	res-=b;
	return res;
}

bool Bitset::operator<(const Bitset& b) const{
	unsigned int s1 = size(), s2 = b.size();
	if(s1<s2)
		return true;
	if(s2<s1)
		return false;
	for(uint32_t i=0;i<tab.size();++i){
		if(tab[i]!=b.tab[i]){
			return tab[i]<b.tab[i];
		}
	}
	return false;	
}

bool Bitset::operator>(const Bitset& b) const{
	unsigned int s1 = size(), s2 = b.size();
	if(s1<s2)
		return false;
	if(s2<s1)
		return true;
	for(uint32_t i=0;i<tab.size();++i){
		if(tab[i]!=b.tab[i]){
			return tab[i]>b.tab[i];
		}
	}
	return false;
}


// In fact, it is the inclusion operators...
bool Bitset::operator>=(const Bitset &bs) const
{
	for(unsigned int i=0;i< GetMaxAttribute();++i)
        if((tab[i] & bs.tab[i]) != bs.tab[i])
            return false;
    return true;
}

bool Bitset::operator<=(const Bitset &bs) const
{
	for(unsigned int i=0;i< GetMaxAttribute();++i)
       	if((tab[i] & bs.tab[i]) != tab[i])
            return false;
    return true;
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
int Bitset::bitsetSize(){
	return tab.size();
}


bool Bitset::has_same_begin( const Bitset &attr ) const // Les 2 attr ont le meme debut
{
    unsigned int a;
        // On cherche les premiers 2 bits differents (attributs differents)
    for ( a = 0; a < tab.size(); a++ )
        if ( contain( a ) != attr.contain( a ) )
            break;

        // On est sur 2 bits differents
    unsigned int j;
        // On verifie qu'il n'y a plus qu'un seul bit a 1
    for ( j = a; j < tab.size() && !contain( j ) ; j++ );
    for ( j++  ; j < tab.size() && !contain( j ) ; j++ );

        // On a au moins 2 bits a 1 donc erreur
    if ( j < tab.size() )
        return false;

        // On verifie qu'il n'y a plus qu'un seul bit a 1
    for ( j = a; j < tab.size() && !attr.contain( j ) ; j++ );
    for ( j++  ; j < tab.size() && !attr.contain( j ) ; j++ );

        // On a au moins 2 bits a 1 donc erreur
    if ( j < tab.size() )
        return false;

    return j == tab.size();
}


bool Bitset::operator==(const Bitset &bs) const
{
	return tab == bs.tab;
}

bool Bitset::operator!=(const Bitset &bs) const
{
	return tab != bs.tab;
}