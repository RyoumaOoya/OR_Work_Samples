#include "Player.h"
#include "PlayerParameter.h"
#include "PlayerManager.h"
#include "typedef.h"
#include "Input/InputManager.h"
#include "Constant/Constant.h"
#include "Time/Time.h"
#include "Message/Message.h"
#include "Math/MyMath.h"
#include "Render/Render.h"
#include "Sound/SoundManager.h"
#include "Sound/SoundEffect.h"
#include "Sound/SoundParameter.h"
#include "GameSystem/GameSystem.h"
#include "GameSystem/FrameRate/FrameRate.h"
#include "GameSystem/Common/Common.h"
#include "Src/Collision/SphereCollision.h"
#include "Src/Camera/CameraManager.h"
#include "Src/Item/ItemManager.h"
#include "Src/Enemy/EnemyManager.h"
#include "Src/AI/AIBase.h"
#include "Src/Stage/StageManager.h"
#include "Src/Bullet/BulletManager.h"
#include <float.h>

using namespace PLAYER_PARAMETER;
namespace
{
	Vector circle_pos;
}

Player::Player() : Collision_()
{
	RenderObject_.Scale_ = PLAYER_SCALE;
	Flags_ = 0x00;
	MovePower_ = 0.0;
	SetActive(true);
	TimeReceivedDamageLast_ = 0.f;
	HP_ = PLAYER_MAX_HP;
	isDamage_ = false;
	isInvincible_ = false;
	Shot = SHOT_ID::SINGLE;
	ShotInterval_ = 0.f;
	SetTag(OBJECT_TAGS::PLAYER);

	auto pad = GET_PAD;
	auto key = GET_KEY;

	//関数ポインタを利用して結果が変わらないif文を減らす
	if (pad && key) {
		MoveInput = [](Pointer<Player> ply, double delteTime, float y) {
			Vector move_result, forad_vec = CameraManager::GetInstance()->GetCamera()->GetCameraLookVec().SetY(0.f).Normalize();
			move_result = ply->PadMove(forad_vec, delteTime);
			if (move_result.isZeroVector()) { move_result = ply->KeyMove(forad_vec, delteTime); }
			move_result.y = y;
			return move_result;
		};
	}
	else if (pad) {
		MoveInput = [](Pointer<Player> ply, double delteTime, float y) {
			Vector move_result, forad_vec = CameraManager::GetInstance()->GetCamera()->GetCameraLookVec().SetY(0.f).Normalize();
			move_result = ply->PadMove(forad_vec, delteTime);
			move_result.y = y;
			return move_result;
		};
	}
	else if (key) {
		MoveInput = [](Pointer<Player> ply, double delteTime, float y) {
			Vector move_result, forad_vec = CameraManager::GetInstance()->GetCamera()->GetCameraLookVec().SetY(0.f).Normalize();
			move_result = ply->KeyMove(forad_vec, delteTime);
			move_result.y = y;
			return move_result;
		};
	}
	else {
		MoveInput = [](Pointer<Player> ply, double delteTime, float y) {return Vector::ZEROVEC(); };
	}
}

Player::~Player()
{
	Fin();
}

void Player::OnCollisionBegin(Pointer<Object> other)
{
	if (other->ObjectHasTag(OBJECT_TAGS::ENEMY)) {
		//Death();
	}
	else if (other->ObjectHasTag(OBJECT_TAGS::ITEM)) {
		auto item = static_cast<Pointer<Item>>(other);
		switch (item->GetID())
		{
		case ITEM_ID::Enhance: {
			if (Shot < SHOT_ID::TRIPLE) {
				Shot = static_cast<SHOT_ID>(static_cast<int>(Shot) + 1);
			}
		}
		break;
		case ITEM_ID::OrbitingBullet: {
			BulletManager::GetInstance()->Shot(SHOT_ID::ORBITING, RenderObject_.Pos_, 0.f);
		}
		break;
		}
	}
}

