#pragma once
#include "Src/Map/Map.h"
#include "Src/Sky/Sky.h"

class Stage
{
	friend class StageManager;
public:
	Stage();
	~Stage();

	void Init(int ID);
	void Load();
	void Step();
	void UpDate();
	void FrontDraw();
	void Delete();
	void Fin();
private:
	Pointer<Map> Map_;
	Pointer<Sky> Sky_;
	int ID_;
private:
	char buffer[4] = { 0 };
};

namespace Size {
	constexpr size_t STAGE = sizeof(Stage);
}