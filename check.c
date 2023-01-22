#include "9cc.h"

extern Node*code[100];

void TokenCheck(Token head){
    for(Token*t=head.next;!(t->kind==TK_IDENT||t->kind==TK_EOF);t=t->next){
        printf("%s\n",t->str);
    }
}

void Check(){
    for(int i=0;code[i];i++){
        printf("_________________________code[%d]__\n",i);
        CheckNode(code[i]);
    }
}

void CheckNode(Node* node){
    printf("_start____________________________\n\n");
    if(node){
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
            case ND_FUNCTION:{
                printf(" %s\n","fanction");
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
