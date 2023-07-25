#pragma once
#include "Literal.h"
#include "Object/Object.h"
#include "Constant/Constant.h"
#include "GameSystem/Allocator/Pointer.h"

/*
	STATE��u64�^�ɂ������R
	enum���p������悤�ȓ��������z�I��������������������̂�
	�������u64�^�ɍĉ��߂��邱�Ƃň�ӂ̒l�ɂ���B
	�ėp�I�Ȃ��̂�Base.h�ɒu���A�q�N���X�̃w�b�_�Œǉ����Ă������Ƃŋ^���I�ɍČ��B
	���̂܂܂ł͔z��C���f�b�N�X�Ɏg���Ȃ���switch���ɂ͎g����悤�ɂȂ��Ă���̂ł������񂱂�Ői�߂�
*/
namespace AI_STATE {
	constexpr u64 STAND			= "Stand"_int64;		//�ҋ@
	constexpr u64 FOLLOW_PLAYER = "Flo_Ply"_int64;		//�v���C���[�Ɍ�����
	constexpr u64 FOLLOW_OBJECT = "Flo_Obj"_int64;		//�I�u�W�F�N�g�Ɍ�����
	constexpr u64 APPROACHED	= "Appred"_int64;		//�߂Â�����̑ҋ@���
	constexpr u64 APPR_PLAYER	= "Appr_Ply"_int64;
	constexpr u64 RETURN		= "Return"_int64;		//�߂�
};

class AIBase
{
public:
	AIBase();
	virtual ~AIBase();

public:
	virtual u64 GetNextState(Pointer<Object>) = 0;

protected:
	//�v���C���[�܂ł̋������Z�o����
	//����xz��y���𖳎����邩
	float	GetPlayerDist(const Pointer<Object>& object, bool xz);
	float	GetPlayerDistSq(const Pointer<Object>& object, bool xz);
	Vector	GetPlayerForwardVec(const Pointer<Object>& object);
};