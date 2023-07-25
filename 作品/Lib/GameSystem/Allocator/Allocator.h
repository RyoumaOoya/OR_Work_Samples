#pragma once
#include <memory>
#include <stdarg.h>
#include "../Thread/CriticalSection.h"
#include "Pointer.h"
#include "../../Literal.h"

#if _DEBUG
//メモリリークを検知するか
#define MOMORY_LEAK_DETECT 1
#endif
#define _DX_LIB_INC
//#define REFERENCE_COUNTER

#ifdef _DX_LIB_INC
#include "DxLib.h"
#endif // _DX_LIB_INC

#define Tags(...) TagResult({__VA_ARGS__})
#if MOMORY_LEAK_DETECT
/*
	メモリ割り当て
	引数: class_name クラス型, type 配置場所, tag メモリのタグ, ... class_nameのコンストラクタ引数
	戻り値: 確保したメモリのポインタ
*/
#define MyNew(class_name, type, tag, ...) MyLib::Allocator::Allocate<class_name>(type, tag, __FILE__, __LINE__, __VA_ARGS__)
#else
/*
	メモリ割り当て
	引数: class_name クラス型, type 配置場所, tag メモリのタグ, ... class_nameのコンストラクタ引数
	戻り値: 確保したメモリのポインタ
*/
#define MyNew(class_name, type, tag, ...) MyLib::Allocator::Allocate<class_name>(type, tag, __VA_ARGS__)
#endif // MOMORY_LEAK_DETECT
/*
	メモリの解放(自動的にnull代入される)
*/
#define MyFree(ptr) MyLib::Allocator::Delete(ptr)

namespace MyLib
{
	enum class AllocType :i64 {
		Reference = -2,
		NullPtr = -1,

		Library,
		Prototype,
		Manager,
		Sound,
		UI,
		SceneData,
		Camera,
		Stage,
		Player,
		Bullet,
		Enemy,
		Item,
		Collision,
		Produce,

		Num
	};

#define IAllocType (int)AllocType

	enum class MemTag {
		Library,
		Prototype,
		UI,
		MainScene,
		SubScene,
		Stage,
		Enemy,
		Bullet,
		Player,
		Collision,
		Produce,
		StaticObject,
		BackGround,
		Sound,

		Num
	};

#define IMemTag (int)MemTag

	//インライン化しないとエラーになる
	constexpr inline int TagResult(std::initializer_list<MemTag> args) {
		int result = 0;
		for (const MemTag tag : args) {
			result |= 1 << (int)tag;
		}
		return result;
	}

	class Allocator
	{
	public:
		Allocator() = delete;
		~Allocator() = delete;
	private:
		//管理用ポインタ構造体
		struct ForManagementPointer
		{
#ifdef REFERENCE_COUNTER
			ForManagementPointer() : pointer(nullptr), size(0), tag(0), use(0), destructor(nullptr) { }
			ForManagementPointer(size_t s, AllocType Type, int Tag, void(*d)(void*)) : pointer(nullptr), size(s), tag(Tag), use(0), destructor(d) { /*使用中フラグを立てる*/tag |= TAG_SIGN_BIT; }
#else
			ForManagementPointer() : pointer(nullptr), size(0), tag(0), destructor(nullptr) { }
			ForManagementPointer(size_t s, AllocType Type, int Tag, void(*d)(void*)) : pointer(nullptr), size(s), tag(Tag), destructor(d) { /*使用中フラグを立てる*/tag |= TAG_SIGN_BIT; }
#endif
			~ForManagementPointer();

			auto operator<=>(const ForManagementPointer& b) const;
			auto operator<=>(const void* b) const;

			int tag;
#ifdef REFERENCE_COUNTER
			int use;
#endif // REFERENCE_COUNTER
			void(*destructor)(void*);
			char* pointer;				//ポインタの実態
			size_t size;
#ifndef _WIN64
			char buffer[4] = { 0 };
#endif
#if MOMORY_LEAK_DETECT
			int lineNumber = 0;
			char fileName[256] = { 0 };
#endif // MOMORY_LEAK_DETECT
		};
		static constexpr size_t FOR_MANAGEMENT_POINTER_SIZE = sizeof(ForManagementPointer);

