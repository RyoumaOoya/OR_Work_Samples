#pragma once
#include <memory>
#include <stdarg.h>
#include "../Thread/CriticalSection.h"
#include "Pointer.h"
#include "../../Literal.h"

#if _DEBUG
//���������[�N�����m���邩
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
	���������蓖��
	����: class_name �N���X�^, type �z�u�ꏊ, tag �������̃^�O, ... class_name�̃R���X�g���N�^����
	�߂�l: �m�ۂ����������̃|�C���^
*/
#define MyNew(class_name, type, tag, ...) MyLib::Allocator::Allocate<class_name>(type, tag, __FILE__, __LINE__, __VA_ARGS__)
#else
/*
	���������蓖��
	����: class_name �N���X�^, type �z�u�ꏊ, tag �������̃^�O, ... class_name�̃R���X�g���N�^����
	�߂�l: �m�ۂ����������̃|�C���^
*/
#define MyNew(class_name, type, tag, ...) MyLib::Allocator::Allocate<class_name>(type, tag, __VA_ARGS__)
#endif // MOMORY_LEAK_DETECT
/*
	�������̉��(�����I��null��������)
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

	//�C�����C�������Ȃ��ƃG���[�ɂȂ�
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
		//�Ǘ��p�|�C���^�\����
		struct ForManagementPointer
		{
#ifdef REFERENCE_COUNTER
			ForManagementPointer() : pointer(nullptr), size(0), tag(0), use(0), destructor(nullptr) { }
			ForManagementPointer(size_t s, AllocType Type, int Tag, void(*d)(void*)) : pointer(nullptr), size(s), tag(Tag), use(0), destructor(d) { /*�g�p���t���O�𗧂Ă�*/tag |= TAG_SIGN_BIT; }
#else
			ForManagementPointer() : pointer(nullptr), size(0), tag(0), destructor(nullptr) { }
			ForManagementPointer(size_t s, AllocType Type, int Tag, void(*d)(void*)) : pointer(nullptr), size(s), tag(Tag), destructor(d) { /*�g�p���t���O�𗧂Ă�*/tag |= TAG_SIGN_BIT; }
#endif
			~ForManagementPointer();

			auto operator<=>(const ForManagementPointer& b) const;
			auto operator<=>(const void* b) const;

			int tag;
#ifdef REFERENCE_COUNTER
			int use;
#endif // REFERENCE_COUNTER
			void(*destructor)(void*);
			char* pointer;				//�|�C���^�̎���
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
			/*�z�u����ꏊ�̐ݒ�*/ {
				ScopedCS CS(&cs);
				//�f�Љ����Ă�̈�ɓ�����邩�𔻒�
				for (auto itr = temp.freeSpaceList.begin(); itr != temp.freeSpaceList.end(); ++itr) {
					auto free_space = *itr;
					//�m�ۗ\��̃T�C�Y�ƒf�Љ����Ă���̈�̔�r
					if (size > free_space->size)
						continue;
					//�f�Љ��̈悩��؂�o���Ĕz�u����ꏊ�����肷��
					alloc_ptr = free_space->ptr;
					if (size != free_space->size) {
						//�f�Љ��̈�̍Đݒ�
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
				//�f�Љ��̈�ɂȂ������ꍇ��Ԍ��ɔz�u
				if (!alloc_ptr) {
					//�󂫗e�ʂ���?
					if (temp.nextPointer + size > temp.endPointer)
						throw;	//�����ŗ�O���o�����Ȃ�m�ۍς݂̃�����������Ȃ�
					alloc_ptr = temp.nextPointer;
				}
				if (alloc_ptr == temp.nextPointer)
					temp.nextPointer += size;
			}
			//2�d�|�C���^�̃|�C���^���̎��Ԃ����
			ForManagementPointer* ptr = ManagementPointerAllocate(size, type, tag, [](void* p) { ((T*)p)->~T(); p = nullptr; });
#if MOMORY_LEAK_DETECT	//���������[�N���m�p
			strcpy_s(ptr->fileName, sizeof(ForManagementPointer::fileName), fileName);
			ptr->lineNumber = lineNumber;
#endif // MOMORY_LEAK_DETECT

#ifdef REFERENCE_COUNTER
			Pointer<T> result((T**)&ptr->pointer, type, &ptr->use);
#else
			Pointer<T> result((T**)&ptr->pointer, i_type);
#endif // REFERENCE_COUNTER

			//�Ǘ��p�ϐ������X�g�Ɋi�[
			temp.list.push_back(ptr);

			//���Ԃ����
			ptr->pointer = reinterpret_cast<char*>(new(alloc_ptr) T(args...));

			return result;
		}
		template<class T>
		static _inline void Delete(Pointer<T>& p)
		{
			auto& t_list = PointerList[p.type];
			CriticalSection& cs = ListCS[p.type];
			//t_list�̊Ǘ��O�|�C���^?
			if (t_list.endPointer < reinterpret_cast<char*>(p.get()) || t_list.fastPointer > reinterpret_cast<char*>(p.get()))
				throw;
			//p������
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
					//�f�X�g���N�^����T�̃f�X�g���N�^���Ăяo���Ă���(�A���P�[�g���Ƀf�X�g���N�^��ۑ����Ă��邽��T�����N���X�|�C���^�ł����Ă��m���ɊJ�������)
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
		/*�|�C���^�̊i�[�ꏊ���擾����
		�������d���̂łł��邾���g�p���Ȃ�*/
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

		//type�̎�ނ̃C���X�^���X��S�Ĕj������(list���Ń|�C���^���Q�Ƃ��Ă���ꍇ�Anullptr�ɂȂ邽�ߒ���)
		static void Clear(AllocType type);
		//�w���Tag�������C���X�^���X��S�Ĕj������(list���Ń|�C���^���Q�Ƃ��Ă���ꍇ�Anullptr�ɂȂ邽�ߒ���)
		static void Clear(MemTag type);

	private:
		//�����p
		static void Delete(std::list<ForManagementPointer*>::iterator itr, AllocType type);
		//�Ǘ��p�|�C���^���m�ۂ���
		static ForManagementPointer* ManagementPointerAllocate(const ForManagementPointer& p);
		//�Ǘ��p�|�C���^���m�ۂ���
		static ForManagementPointer* ManagementPointerAllocate(size_t s, AllocType Type, int Tag, void(*d)(void*));
		//�f�Љ����i�[�p�|�C���^���m�ۂ���
		static FreeSpaceData* FreeSpaceDataAllocate();
		//�f�Љ����𐳋K������
		static void FreeSpaceDataNormalize();
		//�f�Љ����𐳋K������(��ގw��)
		static void FreeSpaceDataNormalize(__int64 type);

	public:
		/*	�Ǘ��p�ϐ�:�^�O�̕����r�b�g
			�R���p�C���G���[�΍�Ƀ����o�Œ�`	*/
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
		//pointerList_���g��������
		static inline char* Pool = nullptr;
		static inline ManagementPointerList* PointerList = nullptr;
		//�Ǘ��p�ϗp���������
		static inline ManagerData<ForManagementPointer> PointerData;
	public:
		static inline ManagerData<FreeSpaceData> FSData;
	};
}