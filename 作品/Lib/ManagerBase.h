#pragma once
#include "Define.h"
#include "GameSystem/Allocator/Allocator.h"
#include <compare>

//�}�l�[�W���[�p�t�����h�錾
#define MANAGER \
friend class Allocator;\
template<class T>\
friend class SingletonBase

#define PTR_LIST(type) std::list<Pointer<type>>
#define PTR_VECTOR(type) std::vector<Pointer<type>>

namespace MyLib
{
	enum class MANAGER_INDEX :u64 {
		MultiThread,		//�}���`�X���b�h�Ŏ��s����悤
		SequentialProcess,	//��������(���s��)
		CollisionStep,
		CameraStep,
		Last				//��������(�Ō�)
	};

#define IMANAGER_INDEX (int)MyLib::MANAGER_INDEX

	/*
		�}�l�[�W���[�̊��N���X
		���X�g�ł̊Ǘ��p
	*/
	class ManagerBase
	{
	public:
		ManagerBase() :Index_(MANAGER_INDEX::MultiThread) {}
		virtual ~ManagerBase() = default;
	public:
		virtual void Init() = 0;
		virtual void Load() = 0;
		virtual void OnLoadEnd() {}		//���[�h�I����̃I�u�W�F�N�g�����Ȃǂ��s������
		virtual void Step() = 0;
		virtual void DeleteStep() {}	//���s���̒�߂��Ă��Ȃ��^�C�~���O�ɔj���������s���ꍇ�̊֐�
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
		�V���O���g�����p������}�l�[�W���[�N���X
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