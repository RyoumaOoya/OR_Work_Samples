#pragma once
#include "Collision.h"

class HitPolygonList
{
public:
	enum class HITTYPE {
		Null = -1, 
		Floor,	//床
		Ceil,	//天井
		xy_wall,//XY平面上の壁
		yz_wall,//YZ平面上の壁
	};
	//当たったポリゴンデータ
	struct HitPolygon{
		HitPolygon() :HitType_(HITTYPE::Null), Polygon_() {};
		HitPolygon(HITTYPE hit_type, POLYGON polygon) :HitType_(hit_type), Polygon_(polygon) {};
		HITTYPE HitType_;
		POLYGON Polygon_;
	};

	HitPolygonList() {};
	HitPolygonList(const Pointer<Object>& hitObject) { HitObject_ = hitObject; };

	Pointer<Object> HitObject_;
	std::list<HitPolygon> HitPolygons_;
private:
#ifdef _WIN64
	char buffer[4] = { 0 };
#endif // _X_86_
};

//作成中
class ModelCollision :public Collision
{
	friend class ModelCollision;
public:
	ModelCollision();
	ModelCollision(int handle);
	ModelCollision(const ModelCollision& model_col);
	~ModelCollision() override;

public:
	void Init() override;
	void Step() override;
	void UpDate() override;
	void Fin() override;
	Pointer<Collision> Clone() override;

	bool CheckCollision(Pointer<Collision> other) override;
public:
	void LoadCollisionModel(const char* model_path);
	void DeleteCollisionModel();

	int GetHandle()const { return Handle_; }

	void AddHitPolygonList(const HitPolygonList& polygon) { CurrentHitPolygon_.push_back(polygon); }
	HitPolygonList GetOidHitPolygon(const Pointer<Object>& hitobject) {
		for (auto polygons : OidHitPolygon_) {
			if (polygons.HitObject_ == hitobject) return polygons;
		}
		return HitPolygonList();
	}
	HitPolygonList GetCurrentHitPolygon(const Pointer<Object>& hitobject) {
		for (auto polygons : CurrentHitPolygon_) {
			if (polygons.HitObject_ == hitobject) return polygons;
		}
		return HitPolygonList();
	}

	const MV1_REF_POLYGONLIST& GetPolygonsList() { return Polygons_; }
private:
	std::list<HitPolygonList> OidHitPolygon_;
	std::list<HitPolygonList> CurrentHitPolygon_;

	MV1_REF_POLYGONLIST Polygons_;

	int Handle_;
private:
	char buffer[4] = { 0 };
};

/*
	ModelCollision メモリイメージ

	Collision{
		oidCollision_		12byte
		currentCollison_	12byte

		owner_		16byte
		pos_		24byte
		
		if _X_64_
			posCalc_	8byte
			shape_	4byte	state_	4byte

		else if _X86_
			posCalc_	4byte	shape_	2byte	state_	2byte

		vfptr		8byte
	}

	handle_	4byte	buffer	4byte
*/

namespace Size {
	constexpr size_t HIT_POLYGON = sizeof(HitPolygonList);
	constexpr size_t MODEL_COLLISION = sizeof(ModelCollision);
}