#pragma once
#include "../Define.h"
#include "../typedef.h"

namespace MyLib
{
	enum class OBJECT_TAGS :u8
	{
		PLAYER,
		ENEMY,
		ITEM,
		STAGE,
		BULLET,

		NUM
	};
	static_assert((int)OBJECT_TAGS::NUM < 32, "Only 32 OBJECT_TAGS can be defined");
	ENUM_OPERATORS(OBJECT_TAGS, u8);

	enum class OBJECT_FLAGS :u8
	{
		Active,
		SetRender,
		Grounding,
		Ceiling,
		Wall,
		
		NUM
	};

	static_assert((int)OBJECT_FLAGS::NUM < 32, "Only 32 OBJECT_FLAGS can be defined");
	ENUM_OPERATORS(OBJECT_FLAGS, u8);
}

#define IOBJECT_TAGS (int)MyLib::OBJECT_TAGS
#define IOBJECT_STATE (int)MyLib::OBJECT_FLAGS

