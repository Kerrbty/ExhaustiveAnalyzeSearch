#include "SearchBase.h"
#include <defs.h>
#include <string.h>
#include "printInfo.h"

bool CSearchBase::CompareStr(void* addr, const char* str)
{
    return (strcmp((char*)addr, str) == 0);
}

bool CSearchBase::CompareStr(void* addr, const wchar_t* str)
{
    return (wcscmp((wchar_t*)addr, str) == 0);
}

bool CSearchBase::CompareStrI(void* addr, const char* str)
{
    return (stricmp((char*)addr, str) == 0);
}

bool CSearchBase::CompareStrI(void* addr, const wchar_t* str)
{
    return (wcsicmp((wchar_t*)addr, str) == 0);
}

void* CSearchBase::ReadDword(void* addr) 
{
    unsigned int* pval = (unsigned int*)addr;
    return (void*)*pval;
}

void* CSearchBase::ReadQword(void* addr)
{
    unsigned __int64* pval = (unsigned __int64*)addr;
    return (void*)*pval;
}

// 比较的是字符串类型的二进制数据 
bool CSearchBase::CompareBytes(void* addr, size_t range, const char* szbytes)
{
    bool result = false;
    unsigned char* bytes = NULL;
    unsigned char* flags = NULL;
    size_t len = BytesString2Bytes(szbytes, &bytes, &flags);
    if ( len > 0 )
    {
        result = CompareChgResources(addr, range, bytes, flags, len);
        FreeChgResources(&bytes, &flags);
    }
    return result;
}

// 比较的纯二进制buffer 
bool CSearchBase::CompareBytes(void* addr, size_t range, const unsigned char* szbytes, size_t len)
{
    if (range<len)
    {
        return false;
    }

    return (memcmp(addr, szbytes, len) == 0);
}

void* CSearchBase::SearchBytes(void* addr, size_t range, const char* szbytes)
{
    void* result = NULL;
    unsigned char* bytes = NULL;
    unsigned char* masks = NULL;
    size_t len = BytesString2Bytes(szbytes, &bytes, &masks);
    if ( len > 0 )
    {
        for (size_t i=0; i<range; i++)
        {
            if (CompareChgResources((unsigned char*)addr+i, range-i, bytes, masks, len))
            {
                result = (unsigned char*)addr + i;
                break;
            }
        }
        FreeChgResources(&bytes, &masks);
    }
    return result;
}

void* CSearchBase::SearchBytes(void* addr, size_t range, const wchar_t* szbytes)
{
    void* result = NULL;
    unsigned char* bytes = NULL;
    unsigned char* masks = NULL;
    size_t len = BytesString2Bytes(szbytes, &bytes, &masks);
    if ( len > 0 )
    {
        for (size_t i=0; i<range; i++)
        {
            if (CompareChgResources((unsigned char*)addr+i, range-i, bytes, masks, len))
            {
                result = (unsigned char*)addr + i;
                break;
            }
        }
        FreeChgResources(&bytes, &masks);
    }
    return result;
}

void* CSearchBase::SearchBytes(void* addr, size_t range, const unsigned char* szbytes, size_t len)
{
    for (size_t i=0; i<range; i++)
    {
        if (CompareBytes((unsigned char*)addr+i, range-i, szbytes, len))
        {
            return (unsigned char*)addr + i;
        }
    }
    return NULL;
}

void* CSearchBase::SearchBytes(void* addr, size_t range, unsigned char* bytes, unsigned char* masks, size_t len)
{
    for (size_t i=0; i<range; i++)
    {
        if (CompareChgResources((unsigned char*)addr+i, range-i, bytes, masks, len))
        {
            return (unsigned char*)addr + i;
        }
    }
    return NULL;
}

void* CSearchBase::SearchString(void* addr, size_t range, const char* str)
{
    size_t len = strlen(str);
    if (len>range)
    {
        return NULL;
    }

    for (size_t i=0; i<range-len; i++)
    {
        if (CompareStr((unsigned char*)addr+i, str))
        {
            return (unsigned char*)addr + i;
        }
    }
    return NULL;
}

void* CSearchBase::SearchString(void* addr, size_t range, const wchar_t* str)
{
    size_t len = wcslen(str)*sizeof(wchar_t);
    if (len>range)
    {
        return NULL;
    }


    for (size_t i=0; i<range-len; i++)
    {
        if (CompareStr((unsigned char*)addr+i, str))
        {
            return (unsigned char*)addr + i;
        }
    }
    return NULL;
}

