#pragma once

//シーン状態
enum class SCENE_STATE :int {
	INIT,		//初期化
	LOAD,		//ロード
	LOAD_END,	//ロード終了
	LOOP,		//ループ
	FIN,		//後処理
	FIN_END,
};

#define ISCENE_STATE (int)SCENE_STATE
//シーンID
enum class SCENE_ID :unsigned __int16
{
	TATLE,
	PLAY,	//プレイシーン
	NUM,
};

#define ISCENE_ID (int)SCENE_ID

enum class SUB_SCENE_ID :unsigned __int16
{

};