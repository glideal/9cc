#include "9cc.h"

extern char* user_input;
extern char** input;
extern Token*token;
extern Node*code[100];
int Nvar;//the number of variable

int main(int argc, char**argv){//ok//kk
    if(argc!=2){
        error("%s: invalid number of argument",argv[0]);
    }
    user_input=argv[1];
    //printf("ok\n");
    //change for 'error_at' function
    error_line(argv[1]);

    char*p=user_input;
    //printf("ok\n");
    token=tokenize();//change for 'error_at' function
    //printf("ok1\n");
    program();
    //Check();


    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    //printf("main:\n");

    //prologue

    for(int i=0;code[i];i++){
        Nvar=code[i]->argc;
        gen(code[i]);

        printf("  pop rax\n");
    }

    // printf("  mov rsp, rbp\n");
    // printf("  pop rbp\n");
    // printf("  ret\n");
    return 0;
}
