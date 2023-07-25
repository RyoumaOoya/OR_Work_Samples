#include "Map.h"
#include "MapParameter.h"
#include "Render/Render.h"
#include "Object/ObjectParameter.h"
#include "Src/Collision/SphereCollision.h"
#include "Src/Player/PlayerManager.h"
#include "Src/Item/Item.h"
#include "Src/Enemy/EnemyBase.h"
#include "Src/Camera/CameraManager.h"

namespace
{
	int AddState(int state, int ID)
	{
		return (state | (0x01 << ID));
	}
	bool isState(int state, int ID)
	{
		return (state & (0x01 << ID)) != 0;
	}
}

Map::Map() :ID_(0), Object()
{
	RenderObject_.Pos_ = Vector(0.f, -1.35f, 0.f);
	SetTag(OBJECT_TAGS::STAGE);
	RenderObject_.SetTag(RenderTags::StaticShadow);
	RenderObject_.RemoveTag(RenderTags::ShadowWrite);
}

Map::Map(int ID) :Map()
{
	ID_ = ID;
}

Map::~Map()
{
	Fin();
}

void Map::OnCollisionBegin(Pointer<Object> other)
{
	OnCollisionEnter(other);
}

void Map::OnCollisionEnter(Pointer<Object> other)
{
	if (other->ObjectHasTag(OBJECT_TAGS::PLAYER)) {
		const HitPolygonList& hit_polygons = GetCollision()->GetCurrentHitPolygon(other);
		Pointer<Player> player = static_cast<Pointer<Player>>(other);

		Vector player_move = player->GetMove();

		int state = 0;

		player->SetPos(ExtrudePos(player->GetPos(), hit_polygons.HitPolygons_, player->GetCollision()->GetRadius(), &player_move, &state));
		player->SetMove(player_move);

		//接地状態を更新
		::isState(state, IOBJECT_STATE::Grounding) ? player->AddFlag(OBJECT_FLAGS::Grounding) : player->RemoveFlag(OBJECT_FLAGS::Grounding);
	}
	else if (other->ObjectHasTag(OBJECT_TAGS::ITEM)) {
		const HitPolygonList& hit_polygons = GetCollision()->GetCurrentHitPolygon(other);
		Pointer<Item> item = static_cast<Pointer<Item>>(other);

		Vector item_move = item->GetMove();

		int state = 0;

		item->SetPos(ExtrudePos(item->GetPos(), hit_polygons.HitPolygons_, item->GetCollision()->GetRadius(), &item_move, &state));
		item->SetMove(item_move);

		//接地状態を更新
		::isState(state, IOBJECT_STATE::Grounding) ? item->AddFlag(OBJECT_FLAGS::Grounding) : item->RemoveFlag(OBJECT_FLAGS::Grounding);
	}
	else if (other->ObjectHasTag(OBJECT_TAGS::ENEMY)) {
		const HitPolygonList& hit_polygons = GetCollision()->GetCurrentHitPolygon(other);
		Pointer<EnemyBase> enemy = static_cast<Pointer<EnemyBase>>(other);

		Vector enemy_move = enemy->GetMove();

		int state = 0;

		enemy->SetPos(ExtrudePos(enemy->GetPos(), hit_polygons.HitPolygons_, enemy->GetCollision()->GetRadius(), &enemy_move, &state));
		enemy->SetMove(enemy_move);

		//接地状態を更新
		::isState(state, IOBJECT_STATE::Grounding) ? enemy->AddFlag(OBJECT_FLAGS::Grounding) : enemy->RemoveFlag(OBJECT_FLAGS::Grounding);
	}
}

void Map::OnCollisionEnd(Pointer<Object> other)
{
	other->RemoveFlag(OBJECT_FLAGS::Grounding);
}

void Map::Init()
{
	Init(Allocator::GetPointerStorage(this, AllocType::Stage));
}

void Map::Init(const Pointer<Map>& this_ptr)
{
	__super::Init();
	SetThisPtr((Pointer<Object>)this_ptr);
	PushModelList();
	CollisionMesh_ = static_cast<Pointer<ModelCollision>>(CollisionManager::GetInstance()->CreateCollision(ICollisionShape::MODEL, ICollisionCategory::STAGE));
	CollisionMesh_->Init();
	CollisionMesh_->SetOwner(This_);
}

void Map::Load()
{
	__super::Load(StageFilePath[ID_].data());
	RenderObject_.WaitLoad();
	UpDate();
	LoadCollisionModel(StageCollisionFilePath[ID_].data());
}

void Map::Step()
{
}

void Map::Delete()
{
	__super::Delete();
	EreseModelList();
	if (CollisionMesh_)
		CollisionMesh_->SetState(ICollisionState::UNUSE);
}

void Map::Fin()
{
	__super::Fin();
	Delete();
}

