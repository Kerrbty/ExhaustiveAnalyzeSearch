#ifndef _JSON_SEARCH_LIST_HEADER_HH_H_
#define _JSON_SEARCH_LIST_HEADER_HH_H_ 

typedef void (__stdcall* PGetFindData)(void* addr, void* user_info);

// 解析json文件，并根据文件内容进行搜索
// 返回json文件里面的列表个数，具体的搜索成功个数可以从回调函数cb中统计 
unsigned int SearchByJsonA(const char* JsonFile, const char* filename, PGetFindData cb, void* userdata);
unsigned int SearchByJsonW(const wchar_t* JsonFile, const wchar_t* filename, PGetFindData cb, void* userdata);


#endif // _JSON_SEARCH_LIST_HEADER_HH_H_ 
