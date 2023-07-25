#pragma once
//SceneManager.h
//シーン管理
//各シーンへのアクセスは必ずSceneManagerを経由
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
	//初期化
	void Init();

	//ループ
	void Loop();

	void Draw();

	//後処理
	void Fin();

	//１番最初のシーンを開始する
	void StartFirstScene(u16 sceneID);

	//引数のシーンに変更する
	void ChangeScene(u16 sceneID);

	//サブシーンを追加する
	void AddSubScene(u16 subSceneId);

	// 現在のシーンを取得する
	SCENE<Scene>* GetNowScene() { return &Scene_; }
	std::vector<Pointer<SCENE<SubScene>>>& GetSubScenes() { return SubScene_; };
private:
	//シーンを生成する
	void CreateScene(u16 sceneID);

	void DelSubScene();

	//次の画面に遷移させる
	void ChangeNextScene();

	// シーンループ中の各関数
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
	//基底ポインタ１つで管理する
	SCENE<Scene> Scene_;
	std::vector<Pointer<SCENE<SubScene>>> SubScene_;

	//現在のシーンID
	u16 CurrentSceneID_;
	u16 NextSceneID_;
	//メインシーンのステップを実行するか
	bool MainStep_;
	bool FinFlg_;
private:
	char buffer[2] = { 0 };
};

namespace Size {
	constexpr size_t SCENE_MANAGER = sizeof(SceneManager);
}