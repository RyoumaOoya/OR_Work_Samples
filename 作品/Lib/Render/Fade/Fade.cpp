#include "Fade.h"
#include "DxLib.h"
#include "GameSystem/Common/Common.h"
#include "../../Constant/Constant.h"
#include "../BlendData/ScopedBlend.h"
#include "../../Time/Time.h"
#include "../../Math/MyMath.h"
#include "../../GameSystem/GameSystem.h"

constexpr auto PARAM_MAX = 255.f;

void Fade::Init()
{
}

// フェード更新
void Fade::Step()
{
	if (Status == FADE_STATUS::Out) {
		Param = Interpolation.Interpolate(CTime::GetCountSecf(Time), 0.f, 255.f);
		if (Param >= PARAM_MAX) {
			Param = PARAM_MAX;
			Status = FADE_STATUS::End;
		}
	}
	else if (Status == FADE_STATUS::In) {
		Param = MyMath::Abs(PARAM_MAX - Interpolation.Interpolate(CTime::GetCountSecf(Time), 0.f, 255.f));
		if (Param <= 0.f) {
			Param = 0.f;
			Status = FADE_STATUS::End;
		}
	}
}

// フェード描画
void Fade::Draw()
{
	const auto window = GameSystem::GetWindowSize();
	ScopedBlend sb(static_cast<int>(BLEND_MODE::ALPHA), static_cast<int>(Param));
	DrawBox(0, 0, window.x, window.y, Constant::Color(0, 0, 0), true);
}

void Fade::Fin()
{
}

// フェードイン開始
void Fade::FadeIn(float time, Interpolation::KIND kind)
{
	Interpolation.SetKind(kind);
	Interpolation.SetTotalTime(time);
	Time = CTime::StartCountSecf();
	Param = PARAM_MAX;
	Status = FADE_STATUS::In;
}

// フェードアウト開始
void Fade::FadeOut(float time, Interpolation::KIND kind)
{
	Interpolation.SetKind(kind); 
	Interpolation.SetTotalTime(time);
	Time = CTime::StartCountSecf();
	Param = 0;
	Status = FADE_STATUS::Out;
}