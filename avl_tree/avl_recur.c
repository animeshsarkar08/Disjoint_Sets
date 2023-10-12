#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 20

typedef struct BTnode {
    int value;
    struct BTnode *left,*right;
	int height;
}BTnode;

BTnode *createnode(int num);
BTnode *insert(BTnode *root,BTnode *parent,int num);
BTnode *delete(BTnode *root,BTnode *parent,int num);
bool leaf(BTnode *node);
BTnode *rotateright(BTnode *node);
BTnode *rotateleft(BTnode *node);
BTnode *rotateleftright(BTnode *node);
BTnode *rotaterightleft(BTnode *node);
void preordertree(BTnode *node);
void inordertree(BTnode *node);
int Height(BTnode *node);
int numsuccessor(BTnode *node);
int numpredecessor(BTnode *node);
BTnode *presuccessor(BTnode *node);
BTnode *prepredecessor(BTnode *node);
void dealloc(BTnode **root);

int main (int argc , char *argv[]){
    BTnode *r = NULL;
	r = insert(r,NULL,6);
	r = delete(r,NULL,6);
	dealloc(&r);
	return 0;
}

int Height(BTnode *node){
	if(!node) return -1;
	return node->height;
}

bool leaf(BTnode *node){
	if(!(node)) return false;
	if(!(node->left) && !(node->right))
		return true;
	else
		return false;
}

int numsuccessor(BTnode *node){
    if(!node || !node->right) return 0;
    node = node->right;
	int i = 1;
    while(!node){
        node = node->left;
		i++;
	}
    return i;
}

int numpredecessor(BTnode *node){
    if(!node || !node->left) return 0;
    node = node->left;
	int i = 1;
    while(!node){
        node = node->right;
		i++;
	}
    return i;
}
BTnode *prepredecessor(BTnode *node){
	BTnode *prev = NULL;
    if(!node || !node->left) return NULL;
	prev = node;
    node = node->left;
    while(node->right){
		prev = node;
        node = node->right;
	}
    return prev;
}
BTnode *presuccessor(BTnode *node){
    BTnode *prev = NULL;
    if(!node || !node->right) return NULL;
    prev = node;
    node = node->right;
    while(node->left ){
        prev = node;
        node = node->left;
    }
    return prev;
}
BTnode *rotateright(BTnode *node){
	if(!node) return NULL;
	//BTnode *node = *root;
	BTnode *parent = node->left;
	node->left = parent->right;
	parent->right = node;
	//*root = parent;
	return parent;
}

BTnode *rotateleft(BTnode *node){
	if(!(node)) return NULL;
	//BTnode *node = *root;
	BTnode *parent = node->right;
	node->right = parent->left;
	parent->left = node;
	//*root = parent;
	return parent;
}

BTnode *rotateleftright(BTnode *node){
	node->left = rotateleft(node->left);
	return rotateright(node);
}


BTnode *rotaterightleft(BTnode *node){
	 node->right = rotateright(node->right);
	 return rotateleft(node);
}

BTnode *createnode(int num){
    BTnode *new = malloc(sizeof(*new));
	if(!new) return NULL;
    new->value = num;
	new->height = 0;
    new->left = new->right = NULL;
    return new;
}

void preordertree(BTnode *root){
    if(!root){
        return;
	}
	printf("%d height %d\n",root->value,root->height);
    preordertree(root->left);
    preordertree(root->right);
}

void inordertree(BTnode *node){
	if(!node) return;
	inordertree(node->left);
	printf("%d ",node->value);
	inordertree(node->right);
}

BTnode *delete(BTnode *root,BTnode *parent,int num){
	if(!root){
		fputs(stderr,"Node not found.");
		return false;
	}
	else if(num < root->value ){
		root = delete(root->left,root,num);
		int factor = Height(root->right) - Height(root->left);
	    if(factor == 2){
			if(num < root->right->value)
				root = rotateleft(root);
			else 
				root = rotaterightleft(root);
		}
	}
	else if(num > root->value){
        root = delete(root->right,root,num);
		int factor = Height((root)->left) - Height((root)->right); 
		if(factor == 2){
			if(num < root->left->value)
				root = rotateright(root);
			else 
				root = rotateleftright(root);
		}
	}
    
	//removing a node with no child
    if(!root->left && !root->right){
		//if we removing the root node 
		if(!parent)
		   parent = NULL;

		//removing the left child
		else if(parent->value > num)
			parent->left = NULL;
		//removing the right child
		else
			parent->right = NULL;
		free(root);
	}
	
	//removing a node having one child

	// removing a node having a right child
	else if(!root->left && root->right){

		//if we removing the root node 
		if(!parent)
			parent = root->right;

	   //cur is left of parent
		else if(parent->value > num)
	        parent->left = root->right;
	   //cur is right of parent
		else
			parent->right = root->right;

		free(root);
	}
	// removing node having a left child
	else if(root->left && !root->right){
		//if we removing the root node 
		if(!parent)
			parent = root->left;
		if(parent->value > num)
			parent->left = root->left;
		else
			parent->right = root->left;
	
		free(root);
	}
	
		// removing child having two children
	else if(root->left && root->right ){
		BTnode *pre = NULL,*aux = NULL;
		int pre_num = numpredecessor(root);
		int suc_num = numsuccessor(root);
		if(pre_num == 1 && suc_num == 1){
			if(leaf(root->left) && !leaf(root->right)){
				aux = root->right;
				root->right = root->right->right;
			}
			else if(leaf(root->right) && !leaf(root->left)){
				aux = root->left;
				root->left = root->left->left;
			}
			else if(leaf(root->right) && leaf(root->left)){
				aux = root->right;
				root->right = root->right->right;
			}
			else;
		}
		else if(suc_num > pre_num){
			pre = prepredecessor(root);
			aux = pre->right; 
			pre->right = aux->left;
		}
		else{
			pre= presuccessor(root);
			aux = pre->left;
			pre->left = aux->right;
		}
		root->value = aux->value;
		parent = pre;
		free(aux);
	}

	if(Height(parent->left) > Height(parent->right))
	     parent->height = Height(parent->left) + 1;
	else
		parent->height = Height(parent->right) + 1;

	return parent;
}

void dealloc(BTnode **root){
     if(!(*root)) return;
    dealloc(&((*root)->left));
    dealloc(&((*root)->right));

    if(!((*root)->left) && !((*root)->right)){
        free(*root);
        *root = NULL;
        return;
    } 
}

BTnode *insert(BTnode *root,BTnode *parent,int num){
	if(!(root)){
		root = createnode(num);
		if(!(root)) return NULL;
	}
	else if(num < (root)->value){
		(root)->left = recrinsert(root->left,root,num);
		int factor = Height((root)->left) - Height((root)->right); 
		if(factor == 2){
			if(num < root->left->value)
				root = rotateright(root);
			else 
				root = rotateleftright(root);
		}
	}
	else if(num > root->value){
	root->right = recrinsert(root->right,root,num);
		int factor = Height((root)->right) -  Height((root)->left);  
		if(factor == 2){
			if(num < root->right->value)
				root = rotateleft(root);
			else 
				root = rotaterightleft(root);
		}
	}
    if(Height((root)->left) > Height((root)->right))
		(root)->height = Height((root)->left) + 1;
	else if(Height((root)->left) < Height((root)->right))
		(root)->height = Height((root)->right) + 1;
	else 
		(root)->height = Height((root)->left) + 1;
	return root;
}
