//SceneManager.cpp
//シーン管理
//各シーンへのアクセスは必ずSceneManagerを経由
#include "DxLib.h"
#include "SceneManager.h"
#include "SceneParameter.h"
#include "SubScene/SubScene.h"
#include "Play.h"
#include "Title.h"
#include "Render/Fade/Fade.h"
#include "Message/Message.h"
#ifdef _DEBUG
#include "Input/InputManager.h"
#endif

SceneManager::SceneManager()
	: Scene_(Pointer<Scene>(), static_cast<u32>(SCENE_STATE::INIT))
{
	NextSceneID_ = static_cast<u16>(SCENE_ID::NUM);
	CurrentSceneID_ = static_cast<u16>(SCENE_ID::NUM);
	MainStep_ = true;
	FinFlg_ = false;
}

SceneManager::~SceneManager()
{
	Fin();
}

//初期化
void SceneManager::Init()
{
	//ここはシーン管理の初期化であって、
	//各シーンの初期化ではない
	Fade::Init();
}

//ループ
void SceneManager::Loop()
{
	static void (SceneManager::*const Scene_func[])() = {
		&SceneManager::SceneInit,
		&SceneManager::SceneLoad,
		&SceneManager::SceneLoadEnd,
		&SceneManager::SceneLoop,
		&SceneManager::SceneFin,
	};

	(this->*Scene_func[Scene_.State_])();

	static void (SceneManager::*const SubScene_func[])(Pointer<SCENE<SubScene>>&) = {
		&SceneManager::SubSceneInit,
		&SceneManager::SubSceneLoad,
		&SceneManager::SubSceneLoadEnd,
		&SceneManager::SubSceneLoop,
		&SceneManager::SubSceneFin
	};

	for (auto sub_scene : SubScene_) {
		(this->*SubScene_func[sub_scene->State_])(sub_scene);
	}
	if (Scene_.State_ == static_cast<u32>(SCENE_STATE::LOOP)) {
		Scene_.pScene_->UpDate();
	}
	for (auto sub_scene : SubScene_) {
		if (sub_scene->State_ != static_cast<u32>(SCENE_STATE::LOOP))
			continue;
		sub_scene->pScene_->UpDate();
	}
	DelSubScene();

	Fade::Step();
}

void SceneManager::Draw()
{
	if (Scene_.State_ == static_cast<u32>(SCENE_STATE::LOOP)) {
		Scene_.pScene_->FrontDraw();
	}
	for (auto sub_scene : SubScene_) {
		if (sub_scene->State_ != static_cast<u32>(SCENE_STATE::LOOP))
			continue;
		sub_scene->pScene_->Draw();
	}
}

// シーンループ中の各関数
void SceneManager::SceneInit()
{
	Scene_.pScene_->Init();
	Scene_.State_ = static_cast<u32>(SCENE_STATE::LOAD);
}

void SceneManager::SceneLoad()
{
	Scene_.pScene_->Load();
	Scene_.State_ = static_cast<u32>(SCENE_STATE::LOAD_END);
}

void SceneManager::SceneLoadEnd()
{
	Scene_.pScene_->OnLoadEnd();
	Scene_.State_ = static_cast<u32>(SCENE_STATE::LOOP);
}

void SceneManager::SceneLoop()
{
#ifdef _DEBUG
	if (MainStep_) {
		// デバッグの早送り機能
		int loop = (GET_KEY->Input(KEY_INPUT_Q, INPUT_TYPE::Down)) ? 10 : 1;
		for (int i = 0; i < loop; i++) {
			Scene_.pScene_->Step();
		}
	}
#else
	if (MainStep_) {
		Scene_.pScene_->Step();
	}
#endif
}

void SceneManager::SceneFin()
{
	Scene_.pScene_->Fin();
	ChangeNextScene();
}

void SceneManager::SubSceneInit(Pointer<SCENE<SubScene>>& scene)
{
	scene->pScene_->Init();
	scene->State_ = static_cast<u32>(SCENE_STATE::LOAD);
}

void SceneManager::SubSceneLoad(Pointer<SCENE<SubScene>>& scene)
{
	scene->pScene_->Load();
	scene->State_ = static_cast<u32>(SCENE_STATE::LOAD_END);
}

