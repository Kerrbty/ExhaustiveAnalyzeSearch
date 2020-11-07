#include <defs.h>
#include <lib_json/json/json.h>
#include "printInfo.h"
#include "MemoryInfo.h"
#include "PEFileMemoryInfo.h"
#include "MemorySearch.h"


bool __stdcall printaddr(void* addr, void* user_info)
{
    printInfoA("find success£º %p\n", addr);
    return true;
}


int main()
{

//     PEFileMemoryInfo curproc("D:\\Program Files (x86)\\WeChat\\WeChat.exe");
//     MemoryInfo curproc;
//     std::vector<meminfo> section;
//     curproc.SearchMemory(MEMORY_EXECUTEABLE|MEMORY_READABLE, section);

    MemorySearch memsearch("D:\\Program Files (x86)\\WXWork3.0.31.2998\\WXWork.exe");
    memsearch.SearchTargetBytes("89 3d (.{8}) 89 35 .{8} e8 .{8}", printaddr, NULL);

//     MemorySearch memsearch("D:\\Program Files (x86)\\WeChat\\WeChat.exe");
//     std::vector<meminfo> section;
//     memsearch.InitializeTargetMemory(MEMORY_EXECUTEABLE|MEMORY_READABLE, section);
// 
//     HANDLE hFile = FileOpen(TEXT("Search.json"), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
//     if (hFile != INVALID_HANDLE_VALUE)
//     {
//         DWORD dwFileSize = GetFileSize(hFile, NULL);
//         LPBYTE JsonBuf = (LPBYTE)AllocMemory(dwFileSize+1);
//         if (JsonBuf)
//         {
//             DWORD dwBytes = 0;
//             ReadFile(hFile, JsonBuf, dwFileSize, &dwBytes, NULL);
//             Json::Reader reader;
//             Json::Value SearchBlock;
//             if (reader.parse((char*)JsonBuf, SearchBlock))
//             {
//                 if (!SearchBlock.isArray())
//                 {
//                     printf("Error: Json is not an array!\n");
//                 }
//                 for (unsigned int i = 0; i < SearchBlock.size(); i++)
//                 {
//                     std::vector<JSONCPP_STRING> members = SearchBlock[i].getMemberNames();
//                     for (std::vector<JSONCPP_STRING>::iterator& string = members.begin(); 
//                         string != members.end();
//                         string++)
//                     {
//                         printErrA("%s\n", (*string).c_str());
//                     }
//                 }
// 
//                 
//             }
//             FreeMemory(JsonBuf);
//         }
//         DeleteHandle(hFile);
//     }
    return 0;
}