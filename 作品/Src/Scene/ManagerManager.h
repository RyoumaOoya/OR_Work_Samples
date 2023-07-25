#pragma once
#include "ManagerBase.h"
#include <vector>
#include <algorithm>
#include <initializer_list>

class ManagerManager
{
	ManagerManager() = delete;
	ManagerManager(const ManagerManager&) = delete;
	~ManagerManager() = delete;
public:
	static void Init();
	static void Load();
	static void OnLoadEnd();
	static void Step();
	static void ConflictStep();
	static void Update();
	static void ShadowDraw();
	static void Draw();
	static void FrontDraw();
	static void Delete();
	static void Fin();

	static void AddManager(const Pointer<ManagerBase>& manager_ptr) { Managers_.emplace_back(manager_ptr); std::sort(std::begin(Managers_), std::end(Managers_)); }
	static void AddManager(const std::initializer_list<Pointer<ManagerBase>> manager_ptrs) 
	{ for (auto manager_ptr : manager_ptrs) Managers_.emplace_back(manager_ptr); std::sort(std::begin(Managers_), std::end(Managers_)); }
private:
	static float DeleteTime_;
	static std::vector<Pointer<ManagerBase>> Managers_;
};

