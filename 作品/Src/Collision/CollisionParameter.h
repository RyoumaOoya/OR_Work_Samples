#pragma once
// �R���W�����Ɏg�p����p�����[�^�[

// �R���W�����̌`��
enum class CollisionShape :unsigned __int16 {
	SPHERE,	// ���̓����蔻��
	BOX,	// �{�b�N�X�̓����蔻��iAABB�j
	VECTOR,	// �x�N�g���̓����蔻��
	MODEL,
	NUM
};

enum class CollisionType :unsigned __int8{
	NOT_COLL,
	NOT_RACE,
	CALC_RACE,
};
#define ICollisionType (int)CollisionType

// �R���W�����̃J�e�S���[
enum class CollisionCategory :int {
	STAGE, 
	ITEM,
	PLAYER,			// �v���C���[
	ENEMY,
	BULLET,
	NUM
};

// �R���W�����̏��
enum class CollisionState :unsigned __int16 {
	ACTIVE,	// �L��
	UNUSE,	// ����
	NONE,	// �g�p�ς�
};

#define ICollisionShape (unsigned __int16)CollisionShape
#define ICollisionCategory (int)CollisionCategory
#define ICollisionState (unsigned __int16)CollisionState