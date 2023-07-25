#pragma once
#include "DxLib.h"
#include "../../GameSystem/Allocator/Allocator.h"

namespace MyLib
{
	class Character;
	//�A�j���[�V�����̍X�V�Ȃ�
	class Anime
	{
	public:
		enum class PlayType :int {
			Loop,	//���[�v�Đ�
			End,	//��񂾂�
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
		//�I�[�i�[�ݒ�
		void				SetOwner(Pointer<Character> owner) { Owner_ = owner; }
		//�I�[�i�[�擾
		Pointer<Character>	GetOwner() const { return Owner_; }

		/*
			�A�j���[�V������؂�ւ���
			����: animeindex ���, playtype �Đ��̎��
		*/
		void ChangeAnime(int animeindex, PlayType playtype);

		//�A�j���̐����擾
		int	GetAnimNum()	const { return AnimNum_; }
		//���݂̃A�j���ԍ����擾
		int	GetAnimIndex()	const { return AnimIndex_; }

		//�A�^�b�`���Ă���A�j���̑��Đ����Ԃ��擾
		double GetTotalTime()	const { return TotalTime_; }
		//���݂̍Đ����Ԃ��擾
		double GetCorrentTime() const { return CurrentTime_; }

		//�A�j���̍Đ����x��ݒ�
		void	SetSpeed(double spd) { AnimSpd_ = spd; }
		//�A�j���̍Đ����x���擾
		double	GetSpeed() const { return AnimSpd_; }

		//�A�j�����I����Ă��邩
		bool isEnd() const { return CurrentTime_ == TotalTime_; }

	private:
		Pointer<Character>	Owner_;				//�A�j���[�V��������Ώ�
		int					AnimNum_;			//�A�j���̑���
		int					AnimIndex_;			//���݂̃A�j���ԍ�
		PlayType			PlayType_;			//�Đ��̎��
		int					AttachAnim_;		//�A�^�b�`�����A�j��
		double				TotalTime_;			//�A�^�b�`���Ă���A�j���̑��Đ�����
		double				CurrentTime_;		//���݂̍Đ�����
		double				AnimSpd_;			//�A�j���̍Đ����x
	};
}
/*
	Anime �������C���[�W

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