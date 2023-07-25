//SceneManager.cpp
//�V�[���Ǘ�
//�e�V�[���ւ̃A�N�Z�X�͕K��SceneManager���o�R
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

//������
void SceneManager::Init()
{
	//�����̓V�[���Ǘ��̏������ł����āA
	//�e�V�[���̏������ł͂Ȃ�
	Fade::Init();
}

//���[�v
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

// �V�[�����[�v���̊e�֐�
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
		// �f�o�b�O�̑�����@�\
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

//�㏈��
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

//�P�ԍŏ��̃V�[�����J�n����
void SceneManager::StartFirstScene(u16 sceneID)
{
	//���ɉ����V�[���������Ă�̂ł���Α��I��
	if (Scene_.pScene_) return;

	//�V�[���𐶐����ď���������J�n����
	CreateScene(sceneID);
	CurrentSceneID_ = sceneID;
	Scene_.State_ = static_cast<u32>(SCENE_STATE::INIT);
}

//�����̃V�[���ɕύX����
void SceneManager::ChangeScene(u16 sceneID)
{
	//���ɑJ�ڂ���V�[�����o���Ă����A�I��������
	NextSceneID_ = sceneID;
	Scene_.State_ = static_cast<u32>(SCENE_STATE::FIN);
}

//�T�u�V�[���ǉ�
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

//�V�[���𐶐�����
void SceneManager::CreateScene(u16 sceneID)
{
	//�V�[���𐶐�
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

//���̃V�[���ɑJ�ڂ���
void SceneManager::ChangeNextScene()
{
	//�I�������V�[�����폜
	MyFree(Scene_.pScene_);
	//���̃V�[���𐶐�
	CreateScene(NextSceneID_);
	//���݂̃V�[��ID�ݒ�
	CurrentSceneID_ = NextSceneID_;
	//��Ԃ��������ɖ߂�
	Scene_.State_ = static_cast<u32>(SCENE_STATE::INIT);
}