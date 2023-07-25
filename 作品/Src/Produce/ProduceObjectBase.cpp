#include "ProduceObjectBase.h"
#include "ProduceParameter.h"
#include "GameSystem/Common/Common.h"
#include "GameSystem/Allocator/Allocator.h"
#include "GameSystem/FrameRate/FrameRate.h"
#include "Time/Time.h"

ProduceObjectBase::ProduceObjectBase() :ID_(PRODUCE_OBJECT_ID::END), ProduceCount_(0), Count_(0.f), ProduceInterval_(0.f)
{
	SetActive(true);
	SetRenderTag(RenderTags::ShadowWrite);
}

ProduceObjectBase::~ProduceObjectBase()
{
}

void ProduceObjectBase::Init()
{
	Init(Allocator::GetPointerStorage(this, AllocType::Produce));
}

void ProduceObjectBase::Init(Pointer<Object> this_ptr)
{
	SetThisPtr(this_ptr);
	Count_ = CTime::StartCountSec();
	ProduceInterval_ = ReconfigureInterval();
}

void ProduceObjectBase::Load()
{
	__super::Load(PRODUCE_OBJECT_DATABASE[static_cast<int>(ID_)].Path.data());
}

void ProduceObjectBase::Step()
{
	if (CTime::GetCountSec(Count_) >= ProduceInterval_) {
		Produce();
		ProduceInterval_ = ReconfigureInterval();
		Count_ = CTime::StartCountSec();
		++ProduceCount_;
	}
}

void ProduceObjectBase::Delete()
{
	__super::Delete();
	EreseModelList();
}

float ProduceObjectBase::ReconfigureInterval()
{
	return Common::GetRandomf(PRODUCE_OBJECT_DATABASE[static_cast<int>(ID_)].ProduceIntervalMin, PRODUCE_OBJECT_DATABASE[static_cast<int>(ID_)].ProduceIntervalMax);
}

void ProduceObjectBase::Death()
{
	SetActive(false);
	SetDeleteFlag();
}
