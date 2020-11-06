#ifndef _RELOAD_MYSELF_DLL_MODEL_
#define _RELOAD_MYSELF_DLL_MODEL_
#include <windows.h>
#include <tchar.h>

HMODULE WINAPI LoadDllFromFileA(PSTR szFileName, BOOL bCallDllMain = TRUE); // 加载DLL文件
HMODULE WINAPI LoadDllFromFileW(PWSTR szFileName, BOOL bCallDllMain = TRUE); // 加载DLL文件
HMODULE WINAPI LoadDllFromMemory(LPVOID pMemAddr, BOOL bCallDllMain = TRUE); // 内存PE文件加载运行 
LPVOID WINAPI MyGetProcAddress(HMODULE hmodule, PSTR szFuncName); // 得到导出函数地址 or Orid 
BOOL WINAPI FreeMyDlls(HMODULE hmodule, BOOL bCallDllMain = TRUE); // 卸载dll


#endif  // _RELOAD_MYSELF_DLL_MODEL_