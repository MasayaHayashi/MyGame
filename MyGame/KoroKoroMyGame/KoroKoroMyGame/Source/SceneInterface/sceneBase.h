//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneBase.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SCENEBASE_H
#define SCENEBASE_H

// ===== クラスの前方宣言 =====
class Camera;

// ===== インクルード部 =====
#include "../Audio/\audiere.h"
#include "d3dx9.h"


// ===== クラス定義 =====
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase();

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