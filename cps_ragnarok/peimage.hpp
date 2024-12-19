#ifndef PEIMAGE_HPP
#define PEIMAGE_HPP

#include "stdafx.h"
#include <map>


class PEImage
{
	HMODULE m_handle;					// �Ȃ��Ă��������ǈꉞ
	IMAGE_NT_HEADERS* m_nt;

	std::map<DWORD, DWORD> m_jmptbl;

public:

	PEImage(void* __module) : m_handle(reinterpret_cast<HMODULE>(__module)) { initialize(); }
	~PEImage() {}

private:

	void initialize();

	DWORD matchFirstByText(const BYTE* __data, size_t __length);
	DWORD matchFirstByData(const char* __string);

public:

	DWORD searchCode(const BYTE* __data, size_t __length);
	DWORD searchCodeRef(const BYTE* __data, size_t __length, size_t __shift);
	DWORD searchString(const char* __string);

	// VirtualProtect���p��RtlCopyMemory
	void attachMemory(void* __dest, const void* __source, size_t __length);

	// ASProtect��image���������`�F�b�N
	bool waitASProtect();

	void analysisFF25();																// jumptable���
	void* rewriteFF25(const TCHAR* __module, const char* __export, void* __function);	// jumptable�C��
	bool isBuildFF25() { return m_jmptbl.empty()==false; }
	void clearFF25() { m_jmptbl.clear(); }
	void displayFF25();																	// debug�p

	void getSectionRange(int __section, DWORD &__start, DWORD &__end);
};


#endif	// #ifndef PEIMAGE_HPP
