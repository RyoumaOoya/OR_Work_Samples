#include "ProduceEnemy.h"
#include "ProduceParameter.h"
#include "Src/Enemy/EnemyManager.h"
#include "Src/Enemy/EnemyBase.h"
#include "Src/Enemy/EnemyParameter.h"
#include "Time/Time.h"
#include "GameSystem/Common/Common.h"

constexpr auto Data = PRODUCE_OBJECT_DATABASE[IPRODUCE_OBJECT_ID::ENEMY];
namespace
{
	constexpr float InitialProbability = 0.05f;
	constexpr float GrowthRate = 0.01f;
	constexpr float ProduceDis = 50.f;
}

ProduceEnemy::ProduceEnemy()
{
	ID_ = PRODUCE_OBJECT_ID::ENEMY;
	ProduceInterval_ = 0.f;
	Time_ = 0.f;
	Probability = InitialProbability;
}

ProduceEnemy::~ProduceEnemy()
{
}

void ProduceEnemy::Init(Pointer<Object> this_ptr)
{
	Time_ = CTime::StartCountSecf();
}

void ProduceEnemy::Step()
{
	ProbabilityUpdate();
	if (CTime::GetCountSec(Count_) >= ProduceInterval_) {
		if (Common::GetRandomf(1.f) <= Probability) {
			Produce();
			Produce();
			Produce();
			++ProduceCount_;
		}
		ProduceInterval_ = ReconfigureInterval();
		Count_ = CTime::StartCountSec();
	}

}

void ProduceEnemy::FrontDraw()
{
	DrawFormatString(0, 1_ROW, color::Black, "¶¬—¦:%.1f", static_cast<double>(Probability) * 100.0);
}

void ProduceEnemy::Produce()
{
	auto angle = Common::GetRandomf(2_PI_F, 8);
	auto enemy = EnemyManager::GetInstance()->CreateEntity(EnemyID::Enemy1);
	auto vec = Vector(0.f, 0.f, ProduceDis).Rotation(Vector(0.f, angle, 0.f));
	enemy->SetPos(vec);
}

void ProduceEnemy::ProbabilityUpdate()
{
	Probability = InitialProbability + logf(1.f + CTime::GetCountSecf(Time_) * GrowthRate);
}
