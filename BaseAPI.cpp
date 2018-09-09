#include "SDK.h"
#include "BaseAPI.h" // Base Header
#include <time.h>
//===================================================================================
CBaseAPI gBaseAPI;
//===================================================================================
HMODULE CBaseAPI::GetModuleHandleSafe(const char* pszModuleName)
{
	HMODULE hmModuleHandle = NULL;

	do
	{
		hmModuleHandle = GetModuleHandle(pszModuleName);
		Sleep(200);
	} while (hmModuleHandle == NULL);

	return hmModuleHandle;
}
//===================================================================================
void CBaseAPI::BaseUponModule(HMODULE hmModule)
{
	GetModuleFileName(hmModule, szDirectory, 512);
	for (int i = (int)strlen(szDirectory); i > 0; i--)
	{
		if (szDirectory[i] == '\\')
		{
			szDirectory[i + 1] = 0; break;
		}
	}
}
//===================================================================================
char* CBaseAPI::GetDirectoryFile(char *szFile)
{
	static char path[320];
	strcpy(path, szDirectory);
	strcat(path, szFile);
	return path;
}
//===================================================================================
void CBaseAPI::BuildDebugConsole(void)
{
	static bool bOnce = false;
	if (!bOnce)
	{
		//create the console
		AllocConsole();
		hDebugConsole = (HANDLE)::GetStdHandle(STD_OUTPUT_HANDLE);
		::SetConsoleTitle("The Debug Console");
		::WriteConsole(hDebugConsole, "Welcome to The Debug Console.\n", strlen("Welcome to The Debug Console.\n"), &dwMessage, NULL);
		bOnce = true;
	}
}
//===================================================================================
void CBaseAPI::LogToConsole(const char* pszMessage, ...)
{
	char szLogbuf[4096];
	vsprintf(szLogbuf, pszMessage, (char*)&pszMessage + _INTSIZEOF(pszMessage));
	strcat(szLogbuf, "\n");
	::WriteConsole(hDebugConsole, szLogbuf, strlen(szLogbuf), &dwMessage, NULL);
}
//===================================================================================
void CBaseAPI::LogToFile(const char * pszMessage, ...)
{
	va_list va_alist;
	char szLogbuf[256];
	FILE * fp;
	struct tm * current_tm;
	time_t current_time;

	time(&current_time);
	current_tm = localtime(&current_time);

	sprintf(szLogbuf, "[%02d:%02d:%02d] ", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec);

	va_start(va_alist, pszMessage);
	_vsnprintf(szLogbuf + strlen(szLogbuf), sizeof(szLogbuf) - strlen(szLogbuf), pszMessage, va_alist);
	va_end(va_alist);

	if ((fp = fopen(GetDirectoryFile("Darkstorm.log"), "a")) != NULL)
	{
		fprintf(fp, "%s\n", szLogbuf);
		fclose(fp);
	}
#if DEBUG
	LogToConsole(pszMessage, ...);
#endif
}
//===================================================================================