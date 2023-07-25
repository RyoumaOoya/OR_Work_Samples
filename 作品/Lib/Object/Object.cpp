#include "Object.h"
#include "../Literal.h"
#include "../Render/Render.h"

namespace MyLib
{
	Object::Object() :RenderObject_()
	{
		constexpr auto state = "Stand"_int64;
		Tag_ = 0x00;
		State_ = state;
		Flags_ = 0;
	}

	Object::~Object()
	{
		Fin();
	}

	void Object::OnCollisionBegin(Pointer<Object> other)
	{
	}

	void Object::OnCollisionEnter(Pointer<Object> other)
	{
	}

	void Object::OnCollisionEnd(Pointer<Object> other)
	{
	}

	void Object::Init()
	{
	}

	void Object::Load()
	{
	}

	void Object::Load(const char* file_path)
	{
		RenderObject_.Load(file_path);
	}

	void Object::Load(int handle)
	{
		RenderObject_.Load(handle);
	}

	void Object::ConflictStep()
	{
	}

	void Object::UpDate()
	{
		RenderObject3DBase::WritingLock wl(&RenderObject_);
		RenderObject_.UpDate();
	}

	/*void Object::ShadowDraw()
	{
		Draw();
	}*/

	/*void Object::Draw()
	{
	}*/

	void Object::FrontDraw()
	{
	}

	void Object::Delete()
	{
		EreseModelList();
		RenderObject_.Delete();
	}

	void Object::Fin()
	{
		Delete();
	}

	void Object::PushModelList()
	{
		if (This_ && !isFlag(OBJECT_FLAGS::SetRender)) {
			Render::PushModelList(This_, &RenderObject_);
			AddFlag(OBJECT_FLAGS::SetRender);
		}
	}

	void Object::EreseModelList()
	{
		if (isFlag(OBJECT_FLAGS::SetRender)) {
			Render::EreseModelList(This_, &RenderObject_);
			RemoveFlag(OBJECT_FLAGS::SetRender);
		}
	}

	void Object::SetDeleteFlag()
	{
		if (isFlag(OBJECT_FLAGS::SetRender)) {
			RenderObject_.SetTag(RenderTags::isDeleteTarget);
			RemoveFlag(OBJECT_FLAGS::SetRender);
		}
	}
}