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
	rotvelocityCamera	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	velocityCameraDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	numvelocityPitch	= 0.0f;
	rotPitchRadian		= 0.0f;
	cameraUpDest		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	cameraFowerd		= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	changeCamera		= true;
	lerpCnt				= 0.0f;

	rotCnt = 0;

	D3DXMatrixIdentity(&myTransform.viewMtx);

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
// デバッグ用移動
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

	FLOAT fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

	const Transform playerTransform = *Collision::getTransform("Player",0);

	fadePos[2] = playerTransform.pos + D3DXVECTOR3( 4.0f, playerTransform.pos.y + 0.3f, -20.0f);
	fadePos[1] = playerTransform.pos + D3DXVECTOR3(-4.0f, playerTransform.pos.y + 0.3f, -20.0f);
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
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化(ステージエディット)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initializeStageEdit()
{
	myTransform.pos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);							// カメラの視点
	myTransform.look = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの注視点
	cameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								// カメラの上方向

	myTransform.posDest = D3DXVECTOR3(0.0f, -0.0f, -9.0f);								// カメラの視点の目的位置
	myTransform.lookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);		// カメラの注視点の目的位置

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	FLOAT fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ初期化(ゲームメイン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initializeMain(Player *pPlayer)
{
	D3DXVECTOR3 playerPos = pPlayer->getPosition();

	fadePos[0]  = playerPos + D3DXVECTOR3(0.0f, 3.0f, -20.0f);
	fadePos[1]  = Collision::getTransform("Player", 0)->pos + D3DXVECTOR3(-5.5f,  -4.3f, 7.0f);
	fadePos[2]  = Collision::getTransform("Player", 0)->pos + D3DXVECTOR3( 2.0f,  -4.3f, 7.0f);

	myTransform.pos		= D3DXVECTOR3(0.0f, 10.0f, -20.0f);		
	myTransform.look	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	cameraUp			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);		

	myTransform.posDest		= D3DXVECTOR3(0.0f, -1.0f, -5.0f);	
	myTransform.lookDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					

	FLOAT fVecX, fVecZ;			// カメラから注視点までの距離
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// カメラの視点と注視点の距離
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
void Camera::update(Player &pPlayer,INT countdown)
{
	currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		updateGameMain(pPlayer, countdown);
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
	myTransform.fowerd = myTransform.look - myTransform.pos;

	myTransform.posDest = PlayerPos - ForwardVec * 10;
	
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
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新(ステージエディットシーン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateStageEdit(std::string keyName,UINT selectIndex)
{
	D3DXVECTOR3 blockPos = Collision::getTransform(keyName, selectIndex)->pos;
	static D3DXVECTOR3 forword = D3DXVECTOR3(0.0f,0.0f,0.0f);

	myTransform.posDest = D3DXVECTOR3(blockPos.x, blockPos.y + 40.0f, blockPos.z - 30.0f) + forword;
	myTransform.look = blockPos;

	static float fcnt = 0.0f;
	fcnt += 0.0001f;
	D3DXVec3Lerp(&myTransform.pos,  &myTransform.pos,	&myTransform.posDest,  fcnt);
	D3DXVec3Lerp(&myTransform.look, &myTransform.look,	&myTransform.lookDest, fcnt);


	D3DXVECTOR3 forwd = myTransform.pos - blockPos;
	D3DXVec3Normalize(&forwd, &forwd);

	if (Keyboard::getPress(DIK_I))
	{
		forword += forwd ;
	}

	if (Keyboard::getPress(DIK_K))
	{
		forword -= forwd ;
	}

//	rotation(blockPos, 10.0f);
//	editInput(blockPos);

	if (fcnt > 0.3f)
	{
		fcnt = 0.0f;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ更新(ゲームメイン)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateGameMain(Player &pPlayer,INT countDown)
{
	GameManager::GameType currentGameType = GameManager::getGameType();

	switch (currentGameType)
	{
		case GameManager::GameType::Ready:
			updateGameMainReady(pPlayer, countDown);
			break;
		case GameManager::GameType::Miss:
			updateGameMainMiss();
			break;
		case GameManager::GameType::Playing:
			updateGameMainPlay(pPlayer,countDown);
			break;
		case GameManager::GameType::FallMiss:
			updateFallMiss();
		default:
			break;
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 準備
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateGameMainReady(Player &pPlayer, INT countDown)
{
//	myTransform.look = pPlayer.getPosition();
	D3DXVECTOR3 playerToCamera = myTransform.pos - pPlayer.getPosition();

//	myTransform.pos = myTransform.pos + playerToCamera;

	myMoveType		= MoveStateType::StartFade;

	if (countDown <= 0)
	{
		countDown = 0;
	}

	DirectX3D::printDebug("XXX%f\n", myTransform.pos.x);
	DirectX3D::printDebug("YYY%f\n", myTransform.pos.y);
	DirectX3D::printDebug("ZZZ%f\n", myTransform.pos.z);

	myTransform.pos = fadePos[countDown - 1];
	myTransform.look = Collision::getTransform("Player", 0)->pos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 失敗
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateGameMainMiss()
{
	myTransform.pos = Collision::getTransform("Player", 0)->pos - myTransform.fowerd * 20;
	myTransform.pos.y += 15.0f;
	myTransform.look = Collision::getTransform("Player", 0)->pos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイ中更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateGameMainPlay(Player &pPlayer,Board countDown)
{
	D3DXVECTOR3 PlayerPos		= pPlayer.getOffset();
	D3DXVECTOR3 ForwardVec		= pPlayer.getForwardVec();
	D3DXVECTOR3 UpVec			= pPlayer.getUpVec();

	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);
/*
	if (GameManager::isGameType(GameManager::GameType::Ready))
	{
		myTransform.pos = pPlayer.getPosition();
		myTransform.pos.y += 2.0f;
		myTransform.pos.z -= 10.0f;
		myTransform.look = pPlayer.getPosition();
	}*/



	myTransform.posDest = Collision::getTransform("Player", 0)->pos - *D3DXVec3Normalize(&cameraFowerd, &cameraFowerd) * 6.0f;
	myTransform.posDest.y += 4.0f;
	myTransform.lookDest = Collision::getTransform("Player", 0)->pos + *D3DXVec3Normalize(&cameraFowerd, &cameraFowerd) * 10.0f;

	lerpCnt += 0.02f;

	if (lerpCnt > 1.0f)
	{
		lerpCnt = 1.0f;
	}

	D3DXVec3Lerp(&myTransform.pos, &myTransform.pos, &myTransform.posDest, lerpCnt);
	D3DXVec3Lerp(&myTransform.look, &myTransform.look, &myTransform.lookDest, lerpCnt);

	DirectX3D::printDebug("プレイヤー位置%f\n", Collision::getTransform("Player", 0)->pos.z);

	DirectX3D::printDebug("カメラ位置Z:%f\n", myTransform.pos.z);
	DirectX3D::printDebug("カメラ注視点Z:%f\n", myTransform.lookDest.z);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 回転
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::rotation(D3DXVECTOR3* pVecCenter, FLOAT fRadius)
{
	D3DXMATRIX matRotation;	// 掛け合わせ用回転行列
	INT rot = rotCnt / 1000;

	//原点に半径を足しただけの座標を用意
	D3DXVECTOR3 vecTarget(-fRadius, 0.0f, -fRadius);

	//原点を中心とした回転（オイラー回転）の行列を作る
	D3DXMatrixRotationY(&matRotation, static_cast<FLOAT>(rot));
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
	D3DXMatrixIdentity(&myTransform.viewMtx);
	
	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&myTransform.viewMtx,
		&myTransform.pos,			// カメラの視点
		&myTransform.look,			// カメラの注視点
		&myTransform.up);			// カメラの上方向

	// ビューマトリックスの設定
	devicePtr->SetTransform(D3DTS_VIEW, &myTransform.viewMtx);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&myTransform.projectionMtx);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&myTransform.projectionMtx,
		ViewAngle,			// 視野角
		static_cast<FLOAT> (Application::ScreenWidth) / static_cast<FLOAT>(Application::ScreenHeight),		// アスペクト比
		ViewNearZ,		// ビュー平面のNearZ値
		ViewFarZ);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	devicePtr->SetTransform(D3DTS_PROJECTION, &myTransform.projectionMtx);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ回転
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::rotationCamera(D3DXVECTOR3 Center)
{

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
	return myTransform.viewMtx;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プロジェクションマトリックス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXMATRIX Camera::getProjectionMtx()
{
	return myTransform.projectionMtx;
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

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 回転
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::rotation(D3DXVECTOR3 center, FLOAT radius)
{
	D3DXMATRIX matRotation;	// 掛け合わせ用回転行列
	FLOAT rot = rotCnt / 1000.0f;

	//原点に半径を足しただけの座標を用意
	D3DXVECTOR3 vecTarget(-radius, 0.0f, -radius);

	//原点を中心とした回転（オイラー回転）の行列を作る
	D3DXMatrixRotationY(&matRotation, rot);
	D3DXVec3TransformCoord(&vecTarget, &vecTarget, &matRotation);

	//最後に本来の座標（回転対象の座標）を足し合わせ
	D3DXVec3Add(&vecTarget, &vecTarget, &center);
	myTransform.pos.x = vecTarget.x;
	myTransform.pos.z = vecTarget.z;

	rotCnt++;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::initializeStatus()
{
	lerpCnt = 0;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カメラ用入力
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::editInput(D3DXVECTOR3 blockPos)
{
	D3DXVECTOR3 forwd = myTransform.pos - blockPos;
	D3DXVec3Normalize(&forwd, &forwd);

	if (Keyboard::getPress(DIK_I))
	{
		myTransform.look += forwd;
	}

	if (Keyboard::getPress(DIK_K))
	{
		myTransform.look -= forwd;
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 落下
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Camera::updateFallMiss()
{
	myTransform.look = Collision::getTransform("Player", 0)->pos;

}