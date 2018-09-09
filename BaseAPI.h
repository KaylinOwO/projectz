#ifndef _CBASEAPI_H_
#define _CBASEAPI_H_

#include <minwindef.h>

//===================================================================================
class CBaseAPI
{
public:
	HMODULE GetModuleHandleSafe(const char* pszModuleName);
	void BaseUponModule(HMODULE hmModule);
	char* GetDirectoryFile(char *szFile);
	void BuildDebugConsole(void);
	void LogToConsole(const char* pszMessage, ...);
	void LogToFile(const char* pszMessage, ...);

private:
	char szDirectory[255];
	DWORD dwMessage;
	HANDLE hDebugConsole;
};
//===================================================================================
extern CBaseAPI gBaseAPI;
//==================================================================================
#endif