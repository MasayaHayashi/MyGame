//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneResult.h
// シーンリザルトクラス
// Author : Ryota Inoue
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef __C_SCENE_RESULT_H__
#define __C_SCENE_RESULT_H__

// ===== インクルード部 =====
#include "C_SceneBase.h"
#include "main.h"

// ===== クラスの前方定義 =====
class C_SKYDOME;
class C_PLAYER;
class C_CAMERA;

// ===== クラス定義 =====
class C_SCENE_RESULT : public C_SCENE_BASE
{
public:
	C_SCENE_RESULT();
	~C_SCENE_RESULT();

	void InitScene();
	void UninitScene();
	void UpdateScene();
	void DrawScene();

	void InitStatus();

	// ゲッター関連
	C_CAMERA* GetCamera();

private:
	bool bSEPlay;			// 音再生中か
	C_SKYDOME *pSkydome;	// スカイドーム
	C_PLAYER  *pPlayer;		// プレイヤー

};

#endif