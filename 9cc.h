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
    TK_SIZEOF,

    TK_TYPE,
    TK_EOF,       //token to remark end of input
}TokenKind;


//typedef struct Token Token;

typedef struct Token Token_t;

struct Token{
    TokenKind kind;
    Token_t*next;
    int val;
    char *str;
    int len;
    int line;//for finction Error
};

//Token *token;
void error_line(char*p);
Token_t*error_tokenize();
void error(char* fmt,...);
void error_at(int line,char*fmt,...);

void expect(char* op);
int expect_number();
bool at_eof();
Token_t *new_token(TokenKind kind,Token_t *cur,char *str,int len);
bool startswith(char*p,char*q);
void StrIdent(char* s,char** endptr);
int is_alnum(char c);
int is_keyword(char*p);
TokenKind identifier(char*p);
Token_t *tokenize();

typedef struct ReservedWord ReservedWord_t;
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

typedef struct Node Node_t;

typedef struct Type Type_t;
struct Type{
    enum{INT,PTR}ty;
    Type_t *ptr_to;
};

struct Node{
    NodeKind kind;
    Node_t*lhs;
    Node_t*rhs;
    Node_t**block;
    Node_t**argv;
    int argc;
    char*funcname;
    int val;
    int offset;
    Type_t*type;
};

Node_t*new_node(NodeKind kind);
Node_t*new_num(int val);
Node_t*new_binary(NodeKind kind,Node_t*lhs,Node_t*rhs);

typedef struct LVar LVar_t;

struct LVar{
    LVar_t*next;//struct Lvar*next
    char*name;
    int len;
    int offset;
    Type_t*type;
};


LVar_t *find_lvar(Token_t*tok);

bool consume(char* op);
Token_t* consume_kind(TokenKind kind);

Node_t*define_variable();
Node_t*call_variable(Token_t*tok);

Node_t*program();
Node_t*func();
Node_t*stmt();
Node_t*expr();
Node_t*assign();
Node_t*equality();
Node_t*relational();
Node_t*add();
Node_t*mul();
Node_t*unary();
Node_t*primary();

void gen(Node_t*node);
/*__________________________check______________________________*/
void TokenCheck(Token_t head);
void Check();
void CheckNode(Node_t*node);


#endif // _9CC_H_