//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_StageEditor.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "../Editor/StageEditor.h"
#include "../../Light/Light.h"
#include "../../Camera/Camera.h"
#include "../../Player/Player.h"
#include "../../MainField/MainField.h"
#include "../../MyDelete/MyDelete.h"
#include "../../EditUI/WhatStageSaveUI.h"
#include "../../Goal/Goal.h"
#include <fstream>

//
//#include "C_MoveBlock.h"
//#include "C_ItemStar.h"
//#include "C_GoalObj.h"
//#include "C_Pause.h"
//#include "C_Enemy.h"
//#include "C_WhatStageLoadUI.h"
//#include "C_WhatStageSaveUI.h"
//#include "C_StageUI.h"
//#include "C_NumbersUIBase.h"
//#include "C_SaveOkUI.h"
//#include "C_StageEditUI.h"
//#include "C_EditDescrUI.h"
//#include "input.h"
//#include "collision.h"

// ===== 定数・マクロ定義 =====

//#define TEXTURE_NAME "data/TEXTURE/penguin1.png"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
StageEditor::StageEditor()
{
	// フラグ初期化
	nErrorFopen			= false;
	isSelectSaveStage	= false;
	uSaveStage			= static_cast<INT>(STAGE_SELECT_TYPE::STAGE_SELECT1);
	uSelectMode			= static_cast<INT>(EDIT_MODE_TYPE::MODE_EDIT);
	
	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		creteGameObj(gameObjTypeIndex);
	}

	for (UINT boardObjTypeIndex = 0; boardObjTypeIndex < GameObjectBase::MaxGameObjType; boardObjTypeIndex++)
	{
		createBoard(boardObjTypeIndex);
	}

	/*
	for (INT gameObjType = 0; gameObjType < GameObjectBase::MaxGameObjType; gameObjType++)
	{
		gameObjPtr[gameObjType].push_back(NEW Pawn());
	}
	*/

	/*
	// ステージ読み込み初期化
	for (INT StageCnt = 0; StageCnt < MAX_STAGE; StageCnt++)
	{
		switch (StageCnt)
		{
		case 0 :
			strcpy_s(szStageFileName[StageCnt][0], STAGE_TUTORIAL_FILE_NAME);
			break;
		case 1:
			strcpy(&szStageFileName[StageCnt][0], STAGE_1_FILE_NAME);
			break;
		case 2:
			strcpy(&szStageFileName[StageCnt][0], STAGE_2_FILE_NAME);
			break;
		case 3:
			strcpy(&szStageFileName[StageCnt][0], STAGE_3_FILE_NAME);
			break;

		default:
			break;
		}
	}
	*/

	lightPtr.reset(NEW Light());
	cameraPtr.reset(NEW Camera());
	playerPtr.reset(NEW Player(D3DXVECTOR3(0.0f,5.0f,0.0f),0));
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
StageEditor::~StageEditor()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::initialize()
{
	lightPtr->initialize();
	playerPtr->initialize();
	cameraPtr->initialize();

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->initialize();
		}
	}

	for (UINT boardTypeIndex = 0; boardTypeIndex < MaxBoardObj; boardTypeIndex++)
	{
		for (auto itr = boardObjPtr[boardTypeIndex].begin(); itr != boardObjPtr[boardTypeIndex].end(); itr++)
		{
			(*itr)->initialize();
		}
	}

	const auto currentGameObject = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);

	(*currentGameObject)->setUsedFlg(true);


//	loadStageData(1);

	/*
	// UI初期化
	for (INT UiCnt = 0; UiCnt < MAX_EDITOR_UI_TYPE; UiCnt++)
	{
		switch (UiCnt)
		{
			case UI_WTHAT_STAGE_SAVE:
				pUIObj[UiCnt] = NEW C_WHAT_STAGE_SAVE_UI;
				break;
			case UI_WTHAT_STAGE_LOAD:
				pUIObj[UiCnt] = NEW C_WHAT_STAGE_LOAD_UI;
				break;
			case UI_STAGE:
				pUIObj[UiCnt] = NEW C_STAGE_UI;
				break;
			case UI_NUMBERS:
				pUIObj[UiCnt] = NEW C_NUMBERS_UI_BASE;
				break;
			case UI_SAVE_OK:
				pUIObj[UiCnt] = NEW C_SAVE_OK_UI;
				break;
			case UI_STAGE_EDIT:
				pUIObj[UiCnt] = NEW C_STAGE_EDIT_UI;
				break;
			case UI_DESCR:
				pUIObj[UiCnt] = NEW C_EDIT_DESCR_UI;
				break;
			default:
				break;
		}
		pUIObj[UiCnt]->InitObject();
	}
	*/


	//// 衝突用クラス生成
	//pCollision = NEW C_COLLISION;

	//// ポーズ用オブジェクト初期化
	//pPause = NEW C_PAUSE;
	//pPause->InitObject();
	//bPause = false;

	//// 現在の選択ブロック初期化
	//CurrentSelectType = NORMAL_BLOCK_OBJ;
	//for (INT i = 0; i < MAX_GameObjectType; i++)
	//	uSelectObjNum[i] = 0;

	//// 使用フラグセット
	//pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//// ステージ読み込み
	//LoadStageData(0);

	// BGM再生
