#pragma once
#include <list>
#include "GameSystem/Allocator/Pointer.h"
#include "RenderObject/RenderObject2D.h"
#include "RenderObject/RenderObject3D.h"
#include <condition_variable>

#define is3D true
namespace MyLib
{
	//�����_�����O�Ǘ��N���X
	class Render
	{
	public:
		//�`��ΏۃX�N���[����ݒ肷��
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
		//�ߋ����p�V���h�E�}�b�v�̒��S�_�����߂�
		static void SetNearShadowDrawArea(const std::function<Vector()>& func) { SetNearShadowAreaFunc = func; }
		//�ߋ����p�V���h�E�}�b�v�̒��S�_�����߂�
		static void SetNearShadowDrawArea(std::function<Vector()>&& func) { SetNearShadowAreaFunc = func; }
		//�X�^�e�B�b�N�I�u�W�F�N�g�̉e����������
		static void WriteStaticShadow();

		//�`�惊�X�g�ɒǉ�����
		static void PushModelList(const OffSetPtr<RenderObject3DBase>& ptr) { std::lock_guard<std::mutex> lock(ModelMtx_); DrawModelList_.push_back(ptr); }
		//�`�惊�X�g�ɒǉ�����
		template<class T>
		static void PushModelList(const Pointer<T> p, const RenderObject3DBase* o) { auto ptr = OffSetPtr<RenderObject3DBase>(p, o); std::lock_guard<std::mutex> lock(ModelMtx_); DrawModelList_.push_back(ptr); }

		//�`�惊�X�g���珜�O����
		static void EreseModelList(const OffSetPtr<RenderObject3DBase>& ptr) {
			std::lock_guard<std::mutex> lock(ModelMtx_);
			auto itr = std::find(DrawModelList_.begin(), DrawModelList_.end(), ptr);
			if (itr != DrawModelList_.end())
				DrawModelList_.erase(itr);
		}
		//�`�惊�X�g���珜�O����
		template<class T>
		static void EreseModelList(const Pointer<T> p, const RenderObject3DBase* o) {
			auto ptr = OffSetPtr<RenderObject3DBase>(p, o);
			std::lock_guard<std::mutex> lock(ModelMtx_);
			auto itr = std::find(DrawModelList_.begin(), DrawModelList_.end(), ptr);
			if (itr != DrawModelList_.end())
				DrawModelList_.erase(itr);
		}
#endif // is3D
		//�`�惊�X�g�ɒǉ�����
		static void PushSpriteList(const OffSetPtr<RenderObject2D>& ptr) { std::lock_guard<std::mutex> lock(SpriteMtx_); DrawSpriteList_.push_back(ptr); }
		//�`�惊�X�g�ɒǉ�����
		template<class T>
		static void PushSpriteList(const Pointer<T> p, const RenderObject2D* o) { auto ptr = OffSetPtr<RenderObject2D>(p, o); { std::lock_guard<std::mutex> lock(SpriteMtx_); DrawSpriteList_.push_back(ptr); } }

		//�`�惊�X�g���珜�O����
		static void EreseSpriteList(const OffSetPtr<RenderObject2D>& ptr) {
			std::lock_guard<std::mutex> lock(SpriteMtx_);
			auto itr = std::find(DrawSpriteList_.begin(), DrawSpriteList_.end(), ptr);
			if (itr != DrawSpriteList_.end())
				DrawSpriteList_.erase(itr);
		}
		//�`�惊�X�g���珜�O����
		template<class T>
		static void EreseSpriteList(const Pointer<T> p, const RenderObject2D* o) {
			auto ptr = OffSetPtr<RenderObject2D>(p, o);
			std::lock_guard<std::mutex> lock(SpriteMtx_);
			auto itr = std::find(DrawSpriteList_.begin(), DrawSpriteList_.end(), ptr);
			if (itr != DrawSpriteList_.end())
				DrawSpriteList_.erase(itr);
		}

		//�摜���\�[�g����
		static void SortSpriteList() { std::lock_guard<std::mutex> lock(SpriteMtx_); DrawSpriteList_.sort([](const OffSetPtr<RenderObject2D> a, const OffSetPtr<RenderObject2D> b) {return *a < *b; }); }

		//�`�惊�X�g�ɒǉ�����
		static void PushStringList(const OffSetPtr<RenderObjectString>& ptr) { std::lock_guard<std::mutex> lock(StringMtx_); DrawStringList_.push_back(ptr); }
		//�`�惊�X�g�ɒǉ�����
		template<class T>
		static void PushStringList(const Pointer<T> p, const RenderObjectString* o) { auto ptr = OffSetPtr<RenderObjectString>(p, o); std::lock_guard<std::mutex> lock(StringMtx_); DrawStringList_.push_back(ptr); }

		//�`�惊�X�g���珜�O����
		static void EreseStringList(const OffSetPtr<RenderObjectString>& ptr) {
			std::lock_guard<std::mutex> lock(StringMtx_);
			auto itr = std::find(DrawStringList_.begin(), DrawStringList_.end(), ptr);
			if (itr != DrawStringList_.end())
				DrawStringList_.erase(itr);
		}
		//�`�惊�X�g���珜�O����
		template<class T>
		static void EreseStringList(const Pointer<T> p, const RenderObjectString* o) {
			auto ptr = OffSetPtr<RenderObjectString>(p, o);
			std::lock_guard<std::mutex> lock(StringMtx_);
			auto itr = std::find(DrawStringList_.begin(), DrawStringList_.end(), ptr);
			if (itr != DrawStringList_.end())
				DrawStringList_.erase(itr);
		}

		//isRendering_��true�ɂȂ�܂őҋ@����
		static void WaitRender() { std::unique_lock<std::mutex> lock(RenderMtx_); RenderCV_.wait(lock, [] { return Rendering_; }); }
		//Render���ҋ@��ԂɂȂ�܂őҋ@����
		static void WaitRenderStandby() { std::unique_lock<std::mutex> lock(RenderMtx_); RenderCV_.wait(lock, [] { return Waiting_; }); }
		static void SetRenderFlag(bool f) {
			std::lock_guard<std::mutex> lock(RenderMtx_);
			Rendering_ = f;
			RenderCV_.notify_one();  // �����ϐ���ʒm
		}
	private:
		static inline bool										Rendering_ = true;//�����_�����O�����邩�̔���(���X�g�̍폜�Ɏg�p)
		static inline bool										Waiting_ = false;	//�����_�����O�����邩�̔���(���X�g�̍폜�Ɏg�p)
		static inline std::mutex								RenderMtx_;			
		static inline std::condition_variable					RenderCV_;			
		static inline std::list<OffSetPtr<RenderObject2D>>		DrawSpriteList_;	//�摜���X�g
		static inline std::mutex								SpriteMtx_;			//�摜(DrawSpriteList_)�p�~���[�e�b�N�X
		static inline std::list<OffSetPtr<RenderObjectString>>	DrawStringList_;	//�����񃊃X�g
		static inline std::mutex								StringMtx_;			//������(DrawStringList_)�p�~���[�e�b�N�X
#if is3D
		static inline std::list<OffSetPtr<RenderObject3DBase>>	DrawModelList_;		//���f�����X�g
		static inline std::mutex								ModelMtx_;			//���f��(DrawModelList_)�p�~���[�e�b�N�X
		static inline int										StaticShadow_ = 0;
		static inline int										NearShadow_ = 0;
		static inline std::function<Vector()>					SetNearShadowAreaFunc;
#endif // is3D
	};
}