//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneStageEdit.h
// シーンタイトルクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef __C_SCENE_STAGE_EDIT_H__
#define __C_SCENE_STAGE_EDIT_H__

// ===== インクルード部 =====
#include "C_SceneBase.h"
#include "C_Block.h"

// ===== 定数・マクロ定義 =====

// ===== 列挙体定義 =====

// ===== クラスの前方定義 =====
class C_LIGHT;
class C_CAMERA;
class C_SKYDOME;
class C_STAGE_EDITOR;

// ===== クラス定義 =====
class C_SCENE_STAGE_EDIT : public C_SCENE_BASE
{
public:
	C_SCENE_STAGE_EDIT();	// コンストラクタ
	~C_SCENE_STAGE_EDIT();	// デストラクタ

	void InitScene();
	void UninitScene();
	void UpdateScene();
	void DrawScene();

	void InitStatus();

	// ゲッター関連
	C_CAMERA* GetCamera();


private:

	C_SKYDOME				*pSkydome;									// スカイドーム
	C_STAGE_EDITOR			*pStageEditor;								// ステージエディター

};

#endif