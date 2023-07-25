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

	//�T�E���h�Ǘ��N���X
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
			SE�n���h�����쐬����
			����: id SE�̎��
			�߂�l: SE�̃|�C���^
		*/
		Pointer<SoundBase> CreateSEHandle(int id);
		/*
			�Đ����I�������玩���폜�����SE���Đ�
			����: id SE�̎��, pos ���W, volume ����, radius �Đ������͈�
		*/
		Pointer<SoundBase> PlaySE(int id, Vector pos, u8 volume = 0, float radius = 0.f);

		//BGM���Đ�����Ă��邩
		bool isBGMPlay();
		//BGM���Đ�
		void BGMPlay();
		//BGM���~�߂�
		void BGMStop();
		/*
			BGM��ύX����
			����: bgm_id BGM�̎��, play ���̂܂܍Đ����邩
		*/
		void ChangeBGM(int bgm_id, bool play = true);

		//���X�i�[�̈ʒu�ƌ�����ݒ肷��
		void SetListenerCalc(std::function<void(Vector*, Vector*)> func) { ListenerPosAndFrontCalc_ = func; }
		//���X�i�[�̍��W���擾
		Vector GetListenerPos() { return ListenerPos_; }
		//���X�i�̌����Ă���������擾
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