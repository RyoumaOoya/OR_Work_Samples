#pragma once
#include "Scene.h"

class Title :public Scene
{
public:
	Title();
	~Title();
public:
	//初期化
	void Init()override;
	//ロード
	void Load()override;
	//ロード完了
	void OnLoadEnd()override;
	//ステップ処理
	void Step()override;
	//更新処理
	void UpDate()override;
	//描画
	void Draw()override;
	//最前面描画
	void FrontDraw()override;
	//後処理
	void Fin()override;
};

