#ifndef _RELOAD_MYSELF_DLL_MODEL_
#define _RELOAD_MYSELF_DLL_MODEL_
#include <windows.h>
#include <tchar.h>

HMODULE WINAPI LoadDllFromFileA(PSTR szFileName, BOOL bCallDllMain = TRUE); // ����DLL�ļ�
HMODULE WINAPI LoadDllFromFileW(PWSTR szFileName, BOOL bCallDllMain = TRUE); // ����DLL�ļ�
HMODULE WINAPI LoadDllFromMemory(LPVOID pMemAddr, BOOL bCallDllMain = TRUE); // �ڴ�PE�ļ��������� 
LPVOID WINAPI MyGetProcAddress(HMODULE hmodule, PSTR szFuncName); // �õ�����������ַ or Orid 
BOOL WINAPI FreeMyDlls(HMODULE hmodule, BOOL bCallDllMain = TRUE); // ж��dll


#endif  // _RELOAD_MYSELF_DLL_MODEL_