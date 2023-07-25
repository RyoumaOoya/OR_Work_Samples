#pragma once
//SceneManager.h
//�V�[���Ǘ�
//�e�V�[���ւ̃A�N�Z�X�͕K��SceneManager���o�R
#include "Define.h"
#include "GameSystem/Allocator/Allocator.h"
#include "GameSystem/Thread/Thread.h"
#include "GameSystem/Thread/WinEvent.h"
#include <vector>

class Scene;
class SubScene;

class SceneManager
{
	SINGLETON(SceneManager);
private:
	template<class T>
	struct SCENE{
	public:
		constexpr SCENE(Pointer<T> scene, int state) :pScene_(scene), State_(state) {};
		~SCENE() { if (pScene_)MyFree(pScene_); }
		Pointer<T> pScene_;
		unsigned __int32 State_;
	private:
		char buffer[4] = { 0 };
	};

	static constexpr size_t SCENEDATA_SIZE = sizeof(SceneManager::SCENE<int>);
private:
	SceneManager();
	~SceneManager();
public:
	//������
	void Init();

	//���[�v
	void Loop();

	void Draw();

	//�㏈��
	void Fin();

	//�P�ԍŏ��̃V�[�����J�n����
	void StartFirstScene(u16 sceneID);

	//�����̃V�[���ɕύX����
	void ChangeScene(u16 sceneID);

	//�T�u�V�[����ǉ�����
	void AddSubScene(u16 subSceneId);

	// ���݂̃V�[�����擾����
	SCENE<Scene>* GetNowScene() { return &Scene_; }
	std::vector<Pointer<SCENE<SubScene>>>& GetSubScenes() { return SubScene_; };
private:
	//�V�[���𐶐�����
	void CreateScene(u16 sceneID);

	void DelSubScene();

	//���̉�ʂɑJ�ڂ�����
	void ChangeNextScene();

	// �V�[�����[�v���̊e�֐�
	void SceneInit();
	void SceneLoad();
	void SceneLoadEnd();
	void SceneLoop();
	void SceneFin();

	void SubSceneInit(Pointer<SCENE<SubScene>>& scene);
	void SubSceneLoad(Pointer<SCENE<SubScene>>& scene);
	void SubSceneLoadEnd(Pointer<SCENE<SubScene>>& scene);
	void SubSceneLoop(Pointer<SCENE<SubScene>>& scene);
	void SubSceneFin(Pointer<SCENE<SubScene>>& scene);

	bool isMainLoop();
public:
	bool GetFinFlg() { return FinFlg_; }
private:
	//���|�C���^�P�ŊǗ�����
	SCENE<Scene> Scene_;
	std::vector<Pointer<SCENE<SubScene>>> SubScene_;

	//���݂̃V�[��ID
	u16 CurrentSceneID_;
	u16 NextSceneID_;
	//���C���V�[���̃X�e�b�v�����s���邩
	bool MainStep_;
	bool FinFlg_;
private:
	char buffer[2] = { 0 };
};

namespace Size {
	constexpr size_t SCENE_MANAGER = sizeof(SceneManager);
}