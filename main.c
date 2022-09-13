#include "9cc.h"
#include <stdio.h>

extern char* user_input;
extern Token*token;
extern Node*code[100];
extern LVar*locals;


int main(int argc, char**argv){//ok//kk
    if(argc!=2){
        error("%s: invalid number of argument",argv[0]);
    }

    user_input=argv[1];//change for 'error_at' function
    token=tokenize();//change for 'error_at' function
    program();


    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    //prologue

    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, %d\n",8*26);

    for(int i=0;code[i];i++){
        gen(code[i]);

        printf("  pop rax\n");
    }

    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;
}
