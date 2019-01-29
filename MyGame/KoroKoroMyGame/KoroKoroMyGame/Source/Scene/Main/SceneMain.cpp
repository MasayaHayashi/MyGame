//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneTitle.cpp
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
#include "../../Audio/MyAudiere.h"
#include <fstream>

/*
#include "C_Board.h"
#include "C_Pause.h"
#include "C_Player.h"
#include "C_Enemy.h"
#include "C_Light.h"
#include "C_Camera.h"
#include "C_Skydome.h"
#include "C_velocityBlock.h"
#include "C_GoalObj.h"
#include "C_Pause.h"
#include "C_ItemStar.h"
#include "input.h"
#include "C_Ready.h"
#include "C_StarStar.h"
#include "C_MissUI.h"
#include "C_GoalUI.h"
#include "C_StageLoader.h"
#include "C_BgEffect.h"
#include "C_ScoreUi.h"
#include "C_StarUI.h"
#include "C_Goal.h"
#include "C_TutorialUI.h"
#include "C_HeartBoard.h"
#include "C_NextUI.h"
#include "C_Audio.h"

#if _DEBUG
	#include "debugproc.h"
#endif
*/

// ===== 静的メンバ変数 =====
UINT	SceneMain::currentStage = 0;
UINT	SceneMain::prevScore	= 0;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneMain::SceneMain()
{
	prevScore = 0;
	lightPtr.reset(NEW Light());
	cameraPtr.reset(NEW Camera());

	playeresPtr.push_back(static_cast<std::unique_ptr<Player>>(NEW Player(D3DXVECTOR3(-2.0f, 0.0f, 0.0f), playeresPtr.size())));

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		creteGameObj(gameObjTypeIndex);
	}

	skyDomePtr.push_back(		std::unique_ptr<Skydome>(	NEW Skydome())	);

	for (int i = 0; i < StarParticle::MaxParticle; i++)
	{
		boardObjectesPtr.push_back(std::unique_ptr<Board>(NEW StarParticle()));
	}

	boardObjectesPtr.push_back( std::unique_ptr<Board>(		NEW Countdown() ));
	boardObjectesPtr.push_back( std::unique_ptr<Board>(		NEW StageClearUI() ));
	collisionPtr.reset( NEW Collision() );

	for (auto& player : playeresPtr)
	{
		collisionPtr->registerPlayer(player.get());
	}

	for (size_t gameObjType = 0; gameObjType < GameObjectBase::MaxGameObjType; gameObjType++)
	{
		for (auto gameObj : gameObjPtr[gameObjType])
		{
			if (gameObj->getGameObjectType() == GameObjectType::NormalBlockObj)
			{
				collisionPtr->registerBlock(*gameObj);
			}
		}
	}


	for (size_t gameObjType = 0; gameObjType < GameObjectBase::MaxGameObjType; gameObjType ++)
	{
		for (auto gameObj : gameObjPtr[gameObjType])
		{
			if (gameObj->getIsFieldObject())
			{
				collisionPtr->registerField(gameObj);
			}
		}
	}
	

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneMain::~SceneMain()
{
	for (auto& ballPtr : ballsPtr)
	{
		ballPtr->finalize();
		Mydelete::safeDelete(ballPtr);
	}
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

	cameraPtr->update(*playeresPtr.front().get(), *boardObjectesPtr.back());
	
	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}

	collisionPtr->update();
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

#if 0

	// ポーズ処理
	if (uGameState == GAME_PAUSE)
	{
		// ポーズ描画
		pPause->drawObject();
	}

	// プレイヤー描画
	pPlayer->drawObject();

	// ゲームオブジェクト描画
	for (INT i = 0; i < MAX_GameObjectType; i++)
		for (INT j = 0; j < MAX_GAME_OBJ; j++)
			pGameObj[i][j]->drawObject(pCamera->getMtxView(),pCamera->getProjectionMtx());

	// スカイドーム描画
	pSkydome->drawObject();

	// パーティクル描画
	for (INT TypeCnt = 0; TypeCnt < MAX_Star_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_Star; ObjCnt++)
			pStarObj[TypeCnt][ObjCnt]->drawObject();

	// カメラセット
	pCamera->setCamera();

	// スコア描画
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
		pUIScore[i]->drawObject();

	// UI描画
	for (INT ObjCnt = 0; ObjCnt < MAX_UI_OBJ_TYPE; ObjCnt++)
		pUIObj[ObjCnt]->drawObject();

	// ハート描画
	pHeart->drawObject();

	// ゴール描画
	pGoal->drawObject();

