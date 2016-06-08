#include "AVL.h"


void AVL::insert(int key, int value){
	root = root->insert(root, key, value);
}


pair<bool, int> AVL::find(int key){
	return root->find(root, key);
}


void AVL::remove(int key){
	root = root->remove(root, key);
}

void AVL::print(){
	root->preOrder(root);
	cout<<endl;
	root->inOrder(root);
	cout<<endl;
	root->postOrder(root);
	cout<<endl;
}