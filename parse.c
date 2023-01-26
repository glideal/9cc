#include "9cc.h"


char* user_input;
char**input;

Token *token;

LVar*locals;
void expect(char* op){
    if(token->kind != TK_RESERVED || strlen(op)!=token->len||memcmp(token->str,op,token->len)){
        error_at(token->line,"expected \"%s\"",op);
    }
    token=token->next;
}

int expect_number(){
    if(token->kind!=TK_NUM){
        error_at(token->line,"数ではありません");
    }
    int val=token->val;
    token=token->next;
    return val;
}

bool at_eof(){
    return token->kind==TK_EOF;
}

Token *new_token(TokenKind kind,Token *cur,char *str,int len){
    Token *tok=calloc(1,sizeof(Token));
    tok->kind=kind;
    tok->str=str;
    tok->len=len;
    cur->next=tok;
    return tok;
}

bool startswith(char*p,char*q){
    return memcmp(p,q,strlen(q))==0;
}

void StrIdent(char*s,char** endptr){
    char*str=calloc(100,sizeof(char));
    for(;('a'<=*s&&*s<='z');s++){

    }
    *endptr=s;
}

int is_alnum(char c){
    return  ('a'<=c&&c<='z')||
            ('A'<=c&&c<='Z')||
            ('0'<=c&&c<='9')||
            (c=='_');

}


