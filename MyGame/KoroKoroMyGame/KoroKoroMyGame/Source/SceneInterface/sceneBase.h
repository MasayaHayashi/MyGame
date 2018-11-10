//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneBase.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef C_SCENEBASE_H
#define C_SCENEBASE_H

// ===== クラスの前方宣言 =====
class Camera;

// ===== インクルード部 =====
#include "../Audio/\audiere.h"
#include "d3dx9.h"

// ===== 列挙体定義 =====
enum class GAME_STATE
{
	GAME_READY = 0,	// 準備状態
	GAME_TUTORIAL,	// チュートリアル表示
	GAME_NORMAL,	// 通常状態
	GAME_PAUSE,		// ポーズ状態
	GAME_GOAL,		// ゴール状態
	GAME_MISS,		// 落下状態
	MAX_GAME_STATE,
};

// ===== クラス定義 =====
class C_SCENE_BASE
{
public:
	C_SCENE_BASE();
	virtual ~C_SCENE_BASE();

	virtual void initialize()   = 0;
	virtual void finalize() = 0;
	virtual void update() = 0;
	virtual void draw()   = 0;

	virtual void initializeStatus()  = 0;		// 各種ステータスリセット

	// ゲッター関連
	virtual Camera* getCamera() = 0;

	GAME_STATE getGameState();

protected:
	audiere::OutputStreamPtr bgmPtr;
	audiere::SoundEffectPtr	 sePtr;

	UINT gameState;
	
private:
};

#endif