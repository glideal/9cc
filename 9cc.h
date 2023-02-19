#ifndef _9CC_H_
#define _9CC_H_

#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdarg.h>


typedef enum {
    TK_RESERVED,  //token(symbol)
    TK_IF,        //制御構文
    TK_ELSE,
    TK_WHILE,
    TK_FOR, 
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
    int line;
};

//Token *token;
void error_line(char*p);
Token*error_tokenize();
void error(char* fmt,...);
void error_at(int line,char*fmt,...);

void expect(char* op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind,Token *cur,char *str,int len);
bool startswith(char*p,char*q);
void StrIdent(char* s,char** endptr);
int is_alnum(char c);
int is_keyward(char*p);
TokenKind identifier(char*p);
Token *tokenize();

typedef struct ReservedWord ReservedWord;
struct ReservedWord{
    char*word;
    TokenKind kind;
};

/*______________________________________________Node_________________________________________________*/

typedef enum{
    ND_IF,
    ND_ELSE,
    ND_WHILE,
    ND_FOR,

    ND_BLOCK,//5

    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,

    ND_ASSIGN,// =        //10
    ND_LVAR,   // ローカル変数
    ND_FUNC_CALL,
    ND_FUNC_DEF,

    ND_EQ,
    ND_NE,
    ND_LT,//15
    ND_LE,
    ND_RETURN,

    ND_ADDR,//address
    ND_DEREF,//dereference
    ND_NUM,
}NodeKind;

//typedef struct Node Node;

typedef struct Node Node;

struct Node{
    NodeKind kind;
    Node*lhs;
    Node*rhs;
    Node**block;
    Node**argv;
    int argc;
    char*funcname;
    int val;
    int offset;
};

Node*new_node(NodeKind kind);
Node*new_num(int val);
Node*new_binary(NodeKind kind,Node*lhs,Node*rhs);

typedef struct LVar LVar;

struct LVar{
    struct LVar*next;
    char*name;
    int len;
    int offset;
};

typedef struct Func Func;

struct Func{
    struct Func*next;
    char*name;
    int argunum;
    int offset;
};

LVar *find_lvar(Token*tok);

bool consume(char* op);
Token* consume_kind(TokenKind kind);

Node*program();
Node*func();
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
/*__________________________check______________________________*/
void TokenCheck(Token head);
void Check();
void CheckNode(Node*node);


#endif // _9CC_H_