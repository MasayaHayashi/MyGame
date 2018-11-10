//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Camera.cpp
// カメラクラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Camera.h"
#include "d3dx9.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../KeyBoard/Keyboard.h"
#include "../Player/Player.h"
#include "../Board/Board.h"
#include "../Application/Application.h"

/*
#include "C_Block.h"
#include "C_Player.h"
#include "C_Ready.h"
#include "C_Xinput.h"

#if _DEBUG
#include "debugproc.h"
#endif
*/

// ===== 定数・マクロ定義 =====
#define MAX_PITCH_SPEED		(25.0f)	// ピッチの最大値
#define ACCELE_PITCH_SPEED	(0.5f)	// 1フレームに加速するピッチの値
#define DECELE_PITCH_SPEED	(0.25f)	// 1フレームに減速するピッチの値

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Camera::Camera()
{
	rotMovecamera  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	movecameraDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	numMovePitch	= 0.0f;
	rotPitchRadian = 0.0f;
	cameraUpDest	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	currentState	= cameraState::Type1Person;

	cameraFowerd	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	changecamera	= true;
	lerpCnt		= 0.0f;

	cameraMoveFade = MAIN_START_FADE;

	rot = 0;
	rotCnt = 0;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Camera::~Camera()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::finalize(Player *pPlayer)
{

	// 現在のシーン取得
	currentScene = SceneManager::getCurrentSceneType();


	// シーンに応じて分岐
	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		initializeTitle(pPlayer);
		break;
	case SceneManager::SceneState::SceneMain:
		initializeGameMain(pPlayer);
		break;
	case SceneManager::SceneState::SceneResult:
		break;
	case SceneManager::SceneState::SceneStageEdit:
		initializeStageEdit();
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initialize()
{

	cameraPos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// カメラの視点
	cameraLook  = D3DXVECTOR3(0.0f,0.0f,0.0f);	// カメラの注視点
	cameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);									// カメラの上方向
	
	cameraPosDest = D3DXVECTOR3(0.0f, 0.0f, -9000.0f);								// カメラの視点の目的位置
//	cameraLookDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 1.7f, PlayerPos.z);		// カメラの注視点の目的位置

																					//	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
																					//	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

																					//	ForwardVec *= 5.0f;
																					//	cameraPosDest -= ForwardVec;

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = cameraPosDest.x - cameraLook.x;
	fVecZ = cameraPosDest.z - cameraLook.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

	heightcameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
	heightcameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

										// 行列初期化
	D3DXMatrixIdentity(&mtxView);



}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化（タイトル）
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initializeTitle(Player *pPlayer)
{
	// プレイヤー情報取得
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

	cameraPos  = D3DXVECTOR3(0.0f, 5.0f, -19.0f);							// カメラの視点
	cameraLook = D3DXVECTOR3(PlayerPos.x, PlayerPos.y -0.7f, PlayerPos.z);	// カメラの注視点
	cameraUp   = pPlayer->getUpVec();										// カメラの上方向

	cameraPosDest  = D3DXVECTOR3(0.0f, 0.0f, -9.0f);								// カメラの視点の目的位置
	cameraLookDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 1.7f, PlayerPos.z);		// カメラの注視点の目的位置

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = cameraPosDest.x - cameraLook.x;
	fVecZ = cameraPosDest.z - cameraLook.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

	heightcameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
	heightcameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

	// 行列初期化
	D3DXMatrixIdentity(&mtxView);



}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化(ステージエディット)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initializeStageEdit()
{
	// プレイヤー情報取得

	cameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);							// カメラの視点
	cameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの注視点
	cameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								// カメラの上方向

	cameraPosDest = D3DXVECTOR3(0.0f, 0.0f, -9.0f);								// カメラの視点の目的位置
	cameraLookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);		// カメラの注視点の目的位置

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = cameraPosDest.x - cameraLook.x;
	fVecZ = cameraPosDest.z - cameraLook.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

	heightcameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
	heightcameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

	// 行列初期化
	D3DXMatrixIdentity(&mtxView);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化(ゲームメイン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initializeGameMain(Player *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getPosition();

	fadePos[2]  = PlayerPos + D3DXVECTOR3(10.0f, PlayerPos.y  + 4.3f, 5.0f);
	fadePos[1]  = PlayerPos + D3DXVECTOR3(-4.0f, PlayerPos.y  + 4.3f, 5.0f);
	fadePos[0]  = D3DXVECTOR3(0.0f, 10.0f, -19.0f);


	// プレイヤー情報取得
	cameraPos	= D3DXVECTOR3(0.0f, 10.0f, -19.0f);			// カメラの視点
	cameraLook	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの注視点
	cameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// カメラの上方向

	cameraPosDest	= D3DXVECTOR3(0.0f, 0.0f, -9.0f);		// カメラの視点の目的位置
	cameraLookDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// カメラの注視点の目的位置


	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = cameraPosDest.x - cameraLook.x;
	fVecZ = cameraPosDest.z - cameraLook.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

	heightcameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
	heightcameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

										// 行列初期化
	D3DXMatrixIdentity(&mtxView);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::finalize()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::update(Player *pPlayer,Board *pReadyUI)
{
	// 現在のシーン取得
	currentScene = SceneManager::getCurrentSceneType();

	// シーンに応じて分岐
	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		updateTitle(pPlayer);
		break;
	case SceneManager::SceneState::SceneMain:
		updateGameMain(pPlayer, pReadyUI);
		break;
	case SceneManager::SceneState::SceneResult:
		break;
	case SceneManager::SceneState::SceneStageEdit:
		break;
	default:
		break;
	}


}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新(タイトルシーン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateTitle(Pawn *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

	cameraLook = PlayerPos;
	cameraLook.y += 3.0f;

	cameraFowerd = cameraLook - cameraPos;

//	rotationcamera(pPlayer->getOffset());

	/*
#if 1 // 線形補間処理
	// 共通処理
	D3DXVec3Lerp(&cameraPos, &cameraPos, &cameraPosDest, fLerpCnt);
	D3DXVec3Normalize(&cameraUp, &cameraUp);

	// 線形補間用
	fLerpCnt += 0.0001f;
	if (fLerpCnt >= 1.0f)
	{
		bChangecamera = false;
		fLerpCnt = 1.0f;
	}
#endif

	/*
	// 移動量の補正
	fRotMovecamera += fMovecameraDest;
	*/

	float fVecX, fVecZ;
	fVecX = cameraPos.x - cameraLook.x;
	fVecZ = cameraPos.z - cameraLook.z;
	lengthIntervalcamera = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

#if _DEBUG

#if 0	// デバッグ用カメラ移動
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// 左前移動
			cameraPos.x -= cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// 左後移動
			cameraPos.x -= cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else
		{// 左移動
			cameraPos.x -= cosf(cameraRot.y) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y) * VALUE_MOVE_camera;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// 右前移動
			cameraPos.x += cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// 右後移動
			cameraPos.x += cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else
		{// 右移動
			cameraPos.x += cosf(cameraRot.y) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y) * VALUE_MOVE_camera;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// 前移動
		cameraPos.x += sinf(cameraRot.y) * VALUE_MOVE_camera;
		cameraPos.z += cosf(cameraRot.y) * VALUE_MOVE_camera;

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// 後移動
		cameraPos.x -= sinf(cameraRot.y) * VALUE_MOVE_camera;
		cameraPos.z -= cosf(cameraRot.y) * VALUE_MOVE_camera;

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// 視点旋回「左」
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI * 2.0f;
		}

		cameraPos.x = cameraLook.x - sinf(cameraRot.y) * lengthIntervalcamera;
		cameraPos.z = cameraLook.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_C))
	{// 視点旋回「右」
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI * 2.0f;
		}

		cameraPos.x = cameraLook.x - sinf(cameraRot.y) * lengthIntervalcamera;
		cameraPos.z = cameraLook.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// 視点移動「上」
		cameraPos.y += VALUE_MOVE_camera;
	}
	if (getKeyboardPress(DIK_N))
	{// 視点移動「下」
		cameraPos.y -= VALUE_MOVE_camera;
	}

	if (getKeyboardPress(DIK_Q))
	{// 注視点旋回「左」
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI*2.0f;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_E))
	{// 注視点旋回「右」
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI*2.0f;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_T))
	{// 注視移動「上」
		cameraLook.y += VALUE_MOVE_camera;
	}
	if (getKeyboardPress(DIK_B))
	{// 注視移動「下」
		cameraLook.y -= VALUE_MOVE_camera;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	cameraPos  = pPlayer->getOffset() - Look * 10.0f;
	cameraPos.y += 6.0f;
	cameraLook = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// 注視点の補正
	cameraLook.x += (cameraLookDest.x - cameraLook.x) * RATE_CHASE_camera_L;
	cameraLook.y += (cameraLookDest.y - cameraLook.y) * 0.1f;
	cameraLook.z += (cameraLookDest.z - cameraLook.z) * RATE_CHASE_camera_L;

	// 視点の補正
	cameraPos.x += (cameraPosDest.x - cameraPos.x) * RATE_CHASE_camera_P;
	cameraPos.y += (cameraPosDest.y - cameraPos.y) * RATE_CHASE_camera_P;
	cameraPos.z += (cameraPosDest.z - cameraPos.z) * RATE_CHASE_camera_P;
	*/

	/*
	PrintDebugProc("デバッグ表示ON/OFF ： F1     カメラ、モデルリセット ： SPACE\n");
	PrintDebugProc("[カメラの視点  ：(%f : %f : %f)]\n", cameraPos.x, cameraPos.y, cameraPos.z);
	PrintDebugProc("[カメラの注視点：(%f : %f : %f)]\n", cameraLook.x, cameraLook.y, cameraLook.z);
	PrintDebugProc("[カメラの向き  ：(%f)]\n", cameraRot.y);
	PrintDebugProc("\n");

	PrintDebugProc("*** 視点操作 ***\n");
	PrintDebugProc("前移動 : W\n");
	PrintDebugProc("後移動 : S\n");
	PrintDebugProc("左移動 : A\n");
	PrintDebugProc("右移動 : D\n");
	PrintDebugProc("上移動 : Y\n");
	PrintDebugProc("下移動 : N\n");
	PrintDebugProc("左旋回 : Z\n");
	PrintDebugProc("右旋回 : C\n");
	PrintDebugProc("\n");

	PrintDebugProc("*** 注視点操作 ***\n");
	PrintDebugProc("上移動 : T\n");
	PrintDebugProc("下移動 : B\n");
	PrintDebugProc("左旋回 : Q\n");
	PrintDebugProc("右旋回 : E\n");
	PrintDebugProc("\n");
	*/
#endif

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新(ステージエディットシーン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateStageEdit(const D3DXVECTOR3 &Pos)
{
	D3DXVECTOR3 BlockPos = Pos;

	cameraPosDest = D3DXVECTOR3(BlockPos.x, BlockPos.y + 20.0f, BlockPos.z - 20.0f);

	cameraLookDest = BlockPos;

	static float fcnt = 0.0f;
	fcnt += 0.0001f;
	D3DXVec3Lerp(&cameraPos,  &cameraPos,		  &cameraPosDest,  fcnt);
	D3DXVec3Lerp(&cameraLook, &cameraLook,		  &cameraLookDest, fcnt);
	if (fcnt > 0.2f)
		fcnt = 0.0f;
#if _DEBUG

#if 0	// デバッグ用カメラ移動
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// 左前移動
			cameraPos.x -= cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// 左後移動
			cameraPos.x -= cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else
		{// 左移動
			cameraPos.x -= cosf(cameraRot.y) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y) * VALUE_MOVE_camera;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// 右前移動
			cameraPos.x += cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// 右後移動
			cameraPos.x += cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else
		{// 右移動
			cameraPos.x += cosf(cameraRot.y) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y) * VALUE_MOVE_camera;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// 前移動
		cameraPos.x += sinf(cameraRot.y) * VALUE_MOVE_camera;
		cameraPos.z += cosf(cameraRot.y) * VALUE_MOVE_camera;

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// 後移動
		cameraPos.x -= sinf(cameraRot.y) * VALUE_MOVE_camera;
		cameraPos.z -= cosf(cameraRot.y) * VALUE_MOVE_camera;

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// 視点旋回「左」
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI * 2.0f;
		}

		cameraPos.x = cameraLook.x - sinf(cameraRot.y) * lengthIntervalcamera;
		cameraPos.z = cameraLook.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_C))
	{// 視点旋回「右」
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI * 2.0f;
		}

		cameraPos.x = cameraLook.x - sinf(cameraRot.y) * lengthIntervalcamera;
		cameraPos.z = cameraLook.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// 視点移動「上」
		cameraPos.y += VALUE_MOVE_camera;
	}
	if (getKeyboardPress(DIK_N))
	{// 視点移動「下」
		cameraPos.y -= VALUE_MOVE_camera;
	}

	if (getKeyboardPress(DIK_Q))
	{// 注視点旋回「左」
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI*2.0f;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_E))
	{// 注視点旋回「右」
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI*2.0f;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_T))
	{// 注視移動「上」
		cameraLook.y += VALUE_MOVE_camera;
	}
	if (getKeyboardPress(DIK_B))
	{// 注視移動「下」
		cameraLook.y -= VALUE_MOVE_camera;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	cameraPos  = pPlayer->getOffset() - Look * 10.0f;
	cameraPos.y += 6.0f;
	cameraLook = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// 注視点の補正
	cameraLook.x += (cameraLookDest.x - cameraLook.x) * RATE_CHASE_camera_L;
	cameraLook.y += (cameraLookDest.y - cameraLook.y) * 0.1f;
	cameraLook.z += (cameraLookDest.z - cameraLook.z) * RATE_CHASE_camera_L;

	// 視点の補正
	cameraPos.x += (cameraPosDest.x - cameraPos.x) * RATE_CHASE_camera_P;
	cameraPos.y += (cameraPosDest.y - cameraPos.y) * RATE_CHASE_camera_P;
	cameraPos.z += (cameraPosDest.z - cameraPos.z) * RATE_CHASE_camera_P;
	*/
	/*
	PrintDebugProc("デバッグ表示ON/OFF ： F1     カメラ、モデルリセット ： SPACE\n");
	PrintDebugProc("[カメラの視点  ：(%f : %f : %f)]\n", cameraPos.x, cameraPos.y, cameraPos.z);
	PrintDebugProc("[カメラの注視点：(%f : %f : %f)]\n", cameraLook.x, cameraLook.y, cameraLook.z);
	PrintDebugProc("[カメラの向き  ：(%f)]\n", cameraRot.y);
	PrintDebugProc("\n");

	PrintDebugProc("*** 視点操作 ***\n");
	PrintDebugProc("前移動 : W\n");
	PrintDebugProc("後移動 : S\n");
	PrintDebugProc("左移動 : A\n");
	PrintDebugProc("右移動 : D\n");
	PrintDebugProc("上移動 : Y\n");
	PrintDebugProc("下移動 : N\n");
	PrintDebugProc("左旋回 : Z\n");
	PrintDebugProc("右旋回 : C\n");
	PrintDebugProc("\n");

	PrintDebugProc("*** 注視点操作 ***\n");
	PrintDebugProc("上移動 : T\n");
	PrintDebugProc("下移動 : B\n");
	PrintDebugProc("左旋回 : Q\n");
	PrintDebugProc("右旋回 : E\n");
	PrintDebugProc("\n");
	*/
#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新(ゲームメイン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateGameMain(Player *pPlayer,Board *pReadyUI)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);


	switch (cameraMoveFade)
	{
		case MAIN_TUTORIAL:
			{

			cameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);		// カメラの視点
			cameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの注視点


			}
			break;

		case MAIN_START_FADE:
			{
				UINT Indx = pReadyUI->getCurrentAnim();

				cameraPos = fadePos[Indx - 1];
				cameraLook = pPlayer->getOffset();

				if (Indx == 0)
				{
					cameraMoveFade = MAIN_NORMAL;
					cameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);		// カメラの視点
					cameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの注視点
				}
			}
		break;
		case MAIN_NORMAL:
			{
				D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

				cameraPosDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 4.3f, PlayerPos.z - 9.0f);
				cameraLookDest = PlayerPos + D3DXVECTOR3(0.0f, 0.0f, 30.0f);

				static float fcnt = 0.0f;
				fcnt += 0.001f;
				D3DXVec3Lerp(&cameraPos, &cameraPos, &cameraPosDest, fcnt);
				D3DXVec3Lerp(&cameraLook, &cameraLook, &cameraLookDest, fcnt);

				if (fcnt > 1.0f)
					fcnt = 1.0f;

			}

			break;
		case MAIN_GOAL_FADE:
		{

			D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

			D3DXVECTOR3 VecLength = cameraPos - PlayerPos;
			FLOAT Length = sqrtf(PlayerPos.x * PlayerPos.x + PlayerPos.z * PlayerPos.z);

			rotCnt += RotSpeed;
			RotMove(&PlayerPos, 10.0f);

			cameraPos.y = 10.0f;
			cameraLook = PlayerPos;
			cameraLook.y = 0.0f;

		}
		break;
	default:
		break;
	}


	cameraLook = PlayerPos;
	cameraLook.y += 3.0f;

	cameraFowerd = cameraLook - cameraPos;

