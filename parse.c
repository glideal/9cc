#include "9cc.h"
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

char* user_input;

Token *token;


bool consume(char* op){//remove ';'
    if(token->kind != TK_RESERVED || strlen(op)!=token->len||memcmp(token->str,op,token->len)){
        return false;
    }
    token=token->next;
    return true;
}

void expect(char* op){//remove '\n' ok
    if(token->kind != TK_RESERVED || strlen(op)!=token->len||memcmp(token->str,op,token->len)){
        error_at(token->str,"expected \"%s\"",op);
    }
    token=token->next;
}
/*--------------------------------------------------------------------------------*/
int expect_number(){//remove '\n' ok//kk
    if(token->kind!=TK_NUM){
        error_at(token->str,"数ではありません");
    }
    int val=token->val;
    token=token->next;
    return val;
}

bool at_eof(){//ok//kk
    return token->kind==TK_EOF;
}

Token *new_token(TokenKind kind,Token *cur,char *str,int len){//ok//kk
    Token *tok=calloc(1,sizeof(Token));
    tok->kind=kind;
    tok->str=str;
    tok->len=len;
    cur->next=tok;
    return tok;
}

bool startswith(char*p,char*q){//kk
    return memcmp(p,q,strlen(q))==0;
}

Token *tokenize(){//ok//change for 'error_at' function//kk
    char*p=user_input;//change for 'error_at' function
    Token head;
    head.next=NULL;
    Token *cur=&head;
    while(*p){
        if(isspace(*p)){
            p++;
            continue;
        }

       // if(strchr("+-*/()",*p)){
        /*    cur = new_token(TK_RESERVED,cur,p);
            p++;
            continue;
        }
        */
        if(startswith(p,"==")||startswith(p,"<=")||startswith(p,">=")||startswith(p,"!=")){
            cur=new_token(TK_RESERVED,cur,p,2);
            p+=2;
            continue;
        }

        if(strchr("+-*/()<>",*p)){
            cur=new_token(TK_RESERVED,cur,p,1);
            p++;;
            continue;
        }

        if(isdigit(*p)){
            cur = new_token(TK_NUM,cur,p,0);
            char*q=p;
            cur->val = strtol(p,&p,10);
            cur->len=p-q;
            continue;
        }

        error_at(p,"invalid number\n");//change for 'error_at' function1
    }
    new_token(TK_EOF,cur,p,0);
    return head.next;
}

/*_____________________________________________________Node____________________________________________________________*/

Node*new_node(NodeKind kind){//kk
    Node*node=calloc(1,sizeof(Node));
    node->kind=kind;
    return node;
}

Node*new_binary(NodeKind kind,Node*lhs,Node*rhs){//kk
    Node*node=new_node(kind);
    node->lhs=lhs;
    node->rhs=rhs;
    return node;
}

Node*new_num(int val){//kk
    Node*node=new_node(ND_NUM);
    node->val=val;
    return node;
}


Node*expr(){
   return equality();
}

Node*equality(){//kk
    Node*node=relational();

    for(;;){
        if(consume("==")){
            node=new_binary(ND_EQ,node,relational());
        }
        else if(consume("!=")){
            node=new_binary(ND_NE,node,relational());
        }
        else{
            return node;
        }
    }
}

Node*relational(){//kk
    Node*node=add();

    for(;;){
        if(consume("<")){
            node=new_binary(ND_LT,node,add());
        }
        else if(consume("<=")){
            node=new_binary(ND_LE,node,add());
        }
        else if(consume(">")){
            node=new_binary(ND_LT,add(),node);
        }
        else if(consume(">=")){
            node=new_binary(ND_LE,add(),node);
        }
        else{
            return node;
        }
    }
}

Node*add(){//kk
    Node*node=mul();

    for(;;){
        if(consume("+")){
            node=new_binary(ND_ADD,node,mul());
        }
        else if(consume("-")){
            node=new_binary(ND_SUB,node,mul());
        }
        else{
            return node;
        }
    }
}

Node*mul(){//kk
    Node*node=unary();

    for(;;){
        if(consume("*")){
            node=new_binary(ND_MUL,node,unary());
        }
        else if(consume("/")){
            node=new_binary(ND_DIV,node,unary());
        }
        else{
            return node;
        }
    }
}

Node*unary(){//kk
    if (consume("+")){
        return unary();
    }
    if (consume("-")){
        return new_binary(ND_SUB,new_num(0),unary());
    }
    return primary();
}

Node*primary(){//kk
    if (consume("(")){
        Node*node=expr();
        expect(")");
        return node;
    }

    return new_num(expect_number());
}