//	PlaySound(SOUND_BGM_EDIT);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::finalize()
 {
	cameraPtr->finalize();

	lightPtr->finalize();

	playerPtr->finalize();



	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->finalize();
			Mydelete::safeDelete(*itr);
		}
	}

	//// UI関連後処理
	//for (INT i = 0; i < MAX_EDITOR_UI_TYPE; i++)
	//{
	//	pUIObj[i]->UninitObject();
	//	SAFE_DELETE(pUIObj[i]);
	//}


	//// ブロック後処理
	//for (INT i = 0; i < MAX_GameObjectType; i++)
	//{
	//	for (INT j = 0; j < MAX_GAME_OBJ; j++)
	//	{
	//		pGameObj[i][j]->UninitObject();
	//		SAFE_DELETE(pGameObj[i][j]);
	//	}
	//}

	//// ポーズ後処理
	//pPause->UninitObject();
	//SAFE_DELETE(pPause);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::update()
{
	playerPtr->update(cameraPtr->getFowerd());

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->update();
		}
	}

	for (UINT boardTypeIndex = 0; boardTypeIndex < MaxBoardObj; boardTypeIndex++)
	{
		for (auto itr = boardObjPtr[boardTypeIndex].begin(); itr != boardObjPtr[boardTypeIndex].end(); itr++)
		{
			(*itr)->update();
		}
	}

	move();

	place();

	if (Keyboard::getTrigger(DIK_G))
	{
		gameObjPtr[selectGameObjType][selectGameObjIndex]->setUsedFlg(false);
		selectGameObjType ++;

		selectGameObjType = selectGameObjType % GameObjectBase::MaxGameObjType;

		updateSelectIndex();

		gameObjPtr[selectGameObjType][selectGameObjIndex]->setUsedFlg(true);
	}

	DirectX3D::printDebug("ゲームオブジェクトの種類%d", selectGameObjType);
	DirectX3D::printDebug("ゲームオブジェクトのインデックス%d", selectGameObjIndex);

	saveStageData(1);

	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}
	
	cameraPtr->updateStageEdit(gameObjPtr[selectGameObjType][selectGameObjIndex]->getKeyName(),selectGameObjIndex);

