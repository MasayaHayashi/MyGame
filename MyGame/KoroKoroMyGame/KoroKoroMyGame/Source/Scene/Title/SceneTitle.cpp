//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneTitle.cpp
// Author	 : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "SceneTitle.h"
#include ""
#include "C_MainField.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "input.h"
#include "C_ParticleBase.h"
#include "C_Player.h"
#include "C_TitleObj.h"
#include "C_FADE.h"
#include "C_SceneMainUI.h"
#include "C_StageEditUI.h"
#include "debugproc.h"
#include "C_TitleDesc.h"
#include <stdio.h>	// ファイル入出力用

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneTitle::SceneTitle()
{
	bChangeScene = false;
	nChangeSceneWaitCnt = 0;
	uSelectScene = 0;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneTitle::~SceneTitle()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::initialize()
{
	// ライト初期化
	pLight = new C_LIGHT;
	pLight->InitLight();

	// プレイヤー初期化
	pPlayer = new C_PLAYER;
	pPlayer->InitObject();

	// カメラ初期化
	pCamera = new C_CAMERA;
	pCamera->InitCamera(pPlayer);

	// スカイドーム初期化
	pSkydome = NEW C_SKYDOME;
	pSkydome->InitObject();

	// フィールド初期化
	pField = NEW C_MAIN_FIELD;
	pField->InitMeshField();

	// タイトルUI初期化
	pTitleUI = NEW C_TITLE_UI;
	pTitleUI->InitObject();

	// タイトルオブジェクト初期化
	pTitleObj = NEW C_TITLE_OBJ;
	pTitleObj->InitObject();

	// パーティクル初期化
	for (INT i = 0; i < MAX_PARTICLE; i++)
	{
		pParticle[i] = NEW C_PARTICLE_BASE(i);
		pParticle[i]->InitObject();
	}

	// 
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		switch (BoardCnt)
		{
		case UI_MAIN:
			pBoard[BoardCnt] = NEW C_SCENE_MAIN_UI;
			break;
		case UI_EDIT:
			pBoard[BoardCnt] = NEW C_STAGE_EDIT_UI;
			break;
		case UI_DESC:
			pBoard[BoardCnt] = NEW C_TITLE_DESCR_UI;
			break;
		default:
			break;
		}
		pBoard[BoardCnt]->InitObject();
	}

	pBoard[UI_MAIN]->SetUsedFlg(false);

	// BGM再生
//	PlaySound(SOUND_BGM_TITLE);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::UninitScene()
{
	// BGM停止


	// カメラ後処理
	pCamera->UninitCamera();
	SAFE_DELETE(pCamera);

	// ライト後処理
	pLight->UninitLight();
	SAFE_DELETE(pLight);

	// プレイヤー後処理
	pPlayer->UninitObject();
	SAFE_DELETE(pPlayer);

	// スカイドーム後処理
	pSkydome->UninitObject();
	SAFE_DELETE(pSkydome);

	// フィールド後処理
	pField->UninitMeshField();
	SAFE_DELETE(pField);

	// タイトルUI後処理
	pTitleUI->UninitObject();
	SAFE_DELETE(pTitleUI);

	// タイトルオブジェクト後処理
	pTitleObj->UninitObject();
	SAFE_DELETE(pTitleObj);

	// パーティクル後処理
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		pParticle[i]->UninitObject();
		SAFE_DELETE(pParticle[i]);
	}

	// UI後処理
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->UninitObject();
		SAFE_DELETE(pBoard[BoardCnt]);
	}


}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::UpdateScene()
{
	D3DXVECTOR3 CameraFowerd = pCamera->GetCameraFowerd();

	// タイトルUI更新
	pTitleUI->UpdateObject();

	// パーティクル更新
	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->UpdateObject();

	// プレイヤー更新
	pPlayer->UpdateObject(CameraFowerd);

	// タイトルオブジェクト更新
	pTitleObj->UpdateObject();

	// カメラ更新
	pCamera->UpdateCamera_Title(pPlayer);

	// シーン遷移
	if (GetKeyboardTrigger(DIK_F1))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

		bChangeScene = true;
	}
	if (GetKeyboardTrigger(DIK_F2))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

		bChangeScene = true;
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		if (uSelectScene == 0)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

			bChangeScene = true;
		}
		else if (uSelectScene == 1)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

			bChangeScene = true;
		}
	}

	if (GetKeyboardPress(DIK_A))
	{
		pBoard[UI_MAIN]->SetUsedFlg(false);
		pBoard[UI_EDIT]->SetUsedFlg(true);
		uSelectScene = 0;
	}
	if (GetKeyboardPress(DIK_D))
	{
		pBoard[UI_MAIN]->SetUsedFlg(true);
		pBoard[UI_EDIT]->SetUsedFlg(false);
		uSelectScene = 1;
	}

	// UI更新
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->UpdateObject();
	}


	/*
	// シーン遷移
	if (bChangeScene)
		nChangeSceneWaitCnt++;
	if(nChangeSceneWaitCnt > 120)
	*/

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::DrawScene()
{
	// プレイヤー描画
	pPlayer->DrawObject();

	// スカイドーム描画
	pSkydome->DrawObject();

	// フィールド描画
	pField->DrawMeshField();

	// タイトルロゴ描画
	pTitleUI->DrawObject();

	// タイトルオブジェクト描画
	pTitleObj->DrawObject();

	// カメラセット
	pCamera->SetCamera();

		
	// パーティクル描画
	for (int i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->DrawObject();

	// UI描画
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
		pBoard[BoardCnt]->DrawObject();
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::InitStatus()
{

}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_CAMERA* SceneTitle::GetCamera()
{
	return pCamera;
}