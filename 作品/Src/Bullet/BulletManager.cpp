#include "BulletManager.h"
#include "NormalBullet.h"
#include "OrbitingBullet.h"
BulletManager::BulletManager()
{
	Index_ = MANAGER_INDEX::MultiThread;
}

BulletManager::~BulletManager()
{
}

void BulletManager::Init()
{
#define  BULLET_PROTOTYPE(class_name) MyNew(class_name, AllocType::Prototype, Tags(MemTag::Prototype))
	Prototypes.push_back(BULLET_PROTOTYPE(NormalBullet));
	Prototypes.push_back(BULLET_PROTOTYPE(OrbitingBullet));
#undef  BULLET_PROTOTYPE
}

void BulletManager::Load()
{
	for (auto bullet : Prototypes) {
		bullet->Load();
	}
}

void BulletManager::Step()
{
	for (auto bullet : Bullets) {
		bullet->Step();
	}
}

void BulletManager::DeleteStep()
{
	for (auto itr = Bullets.begin(); itr != Bullets.end();) {
		auto enemy = *itr;
		if (enemy->RenderHasTag(RenderTags::isDeleted)) {
			MyFree(enemy);
			itr = Bullets.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void BulletManager::UpDate()
{
	for (auto bullet : Bullets) {
		bullet->UpDate();
	}
}

void BulletManager::FrontDraw()
{
}

void BulletManager::Delete()
{
	for (auto itr = Bullets.begin(); itr != Bullets.end();) {
		MyFree(*itr);
		itr = Bullets.erase(itr);
	}
	for (auto itr = Prototypes.begin(); itr != Prototypes.end();) {
		MyFree(*itr);
		itr = Prototypes.erase(itr);
	}
}

void BulletManager::Fin()
{
	Delete();
}

void BulletManager::Shot(SHOT_ID ID, const Vector& ShooterPos, float angle)
{
	decltype(&BulletManager::SingleShot) ShotFuncs[ISHOT_ID::NUM] = {
		&BulletManager::SingleShot,
		&BulletManager::DoubleShot,
		&BulletManager::TripleShot,
		&BulletManager::OrbitingShot,
	};
	(this->*ShotFuncs[static_cast<int>(ID)])(ShooterPos, angle);
}

Pointer<BulletBase> BulletManager::CreateBullet(BULLET_ID id)
{
	auto ptr = Prototypes[static_cast<int>(id)]->Clone();
	Bullets.push_back(ptr);
	ptr->ChangeColor();
    return ptr;
}

void BulletManager::SingleShot(const Vector& ShooterPos, float angle)
{
	auto bullet = CreateBullet(BULLET_ID::NORMAL);
	bullet->SetPos(ShooterPos);
	bullet->SetMove(Vector(0, 0, 1).Rotation(Vector(0, angle, 0)));
}

void BulletManager::DoubleShot(const Vector& ShooterPos, float angle)
{
	constexpr float offset = 7.5_DegreeToRadian_F;
	auto left = CreateBullet(BULLET_ID::NORMAL), right = CreateBullet(BULLET_ID::NORMAL);
	left->SetPos(ShooterPos);
	right->SetPos(ShooterPos);

	left->SetMove(Vector(0, 0, 1).Rotation(Vector(0, angle - offset, 0)));
	right->SetMove(Vector(0, 0, 1).Rotation(Vector(0, angle + offset, 0)));
}

void BulletManager::TripleShot(const Vector& ShooterPos, float angle)
{
	constexpr float offset = 15_DegreeToRadian_F;
	auto left = CreateBullet(BULLET_ID::NORMAL), right = CreateBullet(BULLET_ID::NORMAL), middle = CreateBullet(BULLET_ID::NORMAL);
	middle->SetPos(ShooterPos);
	left->SetPos(ShooterPos);
	right->SetPos(ShooterPos);

	middle->SetMove(Vector(0, 0, 1).Rotation(Vector(0, angle, 0)));
	left->SetMove(Vector(0, 0, 1).Rotation(Vector(0, angle - offset, 0)));
	right->SetMove(Vector(0, 0, 1).Rotation(Vector(0, angle + offset, 0)));
}

void BulletManager::OrbitingShot(const Vector& ShooterPos, float angle)
{
	auto bullet = static_cast<Pointer<OrbitingBullet>>(CreateBullet(BULLET_ID::ORBITING));
	bullet->SetPos(ShooterPos);
	bullet->SetRot(angle);
}
