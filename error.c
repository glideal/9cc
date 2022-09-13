#include "9cc.h"
#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>

extern char* user_input;

void error(char *fmt,...){//argument is same to Printf Fanction
  
    va_list ap;
    va_start(ap,fmt);
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    exit(1);
}


void error_at(char*loc,char*fmt,...){
    va_list ap;
    va_start(ap,fmt);

    int pos=loc-user_input;
    
    fprintf(stderr,"%s\n",user_input);
    fprintf(stderr,"%*s",pos,"");//pos個の空白を出力
    fprintf(stderr,"^ ");
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    va_end(ap);
    exit(1);
}