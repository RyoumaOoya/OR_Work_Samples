#pragma once
#include "DxLib.h"
#include "GameSystem/Allocator/Pointer.h"
#include "../typedef.h"

namespace MyLib
{
	//サウンド関連の基底クラス
	class SoundBase
	{
		friend class SoundManager;
		friend class Allocator;
	protected:
		SoundBase();
		virtual ~SoundBase();
	public:
		virtual Pointer<SoundBase> Clone() = 0;

		virtual void Init();
		virtual void Load() = 0;
		void Load(const char* path);
		void Load(int handle);
		//ハンドルのロードが終わるまで待つ
		void WaitLoad() { WaitHandleASyncLoad(Handle_); }
		virtual void UpDate();	//サウンドのボリューム、パンの設定など
		virtual void Delete();
		virtual void Fin();

		/*
			再生
			引数: play_type 再生の種類
		*/
		void Play(int play_type = DX_PLAYTYPE_BACK);
		//再生の停止
		void Stop();
		u8 GetID() { return ID_; }
		u8 GetState() { return State_; }
		u8 GetVolume() { return Volume_; }
		void SetVolume(u8 volume) { Volume_ = volume; ChangeVolumeSoundMem(Volume_, Handle_); }
		void SetState(u8 state) { State_ = state; }
	protected:
		int Handle_;
		u8  Volume_;
		u8 ID_;
		u8 State_;
	};
}