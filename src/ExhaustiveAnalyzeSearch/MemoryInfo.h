#ifndef _MEMORY_INFO_HEADER_HH_H_
#define _MEMORY_INFO_HEADER_HH_H_
#include "MemoryBase.h"
#include <defs.h>

#define MEMORY_READABLE             (0x0001)
#define MEMORY_WRITEABLE            (0x0002)
#define MEMORY_EXECUTEABLE          (0x0004)
#define PAGE_SIZE                   (0x1000)

class MemoryInfo: public CMemoryInfo{
public:
    MemoryInfo();
    MemoryInfo(uint32_t pid);
    virtual ~MemoryInfo();

    virtual uint32_t SearchMemory(uint32_t access, std::vector<meminfo>& memlist);   // 根据访问类型查询分析的内存 
    virtual bool GetMemoryRangeInfo(void* addr, void** base, size_t* len, uint32_t access);  // 获取内存块信息 

    virtual size_t ReadMemory(void* addr, void* buf, uint32_t ulsize);  // 读取内存数据 
    virtual size_t WriteProcess(void* addr, void* buf, uint32_t ulsize);  // 写入内存数据 

protected:
    bool GetProcessHandle();
    void CloseProcessHandle();
    virtual uint32_t AnalyzeMemoryInfo(); // 分析内存信息 

private:
    uint32_t m_pid;
    HANDLE m_hProcess;
};

#endif // _MEMORY_INFO_HEADER_HH_H_ 
