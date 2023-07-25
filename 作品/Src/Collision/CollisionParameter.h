#pragma once
// コリジョンに使用するパラメーター

// コリジョンの形状
enum class CollisionShape :unsigned __int16 {
	SPHERE,	// 球の当たり判定
	BOX,	// ボックスの当たり判定（AABB）
	VECTOR,	// ベクトルの当たり判定
	MODEL,
	NUM
};

enum class CollisionType :unsigned __int8{
	NOT_COLL,
	NOT_RACE,
	CALC_RACE,
};
#define ICollisionType (int)CollisionType

// コリジョンのカテゴリー
enum class CollisionCategory :int {
	STAGE, 
	ITEM,
	PLAYER,			// プレイヤー
	ENEMY,
	BULLET,
	NUM
};

// コリジョンの状態
enum class CollisionState :unsigned __int16 {
	ACTIVE,	// 有効
	UNUSE,	// 無効
	NONE,	// 使用済み
};

#define ICollisionShape (unsigned __int16)CollisionShape
#define ICollisionCategory (int)CollisionCategory
#define ICollisionState (unsigned __int16)CollisionState