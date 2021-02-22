//
// Created by stu on 21/02/2021.
//

#ifndef CY_UTILS_H
#define CY_UTILS_H
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cstring>



extern void PrintError(const char* fmt, ...);
extern void PrintDebug(const char* fmt, ...);
extern long FileSize(FILE* fptr);
extern char *NewCharArray(const char *from, char *to);
extern char *NewCharArray(const char *from, int len);
extern char* GetStringMem(const char *fmt, va_list args);

#endif //CY_UTILS_H