void Player::OnCollisionEnter(Pointer<Object> other)
{
}

void Player::OnCollisionEnd(Pointer<Object> other)
{
}

void Player::Init()
{
	Init(Allocator::GetPointerStorage(this, AllocType::Player));
}

void Player::Init(const Pointer<Player>& this_ptr)
{
	__super::Init();
	SetThisPtr(this_ptr);
	Collision_ = static_cast<Pointer<SphereCollision>>(CollisionManager::GetInstance()->CreateCollision(ICollisionShape::SPHERE, ICollisionCategory::PLAYER));
	Collision_->SetOwner(This_);
	Collision_->SetRadius(PLAYER_RADIUS);
	Anim_.SetOwner(this_ptr);
	DamageSE_ = static_cast<Pointer<SoundEffect>>(SoundManager::GetInstance()->CreateSEHandle(ISE_ID::HIT));
	DamageSE_->SetPosCalc([]() { return GET_PLAYER_POS; });
	PushModelList();
	//const auto window = GameSystem::GetWindowSize();
	//GET_MOUSE->SetMousePos({ window.x >> 1, window.y >> 1 }, DBL_MAX);
}

void Player::Load()
{
	__super::Load(PLAYER_FILE_PATH.data());
}

void Player::Step()
{
	__super::Step();

	Move();
	auto mouse = CInputManager::GetMouse();
	auto map = StageManager::GetInstance()->GetMap();
	circle_pos = map->GetIntersectionWithScreenPos(static_cast<Vector2D>(mouse->GetMousePos()));

	if (CInputManager::Input("Shot",INPUT_TYPE::Down) && CTime::GetCountSecf(ShotInterval_) >= SHOT_INTERVAL) {
		BulletManager::GetInstance()->Shot(Shot, RenderObject_.Pos_, RenderObject_.Pos_.GetFollowRotationXZ(circle_pos));
		ShotInterval_ = CTime::StartCountSecf();
	}

	if (isDamage_) {
		isDamage_ = false;
		TimeReceivedDamageLast_ = CTime::StartCountSecf();
		DamageSE_->Play();
	}
	if (isInvincible_ && CTime::GetCountSecf(TimeReceivedDamageLast_) >= PLAYER_INVINCIBLE) {
		isInvincible_ = false;
	}

	if (HP_ <= 0)
		Death();

	if (RenderObject_.Pos_.y <= Common::Y_LOWER_LIMIT) {
		RenderObject_.Pos_(0.f, 1.f, 0.f);
		Move_.y = 0.f;
	}
}

void Player::ConflictStep()
{
}

void Player::FrontDraw()
{
	u64 i = 0;
	if (!circle_pos.isZeroVector()) {
		DrawSphere3D(circle_pos, 3, 10, color::Black, color::Black, false);
	}
	DrawFormatString(0, operator""_ROW(i++), color::White, "HP %d", HP_);
#ifdef _DEBUG
	DrawFormatString(0, operator""_ROW(i++), color::White, "%.2f, %.2f, %.2f", RenderObject_.Pos_.x, RenderObject_.Pos_.y, RenderObject_.Pos_.z);
	DrawFormatString(0, operator""_ROW(i++), color::White, "%.2f, %.2f, %.2f", Move_.x, Move_.y, Move_.z);
	auto l_vec = GET_PAD->GetLiftStick();
	DrawFormatString(0, operator""_ROW(i++), color::White, "%d, %d", l_vec.x, l_vec.y);
#endif // _DEBUG
}

void Player::Delete()
{
	__super::Delete();
	EreseModelList();
	if(Collision_)
		Collision_->SetState(ICollisionState::NONE);
	if (DamageSE_)
		DamageSE_->SetState(ISoundState::Fin);
	Anim_.Fin();
}

void Player::Fin()
{
	Delete();
	__super::Fin();
}

