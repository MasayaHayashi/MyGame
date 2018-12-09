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
#include "C_StarParticle.h"
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
UINT	SceneMain::prevScore = 0;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneMain::SceneMain()
{
	prevScore = 0;
	lightPtr.reset(NEW Light());
	cameraPtr.reset(NEW Camera());

	playeresPtr.push_back( static_cast<std::unique_ptr<Player>>( NEW Player(D3DXVECTOR3(-2.0f, 0.0f, 0.0f) , playeresPtr.size() )));
	playeresPtr.push_back( static_cast<std::unique_ptr<Player>>( NEW Player(D3DXVECTOR3( 4.0f, 0.0f, 2.0f) , playeresPtr.size()	)));

	gameObjectesPtr.push_back( std::unique_ptr<Pawn>( NEW Skydome())   );
	gameObjectesPtr.push_back( std::unique_ptr<Pawn>( NEW BallObj())   );
	gameObjectesPtr.push_back( std::unique_ptr<Pawn>( NEW MainField()) );

	collisionPtr.reset(NEW Collision(playeresPtr.front().get(),gameObjectesPtr.back().get()));
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
	lightPtr->initialize();
	cameraPtr->initializeMain(playeresPtr.back().get());

	for (const auto &player : playeresPtr)
	{
		player->initialize();
	}

	for (const auto &gameObject : gameObjectesPtr)
	{
		gameObject->initialize();
	}
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

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::update()
{
	for (const auto &gameObject : gameObjectesPtr)
	{
		gameObject->update();
	}

	for (const auto &player : playeresPtr)
	{
		player->update(cameraPtr->getFowerd());
	}	
	
	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneTitle);
	}

	collisionPtr->update();


#if 0
	if (currentGameState == SceneMain::GameState::Tutorial)
	{
		cameraPtr->setState(Camera::MoceType::Tutorial);
	}


	// ゴールから移行処理
	if (uGameState == GAME_GOAL)
	{
		bool bAllUse = true;
		for (INT i = 0; i < MAX_PARTICLE; i++)
		{
			if (pParticleObj[OBJ_2D_STAR_PARTICLE][i]->getUsedFlg())
			{
				bAllUse = false;
				break;
			}
		}
		if (bAllUse)
			getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);
	}

	// ハート更新
	pHeart->updateObject(pPlayer->getOffset());
	
	// ポーズ解除処理
	if (getKeyboardTrigger(DIK_P))
	{
		if (uGameState == GAME_NORMAL)
			uGameState = GAME_PAUSE;
		if (uGameState == GAME_PAUSE)
			uGameState = GAME_NORMAL;
	}

	// 再スタート
	if (uGameState == GAME_MISS)
	{
		nRestartCnt--;
		if (nRestartCnt < 0)
			initializeStatus();
	}

	// UI更新
	for (INT UiTypeCnt = 0; UiTypeCnt < MAX_UI_OBJ_TYPE; UiTypeCnt++)
		pUIObj[UiTypeCnt]->updateObject();

	// スコア更新
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
		pUIScore[i]->updateObject();
	
	// UI更新
	if (uGameState == GAME_MISS)
	{
		pUIObj[OBJ_MISS]->setUsedFlg(true);
		pUIObj[OBJ_BG]->setUsedFlg(true);
	}

	// プレイヤー更新
	pPlayer->updatePlayer_GameMain(pCamera->getCameraFowerd());

	// ゲーム状態更新
	if (!pUIObj[OBJ_READY]->getUsedFlg() && uGameState != GAME_MISS && uGameState != GAME_GOAL && uGameState != GAME_TUTORIAL)
		uGameState = GAME_NORMAL;

	// ポーズ更新
	if (uGameState == GAME_PAUSE)
	{
		pPause->updateObject();
		return;
	}
	if (uGameState == GAME_MISS)
		return;

	// 死亡判定
	if (pPlayer->getState() == PlayerState::TYPE_DEAD)
		uGameState = GAME_MISS;

	// スカイドーム更新
	pSkydome->updateObject();

	// パーティクル更新処理
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
			pParticleObj[TypeCnt][ObjCnt]->updateObject();

	// ゲームオブジェクト更新
	for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
		for (INT j = 0; j < MAX_GAME_OBJ; j++)
			pGameObj[i][j]->updateObject();
	
	// あたり判定チェック
	if (uGameState == GAME_NORMAL || uGameState == GAME_GOAL)
	{
		checkCollision();
	}

