#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXCHR 20
#define MAX 50
int top = -1;
int stack[MAXCHR];
void push (int a);
int pop ();
char* intopost(char *str);
int precedence(char a);
int result(char *str);
int optn(int a,int b,char c);

int main (int argc,char *argv[]){
    char *a, s[MAX];
    fgets(s,MAX,stdin);
    a = intopost(s);
    int b = result(a);
    printf("%d",b);
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
int result(char *str){
    size_t i = 0;
    int a, b, res;
    while(i < strlen(str)){
        if(isdigit(str[i])){
            char* c = &str[i];
            int n = strspn(c,"1234567890");
            int num = strtol(c,NULL,10);
            push(num);
            i = i + n;
            continue;
        }
        switch(str[i]){
            case '+':
            case '*':
            case '/':
            case '^':
            case '-':
                a = pop();
                b = pop();
                res = optn(a,b,str[i]);
                push(res);
                i++;
                //printf("%d ",res);
                break;
            default:
                i++;
        }
    }
    return stack[top];
}
void push (int a){
    if(top == MAXCHR -1){
        printf("stack overflow\n");
        exit(2);
    }
    top = top + 1;
    stack[top] = a;
}
int pop (){
    if(top == -1){
        printf("stack underflow\n");
        exit(3);
    }
    int a;
    a = stack[top];
    top = top - 1;
    return a;
}
char* intopost(char *str){
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
            case '/':  while (top != -1 && (precedence(str[i]) != -1) && (precedence(str[i]) <= precedence(stack[top]))){
                postfix[j++] = (char)pop();
                size = size-1;
            }
            push(str[i]);break;
            case '(': push(str[i]);break;
            case ')' : while((p = pop()) != '('){
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
    while(top != -1){
        postfix[j++] = (char)pop();
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