#pragma once
#include "typedef.h"
#include <vector>

class Scene
{
protected:
	enum class State :u32 {
		NONE,
		FADEIN,
		FADEOUT,
	};
public:
	Scene();
	virtual ~Scene();

	//初期化
	virtual void Init() = 0;
	//ロード
	virtual void Load() = 0;
	//ロード完了
	virtual void OnLoadEnd() = 0;
	//ステップ処理
	virtual void Step();
	//更新処理
	virtual void UpDate() = 0;
	//描画
	virtual void Draw() = 0;
	//最前面描画
	virtual void FrontDraw() = 0;
	//後処理
	virtual void Fin() = 0;
protected:
	void ChangeScene(u32 next_scene, float fade_time = 1);
	virtual void OnFadeinEnd();
	void FadeIn();
private:
	u32 NextScene_;
protected:
	u32 State_;
	// vfptr(8byte)
};

/*
	Scene メモリイメージ

	next_scene_	8byte
	state_		8byte
	vfptr		8byte
*/

namespace Size {
	constexpr size_t SCENE_BASE = sizeof(Scene);
}