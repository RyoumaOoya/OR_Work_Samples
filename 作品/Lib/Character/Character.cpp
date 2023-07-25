#include "Character.h"

namespace MyLib
{
	Character::Character() :Object()
	{
	}

	Character::~Character()
	{
		Anim_.Fin();
	}

	void Character::Load()
	{
		__super::Load();
		Anim_.Load();
	}

	void Character::Load(const char* file_path)
	{
		__super::Load(file_path);
		Anim_.Load();
	}

	void Character::Step()
	{
		Anim_.Step();
	}

	void Character::UpDate()
	{
		__super::UpDate();
		Anim_.UpDate();
	}

	void Character::Fin()
	{
		Anim_.Fin();
		__super::Fin();
	}
}