//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// StageEditor.cpp
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
#include "../../SpikeBlock/SpikeBlock.h"
#include "../../StarItem/StarItem.h"
#include <fstream>

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
StageEditor::StageEditor()
{
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

	lightPtr.reset( NEW Light());
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

	collisionPtr->finalize("Player");
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
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

	saveStageData(1);

	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}
	
	cameraPtr->updateStageEdit(gameObjPtr[selectGameObjType][selectGameObjIndex]->getKeyName(),selectGameObjIndex);
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

			if (exportWorkData.myTransform.isUsed)
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

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// オブジェクト削除
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::deleteObj()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// オブジェクト移動
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::move()
{
	const auto currentGameObject = std::next(gameObjPtr[selectGameObjType].begin(), selectGameObjIndex);

	D3DXVECTOR3 blockPos = (*currentGameObject)->getPosition();
	D3DXVECTOR3 blockRot = (*currentGameObject)->getRotation();
	D3DXVECTOR3 size = (*currentGameObject)->getCollisionBox();
	D3DXVECTOR3 blockSize = D3DXVECTOR3(0.5f,0.5f,0.5f);

	D3DXVECTOR3 pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z);

	if (Keyboard::getPress(DIK_LCONTROL))
	{
		if (Keyboard::getPress(DIK_W))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(0.0f, blockSize.y, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}
		if (Keyboard::getPress(DIK_S))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(0.0f, -blockSize.y, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}

		if (Keyboard::getPress(DIK_A))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(blockSize.x, 0.0f, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}
		if (Keyboard::getPress(DIK_D))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(-blockSize.x, 0.0f, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}


	}

	if (Keyboard::getPress(DIK_LSHIFT))
	{
		if (Keyboard::getPress(DIK_W))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(0.0f, 0.0f, blockSize.z));
			(*currentGameObject)->setUsedFlg(true);
		}
		if (Keyboard::getPress(DIK_S))
		{
			(*currentGameObject)->setPosition(pos - D3DXVECTOR3(0.0f, 0.0f, blockSize.z));
			(*currentGameObject)->setUsedFlg(true);
		}

		if (Keyboard::getPress(DIK_A))
		{
			(*currentGameObject)->setPosition(pos - D3DXVECTOR3(blockSize.x, 0.0f, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}
		if (Keyboard::getPress(DIK_D))
		{
			(*currentGameObject)->setPosition(pos + D3DXVECTOR3(blockSize.x, 0.0f, 0.0f));
			(*currentGameObject)->setUsedFlg(true);
		}

	}


	if (Keyboard::getTrigger(DIK_W))
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z + blockSize.z);
		(*currentGameObject)->setPosition(pos);
		(*currentGameObject)->setUsedFlg(true);
	}

	if (Keyboard::getTrigger(DIK_A))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x - blockSize.x ,blockPos.y ,blockPos.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
	}

	if (Keyboard::getTrigger(DIK_S))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x, blockPos.y, blockPos.z - blockSize.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
	}

	if (Keyboard::getTrigger(DIK_D))
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(blockPos.x + blockSize.x,blockPos.y,blockPos.z);
		(*currentGameObject)->setPosition(Pos);
		(*currentGameObject)->setUsedFlg(true);
	}
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

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ゲームオブジェクト生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StageEditor::creteGameObj(size_t objType)
{
	switch (objType)
	{
	case static_cast<INT>( GameObjectType::NormalBlockObj ) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex ++)
		{
			gameObjPtr[0].push_back(NEW MainObject("Spikes.x","IceStone.png",objIndex, GameObjectType::NormalBlockObj,false));
		}
		break;
	case static_cast<INT>(GameObjectType::MoveBlockOBj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex ++)
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
	case static_cast<INT>((GameObjectType::SpikeObj)) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[4].push_back(NEW SpikeBlock("block_spike1.x", "cube_metal_unity.tga", objIndex, GameObjectType::SpikeObj, false));
		}
		break;
	case static_cast<INT>((GameObjectType::StarObj)) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[5].push_back(NEW StarItem("star.x", "star.png", objIndex, GameObjectType::StarObj , false));
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