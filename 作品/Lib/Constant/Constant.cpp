#include "Constant.h"

void DxLib::SetFogColor(const MyLib::Color& c)
{
	SetFogColor(c.r, c.g, c.b);
}

MyLib::Color DxLib::GetFogColor()
{
	int red, blue, green;
	GetFogColor(&red, &green, &blue);
	return MyLib::Color(static_cast<u8>(red), static_cast<u8>(blue), static_cast<u8>(green));
}
