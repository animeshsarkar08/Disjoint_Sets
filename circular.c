#include<stdio.h>
#include<stdlib.h>

typedef struct Node {
    int a;
    struct Node *next;
}Node;

Node *addnode(Node *ptr,int n);
void dealloc(Node **root);
void print_node(Node *root);
void loop_node(Node *root);
void after_num (Node *root,int aft,int val);
int rmv_elm(Node **root,int n);

int main(){
     Node *root = NULL;
     root = addnode(root,5);
     root = addnode(root,4);
     root = addnode(root,74);
     root = addnode(root,82);
     root = addnode(root,65);
     root = addnode(root,30);
     root = addnode(root,27);
    if(!rmv_elm(&root,27)) printf("remove\n");
     print_node(root);
     dealloc(&root);   
}

int rmv_elm(Node **root,int n){
     if(*root == NULL){
         printf("empty");return 1;
     }
     Node *cur = *root;
     while( cur->next != *root){
         if(cur->next->a == n){
             Node *rmv = cur->next;
             cur->next = cur->next->next;
             free(rmv);return 0;
         }
         cur = cur->next;
     }
     if(cur->next->a == n){
         Node *rmv = cur->next;
        cur->next = cur->next->next;
        free(rmv);
        *root = cur;
        return 0;
     }
     else {
         printf("element not found");return 1;
     }

}
Node *addnode(Node *ptr,int n){
    Node *new = malloc(sizeof(*new));
    if( new == NULL)    exit(1);
    new->a = n;
    if(ptr == NULL){
        new->next = new;
        return new;
    }
    new->next = ptr->next;
    ptr->next = new;
    return new;
}
void dealloc(Node **root){
    Node *cur = *root;
    do{
        Node *aux = cur;
        cur = cur->next;
        free(aux);
    }while(cur != *root);
    *root = NULL;return;
}
void print_node(Node *root){
    Node *cur = root;
    do{
        printf("%d\n",cur->a);
        cur = cur->next;
    }while(cur != root);
    return;
}
void loop_node(Node *root){
    Node *cur = root;
    Node *ptr = root;
    while(cur != NULL && ptr != NULL && ptr->next != NULL){
        cur=cur->next;
        ptr = ptr->next->next;
        if(cur == ptr){
            printf("loop");
           return;
        }
    }

}
void after_num (Node *root,int aft,int val){
    if(root == NULL) {
        printf("empty");
        return;
    }
    Node *cur = root;
   while(cur->next != NULL){
       if(cur->a == aft){
           Node *new_node = malloc(sizeof(Node));
           new_node->a = val;
           new_node->next = cur->next;
           cur->next = new_node;return;
       }
       cur = cur->next;
   }
   if(cur->a == aft){
        Node *new_node = malloc(sizeof(Node));
        if(new_node == NULL) exit(5);
         new_node->a = val;
         new_node->next = cur->next;
         cur->next = new_node;
        return;
   }
   else {
       printf("not found\n");
       return;
   }
}

        
    
