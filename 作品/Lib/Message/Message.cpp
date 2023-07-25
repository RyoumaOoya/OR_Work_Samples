#include "Message.h"
#include "../GameSystem/FrameRate/FrameRate.h"
#include "../GameSystem/GameSystem.h"
#include "../Constant/Constant.h"
#include "../GameSystem/Common/Common.h"
#include "../Literal.h"
namespace MyLib
{
	void Message::Push(const TextData& data)
	{
		memmove(&Text[1], Text, sizeof(TextData[text_len - 1]));

		Text[0] = data;
	}

	void Message::Push(TextData&& data)
	{
		memmove(&Text[1], Text, sizeof(TextData[text_len - 1]));

		Text[0] = data;
	}

	void Message::Step()
	{
		TextData* ptr = Text;
		const float elapsed_time = static_cast<float>(FrameRate::GetDelteTime());
		for (int i = 0; i < text_len; i++, ptr++) {
			ptr->DrawTime -= elapsed_time;
			if (ptr->DrawTime <= 0.f)
				ptr->Text[0] = '\0';
		}
	}

	void Message::Draw()
	{
		TextData* ptr = Text;
		const auto WINDOW_SIZE = GameSystem::GetWindowSize();
		const int x = WINDOW_SIZE.x - (WINDOW_SIZE.x >> 2);
		int y = WINDOW_SIZE.y - 1_ROW;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 256 >> 2);
		DrawBox(x, y - operator""_ROW(text_len - 1), WINDOW_SIZE.x, WINDOW_SIZE.y, color::White, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		for (int i = 0; i < text_len; i++, ptr++) {
			DxLib::DrawFormatString(x, y, ptr->Color, ptr->Text);
			y -= 1_ROW;
		}
	}
}