#include "MemoryBase.h"
#include "SearchBase.h"

class MemorySearch: public CSearchBase{
public:
    MemorySearch(uint32_t m_pid);
    MemorySearch(const char* filename);
    MemorySearch(const wchar_t* filename);
    ~MemorySearch();

    // 设置搜索内存所应该具有的内存属性 
    void SetTargetMemoryAccess(uint32_t access) { m_access = access; }

    virtual unsigned int SearchTargetBytes(const char* szbytes, PGetAddrSuccessCallBack callback, void* user_args);
    virtual unsigned int SearchTargetBytes(const unsigned char* szbytes, size_t len, PGetAddrSuccessCallBack callback, void* user_args);
    virtual unsigned int SearchTargetString(const char* str, PGetAddrSuccessCallBack callback, void* user_args);
    virtual unsigned int SearchTargetString(const wchar_t* str, PGetAddrSuccessCallBack callback, void* user_args);

    virtual void* GetRealAddr(void* findaddr);
protected:
    bool GetTargetMemory();  // 初始化分析目标内存 
    void ReleaseTargetMemory();   // 释放目标资源 
    CMemoryInfo* GetTargetMemoryObject();
    bool WhenFindCallBack(void* findaddr, PGetAddrSuccessCallBack callback, void* user_args);

private:
    bool m_init; // 已经初始化标志位 
    CMemoryInfo* m_TargetMemory;  // 目标内存对象 
    uint32_t m_access;               // 搜索的内存需要应该具有的属性 
    std::vector<meminfo> m_memlist;  // 赛选出来的目标内存列表 
    bool m_select_target_memory;     // 是否已经赛选过目标内存 
    bool m_bPEMemory; 
};