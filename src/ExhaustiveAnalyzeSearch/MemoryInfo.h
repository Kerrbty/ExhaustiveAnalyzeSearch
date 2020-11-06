#ifndef _MEMORY_INFO_HEADER_HH_H_
#define _MEMORY_INFO_HEADER_HH_H_
#include "MemoryBase.h"
#include <defs.h>

class MemoryInfo: public CMemoryInfo{
public:
    MemoryInfo();
    MemoryInfo(uint32_t pid);
    virtual ~MemoryInfo();

    virtual bool IsCurrentProcessMemory();  // �������ڴ������Ǳ������ڵ��� 

    virtual size_t ReadMemory(void* addr, void* buf, uint32_t ulsize);  // ��ȡ�ڴ����� 
    virtual size_t WriteMemory(void* addr, void* buf, uint32_t ulsize);  // д���ڴ����� 

protected:
    bool GetProcessHandle();
    void CloseProcessHandle();
    virtual uint32_t AnalyzeMemoryInfo(); // �����ڴ���Ϣ 

private:
    uint32_t m_pid;
    HANDLE m_hProcess;
};

#endif // _MEMORY_INFO_HEADER_HH_H_ 
