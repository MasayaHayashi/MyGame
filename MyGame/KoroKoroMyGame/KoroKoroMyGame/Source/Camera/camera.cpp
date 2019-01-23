//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Camera.cpp
// カメラクラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Camera.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../KeyBoard/Keyboard.h"
#include "../Player/Player.h"
#include "../Board/Board.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"
#include "../GameManager/GameManager.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Camera::Camera()
{
	rotvelocityCamera  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	velocityCameraDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	numvelocityPitch	= 0.0f;
	rotPitchRadian = 0.0f;
	cameraUpDest	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	cameraFowerd	= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	changeCamera	= true;
	lerpCnt		= 0.0f;


	rot = 0;
	rotCnt = 0;

	Collision::registerList(&myTransform, "Camera");
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
		
		break;
	case SceneManager::SceneState::SceneMain:
		
		break;
	case SceneManager::SceneState::SceneResult:
		break;
	case SceneManager::SceneState::SceneStageEdit:
		
		break;
	default:
		break;
	}
}

#if _DEBUG

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::debugMove()
{
	if (Keyboard::getPress(DIK_W))
	{
		myTransform.pos.y += 0.05f;
	}
	if (Keyboard::getPress(DIK_A))
	{
		myTransform.pos.x -= 0.05f;
	}
	if (Keyboard::getPress(DIK_S))
	{
		myTransform.pos.y -= 0.05f;
	}
	if (Keyboard::getPress(DIK_D))
	{
		myTransform.pos.x += 0.05f;
	}

	DirectX3D::printDebug("myTransform.pos.x%f\n", myTransform.pos.x);
	DirectX3D::printDebug("myTransform.pos.y%f\n", myTransform.pos.y);
	DirectX3D::printDebug("myTransform.pos.z%f\n", myTransform.pos.z);
}

#endif

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initialize()
{

	myTransform.pos	= D3DXVECTOR3(0.0f, 0.0f, -5.0f);							// カメラの視点
	myTransform.look  = D3DXVECTOR3(0.0f,0.0f,0.0f);	// カメラの注視点
	cameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);									// カメラの上方向
	
	myTransform.posDest = D3DXVECTOR3(0.0f, 0.0f, -9000.0f);

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

//	heightcameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
//	heightcameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

	// 行列初期化
	D3DXMatrixIdentity(&mtxView);


	const Transform playerTransform = *Collision::getTransform("Player").front();

	fadePos[2] = playerTransform.pos + D3DXVECTOR3(10.0f, playerTransform.pos.y + 4.3f, 5.0f);
	fadePos[1] = playerTransform.pos + D3DXVECTOR3(-4.0f, playerTransform.pos.y + 4.3f, 5.0f);
	fadePos[0] = D3DXVECTOR3(0.0f, 10.0f, -19.0f);


}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化（タイトル）
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initializeTitle()
{
	myTransform.pos  = D3DXVECTOR3(0.0f, 3.0f, -19.0f);
	myTransform.look = D3DXVECTOR3(0.0f,0.0f,0.0f);
	myTransform.up   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	myTransform.posDest  = D3DXVECTOR3(0.0f, 0.0f, -9.0f);
	myTransform.lookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	FLOAT fVecX, fVecZ;
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);

	D3DXMatrixIdentity(&mtxView);



}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化(ステージエディット)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initializeStageEdit()
{
	// プレイヤー情報取得

	myTransform.pos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);							// カメラの視点
	myTransform.look = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの注視点
	cameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								// カメラの上方向

	myTransform.posDest = D3DXVECTOR3(0.0f, -0.0f, -9.0f);								// カメラの視点の目的位置
	myTransform.lookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);		// カメラの注視点の目的位置

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

