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
bool insert(BTnode **root,int num);
bool delete(BTnode **root,int num);
void printtree(BTnode *root);
void dealloc(BTnode **root);
BTnode *presuccessor(BTnode *node);
BTnode *prepredecessor(BTnode *node);
int numsuccessor(BTnode *node);
int numpredecessor(BTnode *node);
bool leaf(BTnode *node);
BTnode *rotateright(BTnode *node);
BTnode *rotateleft(BTnode *node);
BTnode *rotateleftright(BTnode *node);
BTnode *rotaterightleft(BTnode *node);
void preordertree(BTnode *node);
void inordertree(BTnode *node);
void balancetree(BTnode **root,BTnode *great,BTnode *grand,BTnode *parent,BTnode *cur);
BTnode *recrinsert(BTnode *root,BTnode *parent,int num);
void balance_deleted(BTnode *great,BTnode *grand,BTnode *parent,int num);

int main (int argc , char *argv[]){

    BTnode *start = NULL;
			
	int i= 1;
	printf("Enter:\n");
	while(i){
		puts("0.Quit:");
		puts("1.Insert:");
		puts("2.Delete:");
		puts("3.Print:");

		scanf("%d",&i);
		getchar();
		switch(i){
			case 0:
				return 0;
			case 1:

				puts("Enter the number:");
				scanf("%d",&i);
				getchar();
				insert(&start,i);
				i = 1;
				break;

			case 2:
				
				puts("Enter the number:");
				scanf("%d",&i);
				getchar();
				delete(&start,i);
				i = 1;
				break;
			case 3:

				preordertree(start);
				puts("\n");
				inordertree(start);
				puts("\n");
				break;
			default:
				 fprintf(stderr,"Invalid option number.\n");
		}
	}
	dealloc(&start);
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

BTnode *createnode(int num){
    BTnode *new = malloc(sizeof(*new));
	if(!new) return NULL;
    new->value = num;
	new->height = 0;
    new->left = new->right = NULL;
    return new;
}

bool insert(BTnode **root,int num){
    if(!(*root)){
        *root = createnode(num);
        return true;
    }
   BTnode *great= NULL;
   BTnode *grand = NULL;
	BTnode *parent = NULL;
	BTnode *cur = *root;
	while(cur->value != num){
		if(num < cur->value){
			if(!cur->left){
				bool leaftag = leaf(cur);
				cur->left = createnode(num);
				if(!cur->left){
					printf("node has node created\n");
					return false;
				}
				if(!leaftag){
					return true;
				}
				else{
					balancetree(root,great,grand,parent,cur);
					return true;
				}
		}
			else{
				great = grand;
				grand = parent;
				parent = cur;
				cur = cur->left;
			}
		}

        else{
            if(!cur->right){
                bool leaftag = leaf(cur); 
				cur->right= createnode(num);
				if(!cur->right){
					printf("node has node created\n");
					return false;
				}
				if(!leaftag){
					return true;
				}
				else{
					balancetree(root,great,grand,parent,cur);
					return true;
				}
			}
			else{
				great = grand;
				grand = parent;
				parent = cur;
				cur = cur->right;
			}
		}
	}


	printf("duplicate entry\n");
	return false;
}

void balancetree(BTnode **root,BTnode *great,BTnode *grand,BTnode *parent,BTnode *cur){
		if(parent && parent->left && !parent->right && cur->left){
			if(parent == *root)
				*root = rotateright(parent);
			else{
				if(grand->value < parent->value)
				       grand->right = rotateright(parent);
				else
					  grand->left = rotateright(parent);
				}
		}
		else if(parent && parent->right && !parent->left && cur->left){
			if(parent == *root)
			    *root = rotaterightleft(parent);
			else{
				if(grand->value < parent->value)
				       grand->right = rotaterightleft(parent);
				else
					  grand->left = rotaterightleft(parent);
			}
		}
		else if(grand && parent->right && leaf(grand->right)){
			if(grand == *root)
				*root = rotateright(grand);
			else{
				if(great->value < grand->value)
					great->right = rotateright(grand);
				else 
					great->left = rotateright(grand);						
			}
		}
		else if(parent && parent->right && !parent->left && !cur->left){
			if(parent == *root)
				*root = rotateleft(parent);
			else{
				if(grand->value < parent->value)
				       grand->right = rotateleft(parent);
				else
					  grand->left = rotateleft(parent);
			}
		}
		else if(parent && parent->left && !parent->right && !cur->left){
			if(parent == *root)
				*root = rotateleftright(parent);
			else{
				if(grand->value < parent->value)
					   grand->right = rotateleftright(parent);
				else
					  grand->left = rotateleftright(parent);
			}
		}
		else if(grand && parent->left && leaf(grand->left)){
			if(grand == *root)
				*root = rotateleft(grand);
			else{
				if(great->value < grand->value)
					great->right = rotateleft(grand);
				else 
					great->left = rotateleft(grand);
			}
		}
		else 
			return;
	
}
void preordertree(BTnode *root){
    if(!root) return;
    //non-recursive approach
    BTnode **stack = malloc(8*sizeof(BTnode *));
    int i=-1;
    BTnode *first = root, *prev = NULL;
    while(1){
        while(root){
            printf("%d ",root->value);
            i++;
            //*(stack + i) = root;
            stack[i] = root;
            root = root->left;
        }
        if(i == -1)
            break;
        //root = *(stack + i-1);
         root = stack[i];
         i--;
         root= root->right;
	}
    free(stack);
}
void inordertree(BTnode *node){
	if(!node) return;
	inordertree(node->left);
	printf("%d ",node->value);
	inordertree(node->right);
}

bool delete(BTnode **root,int num){
    BTnode *great = NULL,*grand = NULL,*parent = NULL, *cur = *root;
	int rmv;
    while(cur->value != num){
		great = grand;
		grand = parent;
        parent = cur;
	//	if(!cur) return false; 
        if(num < cur->value )
            cur = cur->left;
        else
            cur = cur->right;
    }
    if(!cur) {
        printf("element not found\n");
		return false;
	}
	//removing the node having no child
	else if(!cur->left && !cur->right){
        if(parent->value > num)
            parent->left = NULL;
		//removing the right child
		else
            parent->right = NULL;
		rmv = cur->value;
		free(cur);
		balance_deleted(great,grand,parent,rmv);
    }
    // removing node having a right child
	else if(!cur->left && cur->right){
		//cur is left of parent
       if(parent->value > num)
            parent->left = cur->right;
	   //cur is right of parent
	   else
            parent->right = cur->right;
		rmv = cur->value;
		free(cur);
		balance_deleted(great,grand,parent,rmv);
    }
    // removing node having a left child
	else if(cur->left && !cur->right){
        if(parent->value > num)
            parent->left = cur->left;
		else
            parent->right = cur->left;
		rmv = cur->value;
		free(cur);
		balance_deleted(great,grand,parent,rmv);
	}
    // removing child having two children
	else if(cur->left && cur->right ){
        BTnode *pre = NULL,*aux = NULL;
		int pre_num = numpredecessor(cur);
		int suc_num = numsuccessor(cur);
		if(pre_num == 1 && suc_num == 1){
			if(leaf(cur->left) && !leaf(cur->right)){
				aux = cur->right;
				cur->right = cur->right->right;
			}
			else if(leaf(cur->right) && !leaf(cur->left)){
				aux = cur->left;
				cur->left = cur->left->left;
			}
			else if(leaf(cur->right) && leaf(cur->left)){
				aux = cur->right;
				cur->right = cur->right->right;
			}
			else;
		}
		else if(suc_num > pre_num){
			pre = prepredecessor(cur);
			aux = pre->right; 
			pre->right = aux->left;
		}
		else{
			pre= presuccessor(cur);
			aux = pre->left;
			pre->left = aux->right;
		}
		cur->value = aux->value;
		free(aux);
    }
	else
		return false ;
	return true;
}


void balance_deleted(BTnode *great,BTnode *grand,BTnode *parent,int num){
	// if node has no child 
	if(leaf(parent)){
		if(grand->value < parent->value){
			parent = grand->left;
			if(leaf(parent)) return;
			else{
				int suc = numsuccessor(parent);
				int pre = numpredecessor(parent);
				if(suc > pre){
					if(great->value >grand->value)
						great->left = rotateleftright(grand);
					else
						great->right = rotateleftright(grand);
				}
				else{
					if(great->value > grand->value)
						great->left = rotateright(grand);
					else
						great->right = rotateright(grand);
				}
			}
		}
		else{
			parent = grand->right;
			if(leaf(parent)) return;	
			else{
				int pre = numpredecessor(parent);
				int suc = numsuccessor(parent);
				if(suc < pre){
					if(great->value >grand->value)
						great->left = rotaterightleft(grand);
					else
						great->right = rotaterightleft(grand);
				}
				else{
					if(great->value >grand->value)
						great->left = rotateleft(grand);
					else
						great->right = rotateleft(grand);
				}
			}
		}
	}
	//node has 1 child
	else if(!leaf(parent)){
			BTnode *aux = NULL;
		if(parent->value < num){
			aux = parent->left;
			if(leaf(aux))
				return;
			else {
				int suc = numsuccessor(aux);
				int pre = numpredecessor(aux);
				if(suc > pre){
					if(grand->value < parent->value)
						grand->left = rotateleftright(parent);
					else
						grand->right = rotateleftright(parent);
				}
				else{
					if(grand->value < parent->value)
						grand->left = rotateright(parent);
					else
						grand->right = rotateright(parent);
				}
			}
		}
		else{
			aux = parent->right;
			if(leaf(aux))
				return;
			else {
				int pre = numpredecessor(aux);
				int suc = numsuccessor(aux);
				if(suc < pre){
					if(grand->value < parent->value)
					    grand->left = rotaterightleft(parent);
					else
						grand->right = rotaterightleft(parent);
				}
				else{
					if(grand->value < parent->value)
						grand->left = rotateleft(parent);
					else 
						grand->right = rotateleft(parent);
				}
			}
		}
	}
}

void dealloc(BTnode **root){
     if(!(*root)) return;
    dealloc(&((*root)->left));
    dealloc(&((*root)->right));

    if(!((*root)->left) && !((*root)->right)){
        free(*root);
        *root = NULL;
        printf("freed ");
        return;
    } 
}

