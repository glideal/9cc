#include "9cc.h"
#include <stdio.h>

extern char* user_input;
extern Token*token;


int main(int argc, char**argv){//ok//kk
    if(argc!=2){
        error("%s: invalid number of argument",argv[0]);
    }

    user_input=argv[1];//change for 'error_at' function
    token=tokenize();//change for 'error_at' function
    Node*node=expr();


    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    /*
    printf("  mov rax, %d\n",expect_number());
    
    while(!at_eof()){//ok
        if(consume('+')){
            printf("  add rax, %d\n", expect_number());
            continue;
        }

       expect('-');
       printf("  sub rax, %d\n",expect_number());
    }
    */

    gen(node);

    printf("  pop rax\n");

    printf("  ret\n");
    return 0;
}
