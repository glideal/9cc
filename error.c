#include "9cc.h"

extern char* user_input;
extern char** input;
void error_line(char*p){
    /*
    int line=0;
    int row=0;
    for(int i=0;i<argc;i++){
        if(*p=='\n'){
            line++;
            row=0;
            continue;
        }
        printf("ok\n");
        input[line][row]=*p;
        printf("ok\n");
        printf("%c\n",p[i]);
        
        row++;
    }
    */


    char*q=p;
    input=(char**)calloc(200,sizeof(char*));
    int i=0,j=0;
    while(1){
        if(j==0){
            input[i]=(char*)calloc(100,sizeof(char));
        }
        if(!*q){
            input[i][j]='\0';
            if(j!=0){
                //to display input[][] after that,the last should definely be '\0'
                static char term[1]={'\0'};
                i++;
                input[i]=term;
            }
            break;
        }
        if(*q=='\n'){
            input[i][j]='\0';
            i++;
            j=0;
        }else{
            input[i][j]=*q;
            j++;
        }
        q++;
    }
    // printf("ok|||||||||||||||\n");
/*
    for(int i = 0; input[i][0]; i++) {
        
        for(int j = 0; input[i][j]; j++) {//;input[i][j]!='\0';と同意
            printf("%c",input[i][j]);
        }
        printf("\n");
        

        //上下のいずれかを使う

        //printf("%s",input[i]);
        //printf("\n");
    }
*/
}

Token*error_tokenize(){
}

void error(char *fmt,...){//argument is same to Printf Fanction
  
    va_list ap;
    va_start(ap,fmt);
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    exit(1);
}


void error_at(int line,char*fmt,...){
    va_list ap;
    va_start(ap,fmt);

    // int pos=loc-user_input;

    // if(pos>strlen(user_input)){
    //     printf("pos is too long\n");
    //     exit(1);
    // }
    
    if(line-1>=0){
        fprintf(stderr,"%s\n",input[line-1]);
    }
    fprintf(stderr,"%s\n",input[line]);
    fprintf(stderr,"Line:%d~%d ",line,line+1);//pos個の空白を出力
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    va_end(ap);
    exit(1);
}