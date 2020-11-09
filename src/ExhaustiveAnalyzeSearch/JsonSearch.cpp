#include "JsonSearch.h"
#include <defs.h>
#include <List.h>
#include <lib_json/json/json.h>
#include <StringFormat/StringFormat.h>
#include "printInfo.h"
#include "MemoryInfo.h"
#include "PEFileMemoryInfo.h"
#include "MemorySearch.h"

enum do_action {
    search_byte = 1,    // �����ַ��� 
    search_string,
    compare_string,
    read_addr,      // ��ȡvoid*�������� 
    read_dword,     // ��ȡ32λ���� 
    read_qword,     // ��ȡ64λ���� 
    ret_addr,       // ֱ�ӷ���addr��ĳ��ƫ�Ƶ�ַ 

    endflag = -1,
};

struct {
    char* name;
    do_action action;
}action_list[] = {
    {"search_byte", search_byte},
    {"search_string", search_string},
    {"compare_string", compare_string},
    {"read_addr", read_addr},
    {"read_dword", read_dword},
    {"read_qword", read_qword},
    {"ret_addr", ret_addr},
};

typedef struct _SearchObject 
{
    char* name;  // �������� 
    unsigned int offset;  // ��Է���Ԥ�ڵĵ�ַƫ�� 
    do_action action;  // �������� 
    char* data;    // ���� 
    bool bsearch_next;   // �Ƿ�������һ�� 
    struct _SearchObject* continueobj;   // ������һ�������Ժ���һ������ 
    LIST_ENTRY next;   // ��һ���������� 
}SearchObject, *PSearchObject;

unsigned int SearchByJsonA(const char* JsonFile, const char* filename, PGetFindData cb, void* userdata)
{
    CMultiAndWide str(JsonFile);
    CMultiAndWide file(filename);
    return SearchByJsonW(str.GetWideChar(), file.GetWideChar(), cb, userdata);
}

PSearchObject AnalyzeSearchObject(Json::Value data)
{
    if (!data.isObject())
    {
        return NULL;
    }
    PSearchObject pSearchList = (PSearchObject)AllocMemory(sizeof(SearchObject));
    if (!pSearchList)
    {
        return NULL;
    }
    pSearchList->continueobj = NULL;
    _InitializeListHead(&pSearchList->next);

    std::vector<JSONCPP_STRING> members = data.getMemberNames();
    for (std::vector<JSONCPP_STRING>::iterator& string = members.begin(); 
        string != members.end();
        string++)
    {
        if (*string == "name")
        {
            if (data["name"].isString())
            {
                const char* name = data["name"].asCString();
                int len = strlen(name);
                pSearchList->name = (char*)AllocMemory(len+1);
                if (pSearchList->name)
                {
                    strcpy_s(pSearchList->name, len+1, name);
                }
            }
        }
        else if (*string == "offset")
        {
            if (data["offset"].asInt())
            {
                pSearchList->offset = data["offset"].asInt();
            }
        }
        else if (*string == "continue")
        {
            pSearchList->continueobj = AnalyzeSearchObject(data["continue"]);
        }
        else
        {            
            for (int i=0; i<sizeof(action_list)/sizeof(*action_list); i++)
            {
                if (*string == action_list[i].name)
                {
                    pSearchList->action = action_list[i].action;
                    FreeMemory(pSearchList->data);
                    if (data[*string].isString())
                    {
                        const char* szStr = data[*string].asCString();
                        int len = strlen(szStr);
                        pSearchList->data = (char*)AllocMemory(len+1);
                        if (pSearchList->data)
                        {
                            strcpy_s(pSearchList->data, len+1, szStr);
                        }
                    }
                }
            }
        }
    }
    return pSearchList;
}

typedef struct
{
    PSearchObject SearchElement;
    MemorySearch* memsearch;
    PGetFindData cb;
    void* userdata;
}CbData, *PCbData;

