#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXINT 15
#define MAXCHR 20

int itop = -1;
int ctop = -1;
char stack[] = "(3+12345678)*10";
char optr[MAXCHR];
int oprnd[MAXINT];

int result ();
void entry ();
void push_int (int a);
void push_chr (char a);
char pop_chr ();
int pop_int ();


int main (int argc,char *argv[]){
     entry();
     
    printf(" %d",result());
}

void entry (){
    char *c = stack;
    size_t i = 1;
    while(i <= strlen(stack)){
        if(strspn(c,"1234567890")){
            int a = strtol(c,&c,10);
            push_int(a);
           // printf("%d\n",a);
        }
        else if(strspn(c,"(+-*")){
            push_chr(*c);
            c++;
          //  printf("%d\n",*c);
        }
        else if (strspn(c,")")){
            int a = pop_int();
            char p = pop_chr();
            int b = pop_int();
            int r =9;
            //if( p == '+'){
                r = a+b;
            //}
            push_int(b);
            push_int(a);
            push_int(r);
            push_chr(p);
            push_chr(*c);
            c++;
            //printf("%c\n",p);
        }
        else;
        i++;
    }
}
int result (){
     int a = pop_int();
     char p = pop_chr();
     int b = pop_int();
    // if(p == '*'){
         int r = a * b;
         push_int(b);
         push_int(a);
         push_chr(p);
         return r;
     //}
 }
void push_int (int a){
    if(itop == MAXINT -1){
        printf("stack overflow int");
        exit(1);
    }
    itop = itop + 1;
    oprnd[itop] = a;
}
void push_chr (char a){
    if(ctop == MAXCHR -1){
        printf("stack overflow chr");
        exit(2);
    }
    ctop = ctop + 1;
    optr[ctop] = a;
}
int pop_int (){
    if(itop == -1){
        printf("stack underflow int");
        exit(2);
    }
    int a;
      a = oprnd[itop];
    itop = itop - 1;
    return a;
}
char pop_chr (){
    if(ctop == -1){
        printf("stack underflow chr");
        exit(3);
    }
    char a;
    a = oprnd[ctop];
    ctop = ctop - 1;
    return a;
}