#pragma once
#include "Scene.h"

class Title :public Scene
{
public:
	Title();
	~Title();
public:
	//������
	void Init()override;
	//���[�h
	void Load()override;
	//���[�h����
	void OnLoadEnd()override;
	//�X�e�b�v����
	void Step()override;
	//�X�V����
	void UpDate()override;
	//�`��
	void Draw()override;
	//�őO�ʕ`��
	void FrontDraw()override;
	//�㏈��
	void Fin()override;
};

