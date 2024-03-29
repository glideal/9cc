#include "9cc.h"


char* user_input;
char** input;

Token_t *token;

LVar_t*locals[100];
int cur_func=0;

void expect(char* op){
    if(token->kind != TK_RESERVED || strlen(op)!=token->len||memcmp(token->str,op,token->len)){
        error_at(token->line,"expected \"%s\"",op);
    }
    token=token->next;
}

int expect_number(){
    if(token->kind!=TK_NUM){
        error_at(token->line,"not number");
    }
    int val=token->val;
    token=token->next;
    return val;
}

bool at_eof(){
    return token->kind==TK_EOF;
}

Token_t *new_token(TokenKind kind,Token_t *cur,char *str,int len){
    Token_t *tok=calloc(1,sizeof(Token_t));
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


Token_t *tokenize(){
    char*p=user_input;
    Token_t head;
    head.next=NULL;
    Token_t *cur=&head;
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

        if(strchr("+-*/()<>=;{}&,",*p)){
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

        int keyword=is_keyword(p);
        if(keyword!=0){
            TokenKind kind=identifier(p);
            cur=new_token(kind,cur,p,keyword);
            cur->line=line;
            p+=keyword;
            //printf("keyword_len=%d\n",keyword);
            continue;
        // if(strncmp(p,"return",6)==0&&(!is_alnum(p[6]))){
        //     cur=new_token(TK_RETURN,cur,p,6);
        //     cur->line=line;
        //     p+=6;
        //     continue;
        // }

        // if(strncmp(p,"if",2)==0&&(!is_alnum(p[2]))){
        //     cur=new_token(TK_IF,cur,p,2);
        //     cur->line=line;
        //     p+=2;
        //     continue;
        // }        

        // if(strncmp(p,"else",4)==0&&(!is_alnum(p[4]))){
        //     cur=new_token(TK_ELSE,cur,p,4);
        //     cur->line=line;
        //     p+=4;
        //     continue;
        // }

        // if(strncmp(p,"while",5)==0&&(!is_alnum(p[5]))){
        //     cur=new_token(TK_WHILE,cur,p,5);
        //     cur->line=line;
        //     p+=5;
        //     continue;
        // }
        
        // if(strncmp(p,"for",3)==0&&(!is_alnum(p[3]))){
        //     cur=new_token(TK_FOR,cur,p,3);
        //     cur->line=line;
        //     p+=3;
        //     continue;
        // }
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

        error_at(line,"invalid number");//change for 'error_at' function1
    }
    new_token(TK_EOF,cur,p,0);
    //TokenCheck(head);
    return head.next;
    }

/*_____________________________________________________Node____________________________________________________________*/

Node_t*code[100];


Node_t*new_node(NodeKind kind){//kk
    Node_t*node=calloc(1,sizeof(Node_t));
    node->kind=kind;
    return node;
}

Node_t*new_binary(NodeKind kind,Node_t*lhs,Node_t*rhs){//kk
    Node_t*node=new_node(kind);
    node->lhs=lhs;
    node->rhs=rhs;
    return node;
}

Node_t*new_num(int val){//kk
    Node_t*node=new_node(ND_NUM);
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

Token_t* consume_kind(TokenKind kind){
    if (token->kind != kind){
        return NULL;
    }
    Token_t* tok=token;
    token=token->next;
    return tok;
}


LVar_t *find_lvar(Token_t*tok){
    for(LVar_t *var=locals[cur_func];var;var=var->next){
        if(var->len==tok->len&&!memcmp(tok->str,var->name,var->len)){//the variable exists, then return that variable
            return var;
        }
    }
    return NULL;
}


Node_t*program(){
    // LVar locals_head;
    // locals_head.next=NULL;
    // locals_head.offset=0;
    // locals=&locals_head;

    int i=0;
    while(!at_eof()){
        code[i]=func();
        if(locals[cur_func]!=NULL){
            code[i]->argc=(int)(locals[cur_func]->offset/8);
        }
        cur_func++;
        i++;
    }
    code[i]=NULL;
}

// func = "int" ident "(" ("int" ident ("," "int" ident)*)? ")" stmt
Node_t*func(){
    Node_t*node;
    // if(consume_kind(TK_TYPE)==NULL){
    //     error_at(token->line,"expect type declaration");
    // }
    Token_t* tok=consume_kind(TK_IDENT);
    if(tok==NULL){
        error("not function\n");
    }
    expect("(");
    node=calloc(1,sizeof(Node_t));
    node->kind=ND_FUNC_DEF;
    node->funcname=calloc(100,sizeof(char));
    memcpy(node->funcname,tok->str,tok->len);
    node->argv=calloc(6,sizeof(Node_t));
    
    for(int i=0;;i++){
        if(i>=6){
            error("stack overflow");
        }
        if(consume(")")){//func(){...}
            break;
        }
        if(consume_kind(TK_TYPE)){
            node->argv[i]=define_variable();
        }else{
            error_at(token->line,"expect type of variable");
        }
        
        if(consume(")")){//func(a,...){...}
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
Node_t*stmt(){
    Node_t*node;
    if(consume("{")){
        node=calloc(1,sizeof(Node_t));
        node->kind=ND_BLOCK;
        /*
        for(int i=0;!consume("}");i++){
            node->block[i]=calloc(1,sizeof(Node));
            node->block[i]=stmt();
        }
        */
        node->block= calloc(300, sizeof(Node_t));
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
        node=calloc(1,sizeof(Node_t));
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
        node=calloc(1,sizeof(Node_t));
        node->kind=ND_WHILE;
        node->lhs=expr();
        expect(")");
        node->rhs=stmt();
        return node;
    }

    if(consume_kind(TK_FOR)){
        expect("(");
        node=calloc(1,sizeof(Node_t));
        node->kind=ND_FOR;
        node->lhs=calloc(1,sizeof(Node_t));
        node->lhs->lhs=calloc(1,sizeof(Node_t));
        node->lhs->rhs=calloc(1,sizeof(Node_t));
        node->rhs=calloc(1,sizeof(Node_t));
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
        node=calloc(1,sizeof(Node_t));
        node->kind=ND_RETURN;
        node->lhs=expr();
        //printf("10\n");
        expect(";");
        return node;
    }

    if(consume_kind(TK_TYPE)){
        node=calloc(1,sizeof(Node_t));
        node=define_variable();
        expect(";");
        return node;
    }
    node=expr();
    //printf("6\n");
    // if(!(node->kind==ND_FUNC_DEF)){
    //     //printf("ok_def\n");
    //     expect(";");
    // }
    expect(";");
    return node;
}

// expr       = assign
Node_t*expr(){
   return assign();
}

// assign     = equality ("=" assign)?
Node_t*assign(){
    Node_t*node=equality();
    if(consume("=")){
        //printf("3\n");
        node=new_binary(ND_ASSIGN,node,assign());
        //printf("5\n");
    }
    return node;
}

// equality   = relational ("==" relational | "!=" relational)*
Node_t*equality(){
    Node_t*node=relational();

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

// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
Node_t*relational(){
    Node_t*node=add();

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

// add        = mul ("+" mul | "-" mul)*
Node_t*add(){
    Node_t*node=mul();

    for(;;){
        int type_size=1;
        if(node->kind==ND_LVAR&&node->type->ty==PTR){
            switch(node->type->ptr_to->ty){
                case INT:{
                    type_size=4;
                    break;
                }
                case PTR:{
                    type_size=8;
                    break;
                }
            }
        }
        if(consume("+")){
            Node_t*n=calloc(1,sizeof(Node_t));
            n=new_binary(ND_MUL,new_num(type_size),mul());
            node=new_binary(ND_ADD,node,n);
        }
        else if(consume("-")){
            Node_t*n=calloc(1,sizeof(Node_t));
            n=new_binary(ND_MUL,new_num(type_size),mul());
            node=new_binary(ND_SUB,node,n);
        }
        else{
            return node;
        }
    }
}

// mul        = unary ("*" unary | "/" unary)*
Node_t*mul(){
    Node_t*node=unary();

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

// unary ="sizeof" unary
//       | "+"? primary
//       | "-"? primary
//       | "*" unary
//       | "&" unary
Node_t*unary(){
    if (consume_kind(TK_SIZEOF)) {
        Node_t*n = unary();
        int type_size = n->type && n->type->ty == PTR ? 8 : 4;
        return new_num(type_size);
    }
    if (consume("+")){
        return unary();
    }
    if (consume("-")){
        return new_binary(ND_SUB,new_num(0),unary());
    }
    if(consume("&")){
        Node_t*node;
        node=calloc(1,sizeof(Node_t));
        node->kind=ND_ADDR;
        node->lhs=unary();
        return node;
    }
    if(consume("*")){
        Node_t*node;
        node=calloc(1,sizeof(Node_t));
        node->kind=ND_DEREF;
        node->lhs=unary();
        return node;
    }
    return primary();
}

// primary = num
//        | ident ("(" expr* ")")?
//        | "(" expr ")"
Node_t*primary(){//kk
    if (consume("(")){
        Node_t*node=expr();
        expect(")");
        return node;
    }

    Token_t*tok=consume_kind(TK_IDENT);
    if(tok){
        //call_function
        if(consume("(")){
            Node_t*node=calloc(1,sizeof(Node_t));
            node->kind=ND_FUNC_CALL;
            node->funcname=calloc(100,sizeof(char));
            memcpy(node->funcname,tok->str,tok->len);
            node->argv=calloc(6,sizeof(Node_t));
            if(consume(")")){
                return node;
            }
            for(int i=0;;i++){
                node->argv[i]=expr();
                if(consume(")")){
                    return node;
                }
                expect(",");
            }
        }
        return call_variable(tok);
    }
    return new_num(expect_number());
}

Node_t*define_variable(){
    Type_t*type;
    type=calloc(1,sizeof(Type_t));
    type->ty=INT;
    type->ptr_to=NULL;
    while(consume("*")){
        Type_t*t;
        t=calloc(1,sizeof(Type_t));
        t->ty=PTR;
        t->ptr_to=type;
        type=t;
    }
    Token_t*tok=consume_kind(TK_IDENT);
    if(tok==NULL){
        error("invalid define variable");
    }
    Node_t*node=calloc(1,sizeof(Node_t));
    node->kind=ND_LVAR;
    LVar_t*lvar=find_lvar(tok);
    if(lvar){
        char*name[100];
        memcmp(name,tok->str,tok->len);
        error_at(tok->line,"already defined variable %s",name);
    }
    else{
        //printf("2\n");
        lvar=calloc(1,sizeof(LVar_t));
        lvar->next=locals[cur_func];
        lvar->name=tok->str;
        lvar->len=tok->len;
        if(locals[cur_func]==NULL){
            lvar->offset=8;
        }else{
            lvar->offset=locals[cur_func]->offset+8;
        }
        lvar->type=type;
        node->offset=lvar->offset;
        node->type=lvar->type;
        locals[cur_func]=lvar;
    }
    return node;
}

Node_t*call_variable(Token_t*tok){
    Node_t*node=calloc(1,sizeof(Node_t));
    node->kind=ND_LVAR;
    LVar_t*lvar=find_lvar(tok);
    if(lvar){
        node->offset=lvar->offset;
        node->type=lvar->type;
    }
    else{
        char*name[100];
        memcmp(name,tok->str,tok->len);
        error_at(tok->line,"not defined variable %s",name);
    }
    return node;
}