#if _DEBUG
	PrintDebugProc("ゲームステート◆%d\n", uGameState);
#endif

	// スコア加算
	for (INT i = 0; i < MAX_PARTICLE; i++)
	{
		if (!pParticleObj[OBJ_2D_STAR_PARTICLE][i]->getUsedFlg())
			continue;

		if (pParticleObj[OBJ_2D_STAR_PARTICLE][i]->getCurveCnt() >= 1.0f)
		{
			pPlayer->addScore();
			pParticleObj[OBJ_2D_STAR_PARTICLE][i]->setCnt(0.0f);
			pParticleObj[OBJ_2D_STAR_PARTICLE][i]->setUsedFlg(false);
		}
	}

	INT nScore = pPlayer->getScore();
	INT nDigit;

	// スコアセット
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
	{
		nDigit = nScore % 10;
		pUIScore[i]->setCurrentAnimPattern(nDigit);
		nScore /= 10;
	}

	// ゴール更新
	pGoal->updateObject();

	// カメラ更新
	pCamera->updateCamera_GameMain(pPlayer,pUIObj[OBJ_READY]);

#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneMain::draw()
{
	for (const auto &gameObject : gameObjectesPtr)
	{
		if (gameObject->isUsedShader())
		{
			gameObject->draw(cameraPtr->getMtxView(), cameraPtr->getProjectionMtx());
		}
		else
		{
			gameObject->draw();
		}
	}

	for (const auto &player : playeresPtr)
	{
		player->draw();
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
	for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
		for (INT j = 0; j < MAX_GAME_OBJ; j++)
			pGameObj[i][j]->drawObject(pCamera->getMtxView(),pCamera->getProjectionMtx());

	// スカイドーム描画
	pSkydome->drawObject();

	// パーティクル描画
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
			pParticleObj[TypeCnt][ObjCnt]->drawObject();

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

	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticleObj[0][i]->setUsedFlg(false);

	// 星の配置を再取得
	pStageLoader->LoadStage(pGameObj, getCurrentStageNum());

	nRestartCnt = RESTART_CNT;

	// スコアリセット
	pPlayer->setScore(0);
	
	// ゲーム状態初期化
	uGameState = GAME_READY;

	// READYアイコン初期化
	pUIObj[OBJ_READY]->setUsedFlg(true);
	pUIObj[OBJ_READY]->setCnt(MAX_READY_CNT);
	pUIObj[OBJ_READY]->setCurrentAnimPattern(4);

	// パーティクル初期化
	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticleObj[OBJ_2D_STAR_PARTICLE][i]->initializeStatus();

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
		if (!pGameObj[STAR_OBJ][i]->getUsedFlg())
			continue;

		// AABB衝突判定
		if (pCollision->IsHitAABBItem(pPlayer, pGameObj[STAR_OBJ][i]))
		{
			pGameObj[STAR_OBJ][i]->setUsedFlg(false);
			INT particleCnt = 0;

//			PlaySound(SOUND_SE_STAR);	// 音再生

			/*
			// パーティクル生成
			for (INT ParticleCnt = 0; ParticleCnt < MAX_PARTICLE; ParticleCnt++)
			{
				if (pParticleObj[OBJ_BILLBOARD_STAR_PARTICLE][ParticleCnt]->getUsedFlg())
					continue;

				particleCnt++;
				pParticleObj[OBJ_BILLBOARD_STAR_PARTICLE][ParticleCnt]->setUsedFlg(true);
				pParticleObj[OBJ_BILLBOARD_STAR_PARTICLE][ParticleCnt]->setPosition(pGameObj[STAR_OBJ][i]->getPosition());

				if (ONE_USE_PARTICLLE < particleCnt)
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

	D3DXVECTOR3 StarObj = pGameObj[STAR_OBJ][Indx]->getPosition();
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

	INT UseParticleCnt = 0;

	for (INT ParticleCnt = 0; ParticleCnt < MAX_PARTICLE; ParticleCnt++)
	{
		// 例外処理
		if (pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->getUsedFlg())
			continue;

		UseParticleCnt ++;

		pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->setUsedFlg(true);
		pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->setPosition(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));
		pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->setStartCurvePos(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));

		if (UseParticleCnt > ONE_USE_PARTICLLE)
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