//	heightcameraP = CHASE_HEIGHT_P;	// 追跡時の視点の高さ
//	heightcameraL = CHASE_HEIGHT_L;	// 追跡時の注視点の高さ

	// 行列初期化
	D3DXMatrixIdentity(&mtxView);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化(ゲームメイン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initializeMain(Player *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getPosition();

	fadePos[0]  = PlayerPos + D3DXVECTOR3(0.0f, 3.0f, -20.0f);
	fadePos[1]  = Collision::getTransform("Player", 0)->pos + D3DXVECTOR3(-0.5f, 4.0f, -12.0f);
	fadePos[2]  = Collision::getTransform("Player", 0)->pos + D3DXVECTOR3(0.0f,4.0f,-7.0f);


	myTransform.pos		= D3DXVECTOR3(0.0f, 30.0f, -20.0f);			// カメラの視点
	myTransform.look	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラの注視点
	cameraUp			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// カメラの上方向

	myTransform.posDest		= D3DXVECTOR3(0.0f, -1.0f, -5.0f);		// カメラの視点の目的位置
	myTransform.lookDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// カメラの注視点の目的位置


	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// カメラの回転量

	FLOAT fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

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
void Camera::update(Player *pPlayer,Board &countdown)
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
		updateGameMain(pPlayer,countdown);
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
	D3DXVECTOR3 PlayerPos  = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec	   = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

	myTransform.look = PlayerPos;
	myTransform.look.y += 10.2f;
	myTransform.fowerd = myTransform.look - myTransform.pos;
	
#if 1 // 線形補間処理
	// 共通処理
	D3DXVec3Lerp(&myTransform.pos, &myTransform.pos, &myTransform.posDest, lerpCnt);
	D3DXVec3Normalize(&cameraUp, &cameraUp);

	// 線形補間用
	lerpCnt += 0.0001f;
	if (lerpCnt >= 1.0f)
	{
		changeCamera = false;
		lerpCnt = 1.0f;
	}
#endif

	
	// 移動量の補正
	rotvelocityCamera += velocityCameraDest;

	FLOAT vecX, vecZ;
	vecX = myTransform.pos.x - myTransform.look.x;
	vecZ = myTransform.pos.z - myTransform.look.z;
	lengthIntervalcamera = sqrtf(vecX * vecX + vecZ * vecZ);

#if _DEBUG

#if 0	// デバッグ用カメラ移動
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// 左前移動
			myTransform.pos.x -= cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// 左後移動
			myTransform.pos.x -= cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else
		{// 左移動
			myTransform.pos.x -= cosf(cameraRot.y) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y) * VALUE_velocity_camera;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// 右前移動
			myTransform.pos.x += cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// 右後移動
			myTransform.pos.x += cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else
		{// 右移動
			myTransform.pos.x += cosf(cameraRot.y) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y) * VALUE_velocity_camera;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// 前移動
		myTransform.pos.x += sinf(cameraRot.y) * VALUE_velocity_camera;
		myTransform.pos.z += cosf(cameraRot.y) * VALUE_velocity_camera;

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// 後移動
		myTransform.pos.x -= sinf(cameraRot.y) * VALUE_velocity_camera;
		myTransform.pos.z -= cosf(cameraRot.y) * VALUE_velocity_camera;

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// 視点旋回「左」
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI * 2.0f;
		}

		myTransform.pos.x = myTransform.look.x - sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.pos.z = myTransform.look.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_C))
	{// 視点旋回「右」
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI * 2.0f;
		}

		myTransform.pos.x = myTransform.look.x - sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.pos.z = myTransform.look.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// 視点移動「上」
		myTransform.pos.y += VALUE_velocity_camera;
	}
	if (getKeyboardPress(DIK_N))
	{// 視点移動「下」
		myTransform.pos.y -= VALUE_velocity_camera;
	}

	if (getKeyboardPress(DIK_Q))
	{// 注視点旋回「左」
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI*2.0f;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_E))
	{// 注視点旋回「右」
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI*2.0f;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_T))
	{// 注視移動「上」
		myTransform.look.y += VALUE_velocity_camera;
	}
	if (getKeyboardPress(DIK_B))
	{// 注視移動「下」
		myTransform.look.y -= VALUE_velocity_camera;
	}
