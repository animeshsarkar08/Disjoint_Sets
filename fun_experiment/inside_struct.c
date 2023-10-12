//using a structure inside another structure but before declaring
#include<stdio.h>
//struct node;
//struct edge;
typedef struct node{
	int a;
	struct edge *E;
}node;
typedef struct edge{
	char c;
	struct node *N;
}edge;
int main(){
   node n,p;
   edge e;
   n.a = 1;
   n.E = &e;
   e.c = 'c';
   e.N = &p;
   p.a = 3;
   p.E = NULL;
   node *m = &n;
   printf("%d ",m->a );
   printf("%c ",m->E->c );
   printf("%d ",m->E->N->a );

}
