//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneTitle.cpp
// シーンタイトルクラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "C_SceneStageEdit.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "C_MainField.h"
#include "C_Player.h"
#include "C_Enemy.h"
#include "C_Collision.h"
#include "C_StageEditor.h"
#include "C_Pause.h"
#include "C_Block.h"
#include "C_MoveBlock.h"
#include "C_ItemStar.h"
#include "C_GoalObj.h"
#include "C_Skydome.h"
#include "input.h"
#include "C_SceneManager.h"
#include <stdio.h>	// ファイル入出力用

#if _DEBUG
	#include "debugproc.h"
#endif

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_SCENE_STAGE_EDIT::C_SCENE_STAGE_EDIT()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_SCENE_STAGE_EDIT::~C_SCENE_STAGE_EDIT()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_STAGE_EDIT::InitScene()
{
	// スカイドーム初期化
	pSkydome = NEW C_SKYDOME;
	pSkydome->InitObject();

	// ステージエディター初初期化
	pStageEditor = NEW C_STAGE_EDITOR;
	pStageEditor->InitObject();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_STAGE_EDIT::UninitScene()
{
	// スカイドーム後処理
	pSkydome->UninitObject();
	SAFE_DELETE(pSkydome);

	// ステージエディター後処理
	pStageEditor->UninitObject();
	SAFE_DELETE(pStageEditor);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_STAGE_EDIT::UpdateScene()
{
	// スカイドーム更新
	pSkydome->UpdateObject();

	// ステージエディター更新
	pStageEditor->UpdateObject();

	// シーン遷移
	if (GetKeyboardTrigger(DIK_F1))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_MAIN);
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_STAGE_EDIT::DrawScene()
{
	// スカイドーム描画
	pSkydome->DrawObject();

	// ステージエディター描画
	pStageEditor->DrawObject();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_STAGE_EDIT::InitStatus()
{

}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_CAMERA* C_SCENE_STAGE_EDIT::GetCamera()
{
	return pStageEditor->GetCamera();
}