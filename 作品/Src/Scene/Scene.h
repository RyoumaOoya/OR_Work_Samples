#pragma once
#include "typedef.h"
#include <vector>

class Scene
{
protected:
	enum class State :u32 {
		NONE,
		FADEIN,
		FADEOUT,
	};
public:
	Scene();
	virtual ~Scene();

	//������
	virtual void Init() = 0;
	//���[�h
	virtual void Load() = 0;
	//���[�h����
	virtual void OnLoadEnd() = 0;
	//�X�e�b�v����
	virtual void Step();
	//�X�V����
	virtual void UpDate() = 0;
	//�`��
	virtual void Draw() = 0;
	//�őO�ʕ`��
	virtual void FrontDraw() = 0;
	//�㏈��
	virtual void Fin() = 0;
protected:
	void ChangeScene(u32 next_scene, float fade_time = 1);
	virtual void OnFadeinEnd();
	void FadeIn();
private:
	u32 NextScene_;
protected:
	u32 State_;
	// vfptr(8byte)
};

/*
	Scene �������C���[�W

	next_scene_	8byte
	state_		8byte
	vfptr		8byte
*/

namespace Size {
	constexpr size_t SCENE_BASE = sizeof(Scene);
}