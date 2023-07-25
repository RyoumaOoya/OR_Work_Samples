#include "Collision.h"
#include "BoxCollision.h"
#include "SphereCollision.h"
#include "VectorCollision.h"
#include "ModelCollision.h"

namespace {
	//�}�b�v�֘A
	constexpr float		MAP_FLOOR_NORM_Y_MIN	= 0.5f;		//�@����Y������Βl�����̒l�ȏ�ł���Ώ�
	constexpr float		MAP_CEIL_NORM_Y			= -0.5f;	//�@����Y������Βl�����̒l�ȏ�ł���Ώ�
	constexpr float		MAP_WALL_NORM_X_MIN		= 0.5f;		//�@����Z������Βl�����̒l�ȏ�ł����YZ���ʂ̕�
	constexpr float		MAP_WALL_NORM_Z_MIN		= 0.5f;		//�@����Z������Βl�����̒l�ȏ�ł����XY���ʂ̕�
	constexpr Vector	COLL_RANGE		(150.f, 30.f, 150.f);	//�}�b�v�̃R���W�������Ƃ�͈�
};

Collision::Collision() :Owner_()
{
	Shape_ = 0;
	State_ = 0;
	PosCalc_ = nullptr;
}

Collision::~Collision()
{
	Fin();
}

void Collision::Init()
{
}

void Collision::Step()
{
	if (Owner_) {
		Pos_ = (PosCalc_) ? (*PosCalc_)(Owner_.get()) : Owner_->GetPos();

		OidCollision_ = CurrentCollison_;

		CurrentCollison_.clear();
	}
}

void Collision::UpDate()
{
	if (Owner_) {
		Pos_ = (PosCalc_) ? (*PosCalc_)(Owner_.get()) : Owner_->GetPos();
	}
}

void Collision::Draw()
{
}

void Collision::Fin()
{
	OidCollision_.clear();
	CurrentCollison_.clear();
}

void Collision::EnterCollision(Pointer<Collision> other)
{
	ScopedCS sc(&ListCS_);
	CurrentCollison_.push_back(other);
}

Collision::HitType Collision::CheckHitType(const Pointer<Collision>& other)
{
	bool is_current = false, is_oid = false;

	for (auto current : CurrentCollison_) {
		if (!current) continue;
		if (!current->isCheck()) continue;
		if (current == other) {
			is_current = true;
			break;
		}
	}
	for (auto oid : OidCollision_) {
		if (!oid) continue;
		if (!oid->isCheck()) continue;

		if (oid == other) {
			is_oid = true;
			break;
		}
	}

	return  (is_oid && is_current)	?	HitType::ENTER:	//�����葱���Ă���
			(is_current)			?	HitType::BEGIN:	//���������u��
			(is_oid)				?	HitType::END:	//������I�����
										HitType::NONE;	//�������Ă��Ȃ�
}

bool Collision::IsHitTriangleXY(const Vector& point, const POLYGON& polygon)
{
	Vector vecAB = polygon.A.Create(polygon.B)	.SetZ(0.f);	// �O�p�`�̕ӂɉ������x�N�g��
	Vector vecAP = polygon.A.Create(point)		.SetZ(0.f);	// �e���_����Ώۂ܂ł̃x�N�g��

	Vector vecBC = polygon.B.Create(polygon.C)	.SetZ(0.f);	// �O�p�`�̕ӂɉ������x�N�g��
	Vector vecBP = polygon.B.Create(point)		.SetZ(0.f);	// �e���_����Ώۂ܂ł̃x�N�g��

	Vector vecCA = polygon.C.Create(polygon.A)	.SetZ(0.f);	// �O�p�`�̕ӂɉ������x�N�g��
	Vector vecCP = polygon.C.Create(point)		.SetZ(0.f);	// �e���_����Ώۂ܂ł̃x�N�g��

	// �Ή�����x�N�g���ǂ����ĊO�ς��v�Z����
	Vector vecABAP = vecAB ^ vecAP;
	Vector vecBCBP = vecBC ^ vecBP;
	Vector vecCACP = vecCA ^ vecCP;

	// �e�O�ς�Z������0�ȉ��ł���Γ������Ă���
	return  (vecABAP.z >= 0.f && vecBCBP.z >= 0.f && vecCACP.z >= 0.f) || 
			(vecABAP.z <= 0.f && vecBCBP.z <= 0.f && vecCACP.z <= 0.f);
}

