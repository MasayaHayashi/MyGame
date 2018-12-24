//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// BallObj.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "BallObj.h"
#include "../KeyBoard/Keyboard.h"
#include "../Collider/Collider.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Collision/Collision.h"

/*
#include "C_MyVector3.h"
#include "collision.h"

#if _DEBUG
#include "debugproc.h"
#endif
*/
// ===== 定数・マクロ定義 =====


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BallObj::BallObj()
{
	// 各種クラス初期化
	pCollider = nullptr;

	// 位置・向きの初期設定
	myTransform.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 拡大率設定
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	isShader = true;

	// ファイルパス設定
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	// 行列初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BallObj::BallObj(UINT setIndex)
{
	// 各種クラス初期化
	pCollider = nullptr;

	// 位置・向きの初期設定
	myTransform.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 拡大率設定
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	isShader = true;

	// ファイルパス設定
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	// 行列初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	idNumber = setIndex;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BallObj::~BallObj()
{

	// 各種クラス解放
	//	SAFE_DELETE(pCollider);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::initialize()
{
	// 現在のシーン取得
	currentScene = SceneManager::getCurrentSceneType();

	// シーンごとに分岐
	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		initializeTitleObj_Title();
		break;
	case SceneManager::SceneState::SceneMain:
		initializeTitleObj_GameMain();
		break;
	case SceneManager::SceneState::SceneResult:
		initializeTitleObj_Result();
		break;
	case SceneManager::SceneState::SceneSelect:
		initializeTitleObj_Title();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		initializeTitleObj_Title();
		break;

	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::finalize()
{
	ResourceManager::destroyAllMesh();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::update(D3DXVECTOR3 pos, D3DXVECTOR3 rotVec)
{
	// 現在のシーン取得
	currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		UpdateTitleObj_Title();
		break;
	case SceneManager::SceneState::SceneMain:
		updateGameMain(pos,rotVec);
		break;
	case SceneManager::SceneState::SceneResult:
		UpdateTitleObj_Result();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		UpdateTitleObj_Title();
		break;
	default:

		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::draw()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 描画
	Pawn::drawObjectLocal();

	// コライダー描画
	//	pCollider->DrawCollider();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// リザルト用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::UpdateTitleObj_Result()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンタイトル用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::initializeTitleObj_Title()
{
	// 位置、移動量、拡大率初期化
	myTransform.pos = D3DXVECTOR3(-0.0, 2.0f, 0.0f);
	myTransform.velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 各種変数初期化
	pD3DTexture = nullptr;
	meshPtr = nullptr;
	materialBufferPtr = nullptr;
	numMat = 0;

	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	// 回転
	D3DXMATRIX mRotX, mRotY, mRotZ;

	D3DXMatrixRotationY(&mRotX, D3DXToRadian(0));
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mRotX);

	// 拡大
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, myTransform.scale.x, myTransform.scale.y, myTransform.scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// 移動
	D3DXMATRIX mtxTranslate;
	D3DXMatrixTranslation(&mtxTranslate, myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxTranslate);

	isUsed = true;

	// コライダー初期化
	//	pCollider = NEW C_COLLIDER(pos, MeshData.collitionBox);
	//	pCollider->initializeCollider();
	//	pCollider->setUsedFlg(false);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンメイン用プレイヤーGameMain
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::initializeTitleObj_GameMain()
{
	myTransform.pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity	 = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	myTransform.rotDeg		 = D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
	myTransform.rotDegDest	 = D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
	myTransform.scale		 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	if (idNumber == 1)
	{
		myTransform.pos.x = 5.0f;
		myTransform.pos.y = 0.0f;
		myTransform.pos.z = 0.0f;
	}

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture			= nullptr;
	meshPtr				= nullptr;
	materialBufferPtr	= nullptr;
	numMat = 0;

	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	myTransform.pos.y = meshDataObj.collitionBox.y * 2;

	tagName = "Ball";
	Collision::registerList(&myTransform, tagName);
	
//	move(idNumber);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// リザルト用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::initializeTitleObj_Result()
{
	myTransform.pos		   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg	   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 拡大率設定
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture = nullptr;
	meshPtr = nullptr;
	materialBufferPtr = nullptr;
	numMat = 0;

	// Xファイルの読み込み

	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンタイトル用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::UpdateTitleObj_Title()
{
	// 回転軸設定
	D3DXVECTOR3 RotAxis = D3DXVECTOR3(1.0f, 5.0f, 2.0f);

	// クォータニオンによる回転
	rotCnt += TitleRotSpeed;
	D3DXQuaternionRotationAxis(&quatanion, &RotAxis, rotCnt);		// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);	// クォータニオンから回転行列掛け合わせ

															// コライダー更新
															//	pCollider->UpdateCollider(worldMtx, DEFAULT_BOXcolor);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンメイン用更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::updateGameMain(D3DXVECTOR3 pos,D3DXVECTOR3 rotVec)
{
	setWorldMtxPos(myTransform.pos);

	myTransform.pos += myTransform.velocity;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 移動
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void BallObj::move(const D3DXVECTOR3 moveVector)
{
	D3DXVECTOR3 playerPos = Collision::getTransform("Player", idNumber)->pos;

	myTransform.pos		= playerPos;
	myTransform.pos.y	= playerPos.y - 0.95f;

	setWorldMtxPos(myTransform.pos);
}