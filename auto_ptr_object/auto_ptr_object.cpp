#include <stdio.h>

#include <memory>
// #include <tchar.h>
// #include <windows.h>
#include <tchar.h>
#include <xutility>
// 
// #include <dbghelp.h>  
// #pragma comment(lib,"Dbghelp.lib")
// #define APP_VERSION "FreeTT 1.5.0"
// 
// 
// void CreateMiniDump(struct _EXCEPTION_POINTERS* ExceptionInfo)  
// {  
// 	//char szFile[MAX_PATH+1] = {0};  
// 	//_snprintf(szFile, MAX_PATH, ".\\cc_%u.dmp", time(NULL));  
// 	HANDLE hFile = CreateFile(L"crash.dmp", GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);  
// 	if( INVALID_HANDLE_VALUE == hFile )
// 	{  
// 		return;
// 	}  
// 
// 	MINIDUMP_EXCEPTION_INFORMATION mei;  
// 	mei.ThreadId = GetCurrentThreadId();
// 	mei.ClientPointers = TRUE;  
// 	mei.ExceptionPointers = ExceptionInfo;  
// 
// 	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &mei, NULL, NULL);  
// 	CloseHandle(hFile);  
// }
// long WINAPI MSJUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)  
// {  
// 	CreateMiniDump(pExceptionInfo);  
// 	return EXCEPTION_CONTINUE_SEARCH;  
// } 
template<typename T>
class auto_iterator /*: public iterator<random_access_iterator_tag, T *>*/
{
public:
	auto_iterator() : _auto_ptr(0) {}
	auto_iterator(std::auto_ptr<T> *auto_ptr) 
	{
		_auto_ptr = auto_ptr;
	}
	auto_iterator operator ++ ()
	{
		_auto_ptr++;
	}
	bool operator != ( auto_iterator<T>& auto_iter) 
	{
		return _auto_ptr != auto_iter._auto_ptr;
	}
	T* operator * ()
	{
		return _auto_ptr->get();
	}
private:
	std::auto_ptr<T> *_auto_ptr;
};
template<typename T>
class auto_vector
{
	typedef auto_iterator<T> iterator;
public:
	auto_vector() {  printf("auto vector created\n");}
	explicit auto_vector( int initial_capacity) :_capacity(initial_capacity),_arr(0),_end(0)
	{ 
		_arr = new std::auto_ptr<T>[_capacity]; 
	}
	~auto_vector()
	{
		if (0 != _arr)
		{
			delete[] _arr; _arr = 0;
		}
	}

	void assign_direct(T * t)
	{
		_arr[_end++].reset(t);
	}
	void push_back(std::auto_ptr<T> &p) { _arr[_end++] = p; }
	std::auto_ptr<T> pop_back() const {  return _arr[--_end]; }

	iterator begin() { return _arr; }
	iterator end() { return _arr + _end; }
private:
	std::auto_ptr<T> *_arr;
	int _end;
	int _capacity;
};

class BASE
{
public:
	BASE () 
	{
		printf("base created\n");
	}
	~BASE () 
	{
		printf("base destructed\n");
	}
};
int _tmain(int argc, _TCHAR* argv[])  
{  
	// 	SetUnhandledExceptionFilter(MSJUnhandledExceptionFilter);  
	auto_vector<BASE> autovec(3);

	BASE* pbase1 = new BASE();

	autovec.assign_direct(pbase1);
	autovec.begin();
	autovec.assign_direct(new BASE());

	std::auto_ptr<BASE> auto1( new BASE());
	std::auto_ptr<BASE> auto2( new BASE());
	autovec.push_back(auto1);
	autovec.push_back(auto2);

	return 0; 

} 
