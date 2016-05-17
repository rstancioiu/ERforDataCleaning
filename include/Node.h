// Node.h
#ifndef NODE_H
#define NODE_H
#include "Util.h"


class Node{	
	
	int key;
	int value;
	int height;
	Node *right,*left;

	private:
		int findheight(Node* node);
		void updateheight(Node* node);
		int diff(Node* node);
		Node* rotateright(Node *node);
		Node* rotateleft(Node *node);
		Node* balance(Node *node);
		Node* findmin(Node *node);
		Node* removemin(Node *node);

	public:
		Node(int k, int val){key = k; value = val; right=left=0; height=1;};
		Node* insert(Node *node,int key, int value);
		Node* remove(Node *node, int key);
		pair<bool, int> find(Node *node, int key);
		void preOrder(Node* node);
		void inOrder(Node* node);
		void postOrder(Node* node);
};
#endif // NODE_H