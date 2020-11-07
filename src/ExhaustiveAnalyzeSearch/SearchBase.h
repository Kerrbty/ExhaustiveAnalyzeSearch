#ifndef _SEARCH_BASE_CALSS_HEADE_HH_H
#define _SEARCH_BASE_CALSS_HEADE_HH_H

typedef bool (__stdcall* PGetAddrSuccessCallBack)(void* addr, void* user_info);

class CSearchBase{
public:
    CSearchBase():m_search_next(false) {};
    virtual ~CSearchBase() {};

    // 在指定的内存区域内搜索数据 
    void* SearchBytes(void* addr, size_t range, const char* szbytes);
    void* SearchBytes(void* addr, size_t range, const wchar_t* szbytes);
    void* SearchBytes(void* addr, size_t range, const unsigned char* szbytes, size_t len);
    void* SearchBytes(void* addr, size_t range, unsigned char* bytes, unsigned char* masks, size_t len);
    void* SearchString(void* addr, size_t range, const char* str);
    void* SearchString(void* addr, size_t range, const wchar_t* str);

    virtual unsigned int SearchTargetBytes(const char* szbytes, PGetAddrSuccessCallBack callback, void* user_args) = 0;
    virtual unsigned int SearchTargetBytes(const unsigned char* szbytes, size_t len, PGetAddrSuccessCallBack callback, void* user_args) = 0;
    virtual unsigned int SearchTargetString(const char* str, PGetAddrSuccessCallBack callback, void* user_args) = 0;
    virtual unsigned int SearchTargetString(const wchar_t* str, PGetAddrSuccessCallBack callback, void* user_args) = 0;

    virtual void* GetRealAddr(void* findaddr) = 0; // 把当前地址转换了 

    // 从addr地址位置开始比较数据 
    bool CompareStr(void* addr, const char* str);
    bool CompareStr(void* addr, const wchar_t* str);
    bool CompareStrI(void* addr, const char* str);
    bool CompareStrI(void* addr, const wchar_t* str);
    bool CompareBytes(void* addr, size_t range, const char* szbytes);
    bool CompareBytes(void* addr, size_t range, const unsigned char* szbytes, size_t len);

    // 获取指定长度的数据 
    void* ReadDword(void* addr);
    void* ReadQword(void* addr);

protected:
    // 是否继续搜索下一个内容呢 
    bool IsSearchNext() { return m_search_next; }
    void SetSearchNext(bool search_continue) { m_search_next = search_continue; }

#ifndef _DEBUG
private:
#endif
    // 只有该类内部使用，因为涉及到资源释放会引起内存泄漏 
    size_t BytesString2Bytes(const char* szbytes, unsigned char** bytes, unsigned char** masks); // 字符串转二进制 
    size_t BytesString2Bytes(const wchar_t* szbytes, unsigned char** bytes, unsigned char** masks); // 字符串转二进制 
    bool CompareChgResources(void* addr, size_t range, unsigned char* bytes, unsigned char* masks, size_t len);  // 利用转换的 m_chg_bytes/m_chg_flags进行内存比较 
    void FreeChgResources(unsigned char** bytes, unsigned char** masks); // 释放m_chg_bytes、m_chg_flags资源 

private:

    bool m_search_next;
};


#endif