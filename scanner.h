//
// Created by stu on 21/02/2021.
//
#ifndef CY_SCANNER_H
#define CY_SCANNER_H
#include <vector>
#include "utils.h"


typedef struct {
    const char *fileName;
    const char *file;
    long size;
}SourceFile;


typedef enum {
    // Single-character tokens.
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS, TOKEN_SEMICOLON, TOKEN_STAR,
    TOKEN_BANG, TOKEN_FORWARDSLASH, TOKEN_BACKSLASH,
    TOKEN_HASH, TOKEN_NEWLINE, TOKEN_INDENT, TOKEN_COLON,

    // One or two character tokens.
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,

    // Literals.
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

    // Keywords.
    TOKEN_FUN, TOKEN_OBJECT,
    TOKEN_WHILE, TOKEN_IF, TOKEN_ELSE, TOKEN_FOR, TOKEN_RETURN,
    TOKEN_NULL, TOKEN_TRUE, TOKEN_FALSE,
}TokenType;



typedef struct {
    SourceFile *source;	// source file
    TokenType type;		// token type
    int lineNumber;		// line number lexeme is on, starts at 1
    int offset;			// offset into line for start of lexeme
    const char* lexeme;	// str copy of lexeme
    void *value;
}Token;




// protos
extern std::vector<SourceFile> AllSrc;

extern void CleanScanner();
extern int  NewSourceFile(const char* fileName, const char *file);
extern void FreeSourceFile(SourceFile& sf);
extern bool OpenSourceFile(const char* fileName);

extern char* GetLineFromSourceFile(SourceFile& sf, int lineNumber);

#endif //CY_SCANNER_H
