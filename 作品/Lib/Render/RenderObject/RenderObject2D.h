#pragma once
#include "../../typedef.h"
#include "../../Math/MyMathData.h"
#include "../BlendData/BlendData.h"
#include "RendorParameter.h"
#include <compare>
#include <functional>
#include "../../Constant/Constant.h"
#include "../../GameSystem/Thread/Thread.h"

namespace MyLib
{
	////2D�`��p�N���X
	class RenderObject2D
	{
	public:
		//�������݃��b�N�̎擾
		class WritingLock {
		public:
			WritingLock(RenderObject2D* p) :ptr(p) { ptr->WaitReading(); ptr->SetWritingFlag(true); }
			~WritingLock() { ptr->SetWritingFlag(false); }

			RenderObject2D* ptr;
		};
		//�ǂݎ�胍�b�N�̎擾
		class ReadingLock {
		public:
			ReadingLock(RenderObject2D* p) :ptr(p) { ptr->WaitWriting(); ptr->GetReading(); }
			~ReadingLock() { ptr->ReleaseReading(); }

			RenderObject2D* ptr;
		};
	public:
		RenderObject2D();
		RenderObject2D(const RenderObject2D& other);
		~RenderObject2D();

		void Load(filepath path);
		void Load(const RenderObject2D& b);
		//�񓯊��ǂݍ��ݗp�֐�
		//�n���h�����ǂݍ��܂��܂őҋ@����
		void WaitLoad() { WaitHandleASyncLoad(Handle_); }
		virtual void Draw();
		virtual void Delete();

		//�`�悷�邩�̐ݒ�
		void SetDrawFlg(bool flg) { (flg) ? SetTag(RenderTags::isDraw) : RemoveTag(RenderTags::isDraw); }
		//�`�悳��Ă��邩
		bool isDraw() const { return HasTag(RenderTags::isDraw); }

		//�^�O�����݂��邩
		_inline bool HasTag(RenderTags tag) const { return (Tags_ & (1 << tag)) != 0; }
		//�^�O��ݒ肷��
		_inline void SetTag(RenderTags tag) { Tags_ |= (1 << tag); }
		//�^�O��ݒ肷��(����)
		_inline void SetTags(const std::initializer_list<RenderTags>& tags) { for (auto tag : tags) Tags_ |= (1 << tag); }
		//�^�O��ݒ肷��(����)
		_inline void SetTags(std::initializer_list<RenderTags>&& tags)		{ for (auto tag : tags) Tags_ |= (1 << tag); }
		//�^�O�����O����
		_inline void RemoveTag(RenderTags tag) { Tags_ &= ~(1ULL << tag); }
		
		//�摜�T�C�Y���擾
		iVector2D GetGraphSize() const { 
			iVector2D result;
			::GetGraphSize(Handle_, &result.x, &result.y);
			return result; }

		auto operator<=>(const RenderObject2D& b) const {
			if (Index_ != b.Index_) {
				return Index_ <=> b.Index_;
			}
			return std::strong_ordering::equivalent;
		}

		//�`�揇���擾
		i8 GetIndex() const { return Index_; }
		//�`�揇��ݒ�
		void SetIndex(i8 i) { Index_ = i; }

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
		atomic<u64> Tags_;
		atomic<u32> ReadingNum_;
	public:
		handle Handle_;
		Vector2D Pos_;
		float ExRate_;
		float Angle_;
		BlendData BlendData_;
	protected:
		i8 Index_;
	};

	/*
		This: �R�s�[����ϐ�(�}�l�[�W���[�̃����o���������X�g�ɑ���ꍇ�}�l�[�W���[�̃C���X�^���X���K�v)
		r_obj:�ݒ肷��I�u�W�F�N�g, str:�`�悷�镶����, ...:�������X�g
	*/
#define SET_R_OBJ_STR_FUNC(This, r_obj, str, ...)\
r_obj.SetFormatDraw(str, [This](const RenderObjectString& obj){ DrawExtendFormatString(obj.Pos_.x, obj.Pos_.y, obj.ExRate_.x, obj.ExRate_.y, obj.Color_.load(), obj.Str_.c_str(), __VA_ARGS__); })

	//�����`��p�N���X
	class RenderObjectString
	{
	public:
		//�������݃��b�N�̎擾
		class WritingLock {
		public:
			WritingLock(RenderObjectString* p) :ptr(p) { ptr->WaitReading(); ptr->SetWritingFlag(true); }
			~WritingLock() { ptr->SetWritingFlag(false); }

			RenderObjectString* ptr;
		};
		//�ǂݎ�胍�b�N�̎擾
		class ReadingLock {
		public:
			ReadingLock(RenderObjectString* p) :ptr(p) { ptr->WaitWriting(); ptr->GetReading(); }
			~ReadingLock() { ptr->ReleaseReading(); }

			RenderObjectString* ptr;
		};
	public:
		static void DefaultDrawFunc(const RenderObjectString& obj) { DrawExtendNString(obj.Pos_.x, obj.Pos_.y, obj.ExRate_.x, obj.ExRate_.y, obj.Str_.c_str(), obj.Str_.size(), obj.Color_); }
	public:
		RenderObjectString() :ReadingNum_(0), Tags_(0), ExRate_(1.0, 1.0), Func_(&DefaultDrawFunc) { SetTags({ RenderTags::isDraw }); }
		~RenderObjectString() {}
		void Draw() const { Func_(*this); }

		//���������킩��Ȃ��ꍇ SET_R_OBJ_STR_FUNC define���g�p���邱��
		void SetFormatDraw(const std::string& str, const std::function<void(const RenderObjectString&)>& func) { WritingLock l(this); Str_ = str; Func_ = func; }

		//�`�敶�����ݒ�
		void SetString(const std::string& str) { WritingLock l(this); Str_ = str; }
		//�`�敶�����ݒ�
		void SetString(std::string&& str) { WritingLock l(this); Str_ = str; }

		//�`�悷�邩�̐ݒ�
		void SetDrawFlag(bool flg) { (flg) ? SetTag(RenderTags::isDraw) : RemoveTag(RenderTags::isDraw); }
		//�`�悳��Ă��邩
		bool isDraw() const { return HasTag(RenderTags::isDraw); }

		//�^�O�����݂��邩
		bool HasTag(RenderTags tag) const { return (Tags_ & (1 << tag)) != 0; }
		//�^�O��ݒ肷��
		void SetTag(RenderTags tag) { Tags_ |= (1 << tag); }
		//�^�O��ݒ肷��(����)
		void SetTags(const std::initializer_list<RenderTags>& tags) { for (auto tag : tags) Tags_ |= (1 << tag); }
		//�^�O��ݒ肷��(����)
		void SetTags(std::initializer_list<RenderTags>&& tags)		{ for (auto tag : tags) Tags_ |= (1 << tag); }
		//�^�O�����O����
		void RemoveTag(RenderTags tag) { Tags_ &= ~(1ULL << tag); }

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
		atomic<u64> Tags_;
		atomic<u32> ReadingNum_;
	public:
		iVector2D Pos_;
		dVector2D ExRate_;
		Color Color_;
	public:
		std::string Str_;
		std::function<void(const RenderObjectString&)> Func_;
	};
}

namespace Size
{
	constexpr size_t R_OBJ_2D = sizeof(MyLib::RenderObject2D);
	constexpr size_t R_OBJ_STR = sizeof(MyLib::RenderObjectString);
}