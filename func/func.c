#include<stdio.h>
#include<stdlib.h>
int foo(){
    printf("function_call ok\n");
}

int bar(int x,int y){
    printf("%d\n",x+y);
}

int sum(int x,int y){
    return x+y;
}
void alloc(int **p,int a,int b,int c,int d){
    // *p=calloc(4,sizeof(int));
    *p=malloc(sizeof(int)*5);
    (*p)[0]=a;//'*p[] is fault. it needs '('and')'
    (*p)[1]=b;
    (*p)[2]=c;
    (*p)[3]=d;
}