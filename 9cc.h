#ifndef _9CC_H_
#define _9CC_H_

#include<stdbool.h>


typedef enum {
    TK_RESERVED,  //token(symbol)
    TK_IDENT,     //識別子
    TK_NUM,       //integer token
    TK_RETURN,
    TK_EOF,       //token to remark end of input
}TokenKind;

//typedef struct Token Token;

typedef struct Token Token;

struct Token{
    TokenKind kind;
    struct Token *next;
    int val;
    char *str;
    int len;
};

//Token *token;

void error(char* fmt,...);

void error_at(char*loc,char*fmt,...);

bool consume(char* op);

Token*consume_ident();

void expect(char* op);

int expect_number();

bool at_eof();

Token *new_token(TokenKind kind,Token *cur,char *str,int len);

bool startswith(char*p,char*q);

typedef struct LVar LVar;

char*StrIdent(char*restrict s,char**restrict endptr);

int is_alnum(char c);

Token *tokenize();

/*______________________________________________Node_________________________________________________*/

typedef enum{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_ASSIGN,// =
    ND_LVAR,   // ローカル変数
    ND_EQ,
    ND_NE,
    ND_LT,
    ND_LE,
    ND_RETURN,
    ND_NUM,
}NodeKind;

//typedef struct Node Node;

typedef struct Node Node;

struct Node{
    NodeKind kind;
    struct Node*lhs;
    struct Node*rhs;
    int val;
    int offset;
};

Node*new_node(NodeKind kind);

Node*new_binary(NodeKind kind,Node*lhs,Node*rhs);

Node*new_num(int val);

struct LVar{
    struct LVar*next;
    char*name;
    int len;
    int offset;
};

LVar *find_lvar(Token*tok);

bool consume_return();

Node*program();
Node*stmt();
Node*expr();
Node*assign();
Node*equality();
Node*relational();
Node*add();
Node*mul();
Node*unary();
Node*primary();

void gen(Node*node);


#endif // _9CC_H_