//	cameraPtr->update();
//{
//	// ポーズ解除処理
//	if (GetKeyboardTrigger(DIK_P))
//		bPause = !bPause;
//
//	// ポーズ更新
//	if (bPause)
//	{
//		pPause->UpdateObject();
//		return;
//	}
//
//	// プレイヤー更新
//	pPlayer->UpdatePlayer_SceneEdit();
//
//	// カメラ更新
//	pCamera->UpdateCamera_StageEdit(pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->GetPosition());
//
//	// UI関連更新
//	for (INT i = 0; i < MAX_EDITOR_UI_TYPE; i++)
//		pUIObj[i]->UpdateObject();
//
//	// ブロック更新
//	for (int j = 0; j < MAX_GameObjectType; j++)
//		for (int i = 0; i < MAX_GAME_OBJ; i++)
//			pGameObj[j][i]->UpdateObject();
//
//	if (uSelectMode == MODE_EDIT)
//	{
//		// 判定処理
//		D3DXVECTOR3 Cross, Normal;
//		D3DXVECTOR3 fLength;
//
//		for (INT i = 0; i < MAX_GameObjectType; i++)
//			for (INT j = 0; j < MAX_GAME_OBJ; j++)
//			{
//				// 例外処理
//				if (!pGameObj[i][j]->GetUsedFlg())
//					continue;
//
//				// プレイヤーとブロックとの判定
//				pCollision->CheckCollisionField(pPlayer, NULL, pGameObj[i][j], Cross, Normal, fLength,D3DXVECTOR3(0.0f,-1.0f,0.0f));
//			}
//
//		// 選択オブジェクト切り替え
//		ChangeSelectObj();
//
//		// 移動
//		MoveObj();
//
//		// 削除
//		DeleteObj();
//	}
//
//	if (GetKeyboardTrigger(DIK_O))
//		// モード切替
//		uSelectMode = MODE_SAVE_CONF;
//
//	// 書き込みステージ確認
//	if (uSelectMode == MODE_SAVE_CONF)
//	{
//		// セーブするかの確認
//		CheckSaveData();
//
//		// 書き込み
//		if (bIsSelectSaveStage)
//			SaveStageData();
//	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステージ関連描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::draw()
{
	playerPtr->draw();

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->draw();
		}
	}

	for (UINT boardTypeIndex = 0; boardTypeIndex < MaxBoardObj; boardTypeIndex++)
	{
		for (auto itr = boardObjPtr[boardTypeIndex].begin(); itr != boardObjPtr[boardTypeIndex].end(); itr++)
		{
			(*itr)->draw();
		}
	}

	cameraPtr->setCamera();


	//// ポーズ処理
	//if (bPause)
	//{
	//	// ポーズ描画
	//	pPause->DrawObject();
	//}

	//// ブロック描画
	//for (int i = 0; i < MAX_GameObjectType; i++)
	//	for (int j = 0; j < MAX_GAME_OBJ; j++)
	//		pGameObj[i][j]->DrawObject();

	//// UI関連描画
	//for (INT i = 0; i < MAX_EDITOR_UI_TYPE; i++)
	//	pUIObj[i]->DrawObject();

	//// カメラセット
	//pCamera->SetCamera();

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステージデータ書き出し
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::saveStageData(size_t stageNumber)
{
	if (!Keyboard::getTrigger(DIK_O))
	{
		return;
	}

	std::ofstream exportFile;

	std::string stageString = std::to_string(stageNumber);
	exportFile.open(StagePassName + stageString + ".bin", std::ios::binary);

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			exportWorkData = (*itr)->getExportData();
			exportFile.write((CHAR*)( &exportWorkData), sizeof(ExportData));
		}
	}

	boardObjPtr[0].back()->setUsedFlg(true);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// データ読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::loadStageData(size_t stageNumber)
{
	std::ifstream loadtFile;

	std::string stageString = std::to_string(stageNumber);
	loadtFile.open(StagePassName + stageString + ".bin", std::ios::binary);

	if (!loadtFile)
	{
		MessageBox(nullptr, TEXT("Error"), TEXT("ステージデータ読み込みエラー"), MB_ICONERROR);
		return;
	}

	for (size_t gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			loadtFile.read((CHAR*) (&exportWorkData), sizeof(ExportData));

			if (exportWorkData.isUsed)
			{
				selectGameObjIndex ++;
			}

			(*itr)->reflectionExportData(exportWorkData);
		}
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 書き出しするかのチェック
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::checkSaveData()
{
//{
//	// UI表示
//	pUIObj[UI_WTHAT_STAGE_SAVE]->SetUsedFlg(true);
//	pUIObj[UI_STAGE]->SetUsedFlg(true);
//	pUIObj[UI_NUMBERS]->SetUsedFlg(true);
//
//	if (GetKeyboardTrigger(DIK_D))
//		uSaveStage ++;
//	if (GetKeyboardTrigger(DIK_A))
//		uSaveStage --;
//
//	
//	uSaveStage %= MAX_STAGE - 1;	// チュートリアルは選択させない
//
//	// 数字表示切替え
//	pUIObj[UI_NUMBERS]->SetCurrentAnimPattern(uSaveStage + 1);
//
//	// セーブ
//	if (GetKeyboardTrigger(DIK_I))
//	{
//		bIsSelectSaveStage = true;
//		uSelectMode = MODE_EDIT;
//
//		// UI非表示
//		pUIObj[UI_WTHAT_STAGE_SAVE]->SetUsedFlg(false);
//		pUIObj[UI_STAGE]->SetUsedFlg(false);
//		pUIObj[UI_NUMBERS]->SetUsedFlg(false);
//
//		pUIObj[UI_SAVE_OK]->SetUsedFlg(true);
//	}
//	// 元の画面へ
//	if (GetKeyboardTrigger(DIK_U))
//	{
//		// UI非表示
//		pUIObj[UI_WTHAT_STAGE_SAVE]->SetUsedFlg(false);
//		pUIObj[UI_STAGE]->SetUsedFlg(false);
//		pUIObj[UI_NUMBERS]->SetUsedFlg(false);
//
//		uSelectMode = MODE_EDIT;
//	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// オブジェクト削除
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::deleteObj()
{
	/*
	if (GetKeyboardPress(DIK_J))
	{
		if (GetKeyboardTrigger(DIK_K))
		{
			for (int i = 0; i < MAX_GAME_OBJ; i++)
			{
				// 例外処理
				if (!pGameObj[CurrentSelectType][i]->GetUsedFlg() || uSelectObjNum[CurrentSelectType] == i)
					continue;

				pCollision->CheckCollisionBlock(pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]], pGameObj[CurrentSelectType][i]);
			}
		}
	}
	*/

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// オブジェクト移動
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::move()
{
	const auto currentGameObject = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);

	D3DXVECTOR3 blockPos = (*currentGameObject)->getPosition();
	D3DXVECTOR3 blockRot = (*currentGameObject)->getRotation();
	D3DXVECTOR3 blockSize = (*currentGameObject)->getCollisionBox();

	D3DXVECTOR3 pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z);

	if (Keyboard::getPress(DIK_LSHIFT))
	{
		if (Keyboard::getPress(DIK_W))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(0.0f, 0.5f, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}
		if (Keyboard::getPress(DIK_S))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(0.0f, -0.5f, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}

	}

	if (Keyboard::getTrigger(DIK_W))
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z + blockSize.z);
		(*currentGameObject)->setPosition(pos);
		(*currentGameObject)->setUsedFlg(true);
	}

	if (Keyboard::getPress(DIK_A))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos - D3DXVECTOR3(0.2f, 0.0f, 0.0f));
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
	}

	if (Keyboard::getTrigger(DIK_S))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z - blockSize.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
	}

	if (Keyboard::getPress(DIK_D))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos + D3DXVECTOR3(0.2f, 0.0f, 0.0f));
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
	}


	/*
	if (GetKeyboardTrigger(DIKA))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x - BlockSize.x, BlockPos.y, BlockPos.z);
		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	}
	*/
	//// キーボード操作
	//D3DXVECTOR3 BlockPos = gameObjPtr[selectGameObjType] [selectGameObjIndex]->GetPosition();
	//D3DXVECTOR3 BlockRot = pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->GetRotation();
	//D3DXVECTOR3 BlockSize = pGameObj[0][0]->GetCollisionBox() * 0.5f;

	//// 左移動
	//if (GetKeyboardTrigger(DIK_A))
	//{
	//	D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x - BlockSize.x, BlockPos.y, BlockPos.z);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//}
	//if (GetKeyboardTrigger(DIK_D))
	//{
	//	D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x + BlockSize.x, BlockPos.y, BlockPos.z);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//}
	//if (GetKeyboardTrigger(DIK_S))
	//{
	//	D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z - BlockSize.z);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//}
	//if (GetKeyboardTrigger(DIK_W))
	//{
	//	D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z + BlockSize.z);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//}

	//// 高速移動
	//if (GetKeyboardPress(DIK_J))
	//{

	//	if (GetKeyboardPress(DIK_A))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x - BlockSize.x * 0.25f, BlockPos.y, BlockPos.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//	}
	//	if (GetKeyboardPress(DIK_D))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x + BlockSize.x * 0.25f, BlockPos.y, BlockPos.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//	}
	//	if (GetKeyboardPress(DIK_S))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z - BlockSize.z * 0.25f);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);

	//	}
	//	if (GetKeyboardPress(DIK_W))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y, BlockPos.z + BlockSize.z * 0.25f);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//	}


	//}
	//// 上下移動、回転

	//// 設置
	//if (GetKeyboardTrigger(DIK_SPACE))
	//{
	//	// 例外処理
	//	if (uSelectObjNum[CurrentSelectType] < MaxGameObj - 1)
	//	{
	//		uSelectObjNum[CurrentSelectType] ++;
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType] - 1]->GetPosition());
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//	}
	//}

	//// 上下移動
	//if (GetKeyboardPress(DIK_RCONTROL))
	//{
	//	if (GetKeyboardTrigger(DIK_UP))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y + BlockSize.y, BlockPos.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	}

	//	if (GetKeyboardTrigger(DIK_DOWN))
	//	{
	//		D3DXVECTOR3 Pos = D3DXVECTOR3(BlockPos.x, BlockPos.y - BlockSize.y * 2.0f, BlockPos.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetPosition(Pos);
	//	}

	//}

	//if (GetKeyboardPress(DIK_U))
	//{
	//	// 回転
	//	if (GetKeyboardTrigger(DIK_A))
	//	{
	//		D3DXVECTOR3 Rot = D3DXVECTOR3(BlockRot.x, BlockRot.y + D3DXToRadian(90), BlockRot.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetRotation(Rot);
	//	}
	//	if (GetKeyboardTrigger(DIK_D))
	//	{
	//		D3DXVECTOR3 Rot = D3DXVECTOR3(BlockRot.x, BlockRot.y - D3DXToRadian(90), BlockRot.z);
	//		pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetRotation(Rot);
	//	}
	//}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 設置
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::place()
{
	if (!Keyboard::getTrigger(DIK_SPACE))
	{
		return;
	}
	
	const auto currentGameObjectType = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);
	const auto nextGameObjectType	 = std::next(gameObjPtr[selectGameObjType].begin(),	selectGameObjIndex + 1);

	D3DXVECTOR3 blockPos  = (*currentGameObjectType)->getPosition();
	D3DXVECTOR3 blockRot  = (*currentGameObjectType)->getRotation();
	D3DXVECTOR3 blockSize = (*currentGameObjectType)->getCollisionBox();

	(*currentGameObjectType)->setPosition((*currentGameObjectType)->getPosition());
	(*currentGameObjectType)->setUsedFlg(true);
	selectGameObjIndex++;

	(*nextGameObjectType)->setPosition((*currentGameObjectType)->getPosition());
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 回転
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::rotObj()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 選択オブジェクト切り替え
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::changeSelectObj()
{
	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// 現在のブロックを未使用へ
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(false);

	//	CurrentSelectType++;
	//	CurrentSelectType = CurrentSelectType % MAX_GameObjectType;
	//	pGameObj[CurrentSelectType][uSelectObjNum[CurrentSelectType]]->SetUsedFlg(true);
	//}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Camera* StageEditor::getCamera()
{
	return cameraPtr.get();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ゲームオブジェクト生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::creteGameObj(size_t objType)
{
	switch (objType)
	{
	case static_cast<INT>( GameObjectType::NormalBlockObj ) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[0].push_back(NEW MainObject("box.x","IceStone.png",objIndex, GameObjectType::NormalBlockObj,false));
		}
		break;
	case static_cast<INT>(GameObjectType::MoveBlockOBj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[1].push_back(NEW MainObject("flatAndHill.x", "double_1.png",objIndex, GameObjectType::MoveBlockOBj,true));
		}
		break;
	case static_cast<INT>((GameObjectType::FieldObj)):
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[2].push_back(NEW MainObject("flat.x","double_1.png",objIndex, GameObjectType::FieldObj,true));
		}
		break;
	case static_cast<INT>((GameObjectType::GoalObj)) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[3].push_back(NEW Goal("heart.x", "heart.png", objIndex));
		}
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボード生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::createBoard(size_t objType)
{
	switch (objType)
	{
	case static_cast<INT>( BoardObjType::WhatSave):
	{
		boardObjPtr[objType].push_back(std::unique_ptr<WhatStageSaveUi>(NEW WhatStageSaveUi()));
	}
	break;

	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::move(INT input)
{
	const auto currentGameObject = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);

	D3DXVECTOR3 blockPos = (*currentGameObject)->getPosition();
	D3DXVECTOR3 blockRot = (*currentGameObject)->getRotation();
	D3DXVECTOR3 blockSize = (*currentGameObject)->getCollisionBox();

	switch (input)
	{
	case DIK_W:
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z + blockSize.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
		break;
	}
	case DIK_A:
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x - blockSize.x, blockPos.y, blockPos.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
		break;
	}
	case DIK_S:
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z - blockSize.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
		break;
	}
	case DIK_D:
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x + blockSize.x, blockPos.y, blockPos.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
		break;
	}
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 使用オブジェクト更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::updateSelectIndex()
{
	selectGameObjIndex = 0;

	for (auto gameObj : gameObjPtr[selectGameObjType])
	{
		if (gameObj->getUsedFlg())
		{
			selectGameObjIndex ++;
		}
		else
		{
			return;
		}
	}
}

/*
template <class T>
void StageEditor::creteGamesObj(T dataType,size_t size)
{
	for (INT objIndex = 0; objIndex < size; objIndex ++)
	{
		gameObjPtr[objIndex].push_back(NEW T);
	}
	break;
}
*/