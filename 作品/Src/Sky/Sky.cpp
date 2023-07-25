#include "Sky.h"
#include "SkyParameter.h"
#include "Src/Camera/CameraManager.h"
#include "Render/Render.h"

Sky::Sky(int id)
{
	ID_ = id;
	RenderObject_.Scale_ = Vector(0.9f, 0.7f, 0.9f);
	SetRenderTag(RenderTags::NotShadowDraw);
	RemoveRenderTag(RenderTags::ShadowWrite);
}

Sky::~Sky()
{
	Fin();
}

void Sky::Init()
{
}

void Sky::Init(Pointer<Sky> t)
{
	SetThisPtr(t);
	PushModelList();
}

void Sky::Load()
{
	__super::Load(SKY_DATABASE[ID_].FILE_PATH.data());
}

void Sky::Step()
{
	// プレイヤーに追従する
	Pointer<CameraBase> camera = CameraManager::GetInstance()->GetCamera();
	if (camera) {
		RenderObject_.Pos_ = camera->GetPos();
		// Y座標のみ固定
		RenderObject_.Pos_.y = SKY_POS_Y;
	}
}

void Sky::FrontDraw()
{
}

void Sky::Delete()
{
	__super::Delete();
	EreseModelList();
}

void Sky::Fin()
{
	Delete();
	__super::Fin();
}