Token *tokenize(){
    char*p=user_input;
    Token head;
    head.next=NULL;
    Token *cur=&head;
    int line=0;
    while(*p){
        //printf("line=%d\n",line);
        if(*p=='\n'){
            line++;
        }
        if(isspace(*p)){
            p++;
            continue;
        }

        if(startswith(p,"==")||startswith(p,"<=")||startswith(p,">=")||startswith(p,"!=")){
            cur=new_token(TK_RESERVED,cur,p,2);
            cur->line=line;
            p+=2;
            continue;
        }

        if(strchr("+-*/()<>=;{},",*p)){
            cur=new_token(TK_RESERVED,cur,p,1);
            cur->line=line;
            p++;;
            continue;
        }

        if(isdigit(*p)){
            cur = new_token(TK_NUM,cur,p,0);
            char*q=p;
            cur->val = strtol(p,&p,10);
            cur->len=p-q;
            cur->line=line;
            continue;
        }

        if(strncmp(p,"return",6)==0&&(!is_alnum(p[6]))){
            cur=new_token(TK_RETURN,cur,p,6);
            cur->line=line;
            p+=6;
            continue;
        }

        if(strncmp(p,"if",2)==0&&(!is_alnum(p[2]))){
            cur=new_token(TK_IF,cur,p,2);
            cur->line=line;
            p+=2;
            continue;
        }        

        if(strncmp(p,"else",4)==0&&(!is_alnum(p[4]))){
            cur=new_token(TK_ELSE,cur,p,4);
            cur->line=line;
            p+=4;
            continue;
        }

        if(strncmp(p,"while",5)==0&&(!is_alnum(p[5]))){
            cur=new_token(TK_WHILE,cur,p,5);
            cur->line=line;
            p+=5;
            continue;
        }
        
        if(strncmp(p,"for",3)==0&&(!is_alnum(p[3]))){
            cur=new_token(TK_FOR,cur,p,3);
            cur->line=line;
            p+=3;
            continue;
        }

        if('a'<=*p&&*p<='z'){
            cur=new_token(TK_IDENT,cur,p,0);
            char*q=p;
            StrIdent(p,&p);
            cur->len=p-q;
            cur->line=line;
            //printf("%d\n",cur->len);
            continue;
        }

        error_at(line,"invalid number\n");//change for 'error_at' function1
    }
    new_token(TK_EOF,cur,p,0);
    //TokenCheck(head);
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


bool consume(char* op){
    if(token->kind != TK_RESERVED || strlen(op)!=token->len||memcmp(token->str,op,token->len)){
        return false;
    }
    token=token->next;
    return true;
}

Token* consume_kind(TokenKind kind){
    if (token->kind != kind){
        return NULL;
    }
    Token* tok=token;
    token=token->next;
    return tok;
}


LVar *find_lvar(Token*tok){
    for(LVar *var=locals;var;var=var->next){
        if(var->len==tok->len&&!memcmp(tok->str,var->name,var->len)){//the variable exists, then return that variable
            return var;
        }
    }
    return NULL;
}




Node*program(){
    LVar locals_head;
    locals_head.next=NULL;
    locals_head.offset=0;
    locals=&locals_head;

    int i=0;
    while(!at_eof()){
        code[i]=func();
        i++;
    }
    code[i]=NULL;
}

Node*func(){
    Node*node;
    Token* tok=consume_kind(TK_IDENT);
    if(tok==NULL){
        error("not function\n");
    }
    expect("(");
    node=calloc(1,sizeof(Node));
    node->kind=ND_FUNC_DEF;
    node->funcname=calloc(100,sizeof(char));
    memcpy(node->funcname,tok->str,tok->len);
    node->arg=calloc(6,sizeof(Node));
    for(int i=0;;i++){
        if(i>=6){
            error("stack overflow");
        }
        if(consume(")")){//func(){...}
            break;
        }
        node->arg[i]=expr();
        if(consume(")")){//func(a,b,...){...}
            break;
        }
        expect(",");
    }
    node->lhs=stmt();
    return node;
}

// stmt    = expr ";"
//        | "{" stmt* "}"
//        | "return" expr ";"
//        | "if" "(" expr ")" stmt ("else" stmt)?
//        | "while" "(" expr ")" stmt
//        | "for" "(" expr? ";" expr? ";" expr? ")" stmt
//        | "int" "*"* ident ";"
Node*stmt(){
    Node*node;

    if(consume("{")){
        node=calloc(1,sizeof(Node));
        node->kind=ND_BLOCK;
        /*
        for(int i=0;!consume("}");i++){
            node->block[i]=calloc(1,sizeof(Node));
            node->block[i]=stmt();
        }
        */
        node->block= calloc(300, sizeof(Node));
        for(int i=0;!consume("}");i++){
            if(i>300){
                error("block flowover");
            }
            node->block[i]=stmt();
        }
        return node;
    }

    if(consume_kind(TK_IF)){
        expect("(");
        node=calloc(1,sizeof(Node));
        node->kind=ND_IF;
        node->lhs=expr();
        expect(")");
        node->rhs=stmt();
        if(consume_kind(TK_ELSE)){
            node->rhs=new_binary(ND_ELSE,node->rhs,stmt());
        }
        return node;
    }

    if(consume_kind(TK_WHILE)){
        expect("(");
        node=calloc(1,sizeof(Node));
        node->kind=ND_WHILE;
        node->lhs=expr();
        expect(")");
        node->rhs=stmt();
        return node;
    }

    if(consume_kind(TK_FOR)){
        expect("(");
        node=calloc(1,sizeof(Node));
        node->kind=ND_FOR;
        node->lhs=calloc(1,sizeof(Node));
        node->lhs->lhs=calloc(1,sizeof(Node));
        node->lhs->rhs=calloc(1,sizeof(Node));
        node->rhs=calloc(1,sizeof(Node));
        if(!consume(";")){
            node->lhs->lhs->lhs=expr();
            expect(";");
        }

        if(!consume(";")){
            node->lhs->lhs->rhs=expr();
            expect(";");
        }

        if(!consume(")")){
            node->lhs->rhs=expr();
            expect(")");
        }
        node->rhs=stmt();
        return node;

    }

    if(consume_kind(TK_RETURN)){
        //printf("8\n");
        node=calloc(1,sizeof(Node));
        node->kind=ND_RETURN;
        node->lhs=expr();
        //printf("10\n");
        expect(";");
        return node;
    }
    node=expr();
    //printf("6\n");
    if(!(node->kind==ND_FUNC_DEF)){
        //printf("ok_def\n");
        expect(";");
    }
    return node;
}


Node*expr(){
   return assign();
}

Node*assign(){
    Node*node=equality();
    if(consume("=")){
        //printf("3\n");
        node=new_binary(ND_ASSIGN,node,assign());
        //printf("5\n");
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
    Token*tok=consume_kind(TK_IDENT);

    if(tok){
        //function
        if(consume("(")){
            Node*node=calloc(1,sizeof(Node));
            node->kind=ND_FUNC_CALL;
            node->funcname=calloc(100,sizeof(char));
            memcpy(node->funcname,tok->str,tok->len);
            node->arg=calloc(6,sizeof(Node));
            if(consume(")")){
                return node;
            }
            for(int i=0;;i++){
                node->arg[i]=expr();
                if(consume(")")){
                    return node;
                }
                expect(",");
            }
        }

        //variable
        Node*node=calloc(1,sizeof(Node));
        node->kind=ND_LVAR;
        LVar*lvar=find_lvar(tok);
        if(lvar){
            //printf("9\n");
            node->offset=lvar->offset;
        }
        else{
            //printf("2\n");
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

