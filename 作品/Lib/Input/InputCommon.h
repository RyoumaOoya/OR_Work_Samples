#pragma once
#include <DxLib.h>
#include "../typedef.h"
#include <vector>
#include <functional>

#define KEY_INPUT_NULL (-1)
#define MOUSE_INPUT_NULL (0)
#define PAD_INPUT_NULL (0)
namespace MyLib
{
	struct ConstBind {
	public:
		constexpr ConstBind() :keyCode(0), mouseCode(0), joypadCode(0) {};
		constexpr ConstBind(u32 KeyCode, u32 MouseCode, u32 JoyPadCode) :keyCode(KeyCode), mouseCode(MouseCode), joypadCode(JoyPadCode) {}

		u32 keyCode;
		u32 mouseCode;
		u32 joypadCode;
	};

	struct Bind {
	public:
		Bind() :keyCode(0), mouseCode(0), joypadCode(0) {};
		Bind(u32 KeyCode, u32 MouseCode, u32 JoyPadCode) :keyCode(KeyCode), mouseCode(MouseCode), joypadCode(JoyPadCode) {}
		Bind(const ConstBind& bind) :keyCode(bind.keyCode), mouseCode(bind.mouseCode), joypadCode(bind.joypadCode) {}

		u32 keyCode;
		u32 mouseCode;
		u32 joypadCode;
	public:

		alignas(64) std::function<void()> pushFunc = nullptr;
		alignas(64) std::function<void()> keepFunc = nullptr;
		alignas(64) std::function<void()> releaseFunc = nullptr;
		alignas(64) std::function<void()> downFunc = nullptr;
	};

	namespace Size {
		constexpr size_t BIND = sizeof(Bind);
	}

	enum class INPUT_TYPE
	{
		Null = -1,
		Push,
		Keep,
		Release,
		Down,

		NUM
	};
}