//	rotationcamera(pPlayer->getOffset());

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::RotMove(D3DXVECTOR3* pVecCenter, FLOAT fRadius)
{
	D3DXMATRIX matRotation;	// 掛け合わせ用回転行列
	rot = rotCnt / 1000.0f;

	//原点に半径を足しただけの座標を用意
	D3DXVECTOR3 vecTarget(-fRadius, 0.0f, -fRadius);

	//原点を中心とした回転（オイラー回転）の行列を作る
	D3DXMatrixRotationY(&matRotation, rot);
	D3DXVec3TransformCoord(&vecTarget, &vecTarget, &matRotation);

	//最後に本来の座標（回転対象の座標）を足し合わせ
	D3DXVec3Add(&vecTarget, &vecTarget, pVecCenter);
	cameraPos.x = vecTarget.x;
	cameraPos.z = vecTarget.z;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラの設定処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::setCamera()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);
	
	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView,
		&cameraPos	,		// カメラの視点
		&cameraLook,		// カメラの注視点
		&cameraUp);			// カメラの上方向

	// ビューマトリックスの設定
	devicePtr->SetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,			// 視野角
		static_cast<FLOAT> (Application::ScreenWidth) / static_cast<FLOAT>(Application::ScreenHeight),		// アスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値
		VIEW_FAR_Z);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	devicePtr->SetTransform(D3DTS_PROJECTION, &mtxProjection);
	
