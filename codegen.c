#include "9cc.h"
#include <stdio.h>

static int control_count=0;

void gen_lval(Node*node){
    if (node->kind!=ND_LVAR){
        error("the left value of assignment is not a variable");
    }
    printf("  mov rax, rbp\n");//rbpはアドレス
    printf("  sub rax, %d\n",node->offset);
    printf("  push rax\n");
}

void gen(Node*node){//kk
    if(node->kind==ND_RETURN){
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
        return;
    }
    
    switch(node->kind){
        case ND_NUM:{
            printf("  push %d\n",node->val);
            return;
        }
        case ND_LVAR:{
            gen_lval(node);

            printf("  pop rax\n");
            printf("  mov rax, [rax]\n");
            printf("  push rax\n");
            return;
        }
        case ND_ASSIGN:{
            gen_lval(node->lhs);
            gen(node->rhs);

            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  mov [rax], rdi\n");
            printf("  push rdi\n");
            return;
        }
        case ND_IF:{
            gen(node->lhs);

            printf("  pop rax\n");
            printf("  cmp rax, 0\n");

            int screen_count=control_count;
            control_count++;

            printf("  je .Lend%03d\n",screen_count);

            gen(node->rhs);

            printf(".Lend%03d:\n",screen_count);

            return;
        }
        case ND_IFELSE:{
            gen(node->lhs);

            printf("  pop rax\n");
            printf("  cmp rax, 0\n");

            int screen_count=control_count;
            control_count++;

            printf("  je .Lelse%03d\n",screen_count);

            gen(node->rhs->lhs);

            printf("  jmp .Lend%03d\n",screen_count);

            printf(".Lelse%03d:\n",screen_count);
            gen(node->rhs->rhs);

            printf(".Lend%03d:\n",screen_count);

            return;
        }
        case ND_WHILE:{
            int screen_count=control_count;
            control_count++;
            printf(".Lbegin%03d:\n",screen_count);
            gen(node->lhs);
            
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lend%03d\n",screen_count);

            gen(node->rhs);
            printf("  jmp .Lbegin%03d\n",screen_count);
            printf(".Lend%03d:\n",screen_count);

            return;
        }
        case ND_FOR:{
            if(node->lhs->lhs){
            gen(node->lhs->lhs);
            }
            int screen_count=control_count;
            control_count++;
            printf(".Lbegin%03d:\n",screen_count);
            if(node->lhs->rhs->lhs){
            gen(node->lhs->rhs->lhs);
            }

            printf("  pop rax\n");
            printf("  cmp rax, 0\n");

            printf("  je .Lend%03d\n",screen_count);

            gen(node->rhs);
            if(node->lhs->rhs->rhs){
            gen(node->lhs->rhs->rhs);
            }

            printf("  jmp .Lbegin%03d\n",screen_count);
            
            printf(".Lend%03d:\n",screen_count);

            return;
        }  
        case ND_BLOCK:{
            for(;node->lhs->rhs;node=node->lhs){
                gen(node->lhs->rhs);
                printf("  pop rax\n");
            }

            return;
        }
    }


    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch(node->kind){
        case ND_ADD:{
            printf("  add rax, rdi\n");
            break;
        }
        case ND_SUB:{
            printf("  sub rax, rdi\n");
            break;
        }
        case ND_MUL:{
            printf("  imul rax, rdi\n");
            break;
        }
        case ND_DIV:{
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
        }
        case ND_EQ:{
            printf("  cmp rax, rdi\n");
            printf("  sete al\n");
            printf("  movzb rax, al\n");
            break;
        }
        case ND_NE:{
            printf("  cmp rax, rdi\n");    
            printf("  setne al\n");
            printf("  movzb rax, al\n");
            break;
        }
        case ND_LT:{
            printf("  cmp rax, rdi\n");
            printf("  setl al\n");
            printf("  movzb rax, al\n");
            break;
        }
        case ND_LE:{
            printf("  cmp rax,rdi\n");
            printf("  setle al\n");
            printf("  movzb rax, al\n");
            break;
        }
    }

    printf("  push rax\n");
}
