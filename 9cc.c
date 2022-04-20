#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum {
    TK_RESERVED,  //token(symbol)
    TK_NUM,       //integer token
    TK_EOF,       //token to remark end of input
}TokenKind;

typedef struct Token Token;

struct Token{
    TokenKind kind;
    Token *next;
    int val;
    char *str;
};

Token *token;

void error(char *fmt,...){//argument is same to Printf Fanction
    va_list ap;
    va_start(ap,fmt);
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    exit(1);
}

bool consume(char op){//remove ';'
    if(token->kind != TK_RESERVED || token->str[0] != op){
        return false;
    }
    token=token->next;
    return true;
}

void expect(char op){//remove '\n' ok
    if(token->kind != TK_RESERVED || token->str[0] != op){
        error("'%c'ではありません",op);
    }
    token=token->next;
}

int expect_number(){//remove '\n' ok
    if(token->kind!=TK_NUM){
        error("数ではありません");
    }
    int val=token->val;
    token=token->next;
    return val;
}

bool at_eof(){//ok
    return token->kind==TK_EOF;
}

Token *new_token(TokenKind kind,Token *cur,char *str){//ok
    Token *tok=calloc(1,sizeof(Token));
    tok->kind=kind;
    tok->str=str;
    cur->next=tok;
    return tok;
}

Token *tokenize(char *p){//ok
    Token head;
    head.next=NULL;
    Token *cur=&head;
    while(*p){
        if(isspace(*p)){
            p++;
            continue;
        }

        if(*p=='+'||*p=='-'){
            cur = new_token(TK_RESERVED,cur,p);
            p++;
            continue;
        }

        if(isdigit(*p)){
            cur = new_token(TK_NUM,cur,p);
            cur->val = strtol(p,&p,10);
            continue;
        }

        error("トークナイズできません\n");
    }
    new_token(TK_EOF,cur,p);
    return head.next;
}





int main(int argc, char**argv){//ok
    if(argc!=2){
        fprintf(stderr,"引数の個数が正しくありません\n");
        return 1;
    }

    token=tokenize(argv[1]);


    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("  mov rax, %d\n",expect_number());
    
    while(!at_eof()){//ok
        if(consume('+')){
            printf("  add rax, %d\n", expect_number());
            continue;
        }

       expect('-');
       printf("  sub rax, %d\n",expect_number());
    }

    printf("  ret\n");
    return 0;
}
