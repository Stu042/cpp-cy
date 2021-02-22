//
// Created by stu on 21/02/2021.
//
#include <cstdio>
#include <cstring>
#include <vector>
#include "scanner.h"
#include "utils.h"
#include "config.h"
using namespace std;

std::vector<SourceFile> AllSrc;


typedef struct {
	int curidx;		// current index of lexeme in file
	int line;		// line idx + 1
	int startidx;	// index of start of lexeme in file
	SourceFile *source;
} Cursor;



void ScannerError(Cursor& cursor, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	if (ConfigColour) {
		printf("\x1B[31m");
	}
	printf("ScannerError: Line:%d, ", cursor.line);
	vfprintf(stdout, fmt, args);
	if (ConfigColour) {
		printf("\033[0m");
	}
	va_end(args);
	putc('\n', stdout);
}


// cleanup scanner of all created SourceFiles
void CleanScanner() {
    while(!AllSrc.empty()) {
        SourceFile sf = AllSrc.back();
        FreeSourceFile(sf);
        AllSrc.pop_back();
    }
}


// Create a new SourceFile struct and add to AllSrc
int NewSourceFile(const char *filename, const char *text, long filesize) {
    int idx;
    auto sf = SourceFile {
            .fileName = filename,
            .file = text,
            .size = filesize
    };
    idx = AllSrc.size();
    AllSrc.push_back(sf);
    return idx;
}

// Frees contents of SourceFile
void FreeSourceFile(SourceFile &sf) {
    delete[] sf.file;
}


// SourceFile file must be null terminated (standard), returns a newly allocated line of the input sourcefile. delete when finished.
char* GetLineFromSourceFile(SourceFile& sf, int lineNumber) {
    char *line;
    const char* s = sf.file;
    while (*s != '\0' && --lineNumber > 0) {
        do {
            s++;
        } while (*s != '\0' && *s != '\n');
    }
    const char* start = s;
    do {
        s++;
    } while (*s != '\0' && *s != '\n');
    long size = s - start;
    line = new char[size];
    memcpy(line, start, size);
    return line;
}



int GetCharOffset(const char *text, int index) {
	int offset = 0;
	while (index >=0 && text[index] != '\n') {
		index--;
		offset++;
	}
	return offset;
}

Token AddToken(TokenType tokenType, Cursor& cursor) {
	int length = cursor.curidx - cursor.startidx;
    Token tok = {
			.source = cursor.source,
			.type = tokenType,
			.lineNumber = cursor.line,
			.offset = GetCharOffset(cursor.source->file, cursor.startidx),
			.lexeme = NewCharArray(cursor.source->file, length),
	};
    return tok;
}

Token AddToken(TokenType tokenType, void* value, Cursor& cursor) {
	int length = cursor.curidx - cursor.startidx;
	Token tok = {
		.source = cursor.source,
		.type = tokenType,
		.lineNumber = cursor.line,
		.offset = GetCharOffset(cursor.source->file, cursor.startidx),
		.lexeme = NewCharArray(cursor.source->file, length),
		.value = value,
	};
	return tok;
}


bool NotAtEnd(Cursor& cursor) {
	return (cursor.curidx < cursor.source->size);
}
bool IsAtEnd(Cursor& cursor) {
	return (cursor.curidx >= cursor.source->size);
}
bool IsNextAtEnd(Cursor& cursor) {
	return (cursor.curidx+1 >= cursor.source->size);
}
char Peek(Cursor& cursor) {
	if (IsAtEnd(cursor))
		return '\0';
	return cursor.source->file[cursor.curidx];
}
char PeekNext(Cursor& cursor) {
	if (IsAtEnd(cursor) || IsNextAtEnd(cursor))
		return '\0';
	return cursor.source->file[cursor.curidx + 1];
}
char Advance(Cursor& cursor) {
	return (cursor.source->file[cursor.curidx++]);
}
bool Match(char tomatch, Cursor& cursor) {
	if (IsAtEnd(cursor))
		return false;
	if (cursor.source->file[cursor.curidx] == tomatch) {
		cursor.curidx++;
		return true;
	}
	return false;
}

