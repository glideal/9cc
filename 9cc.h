#ifndef _9CC_H_
#define _9CC_H_

#include<stdbool.h>


typedef enum {
    TK_RESERVED,  //token(symbol)
    TK_NUM,       //integer token
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

void expect(char* op);

int expect_number();

bool at_eof();

Token *new_token(TokenKind kind,Token *cur,char *str,int len);

bool startswith(char*p,char*q);

Token *tokenize();

/*______________________________________________Node_________________________________________________*/

typedef enum{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQ,
    ND_NE,
    ND_LT,
    ND_LE,
    ND_NUM,
}NodeKind;

//typedef struct Node Node;

typedef struct Node Node;

struct Node{
    NodeKind kind;
    struct Node*lhs;
    struct Node*rhs;
    int val;
};

Node*new_node(NodeKind kind);

Node*new_binary(NodeKind kind,Node*lhs,Node*rhs);

Node*new_num(int val);


Node*expr();
Node*equality();
Node*relational();
Node*add();
Node*mul();
Node*unary();
Node*primary();

void gen(Node*node);


#endif // _9CC_H_