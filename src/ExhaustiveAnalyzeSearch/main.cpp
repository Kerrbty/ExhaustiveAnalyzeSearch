#include <defs.h>
#include "JsonSearch.h"
#include "PEVersionInfo.h"


void __stdcall printaddr(const char* name, void* addr, void* user_info)
{
    int n = 0;
    if (addr)
    {
        n = printf("\t{\"%s\",", name);
    }
    else
    {
        n = printf("\t{nullptr,", name);
    }
    for (int i=n; i<64; i++)
    {
        putchar(' ');
    }
    printf("0x%-8x},\n", addr);
}

void AnalyzeFile(const char* filename, const char* rulefile)
{
    unsigned long dwVersionMS, dwVersionLS;
    GetPEVersionA(filename, &dwVersionMS, &dwVersionLS);
    printf("//%u.%u.%u.%u offset.h \n", 
        HIWORD(dwVersionMS),
        LOWORD(dwVersionMS),
        HIWORD(dwVersionLS),
        LOWORD(dwVersionLS)
        );
    printf("wxversion_table wx%u%u%u%u_table[] = { \n", 
        HIWORD(dwVersionMS),
        LOWORD(dwVersionMS),
        HIWORD(dwVersionLS),
        LOWORD(dwVersionLS)
        );
    SearchByJsonA(rulefile, filename, printaddr, NULL);
    printaddr(NULL, 0, NULL);
    printf("};\n");
}

int main(int argc, char* argv[])
{
    for (int i=1; i<argc; i++)
    {
        AnalyzeFile(argv[i], "OffsetRule.json");
    }

    return 0;
}