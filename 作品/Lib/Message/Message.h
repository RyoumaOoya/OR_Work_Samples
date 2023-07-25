#pragma once
#include "DxLib.h"
#include "../Constant/Constant.h"
#include "../typedef.h"

namespace MyLib
{
	//画面上にログを表示するクラス
	class Message
	{
	public:
		static constexpr size_t text_len = 8;
	public:
		struct TextData {
		public:
			constexpr TextData() :DrawTime(0.f), Color(color::Black) { }
			/*描画時間, 色, 描画内容*/
			TextData(const float time, unsigned __int32 color, const char* text) :DrawTime(time), Color(color) { strcpy_s(Text, text); }
		public:
			float DrawTime;
			u32 Color;
			char Text[64] = { 0 };
		};
	public:
		Message() = delete;
		~Message() = delete;

	public:
		//描画するテキストの追加
		static void Push(const TextData& data);
		//描画するテキストの追加
		static void Push(TextData&& data);
		static void Step();
		static void Draw();

	private:
		static inline TextData Text[text_len];
	};
}