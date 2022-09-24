#include "9cc.h"
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>


char* user_input;

Token *token;

LVar*locals;

bool consume_control(char* op){//remove ';'
    if(token->kind != TK_CONTROL || strlen(op)!=token->len||memcmp(token->str,op,token->len)){
        return false;
    }
    token=token->next;
    return true;
}

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

char*StrIdent(char*s,char** endptr){
    char*str=calloc(100,sizeof(char));
    for(int i=0;('a'<=*s&&*s<='z');s++){
        str[i]=*s;
        i++;
    }
    *endptr=s;
    return &str[0];
}

int is_alnum(char c){
    return  ('a'<=c&&c<='z')||
            ('A'<=c&&c<='Z')||
            ('0'<=c&&c<='9')||
            (c=='_');

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

        if(startswith(p,"==")||startswith(p,"<=")||startswith(p,">=")||startswith(p,"!=")){
            cur=new_token(TK_RESERVED,cur,p,2);
            p+=2;
            continue;
        }

        if(strchr("+-*/()<>=;",*p)){
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

        if(strncmp(p,"return",6)==0&&(!is_alnum(p[6]))){
            cur=new_token(TK_RETURN,cur,p,6);
            p+=6;
            continue;
        }

        if(strncmp(p,"if",2)==0&&(!is_alnum(p[2]))){
            cur=new_token(TK_CONTROL,cur,p,2);
            p+=2;
            continue;
        }

        if(strncmp(p,"while",5)==0&&(!is_alnum(p[5]))){
            cur=new_token(TK_CONTROL,cur,p,5);
            p+=5;
            continue;
        }
        
        if(strncmp(p,"for",3)==0&&(!is_alnum(p[3]))){
            cur=new_token(TK_CONTROL,cur,p,3);
            p+=3;
            continue;
        }        

        if(strncmp(p,"else",4)==0&&(!is_alnum(p[4]))){
            cur=new_token(TK_CONTROL,cur,p,4);
            p+=4;
            continue;
        }

        if('a'<=*p&&*p<='z'){
            cur=new_token(TK_IDENT,cur,p,0);
            char*q=p;
            cur->str=StrIdent(p,&p);
            cur->len=p-q;
            continue;
        }

        error_at(p,"invalid number\n");//change for 'error_at' function1
    }
    new_token(TK_EOF,cur,p,0);
    return head.next;
}

/*_____________________________________________________Node____________________________________________________________*/

Node*code[100];


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

Token* consume_ident(){
    if(token->kind!=TK_IDENT){
        return NULL;
    }
    Token*tok=token;
    token=token->next;
    return tok;
}


LVar *find_lvar(Token*tok){
    for(LVar *var=locals;var;var=var->next){
        if(var->len==tok->len&&!memcmp(tok->str,var->name,var->len)){
            return var;
        }
    }
    return NULL;
}

bool consume_return(){
    if(token->kind!=TK_RETURN){
        return false;
    }
    token=token->next;
    return true;
}



Node*program(){
    LVar locals_head;
    locals_head.next=NULL;
    locals_head.offset=0;
    locals=&locals_head;

    int i=0;
    while(!at_eof()){
        code[i]=stmt();
        i++;
    }
    code[i]=NULL;
}

Node*stmt(){
    Node*node;

    if(token->kind==TK_CONTROL){
        if(consume_control("if")){
            expect("(");
            Token*tok=token;
            Token*Tok=token;
            while(1){
                
                if(Tok->kind==TK_CONTROL&&strncmp("if",Tok->str,2)){
                    break;
                }
                if(Tok->kind==TK_CONTROL&&strncmp(Tok->str,"else",4)){
                    break;
                }
                if(Tok->kind==TK_EOF){
                    break;
                }
                tok=Tok;
                Tok=Tok->next;
            }
            if(tok->next->kind==TK_CONTROL&&strncmp(tok->next->str,"else",4)==0){
                node=calloc(1,sizeof(Node));
                node->kind=ND_IFELSE;
                node->lhs=expr();
                expect(")");
                node->rhs=stmt();
                token=token->next;//consume(token->str=="else")
                node->rhs=new_binary(ND_ELSE,node->rhs,stmt());

                return node;
            }else{
                node=calloc(1,sizeof(Node));
                node->kind=ND_IF;
                node->lhs=expr();
                expect(")");
                node->rhs=stmt();

                return node;
            }
            if(consume_control("else")){
                node=new_binary(ND_ELSE,node,expr());
            }
        }
        if(consume_control("while")){
            expect("(");
            node=calloc(1,sizeof(Node));
            node->kind=ND_WHILE;
            node->lhs=expr();
            expect(")");
            node->rhs=stmt();

            return node;
        }
        
        if(consume_control("for")){
            expect("(");
            node=calloc(1,sizeof(Node));
            node->kind=ND_FOR;
            node->lhs=calloc(1,sizeof(Node));
            if(!(token->kind==TK_RESERVED&&strncmp(token->str,";",1)==0)){
                node->lhs->lhs=expr();
            }
            expect(";");
            node->lhs->rhs=calloc(1,sizeof(Node));
            if(!(token->kind==TK_RESERVED&&strncmp(token->str,";",1)==0)){
                node->lhs->rhs->lhs=expr();
            }
            expect(";");
            if(!(token->kind==TK_RESERVED&&strncmp(token->str,";",1)==0)){
                node->lhs->rhs->rhs=expr();
            }
            expect(")");
            node->rhs=stmt();

            return node;
        }
    }

    if(consume_return()){
        node=calloc(1,sizeof(Node));
        node->kind=ND_RETURN;
        node->lhs=expr();
    }else{
        node=expr();
    }

    if(token->kind=TK_CONTROL&&strncmp(token->str,"else",4)==0){
        return node;
    }

    if(!consume(";")){
        error_at(token->str,"';'ではありません");
    }
    return node;
}


Node*expr(){
   return assign();
}

Node*assign(){
    Node*node=equality();
    if(consume("=")){
        node=new_binary(ND_ASSIGN,node,assign());
    }
    return node;
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
    Token*tok=consume_ident();

    //printf("OK1\n");
    if(tok){
        Node*node=calloc(1,sizeof(Node));
        node->kind=ND_LVAR;


        LVar*lvar=find_lvar(tok);

        if(lvar){
            node->offset=lvar->offset;
        }
        else{
            lvar=calloc(1,sizeof(LVar));
            lvar->next=locals;
            lvar->name=tok->str;
            lvar->len=tok->len;
            lvar->offset=locals->offset+8;
            node->offset=lvar->offset;
            locals=lvar;
        }
        return node;
    }

    return new_num(expect_number());
}

