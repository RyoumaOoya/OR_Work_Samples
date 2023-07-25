#pragma once
#include "AIBase.h"

class EnemyAI :public AIBase
{
public:
	EnemyAI();
	EnemyAI(float Distance, float FollowStart);
	~EnemyAI()override;

	void Init(float Distance, float FollowStart);

	void SetDistance(float Distance) { AdjacentDistance_ = Distance; }
	void SetFollowStartDist(float Distance) { FollowStartDist_ = Distance; }

public:
	u64 GetNextState(Pointer<Object> object) override;

private:
	u64 StandTransition(Pointer<Object> object);
	u64 FloowObjectTransition(Pointer<Object> object);
	u64 ApproachedTransition(Pointer<Object> object);

	float GetTargetDist(const Pointer<Object> object, bool y_flg = true);
	float GetTargetDistSq(const Pointer<Object> object, bool y_flg = true);
protected:
	float AdjacentDistance_;
	float FollowStartDist_;
};

