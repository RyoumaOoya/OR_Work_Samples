#pragma once
#include "DxLib.h"
#include "../../GameSystem/Allocator/Allocator.h"

namespace MyLib
{
	class Character;
	//アニメーションの更新など
	class Anime
	{
	public:
		enum class PlayType :int {
			Loop,	//ループ再生
			End,	//一回だけ
		};
	public:
		Anime();
		Anime(Pointer<Character> Owner);
		~Anime();

	public:
		void Load();
		void Step();
		void UpDate();
		void Fin();

	public:
		//オーナー設定
		void				SetOwner(Pointer<Character> owner) { Owner_ = owner; }
		//オーナー取得
		Pointer<Character>	GetOwner() const { return Owner_; }

		/*
			アニメーションを切り替える
			引数: animeindex 種類, playtype 再生の種類
		*/
		void ChangeAnime(int animeindex, PlayType playtype);

		//アニメの数を取得
		int	GetAnimNum()	const { return AnimNum_; }
		//現在のアニメ番号を取得
		int	GetAnimIndex()	const { return AnimIndex_; }

		//アタッチしているアニメの総再生時間を取得
		double GetTotalTime()	const { return TotalTime_; }
		//現在の再生時間を取得
		double GetCorrentTime() const { return CurrentTime_; }

		//アニメの再生速度を設定
		void	SetSpeed(double spd) { AnimSpd_ = spd; }
		//アニメの再生速度を取得
		double	GetSpeed() const { return AnimSpd_; }

		//アニメが終わっているか
		bool isEnd() const { return CurrentTime_ == TotalTime_; }

	private:
		Pointer<Character>	Owner_;				//アニメーションする対象
		int					AnimNum_;			//アニメの総数
		int					AnimIndex_;			//現在のアニメ番号
		PlayType			PlayType_;			//再生の種類
		int					AttachAnim_;		//アタッチしたアニメ
		double				TotalTime_;			//アタッチしているアニメの総再生時間
		double				CurrentTime_;		//現在の再生時間
		double				AnimSpd_;			//アニメの再生速度
	};
}
/*
	Anime メモリイメージ

	Owner_		 16byte
	AnimNum_	 4byte	AnimIndex_	4byte
	PlayType_	 4byte	AttachAnim_ 4byte
	TotalTime_	 8byte
	CurrentTime_ 8byte
	AnimSpd_	 8byte
*/

namespace Size {
	constexpr size_t ANIME = sizeof(MyLib::Anime);
}