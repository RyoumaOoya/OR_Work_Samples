//SceneBase.cpp
//シーン用基底クラス
#include "Scene.h"
#include "SceneParameter.h"
#include "SceneManager.h"
#include "Render/Fade/Fade.h"

Scene::Scene()
{
	State_ = static_cast<u32>(State::NONE);
	NextScene_ = -1;
}

Scene::~Scene()
{
}

void Scene::Step()
{
	if (State_ == static_cast<u32>(State::FADEIN) && Fade::IsFadeEnd()) {
		SceneManager::GetInstance()->GetNowScene()->pScene_->OnFadeinEnd();
	}
	if (State_ == static_cast<u32>(State::FADEOUT) && Fade::IsFadeEnd()) {
		SceneManager::GetInstance()->ChangeScene(NextScene_);
	}
}

void Scene::ChangeScene(u32 next_scene, float fade_State_time)
{
	if (Fade::IsFadeEnd()) {
		Fade::FadeOut(fade_State_time);
		NextScene_ = next_scene;
		State_ = static_cast<u32>(State::FADEOUT);
	}
}

void Scene::OnFadeinEnd()
{
	State_ = static_cast<u32>(State::NONE);
}

void Scene::FadeIn()
{
	Fade::FadeIn();
	State_ = (int)State::FADEIN;
}
