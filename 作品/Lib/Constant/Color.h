#pragma once
#include "Constant.h"

//適当に増やす
namespace MyLib
{
	namespace color {
#define COLOR(name, code) constexpr Color name = code
		COLOR(IndianRed, 0xCD5C5C);	/**/
		COLOR(LightCoral, 0xF08080);	/**/
		COLOR(Salmon, 0xFA8072);	/**/
		COLOR(DarkSalmon, 0xE9967A);	/**/
		COLOR(LightSalmon, 0xFFA07A);	/**/
		COLOR(Crimson, 0xDC143C);	/**/
		COLOR(Red, 0xFF0000);	/*赤*/
		COLOR(FireBrick, 0xB22222);	/**/
		COLOR(DarkRed, 0x8B0000);	/**/
		COLOR(Pink, 0xFFC0CB);	/**/
		COLOR(LightPink, 0xFFB6C1);	/**/
		COLOR(HotPink, 0xFF69B4);	/**/
		COLOR(DeepPink, 0xFF1493);	/*鮮やかなピンク*/
		COLOR(MediumVioletRed, 0xC71585);	/*紫(ピンクより)*/
		COLOR(PaleVioletRed, 0xDB7093);	/*薄めの紫(ピンクより)*/
		COLOR(Coral, 0xFF7F50);	/**/
		COLOR(Tomato, 0xFF6347);	/**/
		COLOR(OrangeRed, 0xFF4500);	/**/
		COLOR(DarkOrange, 0xFF8C00);	/**/
		COLOR(Orange, 0xFFA500);	/**/
		COLOR(Gold, 0xFFD700);	/*濃いめの黄色*/
		COLOR(Yellow, 0xFFFF00);	/**/
		COLOR(LightYellow, 0xFFFFE0);	/*ほぼ白*/
		COLOR(LemonChiffon, 0xFFFACD);	/**/
		COLOR(LightGoldenrodYellow, 0xFAFAD2);	/**/
		COLOR(PapayaWhip, 0xFFEFD5);	/**/
		COLOR(Moccasin, 0xFFE4B5);	/**/
		COLOR(PeachPuff, 0xFFDAB9);	/*薄目のピンク(ほぼ肌色)*/

		COLOR(Lavender, 0x8A2BE2);	/*ラベンダー*/

		COLOR(Violet, 0xEE82EE);	/*ヴァイオレット*/
		COLOR(Magenta, 0xFF00FF);	/*マゼンタ*/

		COLOR(BlueViolet, 0xE6E6FA);	/*青紫*/

		COLOR(GreenYellow, 0xADFF2F);	/*黄緑*/
		COLOR(Lime, 0x00FF00);	/*ライム*/

		COLOR(Green, 0x008000);	/*緑*/

		COLOR(Cyan, 0x00FFFF);	/*水色*/
		COLOR(SkyBlue, 0x87CEEB);	/*少しくすんだ水色*/
		COLOR(LightSkyBlue, 0x87CEFA);	/*すこし薄めの水色*/
		COLOR(DeepSkyBlue, 0x00BFFF);	/*少し濃いめの水色*/

		COLOR(RoyalBlue, 0x4169E1);	/*少し濃いめの水色*/
		COLOR(Blue, 0x0000FF);	/*青*/

		COLOR(White, 0xFFFFFF);	/*白*/
		COLOR(Black, 0x000000);	/*黒*/
		COLOR(Brown, 0x8f6446);	/*ブラウン*/

		COLOR(Bluish_Purple, 0x411445);	/*紫紺*/

		COLOR(Bellflower, 0x6A4C9C);	/*桔梗色*/
		COLOR(Azuki, 0x98514B);	/*あずき色*/

#undef COLOR
	}
}