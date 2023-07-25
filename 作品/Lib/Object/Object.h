#pragma once
#include "../Math/MyMathData.h"
#include "ObjectParameter.h"
#include"GameSystem/Allocator/Pointer.h"
#include <initializer_list>
#include "../typedef.h"
#include "Literal.h"
#include "../Render/RenderObject/RenderObject3D.h"

namespace MyLib
{
	//�I�u�W�F�N�g���N���X
	//��{�I�ɃQ�[�����ɑ��݂���I�u�W�F�N�g�͂��̃N���X���p������
	class Object
	{
		typedef u32 TAG_TYPE;
		friend class Object;
	public:
		Object();
		virtual ~Object();

	public:
		//���������u��
		virtual void OnCollisionBegin(Pointer<Object> other);
		//�������Ă����
		virtual void OnCollisionEnter(Pointer<Object> other);
		//���ꂽ�u��
		virtual void OnCollisionEnd(Pointer<Object> other);

	public:
		virtual void Init();
		virtual void Load();	//�p���p(���܂����f�[�^��ǂݎ��Ƃ���ID�ɉ��������[�h�Ȃ�)
		virtual void Load(const char* file_path);
		void Load(int handle);	//���f���̃R�s�[�p
		virtual void Step() = 0;
		virtual void ConflictStep();	//�O���̕ϐ��̑���p
		virtual void UpDate();
		//virtual void ShadowDraw();
		//virtual void Draw();
		virtual void FrontDraw();
		virtual void Delete();
		virtual void Fin();

	public:
		//�������w��2�d�|�C���^��ݒ�
		void SetThisPtr(const Pointer<Object>& ptr) { This_ = ptr; }
		//�������w��2�d�|�C���^��ݒ�
		void SetThisPtr(Pointer<Object>&& ptr) { This_ = ptr; }

		//�����_�[�N���X�Ɏ��g��o�^
		void PushModelList();
		//�����_�[�N���X���玩�g�����O
		void EreseModelList();
		//�����_�[�N���X���玩�g�����O���郊�N�G�X�g�𑗂�
		void SetDeleteFlag();

		void SetActive(bool active) { (active) ? AddFlag(OBJECT_FLAGS::Active) : RemoveFlag(OBJECT_FLAGS::Active); }
		bool IsActive()		const { return isFlag(OBJECT_FLAGS::Active); }

		void   SetPos(const Vector& pos) { RenderObject_.Pos_ = pos; }
		void   SetPos(Vector&& pos) { RenderObject_.Pos_ = pos; }
		Vector GetPos()		const { return RenderObject_.Pos_; }

		void   SetRot(const Vector& rot) { RenderObject_.Rot_ = rot; }
		void   SetRot(Vector&& rot) { RenderObject_.Rot_ = rot; }
		Vector GetRot()		const { return RenderObject_.Rot_; }

		void   SetScale(const Vector& scale) { RenderObject_.Scale_ = scale; }
		void   SetScale(Vector&& scale) { RenderObject_.Scale_ = scale; }
		Vector GetScale()	const { return RenderObject_.Scale_; }

		void SetTag(OBJECT_TAGS tag) { Tag_ |= 0x01ULL << tag; }
		void SetTags(const std::initializer_list<OBJECT_TAGS>& args) { for (OBJECT_TAGS tag : args)Tag_ |= 0x01ULL << tag; }
		void SetTags(std::initializer_list<OBJECT_TAGS>&& args)		 { for (OBJECT_TAGS tag : args)Tag_ |= 0x01ULL << tag; }

		void RemoveTag(OBJECT_TAGS tag) { Tag_ &= ~(0x01ULL << tag); }
		TAG_TYPE GetTag()	const { return Tag_; }
		bool ObjectHasTag(OBJECT_TAGS tag)	const { return (Tag_ & (0x01ULL << tag)); }

		//RenderObject�̃^�O�ݒ�(�`�悷�邩��A�e�A�폜���N�G�X�g(�}���`�X���b�h�p)�Ȃ�)
		void SetRenderTag(RenderTags tag) { RenderObject_.SetTag(tag); }
		//RenderObject�̃^�O�ݒ�(�`�悷�邩��A�e�A�폜���N�G�X�g(�}���`�X���b�h�p)�Ȃ�)
		void SetRenderTags(const std::initializer_list<RenderTags>& tags)	{ RenderObject_.SetTags(tags); }
		//RenderObject�̃^�O�ݒ�(�`�悷�邩��A�e�A�폜���N�G�X�g(�}���`�X���b�h�p)�Ȃ�)
		void SetRenderTags(std::initializer_list<RenderTags>&& tags)		{ RenderObject_.SetTags(tags); }
		
		//RenderObject�̃^�O�ݒ�(�`�悷�邩��A�e�A�폜���N�G�X�g(�}���`�X���b�h�p)�Ȃ�)
		void RemoveRenderTag(RenderTags tag) { RenderObject_.RemoveTag(tag); }
		bool RenderHasTag(RenderTags tag) const { return RenderObject_.HasTag(tag); }

		void AddFlag(OBJECT_FLAGS ID) { Flags_ |= (0x01ULL << ID); }
		void RemoveFlag(OBJECT_FLAGS ID) { Flags_ &= ~(0x01ULL << ID); }
		bool isFlag(OBJECT_FLAGS ID) const { return (Flags_ & (0x01ULL << ID)) != 0; }
		u16 GetFlags() const { return Flags_; }
		void SetFlags(u16 state) { Flags_ = state; }

		u64 GetState() const { return State_; }
	protected:
		//thisptr���i�[���Ă���2�d�|�C���^
		Pointer<Object> This_;
		//�`��֘A(���W���Ȃǂ��i�[)
		RenderObject3D RenderObject_;
		u64_char State_;
		TAG_TYPE Tag_;
		u32 Flags_;
	};
}
/*
	Object �������C���[�W

	Pos_	24byte
	Tag_	8byte
	Handle_ 4byte	Active_ 1byte	buffer 3byte
	vfptr	8byte
*/
using namespace MyLib;

namespace Size {
	constexpr size_t OBJECT = sizeof(MyLib::Object);
}