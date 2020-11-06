#ifndef _MEMORY_BASE_HEADER_HH_H_
#define _MEMORY_BASE_HEADER_HH_H_
#include <stdint.h>
#include <vector>

typedef struct _meminfo
{
    void* base;
    size_t len;
    uint32_t access;
}meminfo, *pmeminfo;

class CMemoryInfo{
public:
    CMemoryInfo():m_analyze(false){}
    virtual ~CMemoryInfo() {}

    virtual uint32_t SearchMemory(uint32_t access, std::vector<meminfo>& memlist) = 0;   // 根据访问类型查询分析的内存 
    virtual bool GetMemoryRangeInfo(void* addr, void** base, size_t* len, uint32_t access) = 0;  // 获取内存块信息 

    virtual size_t ReadMemory(void* addr, void* buf, uint32_t ulsize) { return 0; };  // 读取内存数据 
    virtual size_t WriteProcess(void* addr, void* buf, uint32_t ulsize) { return 0; };  // 写入内存数据 

protected:
    virtual uint32_t AnalyzeMemoryInfo() = 0; // 分析内存信息 
    bool isMemoryAnalyze() { return m_analyze; }

    bool m_analyze;   // 是否已经分析 
    std::vector<meminfo> m_memlist;  // 分析完的结果保存 
};

#endif // _MEMORY_BASE_HEADER_HH_H_ 