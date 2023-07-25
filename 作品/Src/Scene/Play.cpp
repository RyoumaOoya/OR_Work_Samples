#include "Play.h"
#include "SceneManager.h"
#include "SceneParameter.h"
#include "DxLib.h"
#include "Constant/Constant.h"
#include "ManagerManager.h"
#include "Input/InputManager.h"
#include "GameSystem/Thread/ThreadManager.h"
#include "Sound/SoundManager.h"
#include "Render/Render.h"
#include "Message/Message.h"
#include "Src/Camera/CameraManager.h"
#include "Src/Camera/CameraParameter.h"
#include "Src/Player/PlayerManager.h"
#include "Src/Stage/StageManager.h"
#include "Src/Produce/ProduceManager.h"
#include "Src/Item/ItemManager.h"
#include "Src/Enemy/EnemyManager.h"
#include "Src/Bullet/BulletManager.h"

namespace
{
	CONSTEXPR_STRING(64) GarbageCollection("GarbageCollection");
}

Play::Play() 
{
}

Play::~Play()
{
	Fin();
}

void Play::Init()
{
	//Collision,Sound‚ÍÅ‰‚É‰Šú‰»‚·‚é
	auto collision_manager = CollisionManager::CreateInstance();
	collision_manager->Init();
	auto sound_manager = SoundManager::CreateInstance();
	sound_manager->Init();
	sound_manager->Load();

	ManagerManager::AddManager({ CameraManager::CreateInstance(), PlayerManager::CreateInstance(), EnemyManager::CreateInstance(),
		ItemManager::CreateInstance(), StageManager::CreateInstance(), ProduceManager::CreateInstance(),BulletManager::CreateInstance() });

	ManagerManager::Init();
	ManagerManager::AddManager(collision_manager);
	//Render::SetNearShadowDrawArea([]() { return PlayerManager::GetInstance()->GetPlayer()->GetPos(); });
}

void Play::Load()
{
	ManagerManager::Load();
	ManagerManager::AddManager(SoundManager::GetInstance());
}

void Play::OnLoadEnd()
{
	DxLib::WaitHandleASyncLoadAll();
	ManagerManager::OnLoadEnd();
	Render::WriteStaticShadow();
	FadeIn();
}

void Play::Step()
{
#if _DEBUG
	auto cameraMng = CameraManager::GetInstance();
	auto cameraID = cameraMng->GetCamera()->GetID();
	if (GET_KEY->Input(KEY_INPUT_K, INPUT_TYPE::Release)) {
		cameraMng->StartCamera((cameraID == CameraID::PLAY) ? ICameraID::DEBUG : ICameraID::PLAY);
	}

	if (cameraID == CameraID::DEBUG) {
		cameraMng->Step();
		return;
	}

#endif // _DEBUG
	__super::Step();

	ManagerManager::Step();

	ThreadManager::WaitTask();

	ManagerManager::ConflictStep();

	if (GET_KEY->Input(KEY_INPUT_G, INPUT_TYPE::Release)) {
		//”O‚Ì‚½‚ß
		ThreadManager::WaitTask();
		Allocator::GarbageCollection();
	}

	if (!PlayerManager::GetInstance()->GetPlayer()->IsActive() || CInputManager::GetJoyPad()->Input(PAD_INPUT_START, INPUT_TYPE::Release))
		ChangeScene(ISCENE_ID::TATLE);
}

void Play::UpDate()
{
	ManagerManager::Update();
	ThreadManager::WaitTask();
}

void Play::Draw()
{
	ManagerManager::Draw();
}

void Play::FrontDraw()
{
	ManagerManager::FrontDraw();
	CollisionManager::GetInstance()->FrontDraw();
}

void Play::Fin()
{
	Render::SetNearShadowDrawArea(nullptr);
	ManagerManager::Fin();
}