bool Collision::IsHitTriangleXZ(const Vector& point, const POLYGON& polygon)
{
	Vector vecAB = polygon.A.Create(polygon.B)	.SetY(0.f);	// �O�p�`�̕ӂɉ������x�N�g��
	Vector vecAP = polygon.A.Create(point)		.SetY(0.f);	// �e���_����Ώۂ܂ł̃x�N�g��

	Vector vecBP = polygon.B.Create(point)		.SetY(0.f);	// �O�p�`�̕ӂɉ������x�N�g��
	Vector vecBC = polygon.B.Create(polygon.C)	.SetY(0.f);	// �e���_����Ώۂ܂ł̃x�N�g��
	
	Vector vecCA = polygon.C.Create(polygon.A)	.SetY(0.f);	// �O�p�`�̕ӂɉ������x�N�g��
	Vector vecCP = polygon.C.Create(point)		.SetY(0.f);	// �e���_����Ώۂ܂ł̃x�N�g��

	// �Ή�����x�N�g���ǂ����ĊO�ς��v�Z����
	Vector vecABAP = vecAB ^ vecAP;
	Vector vecBCBP = vecBC ^ vecBP;
	Vector vecCACP = vecCA ^ vecCP;

	// �e�O�ς�Y������0�ȉ��ł���Γ������Ă���
	return  (vecABAP.y >= 0.f && vecBCBP.y >= 0.f && vecCACP.y >= 0.f) || 
			(vecABAP.y <= 0.f && vecBCBP.y <= 0.f && vecCACP.y <= 0.f);
}

bool Collision::IsHitTriangleYZ(const Vector& point, const POLYGON& polygon)
{
	Vector vecAB = polygon.A.Create(polygon.B)	.SetX(0.f);	// �O�p�`�̕ӂɉ������x�N�g��
	Vector vecAP = polygon.A.Create(point)		.SetX(0.f);	// �e���_����Ώۂ܂ł̃x�N�g��
	
	Vector vecBC = polygon.B.Create(polygon.C)	.SetX(0.f);	// �O�p�`�̕ӂɉ������x�N�g��
	Vector vecBP = polygon.B.Create(point)		.SetX(0.f);	// �e���_����Ώۂ܂ł̃x�N�g��
	
	Vector vecCA = polygon.C.Create(polygon.A)	.SetX(0.f);	// �O�p�`�̕ӂɉ������x�N�g��
	Vector vecCP = polygon.C.Create(point)		.SetX(0.f);	// �e���_����Ώۂ܂ł̃x�N�g��

	// �Ή�����x�N�g���ǂ����ĊO�ς��v�Z����
	Vector vecABAP = vecAB ^ vecAP;
	Vector vecBCBP = vecBC ^ vecBP;
	Vector vecCACP = vecCA ^ vecCP;

	// �e�O�ς�X������0�ȉ��ł���Γ������Ă���
	return  (vecABAP.x >= 0.f && vecBCBP.x >= 0.f && vecCACP.x >= 0.f) || 
			(vecABAP.x <= 0.f && vecBCBP.x <= 0.f && vecCACP.x <= 0.f);
}
//�{�b�N�X�ƃ{�b�N�X
bool Collision::isHitBox(const BoxCollision* boxA, const BoxCollision* boxB)
{
	Vector a_half_size = boxA->GetSize() * 0.5;
	Vector b_half_size = boxB->GetSize() * 0.5;

	Vector a_min = boxA->GetPos() - a_half_size;
	Vector a_max = boxA->GetPos() + a_half_size;

	Vector b_min = boxB->GetPos() - b_half_size;
	Vector b_max = boxB->GetPos() + b_half_size;

	return (a_min.x < b_max.x&& a_max.x > b_min.x &&
			a_min.y < b_max.y&& a_max.y > b_min.y &&
			a_min.z < b_max.z&& a_max.z > b_min.z);
}

