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

    virtual uint32_t SearchMemory(uint32_t access, std::vector<meminfo>& memlist);   // ���ݷ������Ͳ�ѯ�������ڴ� 
    virtual bool GetMemoryRangeInfo(void* addr, void** base, size_t* len, uint32_t access);  // ��ȡ�ڴ����Ϣ 

    virtual size_t ReadMemory(void* addr, void* buf, uint32_t ulsize);  // ��ȡ�ڴ����� 
    virtual size_t WriteProcess(void* addr, void* buf, uint32_t ulsize);  // д���ڴ����� 

protected:
    bool GetProcessHandle();
    void CloseProcessHandle();
    virtual uint32_t AnalyzeMemoryInfo(); // �����ڴ���Ϣ 

private:
    uint32_t m_pid;
    HANDLE m_hProcess;
};

#endif // _MEMORY_INFO_HEADER_HH_H_ 
