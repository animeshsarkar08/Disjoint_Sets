#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int a;
    struct Node *next;
}Node;
Node *first(Node **root,int val){
     Node *new_node = malloc(sizeof(Node));
     if(new_node == NULL){
        exit(4);
     }
     new_node->a = val;
     new_node->next = *root;
     *root = new_node;
     return new_node;
 }
Node *end(Node **root,int n){
    Node *new_node=malloc(sizeof(Node));
    if(new_node == NULL){
        exit(7);
    }
    new_node->a = n;
    new_node->next = NULL;
    if(*root == NULL){
        *root = new_node;
        return new_node;
    }
    Node *cur = *root;
    while(cur->next != NULL){
        cur = cur->next;
    } 
    cur->next = new_node;
    return new_node;
}
Node *addnode(Node *n,int val){
     Node *new_node = malloc(sizeof(Node));
     if(new_node == NULL) exit(5);
     new_node->a = val;
     new_node->next = n->next;
     n->next = new_node;
     return new_node;
 }


void frnode(Node **root){
    Node *cur = *root;
    Node *n = NULL;
  /* while(cur != NULL){
        if(cur->next == NULL){
            free(cur);
            n->next = NULL;
            cur = *root;
        }
        n = cur;
        cur = cur->next;
    }*/
   while(cur != NULL){
        n=cur;
        cur=cur->next;
        free(n);
    }
    free(*root);
    *root = NULL;
}
void print_node(Node *root){
    Node *cur = root;
    if(cur == NULL){
        printf("empty");return;
    }

    while(cur != NULL){
        printf("%d\n",cur->a);
        cur = cur->next;
    }
}
void sort_node(Node **root,int val){
    Node *new_node = malloc(sizeof(Node));
    if(new_node == NULL) exit(7);
    Node *cur = *root;
    new_node->a = val;
    if((cur->a) >= val || *root == NULL){
        new_node->next = *root;
        *root = new_node; 
        return;
    }
    while(cur->next != NULL){
        if(cur->next->a >= val){
           // addnode(cur,val);return;
           break;
        }
        cur = cur->next;
    }
    addnode(cur,val);
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
        addnode(cur,val);return;
   }
   else {
       printf("not found\n");
       return;
   }
}
void loop_node(Node *root){
    Node *cur = root;
    Node *ptr = root;
    //my code (lengthy)
    /*while(cur->next != NULL){
        while(ptr != cur){
            if(ptr == NULL) return ;
            if(ptr != cur->next){
                ptr = ptr->next;
            }
            else {
                printf("loop");
                return;
            }
        }
        ptr = root;
        cur = cur->next;
    }*/
    while(cur != NULL && ptr != NULL && ptr->next != NULL){
        cur=cur->next;
        ptr = ptr->next->next;
        if(cur == ptr){
            printf("loop");
           return;
        }
    }

}
void insert_at(Node **root,int val,int index){
    Node *cur = *root;
    if(index == 0) {
        first(root,val);
        return;
    }
    for(int i = 0;i < (index -1);i++){
        cur = cur->next;
    }
    addnode(cur,val);
}
int rmv_elm(Node **root,int n){
     if(*root == NULL){
         printf("empty");return 1;
     }

     Node *cur = *root;
     if(cur->a == n){
         Node *rmv = cur;
         cur= cur->next;
         free(rmv);
         *root = cur;
         return 0;
     }
     while( cur->next != *root){
         if(cur->next->a == n){
             Node *rmv = cur->next;
             cur->next = cur->next->next;
             free(rmv);return 0;
         }
         cur = cur->next;
     }

 }

int main(){
    Node *r= NULL;
    Node *ptr = first(&r,4);
    ptr = addnode(ptr,5);
    ptr = addnode(ptr,3);
    ptr = addnode(ptr,8);
    ptr = addnode(ptr,9);
    ptr = end(&r,10);
    insert_at(&r,67,3);
 // ptr->next = r->next->next;
    loop_node(r); 
    rmv_elm(&r,4);
  print_node(r);
    frnode(&r);
    return 0;

}