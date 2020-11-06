#ifndef _PRINT_INFO_HEADER_HH_H_
#define _PRINT_INFO_HEADER_HH_H_ 
#include <stdio.h>

bool SetConsoleColor(unsigned short color);

int fprintErrA(FILE *stream, const char *format, ...);
int fprintErrW(FILE *stream, const wchar_t *format, ...);
int fprintInfoA(FILE *stream, const char *format, ...);
int fprintInfoW(FILE *stream, const wchar_t *format, ...);
int fprintDebugA(FILE *stream, const char *format, ...);
int fprintDebugW(FILE *stream, const wchar_t *format, ...);


#define printErrA(format, ...)  fprintErrA(stdout, format, __VA_ARGS__)
#define printErrW(format, ...)  fprintErrW(stdout, format, __VA_ARGS__)
#define printInfoA(format, ...)  fprintInfoA(stdout, format, __VA_ARGS__)
#define printInfoW(format, ...)  fprintInfoW(stdout, format, __VA_ARGS__)
#define printDebugA(format, ...)  fprintDebugA(stdout, format, __VA_ARGS__)
#define printDebugW(format, ...)  fprintDebugW(stdout, format, __VA_ARGS__)

#ifdef _UNICODE
#define fprintErr fprintErrW
#define printErr printErrW
#else
#define fprintErr fprintErrA
#define printErr printErrA
#endif


#endif // _PRINT_INFO_HEADER_HH_H_
