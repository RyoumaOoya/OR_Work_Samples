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
	//3D描画用基底クラス
	class RenderObject3DBase
	{
	public:
		//書き込みロックを取得
		class WritingLock {
		public:
			WritingLock(RenderObject3DBase* p) :ptr(p) { ptr->WaitReading(); ptr->SetWritingFlag(true); }
			~WritingLock() { ptr->SetWritingFlag(false); }

			RenderObject3DBase* ptr;
		};
		//読み取りロックの取得
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
		//ハンドルのロードが終わるまで待つ
		void WaitLoad() { WaitHandleASyncLoad(Handle_); }
		virtual void Draw() = 0;
		virtual void Delete() = 0;

		//タグが存在しているか
		bool HasTag(RenderTags tag) const { return Tags & (1 << tag); }
		//タグ設定する
		void SetTag(RenderTags tag) { Tags |= (1 << tag); }

		//タグ設定する(複数)
		void SetTags(const std::initializer_list<RenderTags>& tags) { for (auto tag : tags) Tags |= (1 << tag); }
		//タグ設定する(複数)
		void SetTags(std::initializer_list<RenderTags>&& tags)		{ for (auto tag : tags) Tags |= (1 << tag); }

		//タグを除外する
		_inline void RemoveTag(RenderTags tag) { Tags &= ~(1ULL << tag); }

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
		atomic<u64> Tags;
		atomic<u32> ReadingNum_;
	public:
		handle Handle_;
		Vector Pos_;
	};

	//モデル描画用クラス
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

		//ディフューズカラーを設定
		void SetColor(Color color, int MaterialIndex = 0)	{ MV1SetMaterialDifColor(Handle_, MaterialIndex, color); }
		//ディフューズカラーを取得
		auto GetColor(int MaterialIndex = 0) const			{ return MV1GetMaterialDifColor(Handle_, MaterialIndex); }

		//スペキュラーカラーを設定
		void SetSpcColor(COLOR_F color, int MaterialIndex = 0){ MV1SetMaterialSpcColor(Handle_, MaterialIndex, color); }
		//スペキュラーカラーを取得
		auto GetSpcColor(int MaterialIndex = 0) const		{ return MV1GetMaterialSpcColor(Handle_, MaterialIndex); }

		//エミッシブカラーを設定
		void SetEmiColor(COLOR_F color, int MaterialIndex = 0){ MV1SetMaterialEmiColor(Handle_, MaterialIndex, color); }
		//エミッシブカラーを取得
		auto GetEmiColor(int MaterialIndex = 0) const		{ return MV1GetMaterialEmiColor(Handle_, MaterialIndex); }

		//アンビエントカラーを設定
		void SetAmbColor(COLOR_F color, int MaterialIndex = 0){ MV1SetMaterialAmbColor(Handle_, MaterialIndex, color); }
		//アンビエントカラーを取得
		auto GetAmbColor(int MaterialIndex = 0) const		{ return MV1GetMaterialAmbColor(Handle_, MaterialIndex); }
	public:
		Vector Rot_;
		Vector Scale_;
	};
	
	//3Dでの画像描画用クラス
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