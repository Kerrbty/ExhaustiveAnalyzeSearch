#include "PEFileMemoryInfo.h"
#include <defs.h>
#include "LoadModel.h"
#include "printInfo.h"

PEFileMemoryInfo::PEFileMemoryInfo(const char* filename)
    :m_PEModule(NULL)
{
    if (filename != NULL)
    {
        int iTextLen = MultiByteToWideChar( CP_ACP,
            0,
            (PCHAR)filename,
            -1,
            NULL,
            0 );

        m_file_name = 
            (PWSTR)AllocMemory((iTextLen+1)*sizeof(WCHAR));

        if (m_file_name)
        {
            MultiByteToWideChar( CP_ACP,
                0,
                (PCHAR)filename,
                -1,
                m_file_name,
                iTextLen );
        }
    }
}

PEFileMemoryInfo::PEFileMemoryInfo(const wchar_t* filename)
    :m_PEModule(NULL)
{
    if (filename != NULL)
    {
        int len = wcslen(filename);
        m_file_name = 
            (PWSTR)AllocMemory((len+1)*sizeof(WCHAR));
        if (m_file_name)
        {
            wcscpy_s(m_file_name, len+1, filename);
        }
    }
}

PEFileMemoryInfo::~PEFileMemoryInfo()
{
    FreeMemory(m_file_name);
    CloseFileHandle();
}

// �������ڴ������Ǳ������ڵ��� 
bool PEFileMemoryInfo::IsCurrentProcessMemory()
{
    // PE���ص����ڴ棬��Զ����true 
    return true;
}

bool PEFileMemoryInfo::LoadFileSuccess()
{
    if (m_file_name == NULL)
    {
        return false;
    }

    m_PEModule = LoadDllFromFileW(m_file_name, FALSE);
    if (m_PEModule)
    {
        return true;
    }
    return false;
}

void PEFileMemoryInfo::CloseFileHandle()
{
    if (m_PEModule)
    {
        FreeMyDlls(m_PEModule, FALSE);
    }
}

// �����ڴ���Ϣ 
uint32_t PEFileMemoryInfo::AnalyzeMemoryInfo()
{
    if (IsAnalyzeMemory())
    {
        return m_memlist.size();
    }

    if (!LoadFileSuccess())
    {
        return 0;
    }

    printDebugW(L"Start Analyze File Info: %s\n", m_file_name);
    // ���� 
    PIMAGE_DOS_HEADER Header = (PIMAGE_DOS_HEADER)m_PEModule;
    PIMAGE_NT_HEADERS peheader = 
        (PIMAGE_NT_HEADERS)((DWORD)Header + Header->e_lfanew);

    PIMAGE_SECTION_HEADER SectionHeader = (PIMAGE_SECTION_HEADER)( (DWORD)peheader + 
        sizeof(peheader->FileHeader) + 
        sizeof(peheader->Signature) +
        peheader->FileHeader.SizeOfOptionalHeader ); 

    DWORD SectionNum = peheader->FileHeader.NumberOfSections; 
    for (DWORD i=0; i<SectionNum; i++) 
    {
        DWORD ulsize = SectionHeader[i].Misc.VirtualSize ;
        if ( ulsize > SectionHeader[i].SizeOfRawData )
        {
            ulsize = SectionHeader[i].SizeOfRawData;
        }

        printDebugA("Find section(%u): base: 0x%p, len: 0x%p, Characteristics: %08x\n", 
            i,
            SectionHeader[i].VirtualAddress,
            ulsize,
            SectionHeader[i].Characteristics );

        meminfo searchmem = {0};
        searchmem.base = (LPBYTE)Header + SectionHeader[i].VirtualAddress;
        searchmem.len = ulsize;
        searchmem.access = 0;
        if (SectionHeader[i].Characteristics&IMAGE_SCN_MEM_EXECUTE)
        {
            searchmem.access |= MEMORY_EXECUTEABLE;
        }
        if (SectionHeader[i].Characteristics&IMAGE_SCN_MEM_READ)
        {
            searchmem.access |= MEMORY_READABLE;
        }
        if (SectionHeader[i].Characteristics&IMAGE_SCN_MEM_WRITE)
        {
            searchmem.access |= MEMORY_WRITEABLE;
        }
        m_memlist.push_back(searchmem);
    }

    SetAnalyzeMemory(true);
    return m_memlist.size();
}

