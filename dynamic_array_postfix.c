#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#define MAX 20
typedef struct arrStack{
    int top, capacity,*arr;
}arrStack;

int stack_size(arrStack *s);
int optn(int a,int b,char c);
int precedence(char a);
int result(arrStack *s,char *str);
char* intopost(arrStack *s,char *str);
arrStack *Stack(int size);
int deallocate(arrStack **s);
int isFull(arrStack *s);
int isNull(arrStack *s);
void push(arrStack *s,int a);
int pop(arrStack *s);
void print_stack(arrStack *s);


int main(int argc,char *argv[]){
    arrStack *stack = Stack(10);
    char expression[MAX];
    printf("enter the expression:");
    fgets(expression,MAX,stdin);
    char *postfix = intopost(stack,expression);
    printf("result = %d",result(stack,postfix));
    deallocate(&stack);

}

arrStack*Stack(int size){
    arrStack *s = (arrStack*)malloc(sizeof(*s));
    if(!s){
        fprintf(stderr,"failed to allocate stack.\n");
        return NULL;
    }
    s->top = -1;
    s->capacity = size;
    s->arr = (int*)malloc(size * sizeof(*(s->arr)));
    if(!s->arr){
        free(s);
        fprintf(stderr,"failed to allocate stack array.\n");
        return NULL;
    }
    return s;
}
void print_stack(arrStack *s){
    if(isNull(s)){
        printf("empty\n");
        return;
    }
    int i, n =stack_size(s);
     for(i = 0; i < n  ; i++){
        printf("%d ",pop(s));
    }
    return;
}
int deallocate(arrStack **s){
    arrStack *rmv = *s;
    if(rmv){
        if(rmv->arr){
            free(rmv->arr);
            rmv->arr = NULL;
        }
        free(rmv);
        *s= NULL;
        return 1;
    }
    else{
        return 0;
    }
}
void push(arrStack *s,int a){
    if(isFull(s)){
        fprintf(stderr,"stack is full\n");
        exit(1);
    }
    (s->top)++;
    s->arr[s->top] = a;
}

int pop(arrStack *s){
    if(isNull(s)){
        fprintf(stderr,"stack is null\n");
        exit(2);
    }
    int a = s->arr[s->top];
    s->arr[s->top] = 0;
    (s->top)--;
    return a;
}
int isFull(arrStack *s){
    return s->top == (s->capacity - 1);
}
int isNull(arrStack *s){
    return s->top == -1;
}
int stack_size(arrStack *s){
    if(isNull(s)){
        return 0;
    }
    else {
        return s->top + 1;
    } 
}

char* intopost(arrStack *s,char *str){
    static char postfix[MAX*2];
    size_t size = MAX*2;
    size_t i = 0,num = 0;
    int j = 0,n = 0;
    char p,*c;
    //if user input is not taken then remove -1 below 
    while(i < (strlen(str)-1)){
        switch(str[i]){
            case '+':
            case '^':
            case '-':
            case '*':
            case '/':  while (s->top != -1 && (precedence(str[i]) != -1) && (precedence(str[i]) <= precedence(s->arr[s->top]))){
                postfix[j++] = (char)pop(s);
                size = size-1;
            }
            push(s,str[i]);break;
            case '(': push(s,str[i]);break;
            case ')' : while((p = pop(s)) != '('){
                postfix[j++] = (char)p;
                size = size-1;
            }
            break;
            default:
            c = &str[i];
            n = strspn(c,"1234567890");
            num = strtol(c,NULL,10);
            snprintf(&postfix[j],size,"%d ",num);
            j = j + n + 1;
            i = i + n-1;
            size = size- n - 1;
        }
        i++;
    }
    while(s->top != -1){
        postfix[j++] = (char)pop(s);
        size = size-1;
    }
    postfix[j] = '\0';
    return postfix;
}
int precedence(char a){
    switch(a){
        case '+':
            return 0;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 3;
        case '^':
            return 4;
        default:
            return -1;

    }
}
int optn(int a,int b,char c){
    switch(c){
        case '+':
            return b+a;
        case '*':
            return b*a;
        case '-':
            return b-a;
        case '/':
            if(a != 0) 
                return b/a;
        case '^':
            return pow(b,a);//pow() takes double as parameters and returns double
        default:
            return c;
    }
}
int result(arrStack *s,char *str){
    size_t i = 0;
    int a, b, res;
    while(i < strlen(str)){
        if(isdigit(str[i])){
            char* c = &str[i];
            int n = strspn(c,"1234567890");
            int num = strtol(c,NULL,10);
            push(s,num);
            i = i + n;
            continue;
        }
        switch(str[i]){
            case '+':
            case '*':
            case '/':
            case '^':
            case '-':
                a = pop(s);
                b = pop(s);
                res = optn(a,b,str[i]);
                push(s,res);
                i++;
                //printf("%d ",res);
                break;
            default:
                i++;
        }
    }
    return s->arr[s->top];
}