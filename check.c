#include "9cc.h"

extern Node_t*code[100];

void TokenCheck(Token_t head){
    Token_t*t=head.next;
    while(t){
        for(;!(t->kind==TK_EOF);){
            printf("line=%d ",t->line);
            printf("%s\n",t->str);
            printf("_________________________\n");
            t=t->next;
        }
        if(t->kind==TK_EOF){
            printf("_EOF_\n");
            printf("_________________________\n");
            t=t->next;
        }
    }
}


void Check(){
    printf("\n");
    printf("\n");
    printf("\n");
    
    printf("Node Check\n");
    for(int i=0;code[i];i++){
        printf("_________________________code[%d]__\n",i);
        CheckNode(code[i]);
    }
}

void CheckNode(Node_t* node){
    if(node){
        printf("_____start________________________\n\n");

        switch(node->kind){
            case ND_IF:{
                printf(" %s\n","if");
                break;
            }
            case ND_ELSE:{
                printf(" %s\n","else");
                break;
            }
            case ND_WHILE:{
                printf(" %s\n","while");
                break;
            }
            case ND_FOR:{
                printf(" %s\n","for");
                break;
            }
            case ND_BLOCK:{
                printf(" %s\n","block");
                printf("block_start:\n");
                for(int i=0;node->block[i];i++){
                    CheckNode(node->block[i]);
                    //printf("  pop rax\n");
                }
                printf("block_end:\n");
                break;
            }
            case ND_ADD:{
                printf(" %s\n","+");
                break;
            }
            case ND_SUB:{
                printf(" %s\n","-(minus)");
                break;
            }   
            case ND_MUL:{
                printf(" %s\n","*");
                break;
            }
            case ND_DIV:{
                printf(" %s\n","%");
                break;
            }
            case ND_ASSIGN:{
                printf(" %s\n","=代入");
                break;
            }
            case ND_LVAR:{
                printf(" %s\n","variable");
                break;
            }
            case ND_FUNC_CALL:{
                printf("fanction call %s\n",node->funcname);
                break;
            }case ND_FUNC_DEF:{
                printf(" %s\n","fanction definition");
                if(node->argv[0]!=NULL){
                    printf("arguments are\n");
                }
                for(int i=0;node->argv[i];i++){
                    switch(node->argv[i]->kind){
                        case ND_NUM:{
                            printf("num[%d]==%d ",i,node->argv[i]->val);
                            break;
                        }
                        case ND_LVAR:{
                            printf("variable[%d]==%d ",i,node->argv[i]->offset);
                            break;
                        }
                        default:{
                            error("argument of function '%s' are not correct\n",node->funcname);
                            exit(1);
                        }
                    }
                }
                printf("\n");
                break;
            }
            case ND_EQ:{
                printf(" %s\n","==");
                break;
            }
            case ND_NE:{
                printf(" %s\n","!=");
                break;
            }
            case ND_LT:{
                printf(" %s\n","<");
                break;
            }
            case ND_LE:{
                printf(" %s\n","<=");
                break;
            }
            case ND_RETURN:{
                printf(" %s\n","return");
                break;
            }
            case ND_ADDR:{
                printf(" &\n");
                break;
            }
            case ND_DEREF:{
                printf(" *(pointer)\n");
                break;
            }
            case ND_NUM:{
                printf(" %s %d\n","number",node->val);
                break;
            }
        }
    

        printf("_left____________________________\n\n");
        CheckNode(node->lhs);

        printf("_right____________________________\n\n");
        CheckNode(node->rhs);

        printf("_end______________________________\n\n");
    }

 }
