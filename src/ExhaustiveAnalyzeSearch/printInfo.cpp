#include "printInfo.h"
#include <defs.h>

bool SetConsoleColor(unsigned short color)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    return (SetConsoleTextAttribute(handle, color)==TRUE)?true:false; // BACKGROUND_BLUE
}

int fprintStrA(FILE *stream, const char* str)
{
    return fprintf(stream, "%s", str);
}

int fprintStrW(FILE *stream, const wchar_t* str)
{
    return fwprintf(stream, L"%s", str);
}

int fprintColorA(WORD Color, FILE *stream, const char* format, va_list args)
{
    int ncount = 0;
    int needsize = 0;
    char* msgstr = NULL;

    needsize = _vscprintf(format, args);
    msgstr = (char*)AllocMemory(needsize+1);
    if (msgstr)
    {
        _vsnprintf(msgstr, needsize+1, format, args);

        if (Color == -1)
        {
            ncount = fprintStrA(stream, msgstr);
        }
        else
        {
            SetConsoleColor(Color);
            ncount = fprintStrA(stream, msgstr);
            SetConsoleColor(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
        }

        FreeMemory(msgstr);
    }

    return ncount;
}

int fprintColorW(WORD Color, FILE *stream, const wchar_t* format, va_list args)
{
    int ncount = 0;
    int needsize = 0;
    wchar_t* msgstr = NULL;

    needsize = _vscwprintf(format, args);
    msgstr = (wchar_t*)AllocMemory((needsize+1)*sizeof(wchar_t));
    if (msgstr)
    {
        _vsnwprintf(msgstr, needsize+1, format, args);

        if (Color == -1)
        {
            ncount = fprintStrW(stream, msgstr);
        }
        else
        {
            SetConsoleColor(Color);
            ncount = fprintStrW(stream, msgstr);
            SetConsoleColor(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
        }

        FreeMemory(msgstr);
    }

    return ncount;
}

int fprintErrA(FILE *stream, const char *format, ...)
{
    int ncount = 0;
    va_list args;
    int needsize = 0;

    va_start(args, format);
    ncount = fprintColorA(FOREGROUND_RED|FOREGROUND_INTENSITY, stream, format, args);
    va_end(args);  

    return ncount;
}

int fprintErrW(FILE *stream, const wchar_t *format, ...)
{
    int ncount = 0;
    va_list args;
    int needsize = 0;

    va_start(args, format);
    ncount = fprintColorW(FOREGROUND_RED|FOREGROUND_INTENSITY, stream, format, args);
    va_end(args);  

    return ncount;
}


int fprintInfoA(FILE *stream, const char *format, ...)
{
    int ncount = 0;
    va_list args;
    int needsize = 0;

    va_start(args, format);
    ncount = fprintColorA(FOREGROUND_BLUE|FOREGROUND_INTENSITY, stream, format, args);
    va_end(args);  

    return ncount;
}

int fprintInfoW(FILE *stream, const wchar_t *format, ...)
{
    int ncount = 0;
    va_list args;
    int needsize = 0;

    va_start(args, format);
    ncount = fprintColorW(FOREGROUND_BLUE|FOREGROUND_INTENSITY, stream, format, args);
    va_end(args);  

    return ncount;
}

int fprintDebugA(FILE *stream, const char *format, ...)
{
    int ncount = 0;
    va_list args;
    int needsize = 0;

    va_start(args, format);
    ncount = fprintColorA(-1, stream, format, args);
    va_end(args);  

    return ncount;
}

int fprintDebugW(FILE *stream, const wchar_t *format, ...)
{
    int ncount = 0;
    va_list args;
    int needsize = 0;

    va_start(args, format);
    ncount = fprintColorW(-1, stream, format, args);
    va_end(args);  

    return ncount;
}