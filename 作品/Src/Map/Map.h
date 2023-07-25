#pragma once
#include "Object/Object.h"
#include "Src/Collision/CollisionManager.h"
#include "Src/Collision/ModelCollision.h"

class Map :public Object
{
public:
	Map();
	Map(int ID);
	 ~Map() override;
	 void OnCollisionBegin(Pointer<Object> other) override;
	 void OnCollisionEnter(Pointer<Object> other) override;
	 void OnCollisionEnd(Pointer<Object> other) override;

public:
	void Init() override;
	void Init(const Pointer<Map>& this_ptr);
	void Load() override;
	void Step() override;
	void Delete() override;
	void Fin() override;

public:
	Pointer<ModelCollision> GetCollision() { return CollisionMesh_; }
	void LoadCollisionModel(const char* file_path) { CollisionMesh_->LoadCollisionModel(file_path); }

	//スクリーン座標とマップのコリジョンの交点を取得
	//当たっていなかったらゼロベクトルを返す
	Vector GetIntersectionWithScreenPos(const Vector2D& vec);
private:
	Vector ExtrudePos(const Vector& pos, const std::list<HitPolygonList::HitPolygon>& hitPolygons, float radius, Vector* move, int* state);
private:
	Pointer<ModelCollision> CollisionMesh_;
	int ID_;
private:
	char buffer[4] = { 0 };
};

/*
	Stage メモリイメージ

	Object{
		Pos_	24byte
		Tag_	8byte
		Handle_ 4byte	Active_ 1byte	buffer 3byte
		vfptr	8byte
	}	:48byte

	collision_mesh_	24byte
*/

namespace Size {
	constexpr size_t MAP = sizeof(Map);
}