#include "MemoryBase.h"
#include "SearchBase.h"

class MemorySearch: public CSearchBase{
public:
    MemorySearch(uint32_t m_pid);
    MemorySearch(const char* filename);
    MemorySearch(const wchar_t* filename);
    ~MemorySearch();

    // ���������ڴ���Ӧ�þ��е��ڴ����� 
    void SetTargetMemoryAccess(uint32_t access) { m_access = access; }

    virtual unsigned int SearchTargetBytes(const char* szbytes, PGetAddrSuccessCallBack callback, void* user_args);
    virtual unsigned int SearchTargetBytes(const unsigned char* szbytes, size_t len, PGetAddrSuccessCallBack callback, void* user_args);
    virtual unsigned int SearchTargetString(const char* str, PGetAddrSuccessCallBack callback, void* user_args);
    virtual unsigned int SearchTargetString(const wchar_t* str, PGetAddrSuccessCallBack callback, void* user_args);

    virtual void* GetRealAddr(void* findaddr);
protected:
    bool GetTargetMemory();  // ��ʼ������Ŀ���ڴ� 
    void ReleaseTargetMemory();   // �ͷ�Ŀ����Դ 
    CMemoryInfo* GetTargetMemoryObject();
    bool WhenFindCallBack(void* findaddr, PGetAddrSuccessCallBack callback, void* user_args);

private:
    bool m_init; // �Ѿ���ʼ����־λ 
    CMemoryInfo* m_TargetMemory;  // Ŀ���ڴ���� 
    uint32_t m_access;               // �������ڴ���ҪӦ�þ��е����� 
    std::vector<meminfo> m_memlist;  // ��ѡ������Ŀ���ڴ��б� 
    bool m_select_target_memory;     // �Ƿ��Ѿ���ѡ��Ŀ���ڴ� 
    bool m_bPEMemory; 
};