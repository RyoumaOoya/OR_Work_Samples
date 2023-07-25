#pragma once
#include "DxLib.h"
#include "../Constant/Constant.h"
#include "../typedef.h"

namespace MyLib
{
	//��ʏ�Ƀ��O��\������N���X
	class Message
	{
	public:
		static constexpr size_t text_len = 8;
	public:
		struct TextData {
		public:
			constexpr TextData() :DrawTime(0.f), Color(color::Black) { }
			/*�`�掞��, �F, �`����e*/
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
		//�`�悷��e�L�X�g�̒ǉ�
		static void Push(const TextData& data);
		//�`�悷��e�L�X�g�̒ǉ�
		static void Push(TextData&& data);
		static void Step();
		static void Draw();

	private:
		static inline TextData Text[text_len];
	};
}