void Player::Move()
{
	auto key = GET_KEY;
	const auto DelteTime = (FrameRate::GetDelteTime());

	Move_ = MoveInput(static_cast<Pointer<Player>>(This_), DelteTime, Move_.y);
	if (!Move_.isZeroVector()) {
		RenderObject_.Rot_ = static_cast<Vector>(RenderObject_.Pos_.Create(RenderObject_.Pos_ + Vector(Move_.x, 0.f, Move_.z)).GetRotationXY());
	}

	Vector temp(0.f, 0.f, 1.f);
	temp = RenderObject_.Pos_ + (temp.Rotation(RenderObject_.Rot_) * 2.f);

	Move_ += Common::Gravity * (double)DelteTime;

	const float g_max = static_cast<float>(FrameRate::GetDelteTime()) * GRAVITY_SPEED_MAX;

	if (MyMath::Abs(Move_.y) > g_max)
		Move_.y = -g_max;

	RenderObject_.Pos_ += Move_;
}

Vector Player::PadMove(const Vector& foradVec, double delteTime)
{
	auto joypad = GET_PAD;
	//Lスティックを参照
	auto l_stick = joypad->GetLiftStick();
	auto fl_stick = static_cast<Vector2D>(l_stick);
	//nullじゃなかったら
	if (!l_stick.isZeroVector()) {
		////y軸回転値を計算
		//auto rot_y = Vector(fl_stick.x, 0.f, -fl_stick.y).GetRotationXY().y;
		////移動する方向を計算
		//auto move_vec = foradVec.Rotation({ 0.f, rot_y, 0.f }).Normalize();
		//入力の強さ計算
		//move_vec *= (fl_stick * 0.001f).Long();
		//移動量をかける
		fl_stick *= 0.001f * (PLAYER_SPEED * delteTime);
		return Vector(-fl_stick.x, 0.f, fl_stick.y);
	}

	return Vector();
}

Vector Player::KeyMove(const Vector& foradVec, double delteTime)
{
	//インプット情報を取得
	alignas(1) const bool input_wasd[4] = {
		CInputManager::Input("Forward", INPUT_TYPE::Down),
		CInputManager::Input("Lift", INPUT_TYPE::Down),
		CInputManager::Input("Back", INPUT_TYPE::Down),
		CInputManager::Input("Right", INPUT_TYPE::Down),
	};
	//int型にキャストしてbool型配列4つともfalseかを一括で判定している
	if (*reinterpret_cast<const i32*>(input_wasd) != 0) {
		//float y = 0.f;
		////W or Sが入力されているか
		//if (input_wasd[0] != input_wasd[2]) {
		//	y = (input_wasd[0]) ? 0_DegreeToRadian_F : 180_DegreeToRadian_F;
		//	//A or D
		//	if (input_wasd[1] != input_wasd[3]) {
		//		y += (input_wasd[1] ? -90_DegreeToRadian_F : 90_DegreeToRadian_F) * (input_wasd[2] ? -0.5f : 0.5f);
		//	}
		//}
		////A or D
		//else if (input_wasd[1] != input_wasd[3]) {
		//	y += (input_wasd[1]) ? -90_DegreeToRadian_F : 90_DegreeToRadian_F;
		//}
		////前方ベクトルを移動方向に回転して移動量をかける
		//return foradVec.Rotation(Vector(0.0, y, 0.0)).Normalize() * (PLAYER_SPEED * delteTime);
		Vector vec;
		//W or Sが入力されているか
		if (input_wasd[0] != input_wasd[2]) {
			vec.z = input_wasd[0] ? 1.f: -1.f;
		}
		//A or D
		if (input_wasd[1] != input_wasd[3]) {
			vec.x = input_wasd[1] ? -1.f : 1.f;
		}
		return vec.Normalize() * (PLAYER_SPEED * delteTime);
	}

	return Vector();
}

void Player::Death()
{
	SetActive(false);
	SetDeleteFlag();
}
