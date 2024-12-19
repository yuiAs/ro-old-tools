#ifndef SHAREDDATA_HPP
#define SHAREDDATA_HPP


#include <windows.h>
#include <tchar.h>


template <class T> 
class CSharedData
{
	T* _data;
	HANDLE _handle;

public:

	CSharedData(const TCHAR* object_name) : _data(0), _handle(NULL) { _acquire(object_name); }
	~CSharedData() { _release(); }

private:

	// �������Ƃ��������̂ւ�

	void _acquire(const TCHAR* object_name)
	{
		if (_handle == NULL)
			_handle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(T), object_name);

		if (_handle != NULL)
		{
			DWORD error = ::GetLastError();

			// �̈�m��
			_data = reinterpret_cast<T*>(::MapViewOfFile(_handle, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0));

			if ((_data != 0) && (error != ERROR_ALREADY_EXISTS))
				::ZeroMemory(_data, sizeof(T));	// �Y��object_name�ŏ��߂Ċm�ۂ��ꂽ�̈�Ȃ�0������
		}
	}

	// �J��

	void _release()
	{
		if (_data != 0)
		{
			::UnmapViewOfFile(_data);
			_data = 0;
		}

		if (_handle != NULL)
		{
			::CloseHandle(_handle);
			_handle = NULL;
		}

		// CreateFileMapping()�͓����ɃJ�E���^�����Ă���ۂ�����
		// �Q�ƃJ�E���^��0�ɂȂ�Ȃ�����object���͍̂폜����Ȃ��݂����ł�
		// ����Ȃ�(GetLastError() == ERROR_ALREADY_EXISTS)�̏ꍇ��Flag�𗧂ĂĂ����Ƃ����Ă�����
	}

public:

	// �C���^�[�t�F�[�X
	// �֐����ł킩�邾�낤���痪

	bool alive() { return ((_handle != NULL) && (_data != 0)); }
	T* get() { return _data; }	// ���E����...

};


#endif	// #ifndef SHAREDDATA_HPP
