// AVL.h
#ifndef AVL_H
#define AVL_H
#include "Util.h"
#include "Node.h"

class AVL{	

	Node* root;
	
	public:
		AVL(){root=0;};
		
		void insert(int key, int value);
		pair<bool, int> find(int key);
		void remove(int key);
		void print();
		~AVL(){delete root;};
};
#endif // AVL_H