void SceneManager::SubSceneLoadEnd(Pointer<SCENE<SubScene>>& scene)
{
	scene->pScene_->OnLoadEnd();
	scene->State_ = static_cast<u32>(SCENE_STATE::LOOP);
}

void SceneManager::SubSceneLoop(Pointer<SCENE<SubScene>>& scene)
{
	if (scene->pScene_->Step())
		scene->State_ = static_cast<u32>(SCENE_STATE::FIN);
}

void SceneManager::SubSceneFin(Pointer<SCENE<SubScene>>& scene)
{
	scene->pScene_->Fin();
	scene->State_ = static_cast<u32>(SCENE_STATE::FIN_END);
}

bool SceneManager::isMainLoop()
{
	bool main_step = true;
	for (auto sub_scene : SubScene_) {
		if (sub_scene->pScene_->isState(static_cast<int>(SubScene::State::MainStop)))
			main_step = false;
	}
	return main_step;
}

//後処理
void SceneManager::Fin()
{
	if (Scene_.pScene_) {
		MyFree(Scene_.pScene_);
	}

	for (auto itr = SubScene_.begin(); itr != SubScene_.end();) {
		Pointer<SCENE<SubScene>> scene = *itr;
		MyFree(scene);
		itr = SubScene_.erase(itr);
	}
	Fade::Fin();
}

//１番最初のシーンを開始する
void SceneManager::StartFirstScene(u16 sceneID)
{
	//既に何かシーンが動いてるのであれば即終了
	if (Scene_.pScene_) return;

	//シーンを生成して初期化から開始する
	CreateScene(sceneID);
	CurrentSceneID_ = sceneID;
	Scene_.State_ = static_cast<u32>(SCENE_STATE::INIT);
}

//引数のシーンに変更する
void SceneManager::ChangeScene(u16 sceneID)
{
	//次に遷移するシーンを覚えておき、終了処理へ
	NextSceneID_ = sceneID;
	Scene_.State_ = static_cast<u32>(SCENE_STATE::FIN);
}

//サブシーン追加
void SceneManager::AddSubScene(u16 subSceneId)
{
	Pointer<SubScene> sucScene;
	switch (SUB_SCENE_ID(subSceneId))
	{
	//case SUB_SCENE_ID::PAUSE: sucScene = (Pointer<SubScene>)MyNew(PAUSE, AllocType::SceneData, Tags(MemTag::SubScene));	break;
	}

	SubScene_.push_back(MyNew(SCENE<SubScene>, AllocType::SceneData, Tags(MemTag::SubScene), sucScene, static_cast<int>(SCENE_STATE::INIT)));

	MainStep_ = isMainLoop();
}

//シーンを生成する
void SceneManager::CreateScene(u16 sceneID)
{
	//シーンを生成
	switch ((SCENE_ID)sceneID) {
	case SCENE_ID::TATLE:	Scene_.pScene_ = MyNew(Title, AllocType::SceneData, Tags(MemTag::MainScene));		break;
	case SCENE_ID::PLAY:	Scene_.pScene_ = MyNew(Play, AllocType::SceneData, Tags(MemTag::MainScene));		break;
	}
}

void SceneManager::DelSubScene()
{
	for (auto itr = SubScene_.begin(); itr != SubScene_.end();) {
		Pointer<SCENE<SubScene>> scene = *itr;
		if (scene->State_ == static_cast<u32>(SCENE_STATE::FIN_END)) {
			MyFree(scene->pScene_);
			MyFree(scene);
			itr = SubScene_.erase(itr);
			MainStep_ = isMainLoop();
		}
		else itr++;
	}
}

//次のシーンに遷移する
void SceneManager::ChangeNextScene()
{
	//終了したシーンを削除
	MyFree(Scene_.pScene_);
	//次のシーンを生成
	CreateScene(NextSceneID_);
	//現在のシーンID設定
	CurrentSceneID_ = NextSceneID_;
	//状態を初期化に戻す
	Scene_.State_ = static_cast<u32>(SCENE_STATE::INIT);
}