//�{�b�N�X�Ɠ_
bool Collision::isHitBoxDot(const BoxCollision* box, const Vector& dot)
{
	Vector box_half_size = box->GetSize() * 0.5;

	Vector box_min = box->GetPos() - box_half_size;
	Vector box_max = box->GetPos() + box_half_size;

	return (box_min.x < dot.x&& box_max.x > dot.x &&
			box_min.y < dot.y&& box_max.y > dot.y &&
			box_min.z < dot.z&& box_max.z > dot.z);
}

//�{�b�N�X�ƃx�N�g��
bool Collision::isHitBoxVector(const BoxCollision* box, const VectorCollision* vector)
{
	Vector pos = vector->GetPos() + (vector->GetVector().Normalize() * vector->GetPos().Create(box->GetPos()).Long());

	SphereCollision temp_vec(pos, vector->GetRadius());
	return isHitBoxSphere(box, &temp_vec);
}

//������
bool Collision::isHitBoxModel(const BoxCollision* box, ModelCollision* model)
{
	return false;
}

//�{�b�N�X�Ƌ�
bool Collision::isHitBoxSphere(const BoxCollision* box, const SphereCollision* sphere)
{
	const Vector box_pos = box->GetPos();
	const Vector box_size = box->GetSize();

	const Vector sphere_pos = sphere->GetPos();
	const float sphere_diameter = sphere->GetRadius() * 2;

	//X�������̔��a�����E�ɐL�΂�����`�Ƌ��̒��S�̓����蔻��
	const BoxCollision tump_x(box_pos, Vector(box_size.x + sphere_diameter, box_size.y, box_size.z));
	if (isHitBoxDot(&tump_x, sphere_pos)) return true;

	//Y�������̔��a���㉺�ɐL�΂�����`�Ƌ��̒��S�̓����蔻��
	const BoxCollision tump_y(box_pos, Vector(box_size.x, box_size.y + sphere_diameter, box_size.z));
	if (isHitBoxDot(&tump_y, sphere_pos)) return true;

	//Z�������̔��a���O��ɐL�΂�����`�Ƌ��̒��S�̓����蔻��
	const BoxCollision tump_z(box_pos, Vector(box_size.x, box_size.y, box_size.z + sphere_diameter));
	if (isHitBoxDot(&tump_z, sphere_pos)) return true;

	const Vector box_half_size = box_size * 0.5;

	const bool is_right = box_pos.x < sphere_pos.x;
	const bool is_top	= box_pos.y < sphere_pos.y;
	const bool is_deep	= box_pos.z < sphere_pos.z;

	//���Ƌ�`�̈�ԋ߂����_�Ƃ̓����蔻��
	return isHitSphereDot(sphere, Vector(
			box_pos.x + (is_right)	? box_half_size.x : -box_half_size.x,
			box_pos.y + (is_top)	? box_half_size.y : -box_half_size.y,
			box_pos.z + (is_deep)	? box_half_size.z : -box_half_size.z));
}

//���Ƌ�
bool Collision::isHitSphere(const SphereCollision* sphereA, const SphereCollision* sphereB)
{
	return ((sphereA->GetPos() - sphereB->GetPos()).LongSq() <= MyMath::Squared(sphereA->GetRadius() + sphereB->GetRadius()));
}

//���Ɠ_
bool Collision::isHitSphereDot(const SphereCollision* sphere, const Vector& dot)
{
	return ((sphere->GetPos() - dot).LongSq() <= MyMath::Squared(sphere->GetRadius()));
}

