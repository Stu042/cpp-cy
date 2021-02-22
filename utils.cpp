//
// Created by stu on 21/02/2021.
//

#include "utils.h"
#include <cstdio>
#include <cstdarg>
#include "console.h"


void PrintError(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
	printf("\x1B[31m");
    vfprintf(stdout, fmt, args);
	puts("\033[0m\n");
    va_end(args);
}

void PrintDebug(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}

long FileSize(FILE* fptr) {
    fseek(fptr, 0L, SEEK_END);
    long size = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);
    return size;
}

char *NewCharArray(const char *from, char *to) {
    int len = to - from;
    char* dest = new char[len + 1];
    strncpy(dest, from, len);
    dest[len] = '\0';
    return dest;
}

char *NewCharArray(const char *from, int len) {
    char* dest = new char[len + 1];
    strncpy(dest, from, len);
    dest[len] = '\0';
    return dest;
}

// free returned mem with delete
char* GetStringMem(const char *fmt, va_list args) {
	int size = snprintf(nullptr, 0, fmt, args);
	char* mem = new char[size + 1];
	return mem;
}