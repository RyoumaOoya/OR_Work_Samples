#include "Mouse.h"
#include "DxLib.h"
#include "../../Define.h"
#include "../../Math/MyMathData.h"
#include "../../GameSystem/FrameRate/FrameRate.h"

namespace MyLib
{
	namespace {
		constexpr MData::Vector2D<int> RangeDefault(10000, 10000);
	}

	CMouseInput::CMouseInput()
	{
		MainKeyBuf_ = 0x00;
		CurrentBuf_ = 0x00;
		PreBuf_ = 0x00;
		OidMousePos_(0, 0);
		CurrentMousePos_(0, 0);
		MainMousePos_(0, 0);
		Range_[0] = -RangeDefault;
		Range_[1] = RangeDefault;
		FixationTime_ = 0.0;
		MouseWheelVol_ = 0;
	}

	CMouseInput::~CMouseInput()
	{

	}

	bool CMouseInput::Input(u32 mouseCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE Type) const
	{
		switch (Type)
		{
		case INPUT_TYPE::Push:	 return ((PreBuf_ & mouseCode) == 0 && (CurrentBuf_ & mouseCode) != 0);
		case INPUT_TYPE::Keep:	 return ((PreBuf_ & mouseCode) != 0 && (CurrentBuf_ & mouseCode) != 0);
		case INPUT_TYPE::Release: return ((PreBuf_ & mouseCode) != 0 && (CurrentBuf_ & mouseCode) == 0);
		case INPUT_TYPE::Down:	 return ((CurrentBuf_ & mouseCode) != 0);
		default:				 return false;
		}
	}

	INPUT_TYPE CMouseInput::Input(u32 mouseCode) const
	{
		return  ((PreBuf_ & mouseCode) != 0 && (CurrentBuf_ & mouseCode) != 0) ? INPUT_TYPE::Keep :
			((PreBuf_ & mouseCode) == 0) ? INPUT_TYPE::Push :
			((CurrentBuf_ & mouseCode) == 0) ? INPUT_TYPE::Release :
			INPUT_TYPE::Null;
	}

	i32 CMouseInput::AnythingInput(_In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE type) const
	{
		FOR(Index, 32) {
			int mouse_code = 1 << Index;
			if (Input(mouse_code, type))
				return mouse_code;
		}

		return -1;
	}

	void CMouseInput::SetMousePos(iVector2D Pos, double fixationTime)
	{
		FixationMousePos_ = Pos;
		SetMousePoint(Pos.x, Pos.y);
		CurrentMousePos_ = Pos;
		FixationTime_ = fixationTime;
	}

	void CMouseInput::SetMouseMoveRange(iVector2D min, iVector2D max)
	{
		if (min.x < max.x) {
			Range_[0].x = min.x;
			Range_[1].x = max.x;
		}
		else {
			Range_[0].x = max.x;
			Range_[1].x = min.x;
		}
		if (min.y < max.y) {
			Range_[0].y = min.y;
			Range_[1].y = max.y;
		}
		else {
			Range_[0].y = max.y;
			Range_[1].y = min.y;
		}
	}

	void CMouseInput::InitMouseMoveRange()
	{
		Range_[0] = -RangeDefault;
		Range_[1] = RangeDefault;
	}

	void CMouseInput::Init()
	{
		CurrentBuf_ = 0x00;
		PreBuf_ = 0x00;
		CurrentMousePos_(0, 0);
	}

	void CMouseInput::Step()
	{
		PreBuf_ = CurrentBuf_;
		//‚·‚×‚Ä‚Ìƒ}ƒEƒXî•ñ‚ðŽæ“¾
		CurrentBuf_ = MainKeyBuf_;

		OidMousePos_ = CurrentMousePos_;
		CurrentMousePos_ = MainMousePos_;

		OffSetMousePos_ = CurrentMousePos_ - OidMousePos_;

		/*if (FixationTime_ > 0.0) {
			FixationTime_ -= (double)CFrameRate::GetDelteTime();
		}*/

		MouseWheelVol_ = GetMouseWheelRotVol();

		AlwaysStep();
	}

	void CMouseInput::MainStep()
	{
		MainKeyBuf_ = GetMouseInput();
		GetMousePoint(&MainMousePos_.x, &MainMousePos_.y);
	}

	void CMouseInput::AlwaysStep()
	{
		if (CurrentMousePos_.x < Range_[0].x) {
			SetMousePos(iVector2D(Range_[0].x, CurrentMousePos_.y));
		}
		else if (CurrentMousePos_.x > Range_[1].x) {
			SetMousePos(iVector2D(Range_[1].x, CurrentMousePos_.y));
		}
		if (CurrentMousePos_.y < Range_[0].y) {
			SetMousePos(iVector2D(CurrentMousePos_.x, Range_[0].y));
		}
		else if (CurrentMousePos_.y > Range_[1].y) {
			SetMousePos(iVector2D(CurrentMousePos_.x, Range_[1].y));
		}

		if (FixationTime_ > 0.0) {
			SetMousePoint(FixationMousePos_.x, FixationMousePos_.y);
			CurrentMousePos_ = FixationMousePos_;
		}
	}

	void CMouseInput::Fin()
	{

	}
}