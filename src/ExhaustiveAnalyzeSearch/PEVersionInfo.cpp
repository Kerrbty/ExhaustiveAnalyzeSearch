#include "PEVersionInfo.h"
#include <defs.h>
#include <StringFormat/StringFormat.h>
#pragma comment(lib, "Version.lib")

bool GetPEVersionA(const char* filename, unsigned long* dwVersionMS, unsigned long* dwVersionLS)
{
    CMultiAndWide str(filename);
    return GetPEVersionW(str.GetWideChar(), dwVersionMS, dwVersionLS);
}

bool GetPEVersionW(const wchar_t* filename, unsigned long* dwVersionMS, unsigned long* dwVersionLS)
{
    bool GetVerSuccess = false;
    DWORD dwHandle;
    DWORD dwSize = GetFileVersionInfoSizeW(filename, &dwHandle);
    if (dwSize<=0)
    {
        return GetVerSuccess;
    }

    PWSTR pVersionData = (PWSTR)AllocMemory(dwSize*sizeof(WCHAR));
    if(GetFileVersionInfoW(filename, dwHandle, dwSize,(void**)pVersionData))  
    {
        UINT dwQuerySize = 0;  
        VS_FIXEDFILEINFO *pTransTable ;
        if (VerQueryValueW(pVersionData, L"\\",(LPVOID*)&pTransTable, &dwQuerySize))  
        {  
            WORD WinMajor = HIWORD(pTransTable->dwProductVersionMS);//major version number    
            WORD WinMinor = LOWORD(pTransTable->dwProductVersionMS); //minor version number

            if (dwVersionMS != NULL)
            {
                *dwVersionMS = pTransTable->dwProductVersionMS;
            }
            if (dwVersionLS != NULL)
            {
                *dwVersionLS = pTransTable->dwProductVersionLS;
            }
            GetVerSuccess = TRUE;
        } 
    }
    FreeMemory(pVersionData);
    return GetVerSuccess;
}
