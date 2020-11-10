#include "MemorySearch.h"
#include "MemoryInfo.h"
#include "PEFileMemoryInfo.h"


MemorySearch::MemorySearch(uint32_t m_pid) 
    :m_TargetMemory(NULL)
    ,m_init(false)
    ,m_select_target_memory(false)
    ,m_access(MEMORY_READABLE)
    ,m_bPEMemory(false)
{
    m_TargetMemory = new MemoryInfo(m_pid);
}

MemorySearch::MemorySearch(const char* filename)
    :m_TargetMemory(NULL)
    ,m_init(false)
    ,m_select_target_memory(false)
    ,m_access(MEMORY_READABLE)
    ,m_bPEMemory(true)
{
    m_TargetMemory = new PEFileMemoryInfo(filename);
}

MemorySearch::MemorySearch(const wchar_t* filename)
    :m_TargetMemory(NULL)
    ,m_init(false)
    ,m_select_target_memory(false)
    ,m_access(MEMORY_READABLE)
    ,m_bPEMemory(true)
{
    m_TargetMemory = new PEFileMemoryInfo(filename);
}

MemorySearch::~MemorySearch()
{
    ReleaseTargetMemory();
    if (m_TargetMemory)
    {
        delete m_TargetMemory;
        m_TargetMemory = NULL;
    }
}

// 初始化分析目标内存 
bool MemorySearch::GetTargetMemory()
{
    if (m_select_target_memory)
    {
        return m_select_target_memory;
    }

    if (m_TargetMemory)
    {
        m_TargetMemory->SearchMemory(m_access, m_memlist);
        m_select_target_memory = true;
    }
    return m_select_target_memory;
}

// 释放目标资源 
void MemorySearch::ReleaseTargetMemory()
{
    if (m_TargetMemory)
    {
        delete m_TargetMemory;
        m_TargetMemory = NULL;
    }
    m_memlist.clear();
    m_init = false;
    m_select_target_memory = false;
}

CMemoryInfo* MemorySearch::GetTargetMemoryObject()
{
    return m_TargetMemory;
}

bool MemorySearch::WhenFindCallBack(void* findaddr, int index, PGetAddrSuccessCallBack callback, void* user_args)
{
    bool get_next = true;
#ifdef _DEBUG
    printDebugA("Search code at: %p\n", GetRealAddr(findaddr));
#endif
    if (callback!=NULL)
    {
        get_next = callback(findaddr, index, user_args);
    }
    return get_next;
}

unsigned int MemorySearch::SearchTargetBytes(const char* szbytes, search_type readtype, PGetAddrSuccessCallBack callback, void* user_args)
{
    unsigned int ncount = 0;
    if ( GetTargetMemory() )
    {
        unsigned char* bytes = NULL;
        unsigned char* flags = NULL;
        unsigned int offset = 0;
        size_t len = BytesString2Bytes(szbytes, &bytes, &flags, &offset);
        if ( len > 0 )
        {
            bool bFind = false;
            for (size_t i=0; i<m_memlist.size() && !bFind; i++)
            {
                void* memorybase = m_memlist[i].base;
                size_t memorylen = m_memlist[i].len;

                void* findaddr = memorybase;
                do 
                {
                    findaddr = SearchBytes(findaddr, 
                        memorylen-((unsigned char*)findaddr-(unsigned char*)memorybase), 
                        bytes, 
                        flags, 
                        len);
                    if (findaddr)
                    {
                        if (readtype == READ_ADDR)
                        {
                            if( !WhenFindCallBack(ReadAddr((char*)findaddr + offset), ncount, callback, user_args) )
                            {
                                bFind = true;
                                break;
                            }
                        }
                        else if (readtype == READ_OFFSET)
                        {
                            void* addr = (char*)findaddr + offset + 4 + (int)ReadDword((char*)findaddr + offset);
                            if( !WhenFindCallBack(addr, ncount, callback, user_args) )
                            {
                                bFind = true;
                                break;
                            }
                        }
                        else 
                        {
                            if( !WhenFindCallBack(findaddr, ncount, callback, user_args) )
                            {
                                bFind = true;
                                break;
                            }
                        }
                        findaddr = (unsigned char*)findaddr + 1;
                        ncount++;
                    }
                    else
                    {
                        printErrA("not find: (%s)\n", szbytes);
                    }
                } while (findaddr);
            }
            FreeChgResources(&bytes, &flags);
        }
    }
    return ncount;
}