#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::initializeStatus()
{
#if 0

	// BGM再生
//	PlaySound(SOUND_BGM_MAIN);

	// プレイヤーステータス初期化
	pPlayer->initializeStatus();

	// UI使用フラグ更新
	pUIObj[OBJ_MISS]->setUsedFlg(false);
	pUIObj[OBJ_BG]->setUsedFlg(false);

	for (INT i = 0; i < MAX_Star; i++)
		pStarObj[0][i]->setUsedFlg(false);

	// 星の配置を再取得
	pStageLoader->LoadStage(pGameObj, getCurrentStageNum());

	nReStartCnt = REStarT_CNT;

	// スコアリセット
	pPlayer->setScore(0);
	
	// ゲーム状態初期化
	uGameState = GAME_READY;

	// READYアイコン初期化
	pUIObj[OBJ_READY]->setUsedFlg(true);
	pUIObj[OBJ_READY]->setCnt(MAX_READY_CNT);
	pUIObj[OBJ_READY]->setCurrentAnimPattern(4);

	// パーティクル初期化
	for (INT i = 0; i < MAX_Star; i++)
		pStarObj[OBJ_2D_Star_Star][i]->initializeStatus();

	// ステータス初期化
	pUIObj[OBJ_MISS]->initializeStatus();

#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Camera* SceneMain::getCamera()
{
	return cameraPtr.get();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 判定関連
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::checkCollision()
{
#if 0

	// 例外処理
	if (pPlayer->getState() == PlayerState::TYPE_JUMP_UP)
		return;

	// ブロックとの判定
	uIsHitBlock = NONE;
	D3DXVECTOR3 Cross, Normal;

	INT nHitIndex = 0;
	D3DXVECTOR3 fLength = D3DXVECTOR3(0,0,0);




	// 移動方向に壁があるか
	D3DXVECTOR3 velocityVec = pPlayer->getvelocityVec();
	D3DXVec3Normalize(&velocityVec, &velocityVec);

	// ブロックとの判定
	uIsHitBlock = NONE;
	Cross = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	nHitIndex = 0;
	fLength = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



	if (pPlayer->getState() != PlayerState::TYPE_JUMP_DOWN)
	{
		for (INT i = 0; i < MAX_BLOCK_TYPE; i++)
		{
			for (INT j = 0; j < MAX_GAME_OBJ; j++)
			{
				// 例外処理
				if (!pGameObj[i][j]->getUsedFlg())
					continue;

				// プレイヤーとブロックとの判定
				uIsHitBlock = pCollision->CheckCollisionWall(pPlayer, nullptr, pGameObj[i][j], Cross, Normal, fLength, velocityVec);

				nHitIndex = j;

				if (uIsHitBlock == RAY_TRUE)
					break;
			}
			if (uIsHitBlock == RAY_TRUE)
				break;
		}

		if (uIsHitBlock == RAY_TRUE)
		{
			if (fLength.x < 0.4f)
			{
				pPlayer->setStatus(PlayerState::TYPE_velocity_HIT_WALL);
				pPlayer->setPosition(pPlayer->getPosition() - pPlayer->getvelocityVec());
				PrintDebugProc("てすとおおおおおおおおおおおおおおおおおおおおおお");
			}
		}

	}




	// 床との判定
	if (pPlayer->getState() != (PlayerState::TYPE_velocity_HIT_WALL))
	{
		for (INT i = 0; i < MAX_BLOCK_TYPE; i++)
		{
			for (INT j = 0; j < MAX_GAME_OBJ; j++)
			{
				// 例外処理
				if (!pGameObj[i][j]->getUsedFlg())
					continue;

				// プレイヤーとブロックとの判定
				uIsHitBlock = pCollision->CheckCollisionField(pPlayer, nullptr, pGameObj[i][j], Cross, Normal, fLength, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
				nHitIndex = j;

				if (uIsHitBlock == RAY_TRUE)
					break;
			}
			if (uIsHitBlock == RAY_TRUE)
				break;
		}


		bool bSphereHit = false;

		FLOAT CheckY = pPlayer->getPosition().y - Cross.y;
		if (CheckY < 0.0f)
			CheckY *= -1;

		if (uIsHitBlock == RAY_TRUE)
		{
			if (fLength.x < 0.1f)
			{
				PrintDebugProc("ヒット！！！！！！！！！！１");
				PrintDebugProc("hit%d", nHitIndex);
				pPlayer->setPosition(Cross - D3DXVECTOR3(0.0f, 0.01f, 0.0f));
				pPlayer->setStatus(PlayerState::TYPE_velocity);
			}

			if (fLength.x > 0.0f)
			{
				pPlayer->setStatus(PlayerState::TYPE_JUMP_DOWN);
			}
		}

		else if (uIsHitBlock == RAY_FALSE)
		{
			pPlayer->setStatus(PlayerState::TYPE_FALL);
			PrintDebugProc("のーひっと！！！！！！！！！！１");

			PrintDebugProc("のーひっと");

		}
		else if (uIsHitBlock == NONE)
		{
			PrintDebugProc("のー");
			pPlayer->setIsGround(false);
		}
	}
	PrintDebugProc("\nCross%f\n", Cross.x);
	PrintDebugProc("Cross%f\n", Cross.y);
	PrintDebugProc("Cross%f\n", Cross.z);






	// アイテムとの判定
	for (INT i = 0; i < MAX_GAME_OBJ; i++)
	{
		// 例外処理
		if (!pGameObj[Star_OBJ][i]->getUsedFlg())
			continue;

		// AABB衝突判定
		if (pCollision->IsHitAABBItem(pPlayer, pGameObj[Star_OBJ][i]))
		{
			pGameObj[Star_OBJ][i]->setUsedFlg(false);
			INT StarCnt = 0;

//			PlaySound(SOUND_SE_Star);	// 音再生

			/*
			// パーティクル生成
			for (INT StarCnt = 0; StarCnt < MAX_Star; StarCnt++)
			{
				if (pStarObj[OBJ_BILLBOARD_Star_Star][StarCnt]->getUsedFlg())
					continue;

				StarCnt++;
				pStarObj[OBJ_BILLBOARD_Star_Star][StarCnt]->setUsedFlg(true);
				pStarObj[OBJ_BILLBOARD_Star_Star][StarCnt]->setPosition(pGameObj[Star_OBJ][i]->getPosition());

				if (ONE_USE_PARTICLLE < StarCnt)
					break;
			}
			*/

			// 3D→2D座標変換
			checkUnProject(i);
		}
	}

	// ゴール判定
	for (INT i = 0; i < MAX_GAME_OBJ; i++)
	{
		// 例外処理
		if (!pGameObj[GOAL_OBJ][i]->getUsedFlg())
			continue;

		// AABB衝突判定
		if (pCollision->IsHitAABBItem(pPlayer, pGameObj[GOAL_OBJ][i]))
		{

			// ゴールフラグ更新
			if (uGameState != GAME_GOAL)
			{
//				PlaySound(SOUND_SE_STAGE_CLEAR);
				pCamera->setCameraState(MAIN_GOAL_FADE);
				uGameState = GAME_GOAL;
				AddStage();		// ステージ情報加算
		//		getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);
			}
		}
	}

#endif
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
// スコアセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::setScore()
{

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
			gameObjPtr[0].push_back(NEW MainObject("box.x","IceStone.png",objIndex,GameObjectType::NormalBlockObj,false));
		}
		break;
	case static_cast<INT>(GameObjectType::MoveBlockOBj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[1].push_back(NEW MainObject("flatAndHill.x","double_1.png", objIndex, GameObjectType::MoveBlockOBj,true));
		}
		break;
	case static_cast<INT>((GameObjectType::FieldObj)) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[2].push_back(NEW MainObject("flat.x","double_1.png", objIndex, GameObjectType::FieldObj,true));
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