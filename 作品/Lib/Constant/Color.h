#pragma once
#include "Constant.h"

//�K���ɑ��₷
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
		COLOR(Red, 0xFF0000);	/*��*/
		COLOR(FireBrick, 0xB22222);	/**/
		COLOR(DarkRed, 0x8B0000);	/**/
		COLOR(Pink, 0xFFC0CB);	/**/
		COLOR(LightPink, 0xFFB6C1);	/**/
		COLOR(HotPink, 0xFF69B4);	/**/
		COLOR(DeepPink, 0xFF1493);	/*�N�₩�ȃs���N*/
		COLOR(MediumVioletRed, 0xC71585);	/*��(�s���N���)*/
		COLOR(PaleVioletRed, 0xDB7093);	/*���߂̎�(�s���N���)*/
		COLOR(Coral, 0xFF7F50);	/**/
		COLOR(Tomato, 0xFF6347);	/**/
		COLOR(OrangeRed, 0xFF4500);	/**/
		COLOR(DarkOrange, 0xFF8C00);	/**/
		COLOR(Orange, 0xFFA500);	/**/
		COLOR(Gold, 0xFFD700);	/*�Z���߂̉��F*/
		COLOR(Yellow, 0xFFFF00);	/**/
		COLOR(LightYellow, 0xFFFFE0);	/*�قڔ�*/
		COLOR(LemonChiffon, 0xFFFACD);	/**/
		COLOR(LightGoldenrodYellow, 0xFAFAD2);	/**/
		COLOR(PapayaWhip, 0xFFEFD5);	/**/
		COLOR(Moccasin, 0xFFE4B5);	/**/
		COLOR(PeachPuff, 0xFFDAB9);	/*���ڂ̃s���N(�قڔ��F)*/

		COLOR(Lavender, 0x8A2BE2);	/*���x���_�[*/

		COLOR(Violet, 0xEE82EE);	/*���@�C�I���b�g*/
		COLOR(Magenta, 0xFF00FF);	/*�}�[���^*/

		COLOR(BlueViolet, 0xE6E6FA);	/*��*/

		COLOR(GreenYellow, 0xADFF2F);	/*����*/
		COLOR(Lime, 0x00FF00);	/*���C��*/

		COLOR(Green, 0x008000);	/*��*/

		COLOR(Cyan, 0x00FFFF);	/*���F*/
		COLOR(SkyBlue, 0x87CEEB);	/*���������񂾐��F*/
		COLOR(LightSkyBlue, 0x87CEFA);	/*���������߂̐��F*/
		COLOR(DeepSkyBlue, 0x00BFFF);	/*�����Z���߂̐��F*/

		COLOR(RoyalBlue, 0x4169E1);	/*�����Z���߂̐��F*/
		COLOR(Blue, 0x0000FF);	/*��*/

		COLOR(White, 0xFFFFFF);	/*��*/
		COLOR(Black, 0x000000);	/*��*/
		COLOR(Brown, 0x8f6446);	/*�u���E��*/

		COLOR(Bluish_Purple, 0x411445);	/*����*/

		COLOR(Bellflower, 0x6A4C9C);	/*�j�[�F*/
		COLOR(Azuki, 0x98514B);	/*�������F*/

#undef COLOR
	}
}