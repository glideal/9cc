#include "9cc.h"

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
    int screen_count=control_count;
    control_count++;
    
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

            printf("  je .Lelse%03d\n",screen_count);
            if(node->rhs->kind==ND_ELSE){
                gen(node->rhs->lhs);
                printf(".Lelse%03d:\n",screen_count);
                gen(node->rhs->rhs);
            }else{
                gen(node->rhs);
                printf(".Lelse%03d:\n",screen_count);
                //ifだけでも"".Lelse%03d%:でおわる"
            }

            return;
        }
        case ND_WHILE:{

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
            if(node->lhs->lhs->lhs){
                gen(node->lhs->lhs->lhs);
            }

            printf(".Lbegin%03d:\n",screen_count);
            if(node->lhs->lhs->rhs){
                gen(node->lhs->lhs->rhs);
            }

            printf("  pop rax\n");
            printf("  cmp rax, 0\n");

            printf("  je .Lend%03d\n",screen_count);

            gen(node->rhs);
            if(node->lhs->rhs){
                gen(node->lhs->rhs);
            }

            printf("  jmp .Lbegin%03d\n",screen_count);
            
            printf(".Lend%03d:\n",screen_count);

            return;
        }  
        case ND_BLOCK:{
            for(int i=0;node->block[i];i++){
                gen(node->block[i]);
                //printf("  pop rax\n");
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
