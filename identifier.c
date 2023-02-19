#include "9cc.h"

int is_keyward(char *p){
    static char*kw[]={
        "return", "if", "else", "for", "while"
    };

    for(int i=0;i<sizeof(kw)/sizeof(*kw);i++){
        if(strncmp(p,kw[i],strlen(kw[i]))==0){
            return strlen(kw[i]);
        }
    }
    return 0;
}

ReservedWord reservedword[]={
  {"return", TK_RETURN},
  {"if", TK_IF},
  {"else", TK_ELSE},
  {"while", TK_WHILE},
  {"for", TK_FOR},
  {"",TK_EOF},
};

TokenKind identifier(char*p){
    for(int i=0;reservedword[i].kind!=TK_EOF;i++){
        if(strncmp(p,reservedword[i].word,strlen(reservedword[i].word))==0){
            return reservedword[i].kind;
        }
    }
}