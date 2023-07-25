#include "Anime.h"
#include "../Character.h"
namespace MyLib
{
	Anime::Anime() : Owner_()
	{
		AnimNum_ = 0;
		AnimIndex_ = 0;
		PlayType_ = PlayType::End;
		AttachAnim_ = 0;
		TotalTime_ = -1.0;
		CurrentTime_ = 0.0;
		AnimSpd_ = 0.0;
	}

	Anime::Anime(Pointer<Character> Owner) :Owner_(Owner)
	{
		AnimNum_ = 0;
		AnimIndex_ = 0;
		PlayType_ = PlayType::End;
		AttachAnim_ = 0;
		TotalTime_ = -1.0;
		CurrentTime_ = 0.0;
		AnimSpd_ = 0.0;
	}

	Anime::~Anime()
	{
		Fin();
	}

	void Anime::Load()
	{
		//アニメの総数はMV1GetAnimNumで取得できる
		AnimNum_ = MV1GetAnimNum(Owner_->RenderObject_.Handle_);
		ChangeAnime(0, PlayType::Loop);
	}

	void Anime::Step()
	{
		CurrentTime_ += 1;

		//時間過ぎたら最初に戻す（繰り返しアニメ）
		if (CurrentTime_ >= TotalTime_)
		{
			//最初に戻す
			CurrentTime_ = (PlayType_ == PlayType::Loop) ? 0.0 : TotalTime_;
		}
	}

	void Anime::UpDate()
	{
		MV1SetAttachAnimTime(Owner_->RenderObject_.Handle_, AttachAnim_, (float)CurrentTime_);
	}

	void Anime::Fin()
	{
		if (!Owner_)
			return;
		AnimNum_ = 0;
		AnimIndex_ = 0;
		PlayType_ = PlayType::End;
		TotalTime_ = -1.0;
		CurrentTime_ = 0.0;
		AnimSpd_ = 0.0;

		//アニメをはずす
		MV1DetachAnim(Owner_->RenderObject_.Handle_, AttachAnim_);
		AttachAnim_ = 0;

		Owner_ = Pointer<Character>();
	}

	void Anime::ChangeAnime(int animeindex, Anime::PlayType playtype)
	{
		if (AnimIndex_ == animeindex) return;

		int handle = Owner_->RenderObject_.Handle_;

		//アニメをはずす
		MV1DetachAnim(handle, AttachAnim_);

		//現在のモデルインデックスを更新
		AnimIndex_ = animeindex;

		//アニメ番号を使用してアニメをアタッチ
		AttachAnim_ = MV1AttachAnim(handle, AnimIndex_, -1, FALSE);

		//再生時間を更新
		TotalTime_ = MV1GetAttachAnimTotalTime(handle, AttachAnim_);

		//現在時間をリセット
		CurrentTime_ = 0.0;
	}
}