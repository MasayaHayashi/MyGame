//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneMain.cpp
// Author    : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "../Main/SceneMain.h"
#include "../../Light/Light.h"
#include "../../Camera/Camera.h"
#include "../../Player/Player.h"
#include "../../Skydome/Skydome.h"
#include "../../MainField/MainField.h"
#include "../../Collision/Collision.h"
#include "../../Ball/BallObj.h"
#include "../../MyDelete/MyDelete.h"
#include "../../StarParticle/StarParticle.h"
#include "../../MainObject/MainObject.h"
#include "../../Countdown/Countdown.h"
#include "../../Goal/Goal.h"
#include "../../StageClearUI/StageClearUI.h"
#include "../../MissUI/MissUI.h"
#include "../../Audio/MyAudiere.h"
#include "../../GameManager/GameManager.h"
#include "../../SpikeBlock/SpikeBlock.h"
#include "../../HeartObj/HeartObj.h"
#include "../../StarItem/StarItem.h"
#include "../../StarUI/StarUI.h"
#include <fstream>

// ===== 静的メンバ変数 =====
UINT	SceneMain::currentStage = 0;
UINT	SceneMain::prevScore	= 0;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneMain::SceneMain()
{
	prevScore = 0;
	lightPtr.reset( NEW Light());
	cameraPtr.reset(NEW Camera());

	playeresPtr.push_back(static_cast<std::unique_ptr<Player>>( NEW Player(D3DXVECTOR3(-2.0f, 0.0f, 0.0f), playeresPtr.size() )));

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		creteGameObj(gameObjTypeIndex);
	}

	skyDomePtr.push_back(		std::unique_ptr<Skydome>(	NEW Skydome())	);
	
	for (int index = 0; index < StarParticle::MaxParticle; index++)
	{
		boardObjectesPtr.push_back(std::unique_ptr<Board>(  NEW StarParticle(index) ));
	}

	boardObjectesPtr.push_back( std::unique_ptr<Board>(		NEW StageClearUI()  ));
	boardObjectesPtr.push_back( std::unique_ptr<Board>(		NEW MissUI()	    ));
	boardObjectesPtr.push_back( std::unique_ptr<Board>(		NEW StarUI()));
	boardObjectesPtr.push_back( std::unique_ptr<Board>(		NEW Countdown())	);

	collisionPtr.reset( NEW Collision() );

	for (auto& player : playeresPtr)
	{
		collisionPtr->registerPlayer(player.get());
	}

	for (size_t gameObjType = 0; gameObjType < GameObjectBase::MaxGameObjType; gameObjType++)
	{
		for (auto &gameObj : gameObjPtr[gameObjType])
		{
			if (gameObj->getGameObjectType() == GameObjectType::NormalBlockObj || 
				gameObj->getGameObjectType() == GameObjectType::SpikeObj)
			{
				collisionPtr->registerBlock(*gameObj);
			}
		}
	}

	for (size_t gameObjType = 0; gameObjType < GameObjectBase::MaxGameObjType; gameObjType ++)
	{
		for (auto &gameObj : gameObjPtr[gameObjType])
		{
			if (gameObj->getIsFieldObject())
			{
				collisionPtr->registerField(gameObj.get());
			}
		}
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneMain::~SceneMain()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::initialize()
{
	for (size_t gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->initialize();
		}
	}

	loadStageData(1);
	
	lightPtr->initialize();

	playeresPtr.front()->initialize();
	
	cameraPtr->initializeMain(playeresPtr.back().get());

	for (const auto &skyDome : skyDomePtr)
	{
		skyDome->initialize();
	}

	for (const auto &boardObject : boardObjectesPtr)
	{
		boardObject->initialize();
	}

	MyAudiere::getBgm(0)->setRepeat(true);
	MyAudiere::getBgm(0)->setVolume(0.1f);
	MyAudiere::getBgm(0)->play();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::finalize()
{
	for (const auto &player : playeresPtr)
	{
		player->finalize();
	}

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->finalize();
		}
	}

	collisionPtr->finalize("Player");
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::update()
{
	if (Keyboard::getTrigger(DIK_2))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneTitle);
	}

	for (const auto &skyDome : skyDomePtr)
	{
		skyDome->update();
	}

	for (const auto &player : playeresPtr)
	{
		player->update(cameraPtr->getFowerd());
	}
	
	for (const auto &boardObject : boardObjectesPtr)
	{
		boardObject->update();
	}

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->update();
		}
	}

	cameraPtr->update(*playeresPtr.front().get(), boardObjectesPtr.back()->getCurrentAnim());
	
	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}

	collisionPtr->update();

	if (GameManager::isGameType(GameManager::GameType::Miss))
	{
		restartCnt ++;

		if (restartCnt > RestartFream)
		{
			initializeStatus();
			GameManager::changeGameType(GameManager::GameType::Ready);
			restartCnt = 0;
		}
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::draw()
{
	for (const auto &gameObject : skyDomePtr)
	{
		gameObject->draw();
	}

	for (const auto &boardObject : boardObjectesPtr)
	{
		boardObject->draw();
	}

	playeresPtr.front()->draw();

	for (size_t gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->draw();
		}
	}

	cameraPtr->setCamera();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::initializeStatus()
{
	// プレイヤーステータス初期化
	playeresPtr.front()->initializeStatus();

	for (const auto &boardObject : boardObjectesPtr)
	{
		boardObject->initializeStatus();
	}

	collisionPtr->initilize("Player", 0);
	collisionPtr->initilize();

	cameraPtr->initializeStatus();

	for (const auto &gameObj : gameObjPtr)
	{
		for (const auto &list : gameObj)
		{
			if (list->getUsedFlg() && 
				list->getGameObjectType() == GameObjectType::SpikeObj)
			{
				list->initializeStatus();
			}
		}
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 3Dから2Dへの変換計算用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::checkUnProject(INT Indx)
{
#if 0

	D3DXVECTOR3 StarObj = pGameObj[Star_OBJ][Indx]->getPosition();
	D3DXMATRIX  ViewMtx = pCamera->getMtxView();
	D3DXMATRIX  ProjectionMtx = pCamera->getProjectionMtx();

	D3DXVECTOR3 OutVec;
	D3DXVECTOR3 OutVec2;

	D3DXVec3TransformCoord(&OutVec, &StarObj, &ViewMtx);
	D3DXVec3TransformCoord(&OutVec2, &OutVec, &ProjectionMtx);

	OutVec2.y *= -1;

	OutVec2.x *= 0.5f;
	OutVec2.y *= 0.5f;

	OutVec2.x += 0.5f;
	OutVec2.y += 0.5f;

	OutVec2.x *= SCREEN_WIDTH;
	OutVec2.y *= SCREEN_HEIGHT;

	INT UseStarCnt = 0;

	for (INT StarCnt = 0; StarCnt < MAX_Star; StarCnt++)
	{
		// 例外処理
		if (pStarObj[OBJ_2D_Star_Star][StarCnt]->getUsedFlg())
			continue;

		UseStarCnt ++;

		pStarObj[OBJ_2D_Star_Star][StarCnt]->setUsedFlg(true);
		pStarObj[OBJ_2D_Star_Star][StarCnt]->setPosition(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));
		pStarObj[OBJ_2D_Star_Star][StarCnt]->setStartCurvePos(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));

		if (UseStarCnt > ONE_USE_PARTICLLE)
			break;
	}

#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// データ読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::loadStageData(size_t stageNumber)
{
	std::ifstream loadtFile;

	std::string stageString = std::to_string(stageNumber);
	loadtFile.open(StageFilePass + stageString + ".bin", std::ios::binary);

	if (!loadtFile)
	{
		MessageBox(nullptr, TEXT("Error"), TEXT("ステージデータ読み込みエラー"), MB_ICONERROR);
		return;
	}

	for (size_t gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			loadtFile.read((CHAR*)(&exportWorkData), sizeof(ExportData));

			if (exportWorkData.isUsed)
			{
				selectGameObjIndex++;
			}

			(*itr)->reflectionExportData(exportWorkData);
		}
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ゲームオブジェクト生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::creteGameObj(size_t objType)
{
	switch (objType)
	{
	case static_cast<INT>(GameObjectType::NormalBlockObj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[0].push_back(static_cast<std::unique_ptr<MainObject>>( NEW MainObject("Spikes.x","IceStone.png",objIndex,GameObjectType::NormalBlockObj,false)));
		}
		break;
	case static_cast<INT>(GameObjectType::MoveBlockOBj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[1].push_back(static_cast<std::unique_ptr<MainObject>>( NEW MainObject("flatAndHill.x","double_1.png", objIndex, GameObjectType::MoveBlockOBj,true)));
		}
		break;
	case static_cast<INT>((GameObjectType::FieldObj)) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[2].push_back(static_cast<std::unique_ptr<MainObject>>( NEW MainObject("flat.x","double_1.png", objIndex, GameObjectType::FieldObj,true)));
		}
		break;
	case static_cast<INT>((GameObjectType::GoalObj)) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[3].push_back(static_cast<std::unique_ptr<MainObject>>( NEW Goal("heart.x", "heart.png", objIndex)));
		}
		break; 
	case static_cast<INT>(GameObjectType::SpikeObj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[4].push_back(static_cast<std::unique_ptr<MainObject>>( NEW SpikeBlock("block_spike1.x", "cube_metal_unity.tga", objIndex, GameObjectType::SpikeObj, false)));
		}
		break;
	case static_cast<INT>(GameObjectType::StarObj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[5].push_back(static_cast<std::unique_ptr<MainObject>>(NEW StarItem("star.x", "star.png", objIndex, GameObjectType::StarObj, false)));
		}
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アイテム生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::createItem(size_t objType)
{

}