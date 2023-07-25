#include "Title.h"
#include "SceneParameter.h"
#include "ManagerManager.h"
#include "GameSystem/GameSystem.h"
#include "GameSystem/Common/Common.h"
#include "Constant/Constant.h"
#include "Input/InputManager.h"
#include "UI/GameUIManager.h"
#include "UI/GameUIParameter.h"
#include "UI/GameUISprite.h"

namespace {
	enum class TITLE_UI_TYPE{
		BG,
	};
#define ITITLE_UI_TYPE (int)TITLE_UI_TYPE
	struct UIPositioningData {
		constexpr UIPositioningData() :T_UI_TYPE(0), Type(-1), Path(), Pos() {}
		constexpr UIPositioningData(const int t_ui_type, const int type, const char* path, const Vector2D pos) : T_UI_TYPE(t_ui_type),Type(type), Path(path), Pos(pos) {}

		int T_UI_TYPE;
		int Type;
		CONSTANT_STRING(64) Path;
		Vector2D Pos;
	};
	CONSTANT_ARRAY(UIPositioningData, 16) UI_POSITIONING_DATA = {
		UIPositioningData(ITITLE_UI_TYPE::BG, IUI_TYPE::SPRITE, "Data/BackGround/Title_BG02.png", {0.f, 0.f}),
	};

	int Handle = -1;
}

Title::Title()
{
}

Title::~Title()
{
}

void Title::Init()
{
	ManagerManager::AddManager(GameUIManager::CreateInstance());
	ManagerManager::Init();
}

void Title::Load()
{
	ManagerManager::Load();

	Handle = LoadGraph("Data/Ui/Circle.png");
}

void Title::OnLoadEnd()
{
	DxLib::WaitHandleASyncLoadAll();
	ManagerManager::OnLoadEnd();
	auto ui_manager = GameUIManager::GetInstance();
	for (auto& ui_data : UI_POSITIONING_DATA) {
		if (ui_data.Type == -1)
			break;
		auto ui = ui_manager->CreateUI(ui_data.Type, (ui_data.Type == ITITLE_UI_TYPE::BG) ? static_cast<const Vector2D>(GameSystem::GetWindowSize() * 0.5) : ui_data.Pos, ui_data.Path.data());
		switch (static_cast<TITLE_UI_TYPE>(ui_data.T_UI_TYPE))
		{
		case TITLE_UI_TYPE::BG:	static_cast<Pointer<GameUISprite>>(ui)->SetWindowSize();	break;
		default: break;
		}
	}
	FadeIn();
}

void Title::Step()
{
	__super::Step();
	ManagerManager::Step();
	ManagerManager::ConflictStep();
	if (CInputManager::Input("Enter",INPUT_TYPE::Release)) {
		ChangeScene(ISCENE_ID::PLAY);
	}
}

void Title::UpDate()
{
	ManagerManager::Update();
}

void Title::Draw()
{
	ManagerManager::Draw();
}

void Title::FrontDraw()
{
	ManagerManager::FrontDraw();
}

void Title::Fin()
{
	ManagerManager::Fin();
	DeleteGraph(Handle);
}
