#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 32

int top = -1 , first = -1;
int stack[MAX];
void push_end (int a);
int pop_end ();
void push_first (int a);
int pop_first ();



int main (int argc,char *argv[]){
    push_first(30);
    push_first(2);
    push_first(5);
    push_first(9);
    pop_first();
    for(int i = 0; i < top + 1; i++){
        printf("%d ",stack[i]);
    }
}
void push_end (int a){
    if(top == MAX -1 && first == MAX -1){
        printf("stack overflow int");
        exit(1);
    }
    top = top + 1;
    stack[top] = a;
    first = top;
}
int pop_end (){
    if(top == -1 && first == -1){
        printf("stack underflow int");
        exit(2);
    }
    int a;
    a = stack[top];
    stack[top] = 0;
    top = top - 1;
    first = top;
    return a;
}
void push_first(int a){
    if(first == MAX - 1 && top == MAX -1 ){
        printf("stack overflow\n");
        exit(2);
    }
    int i;
    first = first + 1;
    for(i = first; i > 0; i--){
        stack[i] = stack[i - 1];
    }
    stack[0] = a;
    top = first;
}
int pop_first(int a){
    if(first == -1 && top == -1){
        printf("stack underflow\n");
        exit(2);
    }
    int i,value;
    value = stack[0];
    for(i = 0; i < first; i++){
        stack[i] = stack[i + 1];
    }
    first = first - 1; 
    top = first;
    return a;
}