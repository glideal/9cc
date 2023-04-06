#include "9cc.h"

int is_keyword(char *p){
    static char*kw[]={
        "return", "if", "else", "for", "while","int","sizeof"
    };

    for(int i=0;i<sizeof(kw)/sizeof(*kw);i++){
        if(strncmp(p,kw[i],strlen(kw[i]))==0){
            return strlen(kw[i]);
        }
    }
    return 0;
}

/*--------------------
struct ReservedWord{
    char*word;
    TokenKind kind;
};
--------------------*/

ReservedWord_t reservedword[]={
  {"return", TK_RETURN},
  {"if", TK_IF},
  {"else", TK_ELSE},
  {"while", TK_WHILE},
  {"for", TK_FOR},
  {"sizeof",TK_SIZEOF},
  {"int",TK_TYPE},

  {"",TK_EOF}
};

TokenKind identifier(char*p){
    for(int i=0;reservedword[i].kind!=TK_EOF;i++){
        if(strncmp(p,reservedword[i].word,strlen(reservedword[i].word))==0){
            return reservedword[i].kind;
        }
    }
}