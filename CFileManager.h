#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_
//===================================================================================
#include <Windows.h>
#include <iostream>
//===================================================================================
class CFileManager
{
public:
	void Initialize(const char* szFileName);
	int ReadInteger(char* szSection, char* szKey, int iDefaultValue);
	float ReadFloat(const char* szSection, const char* szKey, float fltDefaultValue);
	bool ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue);
	char* ReadString(char* szSection, char* szKey, const char* szDefaultValue);
	void WriteInteger(char* szSection, char* szKey, int iValue);
	void WriteFloat(const char* szSection, const char* szKey, float fltValue);
	void WriteBoolean(char* szSection, char* szKey, bool bolValue);
	void WriteString(char* szSection, char* szKey, char* szValue);
private:
	char m_szFileName[255];
};
//===================================================================================
extern CFileManager gFileManager;
//===================================================================================
#endif