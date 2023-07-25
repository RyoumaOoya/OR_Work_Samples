#pragma once
#include "Define.h"
#include "GameSystem/Allocator/Allocator.h"
#include <compare>

//マネージャー用フレンド宣言
#define MANAGER \
friend class Allocator;\
template<class T>\
friend class SingletonBase

#define PTR_LIST(type) std::list<Pointer<type>>
#define PTR_VECTOR(type) std::vector<Pointer<type>>

namespace MyLib
{
	enum class MANAGER_INDEX :u64 {
		MultiThread,		//マルチスレッドで実行するよう
		SequentialProcess,	//逐次処理(順不同)
		CollisionStep,
		CameraStep,
		Last				//逐次処理(最後)
	};

#define IMANAGER_INDEX (int)MyLib::MANAGER_INDEX

	/*
		マネージャーの基底クラス
		リストでの管理用
	*/
	class ManagerBase
	{
	public:
		ManagerBase() :Index_(MANAGER_INDEX::MultiThread) {}
		virtual ~ManagerBase() = default;
	public:
		virtual void Init() = 0;
		virtual void Load() = 0;
		virtual void OnLoadEnd() {}		//ロード終了後のオブジェクト生成などを行う処理
		virtual void Step() = 0;
		virtual void DeleteStep() {}	//実行中の定められていないタイミングに破棄処理を行う場合の関数
		virtual void ConflictStep() = 0;
		virtual void UpDate() = 0;
		virtual void FrontDraw() = 0;
		virtual void Delete() = 0;
		virtual void Fin() = 0;

		auto operator<=>(const ManagerBase& b) {
			if (Index_ != b.Index_) {
				return Index_ <=> b.Index_;
			}
			return std::strong_ordering::equivalent;
		}
	public:
		MANAGER_INDEX Index_;
	};

	/*
		シングルトンを継承するマネージャークラス
	*/
	template<class T>
	class SingletonBase :public ManagerBase
	{
	public:
		SingletonBase() = default;
		virtual ~SingletonBase() override = default;
	public:
		static Pointer<T> CreateInstance() { return (!Instance_) ? Instance_ = MyNew(T, AllocType::Manager, 0) : Instance_; }
		static Pointer<T> GetInstance() { return (Instance_) ? Instance_ : CreateInstance(); }
		static void DeleteInstance() { if (Instance_) MyFree(Instance_); }
	private:
		static inline Pointer<T> Instance_;
	};
}

using namespace MyLib;

namespace Size
{
	constexpr size_t MANAGER_BASE = sizeof(MyLib::ManagerBase);
}