#pragma once
#include "../Math/MyMathData.h"
#include "../Constant/Constant.h"

namespace MyLib
{
	class SetDxAADraw
	{
	public:
		SetDxAADraw() { BeginAADraw(); }
		~SetDxAADraw() { EndAADraw(); }
	};

	namespace Primitive2D
	{
		//ピクセル(2D)
		struct Pixel
		{
		public:
			constexpr Pixel() {}
			constexpr Pixel(Vector2D p, Color c) : pos(p), color(c) {}
			~Pixel() {}

			virtual void Draw() { DrawPixel(static_cast<int>(pos.x), static_cast<int>(pos.y), color); }
		public:
			Vector2D pos;
			Color color;
		};
		//線(2D)
		struct Line :public Pixel
		{
		public:
			constexpr Line() :AA(false) {}
			constexpr Line(Vector2D  p, Vector2D  e, Color c, bool a = false) : Pixel(p, c), end(e), AA(a) {}
			~Line() {}

			void Draw() override {
				(AA) ? DrawLineAA(pos.x, pos.y, end.x, end.y, color) :
					DrawLine(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(end.x), static_cast<int>(end.y), color);
			}
		public:
			bool AA;
			Vector2D  end;
		};
		//矩形(2D)
		struct Box :public Pixel
		{
		public:
			constexpr Box() :fill(true), AA(false) {}
			constexpr Box(Vector2D  p, Vector2D  s, Color c, bool f = true, bool a = false) : Pixel(p, c), size(s), fill(f), AA(a) {}
			~Box() {}

			void Draw() override {
				auto s = pos - size;
				auto e = pos + size;
				(AA) ? DrawBoxAA(s.x, s.y, e.x, e.y, color, fill) :
					DrawBox(static_cast<int>(s.x), static_cast<int>(s.y), static_cast<int>(e.x), static_cast<int>(e.y), color, fill);
			}
		public:
			bool fill;
			bool AA;
			Vector2D size;
		};
		//円(2D)
		struct Circle :public Pixel
		{
		public:
			constexpr Circle() :radius(1.f), fill(true), AA(false), posnum(0) {}
			constexpr Circle(Vector2D  p, float r, Color c, bool f = true, bool a = false, u16 pn = 180) : Pixel(p, c), radius(r), fill(f), AA(a), posnum(pn) {}
			~Circle() {}

			void Draw() override {
				(AA) ? DrawCircleAA(pos.x, pos.y, radius, posnum, color, fill) :
					DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(radius), color, fill);
			}
		public:
			float radius;
			bool fill;
			bool AA;
			u16 posnum;
		};
		//3角形(2D)
		struct Triangle :public Pixel
		{
		public:
			constexpr Triangle() :fill(true), AA(false) {}
			constexpr Triangle(Vector2D  p, Vector2D  p2, Vector2D  p3, Color c, bool f = true, bool a = false) : Pixel(p, c), fill(f), AA(a), pos2(p2), pos3(p3) {}
			~Triangle() {}

			void Draw() override {
				(AA) ? DrawTriangleAA(pos.x, pos.y, pos2.x, pos2.y, pos3.x, pos3.y, color, fill) :
					DrawTriangle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos2.x), static_cast<int>(pos2.y), static_cast<int>(pos3.x), static_cast<int>(pos3.y), color, fill);
			}
		public:
			bool fill : 1;
			bool AA : 1;
			Vector2D  pos2;
			Vector2D  pos3;
		};
	}
}
