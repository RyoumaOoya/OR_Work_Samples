#pragma once
#include "ProduceObjectBase.h"

class ProduceEnemy : public ProduceObjectBase
{
public:
	ProduceEnemy();
	~ProduceEnemy() override;
public:
	void Init(Pointer<Object> this_ptr)override;
	void Step()override;
	void FrontDraw()override;
	void Produce() override;
	//ämó¶ÇÃçXêV
	void ProbabilityUpdate();
private:
	float Probability;
	float Time_;
};