void CreateString(Cursor& cursor) {
	while (Peek(cursor) != '"' && !IsAtEnd(cursor)) {
		if (Peek(cursor) == '\n')
			cursor.line++;
		Advance(cursor);
	}
	if (IsAtEnd(cursor)) {
		ScannerError(cursor, (char *)"Unterminated string.");
		return;
	}
	Advance(cursor);
	char* value = NewCharArray(cursor.source->file, cursor.curidx - cursor.startidx);
	AddToken(TOKEN_STRING, value, cursor);
}

void ScanSourceFile(int sourcefileidx) {
    SourceFile sf = AllSrc[sourcefileidx];
    Cursor cursor;
    cursor.curidx = 0;
    cursor.line = 1;
    cursor.startidx = 0;
    cursor.source = &AllSrc[sourcefileidx];
    const char *file = sf.file;
    std::vector<Token> tokens;
    while (NotAtEnd(cursor)) {
        Token tok;
        char c = Advance(cursor);
        switch (c) {
			case ':':
				tok = AddToken(TOKEN_COLON, cursor);
				break;
			case '(':
                tok = AddToken(TOKEN_LPAREN, cursor);
                break;
            case ')':
                tok = AddToken(TOKEN_RPAREN, cursor);
                break;
            case '{':
                tok = AddToken(TOKEN_LBRACE, cursor);
                break;
            case '}':
                tok = AddToken(TOKEN_RBRACE, cursor);
                break;
            case ',':
                tok = AddToken(TOKEN_COMMA, cursor);
                break;
            case '.':
                tok = AddToken(TOKEN_DOT, cursor);
                break;
            case '-':
                tok = AddToken(TOKEN_MINUS, cursor);
                break;
            case '+':
                tok = AddToken(TOKEN_PLUS, cursor);
                break;
            case ';':
                tok = AddToken(TOKEN_SEMICOLON, cursor);
                break;
            case '\n':
				cursor.line++;
				tok = AddToken(TOKEN_NEWLINE, cursor);
                break;
            case '\t':
                tok = AddToken(TOKEN_INDENT, cursor);
                break;
            case '*':
                tok = AddToken(TOKEN_STAR, cursor);
                break;
			case '/':
				if (Match('/', cursor)) {	// A comment goes until the end of the line.
					while (Peek(cursor) != '\n' && !IsAtEnd(cursor))
						Advance(cursor);
				} else if (Match('*', cursor)) {
					while (Peek(cursor) != '*'  && PeekNext(cursor) != '/' && !IsAtEnd(cursor))
						Advance(cursor);
				} else {
					AddToken(TOKEN_FORWARDSLASH, cursor);
				}
				break;

			case '!':
				AddToken((Match('=', cursor) ? TOKEN_BANG_EQUAL : TOKEN_BANG), cursor);
				break;
			case '=':
				AddToken((Match('=', cursor) ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL), cursor);
				break;
			case '<':
				AddToken((Match('=', cursor) ? TOKEN_LESS_EQUAL : TOKEN_LESS), cursor);
				break;
			case '>':
				AddToken((Match('=', cursor) ? TOKEN_GREATER_EQUAL : TOKEN_GREATER), cursor);
				break;

			case '"':
				CreateString(cursor);
				break;


			default:
            	ScannerError(cursor, (const char*)"ScanSourceFile() Unknown character found '%c'");
                cursor.startidx++;
        }
        tokens.push_back(tok);
    }
}



// Given a file name, will open a file and returned a filled in SourceFile. Free with FreeSourceFile()
bool OpenSourceFile(const char* fileName) {
	PrintDebug(
			(const char *) "Opening %s\n",
			fileName);
    FILE *fptr = fopen(fileName, "r");
    if (fptr) {
		PrintDebug(
				"\tOpened\n");
        long size = FileSize(fptr);
        char* file = new char[size + 2];
        char* c = file;
        do {
            *c = (char)fgetc(fptr);
        } while(*c++ != EOF);
        fclose(fptr);
		*(c-1) = '\n';
        *c = '\0';
		PrintDebug(
				"File Name %s:\n%s\n",
				fileName,
				file);
		PrintDebug(
				"-------------------\n");
        int newSourceFileIdx = NewSourceFile(fileName, file, size);
        ScanSourceFile(newSourceFileIdx);
        return true;
    }
	PrintError(
			"\tFailed to open file: %s\n",
			fileName);
    return false;
}