		struct FreeSpaceData {
			FreeSpaceData() :ptr(nullptr), size(0), use(false) {}
			FreeSpaceData(bool use) :ptr(nullptr), size(0), use(use) {}
			~FreeSpaceData();

			auto operator<=>(const FreeSpaceData& data) const;

			char* ptr;
			size_t size;

			bool use;

		private:
			char buffer[7] = { 0 };
		};
		static constexpr size_t FREE_SPACE_DATA_SIZE = sizeof(FreeSpaceData);

		struct ManagementPointerList
		{
			ManagementPointerList() :fastPointer(nullptr), nextPointer(nullptr), endPointer(nullptr) {}
			~ManagementPointerList() = default;
			char* fastPointer;
			char* nextPointer;
			char* endPointer;
		private:
			char buffer[4] = { 0 };
		public:
			std::list<ForManagementPointer*> list;
			std::list<FreeSpaceData*> freeSpaceList;
			//size_t max_size;
		};
		static constexpr size_t MANAGEMENT_POINTER_LIST_SIZE = sizeof(ManagementPointerList);
	public:
		static void Init();
		static void Load();
		static void Draw();
		static void GarbageCollection();
		static void Reset();
		static void Fin();
#if MOMORY_LEAK_DETECT
		template<class T, class... Args>
		static Pointer<T> Allocate(AllocType type, int tag, const char* fileName, int lineNumber, const Args&... args)
#else
		template<class T, class... Args>
		static Pointer<T> Allocate(AllocType type, int tag, const Args&... args)
#endif // MOMORY_LEAK_DETECT
		{
			const i64 i_type = static_cast<i64>(type);
			CriticalSection& cs = ListCS[i_type];
			ManagementPointerList& temp = PointerList[i_type];
			constexpr size_t size = sizeof(T);
			char* alloc_ptr = nullptr;
			/*配置する場所の設定*/ {
				ScopedCS CS(&cs);
				//断片化してる領域に入れられるかを判定
				for (auto itr = temp.freeSpaceList.begin(); itr != temp.freeSpaceList.end(); ++itr) {
					auto free_space = *itr;
					//確保予定のサイズと断片化している領域の比較
					if (size > free_space->size)
						continue;
					//断片化領域から切り出して配置する場所を決定する
					alloc_ptr = free_space->ptr;
					if (size != free_space->size) {
						//断片化領域の再設定
						auto free_space_data = FreeSpaceDataAllocate();
						free_space_data->ptr = free_space->ptr + size;
						free_space_data->size = free_space->size - size;
						free_space->~FreeSpaceData();
						temp.freeSpaceList.erase(itr);
						temp.freeSpaceList.push_back(free_space_data);
					}
					else {
						free_space->~FreeSpaceData();
						temp.freeSpaceList.erase(itr);
					}
					break;
				}
				//断片化領域になかった場合一番後ろに配置
				if (!alloc_ptr) {
					//空き容量ある?
					if (temp.nextPointer + size > temp.endPointer)
						throw;	//ここで例外が出されるなら確保済みのメモリが足りない
					alloc_ptr = temp.nextPointer;
				}
				if (alloc_ptr == temp.nextPointer)
					temp.nextPointer += size;
			}
			//2重ポインタのポインタ側の実態を作る
			ForManagementPointer* ptr = ManagementPointerAllocate(size, type, tag, [](void* p) { ((T*)p)->~T(); p = nullptr; });
#if MOMORY_LEAK_DETECT	//メモリリーク検知用
			strcpy_s(ptr->fileName, sizeof(ForManagementPointer::fileName), fileName);
			ptr->lineNumber = lineNumber;
#endif // MOMORY_LEAK_DETECT

#ifdef REFERENCE_COUNTER
			Pointer<T> result((T**)&ptr->pointer, type, &ptr->use);
#else
			Pointer<T> result((T**)&ptr->pointer, i_type);
#endif // REFERENCE_COUNTER

			//管理用変数をリストに格納
			temp.list.push_back(ptr);

			//実態を作る
			ptr->pointer = reinterpret_cast<char*>(new(alloc_ptr) T(args...));

			return result;
		}
		template<class T>
		static _inline void Delete(Pointer<T>& p)
		{
			auto& t_list = PointerList[p.type];
			CriticalSection& cs = ListCS[p.type];
			//t_listの管理外ポインタ?
			if (t_list.endPointer < reinterpret_cast<char*>(p.get()) || t_list.fastPointer > reinterpret_cast<char*>(p.get()))
				throw;
			//pを検索
			{
				ScopedCS l_cs(&cs);
				for (auto t_itr = t_list.list.begin(); t_itr != t_list.list.end(); ++t_itr) {
					if (reinterpret_cast<void**>(&(*t_itr)->pointer) != static_cast<void**>(p)) continue;

					auto temp = *t_itr;
					if (t_list.nextPointer == temp->pointer + temp->size) {
						t_list.nextPointer = temp->pointer;
						FreeSpaceDataNormalize(p.type);
					}
					else {
						FreeSpaceData* free_space_data = FreeSpaceDataAllocate();
						free_space_data->ptr = temp->pointer;
						free_space_data->size = temp->size;
						t_list.freeSpaceList.push_back(free_space_data);
						FreeSpaceDataNormalize(p.type);
					}
					//デストラクタ内でTのデストラクタを呼び出している(アロケート時にデストラクタを保存しているためTが基底クラスポインタであっても確実に開放される)
					temp->~ForManagementPointer();

					t_list.list.erase(t_itr);
					break;
				}
			}
			p.p = nullptr;
#ifdef REFERENCE_COUNTER
			p.use_num = nullptr;
#endif // REFERENCE_COUNTER
		}
		/*ポインタの格納場所を取得する
		処理が重いのでできるだけ使用しない*/
		template<class T>
		static Pointer<T> GetPointerStorage(T* ptr, AllocType type) {
			i64 t_type = static_cast<i64>(type);
			auto& t_list = PointerList[t_type];

			for (auto temp : t_list.list) {
				if ((T*)temp->pointer == ptr) {
#ifdef REFERENCE_COUNTER
					return Pointer<T>(reinterpret_cast<T**>(&temp->pointer), t_type, &temp->use);
#else
					return Pointer<T>(reinterpret_cast<T**>(&temp->pointer), t_type);
#endif // REFERENCE_COUNTER
				}
			}

			return  Pointer<T>();
		}

