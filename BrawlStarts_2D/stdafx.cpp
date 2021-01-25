// stdafx.cpp : source file that includes just the standard includes
// BrawlStartsClient.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


/*
void ErrorMessage(char * strMessage)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	TRACE("\n%s - Error Message : %s\n", strMessage, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
*/