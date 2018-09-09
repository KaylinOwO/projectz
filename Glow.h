#include "glowmanager.h"
#include "utlvec.h"

class C_Glow
{
public:
	void init();
	void fsn();

	int registerGlowObject(CBaseEntity *ent, float r, float g, float b, float a, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot);

	CGlowObjectManager* pGlow;

	static bool update;
};

extern C_Glow gGlow;