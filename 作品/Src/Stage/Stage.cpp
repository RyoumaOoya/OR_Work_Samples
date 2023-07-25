#include "Stage.h"
#include "StageParameter.h"

Stage::Stage()
{
	ID_ =0;
}

Stage::~Stage()
{
	Fin();
}

void Stage::Init(int ID)
{
	const STAGE_DATA& palm = STAGE_DATABASE[ID_ = ID];
	Map_ = MyNew(Map, AllocType::Stage, Tags(MemTag::Stage), static_cast<int>(palm.mapID));
	Sky_ = MyNew(Sky, AllocType::Stage, Tags(MemTag::Stage), static_cast<int>(palm.skyID));
	if (Map_)
		Map_->Init(Map_);
	if(Sky_)
		Sky_->Init(Sky_);

	// ‰“‚­‚ð‚Ú‚©‚·ƒtƒHƒO‚ÌÝ’è
	SetFogEnable(TRUE);
	SetFogColor(palm.fogColor);
	SetFogStartEnd(palm.fogStart, palm.fogEnd);
}

void Stage::Load()
{
	Map_->Load();
	Sky_->Load();
}

void Stage::Step()
{
	Map_->Step();
	Sky_->Step();
}

void Stage::UpDate()
{
	Map_->UpDate();
	Sky_->UpDate();
}

void Stage::FrontDraw()
{
	Map_->FrontDraw();
	Sky_->FrontDraw();
}

void Stage::Delete()
{
	if (Map_)
		MyFree(Map_);
	if (Sky_)
		MyFree(Sky_);
}

void Stage::Fin()
{
	Delete();
}
