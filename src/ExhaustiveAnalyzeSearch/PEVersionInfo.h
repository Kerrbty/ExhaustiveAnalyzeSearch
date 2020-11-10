#ifndef _PE_FILE_VERSION_HEADER_HH_H_
#define _PE_FILE_VERSION_HEADER_HH_H_




bool GetPEVersionA(const char* filename, unsigned long* dwVersionMS, unsigned long* dwVersionLS);
bool GetPEVersionW(const wchar_t* filename, unsigned long* dwVersionMS, unsigned long* dwVersionLS);




#endif // _PE_FILE_VERSION_HEADER_HH_H_ 
