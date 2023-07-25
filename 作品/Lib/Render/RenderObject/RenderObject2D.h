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
	////2D描画用クラス
	class RenderObject2D
	{
	public:
		//書き込みロックの取得
		class WritingLock {
		public:
			WritingLock(RenderObject2D* p) :ptr(p) { ptr->WaitReading(); ptr->SetWritingFlag(true); }
			~WritingLock() { ptr->SetWritingFlag(false); }

			RenderObject2D* ptr;
		};
		//読み取りロックの取得
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
		//非同期読み込み用関数
		//ハンドルが読み込まれるまで待機する
		void WaitLoad() { WaitHandleASyncLoad(Handle_); }
		virtual void Draw();
		virtual void Delete();

		//描画するかの設定
		void SetDrawFlg(bool flg) { (flg) ? SetTag(RenderTags::isDraw) : RemoveTag(RenderTags::isDraw); }
		//描画されているか
		bool isDraw() const { return HasTag(RenderTags::isDraw); }

		//タグが存在するか
		_inline bool HasTag(RenderTags tag) const { return (Tags_ & (1 << tag)) != 0; }
		//タグを設定する
		_inline void SetTag(RenderTags tag) { Tags_ |= (1 << tag); }
		//タグを設定する(複数)
		_inline void SetTags(const std::initializer_list<RenderTags>& tags) { for (auto tag : tags) Tags_ |= (1 << tag); }
		//タグを設定する(複数)
		_inline void SetTags(std::initializer_list<RenderTags>&& tags)		{ for (auto tag : tags) Tags_ |= (1 << tag); }
		//タグを除外する
		_inline void RemoveTag(RenderTags tag) { Tags_ &= ~(1ULL << tag); }
		
		//画像サイズを取得
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

		//描画順を取得
		i8 GetIndex() const { return Index_; }
		//描画順を設定
		void SetIndex(i8 i) { Index_ = i; }

		//書き込みが終了するのを待つ
		void WaitWriting() const { WaitConditionSatisfy([this]() {return isWriting(); }); }
		//読み取りが終了するのを待つ
		void WaitReading() const { WaitConditionSatisfy([this]() {return isReading(); }); }

		//書き込み中か
		bool isWriting() const { return HasTag(RenderTags::isWriting); }
		//読み取り中か
		bool isReading() const { return ReadingNum_ != 0; }

		//読み取りを開始するフラグを立てる
		void GetReading() { ++ReadingNum_; }
		//読み取りを終了する
		void ReleaseReading() { --ReadingNum_; }
	protected:
		//書き込みフラグを設定する
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
		This: コピーする変数(マネージャーのメンバを引数リストに送る場合マネージャーのインスタンスが必要)
		r_obj:設定するオブジェクト, str:描画する文字列, ...:引数リスト
	*/
#define SET_R_OBJ_STR_FUNC(This, r_obj, str, ...)\
r_obj.SetFormatDraw(str, [This](const RenderObjectString& obj){ DrawExtendFormatString(obj.Pos_.x, obj.Pos_.y, obj.ExRate_.x, obj.ExRate_.y, obj.Color_.load(), obj.Str_.c_str(), __VA_ARGS__); })

	//文字描画用クラス
	class RenderObjectString
	{
	public:
		//書き込みロックの取得
		class WritingLock {
		public:
			WritingLock(RenderObjectString* p) :ptr(p) { ptr->WaitReading(); ptr->SetWritingFlag(true); }
			~WritingLock() { ptr->SetWritingFlag(false); }

			RenderObjectString* ptr;
		};
		//読み取りロックの取得
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

		//書き方がわからない場合 SET_R_OBJ_STR_FUNC defineを使用すること
		void SetFormatDraw(const std::string& str, const std::function<void(const RenderObjectString&)>& func) { WritingLock l(this); Str_ = str; Func_ = func; }

		//描画文字列を設定
		void SetString(const std::string& str) { WritingLock l(this); Str_ = str; }
		//描画文字列を設定
		void SetString(std::string&& str) { WritingLock l(this); Str_ = str; }

		//描画するかの設定
		void SetDrawFlag(bool flg) { (flg) ? SetTag(RenderTags::isDraw) : RemoveTag(RenderTags::isDraw); }
		//描画されているか
		bool isDraw() const { return HasTag(RenderTags::isDraw); }

		//タグが存在するか
		bool HasTag(RenderTags tag) const { return (Tags_ & (1 << tag)) != 0; }
		//タグを設定する
		void SetTag(RenderTags tag) { Tags_ |= (1 << tag); }
		//タグを設定する(複数)
		void SetTags(const std::initializer_list<RenderTags>& tags) { for (auto tag : tags) Tags_ |= (1 << tag); }
		//タグを設定する(複数)
		void SetTags(std::initializer_list<RenderTags>&& tags)		{ for (auto tag : tags) Tags_ |= (1 << tag); }
		//タグを除外する
		void RemoveTag(RenderTags tag) { Tags_ &= ~(1ULL << tag); }

		//書き込みが終了するのを待つ
		void WaitWriting() const { WaitConditionSatisfy([this]() {return isWriting(); }); }
		//読み取りが終了するのを待つ
		void WaitReading() const { WaitConditionSatisfy([this]() {return isReading(); }); }

		//書き込み中か
		bool isWriting() const { return HasTag(RenderTags::isWriting); }
		//読み取り中か
		bool isReading() const { return ReadingNum_ != 0; }

		//読み取りを開始するフラグを立てる
		void GetReading() { ++ReadingNum_; }
		//読み取りを終了する
		void ReleaseReading() { --ReadingNum_; }
	protected:
		//書き込みフラグを設定する
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