		//typeの種類のインスタンスを全て破棄する(list等でポインタを参照している場合、nullptrになるため注意)
		static void Clear(AllocType type);
		//指定のTagがついたインスタンスを全て破棄する(list等でポインタを参照している場合、nullptrになるため注意)
		static void Clear(MemTag type);

	private:
		//内部用
		static void Delete(std::list<ForManagementPointer*>::iterator itr, AllocType type);
		//管理用ポインタを確保する
		static ForManagementPointer* ManagementPointerAllocate(const ForManagementPointer& p);
		//管理用ポインタを確保する
		static ForManagementPointer* ManagementPointerAllocate(size_t s, AllocType Type, int Tag, void(*d)(void*));
		//断片化情報格納用ポインタを確保する
		static FreeSpaceData* FreeSpaceDataAllocate();
		//断片化情報を正規化する
		static void FreeSpaceDataNormalize();
		//断片化情報を正規化する(種類指定)
		static void FreeSpaceDataNormalize(__int64 type);

	public:
		/*	管理用変数:タグの符号ビット
			コンパイルエラー対策にメンバで定義	*/
		static constexpr int TAG_SIGN_BIT = 1 << ((sizeof(ForManagementPointer::tag) * 8) - 1);
	private:
		template<class T>
		struct ManagerData {
		public:
			ManagerData() :arr(nullptr), next(nullptr), end(nullptr) {}
			T* arr;
			T* next;
			T* end;
			CriticalSection cs;
		};
		static inline handle CircleHandle = -1;
		static inline CriticalSection ListCS[IAllocType::Num];
		//pointerList_が使うメモリ
		static inline char* Pool = nullptr;
		static inline ManagementPointerList* PointerList = nullptr;
		//管理用変用メモリ空間
		static inline ManagerData<ForManagementPointer> PointerData;
	public:
		static inline ManagerData<FreeSpaceData> FSData;
	};
}