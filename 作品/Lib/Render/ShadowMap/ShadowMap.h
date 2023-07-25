#pragma once
#include "../../Math/MyMathData.h"
#include "../../GameSystem/Allocator/Pointer.h"

namespace MyLib
{
	//DxLib�̃V���h�E�}�b�v�̃��b�p�[
	class ShadowMap
	{
		friend class ShadowMapManager;
	private:
		static Vector GlobalLight;
	public:
		class ShadowMapData {
		public:
			constexpr ShadowMapData(iVector2D mapSize, Vector minArea, Vector maxArea, Vector lightDirection = Vector::ZEROVEC())
				:MapSize(mapSize), MinArea(minArea), MaxArea(maxArea), LightDirection(lightDirection) {}
			iVector2D MapSize;		//�𑜓x
			Vector MinArea;			//�͈�(�n�_)
			Vector MaxArea;			//�͈�(�I�_)
			Vector LightDirection;	//�f�B���N�V���i�����C�g

		private:
			char buffer[4] = { 0 };
		};
	public:
		ShadowMap();
		ShadowMap(int index, const ShadowMapData& data);
		~ShadowMap();

		void Init();

		//�e�̏������݂��J�n
		void ShadowWriteStart();
		//�e�̏������݂��I��
		void ShadowWriteFin();

		//�e�t���`��̊J�n
		void ShadowDrawStart();
		//�e�t���`��̏I��
		void ShadowDrawFin();

		void Delete();
		void Fin();

		/*
			�͈͐ݒ�
			����: minArea �n�_, maxArea �I�_
		*/
		void SetShadowMapDrawArea(const Vector& minArea, const Vector& maxArea);
		//�f�B���N�V���i�����C�g�̐ݒ�
		void SetLightDirection(const Vector& direction);
	private:
		int Handle_;
		int Index_;
	};

	/*
		�R���X�g���N�^�ŃV���h�E�}�b�v�̏������݂��J�n�A
		�f�X�g���N�^�Ŋm���ɃV���h�E�}�b�v�̏������݂��I������N���X
	*/
	class ScopedShadowWrite {
	public:
		ScopedShadowWrite(const Pointer<ShadowMap>& shadowMap) :shadowMap(shadowMap) { Start(); }
		~ScopedShadowWrite() { Fin(); }

		void Start() { shadowMap->ShadowWriteStart(); }
		void Fin() { shadowMap->ShadowWriteFin(); }
	private:
		Pointer<ShadowMap> shadowMap;
	};

	/*
		�R���X�g���N�^�ŉe�t���`����J�n�A
		�f�X�g���N�^�Ŋm���ɉe�t���`����I������N���X
	*/
	class ScopedShadowDraw {
	public:
		ScopedShadowDraw(const Pointer<ShadowMap>& shadowMap) :shadowMap(shadowMap) { Start(); }
		~ScopedShadowDraw() { Fin(); }

		void Start() { shadowMap->ShadowDrawStart(); }
		void Fin() { shadowMap->ShadowDrawFin(); }
	private:
		Pointer<ShadowMap> shadowMap;
	};
}

/*
	ShadowMap �������C���[�W

	ShadowMapData{
		MapSize			8byte
		MinArea			24byte
		MaxArea			24byte
		LightDirection	24byte
	}

	handle_	4byte	this_index_ 4byte
*/

namespace Size {
	constexpr size_t SHADOWMAP = sizeof(MyLib::ShadowMap);
	constexpr size_t SHADOWMAP_DATA = sizeof(MyLib::ShadowMap::ShadowMapData);
}