bool __stdcall FindObjectCallBack(void* addr, void* user_info)
{
    PCbData cbd = (PCbData)user_info;

    PSearchObject pCurSearch = cbd->SearchElement;
    if (pCurSearch == NULL)
    {
        if (cbd->cb)
        {
            cbd->cb(cbd->memsearch->GetRealAddr(addr), cbd->userdata);
            return false;
        }
    }
    else
    {
        CbData nextcbd;
        nextcbd.SearchElement = pCurSearch->continueobj;
        nextcbd.memsearch = cbd->memsearch;
        nextcbd.cb = cbd->cb;
        nextcbd.userdata = cbd->userdata;

        switch (pCurSearch->action)
        {
        case search_byte:    // �����ַ��� 
            cbd->memsearch->SearchTargetBytes(pCurSearch->data, FindObjectCallBack, &nextcbd);
            break;
        case search_string:
            cbd->memsearch->SearchTargetString(pCurSearch->data, FindObjectCallBack, &nextcbd);
            break;
        case compare_string:
            if (cbd->memsearch->CompareStr(cbd->memsearch->ReadAddr((unsigned char*)addr+pCurSearch->offset), pCurSearch->data))
            {
                FindObjectCallBack((unsigned char*)addr+pCurSearch->offset, &nextcbd);
            }
            break;
        case read_addr: 
            {
                void* nadr = cbd->memsearch->ReadAddr((unsigned char*)addr+pCurSearch->offset);
                FindObjectCallBack(nadr, &nextcbd);
            }
            break;
        case read_dword:
            {
                void* nadr = cbd->memsearch->ReadDword((unsigned char*)addr+pCurSearch->offset);
                FindObjectCallBack(nadr, &nextcbd);
            }
            break;
        case read_qword:
            {
                void* nadr = cbd->memsearch->ReadQword((unsigned char*)addr+pCurSearch->offset);
                FindObjectCallBack(nadr, &nextcbd);
            }
            break;
        case ret_addr:
            FindObjectCallBack((unsigned char*)addr+pCurSearch->offset, &nextcbd);
            break;
        }
    }
    return true;
}

static void SearchTargetObject(PSearchObject SearchElement, const wchar_t* filename, PGetFindData cb, void* userdata)
{
    MemorySearch memsearch(filename);

    CbData cbd;
    cbd.SearchElement = SearchElement;
    cbd.memsearch = &memsearch;

    cbd.cb = cb;
    cbd.userdata = userdata;
    FindObjectCallBack(NULL, &cbd);
}

unsigned int SearchByJsonW(const wchar_t* JsonFile, const wchar_t* filename, PGetFindData cb, void* userdata)
{
    unsigned int search_count = 0; // �ܵ������������ɹ�����������������cb�ص���ͳ�� 
    if (JsonFile==NULL || filename == NULL)
    {
        return search_count;
    }

    HANDLE hFile = FileOpenW(JsonFile, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return search_count;
    }

    SearchObject SearchList = {0};
    _InitializeListHead(&SearchList.next);

    // ����json�ļ� 
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    LPBYTE JsonBuf = (LPBYTE)AllocMemory(dwFileSize+1);
    if (JsonBuf)
    {
        DWORD dwBytes = 0;
        ReadFile(hFile, JsonBuf, dwFileSize, &dwBytes, NULL);
        Json::Reader reader;
        Json::Value SearchBlock;
        if (reader.parse((char*)JsonBuf, SearchBlock))
        {
            if (!SearchBlock.isArray())
            {
                printErrA("Error: Json is not an array!\n");
            }

            // ����json���� 
            for (unsigned int i = 0; i < SearchBlock.size(); i++)
            {
                PSearchObject SearchElement = AnalyzeSearchObject(SearchBlock[i]);
                if (SearchElement != NULL)
                {
                    _InsertTailList(&SearchList.next, &SearchElement->next);
                }
            }
        }
        FreeMemory(JsonBuf);
    }
    DeleteHandle(hFile);
 
    // ���� SearchList ȥ�������� 
    for (PLIST_ENTRY li = SearchList.next.Flink; li != &SearchList.next; li = li->Flink)
    {
        PSearchObject obj = CONTAINING_RECORD(li, SearchObject, next);
        SearchTargetObject(obj, filename, cb, userdata);
        search_count++;
    }

    // ����Ժ�ɾ����Դ 
    while(!_IsListEmpty(&SearchList.next))
    {
        PLIST_ENTRY li = _RemoveTailList(&SearchList.next);
        PSearchObject obj = CONTAINING_RECORD(li, SearchObject, next);

        PSearchObject pnext = obj->continueobj;
        while (pnext)
        {
            PSearchObject psavenext = pnext->continueobj;
            FreeMemory(pnext);
            pnext = psavenext;
        }
        FreeMemory(obj->name);
        FreeMemory(obj->data);
        FreeMemory(obj);
    }
    return search_count;
}
