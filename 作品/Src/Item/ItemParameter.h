#pragma once
#include "Constant/Constant.h"

enum class ITEM_ID
{
	Enhance,
	OrbitingBullet,
	Num
};

#define IITEM_ID (int)ITEM_ID

struct ItemData {
public:
	constexpr ItemData(const char* path, Color c, Color e) :Path(path), color(c), emiColor(e) {}

	CONSTANT_STRING(64) Path;
	Color color;
	Color emiColor;
};

CONSTANT_ARRAY(ItemData, IITEM_ID::Num) ITEM_DATA = {
	ItemData("Data/Model/Item.x", color::Cyan, color::White),
	ItemData("Data/Model/Item.x", color::BlueViolet, color::White),
};