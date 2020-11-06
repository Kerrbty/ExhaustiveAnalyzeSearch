#ifndef _PE_FILE_MEMORY_INFO_HEADER_HH_H_
#define _PE_FILE_MEMORY_INFO_HEADER_HH_H_
#include "MemoryBase.h"
#include <defs.h>

class PEFileMemoryInfo: public CMemoryInfo{
public:
    PEFileMemoryInfo(const char* filename);
    PEFileMemoryInfo(const wchar_t* filename);
    virtual ~PEFileMemoryInfo();

    virtual bool IsCurrentProcessMemory();  // 分析的内存数据是本进程内的吗 

protected:
    bool LoadFileSuccess();
    void CloseFileHandle();
    virtual uint32_t AnalyzeMemoryInfo(); // 分析内存信息 

private:
    wchar_t* m_file_name;
    HMODULE m_PEModule;
};

#endif // _PE_FILE_MEMORY_INFO_HEADER_HH_H_ 