//	D3DXVec3Unproject()
//	devicePtr->getViewport()	// ビューポートマトリックス取得
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ回転
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::rotationCamera(D3DXVECTOR3 Center)
{
	/*
	C_XINPUT *pXinput = C_XINPUT::getInstance();

	short RightStickX = pXinput->getThumbRX();
	short RightStickY = pXinput->getThumbRY();

	RightStickX /= (32767 * 0.5f);
	
	if (getKeyboardPress(DIK_L))
		RightStickX ++;
	if (getKeyboardPress(DIK_J))
		RightStickX --;
	if (getKeyboardPress(DIK_K))
		RightStickY --;
	if (getKeyboardPress(DIK_I))
		RightStickY ++;

	*/
//	D3DXVECTOR3 cameraForwrd = GetCameraFowerd();

	RotMove(&Center, 5.0f);

	// 回転処理
//	rotCnt += camera_ROT_SPEED * RightStickX;


//	D3DXVec3Normalize(&cameraForwrd, &cameraForwrd);


//	cameraPos.y += RightStickY;
	/*
	if (RightStickY > 0.0f)
	{
		cameraPos.y -= 0.1f;
	}
	else if (RightStickY < 0.0f)
	{
		cameraPos.y += 0.1f;
	}
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラの向き取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Camera::getRot()
{
	return cameraRot;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ビューマトリックスの取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXMATRIX Camera::getMtxView()
{
	return mtxView;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プロジェクションマトリックス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXMATRIX Camera::getProjectionMtx()
{
	return mtxProjection;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ビューポートマトリックス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DVIEWPORT9* Camera::getViwPort()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	devicePtr->GetViewport(&viewPort);

	return &viewPort;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ位置取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Camera::getPos()
{
	return cameraPos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ注視点取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Camera::getLook()
{
	return cameraLook;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラアップベクトル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3* Camera::getUp()
{
	return &cameraUp;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラの前ベクトル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Camera::getFowerd()
{
	D3DXVECTOR3 cameraFowerd = cameraLook - cameraPos;
	D3DXVec3Normalize(&cameraFowerd, &cameraFowerd);
	
	return cameraFowerd;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラステートセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::setState(camera_MOVE_TYPE setState)
{
	cameraMoveFade = setState;
}