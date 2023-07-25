#pragma once
#include <DxLib.h>
#include "Object/Object.h"

class Sky :public Object
{
public:
	Sky(int ID);
	~Sky() override;
public:
	void Init() override;
	void Init(Pointer<Sky> t);
	void Load() override;
	void Step() override;
	void FrontDraw() override;
	void Delete() override;
	void Fin() override;
private:
	int ID_;
private:
	char buffer[4] = { 0 };
};

namespace Size {
	constexpr size_t SKY = sizeof(Sky);
}