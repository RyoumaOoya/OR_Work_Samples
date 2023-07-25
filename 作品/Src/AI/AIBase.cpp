#include "AIBase.h"
#include "Src/Player/PlayerManager.h"

AIBase::AIBase()
{
}

AIBase::~AIBase()
{
}

float AIBase::GetPlayerDist(const Pointer<Object>& object, bool xz)
{
	if (xz)
		return object->GetPos().Create(GET_PLAYER_POS).SetY(0.0f).Long();
	else
		return object->GetPos().Create(GET_PLAYER_POS).Long();
}

float AIBase::GetPlayerDistSq(const Pointer<Object>& object, bool xz)
{
	if (xz)
		return object->GetPos().Create(GET_PLAYER_POS).SetY(0.0f).LongSq();
	else
		return object->GetPos().Create(GET_PLAYER_POS).LongSq();
}

Vector AIBase::GetPlayerForwardVec(const Pointer<Object>& object)
{
	return object->GetPos().Create(GET_PLAYER_POS).Normalize();
}
