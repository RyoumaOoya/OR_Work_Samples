#pragma once
#include "DxLib.h"
#include "GameSystem/Allocator/Pointer.h"
#include "../ManagerBase.h"
#include <list>
#include <vector>
#include "GameSystem/Thread/CriticalSection.h"
#include <functional>
#include "../Math/MyMathData.h"

namespace MyLib
{
	class SoundBase;
	class BGM;

	//サウンド管理クラス
	class SoundManager :public SingletonBase<SoundManager>
	{
		MANAGER;
	private:
		SoundManager();
		~SoundManager();
	public:
		void Init()override;
		void Load()override;
		void Step()override {}
		void ConflictStep()override {}
		void UpDate()override;
		void FrontDraw()override {}
		void Delete()override;
		void Fin()override;

		/*
			SEハンドルを作成する
			引数: id SEの種類
			戻り値: SEのポインタ
		*/
		Pointer<SoundBase> CreateSEHandle(int id);
		/*
			再生が終了したら自動削除されるSEを再生
			引数: id SEの種類, pos 座標, volume 音量, radius 再生される範囲
		*/
		Pointer<SoundBase> PlaySE(int id, Vector pos, u8 volume = 0, float radius = 0.f);

		//BGMが再生されているか
		bool isBGMPlay();
		//BGMを再生
		void BGMPlay();
		//BGMを止める
		void BGMStop();
		/*
			BGMを変更する
			引数: bgm_id BGMの種類, play そのまま再生するか
		*/
		void ChangeBGM(int bgm_id, bool play = true);

		//リスナーの位置と向きを設定する
		void SetListenerCalc(std::function<void(Vector*, Vector*)> func) { ListenerPosAndFrontCalc_ = func; }
		//リスナーの座標を取得
		Vector GetListenerPos() { return ListenerPos_; }
		//リスナの向いている方向を取得
		Vector GetListenerFrontPos() { return ListenerFrontPos_; }
	private:
		std::vector<Pointer<SoundBase>> SE_Prototypes_;
		std::list<Pointer<SoundBase>> Sounds_;
		Pointer<BGM> BGM_;
		std::function<void(Vector*, Vector*)> ListenerPosAndFrontCalc_;
		Vector ListenerPos_;
		Vector ListenerFrontPos_;

		CriticalSection CS_;
		decltype(Sounds_.begin()) SoundItr_;
	};
}