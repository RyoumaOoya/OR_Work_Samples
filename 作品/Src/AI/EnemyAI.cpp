#include "EnemyAI.h"
#include "Math/MyMath.h"
#include "Src/Enemy/EnemyBase.h"
#include "Src/Entity/EntityManager.h"
#include "Src/Entity/Entity.h"
#include <float.h>

EnemyAI::EnemyAI() :AdjacentDistance_(0.f), FollowStartDist_(0.f)
{
}

EnemyAI::EnemyAI(float Distance, float FollowStart) : AdjacentDistance_(Distance), FollowStartDist_(FollowStart)
{
}

EnemyAI::~EnemyAI()
{
}

void EnemyAI::Init(float Distance, float FollowStart)
{
	AdjacentDistance_ = Distance;
	FollowStartDist_ = FollowStart;
}

u64 EnemyAI::GetNextState(Pointer<Object> object)
{
	switch (object->GetAIState())
	{
	case AI_STATE::STAND:			return StandTransition(object);
	case AI_STATE::FOLLOW_PLAYER:	return StandTransition(object);
	case AI_STATE::FOLLOW_OBJECT:	return FloowObjectTransition(object);
	case AI_STATE::APPROACHED:		return ApproachedTransition(object);
	default:						return AI_STATE::STAND;
	}
}

u64 EnemyAI::StandTransition(Pointer<Object> object)
{
	const auto& entity_list = EntityManager::GetInstance()->GetEntityList();

	float max_dist = MyMath::Squared(FollowStartDist_);
	Pointer<Entity> target;

	for (auto& entity : entity_list) {
		float dist = object->GetPos().Create(entity->GetPos().SetY(0.f)).LongSq();
		if (max_dist > dist) {
			max_dist = dist;
			target = entity;
		}
	}

	if (!target)
		return AI_STATE::FOLLOW_PLAYER;

	static_cast<Pointer<EnemyBase>>(object)->Target_ = target;
	return AI_STATE::FOLLOW_OBJECT;
}

u64 EnemyAI::FloowObjectTransition(Pointer<Object> object)
{
	auto entity = static_cast<Pointer<EnemyBase>>(object);
	if (!entity->Target_)
		return AI_STATE::STAND;

	float dist = GetTargetDistSq(object, false);

	static auto is_stand_transition = [this, dist](Pointer<EnemyBase>& entity) {
		if (!entity->Target_->IsActive() || MyMath::Squared(FollowStartDist_) < dist) {
			entity->Target_ = Pointer<Entity>();
			return true;
		}

		return false;
	};

	return	(is_stand_transition(entity)) ? AI_STATE::STAND :
		(dist < MyMath::Squared(AdjacentDistance_)) ? AI_STATE::APPROACHED :
		AI_STATE::FOLLOW_OBJECT;
}

u64 EnemyAI::ApproachedTransition(Pointer<Object> object)
{
	static auto is_stand_transition = [](const Pointer<Object>& object) {
		auto entity = static_cast<Pointer<EnemyBase>>(object);

		if (!entity->Target_)
			return true;
		if (!static_cast<Pointer<EnemyBase>>(object)->Target_->IsActive()) {
			entity->Target_ = Pointer<Entity>();
			return true;
		}

		return false;
	};

	return	(is_stand_transition(object)) ? AI_STATE::STAND :
		(GetTargetDistSq(object, false) > MyMath::Squared(AdjacentDistance_)) ? AI_STATE::FOLLOW_OBJECT :
		AI_STATE::APPROACHED;
}

float EnemyAI::GetTargetDist(const Pointer<Object> object, bool y_flg)
{
	auto enemy = static_cast<const Pointer<EnemyBase>>(object);
	if (y_flg)
		return enemy->GetPos().Create(enemy->Target_->GetPos()).Long();

	return enemy->GetPos().Create(enemy->Target_->GetPos()).SetY(0.f).Long();
}

float EnemyAI::GetTargetDistSq(const Pointer<Object> object, bool y_flg)
{
	auto enemy = static_cast<const Pointer<EnemyBase>>(object);
	if (y_flg)
		return enemy->GetPos().Create(enemy->Target_->GetPos()).LongSq();

	return enemy->GetPos().Create(enemy->Target_->GetPos()).SetY(0.f).LongSq();
}