// 字符串转二进制 
size_t CSearchBase::BytesString2Bytes(const char* szbytes, unsigned char** bytes, unsigned char** masks)
{
    if (szbytes == NULL || bytes == NULL || masks == NULL)
    {
        return 0;
    }

    // 计算长度 
    int len = 0;
    const char* pByte = szbytes;
    for (;*pByte;pByte++)
    {
        if ( *pByte>='0' && *pByte<='9' )
        {
            len++;
        }
        else if ((*pByte|0x20)>='a' && (*pByte|0x20)<='f')
        {
            len++;
        }
        else if (*pByte == '.')
        {
            if ( *(pByte+1) == '{')
            {
                char* bytes = NULL;
                len += strtoul(pByte+2, &bytes, 10);
                pByte = bytes;
            }
            else
            {
                len++;
            }
        }
    }
    printDebugA("Analyze Binary Code len: %u\n", len);

    if (len == 0)
    {
        return 0;
    }
    *bytes = (unsigned char*)AllocMemory(len/2 + 2);
    if (*bytes == NULL)
    {
        printErrA("alloc memory error(%s: %u)!\n", __FILE__, __LINE__);
        return 0;
    }
    *masks = (unsigned char*)AllocMemory(len/2 + 2);
    if (*masks == NULL)
    {
        FreeMemory(*bytes);
        printErrA("alloc memory error(%s: %u)!\n", __FILE__, __LINE__);
        return 0;
    }

    // 转换数据 
    int ipos = 0;
    pByte = szbytes;
    for (;*pByte;pByte++)
    {
        if ( *pByte>='0' && *pByte<='9' )
        {
            (*bytes)[ipos/2] = ((*bytes)[ipos/2]<<4)| (*pByte - '0');
            (*masks)[ipos/2] = ((*masks)[ipos/2]<<4)| 0xF;
            ipos++;
        }
        else if ((*pByte|0x20)>='a' && (*pByte|0x20)<='f')
        {
            (*bytes)[ipos/2] = ((*bytes)[ipos/2]<<4)| (*pByte - 'a' + 10);
            (*masks)[ipos/2] = ((*masks)[ipos/2]<<4)| 0xF;
            ipos++;
        }
        else if (*pByte == '.')
        {
            if ( *(pByte+1) == '{')
            {
                char* bytes = NULL;
                ipos += strtoul(pByte+2, &bytes, 10);
                pByte = bytes;
            }
            else
            {
                ipos++;
            }
        }
    }
    return ipos/2;
}

size_t CSearchBase::BytesString2Bytes(const wchar_t* szbytes, unsigned char** bytes, unsigned char** masks)
{
    if (szbytes == NULL || bytes == NULL || masks == NULL)
    {
        return 0;
    }

    // 计算长度 
    int len = 0;
    const wchar_t* pByte = szbytes;
    for (;*pByte;pByte++)
    {
        if ( *pByte>=L'0' && *pByte<=L'9' )
        {
            len++;
        }
        else if ((*pByte|0x20)>=L'a' && (*pByte|0x20)<=L'f')
        {
            len++;
        }
        else if (*pByte == L'.')
        {
            if ( *(pByte+1) == L'{')
            {
                wchar_t* bytes = NULL;
                len += wcstoul(pByte+2, &bytes, 10);
                pByte = bytes;
            }
            else
            {
                len++;
            }
        }
    }
    printDebugA("Analyze Binary Code len: %u\n", len);

    if (len == 0)
    {
        return 0;
    }
    *bytes = (unsigned char*)AllocMemory(len/2 + 2);
    if (*bytes == NULL)
    {
        printErrA("alloc memory error(%s: %u)!\n", __FILE__, __LINE__);
        return 0;
    }
    *masks = (unsigned char*)AllocMemory(len/2 + 2);
    if (*masks == NULL)
    {
        FreeMemory(*bytes);
        printErrA("alloc memory error(%s: %u)!\n", __FILE__, __LINE__);
        return 0;
    }

    // 转换数据 
    int ipos = 0;
    pByte = szbytes;
    for (;*pByte;pByte++)
    {
        if ( *pByte>=L'0' && *pByte<=L'9' )
        {
            (*bytes)[ipos/2] = ((*bytes)[ipos/2]<<4)| (*pByte - L'0');
            (*masks)[ipos/2] = ((*masks)[ipos/2]<<4)| 0xF;
            ipos++;
        }
        else if ((*pByte|0x20)>=L'a' && (*pByte|0x20)<=L'f')
        {
            (*bytes)[ipos/2] = ((*bytes)[ipos/2]<<4)| (*pByte - L'a' + 10);
            (*masks)[ipos/2] = ((*masks)[ipos/2]<<4)| 0xF;
            ipos++;
        }
        else if (*pByte == L'.')
        {
            if ( *(pByte+1) == L'{')
            {
                wchar_t* bytes = NULL;
                ipos += wcstoul(pByte+2, &bytes, 10);
                pByte = bytes;
            }
            else
            {
                ipos++;
            }
        }
    }
    return ipos/2;
}

// 利用转换的 m_chg_bytes/m_chg_flags进行内存比较 
bool CSearchBase::CompareChgResources(void* addr, size_t range, unsigned char* bytes, unsigned char* masks, size_t len)
{
    if (range < len)
    {
        return false;
    }

    // 根据掩码与以后比较数据 
    unsigned char* pbuf = (unsigned char*)addr;
    for (size_t i=0; i<len; i++)
    {
        if ( (pbuf[i]&masks[i]) != bytes[i] )
        {
            return false;
        }
    }
    return true;
}

// 释放m_chg_bytes、m_chg_flags资源 
void CSearchBase::FreeChgResources(unsigned char** bytes, unsigned char** masks)
{
    if (bytes!=NULL)
    {
        FreeMemory(*bytes);
    }
    
    if (masks)
    {
        FreeMemory(*masks);
    }
}

