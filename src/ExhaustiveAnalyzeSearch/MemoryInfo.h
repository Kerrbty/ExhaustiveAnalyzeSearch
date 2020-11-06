#ifndef _MEMORY_INFO_HEADER_HH_H_
#define _MEMORY_INFO_HEADER_HH_H_
#include "MemoryBase.h"
#include <defs.h>

class MemoryInfo: public CMemoryInfo{
public:
    MemoryInfo();
    MemoryInfo(uint32_t pid);
    virtual ~MemoryInfo();

    virtual bool IsCurrentProcessMemory();  // 分析的内存数据是本进程内的吗 

    virtual size_t ReadMemory(void* addr, void* buf, uint32_t ulsize);  // 读取内存数据 
    virtual size_t WriteMemory(void* addr, void* buf, uint32_t ulsize);  // 写入内存数据 

protected:
    bool GetProcessHandle();
    void CloseProcessHandle();
    virtual uint32_t AnalyzeMemoryInfo(); // 分析内存信息 

private:
    uint32_t m_pid;
    HANDLE m_hProcess;
};

#endif // _MEMORY_INFO_HEADER_HH_H_ 
