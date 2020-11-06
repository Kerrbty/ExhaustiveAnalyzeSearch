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

    virtual uint32_t SearchMemory(uint32_t access, std::vector<meminfo>& memlist)   // ���ݷ������Ͳ�ѯ�������ڴ� 
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

    virtual bool GetMemoryRangeInfo(void* addr, meminfo* mem)  // ��ȡ�ڴ����Ϣ 
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

    virtual bool IsCurrentProcessMemory() = 0; // �������ڴ������Ǳ������ڵ��� 

    // Զ�̽�����Ҫ��ȡ������������ 
    virtual size_t ReadMemory(void* addr, void* buf, uint32_t ulsize) { return 0; };  // ��ȡ�ڴ����� 
    virtual size_t WriteMemory(void* addr, void* buf, uint32_t ulsize) { return 0; };  // д���ڴ����� 

protected:
    virtual uint32_t AnalyzeMemoryInfo() = 0; // �����ڴ���Ϣ 

    // �Ƿ��Ѿ������ڴ�? 
    bool IsAnalyzeMemory() {return m_analyze;}
    void SetAnalyzeMemory(bool flag) { m_analyze = flag; }

    // �Ƿ����Լ����ڴ�? 
    bool IsSelfMemoryFlag() { return m_isSelfMemory; };
    void SetSelfMemoryFlag(bool flag) { m_isSelfMemory = flag; };

    std::vector<meminfo> m_memlist;  // ������Ľ������ 

private:
    bool m_analyze;   // �Ƿ��Ѿ����� 
    bool m_isSelfMemory;   // �Լ������ڴ��־ 
};

#endif // _MEMORY_BASE_HEADER_HH_H_ 