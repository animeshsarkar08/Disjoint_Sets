#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 20

typedef struct BTnode {
    int value;
    bool Ltag,Rtag;
    struct BTnode *left,*right;
}BTnode;

typedef enum Rmvtag { DEFAULT, LEFT, RIGHT} Rmvtag;
BTnode *createnode(int num);
bool insert(BTnode **root,int num);
bool delete(BTnode **root,int num);
void printtree(BTnode *root);
void dealloc(BTnode **root);
BTnode *successor(BTnode *node);
BTnode *predecessor(BTnode *node);
BTnode *prenodesuccr(BTnode *node);

int main (int argc , char argv[]){
    BTnode *start = NULL;
    insert(&start,20);
    insert(&start,10);
    insert(&start,30);
    insert(&start,5);
    insert(&start,15);
    insert(&start,25);
    insert(&start,40);
	insert(&start,6);
    printf("%d",delete(&start, 10));
    printtree(start);
    dealloc(&start);
	return 0;
}

BTnode *successor(BTnode *node){
   /* BTnode *position = node;
    position = position->right;
    while(!position->Ltag )
        position = position->left;
    return position;*/
    if(!node || !node->right) return NULL;
    node = node->right;
    while(!node->Ltag )
        node = node->left;
    return node;
}

BTnode *predecessor(BTnode *node){
    if(!node || !node->left) return NULL;
    node = node->left;
    while(!node->Rtag )
        node = node->right;
    return node;
}
BTnode *prenodesuccr(BTnode *node){
    BTnode *prev = NULL;
    if(!node || !node->right) return NULL;
    prev = node;
    node = node->right;
    while(!node->Ltag ){
        prev = node;
        node = node->left;
    }
    return prev;
}

BTnode *createnode(int num){
    BTnode *new = malloc(sizeof(*new));
    new->value = num;
    new->Ltag = new->Rtag = true;
    new->left = new->right = NULL;
    return new;
}

bool insert(BTnode **root,int num){
    if(!(*root)){
        *root = createnode(num);
        return true;
    }
    BTnode *cur = *root;
    while(cur->value != num){
        if(num < cur->value){
            if(cur->Ltag ){
                BTnode *aux = cur->left;
                cur->left = createnode(num);
                cur->left->left = aux;
                cur->left->right = cur;
                cur->Ltag = false;
                return true;
            }
            cur = cur->left;
        }
        else{
            if(cur->Rtag){
                BTnode *aux = cur->right;
                cur->right= createnode(num);
                cur->right->right = aux;
                cur->right->left = cur;
                cur->Rtag = false;
                return true;
            }
            cur = cur->right;
        }
    }
    printf("duplicate entry\n");
    return false;
}

void printtree(BTnode *root){
    //recursive approach
    /*if(!root){
        return;
    }
    printf("%d ",root->value);
    printtree(root->left);
    printtree(root->right);*/
    
    //non-recursive approach
    /*BTnode **stack = malloc(8*sizeof(BTnode *));
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
        if(i != -1)
            break;
        //root = *(stack + i-1);
         root = stack[i];
         i--;
         root= root->right;
        }
    }
    free(stack);*/
    // threaded 
    if(!root ) {
        printf("empty\n");
        return;
    }
    BTnode *cur = root;
    while(!cur->Rtag){
        cur = cur->right;
    }
    while(cur){
        printf("%d ",cur->value);
        if(cur->Ltag)
            cur = cur->left;
        else {
            //cur = successor(cur);
            cur = cur->left;
            while(!cur->Rtag )
               cur = cur->right;
        }
    }
    
}
bool delete(BTnode **root,int num){
    BTnode *parent = NULL, *cur = *root;

    //this tag indicates on which side of the parent node is the removing node
    Rmvtag tag = DEFAULT;

    while(cur->value != num ){
        tag = DEFAULT;
        parent = cur;
        if(num < cur->value ){
            cur = cur->left;
            tag = LEFT;
            printf("%d ",LEFT);
            
        }
        else{
            cur = cur->right;
            tag = RIGHT;
            printf("%d ",RIGHT);
           
        }
    }
    if(!cur) {
        tag = DEFAULT;
        printf("element not found\n");
        return false;
    }
    //removing node having no child
    if(cur->Ltag && cur->Rtag){
        if(tag == LEFT){
            //removing the left child
            parent->left = cur->left;
            parent->Ltag = true;
            free(cur);
            printf("left no ");
            return true;
        }
        if(tag == RIGHT){
            //removing the right child
            parent->right = cur->right;
            parent->Rtag = true;
            free(cur);
            printf("right no ");
            return true;
        }
    }
    // removing node having a right child
    if(!(cur->Rtag) && cur->Ltag){
        BTnode *succr = NULL;
       if(tag == LEFT){
            parent->left = cur->right;
            succr = successor(cur);
            succr->left = cur->left;
            free(cur);
            printf("left 1");
            return true;
       }
       if(tag == RIGHT){
            parent->right = cur->right;
            succr = successor(cur);
            succr->left = cur->left;
            free(cur);
            printf(" left 1 1 ");
            return true;
       }
    }
    // removing node having a left child
    if(cur->Rtag && !(cur->Ltag)){
        BTnode *predcr = NULL;
        if(tag == LEFT){
            parent->left = cur->left;
            predcr = predecessor(cur);
            predcr->right = cur->right;
            free(cur);
             printf("right 1 ");
            return true;
        }
        if(tag == RIGHT){
            parent->right = cur->left;
            predcr = predecessor(cur);
            predcr->right = cur->right;
            free(cur);
             printf("right 2");
            return true;
        }
    }
    // removing child having two children
    if(!(cur->Ltag) && !(cur->Rtag) ){
        BTnode *pre = NULL, *succr = NULL;
        pre = prenodesuccr(cur);
        if(pre == cur){
            succr = pre->right;
            if(tag == LEFT){
                parent->left = succr;
            }
            if(tag == RIGHT){
                parent->right = succr;
            }
            succr->left = cur->left;
            if(cur->Ltag){ 
                succr->Ltag = true;
            }
            else{
                succr->Ltag = false;
            }
            BTnode *cur_predcr = predecessor(cur);
			cur_predcr->right = succr;
            free(cur);
            printf("right9 ");
            return true;
        }
        // doubt
        else{
            succr = pre->left;
            cur->value = succr->value;
            if(succr->Rtag){
                pre->left = succr->left;
                pre->Ltag = true;
                free(succr);
                printf("right 6 ");
                return true;
            }
            pre->left = succr->right;
            if(succr->right->Ltag) {
                succr->right->left = succr->left;
            }
            else{
               BTnode *succr_succr = successor(succr);
                succr_succr->left = succr->left;
            }
            free(succr);
             printf("right 4 ");
            return true;
        }
    }
    return false;
}
void dealloc(BTnode **root){
   
    // NULL checker
   /* if(!(*root)) return;
    BTnode *prev = *root;
    dealloc(&((*root)->left));
    dealloc(&((*root)->right));

    //base case
    if((*root)->Ltag && (*root)->Rtag){
        free(*root);
        *root = NULL;
        printf("freed ");
        return;
    } */
     // threaded 
    if(!root ) {
        printf("empty\n");
        return;
    }
    BTnode *cur = *root;
    while(!cur->Ltag){
        cur = cur->left;
    }
    while(cur){
        BTnode *rmv = cur;
        if(cur->Rtag)
            cur = cur->right;
        else {
            cur = successor(cur);
        }
        free(rmv);
        printf("a ");
    }
    *root = NULL;
       
}
