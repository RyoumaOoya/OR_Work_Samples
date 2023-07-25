#pragma once
#include <list>
#include "GameSystem/Allocator/Pointer.h"
#include "RenderObject/RenderObject2D.h"
#include "RenderObject/RenderObject3D.h"
#include <condition_variable>

#define is3D true
namespace MyLib
{
	//レンダリング管理クラス
	class Render
	{
	public:
		//描画対象スクリーンを設定する
		class SetDrawScreen {
		public:
			SetDrawScreen(int scrrenHandle) { OidScreen = GetDrawScreen(); DxLib::SetDrawScreen(scrrenHandle); }
			~SetDrawScreen() { DxLib::SetDrawScreen(OidScreen); }
		private:
			int OidScreen;
		};
	public:
		Render() = delete;
		~Render() = delete;

		static void Init();
		static void Draw();
		static void Fin();

#if is3D
		//近距離用シャドウマップの中心点を決める
		static void SetNearShadowDrawArea(const std::function<Vector()>& func) { SetNearShadowAreaFunc = func; }
		//近距離用シャドウマップの中心点を決める
		static void SetNearShadowDrawArea(std::function<Vector()>&& func) { SetNearShadowAreaFunc = func; }
		//スタティックオブジェクトの影を書き込む
		static void WriteStaticShadow();

		//描画リストに追加する
		static void PushModelList(const OffSetPtr<RenderObject3DBase>& ptr) { std::lock_guard<std::mutex> lock(ModelMtx_); DrawModelList_.push_back(ptr); }
		//描画リストに追加する
		template<class T>
		static void PushModelList(const Pointer<T> p, const RenderObject3DBase* o) { auto ptr = OffSetPtr<RenderObject3DBase>(p, o); std::lock_guard<std::mutex> lock(ModelMtx_); DrawModelList_.push_back(ptr); }

		//描画リストから除外する
		static void EreseModelList(const OffSetPtr<RenderObject3DBase>& ptr) {
			std::lock_guard<std::mutex> lock(ModelMtx_);
			auto itr = std::find(DrawModelList_.begin(), DrawModelList_.end(), ptr);
			if (itr != DrawModelList_.end())
				DrawModelList_.erase(itr);
		}
		//描画リストから除外する
		template<class T>
		static void EreseModelList(const Pointer<T> p, const RenderObject3DBase* o) {
			auto ptr = OffSetPtr<RenderObject3DBase>(p, o);
			std::lock_guard<std::mutex> lock(ModelMtx_);
			auto itr = std::find(DrawModelList_.begin(), DrawModelList_.end(), ptr);
			if (itr != DrawModelList_.end())
				DrawModelList_.erase(itr);
		}
#endif // is3D
		//描画リストに追加する
		static void PushSpriteList(const OffSetPtr<RenderObject2D>& ptr) { std::lock_guard<std::mutex> lock(SpriteMtx_); DrawSpriteList_.push_back(ptr); }
		//描画リストに追加する
		template<class T>
		static void PushSpriteList(const Pointer<T> p, const RenderObject2D* o) { auto ptr = OffSetPtr<RenderObject2D>(p, o); { std::lock_guard<std::mutex> lock(SpriteMtx_); DrawSpriteList_.push_back(ptr); } }

		//描画リストから除外する
		static void EreseSpriteList(const OffSetPtr<RenderObject2D>& ptr) {
			std::lock_guard<std::mutex> lock(SpriteMtx_);
			auto itr = std::find(DrawSpriteList_.begin(), DrawSpriteList_.end(), ptr);
			if (itr != DrawSpriteList_.end())
				DrawSpriteList_.erase(itr);
		}
		//描画リストから除外する
		template<class T>
		static void EreseSpriteList(const Pointer<T> p, const RenderObject2D* o) {
			auto ptr = OffSetPtr<RenderObject2D>(p, o);
			std::lock_guard<std::mutex> lock(SpriteMtx_);
			auto itr = std::find(DrawSpriteList_.begin(), DrawSpriteList_.end(), ptr);
			if (itr != DrawSpriteList_.end())
				DrawSpriteList_.erase(itr);
		}

		//画像をソートする
		static void SortSpriteList() { std::lock_guard<std::mutex> lock(SpriteMtx_); DrawSpriteList_.sort([](const OffSetPtr<RenderObject2D> a, const OffSetPtr<RenderObject2D> b) {return *a < *b; }); }

		//描画リストに追加する
		static void PushStringList(const OffSetPtr<RenderObjectString>& ptr) { std::lock_guard<std::mutex> lock(StringMtx_); DrawStringList_.push_back(ptr); }
		//描画リストに追加する
		template<class T>
		static void PushStringList(const Pointer<T> p, const RenderObjectString* o) { auto ptr = OffSetPtr<RenderObjectString>(p, o); std::lock_guard<std::mutex> lock(StringMtx_); DrawStringList_.push_back(ptr); }

		//描画リストから除外する
		static void EreseStringList(const OffSetPtr<RenderObjectString>& ptr) {
			std::lock_guard<std::mutex> lock(StringMtx_);
			auto itr = std::find(DrawStringList_.begin(), DrawStringList_.end(), ptr);
			if (itr != DrawStringList_.end())
				DrawStringList_.erase(itr);
		}
		//描画リストから除外する
		template<class T>
		static void EreseStringList(const Pointer<T> p, const RenderObjectString* o) {
			auto ptr = OffSetPtr<RenderObjectString>(p, o);
			std::lock_guard<std::mutex> lock(StringMtx_);
			auto itr = std::find(DrawStringList_.begin(), DrawStringList_.end(), ptr);
			if (itr != DrawStringList_.end())
				DrawStringList_.erase(itr);
		}

		//isRendering_がtrueになるまで待機する
		static void WaitRender() { std::unique_lock<std::mutex> lock(RenderMtx_); RenderCV_.wait(lock, [] { return Rendering_; }); }
		//Renderが待機状態になるまで待機する
		static void WaitRenderStandby() { std::unique_lock<std::mutex> lock(RenderMtx_); RenderCV_.wait(lock, [] { return Waiting_; }); }
		static void SetRenderFlag(bool f) {
			std::lock_guard<std::mutex> lock(RenderMtx_);
			Rendering_ = f;
			RenderCV_.notify_one();  // 条件変数を通知
		}
	private:
		static inline bool										Rendering_ = true;//レンダリングをするかの判定(リストの削除に使用)
		static inline bool										Waiting_ = false;	//レンダリングをするかの判定(リストの削除に使用)
		static inline std::mutex								RenderMtx_;			
		static inline std::condition_variable					RenderCV_;			
		static inline std::list<OffSetPtr<RenderObject2D>>		DrawSpriteList_;	//画像リスト
		static inline std::mutex								SpriteMtx_;			//画像(DrawSpriteList_)用ミューテックス
		static inline std::list<OffSetPtr<RenderObjectString>>	DrawStringList_;	//文字列リスト
		static inline std::mutex								StringMtx_;			//文字列(DrawStringList_)用ミューテックス
#if is3D
		static inline std::list<OffSetPtr<RenderObject3DBase>>	DrawModelList_;		//モデルリスト
		static inline std::mutex								ModelMtx_;			//モデル(DrawModelList_)用ミューテックス
		static inline int										StaticShadow_ = 0;
		static inline int										NearShadow_ = 0;
		static inline std::function<Vector()>					SetNearShadowAreaFunc;
#endif // is3D
	};
}