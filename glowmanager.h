#include <Windows.h>
#include "utlvec.h"
#include "Pred.h"

struct GlowObjectDefinition_t {
	CBaseHandle m_hEntity;
	Vector m_vGlowColor;
	float m_flGlowAlpha;
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	int m_nSplitScreenSlot;
	int m_nNextFreeSlot;
};

class CGlowObjectManager {
public:
	CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions;
	int m_nFirstFreeSlot;
};