#pragma once
#include "SDK.h"

class CMisc
{
public:

	void Run(CBaseEntity* pLocal, CUserCmd* pCommand, bool bSendPacket);
	bool Spam;


private:

	//Other funcs
	void	RandomSeed(int iSeed);
	void NoisemakerSpam(PVOID kv);
};

extern CMisc gMisc;