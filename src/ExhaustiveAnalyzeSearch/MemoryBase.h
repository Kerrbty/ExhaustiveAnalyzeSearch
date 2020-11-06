#ifndef _MEMORY_BASE_HEADER_HH_H_
#define _MEMORY_BASE_HEADER_HH_H_
#include <stdint.h>
#include <vector>
#include "printInfo.h"

#define MEMORY_READABLE             (0x0001)
#define MEMORY_WRITEABLE            (0x0002)
#define MEMORY_EXECUTEABLE          (0x0004)
#define PAGE_SIZE                   (0x1000)

typedef struct _meminfo
{
    void* base;
    size_t len;
    uint32_t access;
}meminfo, *pmeminfo;

class CMemoryInfo{
public:
    CMemoryInfo():m_analyze(false),m_isSelfMemory(false){}
    virtual ~CMemoryInfo() {}

    virtual uint32_t SearchMemory(uint32_t access, std::vector<meminfo>& memlist)   // 根据访问类型查询分析的内存 
    {
        if (AnalyzeMemoryInfo()==0)
        {
            return 0;
        }

        for (
            std::vector<meminfo>::iterator it = m_memlist.begin();
            it != m_memlist.end();
        it++
            )
        {
            if ( (it->access&access) == access)
            {
                printInfoA("Get Block addr: 0x%p, len: 0x%p, access: %u \n",
                    it->base,
                    it->len,
                    it->access);

                memlist.push_back(*it);
            }
        }
        return memlist.size();
    }

    virtual bool GetMemoryRangeInfo(void* addr, meminfo* mem)  // 获取内存块信息 
    {
        if (AnalyzeMemoryInfo()==0)
        {
            return false;
        }

        for (
            std::vector<meminfo>::iterator it = m_memlist.begin();
            it != m_memlist.end();
        it++
            )
        {
            if ( 
                (uint8_t*)it->base>=(uint8_t*)addr && 
                (uint8_t*)it->base+it->len<=(uint8_t*)addr
                )
            {
                printInfoA("Get Rage Info, addr: 0x%p, len: 0x%p, access: %u \n",
                    it->base,
                    it->len,
                    it->access);
                mem->base = it->base;
                mem->len = it->len;
                mem->access = it->access;
                return true;
            }
        }
        return false;
    }

    virtual bool IsCurrentProcessMemory() = 0; // 分析的内存数据是本进程内的吗 

    // 远程进程需要读取回来再做处理 
    virtual size_t ReadMemory(void* addr, void* buf, uint32_t ulsize) { return 0; };  // 读取内存数据 
    virtual size_t WriteMemory(void* addr, void* buf, uint32_t ulsize) { return 0; };  // 写入内存数据 

protected:
    virtual uint32_t AnalyzeMemoryInfo() = 0; // 分析内存信息 

    // 是否已经分析内存? 
    bool IsAnalyzeMemory() {return m_analyze;}
    void SetAnalyzeMemory(bool flag) { m_analyze = flag; }

    // 是否是自己的内存? 
    bool IsSelfMemoryFlag() { return m_isSelfMemory; };
    void SetSelfMemoryFlag(bool flag) { m_isSelfMemory = flag; };

    std::vector<meminfo> m_memlist;  // 分析完的结果保存 

private:
    bool m_analyze;   // 是否已经分析 
    bool m_isSelfMemory;   // 自己进程内存标志 
};

#endif // _MEMORY_BASE_HEADER_HH_H_ 