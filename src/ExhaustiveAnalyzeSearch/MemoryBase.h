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

    virtual uint32_t SearchMemory(uint32_t access, std::vector<meminfo>& memlist) = 0;   // ���ݷ������Ͳ�ѯ�������ڴ� 
    virtual bool GetMemoryRangeInfo(void* addr, void** base, size_t* len, uint32_t access) = 0;  // ��ȡ�ڴ����Ϣ 

    virtual size_t ReadMemory(void* addr, void* buf, uint32_t ulsize) { return 0; };  // ��ȡ�ڴ����� 
    virtual size_t WriteProcess(void* addr, void* buf, uint32_t ulsize) { return 0; };  // д���ڴ����� 

protected:
    virtual uint32_t AnalyzeMemoryInfo() = 0; // �����ڴ���Ϣ 
    bool isMemoryAnalyze() { return m_analyze; }

    bool m_analyze;   // �Ƿ��Ѿ����� 
    std::vector<meminfo> m_memlist;  // ������Ľ������ 
};

#endif // _MEMORY_BASE_HEADER_HH_H_ 