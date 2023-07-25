#pragma once
#include "../Object/Object.h"
#include "Anime/Anime.h"
namespace MyLib
{
	//�A�j���[�V�������܂�3D�I�u�W�F�N�g�p���N���X
	class Character :public Object
	{
		friend class Anime;
	public:
		Character();
		virtual ~Character();

		virtual void Load() override;
		virtual void Load(const char* file_path) override;
		virtual void Step() override;
		virtual void UpDate() override;
		virtual void Fin() override;

	protected:
		Anime Anim_;
	};
}
namespace Size {
	constexpr size_t CHARACTER = sizeof(MyLib::Character);
}