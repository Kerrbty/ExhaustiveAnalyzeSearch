#ifndef _SEARCH_BASE_CALSS_HEADE_HH_H
#define _SEARCH_BASE_CALSS_HEADE_HH_H

typedef bool (__stdcall* PGetAddrSuccessCallBack)(void* addr, void* user_info);

class CSearchBase{
public:
    CSearchBase():m_search_next(false) {};
    virtual ~CSearchBase() {};

    // ��ָ�����ڴ��������������� 
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

    virtual void* GetRealAddr(void* findaddr) = 0; // �ѵ�ǰ��ַת���� 

    // ��addr��ַλ�ÿ�ʼ�Ƚ����� 
    bool CompareStr(void* addr, const char* str);
    bool CompareStr(void* addr, const wchar_t* str);
    bool CompareStrI(void* addr, const char* str);
    bool CompareStrI(void* addr, const wchar_t* str);
    bool CompareBytes(void* addr, size_t range, const char* szbytes);
    bool CompareBytes(void* addr, size_t range, const unsigned char* szbytes, size_t len);

    // ��ȡָ�����ȵ����� 
    void* ReadDword(void* addr);
    void* ReadQword(void* addr);

protected:
    // �Ƿ����������һ�������� 
    bool IsSearchNext() { return m_search_next; }
    void SetSearchNext(bool search_continue) { m_search_next = search_continue; }

#ifndef _DEBUG
private:
#endif
    // ֻ�и����ڲ�ʹ�ã���Ϊ�漰����Դ�ͷŻ������ڴ�й© 
    size_t BytesString2Bytes(const char* szbytes, unsigned char** bytes, unsigned char** masks); // �ַ���ת������ 
    size_t BytesString2Bytes(const wchar_t* szbytes, unsigned char** bytes, unsigned char** masks); // �ַ���ת������ 
    bool CompareChgResources(void* addr, size_t range, unsigned char* bytes, unsigned char* masks, size_t len);  // ����ת���� m_chg_bytes/m_chg_flags�����ڴ�Ƚ� 
    void FreeChgResources(unsigned char** bytes, unsigned char** masks); // �ͷ�m_chg_bytes��m_chg_flags��Դ 

private:

    bool m_search_next;
};


#endif