unsigned int MemorySearch::SearchTargetBytes(const unsigned char* szbytes, size_t len, PGetAddrSuccessCallBack callback, void* user_args)
{
    unsigned int ncount = 0;
    if ( GetTargetMemory() )
    {
        for (size_t i=0; i<m_memlist.size(); i++)
        {
            void* memorybase = m_memlist[i].base;
            size_t memorylen = m_memlist[i].len;

            void* findaddr = memorybase;
            do 
            {
                findaddr = SearchBytes(findaddr, 
                    memorylen-((unsigned char*)findaddr-(unsigned char*)memorybase), 
                    szbytes, 
                    len);
                if (findaddr)
                {
                    if( !WhenFindCallBack(findaddr, ncount, callback, user_args) )
                    {
                        break;
                    }
                    findaddr = (unsigned char*)findaddr + 1;
                    ncount++;
                }
            } while (findaddr);
        }
    }
    return ncount;
}

unsigned int MemorySearch::SearchTargetString(const char* str, PGetAddrSuccessCallBack callback, void* user_args)
{
    unsigned int ncount = 0;
    if ( GetTargetMemory() )
    {
        for (size_t i=0; i<m_memlist.size(); i++)
        {
            void* memorybase = m_memlist[i].base;
            size_t memorylen = m_memlist[i].len;

            void* findaddr = memorybase;
            do 
            {
                findaddr = SearchString(findaddr, 
                    memorylen-((unsigned char*)findaddr-(unsigned char*)memorybase), 
                    str);
                if (findaddr)
                {
                    if( !WhenFindCallBack(findaddr, ncount, callback, user_args) )
                    {
                        break;
                    }
                    findaddr = (unsigned char*)findaddr + 1;
                    ncount++;
                }
            } while (findaddr);
        }
    }
    return ncount;
}

unsigned int MemorySearch::SearchTargetString(const wchar_t* str, PGetAddrSuccessCallBack callback, void* user_args)
{
    unsigned int ncount = 0;
    if ( GetTargetMemory() )
    {
        for (size_t i=0; i<m_memlist.size(); i++)
        {
            void* memorybase = m_memlist[i].base;
            size_t memorylen = m_memlist[i].len;

            void* findaddr = memorybase;
            do 
            {
                findaddr = SearchString(findaddr, 
                    memorylen-((unsigned char*)findaddr-(unsigned char*)memorybase), 
                    str);
                if (findaddr)
                {
                    if( !WhenFindCallBack(findaddr, ncount, callback, user_args) )
                    {
                        break;
                    }
                    findaddr = (unsigned char*)findaddr + 1;
                    ncount++;
                }
            } while (findaddr);
        }
    }
    return ncount;
}

void* MemorySearch::GetRealAddr(void* findaddr)
{
    if (m_bPEMemory)
    {
        // pe内存偏移 
        void* base;
        void* offset = m_TargetMemory->GetRealAddr(findaddr, &base);
        return (unsigned char*)base+(unsigned long)offset;
    }
    else if ( m_TargetMemory->IsCurrentProcessMemory() )
    {
        // 内进程内存不变 
        return findaddr;
    }
    else
    {
        printErrA("未实现\n");
        return NULL;
    }
}

void* MemorySearch::GetBaseAddr()
{
    void* base;
    m_TargetMemory->GetRealAddr(NULL, &base);
    return base;
}