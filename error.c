#include "9cc.h"

extern char* user_input;
extern char** input;
void error_line(int argc,char*p){
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

    for(int i=0;*q;i++){
        //if(i==0){
        //    input[0][0]=*q;
        //}
        //q++;
        //printf("ok1\n");
        input[i]=(char*)calloc(25,sizeof(char));
        //printf("ok2\n");
        for(int j=0;*q;j++){
            //if(i==0&&j==0){
            //    j=1;
            //}
            if(*q=='\n'){
                //printf("ok new line_%c\n",*q);
                input[i][j]='\0';
                break;
            }
            printf("ok3_%c ",*q);
            printf("(i,j)=(%d,%d)\n",i,j);
            input[i][j]=*q;
            q++;
        }
        printf("ok0000000\n");
        
        if(!(*q)){
            break;
        }
        q++;
    }

    // printf("ok|||||||||||||||\n");

    for(int i = 0; input[i][0]; i++) {
        
        for(int j = 0; input[i][j]!='\0'; j++) {
            printf("%c",input[i][j]);
        }
        printf("\n");
        

        //上下のいずれかを使う

        //printf("%s",input[i]);
        //printf("\n");
    }


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
    

    fprintf(stderr,"%s\n",input[line-1]);
    fprintf(stderr,"%s\n",input[line]);
    fprintf(stderr,"Line:%d~%d ",line,line+1);//pos個の空白を出力
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    va_end(ap);
    exit(1);
}