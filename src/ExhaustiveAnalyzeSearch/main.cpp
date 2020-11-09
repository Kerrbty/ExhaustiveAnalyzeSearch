#include <defs.h>
#include "JsonSearch.h"


void __stdcall printaddr(void* addr, void* user_info)
{
    printf("find success£º %p\n", addr);
}


int main()
{

//     PEFileMemoryInfo curproc("D:\\Program Files (x86)\\WeChat\\WeChat.exe");
//     MemoryInfo curproc;
//     std::vector<meminfo> section;
//     curproc.SearchMemory(MEMORY_EXECUTEABLE|MEMORY_READABLE, section);

    SearchByJsonA("Search.json", "D:\\Program Files (x86)\\WXWork3.0.31.3306\\WXWork.exe", printaddr, NULL);
//     MemorySearch memsearch("D:\\Program Files (x86)\\WXWork3.0.31.3306\\WXWork.exe");
//     memsearch.SearchTargetBytes("C645.{4} 85C9 74.{2} 68.{8} 51 E8.{8} 83C408", printaddr, &memsearch);

//     MemorySearch memsearch("C:\\windows\\system32\\kernelbase.dll");
//     memsearch.SearchTargetBytes("83E001 C744240C.{8} 89442404 85C9 74", printaddr, NULL);

//     MemorySearch memsearch("D:\\Program Files (x86)\\WeChat\\WeChat.exe");
//     std::vector<meminfo> section;
//     memsearch.InitializeTargetMemory(MEMORY_EXECUTEABLE|MEMORY_READABLE, section);
// 

    return 0;
}