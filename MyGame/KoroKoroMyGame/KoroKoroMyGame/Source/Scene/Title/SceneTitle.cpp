//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneTitle.cpp
// Author	 : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "SceneTitle.h"
#include "../../Light/Light.h"
#include "../../Player/Player.h"
#include "../../camera/camera.h"
/*
#include ""
#include "C_MainField.h"
#include "C_camera.h"
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
*/

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
	pLight = new Light;
	pLight->InitLight();

	// プレイヤー初期化
	pPlayer = new C_PLAYER;
	pPlayer->InitObject();

	// カメラ初期化
	pcamera = new camera;
	pcamera->initialize(pPlayer);

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

	pBoard[UI_MAIN]->setUsedFlg(false);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::finalizeScene()
{

	// カメラ後処理
	pcamera->finalizecamera();
	SAFE_DELETE(pcamera);

	// ライト後処理
	pLight->finalizeLight();
	SAFE_DELETE(pLight);

	// プレイヤー後処理
	pPlayer->finalizeObject();
	SAFE_DELETE(pPlayer);

	// スカイドーム後処理
	pSkydome->finalizeObject();
	SAFE_DELETE(pSkydome);

	// フィールド後処理
	pField->finalizeMeshField();
	SAFE_DELETE(pField);

	// タイトルUI後処理
	pTitleUI->finalizeObject();
	SAFE_DELETE(pTitleUI);

	// タイトルオブジェクト後処理
	pTitleObj->finalizeObject();
	SAFE_DELETE(pTitleObj);

	// パーティクル後処理
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		pParticle[i]->finalizeObject();
		SAFE_DELETE(pParticle[i]);
	}

	// UI後処理
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->finalizeObject();
		SAFE_DELETE(pBoard[BoardCnt]);
	}


}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::updateScene()
{
	D3DXVECTOR3 cameraFowerd = pcamera->getcameraFowerd();

	// タイトルUI更新
	pTitleUI->updateObject();

	// パーティクル更新
	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->updateObject();

	// プレイヤー更新
	pPlayer->updateObject(cameraFowerd);

	// タイトルオブジェクト更新
	pTitleObj->updateObject();

	// カメラ更新
	pcamera->updatecamera_Title(pPlayer);

	// シーン遷移
	if (getKeyboardTrigger(DIK_F1))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

		bChangeScene = true;
	}
	if (getKeyboardTrigger(DIK_F2))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

		bChangeScene = true;
	}

	if (getKeyboardTrigger(DIK_SPACE))
	{
		if (uSelectScene == 0)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

			bChangeScene = true;
		}
		else if (uSelectScene == 1)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

			bChangeScene = true;
		}
	}

	if (getKeyboardPress(DIK_A))
	{
		pBoard[UI_MAIN]->setUsedFlg(false);
		pBoard[UI_EDIT]->setUsedFlg(true);
		uSelectScene = 0;
	}
	if (getKeyboardPress(DIK_D))
	{
		pBoard[UI_MAIN]->setUsedFlg(true);
		pBoard[UI_EDIT]->setUsedFlg(false);
		uSelectScene = 1;
	}

	// UI更新
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->updateObject();
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
void SceneTitle::drawScene()
{
	// プレイヤー描画
	pPlayer->drawObject();

	// スカイドーム描画
	pSkydome->drawObject();

	// フィールド描画
	pField->drawMeshField();

	// タイトルロゴ描画
	pTitleUI->drawObject();

	// タイトルオブジェクト描画
	pTitleObj->drawObject();

	// カメラセット
	pcamera->setcamera();

		
	// パーティクル描画
	for (int i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->drawObject();

	// UI描画
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
		pBoard[BoardCnt]->drawObject();
	
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
const camera* SceneTitle::getcamera()
{
	return pcamera;
}