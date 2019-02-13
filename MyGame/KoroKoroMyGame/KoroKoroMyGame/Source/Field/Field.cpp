//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Field.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Field.h"
#include "../KeyBoard/Keyboard.h"
#include "../Collider/Collider.h"
#include "../DirectX3D/DirectX3D.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Field::Field()
{
	// 位置・向きの初期設定
	myTransform.pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 拡大率設定
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// ファイルパス設定
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	// 行列初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	isFieldObject = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Field::~Field()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::initialize()
{
	// 現在のシーン取得
	currentScene = SceneManager::getCurrentSceneType();

	// シーンごとに分岐
	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		initializeTitle();
		break;
	case SceneManager::SceneState::SceneMain:
		initializeGameMain();
		break;
	case SceneManager::SceneState::SceneResult:
		initializeResult();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		initializeTitle();
		break;

	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::finalize()
{
	ResourceManager::destroyAllMesh();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::update()
{
	// 現在のシーン取得
	currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		updateTitle();
		break;
	case SceneManager::SceneState::SceneMain:
		updateGameMain();
		break;
	case SceneManager::SceneState::SceneResult:
		updateResult();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		break;
	default:

		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::draw()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();


	// 描画
	Pawn::drawObjectLocal();

	// コライダー描画
//	pCollider->DrawCollider();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();


	// 描画
	Pawn::drawObjectLocal(mtxView, mtxProj);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// リザルト用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::updateResult()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンタイトル用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::initializeTitle()
{
	// 位置、移動量、拡大率初期化
	myTransform.pos		= D3DXVECTOR3(-8.5f, -2.0f, 0.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 各種変数初期化
	pD3DTexture	 = nullptr;
	meshPtr	 = nullptr;
	materialBufferPtr = nullptr;
	numMat		 = 0;

	// Xファイルの読み込み
//	ResourceManager *pResourceMgr = GetResourceManager();

//	ResourceManager::MakevertexBoard(vertexBoard, fileName);
//	ResourceManager::CreateTexture(Texture, fileName);

	ResourceManager::makeModel(meshDataObj, fileName,meshType);
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

	myTransform.isUsed = true;

	// コライダー初期化
//	pCollider = NEW C_COLLIDER(pos, MeshData.collitionBox);
//	pCollider->initializeCollider();
//	pCollider->setUsedFlg(false);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンメイン用プレイヤーGameMain
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::initializeGameMain()
{
	myTransform.pos		= D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture	 = nullptr;
	meshPtr	 = nullptr;
	materialBufferPtr = nullptr;
	numMat		 = 0;

	// Xファイルの読み込み
//	ResourceManager *pResourceMgr = GetResourceManager();


	ResourceManager::makeModel(meshDataObj, fileName,meshType);
	ResourceManager::createTexture(textureData, texFileName);
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// リザルト用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::initializeResult()
{
	myTransform.pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 拡大率設定
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture			= nullptr;
	meshPtr				= nullptr;
	materialBufferPtr	= nullptr;
	numMat				= 0;

	ResourceManager::makeModel(meshDataObj, fileName,meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンタイトル用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::updateTitle()
{
	// 回転軸設定
	D3DXVECTOR3 rotAxis = D3DXVECTOR3(1.0f,5.0f,2.0f);
	
	// クォータニオンによる回転
	rotCnt += TitleRotSpeed;
	D3DXQuaternionRotationAxis(&quatanion, &rotAxis, rotCnt);		// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);			// クォータニオンから回転行列掛け合わせ

	// コライダー更新
//	pCollider->UpdateCollider(worldMtx, DEFAULT_BOXcolor);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンメイン用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Field::updateGameMain()
{
	// 回転軸設定
	D3DXVECTOR3 rotAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// クォータニオンによる回転
	rotCnt += TitleRotSpeed;
	D3DXQuaternionRotationAxis(&quatanion, &rotAxis, rotCnt);		// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);			// クォータニオンから回転行列掛け合わせ
}