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
		//�A�j���̑�����MV1GetAnimNum�Ŏ擾�ł���
		AnimNum_ = MV1GetAnimNum(Owner_->RenderObject_.Handle_);
		ChangeAnime(0, PlayType::Loop);
	}

	void Anime::Step()
	{
		CurrentTime_ += 1;

		//���ԉ߂�����ŏ��ɖ߂��i�J��Ԃ��A�j���j
		if (CurrentTime_ >= TotalTime_)
		{
			//�ŏ��ɖ߂�
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

		//�A�j�����͂���
		MV1DetachAnim(Owner_->RenderObject_.Handle_, AttachAnim_);
		AttachAnim_ = 0;

		Owner_ = Pointer<Character>();
	}

	void Anime::ChangeAnime(int animeindex, Anime::PlayType playtype)
	{
		if (AnimIndex_ == animeindex) return;

		int handle = Owner_->RenderObject_.Handle_;

		//�A�j�����͂���
		MV1DetachAnim(handle, AttachAnim_);

		//���݂̃��f���C���f�b�N�X���X�V
		AnimIndex_ = animeindex;

		//�A�j���ԍ����g�p���ăA�j�����A�^�b�`
		AttachAnim_ = MV1AttachAnim(handle, AnimIndex_, -1, FALSE);

		//�Đ����Ԃ��X�V
		TotalTime_ = MV1GetAttachAnimTotalTime(handle, AttachAnim_);

		//���ݎ��Ԃ����Z�b�g
		CurrentTime_ = 0.0;
	}
}