//���ƃx�N�g��
bool Collision::isHitSphereVector(const SphereCollision* sphere, const VectorCollision* vector)
{
	Vector V = vector->GetVector();
	Vector R = vector->GetPos() - sphere->GetPos();
	double radius = MyMath::Squared(sphere->GetRadius() + vector->GetRadius());
	Vector C = R - (V * (R | V));

	return (C.LongSq() - radius < 0.f);
}
//���쒆
bool Collision::isHitSphereModel(const SphereCollision* sphere, ModelCollision* model)
{
	typedef HitPolygonList::HitPolygon hit_polygon;
	typedef HitPolygonList::HITTYPE hit_type;

	// �|���S�������擾
	const MV1_REF_POLYGONLIST& polygons = model->GetPolygonsList();
	auto sphere_pos = sphere->GetPos();

	//�R���W�������Ƃ�͈͂̐ݒ�
	const Vector min_range(sphere_pos.x - COLL_RANGE.x, sphere_pos.y - COLL_RANGE.y, sphere_pos.z - COLL_RANGE.z);
	const Vector max_range(sphere_pos.x + COLL_RANGE.x, sphere_pos.y + COLL_RANGE.y, sphere_pos.z + COLL_RANGE.z);

	//�͈͓����𔻒肷��֐�
	static auto is_within_range = [](const Vector& dot, const Vector& min, const Vector& max) {
		return	(min.x <= dot.x&& max.x >= dot.x)&&
				(min.y <= dot.y&& max.y >= dot.y)&&
				(min.z <= dot.z&& max.z >= dot.z);
	};
	//���������|���S������ۑ����郊�X�g
	HitPolygonList HitList(sphere->GetOwner());

	const MV1_REF_POLYGON* poingons_ptr = polygons.Polygons;
	for (int i = 0; i < polygons.PolygonNum; ++i, ++poingons_ptr) {
		auto ptr = poingons_ptr->VIndex;
		POLYGON vertexs(polygons.Vertexs[(ptr[0])].Position,
						polygons.Vertexs[(ptr[1])].Position,
						polygons.Vertexs[(ptr[2])].Position);
		//���_�S�Ă��͈͊O�Ȃ���s���Ȃ�
		if (!is_within_range(vertexs.A, min_range, max_range) &&
			!is_within_range(vertexs.B, min_range, max_range) &&
			!is_within_range(vertexs.C, min_range, max_range))
			continue;
		//�|���S���̖@��
		Vector norm = MyMath::GetPolygonNormalVec(vertexs);
		if (norm.y >= MAP_FLOOR_NORM_Y_MIN) {

			bool	sphere_hit = IsHitTriangleXZ(sphere_pos, vertexs), 
					closer_hit = IsHitTriangleXZ(sphere_pos + (-norm * sphere->GetRadius()), vertexs);
			
			if (!sphere_hit && !closer_hit)	continue;

			HitList.HitPolygons_.push_back(hit_polygon(hit_type::Floor, vertexs));
		}
		else if (norm.y <= MAP_CEIL_NORM_Y) {
			if (!IsHitTriangleXZ(sphere_pos, vertexs)) continue;

			HitList.HitPolygons_.push_back(hit_polygon(hit_type::Ceil, vertexs));
		}
		else {
			if (MyMath::Abs(norm.z) >= MAP_WALL_NORM_Z_MIN) {
				if (!IsHitTriangleXY(sphere_pos, vertexs)) continue;

				HitList.HitPolygons_.push_back(hit_polygon(hit_type::xy_wall, vertexs));
			}
			else {
				if (!IsHitTriangleYZ(sphere_pos, vertexs)) continue;

				HitList.HitPolygons_.push_back(hit_polygon(hit_type::yz_wall, vertexs));
			}
		}
	}//for (int i = 0; i < polygons.PolygonNum; i++)

	if (HitList.HitPolygons_.size() > 0) {
		ScopedCS cs(model->GetCriticalSection());
		model->AddHitPolygonList(HitList);
		return true;
	}

	return false;
}

//�x�N�g���ƃx�N�g��
bool Collision::isHitVector(const VectorCollision* vectorA, const VectorCollision* vectorB)
{
	Vector v12 = vectorB->GetPos() - vectorA->GetPos();
	Vector m = (vectorA->GetVector() ^ vectorB->GetVector()).Normalize();
	float radius = vectorA->GetRadius() + vectorB->GetRadius();

	return (v12 | m) <= radius;
}

bool Collision::isHitVectorModel(const VectorCollision* vector, ModelCollision* model)
{
	return static_cast<bool>(MV1CollCheck_Line(model->GetHandle(), -1, vector->GetPos(), vector->GetPos() + (vector->GetVector() * vector->GetLong())).HitFlag);
}

//������
bool Collision::isHitModel(ModelCollision* modelA, ModelCollision* modelB)
{
	return false;
}
