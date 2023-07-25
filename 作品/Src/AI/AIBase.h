#pragma once
#include "Literal.h"
#include "Object/Object.h"
#include "Constant/Constant.h"
#include "GameSystem/Allocator/Pointer.h"

/*
	STATEをu64型にした理由
	enumを継承するような動きが理想的だったが難しそうだったので
	文字列をu64型に再解釈することで一意の値にする。
	汎用的なものをBase.hに置き、子クラスのヘッダで追加していくことで疑似的に再現。
	そのままでは配列インデックスに使えないがswitch文には使えるようになっているのでいったんこれで進める
*/
namespace AI_STATE {
	constexpr u64 STAND			= "Stand"_int64;		//待機
	constexpr u64 FOLLOW_PLAYER = "Flo_Ply"_int64;		//プレイヤーに向かう
	constexpr u64 FOLLOW_OBJECT = "Flo_Obj"_int64;		//オブジェクトに向かう
	constexpr u64 APPROACHED	= "Appred"_int64;		//近づいた後の待機状態
	constexpr u64 APPR_PLAYER	= "Appr_Ply"_int64;
	constexpr u64 RETURN		= "Return"_int64;		//戻る
};

class AIBase
{
public:
	AIBase();
	virtual ~AIBase();

public:
	virtual u64 GetNextState(Pointer<Object>) = 0;

protected:
	//プレイヤーまでの距離を算出する
	//引数xzはy軸を無視するか
	float	GetPlayerDist(const Pointer<Object>& object, bool xz);
	float	GetPlayerDistSq(const Pointer<Object>& object, bool xz);
	Vector	GetPlayerForwardVec(const Pointer<Object>& object);
};