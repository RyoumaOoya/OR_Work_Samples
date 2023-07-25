#pragma once
#include "Scene.h"
#include <thread>

class Play :public Scene
{
public:
	Play();
	~Play() override;
	void Init() override;
	void Load() override;
	void OnLoadEnd() override;
	void Step() override;
	void UpDate() override;
	void Draw() override;
	void FrontDraw() override;
	void  Fin() override;
};

/*
	Play ƒƒ‚ƒŠƒCƒ[ƒW

	Scene{
		next_scene_	4byte
		state_		4byte
		vfptr	8byte
	}	:24byte

	staticShadow_ 4byte		playerNearShadow_ 4byte
*/

namespace Size {
	constexpr size_t PLAY_SCENE = sizeof(Play);
}