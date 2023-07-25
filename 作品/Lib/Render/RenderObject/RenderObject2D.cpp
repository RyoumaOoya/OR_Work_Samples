#include "RenderObject2D.h"
#include "../BlendData/ScopedBlend.h"

namespace MyLib
{
	RenderObject2D::RenderObject2D() :Handle_(-1), ExRate_(1.f), Angle_(0.f), Tags_(0)
	{
		SetTags({ RenderTags::isDraw });
	}

	RenderObject2D::RenderObject2D(const RenderObject2D& other) : RenderObject2D()
	{
		Tags_ = other.Tags_.load();
		ReadingNum_ = 0;
		Handle_ = other.Handle_;
		Pos_ = other.Pos_;
		ExRate_ = other.ExRate_;
		Angle_ = other.Angle_;
		BlendData_ = other.BlendData_;
	}

	RenderObject2D::~RenderObject2D()
	{
	}

	void RenderObject2D::Load(filepath path)
	{
		if (Handle_ == -1) {
			Handle_ = LoadGraph(path.string().c_str());
		}
	}

	void RenderObject2D::Load(const RenderObject2D& b)
	{
		if (Handle_ == -1) {
			Handle_ = b.Handle_;
		}
	}

	void RenderObject2D::Draw()
	{
		if (BlendData_) {
			ScopedBlend sb(BlendData_);
			DrawRotaGraphFast(static_cast<iVector2D>(Pos_), ExRate_, Angle_, Handle_, true, HasTag(RenderTags::isReverseX), HasTag(RenderTags::isReverseY));
		}
		else {
			DrawRotaGraphFast(static_cast<iVector2D>(Pos_), ExRate_, Angle_, Handle_, true, HasTag(RenderTags::isReverseX), HasTag(RenderTags::isReverseY));
		}
	}

	void RenderObject2D::Delete()
	{
		if (Handle_ != -1) {
			DeleteGraph(Handle_);
			Handle_ = -1;
		}
	}
}