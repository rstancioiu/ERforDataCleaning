// Bitset.h
#ifndef BITSET_H
#define BITSET_H
#include "Util.h"


class Bitset{	
	private:
		static const unsigned int MAX_ATTRIBUTE = 256;

	public:
	    static unsigned int GetMaxAttribute() { return MAX_ATTRIBUTE; }

	public:
		vector<bool> tab = vector<bool>(MAX_ATTRIBUTE,0);

	public:
		Bitset();
		Bitset(uint32_t  V);
		Bitset(vector<bool>& new_bitset);
		Bitset(const Bitset &obj);
		~Bitset();
		void init();
		void clear();
		bool empty() const;
		bool contain(uint32_t x) const;
		void erase(uint32_t x);
		void insert(uint32_t i);
		uint32_t size() const;

		Bitset operator&(const Bitset& b) const;
		Bitset operator|(const Bitset& b) const;
		Bitset operator-(const Bitset& b) const;
		Bitset &operator&=(const Bitset& b);
		Bitset &operator|=(const Bitset& b);
		Bitset &operator-=(const Bitset& b);

		bool operator<(Bitset const& b) const;
		bool operator>(Bitset const& b) const;
		bool operator==(const Bitset& b) const;
		bool operator!=(const Bitset& b) const;
		bool operator>=(const Bitset& b) const;
		bool operator<=(const Bitset& b) const;
		bool has_same_begin(const Bitset& b) const;		

		bool check(uint32_t n);
		int bitsetSize();
		void updateSize(int n);
		string toString();

		static void InitMaxNbAttribute(uint32_t uMaxNbAttribute);
};

#endif // BITSET_H