#include "Node.h"

int Node::findheight(Node* node){
	if(node) return node->height;
	else return 0;
}

void Node::updateheight(Node* node){
	int hl = findheight(node->left);
	int hr = findheight(node->right);
	node->height = max(hl,hr)+1;
}

int Node::diff(Node* node){
	return findheight(node->right)-findheight(node->left);
}

Node* Node::rotateright(Node *node){
	Node *q = node->left;
	node->left = q->right;
	q->right = node;
	updateheight(node);
	updateheight(q);
	return q;
}

Node* Node::rotateleft(Node *node){
	Node *q = node->right;
	node->right = q->left;
	q->left = node;
	updateheight(node);
	updateheight(q);
	return q;
}

Node* Node::balance(Node *node){
	updateheight(node);
	int difference = diff(node);
	if(difference==-2){
		if(diff(node->left)>0)
			node->left = rotateleft(node->left);	
		return rotateright(node);
	}
	else if(difference==2){
		if(diff(node->right)<0)
			node->right = rotateright(node->right);
		return rotateleft(node);
	}
	return node;
}

Node* Node::findmin(Node *node){
	if(node->left) return findmin(node->left);
	else return node; 
}

Node* Node::removemin(Node *node){
	if(!node->left){
		if(node->right)
		{
			node = node->right;
			return node;
		}
		return 0;
	} else {
		removemin(node->left);
	}
	return node;
}

Node* Node::insert(Node *node,int key, int value){
	if(!node) return new Node(key, value);
	if(key>node->key){
		node->right = insert(node->right, key, value);
	} else {
		node->left = insert(node->left, key, value);
	}
	return balance(node);
}

Node* Node::remove(Node *node, int key){
	if(!node) return 0;
	if(key>node->key){
		node->right = remove(node->right,key);
	} else if(key<node->key){
		node->left = remove(node->left,key);
	} else {
		Node* l = node->left;
		Node* r = node->right;
		delete node;
		Node* minimum = findmin(r);
		minimum->right = removemin(r);
		minimum->left = l;
		return balance(minimum);
	}
	return balance(node);
}


pair<bool, int> Node::find(Node *node, int key){
	if(!node) return make_pair(false, -1);
	if(node->key==key) return make_pair(true, node->value);
	else if(node->key<key) return find(node->right, key);
	else return find(node->left, key);
}


void Node::preOrder(Node* node){
	if(!node) return;
	cout<<node->key<<" "<<node->value<<endl;
	preOrder(node->left);
	preOrder(node->right);
}

void Node::inOrder(Node *node){
	if(!node) return;
	inOrder(node->left);
	cout<<node->key<<" "<<node->value<<endl;
	inOrder(node->right);
}

void Node::postOrder(Node* node){
	if(!node) return;
	postOrder(node->left);
	postOrder(node->right);
	cout<<node->key<<" "<<node->value<<endl;
}