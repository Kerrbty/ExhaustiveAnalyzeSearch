#ifndef _JSON_SEARCH_LIST_HEADER_HH_H_
#define _JSON_SEARCH_LIST_HEADER_HH_H_ 

typedef void (__stdcall* PGetFindData)(void* addr, void* user_info);

// ����json�ļ����������ļ����ݽ�������
// ����json�ļ�������б����������������ɹ��������Դӻص�����cb��ͳ�� 
unsigned int SearchByJsonA(const char* JsonFile, const char* filename, PGetFindData cb, void* userdata);
unsigned int SearchByJsonW(const wchar_t* JsonFile, const wchar_t* filename, PGetFindData cb, void* userdata);


#endif // _JSON_SEARCH_LIST_HEADER_HH_H_ 
