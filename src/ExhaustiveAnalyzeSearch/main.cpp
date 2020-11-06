#include <defs.h>
#include <lib_json/json/json.h>
#include "printInfo.h"
#include "MemoryInfo.h"

int main()
{

    MemoryInfo curproc;
    curproc.ReadMemory(NULL, NULL, 0);
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