Vector Map::GetIntersectionWithScreenPos(const Vector2D& vec)
{
	Vector pos = ConvScreenPosToWorldPos(static_cast<Vector>(vec).SetZ(1.f));
	auto coll_result = MV1CollCheck_Line(CollisionMesh_->GetHandle(), -1, CameraManager::GetInstance()->GetPos(), pos);
	return coll_result.HitFlag ? Vector(coll_result.HitPosition) : Vector();
}

/*
	押し出し処理
	pos: 元座標	,	hitPolygons: 当たったポリゴンデータ	,	radius: 当たったオブジェクトの半径	,	move: オブジェクトのmove変数
	return 押し出し先の座標
*/
Vector Map::ExtrudePos(const Vector& pos, const std::list<HitPolygonList::HitPolygon>& hitPolygons, float radius, Vector* move, int* state)
{
	typedef HitPolygonList::HITTYPE hittype;
	static auto closer_pos = [](const Vector& sphere_pos, double radius, const Vector& norm) {
		return sphere_pos + (-norm * radius);
	};

	//床押し出し処理
	static auto hot_floor = [](const POLYGON& polygon, Vector* pos, Vector* move, float radius, int* state) {
		Vector norm = MyMath::GetPolygonNormalVec(polygon);
		Vector temp = closer_pos(*pos, radius, norm);
		float floor_height = MyMath::GetTriangleHeightXZ(temp, polygon);
		float dist = (temp.y) - floor_height;

		if (dist < 0.f && MyMath::Abs(dist) <= radius) {
			pos->y = floor_height + (radius * norm.y);
			//result += (MyMath::GetPolygonNormalVec(hit_polygon.Polygon) * radius);
			move->y = 0.f;
			*state = ::AddState(*state, IOBJECT_STATE::Grounding);
		}
	};

	//天井押し出し処理
	static auto hot_ceil = [](const POLYGON& polygon, Vector* pos, Vector* move, float radius, int* state) {
		float ceil_height = MyMath::GetTriangleHeightXZ(*pos, polygon);
		float dist = (pos->y + radius) - ceil_height;

		if (dist > 0.f && MyMath::Abs(dist) <= radius) {
			pos->y = ceil_height - radius;
			move->y = 0.f;
			*state = ::AddState(*state, IOBJECT_STATE::Ceiling);
		}
	};

	//xy壁の押し出し処理
	static auto hot_xy_wall = [](const POLYGON& polygon, Vector* pos, Vector* move, float radius, int* state) {
		Vector norm = MyMath::GetPolygonNormalVec(polygon);
		float dist = ((norm.z < 0.f) ? pos->z + radius : pos->z - radius) - MyMath::GetTriangleHeightXY(*pos, polygon);

		if (MyMath::Abs(dist) > radius)	return;

		if ((norm.z < 0.f && dist > 0.f) || (norm.z > 0.f && dist < 0.f)) {
			Vector push = norm * MyMath::Abs(dist);
			*pos += push;
			*state = ::AddState(*state, IOBJECT_STATE::Wall);
			if ((push.x < 0.f && move->x < 0.f) ||
				(push.x > 0.f && move->x > 0.f))
				move->x = 0.f;
			if ((push.z < 0.f && move->z < 0.f) ||
				(push.z > 0.f && move->z > 0.f))
				move->z = 0.f;
		}
	};

	//yz壁の押し出し処理
	static auto hot_yz_wall = [](const POLYGON& polygon, Vector* pos, Vector* move, float radius, int* state) {
		Vector norm = MyMath::GetPolygonNormalVec(polygon);
		float dist = ((norm.x < 0.f) ? pos->x + radius : pos->x - radius) - MyMath::GetTriangleHeightYZ(*pos, polygon);

		if (MyMath::Abs(dist) > radius) return;

		if ((norm.x < 0.f && dist > 0.f) || (norm.x > 0.f && dist < 0.f)) {
			Vector push = norm * MyMath::Abs(dist);
			*pos += push;
			*state = ::AddState(*state, IOBJECT_STATE::Wall);
			move->x += push.x;
			if ((push.x < 0.f && move->x < 0.f) ||
				(push.x > 0.f && move->x > 0.f))
				move->x = 0.f;
			if ((push.z < 0.f && move->z < 0.f) ||
				(push.z > 0.f && move->z > 0.f))
				move->z = 0.f;
		}
	};

	Vector result = pos;

	for (const HitPolygonList::HitPolygon& hit_polygon : hitPolygons) {
		switch (hit_polygon.HitType_)
		{
		case hittype::Floor:	hot_floor(hit_polygon.Polygon_, &result, move, radius, state);	break;
		case hittype::Ceil:		hot_ceil(hit_polygon.Polygon_, &result, move, radius, state);	break;
		case hittype::xy_wall:	hot_xy_wall(hit_polygon.Polygon_, &result, move, radius, state); break;
		case hittype::yz_wall:	hot_yz_wall(hit_polygon.Polygon_, &result, move, radius, state); break;
		}
	}

	return result;
}
