#include "AutoDetonate.h"
#include "ClassIds.h"

/*void CAutoSticky::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	for (int p = 0; p < gInts.EntList->GetHighestEntityIndex(); p++)
	{
		auto first_entity = GetBaseEntity(p);
		if (!first_entity) continue;

		Vector bomb_position; first_entity->GetWorldSpaceCenter(bomb_position);
		int class_id = first_entity->GetClientClass()->iClassID;

		auto is_local_grenade = [&first_entity, &class_id, &pLocal]()
		{
			if (class_id != 212)
				return false;
			if (first_entity->GetPipeType() != 1)
				return false;

			auto get_thrower = first_entity->GetThrower();
			if (!get_thrower) return false;

			auto entity_handle = gInts.EntList2->GetClientEntityFromHandle(get_thrower);
			if (!entity_handle) return false;

			if (entity_handle != pLocal)
				return false;

			return true;
		};

		if (is_local_grenade())
		{
			for (int i = 1; i < gInts.Engine->GetMaxClients(); i++)
			{
				if (i == me) continue;
				auto second_entity = GetBaseEntity(i);
				if (!second_entity || second_entity->IsDormant() || second_entity->GetLifeState() != LIFE_ALIVE) continue;
				if (second_entity->GetTeamNum() == pLocal->GetTeamNum()) continue;

				if (Util->flGetDistance(bomb_position, second_entity->GetAbsOrigin()) < 12.0f)
					pCommand->buttons |= IN_ATTACK2;
			}
		}
	}
}*/
