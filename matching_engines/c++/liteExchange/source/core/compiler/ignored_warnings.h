#ifndef _IGNORED_WARNINGS_H_
#define _IGNORED_WARNINGS_H_

#if defined(_MSC_VER)
//We want to avoid
//"C++ exception specification ignored except to indicate a function is not _declspec(nothrow)"
//https://msdn.microsoft.com/en-us/library/sa28fef8.aspx
#pragma warning(push)
#pragma warning(disable:4290)
#pragma warning(disable:4996)

#endif

#endif