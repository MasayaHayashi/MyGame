//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Camera.cpp
// カメラクラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "camera.h"
#include "../SceneManager/SceneManager.h"
#include "input.h"
#include "C_Block.h"
#include "C_Player.h"
#include "C_Ready.h"
#include "C_Xinput.h"

#if _DEBUG
#include "debugproc.h"
#endif

// ===== 定数・マクロ定義 =====
#define MAX_PITCH_SPEED		(25.0f)	// ピッチの最大値
#define ACCELE_PITCH_SPEED	(0.5f)	// 1フレームに加速するピッチの値
#define DECELE_PITCH_SPEED	(0.25f)	// 1フレームに減速するピッチの値
#define ROT_SPEED (10.0f)

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Camera::Camera()
{
	rotMoveCamera  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	moveCameraDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	numMovePitch	= 0.0f;
	rotPitchRadian = 0.0f;
	cameraUpDest	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	currentState	= CameraState::Type1Person;

	cameraFowerd	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	changeCamera	= true;
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
void Camera::finalize(C_PLAYER *pPlayer)
{

	// 現在のシーン取得
	currentScene = getSceneManager()->getCurrentScene();


	// シーンに応じて分岐
	switch (CurrentScene)
	{
	case C_SCENE_MANAGER::SCENE_TITLE:
		InitCamera_Title(pPlayer);
		break;
	case C_SCENE_MANAGER::SCENE_MAIN:
		InitCamera_GameMain(pPlayer);
		break;
	case C_SCENE_MANAGER::SCENE_RESULT:
		break;
	case C_SCENE_MANAGER::SCENE_STAGE_EDIT:
		InitCamera_StageEdit();
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::InitCamera()
{

	CameraPos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// カメラの視点
	CameraLook  = D3DXVECTOR3(0.0f,0.0f,0.0f);	// カメラの注視点
	CameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);									// カメラの上方向

	CameraPosDest = D3DXVECTOR3(0.0f, 0.0f, -9000.0f);								// カメラの視点の目的位置
//	CameraLookDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 1.7f, PlayerPos.z);		// カメラの注視点の目的位置

																					//	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
																					//	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

																					//	ForwardVec *= 5.0f;
																					//	CameraPosDest -= ForwardVec;

	CameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = CameraPosDest.x - CameraLook.x;
	fVecZ = CameraPosDest.z - CameraLook.z;

	fLengthIntervalCamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

	fHeightCameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
	fHeightCameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

										// 行列初期化
	D3DXMatrixIdentity(&mtxView);



}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化（タイトル）
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::InitCamera_Title(C_PLAYER *pPlayer)
{
	// プレイヤー情報取得
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

	CameraPos  = D3DXVECTOR3(0.0f, 5.0f, -19.0f);							// カメラの視点
	CameraLook = D3DXVECTOR3(PlayerPos.x, PlayerPos.y -0.7f, PlayerPos.z);	// カメラの注視点
	CameraUp   = pPlayer->getUpVec();										// カメラの上方向

	CameraPosDest  = D3DXVECTOR3(0.0f, 0.0f, -9.0f);								// カメラの視点の目的位置
	CameraLookDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 1.7f, PlayerPos.z);		// カメラの注視点の目的位置

//	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
//	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

//	ForwardVec *= 5.0f;
//	CameraPosDest -= ForwardVec;

	CameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = CameraPosDest.x - CameraLook.x;
	fVecZ = CameraPosDest.z - CameraLook.z;

	fLengthIntervalCamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

	fHeightCameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
	fHeightCameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

	// 行列初期化
	D3DXMatrixIdentity(&mtxView);





	//	
	//	if (getKeyboardPress(DIK_1))
	//	{
	//		bChangeCamera = true;
	//		CurrentState = TYPE_1PERSON;
	//	}
	//	if (getKeyboardPress(DIK_2))
	//	{
	//		bChangeCamera = true;
	//
	//		CurrentState = TYPE_3PERSON;
	//	}
	//
	//	switch (CurrentState)
	//	{
	//	case TYPE_3PERSON:
	//		D3DXVec3TransformCoord(&CameraPos, &D3DXVECTOR3(0.0f, 5.5f, -50.0f), pPlayer->getWorldMtx());
	//		CameraLook = D3DXVECTOR3(PlayerPos.x, PlayerPos.y, PlayerPos.z);			// カメラの注視点	y 5000 z5000
	//		CameraUp = UpVec;				// カメラの上方向
	//		fLerpCnt = 0.0f;
	//		break;
	//	case TYPE_1PERSON:
	//		D3DXVec3TransformCoord(&CameraPosDest, &D3DXVECTOR3(0.0f, 2.5f, 12.0f), pPlayer->getWorldMtx());
	//		CameraLook = D3DXVECTOR3(PlayerPos.x, PlayerPos.y, PlayerPos.z) + ForwardVec * 50.0f;			// カメラの注視点	y 5000 z5000
	//		CameraUp = UpVec;				// カメラの上方向
	//		break;
	//	default:
	//		break;
	//
	//	}
	//	// 共通処理
	//	D3DXVec3Lerp(&CameraPos, &CameraPos, &CameraPosDest, fLerpCnt);
	//	D3DXVec3Normalize(&CameraUp, &CameraUp);
	//
	//	// 線形補間用
	//	fLerpCnt += 0.01f;
	//	if (fLerpCnt >= 1.0f)
	//	{
	//		bChangeCamera = false;
	//		fLerpCnt = 1.0f;
	//	}
	//#if _DEBUG
	//	PrintDebugProc("X:%f", PlayerPos.x);
	//	PrintDebugProc("Y:%f", PlayerPos.y);
	//	PrintDebugProc("Z:%f", PlayerPos.z);
	//#endif
	//
	//	// カメラ振動
	//	if (nCntShakeFrame > 0)
	//	{
	//		nCntShakeFrame--;
	//
	//		// サインカーブ角度を更新
	//		fRadShake += 2.0f * D3DX_PI / (float)DEFAULT_SHAKE_COUNT;
	//
	//		if (fRadShake >= D3DX_PI)
	//			fRadShake -= 2.0f * D3DX_PI;
	//
	//		// 終了時に角度を0に戻す
	//		if (0 == nCntShakeFrame)
	//			fRadShake = 0.0f;
	//
	//	}

	/*
	if (getKeyboardPress(DIK_LEFT))
	{// ズームイン
	if (fLengthIntervalCamera > 100.0f)
	{
	fLengthIntervalCamera -= 1.0f;
	fHeightCameraP -= 0.35f;
	}
	}
	if (getKeyboardPress(DIK_RIGHT))
	{// ズームアウト
	if (fLengthIntervalCamera < 300.0f)
	{
	fLengthIntervalCamera += 1.0f;
	fHeightCameraP += 0.35f;
	}
	}
	*/

	/*
	if (getKeyboardPress(DIK_Z))
	{// 視点旋回「左」
	rotCamera.y += VALUE_ROTATE_CAMERA;
	if (rotCamera.y > D3DX_PI)
	{
	rotCamera.y -= D3DX_PI * 2.0f;
	}

	posCameraP.x = posCameraL.x - sinf(rotCamera.y) * fLengthIntervalCamera;
	posCameraP.z = posCameraL.z - cosf(rotCamera.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_C))
	{// 視点旋回「右」
	rotCamera.y -= VALUE_ROTATE_CAMERA;
	if (rotCamera.y < -D3DX_PI)
	{
	rotCamera.y += D3DX_PI * 2.0f;
	}

	posCameraP.x = posCameraL.x - sinf(rotCamera.y) * fLengthIntervalCamera;
	posCameraP.z = posCameraL.z - cosf(rotCamera.y) * fLengthIntervalCamera;
	}
	*/

	/*
	// 注視点の補正
	posCameraL.x += (posCameraLDest.x - posCameraL.x) * fRateChaseCameraL;
	posCameraL.y += (posCameraLDest.y - posCameraL.y) * 0.1f;
	posCameraL.z += (posCameraLDest.z - posCameraL.z) * fRateChaseCameraL;

	// 視点の補正
	posCameraP.x += (posCameraPDest.x - posCameraP.x) * fRateChaseCameraP;
	posCameraP.y += (posCameraPDest.y - posCameraP.y) * fRateChaseCameraP;
	posCameraP.z += (posCameraPDest.z - posCameraP.z) * fRateChaseCameraP;

	// 移動量の補正
	fRotMoveCamera += fMoveCameraDest;
	*/


}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化(ステージエディット)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::InitCamera_StageEdit()
{
	// プレイヤー情報取得

	CameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);							// カメラの視点
	CameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの注視点
	CameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								// カメラの上方向

	CameraPosDest = D3DXVECTOR3(0.0f, 0.0f, -9.0f);								// カメラの視点の目的位置
	CameraLookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);		// カメラの注視点の目的位置

																					//	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
																					//	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

																					//	ForwardVec *= 5.0f;
																					//	CameraPosDest -= ForwardVec;

	CameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = CameraPosDest.x - CameraLook.x;
	fVecZ = CameraPosDest.z - CameraLook.z;

	fLengthIntervalCamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

	fHeightCameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
	fHeightCameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

	// 行列初期化
	D3DXMatrixIdentity(&mtxView);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化(ゲームメイン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::InitCamera_GameMain(C_PLAYER *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getPosition();

	FadePos[2]  = PlayerPos + D3DXVECTOR3(10.0f, PlayerPos.y  + 4.3f, 5.0f);
	FadePos[1]  = PlayerPos + D3DXVECTOR3(-4.0f, PlayerPos.y  + 4.3f, 5.0f);
	FadePos[0]  = D3DXVECTOR3(0.0f, 10.0f, -19.0f);


	// プレイヤー情報取得
	CameraPos	= D3DXVECTOR3(0.0f, 10.0f, -19.0f);			// カメラの視点
	CameraLook	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの注視点
	CameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// カメラの上方向

	CameraPosDest	= D3DXVECTOR3(0.0f, 0.0f, -9.0f);		// カメラの視点の目的位置
	CameraLookDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// カメラの注視点の目的位置


	CameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = CameraPosDest.x - CameraLook.x;
	fVecZ = CameraPosDest.z - CameraLook.z;

	fLengthIntervalCamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

	fHeightCameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
	fHeightCameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

										// 行列初期化
	D3DXMatrixIdentity(&mtxView);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::finalizeCamera()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateCamera(C_PLAYER *pPlayer,C_BOARD *pReadyUI)
{
	// 現在のシーン取得
	CurrentScene = getSceneManager()->getCurrentScene();

	// シーンに応じて分岐
	switch (CurrentScene)
	{
	case C_SCENE_MANAGER::SCENE_TITLE:
		updateCamera_Title(pPlayer);
		break;
	case C_SCENE_MANAGER::SCENE_MAIN:
		updateCamera_GameMain(pPlayer, pReadyUI);
		break;
	case C_SCENE_MANAGER::SCENE_RESULT:
		break;
	case C_SCENE_MANAGER::SCENE_STAGE_EDIT:
		break;
	default:
		break;
	}


}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新(タイトルシーン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateCamera_Title(Pawn *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

	CameraLook = PlayerPos;
	CameraLook.y += 3.0f;

	CameraFowerd = CameraLook - CameraPos;

	RotationCamera(pPlayer->getOffset());

	/*
#if 1 // 線形補間処理
	// 共通処理
	D3DXVec3Lerp(&CameraPos, &CameraPos, &CameraPosDest, fLerpCnt);
	D3DXVec3Normalize(&CameraUp, &CameraUp);

	// 線形補間用
	fLerpCnt += 0.0001f;
	if (fLerpCnt >= 1.0f)
	{
		bChangeCamera = false;
		fLerpCnt = 1.0f;
	}
#endif

	/*
	// 移動量の補正
	fRotMoveCamera += fMoveCameraDest;
	*/

	float fVecX, fVecZ;
	fVecX = CameraPos.x - CameraLook.x;
	fVecZ = CameraPos.z - CameraLook.z;
	fLengthIntervalCamera = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

#if _DEBUG

#if 1	// デバッグ用カメラ移動
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// 左前移動
			CameraPos.x -= cosf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else if (getKeyboardPress(DIK_S))
		{// 左後移動
			CameraPos.x -= cosf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else
		{// 左移動
			CameraPos.x -= cosf(CameraRot.y) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// 右前移動
			CameraPos.x += cosf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else if (getKeyboardPress(DIK_S))
		{// 右後移動
			CameraPos.x += cosf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else
		{// 右移動
			CameraPos.x += cosf(CameraRot.y) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// 前移動
		CameraPos.x += sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		CameraPos.z += cosf(CameraRot.y) * VALUE_MOVE_CAMERA;

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// 後移動
		CameraPos.x -= sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		CameraPos.z -= cosf(CameraRot.y) * VALUE_MOVE_CAMERA;

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// 視点旋回「左」
		CameraRot.y += VALUE_ROTATE_CAMERA;
		if (CameraRot.y > D3DX_PI)
		{
			CameraRot.y -= D3DX_PI * 2.0f;
		}

		CameraPos.x = CameraLook.x - sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraPos.z = CameraLook.z - cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_C))
	{// 視点旋回「右」
		CameraRot.y -= VALUE_ROTATE_CAMERA;
		if (CameraRot.y < -D3DX_PI)
		{
			CameraRot.y += D3DX_PI * 2.0f;
		}

		CameraPos.x = CameraLook.x - sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraPos.z = CameraLook.z - cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// 視点移動「上」
		CameraPos.y += VALUE_MOVE_CAMERA;
	}
	if (getKeyboardPress(DIK_N))
	{// 視点移動「下」
		CameraPos.y -= VALUE_MOVE_CAMERA;
	}

	if (getKeyboardPress(DIK_Q))
	{// 注視点旋回「左」
		CameraRot.y -= VALUE_ROTATE_CAMERA;
		if (CameraRot.y < -D3DX_PI)
		{
			CameraRot.y += D3DX_PI*2.0f;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_E))
	{// 注視点旋回「右」
		CameraRot.y += VALUE_ROTATE_CAMERA;
		if (CameraRot.y > D3DX_PI)
		{
			CameraRot.y -= D3DX_PI*2.0f;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_T))
	{// 注視移動「上」
		CameraLook.y += VALUE_MOVE_CAMERA;
	}
	if (getKeyboardPress(DIK_B))
	{// 注視移動「下」
		CameraLook.y -= VALUE_MOVE_CAMERA;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	CameraPos  = pPlayer->getOffset() - Look * 10.0f;
	CameraPos.y += 6.0f;
	CameraLook = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// 注視点の補正
	CameraLook.x += (CameraLookDest.x - CameraLook.x) * RATE_CHASE_CAMERA_L;
	CameraLook.y += (CameraLookDest.y - CameraLook.y) * 0.1f;
	CameraLook.z += (CameraLookDest.z - CameraLook.z) * RATE_CHASE_CAMERA_L;

	// 視点の補正
	CameraPos.x += (CameraPosDest.x - CameraPos.x) * RATE_CHASE_CAMERA_P;
	CameraPos.y += (CameraPosDest.y - CameraPos.y) * RATE_CHASE_CAMERA_P;
	CameraPos.z += (CameraPosDest.z - CameraPos.z) * RATE_CHASE_CAMERA_P;
	*/

	PrintDebugProc("デバッグ表示ON/OFF ： F1     カメラ、モデルリセット ： SPACE\n");
	PrintDebugProc("[カメラの視点  ：(%f : %f : %f)]\n", CameraPos.x, CameraPos.y, CameraPos.z);
	PrintDebugProc("[カメラの注視点：(%f : %f : %f)]\n", CameraLook.x, CameraLook.y, CameraLook.z);
	PrintDebugProc("[カメラの向き  ：(%f)]\n", CameraRot.y);
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
#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新(ステージエディットシーン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateCamera_StageEdit(const D3DXVECTOR3 &Pos)
{
	D3DXVECTOR3 BlockPos = Pos;

	CameraPosDest = D3DXVECTOR3(BlockPos.x, BlockPos.y + 20.0f, BlockPos.z - 20.0f);

	CameraLookDest = BlockPos;

	static float fcnt = 0.0f;
	fcnt += 0.0001f;
	D3DXVec3Lerp(&CameraPos,  &CameraPos,		  &CameraPosDest,  fcnt);
	D3DXVec3Lerp(&CameraLook, &CameraLook,		  &CameraLookDest, fcnt);
	if (fcnt > 0.2f)
		fcnt = 0.0f;
#if _DEBUG

#if 0	// デバッグ用カメラ移動
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// 左前移動
			CameraPos.x -= cosf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else if (getKeyboardPress(DIK_S))
		{// 左後移動
			CameraPos.x -= cosf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else
		{// 左移動
			CameraPos.x -= cosf(CameraRot.y) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// 右前移動
			CameraPos.x += cosf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else if (getKeyboardPress(DIK_S))
		{// 右後移動
			CameraPos.x += cosf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else
		{// 右移動
			CameraPos.x += cosf(CameraRot.y) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// 前移動
		CameraPos.x += sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		CameraPos.z += cosf(CameraRot.y) * VALUE_MOVE_CAMERA;

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// 後移動
		CameraPos.x -= sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		CameraPos.z -= cosf(CameraRot.y) * VALUE_MOVE_CAMERA;

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// 視点旋回「左」
		CameraRot.y += VALUE_ROTATE_CAMERA;
		if (CameraRot.y > D3DX_PI)
		{
			CameraRot.y -= D3DX_PI * 2.0f;
		}

		CameraPos.x = CameraLook.x - sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraPos.z = CameraLook.z - cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_C))
	{// 視点旋回「右」
		CameraRot.y -= VALUE_ROTATE_CAMERA;
		if (CameraRot.y < -D3DX_PI)
		{
			CameraRot.y += D3DX_PI * 2.0f;
		}

		CameraPos.x = CameraLook.x - sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraPos.z = CameraLook.z - cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// 視点移動「上」
		CameraPos.y += VALUE_MOVE_CAMERA;
	}
	if (getKeyboardPress(DIK_N))
	{// 視点移動「下」
		CameraPos.y -= VALUE_MOVE_CAMERA;
	}

	if (getKeyboardPress(DIK_Q))
	{// 注視点旋回「左」
		CameraRot.y -= VALUE_ROTATE_CAMERA;
		if (CameraRot.y < -D3DX_PI)
		{
			CameraRot.y += D3DX_PI*2.0f;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_E))
	{// 注視点旋回「右」
		CameraRot.y += VALUE_ROTATE_CAMERA;
		if (CameraRot.y > D3DX_PI)
		{
			CameraRot.y -= D3DX_PI*2.0f;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_T))
	{// 注視移動「上」
		CameraLook.y += VALUE_MOVE_CAMERA;
	}
	if (getKeyboardPress(DIK_B))
	{// 注視移動「下」
		CameraLook.y -= VALUE_MOVE_CAMERA;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	CameraPos  = pPlayer->getOffset() - Look * 10.0f;
	CameraPos.y += 6.0f;
	CameraLook = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// 注視点の補正
	CameraLook.x += (CameraLookDest.x - CameraLook.x) * RATE_CHASE_CAMERA_L;
	CameraLook.y += (CameraLookDest.y - CameraLook.y) * 0.1f;
	CameraLook.z += (CameraLookDest.z - CameraLook.z) * RATE_CHASE_CAMERA_L;

	// 視点の補正
	CameraPos.x += (CameraPosDest.x - CameraPos.x) * RATE_CHASE_CAMERA_P;
	CameraPos.y += (CameraPosDest.y - CameraPos.y) * RATE_CHASE_CAMERA_P;
	CameraPos.z += (CameraPosDest.z - CameraPos.z) * RATE_CHASE_CAMERA_P;
	*/

	PrintDebugProc("デバッグ表示ON/OFF ： F1     カメラ、モデルリセット ： SPACE\n");
	PrintDebugProc("[カメラの視点  ：(%f : %f : %f)]\n", CameraPos.x, CameraPos.y, CameraPos.z);
	PrintDebugProc("[カメラの注視点：(%f : %f : %f)]\n", CameraLook.x, CameraLook.y, CameraLook.z);
	PrintDebugProc("[カメラの向き  ：(%f)]\n", CameraRot.y);
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
#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新(ゲームメイン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateCamera_GameMain(C_PLAYER *pPlayer,C_BOARD *pReadyUI)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);


	switch (CameraMoveFade)
	{
		case MAIN_TUTORIAL:
			{

			CameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);		// カメラの視点
			CameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの注視点


			}
			break;

		case MAIN_START_FADE:
			{
				UINT Indx = pReadyUI->getCurrentAnim();

				CameraPos = FadePos[Indx - 1];
				CameraLook = pPlayer->getOffset();

				if (Indx == 0)
				{
					CameraMoveFade = MAIN_NORMAL;
					CameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);		// カメラの視点
					CameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの注視点
				}
			}
		break;
		case MAIN_NORMAL:
			{
				D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

				CameraPosDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 4.3f, PlayerPos.z - 9.0f);
				CameraLookDest = PlayerPos + D3DXVECTOR3(0.0f, 0.0f, 30.0f);

				static float fcnt = 0.0f;
				fcnt += 0.001f;
				D3DXVec3Lerp(&CameraPos, &CameraPos, &CameraPosDest, fcnt);
				D3DXVec3Lerp(&CameraLook, &CameraLook, &CameraLookDest, fcnt);

				if (fcnt > 1.0f)
					fcnt = 1.0f;

			}

			break;
		case MAIN_GOAL_FADE:
		{

			D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

			D3DXVECTOR3 VecLength = CameraPos - PlayerPos;
			FLOAT Length = sqrtf(PlayerPos.x * PlayerPos.x + PlayerPos.z * PlayerPos.z);

			nRotCnt += ROT_SPEED;
			RotMove(&PlayerPos, 10.0f);

			CameraPos.y = 10.0f;
			CameraLook = PlayerPos;
			CameraLook.y = 0.0f;

		}
		break;
	default:
		break;
	}


	CameraLook = PlayerPos;
	CameraLook.y += 3.0f;

	CameraFowerd = CameraLook - CameraPos;

	RotationCamera(pPlayer->getOffset());

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::RotMove(D3DXVECTOR3* pVecCenter, FLOAT fRadius)
{
	D3DXMATRIX matRotation;	// 掛け合わせ用回転行列
	fRot = nRotCnt / 1000.0f;

	//原点に半径を足しただけの座標を用意
	D3DXVECTOR3 vecTarget(-fRadius, 0.0f, -fRadius);

	//原点を中心とした回転（オイラー回転）の行列を作る
	D3DXMatrixRotationY(&matRotation, fRot);
	D3DXVec3TransformCoord(&vecTarget, &vecTarget, &matRotation);

	//最後に本来の座標（回転対象の座標）を足し合わせ
	D3DXVec3Add(&vecTarget, &vecTarget, pVecCenter);
	CameraPos.x = vecTarget.x;
	CameraPos.z = vecTarget.z;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラの設定処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::setCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView,
		&CameraPos	,		// カメラの視点
		&CameraLook,		// カメラの注視点
		&CameraUp);			// カメラの上方向

	// ビューマトリックスの設定
	pDevice->setTransform(D3DTS_VIEW, &mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,			// 視野角
		VIEW_ASPECT,		// アスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値
		VIEW_FAR_Z);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->setTransform(D3DTS_PROJECTION, &mtxProjection);
	
//	D3DXVec3Unproject()
//	pDevice->getViewport()	// ビューポートマトリックス取得
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ回転
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::RotationCamera(D3DXVECTOR3 Center)
{
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


	D3DXVECTOR3 CameraForwrd = getCameraFowerd();

	RotMove(&Center, 5.0f);

	// 回転処理
	nRotCnt += CAMERA_ROT_SPEED * RightStickX;


	D3DXVec3Normalize(&CameraForwrd, &CameraForwrd);


	CameraPos.y += RightStickY;
	if(RightStickY > 0.0f)
		CameraPos.y -= 0.1f;
	else if(RightStickY < 0.0f)
		CameraPos.y += 0.1f;

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラの向き取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Camera::getCameraRot()
{
	return CameraRot;
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
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	pDevice->getViewport(&ViePort);

	return &ViePort;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ位置取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Camera::getCameraPos()
{
	return CameraPos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ注視点取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Camera::getCameraLook()
{
	return CameraLook;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラアップベクトル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3* Camera::getCameraUp()
{
	return &CameraUp;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラの前ベクトル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Camera::getCameraFowerd()
{
	D3DXVECTOR3 CameraFowerd = CameraLook - CameraPos;
	D3DXVec3Normalize(&CameraFowerd, &CameraFowerd);
	
	return CameraFowerd;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラステートセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::setCameraState(CAMERA_MOVE_TYPE setState)
{
	CameraMoveFade = setState;
}