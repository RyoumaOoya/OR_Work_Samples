#pragma once

//�V�[�����
enum class SCENE_STATE :int {
	INIT,		//������
	LOAD,		//���[�h
	LOAD_END,	//���[�h�I��
	LOOP,		//���[�v
	FIN,		//�㏈��
	FIN_END,
};

#define ISCENE_STATE (int)SCENE_STATE
//�V�[��ID
enum class SCENE_ID :unsigned __int16
{
	TATLE,
	PLAY,	//�v���C�V�[��
	NUM,
};

#define ISCENE_ID (int)SCENE_ID

enum class SUB_SCENE_ID :unsigned __int16
{

};