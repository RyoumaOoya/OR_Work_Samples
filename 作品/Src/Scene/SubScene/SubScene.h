#pragma once

class SubScene
{
public:
	friend class SceneManager;
	enum class State{
		MainStop,	//メインシーンを止めるか

	};
public:
	SubScene();
	virtual ~SubScene();

	//初期化
	virtual void Init() = 0;

	//ロード
	virtual void Load() = 0;

	//ロード完了
	virtual void OnLoadEnd() = 0;

	//ステップ処理
	virtual bool Step();

	//更新処理
	virtual void UpDate() = 0;

	//描画
	virtual void Draw() = 0;

	//後処理
	virtual void Fin() = 0;

public:
	inline bool isState(int state) const { return (State_ & (0x01ULL << state)) != 0; }

protected:
	inline void AddState(int state) { State_ |= 0x01ULL << state; }
	void Delete();

private:
	unsigned __int64 State_;

	// vfptr(8byte)
};

/*
	SubScene メモリイメージ

	State_	8byte
	vfptr	8byte
*/

namespace Size {
	constexpr size_t SUB_SCENE_BASE = sizeof(SubScene);
}