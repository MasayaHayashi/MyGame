//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_StageEditor.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "../Editor/StageEditor.h"
#include "../../Light/Light.h"
#include "../../Camera/Camera.h"
#include "../../Player/Player.h"
#include "../../Block/Block.h"
#include "../../MyDelete/MyDelete.h"
#include "../../EditUI/WhatStageSaveUI.h"
#include <stdio.h>

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
	cameraPtr->initialize();
	playerPtr->initialize();

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
	//for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
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

	for (UINT gameObjIndex = 0; gameObjIndex < GameObjectBase::MaxGameObjType; gameObjIndex++)
	{
		for (auto itr = gameObjPtr[gameObjIndex].begin(); itr != gameObjPtr[gameObjIndex].end(); itr++)
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
	//for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
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
	
	cameraPtr->updateStageEdit(selectGameObjIndex);

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
//	for (int j = 0; j < MAX_GAME_OBJ_TYPE; j++)
//		for (int i = 0; i < MAX_GAME_OBJ; i++)
//			pGameObj[j][i]->UpdateObject();
//
//	if (uSelectMode == MODE_EDIT)
//	{
//		// 判定処理
//		D3DXVECTOR3 Cross, Normal;
//		D3DXVECTOR3 fLength;
//
//		for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
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
	//for (int i = 0; i < MAX_GAME_OBJ_TYPE; i++)
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
void StageEditor::saveStageData()
{
//{
//	// ファイルオープン
//	nErrorFopen = fopen_s(&fp,&szStageFileName[uSaveStage + 1][0], "wb");
//
//	// 例外処理
//	if (nErrorFopen)
//		return;
//
//	for (int j = 0; j < MAX_GAME_OBJ_TYPE; j++)
//		for (int i = 0; i < MAX_GAME_OBJ; i++)
//			fwrite(pGameObj[j][i]->GetExportData(), sizeof(EXPORT_GAMEOBJ_DATA), 1, fp);
//
//	// ファイルクローズ
//	fclose(fp);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステージデータ読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::loadStageData(UINT LoadStageNum)
{
//	// ファイルオープン
//	nErrorFopen = fopen_s(&fp, &szStageFileName[LoadStageNum + 1][0], "rb");
//
//	// 例外処理
//	if (nErrorFopen)
//		return;
//
//#if 1
//
//	// ファイル読み込み
//	INT nLoadCnt[MAX_GAME_OBJ_TYPE] = { 0, 0, 0, };
//
//	for (INT i = 0; i < MAX_GAME_OBJ * MAX_GAME_OBJ_TYPE; i++)
//	{
//		fread(&ExportWorkData, sizeof(EXPORT_GAMEOBJ_DATA), 1, fp);
//		switch (ExportWorkData.ObjType)
//		{
//		case NORMAL_BLOCK_OBJ:
//			// 読み込みデータセット
//			pGameObj[NORMAL_BLOCK_OBJ][nLoadCnt[NORMAL_BLOCK_OBJ]]->SetExportData(ExportWorkData);
//
//			// 選択番号更新
//			if (pGameObj[NORMAL_BLOCK_OBJ][nLoadCnt[NORMAL_BLOCK_OBJ]]->GetUsedFlg())
//				uSelectObjNum[NORMAL_BLOCK_OBJ] ++;
//
//			// 読み込みカウンタ更新
//			nLoadCnt[NORMAL_BLOCK_OBJ] ++;
//
//			break;
//
//		case MOVE_BLOCK_OBJ:
//			// 読み込みデータセット
//			pGameObj[MOVE_BLOCK_OBJ][nLoadCnt[MOVE_BLOCK_OBJ]]->SetExportData(ExportWorkData);
//
//			// 選択番号更新
//			if (pGameObj[MOVE_BLOCK_OBJ][nLoadCnt[MOVE_BLOCK_OBJ]]->GetUsedFlg())
//				uSelectObjNum[MOVE_BLOCK_OBJ] ++;
//
//			// 読み込みカウンタ更新
//			nLoadCnt[MOVE_BLOCK_OBJ] ++;
//
//			break;
//
//		case STAR_OBJ:
//			// 読み込みデータセット
//			pGameObj[STAR_OBJ][nLoadCnt[STAR_OBJ]]->SetExportData(ExportWorkData);
//
//			// 選択番号更新
//			if (pGameObj[STAR_OBJ][nLoadCnt[STAR_OBJ]]->GetUsedFlg())
//				uSelectObjNum[STAR_OBJ] ++;
//
//			// 読み込みカウンタ更新
//			nLoadCnt[STAR_OBJ] ++;
//
//			break;
//		default:
//			break;
//		}
//	}
//
//#endif
//
//	// ファイルクローズ
//	fclose(fp);
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
	if (Keyboard::getTrigger(DIK_W))
	{
		move(DIK_W);
	}

	if (Keyboard::getTrigger(DIK_A))
	{
		move(DIK_A);
	}

	if (Keyboard::getTrigger(DIK_S))
	{
		move(DIK_S);
	}

	if (Keyboard::getTrigger(DIK_D))
	{
		move(DIK_D);
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

	const auto currentGameObject = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);

	D3DXVECTOR3 blockPos = (*currentGameObject)->getPosition();
	D3DXVECTOR3 blockRot = (*currentGameObject)->getRotation();
	D3DXVECTOR3 blockSize = (*currentGameObject)->getCollisionBox();

	(*currentGameObject)->setPosition((*currentGameObject)->getPosition());
	(*currentGameObject)->setUsedFlg(true);
	selectGameObjIndex++;
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
	//	CurrentSelectType = CurrentSelectType % MAX_GAME_OBJ_TYPE;
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
	case static_cast<INT>( GAME_OBJ_TYPE::NormalBlockObj ):
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[objType].push_back(NEW Block(objIndex));
		}

		break;
	case static_cast<INT>(GAME_OBJ_TYPE::MoveBlockOBj):
		break;
	case static_cast<INT>((GAME_OBJ_TYPE::StarObj)):
		break;
	case static_cast<INT>((GAME_OBJ_TYPE::GoalObj)):
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