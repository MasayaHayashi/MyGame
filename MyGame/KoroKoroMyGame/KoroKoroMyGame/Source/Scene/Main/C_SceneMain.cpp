//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneTitle.cpp
// シーンメイン
// Author    : MasayaHayashi
// 更新履歴	 :
//				2018/09/16  ゲームステート関連のコードを読みやすいように変更
//							落下処理のバグを修正
//
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "C_SceneMain.h"
#include "C_Board.h"
#include "C_Pause.h"
#include "C_Player.h"
#include "C_Enemy.h"
#include "C_Light.h"
#include "C_Camera.h"
#include "C_Skydome.h"
#include "C_MoveBlock.h"
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

// ===== 定数・マクロ定義 =====
#define RESTART_CNT (120)	// ミスしてから再スタートの間隔
#define STAGE_TUTORIAL_FILE_NAME "StageTutorial.bin"
#define STAGE_1_FILE_NAME		 "Stage1.bin"
#define STAGE_2_FILE_NAME		 "Stage2.bin"
#define STAGE_3_FILE_NAME		 "Stage3.bin"

// ===== 静的メンバ変数 =====
UINT	C_SCENE_MAIN::uCurrentStage = 0;	// 現在のステージ

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_SCENE_MAIN::C_SCENE_MAIN()
{
	uPrevScore = 0;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_SCENE_MAIN::~C_SCENE_MAIN()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_MAIN::InitScene()
{
	// プレイヤー初期化
	pPlayer = NEW C_PLAYER;
	pPlayer->InitObject();

	// ライト初期化
	pLight = NEW C_LIGHT;
	pLight->InitLight();

	// カメラ初期化
	pCamera = NEW C_CAMERA;
	pCamera->InitCamera(pPlayer);

	// スカイドーム初期化
	pSkydome = NEW C_SKYDOME;
	pSkydome->InitObject();

	// パーティクル初期化
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
		{
			switch (TypeCnt)
			{
				case OBJ_BILLBOARD_STAR_PARTICLE:
					pParticleObj[TypeCnt][ObjCnt] = NEW C_STAR_PARTICLE();
					break;
				case OBJ_2D_STAR_PARTICLE:
					pParticleObj[TypeCnt][ObjCnt] = NEW C_PARTICLE_BASE(ObjCnt);
					break;
				default:
					break;
			}
			pParticleObj[TypeCnt][ObjCnt]->InitObject();
		}

	// UI初期化
	for (INT UiTypeCnt = 0; UiTypeCnt < MAX_UI_OBJ_TYPE; UiTypeCnt++)
	{
		switch (UiTypeCnt)
		{
		case OBJ_READY :
			pUIObj[UiTypeCnt] = NEW C_READY();
			break;
		case OBJ_MISS :
			pUIObj[UiTypeCnt] = NEW C_MISS_UI();
			break;
		case OBJ_BG:
			pUIObj[UiTypeCnt] = NEW C_BG_EFFECT();
			break;
		case OBJ_STAR:
			pUIObj[UiTypeCnt] = NEW C_STAR_UI();
			break;
		case OBJ_TUTORIAL:
			pUIObj[UiTypeCnt] = NEW C_TUTORIAL_UI();
			break;
		case OBJ_NEXT:
			pUIObj[UiTypeCnt] = NEW C_NEXT_UI();
			break;
		default:
			break;
		}
		pUIObj[UiTypeCnt]->InitObject();
	}

	// ゲームオブジェクト初期化
	for (INT BlockType = 0; BlockType < MAX_GAME_OBJ_TYPE; BlockType++)
		for (INT i = 0; i < MAX_GAME_OBJ; i++)
		{
			switch (BlockType)
			{
			case NORMAL_BLOCK_OBJ:
				pGameObj[BlockType][i] = NEW C_BLOCK(i);
				break;
			case MOVE_BLOCK_OBJ:
				pGameObj[BlockType][i] = NEW C_MOVE_BLOCK(i);
				break;
			case STAR_OBJ:
				pGameObj[BlockType][i] = NEW C_ITEM_STAR();
				break;
			case GOAL_OBJ:
				pGameObj[BlockType][i] = NEW C_GOAL_OBJ();
				break;				
			default:
				break;
			}
			pGameObj[BlockType][i]->InitObject();
		}

	// 衝突用クラス生成
	pCollision = NEW C_COLLISION;

	// ポーズ用オブジェクト初期化
	pPause = NEW C_PAUSE;
	pPause->InitObject();

	// ステージ読み込み初期化
	pStageLoader = NEW C_STAGE_LOADER;
	pStageLoader->LoadStage(pGameObj, GetCurrentStageNum());

	// スコア初期化
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
	{
		pUIScore[i] = NEW C_SCORE_UI(i);
		pUIScore[i]->InitObject();
	}

	// ゴール初期化
	pGoal = NEW C_GOAL;
	pGoal->InitObject();

	// ハート初期化
	pHeart = NEW C_HEART_BOARD;
	pHeart->InitObject();

	// ゲーム状態初期化
	uGameState  = GAME_TUTORIAL;

	nRestartCnt = RESTART_CNT;

	/*
	// BGM再生
	auto Bgm = C_AUDIO::GetBgm(AUDIO::BGM_MAIN);

	
	float vol = 0.3f;
	Bgm->setVolume(vol);

	Bgm->play();
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_MAIN::UninitScene()
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

	// 衝突用後処理
	SAFE_DELETE(pCollision);

	// ポーズ後処理
	pPause->UninitObject();
	SAFE_DELETE(pPause);

	// パーティクル後処理
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
		{
			pParticleObj[TypeCnt][ObjCnt]->UninitObject();
			SAFE_DELETE(pParticleObj[TypeCnt][ObjCnt]);
		}


	// ゲームオブジェクト後処理
	for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
		for (INT j = 0; j < MAX_GAME_OBJ; j++)
		{
			pGameObj[i][j]->UninitObject();
			SAFE_DELETE(pGameObj[i][j]);
		}

	// UI後処理
	for (INT TypeCnt = 0; TypeCnt < MAX_UI_OBJ_TYPE; TypeCnt++)
	{
		pUIObj[TypeCnt]->UninitObject();
		SAFE_DELETE(pUIObj[TypeCnt]);
	}

	// スコア後処理
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
	{
		pUIScore[i]->UninitObject();
		SAFE_DELETE(pUIScore[i]);
	}

	// ステージローダー後処理
	pStageLoader->UninitObject();
	SAFE_DELETE(pStageLoader);

	// ゴール後処理
	pGoal->UninitObject();
	SAFE_DELETE(pGoal);

	// ハート後処理
	pHeart->UninitObject();
	SAFE_DELETE(pHeart);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_MAIN::UpdateScene()
{
	// チュートリアルステージ用処理
	if (uGameState == GAME_TUTORIAL)
	{
		pCamera->SetCameraState(MAIN_TUTORIAL);
		if (GetKeyboardPress(DIK_SPACE))
		{
			pUIObj[OBJ_NEXT]->SetUsedFlg(false);
			pUIObj[OBJ_TUTORIAL]->SetNarrowFlg(true);
		}

		if (!pUIObj[OBJ_TUTORIAL]->GetUsedFlg())
		{
			pUIObj[OBJ_READY]->SetUsedFlg(true);
				pCamera->SetCameraState(MAIN_START_FADE);
				uGameState = GAME_READY;
		}
	}

	// ゴールから移行処理
	if (uGameState == GAME_GOAL)
	{
		bool bAllUse = true;
		for (INT i = 0; i < MAX_PARTICLE; i++)
		{
			if (pParticleObj[OBJ_2D_STAR_PARTICLE][i]->GetUsedFlg())
			{
				bAllUse = false;
				break;
			}
		}
		if (bAllUse)
			GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_MAIN);
	}

	// ハート更新
	pHeart->UpdateObject(pPlayer->GetOffset());
	
	// ポーズ解除処理
	if (GetKeyboardTrigger(DIK_P))
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
			InitStatus();
	}

	// UI更新
	for (INT UiTypeCnt = 0; UiTypeCnt < MAX_UI_OBJ_TYPE; UiTypeCnt++)
		pUIObj[UiTypeCnt]->UpdateObject();

	// スコア更新
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
		pUIScore[i]->UpdateObject();
	
	// UI更新
	if (uGameState == GAME_MISS)
	{
		pUIObj[OBJ_MISS]->SetUsedFlg(true);
		pUIObj[OBJ_BG]->SetUsedFlg(true);
	}

	// プレイヤー更新
	pPlayer->UpdatePlayer_GameMain(pCamera->GetCameraFowerd());

	// ゲーム状態更新
	if (!pUIObj[OBJ_READY]->GetUsedFlg() && uGameState != GAME_MISS && uGameState != GAME_GOAL && uGameState != GAME_TUTORIAL)
		uGameState = GAME_NORMAL;

	// ポーズ更新
	if (uGameState == GAME_PAUSE)
	{
		pPause->UpdateObject();
		return;
	}
	if (uGameState == GAME_MISS)
		return;

	// 死亡判定
	if (pPlayer->GetState() == PLAYER_STATE::TYPE_DEAD)
		uGameState = GAME_MISS;

	// スカイドーム更新
	pSkydome->UpdateObject();

	// パーティクル更新処理
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
			pParticleObj[TypeCnt][ObjCnt]->UpdateObject();

	// ゲームオブジェクト更新
	for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
		for (INT j = 0; j < MAX_GAME_OBJ; j++)
			pGameObj[i][j]->UpdateObject();
	
	// あたり判定チェック
	if (uGameState == GAME_NORMAL || uGameState == GAME_GOAL)
	{
		CheckCollision();
	}

#if _DEBUG
	PrintDebugProc("ゲームステート◆%d\n", uGameState);
#endif

	// スコア加算
	for (INT i = 0; i < MAX_PARTICLE; i++)
	{
		if (!pParticleObj[OBJ_2D_STAR_PARTICLE][i]->GetUsedFlg())
			continue;

		if (pParticleObj[OBJ_2D_STAR_PARTICLE][i]->GetCurveCnt() >= 1.0f)
		{
			pPlayer->AddScore();
			pParticleObj[OBJ_2D_STAR_PARTICLE][i]->SetCnt(0.0f);
			pParticleObj[OBJ_2D_STAR_PARTICLE][i]->SetUsedFlg(false);
		}
	}

	INT nScore = pPlayer->GetScore();
	INT nDigit;

	// スコアセット
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
	{
		nDigit = nScore % 10;
		pUIScore[i]->SetCurrentAnimPattern(nDigit);
		nScore /= 10;
	}

	// ゴール更新
	pGoal->UpdateObject();

	// カメラ更新
	pCamera->UpdateCamera_GameMain(pPlayer,pUIObj[OBJ_READY]);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_MAIN::DrawScene()
{
	// ポーズ処理
	if (uGameState == GAME_PAUSE)
	{
		// ポーズ描画
		pPause->DrawObject();
	}

	// プレイヤー描画
	pPlayer->DrawObject();

	// ゲームオブジェクト描画
	for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
		for (INT j = 0; j < MAX_GAME_OBJ; j++)
			pGameObj[i][j]->DrawObject(pCamera->GetMtxView(),pCamera->GetProjectionMtx());

	// スカイドーム描画
	pSkydome->DrawObject();

	// パーティクル描画
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
			pParticleObj[TypeCnt][ObjCnt]->DrawObject();

	// カメラセット
	pCamera->SetCamera();

	// スコア描画
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
		pUIScore[i]->DrawObject();

	// UI描画
	for (INT ObjCnt = 0; ObjCnt < MAX_UI_OBJ_TYPE; ObjCnt++)
		pUIObj[ObjCnt]->DrawObject();

	// ハート描画
	pHeart->DrawObject();

	// ゴール描画
	pGoal->DrawObject();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_MAIN::InitStatus()
{
	// BGM再生
//	PlaySound(SOUND_BGM_MAIN);

	// プレイヤーステータス初期化
	pPlayer->InitStatus();

	// UI使用フラグ更新
	pUIObj[OBJ_MISS]->SetUsedFlg(false);
	pUIObj[OBJ_BG]->SetUsedFlg(false);

	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticleObj[0][i]->SetUsedFlg(false);

	// 星の配置を再取得
	pStageLoader->LoadStage(pGameObj, GetCurrentStageNum());

	nRestartCnt = RESTART_CNT;

	// スコアリセット
	pPlayer->SetScore(0);
	
	// ゲーム状態初期化
	uGameState = GAME_READY;

	// READYアイコン初期化
	pUIObj[OBJ_READY]->SetUsedFlg(true);
	pUIObj[OBJ_READY]->SetCnt(MAX_READY_CNT);
	pUIObj[OBJ_READY]->SetCurrentAnimPattern(4);

	// パーティクル初期化
	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticleObj[OBJ_2D_STAR_PARTICLE][i]->InitStatus();

	// ステータス初期化
	pUIObj[OBJ_MISS]->InitStatus();

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_CAMERA* C_SCENE_MAIN::GetCamera()
{
	return pCamera;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 判定関連
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_MAIN::CheckCollision()
{
	// 例外処理
	if (pPlayer->GetState() == PLAYER_STATE::TYPE_JUMP_UP)
		return;

	// ブロックとの判定
	uIsHitBlock = NONE;
	D3DXVECTOR3 Cross, Normal;

	INT nHitIndex = 0;
	D3DXVECTOR3 fLength = D3DXVECTOR3(0,0,0);




	// 移動方向に壁があるか
	D3DXVECTOR3 MoveVec = pPlayer->GetMoveVec();
	D3DXVec3Normalize(&MoveVec, &MoveVec);

	// ブロックとの判定
	uIsHitBlock = NONE;
	Cross = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	nHitIndex = 0;
	fLength = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



	if (pPlayer->GetState() != PLAYER_STATE::TYPE_JUMP_DOWN)
	{
		for (INT i = 0; i < MAX_BLOCK_TYPE; i++)
		{
			for (INT j = 0; j < MAX_GAME_OBJ; j++)
			{
				// 例外処理
				if (!pGameObj[i][j]->GetUsedFlg())
					continue;

				// プレイヤーとブロックとの判定
				uIsHitBlock = pCollision->CheckCollisionWall(pPlayer, NULL, pGameObj[i][j], Cross, Normal, fLength, MoveVec);

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
				pPlayer->SetStatus(PLAYER_STATE::TYPE_MOVE_HIT_WALL);
				pPlayer->SetPosition(pPlayer->GetPosition() - pPlayer->GetMoveVec());
				PrintDebugProc("てすとおおおおおおおおおおおおおおおおおおおおおお");
			}
		}

	}




	// 床との判定
	if (pPlayer->GetState() != (PLAYER_STATE::TYPE_MOVE_HIT_WALL))
	{
		for (INT i = 0; i < MAX_BLOCK_TYPE; i++)
		{
			for (INT j = 0; j < MAX_GAME_OBJ; j++)
			{
				// 例外処理
				if (!pGameObj[i][j]->GetUsedFlg())
					continue;

				// プレイヤーとブロックとの判定
				uIsHitBlock = pCollision->CheckCollisionField(pPlayer, NULL, pGameObj[i][j], Cross, Normal, fLength, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
				nHitIndex = j;

				if (uIsHitBlock == RAY_TRUE)
					break;
			}
			if (uIsHitBlock == RAY_TRUE)
				break;
		}


		bool bSphereHit = false;

		FLOAT CheckY = pPlayer->GetPosition().y - Cross.y;
		if (CheckY < 0.0f)
			CheckY *= -1;

		if (uIsHitBlock == RAY_TRUE)
		{
			if (fLength.x < 0.1f)
			{
				PrintDebugProc("ヒット！！！！！！！！！！１");
				PrintDebugProc("hit%d", nHitIndex);
				pPlayer->SetPosition(Cross - D3DXVECTOR3(0.0f, 0.01f, 0.0f));
				pPlayer->SetStatus(PLAYER_STATE::TYPE_MOVE);
			}

			if (fLength.x > 0.0f)
			{
				pPlayer->SetStatus(PLAYER_STATE::TYPE_JUMP_DOWN);
			}
		}

		else if (uIsHitBlock == RAY_FALSE)
		{
			pPlayer->SetStatus(PLAYER_STATE::TYPE_FALL);
			PrintDebugProc("のーひっと！！！！！！！！！！１");

			PrintDebugProc("のーひっと");

		}
		else if (uIsHitBlock == NONE)
		{
			PrintDebugProc("のー");
			pPlayer->SetIsGround(false);
		}
	}
	PrintDebugProc("\nCross%f\n", Cross.x);
	PrintDebugProc("Cross%f\n", Cross.y);
	PrintDebugProc("Cross%f\n", Cross.z);






	// アイテムとの判定
	for (INT i = 0; i < MAX_GAME_OBJ; i++)
	{
		// 例外処理
		if (!pGameObj[STAR_OBJ][i]->GetUsedFlg())
			continue;

		// AABB衝突判定
		if (pCollision->IsHitAABBItem(pPlayer, pGameObj[STAR_OBJ][i]))
		{
			pGameObj[STAR_OBJ][i]->SetUsedFlg(false);
			INT particleCnt = 0;

//			PlaySound(SOUND_SE_STAR);	// 音再生

			/*
			// パーティクル生成
			for (INT ParticleCnt = 0; ParticleCnt < MAX_PARTICLE; ParticleCnt++)
			{
				if (pParticleObj[OBJ_BILLBOARD_STAR_PARTICLE][ParticleCnt]->GetUsedFlg())
					continue;

				particleCnt++;
				pParticleObj[OBJ_BILLBOARD_STAR_PARTICLE][ParticleCnt]->SetUsedFlg(true);
				pParticleObj[OBJ_BILLBOARD_STAR_PARTICLE][ParticleCnt]->SetPosition(pGameObj[STAR_OBJ][i]->GetPosition());

				if (ONE_USE_PARTICLLE < particleCnt)
					break;
			}
			*/

			// 3D→2D座標変換
			CheckUnProject(i);
		}
	}

	// ゴール判定
	for (INT i = 0; i < MAX_GAME_OBJ; i++)
	{
		// 例外処理
		if (!pGameObj[GOAL_OBJ][i]->GetUsedFlg())
			continue;

		// AABB衝突判定
		if (pCollision->IsHitAABBItem(pPlayer, pGameObj[GOAL_OBJ][i]))
		{

			// ゴールフラグ更新
			if (uGameState != GAME_GOAL)
			{
//				PlaySound(SOUND_SE_STAGE_CLEAR);
				pCamera->SetCameraState(MAIN_GOAL_FADE);
				uGameState = GAME_GOAL;
				AddStage();		// ステージ情報加算
		//		GetSceneManager()->SetSceneChange(C_SCENE_MANAGER::SCENE_MAIN);
			}
		}
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 3Dから2Dへの変換計算用
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_MAIN::CheckUnProject(INT Indx)
{
	D3DXVECTOR3 StarObj = pGameObj[STAR_OBJ][Indx]->GetPosition();
	D3DXMATRIX  ViewMtx = pCamera->GetMtxView();
	D3DXMATRIX  ProjectionMtx = pCamera->GetProjectionMtx();

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
		if (pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->GetUsedFlg())
			continue;

		UseParticleCnt ++;

		pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->SetUsedFlg(true);
		pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->SetPosition(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));
		pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->SetStartCurvePos(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));

		if (UseParticleCnt > ONE_USE_PARTICLLE)
			break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スコアセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_MAIN::SetScore()
{

}
