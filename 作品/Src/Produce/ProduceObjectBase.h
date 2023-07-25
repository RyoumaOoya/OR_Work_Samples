#pragma once
#include "Object/Object.h"
#include "ProduceParameter.h"

class ProduceObjectBase :public Object
{
public:
	ProduceObjectBase();
	virtual ~ProduceObjectBase() override;
public:
	virtual void Init()override;
	virtual void Init(Pointer<Object> this_ptr);
	virtual void Load() override;
	virtual void Step()override;
	virtual void Delete()override;
public:
	virtual void Produce() = 0;
	auto GetID() { return ID_; }
protected:
	//インターバルの再設定
	float ReconfigureInterval();
	//死亡時の共通処理
	void Death();
protected:
	PRODUCE_OBJECT_ID ID_;
	u32	ProduceCount_;
	double Count_;
	double ProduceInterval_;
};

namespace Size {
	constexpr size_t PRODUCE_OBJECT_BASE = sizeof(ProduceObjectBase) - sizeof(Object);
}