#endif
#endif

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新(ステージエディットシーン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateStageEdit(std::string keyName,UINT selectIndex)
{
	D3DXVECTOR3 BlockPos = Collision::getTransform(keyName, selectIndex)->pos;

	myTransform.posDest = D3DXVECTOR3(BlockPos.x, BlockPos.y + 40.0f, BlockPos.z - 30.0f);

	myTransform.lookDest = BlockPos;

	static float fcnt = 0.0f;
	fcnt += 0.0001f;
	D3DXVec3Lerp(&myTransform.pos,  &myTransform.pos,		  &myTransform.posDest,  fcnt);
	D3DXVec3Lerp(&myTransform.look, &myTransform.look,		  &myTransform.lookDest, fcnt);

	if (fcnt > 0.2f)
	{
		fcnt = 0.0f;
	}

#if _DEBUG

#if 0	// デバッグ用カメラ移動
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// 左前移動
			myTransform.pos.x -= cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// 左後移動
			myTransform.pos.x -= cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else
		{// 左移動
			myTransform.pos.x -= cosf(cameraRot.y) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y) * VALUE_velocity_camera;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// 右前移動
			myTransform.pos.x += cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// 右後移動
			myTransform.pos.x += cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else
		{// 右移動
			myTransform.pos.x += cosf(cameraRot.y) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y) * VALUE_velocity_camera;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// 前移動
		myTransform.pos.x += sinf(cameraRot.y) * VALUE_velocity_camera;
		myTransform.pos.z += cosf(cameraRot.y) * VALUE_velocity_camera;

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// 後移動
		myTransform.pos.x -= sinf(cameraRot.y) * VALUE_velocity_camera;
		myTransform.pos.z -= cosf(cameraRot.y) * VALUE_velocity_camera;

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// 視点旋回「左」
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI * 2.0f;
		}

		myTransform.pos.x = myTransform.look.x - sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.pos.z = myTransform.look.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_C))
	{// 視点旋回「右」
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI * 2.0f;
		}

		myTransform.pos.x = myTransform.look.x - sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.pos.z = myTransform.look.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// 視点移動「上」
		myTransform.pos.y += VALUE_velocity_camera;
	}
	if (getKeyboardPress(DIK_N))
	{// 視点移動「下」
		myTransform.pos.y -= VALUE_velocity_camera;
	}

	if (getKeyboardPress(DIK_Q))
	{// 注視点旋回「左」
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI*2.0f;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_E))
	{// 注視点旋回「右」
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI*2.0f;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_T))
	{// 注視移動「上」
		myTransform.look.y += VALUE_velocity_camera;
	}
	if (getKeyboardPress(DIK_B))
	{// 注視移動「下」
		myTransform.look.y -= VALUE_velocity_camera;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	myTransform.pos  = pPlayer->getOffset() - Look * 10.0f;
	myTransform.pos.y += 6.0f;
	myTransform.look = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// 注視点の補正
	myTransform.look.x += (myTransform.lookDest.x - myTransform.look.x) * RATE_CHASE_camera_L;
	myTransform.look.y += (myTransform.lookDest.y - myTransform.look.y) * 0.1f;
	myTransform.look.z += (myTransform.lookDest.z - myTransform.look.z) * RATE_CHASE_camera_L;

	// 視点の補正
	myTransform.pos.x += (myTransform.posDest.x - myTransform.pos.x) * RATE_CHASE_camera_P;
	myTransform.pos.y += (myTransform.posDest.y - myTransform.pos.y) * RATE_CHASE_camera_P;
	myTransform.pos.z += (myTransform.posDest.z - myTransform.pos.z) * RATE_CHASE_camera_P;
	*/
	/*
	PrintDebugProc("デバッグ表示ON/OFF ： F1     カメラ、モデルリセット ： SPACE\n");
	PrintDebugProc("[カメラの視点  ：(%f : %f : %f)]\n", myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	PrintDebugProc("[カメラの注視点：(%f : %f : %f)]\n", myTransform.look.x, myTransform.look.y, myTransform.look.z);
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
void Camera::updateGameMain(Player *pPlayer,Board &countDown)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();

	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

	if (GameManager::isGameType(GameManager::GameType::Ready))
	{
		myTransform.pos = pPlayer->getPosition();
		myTransform.pos.y += 7.0f;
		myTransform.pos.z -= 10.0f;
		myTransform.look = pPlayer->getPosition();
	}
	currentFadeType = countDown.getCurrentAnim() - 1;

	if (currentFadeType <= 0)
	{
		currentFadeType = 0;
	}

	if (GameManager::isGameType(GameManager::GameType::Ready))
	{
		myTransform.pos = fadePos[currentFadeType];
		myTransform.look = Collision::getTransform("Player", 0)->pos;
	}

	if (GameManager::isGameType(GameManager::GameType::Playing))
	{
		myTransform.posDest = Collision::getTransform("Player", 0)->pos - *D3DXVec3Normalize(&cameraFowerd, &cameraFowerd) * 6.0f;
		myTransform.posDest.y += 4.0f;
		myTransform.lookDest = Collision::getTransform("Player", 0)->pos + *D3DXVec3Normalize(&cameraFowerd, &cameraFowerd) * 10.0f;
		//myTransform.pos -= cameraFowerd;

		static float fcnt = 0.0f;
		fcnt += 0.02f;

		if (fcnt > 1.0f)
		{
			fcnt = 1.0f;
		}

		D3DXVec3Lerp(&myTransform.pos, &myTransform.pos, &myTransform.posDest, fcnt);
		D3DXVec3Lerp(&myTransform.look, &myTransform.look, &myTransform.lookDest, fcnt);

		DirectX3D::printDebug("プレイヤー位置%f\n", Collision::getTransform("Player", 0)->pos.z);

		DirectX3D::printDebug("カメラ位置Z:%f\n", myTransform.pos.z);
		DirectX3D::printDebug("カメラ注視点Z:%f\n", myTransform.lookDest.z);

	}

	/*
		else
	{


		cameraPosDest = D3DXVECTOR3(0.0f,4.3f,  9.0f);
		cameraLookDest =  D3DXVECTOR3(0.0f, 0.0f, 30.0f);

		static float fcnt = 0.0f;
		fcnt += 0.001f;
		D3DXVec3Lerp(&cameraPos, &cameraPos, &cameraPosDest, fcnt);
		D3DXVec3Lerp(&cameraLook, &cameraLook, &cameraLookDest, fcnt);
	}
	*/
	myMoveType = MoveStateType::StartFade;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::Rotvelocity(D3DXVECTOR3* pVecCenter, FLOAT fRadius)
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
	myTransform.pos.x = vecTarget.x;
	myTransform.pos.z = vecTarget.z;
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
		&myTransform.pos,			// カメラの視点
		&myTransform.look,			// カメラの注視点
		&myTransform.up);			// カメラの上方向

	// ビューマトリックスの設定
	devicePtr->SetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		ViewAngle,			// 視野角
		static_cast<FLOAT> (Application::ScreenWidth) / static_cast<FLOAT>(Application::ScreenHeight),		// アスペクト比
		ViewNearZ,		// ビュー平面のNearZ値
		ViewFarZ);		// ビュー平面のFarZ値

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

	Rotvelocity(&Center, 5.0f);

	// 回転処理
//	rotCnt += camera_ROT_SPEED * RightStickX;


//	D3DXVec3Normalize(&cameraForwrd, &cameraForwrd);


//	myTransform.pos.y += RightStickY;
	/*
	if (RightStickY > 0.0f)
	{
		myTransform.pos.y -= 0.1f;
	}
	else if (RightStickY < 0.0f)
	{
		myTransform.pos.y += 0.1f;
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
	return myTransform.pos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ注視点取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Camera::getLook()
{
	return myTransform.look;
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
	D3DXVECTOR3 cameraFowerd = myTransform.look - myTransform.pos;
	D3DXVec3Normalize(&cameraFowerd, &cameraFowerd);
	
	return cameraFowerd;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラステートセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::setState(MoveStateType setState)
{
	myMoveType = setState;
}