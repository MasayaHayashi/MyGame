//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneTitle.cpp
// Author	 : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "SceneTitle.h"
#include "../../Light/Light.h"
#include "../../Player/Player.h"
#include "../../Camera/camera.h"
#include "../../Skydome/Skydome.h"
#include "../../MainField/MainField.h"
#include "../../TitleUI/TitleUI.h"
#include "../../HeartObj/HeartObj.h"

/*
#include "C_MainField.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "input.h"
#include "C_StarBase.h"
#include "C_Player.h"
#include "C_TitleObj.h"
#include "FadeUI.h"
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

	lightPtr.reset(NEW Light());
	cameraPtr.reset(NEW Camera());
	skydomePtr.reset(NEW Skydome());
	playerPtr.reset(NEW Player());
	fieldPtr.reset(NEW MainField());
	titleUiPtr.reset(NEW TitleUI());
	heartObjPtr.reset(NEW HeartObj());
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
	playerPtr->initialize();
	lightPtr->initialize();
	cameraPtr->initializeTitle();
	skydomePtr->initialize();
	fieldPtr->initialize();
	titleUiPtr->initialize();
	heartObjPtr->initialize();

	/*
	// パーティクル初期化
	for (INT i = 0; i < MAX_Star; i++)
	{
		pStar[i] = NEW C_Star_BASE(i);
		pStar[i]->initialize();
	}
	*/
	/*

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
		pBoard[BoardCnt]->initialize();
	}

	pBoard[UI_MAIN]->setUsedFlg(false);
	*/

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::finalize()
{
	cameraPtr->finalize();
	playerPtr->finalize();

	/*
	// カメラ後処理
	pCamera->finalizeCamera();
	SAFE_DELETE(pCamera);

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
	for (int i = 0; i < MAX_Star; i++)
	{
		pStar[i]->finalizeObject();
		SAFE_DELETE(pStar[i]);
	}

	// UI後処理
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->finalizeObject();
		SAFE_DELETE(pBoard[BoardCnt]);
	}

	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::update()
{
	playerPtr->update(SceneManager::getInstanse()->getCamera()->getFowerd());
	heartObjPtr->update();
	skydomePtr->update();
	fieldPtr->update();
	cameraPtr->updateTitle(playerPtr.get());

	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}

	/*
	D3DXVECTOR3 CameraFowerd = pCamera->getCameraFowerd();

	// タイトルUI更新
	pTitleUI->updateObject();

	// パーティクル更新
	for (INT i = 0; i < MAX_Star; i++)
		pStar[i]->updateObject();

	// プレイヤー更新
	pPlayer->updateObject(CameraFowerd);

	// タイトルオブジェクト更新
	pTitleObj->updateObject();

	// カメラ更新
	pCamera->updateCamera_Title(pPlayer);

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
	*/

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::draw()
{
	playerPtr->draw();
	heartObjPtr->draw();
	skydomePtr->draw();
	fieldPtr->draw();

	// スカイドーム描画
//	pSkydome->draw();

	/*
	// フィールド描画
	pField->drawMeshField();

	// タイトルロゴ描画
	pTitleUI->drawObject();

	// タイトルオブジェクト描画
	pTitleObj->drawObject();

	*/

	// カメラセット
	cameraPtr->setCamera();

	/*
	// パーティクル描画
	for (int i = 0; i < MAX_Star; i+)+
		pStar[i]->drawObject();

	// UI描画
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
		pBoard[BoardCnt]->drawObject();
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneTitle::initializeStatus()
{

}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Camera* SceneTitle::getCamera()
{
	return cameraPtr.get();
}