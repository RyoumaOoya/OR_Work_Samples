#include "Render.h"
#include "../Math/MyMath.h"
#include "../GameSystem/Thread/ThreadManager.h"
#include <queue>
#if is3D
#include "ShadowMap/ShadowMapManager.h"
namespace {
	constexpr ShadowMap::ShadowMapData StaticShadow(
		iVector2D(MyMath::Exponentiation(2, 14), MyMath::Exponentiation(2, 14)),	/*�𑜓x*/
		Vector(-500.0f, -1.0f, -500.0f), Vector(500.0f, 200.0f, 500.0f),					/*�͈�*/
		Vector(0.2f, -1.0f, 0.3f));														/*���C�g�x�N�g��*/

	constexpr ShadowMap::ShadowMapData NearShadow(
		iVector2D(MyMath::Exponentiation(2, 13), MyMath::Exponentiation(2, 13)),	/*�𑜓x*/
		Vector(-50.0f, -25.0f, -30.0f), Vector(50.0f, 15.0f, 30.0f),						/*�͈�*/
		Vector(0.2f, -1.0f, 0.3f));														/*���C�g�x�N�g��*/
}
#endif // is3D

void Render::Init()
{
#if is3D
	auto shadow_map_manager = ShadowMapManager::CreateInstance();
	StaticShadow_ = shadow_map_manager->CreateShadowMap(StaticShadow);
	NearShadow_ = shadow_map_manager->CreateShadowMap(NearShadow);
#endif // is3D
}

void Render::Draw()
{
	//DxLib::GetWindowSizeChangeEnableFlag
	if (!Rendering_) {
		Waiting_ = true;
		WaitRender();
		Waiting_ = false;
	}
#if is3D
	auto static_shadow = ShadowMapManager::GetInstance()->GetShadowMap(StaticShadow_);
	auto near_shadow = ShadowMapManager::GetInstance()->GetShadowMap(NearShadow_);

	static std::queue<OffSetPtr<RenderObject3DBase>> ShadowQueue;		//�e�`��p�L���[
	static std::queue<OffSetPtr<RenderObject3DBase>> DrawQueue;			//�ʏ�`��p�L���[
	static std::queue<OffSetPtr<RenderObject3DBase>> NotShadowDrawQueue;//�e���������܂Ȃ��`��p�L���[
	static std::queue<OffSetPtr<RenderObject3DBase>> FrontQueue;		//�őO�ʕ`��p�L���[(�e�Ȃ�)

	if (SetNearShadowAreaFunc) {
		auto pos = SetNearShadowAreaFunc();
		near_shadow->SetShadowMapDrawArea(pos + NearShadow.MinArea, pos + NearShadow.MaxArea);
	}

	{
		std::lock_guard<std::mutex> lock(ModelMtx_);

		for (auto itr = DrawModelList_.begin(); itr != DrawModelList_.end();) {
			auto model = *itr;

			if (model->HasTag(RenderTags::isDeleteTarget)) {
				itr = DrawModelList_.erase(itr);
				model->RemoveTag(RenderTags::isDeleteTarget);
				model->SetTag(RenderTags::isDeleted);
				continue;
			}

			if (model->HasTag(RenderTags::ShadowWrite)) {
				ShadowQueue.push(model);
			}
			if (model->HasTag(RenderTags::isDraw)) {
				(model->HasTag(RenderTags::NotShadowDraw)) ? NotShadowDrawQueue.push(model) : DrawQueue.push(model);
			}
			if (model->HasTag(RenderTags::FrontDraw)) {
				FrontQueue.push(model);
			}
			++itr;
		}
	}

	{	
		//�e�������ݏ���
		ScopedShadowWrite s_sw(near_shadow);
		while (!ShadowQueue.empty())
		{
			auto model = ShadowQueue.front();
			ShadowQueue.pop();
			RenderObject3D::ReadingLock rl(model.get());
			model->Draw();
		}
	}
	{	//�`��
		ScopedShadowDraw s_sw(static_shadow);
		ScopedShadowDraw n_sw(near_shadow);
		while (!DrawQueue.empty())
		{
			auto model = DrawQueue.front();
			DrawQueue.pop();
			RenderObject3D::ReadingLock rl(model.get());
			model->Draw();
		}
	}
	//�e��`�悵�Ȃ��ʏ�`��
	while (!NotShadowDrawQueue.empty())
	{
		auto model = NotShadowDrawQueue.front();
		NotShadowDrawQueue.pop();
		RenderObject3D::ReadingLock rl(model.get());
		model->Draw();
	}
	ClearDrawScreenZBuffer();
	//�őO�ʕ`��
	while (!FrontQueue.empty())
	{
		auto model = FrontQueue.front();
		FrontQueue.pop();
		RenderObject3D::ReadingLock rl(model.get());
		model->Draw();
	}
#endif // is3D
	SortSpriteList();
	{
		std::lock_guard<std::mutex> lock(SpriteMtx_);
		for (auto itr = DrawSpriteList_.begin(); itr != DrawSpriteList_.end();) {
			auto sprite = *itr;

			if (!sprite) {
				itr = DrawSpriteList_.erase(itr);
				continue;
			}

			RenderObject2D::ReadingLock rl(sprite.get());

			if (sprite->HasTag(RenderTags::isDeleteTarget)) {
				itr = DrawSpriteList_.erase(itr);
				sprite->RemoveTag(RenderTags::isDeleteTarget);
				sprite->SetTag(RenderTags::isDeleted);
				continue;
			}

			if (sprite->HasTag(RenderTags::isDraw)) {
				sprite->Draw();
			}
			++itr;
		}
	}
	{
		std::lock_guard<std::mutex> lock(StringMtx_);
		for (auto itr = DrawStringList_.begin(); itr != DrawStringList_.end();) {
			auto str = *itr;

			if (!str) {
				itr = DrawStringList_.erase(itr);
				continue;
			}

			RenderObjectString::ReadingLock rl(str.get());

			if (str->HasTag(RenderTags::isDeleteTarget)) {
				itr = DrawStringList_.erase(itr);
				str->SetTag(RenderTags::isDeleted);
				continue;
			}

			if (str->HasTag(RenderTags::isDraw)) {
				str->Draw();
			}
			++itr;
		}
	}
}

void Render::Fin()
{
#if is3D
	ShadowMapManager::GetInstance()->Fin();
	DrawModelList_.clear();
#endif // is3D
	DrawSpriteList_.clear();
	DrawStringList_.clear();
}

#if is3D
void Render::WriteStaticShadow()
{
	auto static_shadow = ShadowMapManager::GetInstance()->GetShadowMap(StaticShadow_);
	ScopedShadowWrite s_sw(static_shadow);
	{
		std::lock_guard<std::mutex> lock(ModelMtx_);
		for (auto model : DrawModelList_) {
			if (model->HasTag(RenderTags::StaticShadow)) {
				model->Draw();
			}
		}
	}
}
#endif // is3D
