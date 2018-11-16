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

protected:
	audiere::OutputStreamPtr bgmPtr;
	audiere::SoundEffectPtr	 sePtr;
private:
};

#endif