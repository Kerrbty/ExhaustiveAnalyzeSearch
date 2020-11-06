#include "config.h"
#include "MemoryInfo.h"


MemoryInfo::MemoryInfo()
    :m_pid(-1)
    ,m_hProcess(NULL)
{
}

MemoryInfo::MemoryInfo(uint32_t pid)
    :m_pid(pid)
    ,m_hProcess(NULL)
{ 
}

MemoryInfo::~MemoryInfo()
{
    CloseProcessHandle();
}


// 分析内存信息 
uint32_t MemoryInfo::AnalyzeMemoryInfo() 
{
    if (MemoryInfo::m_analyze)
    {
        return MemoryInfo::m_memlist.size();
    }

    if ( !GetProcessHandle() )
    {
        return 0;
    }

    // 分析 
    LPBYTE PageAddr = 0;
    while (TRUE)
    {
        MEMORY_BASIC_INFORMATION membase = {0};
        if( VirtualQueryEx(m_hProcess, PageAddr, &membase, sizeof(membase)) == 0)
        {
            if (PageAddr > PageAddr+PAGE_SIZE)
            {
                // 溢出了 
                break;
            }
            PageAddr += PAGE_SIZE;
        }
        else 
        {
            if ( membase.Protect & PAGE_NOACCESS )
            {
                PageAddr += membase.RegionSize;
            }
            else
            {
                meminfo searchmem = {0};
                searchmem.base = membase.BaseAddress;
                searchmem.len = membase.RegionSize;
                switch(membase.Protect&0xFF)
                {
                case PAGE_READONLY:
                    searchmem.access = MEMORY_READABLE;
                    break;
                case PAGE_READWRITE:
                    searchmem.access = MEMORY_READABLE|MEMORY_WRITEABLE;
                    break;
                case PAGE_WRITECOPY:
                    searchmem.access = MEMORY_WRITEABLE;
                    break;
                case PAGE_EXECUTE:
                    searchmem.access = MEMORY_EXECUTEABLE;
                    break;
                case PAGE_EXECUTE_READ:
                    searchmem.access = MEMORY_READABLE|MEMORY_EXECUTEABLE;
                    break;
                case PAGE_EXECUTE_READWRITE:
                    searchmem.access = MEMORY_READABLE|MEMORY_WRITEABLE|MEMORY_EXECUTEABLE;
                    break;
                case PAGE_EXECUTE_WRITECOPY:
                    searchmem.access = MEMORY_WRITEABLE|MEMORY_EXECUTEABLE;
                    break;
                }
                MemoryInfo::m_memlist.push_back(searchmem);
                PageAddr += membase.RegionSize;
            }
        }
    }

    MemoryInfo::m_analyze = true;
    return 0;
}

// 根据访问类型查询分析的内存 
uint32_t MemoryInfo::SearchMemory(uint32_t access, std::vector<meminfo>& memlist)
{
    return 0;
}

// 获取内存块信息 
bool MemoryInfo::GetMemoryRangeInfo(void* addr, void** base, size_t* len, uint32_t access)
{
    return true;
}

// 读取内存数据 
size_t MemoryInfo::ReadMemory(void* addr, void* buf, uint32_t ulsize)
{
    AnalyzeMemoryInfo();
    return MemoryInfo::m_analyze;
}

// 写入内存数据 
size_t MemoryInfo::WriteProcess(void* addr, void* buf, uint32_t ulsize)
{
    return 0;
}

// 打开进程 
bool MemoryInfo::GetProcessHandle()
{
    if (m_hProcess != NULL)
    {
        return true;
    }

    if (m_pid == -1)
    {
        m_pid = GetCurrentProcessId();
    }
    m_hProcess = OpenProcess(
        PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_QUERY_INFORMATION, 
        FALSE,
        m_pid);

    if (m_pid == NULL)
    {
        return false;
    }
    return true;
}

// 关闭进程 
void MemoryInfo::CloseProcessHandle()
{
    DeleteHandle(m_hProcess);
    m_hProcess = NULL;
    MemoryInfo::m_memlist.clear();
    MemoryInfo::m_analyze = false;
}