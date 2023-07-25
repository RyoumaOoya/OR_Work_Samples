#pragma once
#include <initializer_list>
#include <filesystem>
#include "RendorParameter.h"
#include "../../Constant/Color.h"
#include "../../typedef.h"
#include "../../Math/MyMathData.h"
#include "../../GameSystem/Thread/Thread.h"

namespace MyLib
{
	//3D�`��p���N���X
	class RenderObject3DBase
	{
	public:
		//�������݃��b�N���擾
		class WritingLock {
		public:
			WritingLock(RenderObject3DBase* p) :ptr(p) { ptr->WaitReading(); ptr->SetWritingFlag(true); }
			~WritingLock() { ptr->SetWritingFlag(false); }

			RenderObject3DBase* ptr;
		};
		//�ǂݎ�胍�b�N�̎擾
		class ReadingLock {
		public:
			ReadingLock(RenderObject3DBase* p) :ptr(p) { ptr->WaitWriting(); ptr->GetReading(); }
			~ReadingLock() { ptr->ReleaseReading(); }

			RenderObject3DBase* ptr;
		};
	public:
		RenderObject3DBase();
		virtual ~RenderObject3DBase();

		virtual void Load(const char* path) = 0;
		//�n���h���̃��[�h���I���܂ő҂�
		void WaitLoad() { WaitHandleASyncLoad(Handle_); }
		virtual void Draw() = 0;
		virtual void Delete() = 0;

		//�^�O�����݂��Ă��邩
		bool HasTag(RenderTags tag) const { return Tags & (1 << tag); }
		//�^�O�ݒ肷��
		void SetTag(RenderTags tag) { Tags |= (1 << tag); }

		//�^�O�ݒ肷��(����)
		void SetTags(const std::initializer_list<RenderTags>& tags) { for (auto tag : tags) Tags |= (1 << tag); }
		//�^�O�ݒ肷��(����)
		void SetTags(std::initializer_list<RenderTags>&& tags)		{ for (auto tag : tags) Tags |= (1 << tag); }

		//�^�O�����O����
		_inline void RemoveTag(RenderTags tag) { Tags &= ~(1ULL << tag); }

		//�������݂��I������̂�҂�
		void WaitWriting() const { WaitConditionSatisfy([this]() {return isWriting(); }); }
		//�ǂݎ�肪�I������̂�҂�
		void WaitReading() const { WaitConditionSatisfy([this]() {return isReading(); }); }
		
		//�������ݒ���
		bool isWriting() const { return HasTag(RenderTags::isWriting); }
		//�ǂݎ�蒆��
		bool isReading() const { return ReadingNum_ != 0; }

		//�ǂݎ����J�n����t���O�𗧂Ă�
		void GetReading() { ++ReadingNum_; }
		//�ǂݎ����I������
		void ReleaseReading() { --ReadingNum_; }
	protected:
		//�������݃t���O��ݒ肷��
		void SetWritingFlag(bool flg) { (flg) ? SetTag(RenderTags::isWriting) : RemoveTag(RenderTags::isWriting); }
	private:
		atomic<u64> Tags;
		atomic<u32> ReadingNum_;
	public:
		handle Handle_;
		Vector Pos_;
	};

	//���f���`��p�N���X
	class RenderObject3D : public RenderObject3DBase
	{
	public:
		RenderObject3D();
		RenderObject3D(const RenderObject3D& other);
		~RenderObject3D()override;

	public:
		void Load(const char* path) override;
		void Load(handle handle);
		void UpDate();
		void Draw() override;
		void Delete() override;

		//�f�B�t���[�Y�J���[��ݒ�
		void SetColor(Color color, int MaterialIndex = 0)	{ MV1SetMaterialDifColor(Handle_, MaterialIndex, color); }
		//�f�B�t���[�Y�J���[���擾
		auto GetColor(int MaterialIndex = 0) const			{ return MV1GetMaterialDifColor(Handle_, MaterialIndex); }

		//�X�y�L�����[�J���[��ݒ�
		void SetSpcColor(COLOR_F color, int MaterialIndex = 0){ MV1SetMaterialSpcColor(Handle_, MaterialIndex, color); }
		//�X�y�L�����[�J���[���擾
		auto GetSpcColor(int MaterialIndex = 0) const		{ return MV1GetMaterialSpcColor(Handle_, MaterialIndex); }

		//�G�~�b�V�u�J���[��ݒ�
		void SetEmiColor(COLOR_F color, int MaterialIndex = 0){ MV1SetMaterialEmiColor(Handle_, MaterialIndex, color); }
		//�G�~�b�V�u�J���[���擾
		auto GetEmiColor(int MaterialIndex = 0) const		{ return MV1GetMaterialEmiColor(Handle_, MaterialIndex); }

		//�A���r�G���g�J���[��ݒ�
		void SetAmbColor(COLOR_F color, int MaterialIndex = 0){ MV1SetMaterialAmbColor(Handle_, MaterialIndex, color); }
		//�A���r�G���g�J���[���擾
		auto GetAmbColor(int MaterialIndex = 0) const		{ return MV1GetMaterialAmbColor(Handle_, MaterialIndex); }
	public:
		Vector Rot_;
		Vector Scale_;
	};
	
	//3D�ł̉摜�`��p�N���X
	class RenderObjectBillboard :public RenderObject3DBase
	{
	public:
		RenderObjectBillboard();
		~RenderObjectBillboard() override;
	public:
		void Load(const char* path) override;
		void Draw() override;
		void Delete() override;
	public:
		float ExRate_;
		float Angle_;
		bool ReverseX_ : 1;
		bool ReverseY_ : 1;
	};
}

namespace Size
{
	constexpr size_t R_OBJ_3D = sizeof(MyLib::RenderObject3D);
	constexpr size_t R_OBJ_BILLBOARD = sizeof(MyLib::RenderObjectBillboard);
}