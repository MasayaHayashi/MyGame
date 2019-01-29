//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_Player.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Player.h"
#include "../KeyBoard/Keyboard.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../MyVector3/MyVector3.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../Ball/BallObj.h"
#include "../Random/MyRandom.h"
#include "../GameManager/GameManager.h"
#include "../Audio/MyAudiere.h"
#include "../Audio/audiere.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Player::Player()
{
	setDefaultValue();

	// ファイルパス設定
	strcpy_s(fileName, ResourceManager::ModelPenchanPass);

	// 識別用タグ設定
	tagName		= "Player";

	// 状態初期化
	playerStateType = PlayerState::Stop;

	// 行列初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	score = 0;

	currentModelType = idNumber;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Player::Player(D3DXVECTOR3 startPosition,UINT setNumber)
{
	// 位置・向きの初期設定
	myTransform.pos = (startPosition);
	myTransform.velocity = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	setDefaultValue();

	// ファイルパス設定
	strcpy_s(fileName, ResourceManager::ModelPenchanPass);

	// 識別用タグ設定
	tagName = "Player";

	// 状態初期化
	playerStateType = PlayerState::Stop;

	// 行列初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	score = 0;

	idNumber = setNumber;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Player::~Player()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::initialize()
{
	SceneManager::SceneState currentScene = SceneManager::getCurrentSceneType();

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
	case SceneManager::SceneState::SceneSelect:
		initializeTitle();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		initializeSceneEdit();
		break;

	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::finalize()
{
	ResourceManager::destroyAllResouce(idNumber);
	Collision::release("Player");
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::update(D3DXVECTOR3 CameraForward)
{
	SceneManager::SceneState currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		updateTitle(CameraForward);
		break;
	case  SceneManager::SceneState::SceneMain:
		updateGameMain(CameraForward);
		break;
	case  SceneManager::SceneState::SceneResult:
		updateResult();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		updateStageEdit();
		break;
	case SceneManager::SceneState::SceneSelect:
		updateSelect();
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::draw()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	
	// 描画
	Pawn::drawFrame(hierarchyMeshData.pFrameRoot);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// リザルト用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::updateResult()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンタイトル用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::initializeTitle()
{
	// 位置、移動量、拡大率初期化
	myTransform.pos = (D3DXVECTOR3(-10.0f, -0.01f, -5.0f));

	setDefaultValue();

	// デバイス取得
	IDirect3DDevice9 *devicePtr = DirectX3D::getDevice();

	// 各種変数初期化
	pD3DTexture			 = nullptr;
	meshPtr				 = nullptr;
	materialBufferPtr	 = nullptr;
	numMat				 = 0;


	// 回転
	D3DXMATRIX mRotX, mRotY, mRotZ;

	D3DXMatrixRotationY(&mRotY, myTransform.rotDeg.y);
	D3DXMatrixMultiply(&worldMtx, &worldMtx,&mRotY);

	// 拡大
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, myTransform.scale.x, myTransform.scale.y, myTransform.scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// 移動
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	moveVector = myTransform.pos;

	moveVector.z -= 100.0f;
	moveVector.x -= 100.0f;
	moveVector.y += 0.0f;
	D3DXVec3Normalize(&moveVector, &moveVector);

	// コライダー初期化
	//pCollider = NEW Collider(pos, hierarchyMeshData.collitionBox);
	//pCollider->initializeCollider();

	// 表示フラグ初期化
	//pCollider->setUsedFlg(false);
	
	ResourceManager::makeModelHierarchyResouce(hierarchyMeshData, ResourceManager::ModelPenchanPass, ResourceManager::ModelPenchanPass, meshType, idNumber);
	ResourceManager::makeModelHierarchyResouce(hierarchyMeshData, ResourceManager::ModelChick, ResourceManager::ModelChick, meshType, idNumber);
	ResourceManager::makeModelHierarchyResouce(hierarchyMeshData, ResourceManager::ModelPenNoHahaPass, ResourceManager::ModelPenNoHahaPass, meshType, idNumber);

	meshType = MeshObjType::HierarchyModel;

	ResourceManager::setHierarchy(&hierarchyMeshData, SelectManager::getModelPass(idNumber), idNumber);

	myTransform.collisionBox = hierarchyMeshData.CollitionBox;
	Collision::registerList(&myTransform, "Player");

	myTransform.pos.y -= hierarchyMeshData.CollitionBox.y * 2;

	//// 現在のアニメーションセットの設定値を取得
	//D3DXTRACK_DESC TD;   // トラックの能力
	//hierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	//// 今のアニメーションをトラック1に移行し
	//// トラックの設定値も移行
	//hierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
	//hierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	//// 新しいアニメーションセットをトラック0に設定
	//hierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[2]);

	isUsed = true;

	D3DXQuaternionRotationAxis(&StartQuaternion, &getUpVec(), 0);
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンメイン用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::initializeGameMain()
{
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 180.0f, 0.0f);
	myTransform.rotDegDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.scale		= D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);
	playerStateType			= PlayerState::velocity;

	switch (idNumber)
	{
	case 0:
		myTransform.pos = D3DXVECTOR3(0.0f, 2.0f, 5.0f);
		break;
	case 1:
		myTransform.pos = D3DXVECTOR3(-10.0f, 5.0f, -5.0f);
		break;
	case 2:
		myTransform.pos = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
		break;
	case 3:
		myTransform.pos = D3DXVECTOR3(5.0f, 5.0f, -5.0f);
		break;
	default:
		break;
	}

	oldPos = myTransform.pos;

	setDefaultValue();

	pD3DTexture		  = nullptr;
	meshPtr			  = nullptr;
	materialBufferPtr = nullptr;
	numMat			  = 0;

	// Xファイルの読み込み
	ResourceManager::makeModelHierarchyResouce(hierarchyMeshData, ResourceManager::ModelPenchanPass,	ResourceManager::ModelPenchanPass,		meshType, idNumber);
	ResourceManager::makeModelHierarchyResouce(hierarchyMeshData, ResourceManager::ModelChick,			ResourceManager::ModelChick,			meshType, idNumber);
	ResourceManager::makeModelHierarchyResouce(hierarchyMeshData, ResourceManager::ModelPenNoHahaPass,  ResourceManager::ModelPenNoHahaPass,	meshType, idNumber);

	meshType = MeshObjType::HierarchyModel;

	ResourceManager::setHierarchy(&hierarchyMeshData, SelectManager::getModelPass(idNumber), idNumber);
	myTransform.collisionBox = hierarchyMeshData.CollitionBox;

	Collision::registerList(&myTransform, "Player");
	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(myTransform.rotDeg.y));

	// 拡大
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, myTransform.scale.x, myTransform.scale.y, myTransform.scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// 移動
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;
	hierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);
	hierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	playerStateType = PlayerState::Stop;
	isGround		= true;
	isUsed			= true;

}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンエディット用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::initializeSceneEdit()
{
	// 位置、移動量、拡大率初期化
	myTransform.pos			= D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.scale		= D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 各種変数初期化
	pD3DTexture			= nullptr;
	meshPtr				= nullptr;
	materialBufferPtr	= nullptr;
	numMat				= 0;

	ResourceManager::makeModelHierarchyResouce(hierarchyMeshData, ResourceManager::ModelPenchanPass,	ResourceManager::ModelPenchanPass,		meshType, idNumber);
	ResourceManager::makeModelHierarchyResouce(hierarchyMeshData, ResourceManager::ModelChick,			ResourceManager::ModelChick,			meshType, idNumber);
	ResourceManager::makeModelHierarchyResouce(hierarchyMeshData, ResourceManager::ModelPenNoHahaPass,	ResourceManager::ModelPenNoHahaPass,	meshType, idNumber);

	meshType = MeshObjType::HierarchyModel;
	 
	ResourceManager::setHierarchy(&hierarchyMeshData, SelectManager::getModelPass(idNumber), idNumber);

	Collision::registerList(&myTransform, "Player");

	// モデル回転
	myTransform.pos.y -= hierarchyMeshData.CollitionBox.y * 2;
	
	// 回転
	D3DXMATRIX mRotX, mRotY, mRotZ;



	// 拡大
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, myTransform.scale.x, myTransform.scale.y, myTransform.scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	D3DXMatrixRotationY(&mRotX, D3DXToRadian(90.0f));
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mRotX);

	// 移動
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	// コライダー初期化
//	pCollider = NEW Collider(pos, hierarchyMeshData.collitionBox);
//	pCollider->initializeCollider();

	// 表示フラグ初期化
//	pCollider->setUsedFlg(false);

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	hierarchyMeshData.pAnimCtrl->GetTrackDesc(1, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
//	hierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
//	hierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	hierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[0]);


//	setAnimChange(0, 1);
//	setTime(00);


}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// リザルト用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::initializeResult()
{
	myTransform.pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 拡大率設定
	myTransform.scale = D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture		= nullptr;
	meshPtr			= nullptr;
	materialBufferPtr	= nullptr;
	numMat			= 0;

	// Xファイルの読み込み
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::initializeStatus()
{
	Pawn::initializeStatus();
	
	playerStateType = PlayerState::Stop;
	isGround = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンタイトル用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::updateTitle(D3DXVECTOR3 CameraForward)
{
	if (Keyboard::getTrigger(DIK_2))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}

/*
	if (Keyboard::getRelease(DIK_2))
	{
		int a = 0;
	}
	*/

//	C_XINPUT *pXinput = C_XINPUT::GetInstance();

//	short Xnum = pXinput->GetThumbLX();
//	short Ynum = pXinput->GetThumbLY();

	D3DXVECTOR3 CameraRight = D3DXVECTOR3(CameraForward.z,0.0f,-CameraForward.x);

//	PrintDebugProc("xxxxxxxxxxxxxxxxxxxxx:%d\n", Xnum);
//	PrintDebugProc("YYYYYYYYYYYYYYYYYYYYY:%d\n", Ynum);

//	D3DXVec3Normalize(&TestVec, &TestVec);

//	TestVec.x = CameraRight.x * CameraForward.x +Xnum;
//	TestVec.z = CameraForward.z * CameraRight.z + Ynum;

//	testVec = CameraRight * Xnum + CameraForward * Ynum;
	
	/*
	D3DXVECTOR3 UpVec = getUpVec();
	D3DXVec3Normalize(&UpVec,&UpVec);

	D3DXQuaternionRotationAxis(&quatanion, &UpVec, 0);			// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);	// クォータニオンから回転行列掛け合わせ
	D3DXVec3Normalize(&testVec, &testVec);

	testVec.y = 0.0f;
	pos += testVec * 0.3f;
	
	D3DXVECTOR3 FowrdVec = getForwardVec();
	D3DXVECTOR3	RightVec = getRightVec();
	D3DXVECTOR3 Upvec	 = getUpVec();	
	
	radRot = MyVector3::CalcAngleDegree(testVec, -FowrdVec);
	D3DXQUATERNION quatanion;
	*/
	/*
	if (radRot == 0.0f)
	{
		D3DXQuaternionRotationAxis(&quatanion, &Upvec, oldRadRot);
		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

		StartQuaternion = quatanion;
		fCnt = 0.0f;

		PrintDebugProc("ああああああああああああ");
	}
	else
	{
		D3DXQuaternionRotationAxis(&destQua, &Upvec, radRot);


		D3DXQuaternionSlerp(&quatanion, &StartQuaternion, &destQua, 1.0f);
		fCnt += 0.1f;

		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		oldRadRot = radRot;

	}

	
	if (fCnt >= 1.0f)
		fCnt = 1.0f;
	
	PrintDebugProc("xxx%f\n", testVec.x);
	PrintDebugProc("yyy%f\n", testVec.y);
	PrintDebugProc("zzz%f\n", testVec.z);
	PrintDebugProc("ddddddddddd%f",fCnt);
	*/

	setWorldMtxPos(myTransform.pos);

	updateAnimation();



	// コライダー更新
//	pCollider->UpdateCollider(worldMtx,DEFAULT_BOXcolor);

#if _DEBUG
//	PrintDebugProc("ぷれいや～～～X%f\n", pos.x);
//	PrintDebugProc("ぷれいや～～～Y%f\n", pos.y);
//	PrintDebugProc("ぷれいや～～～Z%f\n", pos.z);
#endif

	/*
	// マテリアル色設定
	if (bHit)
		pCollider->figurePtr->SetMatcolor(HIT_BOX_color);
	else
		pCollider->figurePtr->SetMatcolor(DEFAULT_BOXcolor);
	*/

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンメイン用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::updateGameMain(D3DXVECTOR3 CameraForward)
{
	switch (GameManager::getGameType())
	{
	case GameManager::GameType::Ready:
		updatePlayng(CameraForward);

		break;
	case GameManager::GameType::Playing:
		updatePlayng(CameraForward);
		break;
	case GameManager::GameType::Goal:
		updateGoal();
		break;
	case GameManager::GameType::Pause:
		break;
	default:
		break;
	}


}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンセレクト用更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::updateSelect()
{
	changeModel();

	if (idNumber == 0)
	{
		if (Keyboard::getRelease(DIK_H))
		{
			SelectManager::subSelect();
		}

		if (Keyboard::getTrigger(DIK_G))
		{
			SelectManager::addSelect();
		}
	}

	DirectX3D::printDebug("%d", (INT)SelectManager::getSelect());


	/*
	if (idNumber == 1)
	{
		if (Keyboard::getTrigger(DIK_RIGHTARROW))
		{
			ResourceManager::setHierarchy(&hierarchyMeshData, ModelPenchanPass, idNumber);
		}
		if (Keyboard::getTrigger(DIK_LEFTARROW))
		{
			ResourceManager::setHierarchy(&hierarchyMeshData, ModelPenNoHahaPass, idNumber);
		}
	}*/

	if (Keyboard::getTrigger(DIK_2))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}

	updateAnimation();

	/*
	static bool testFlg = true;
	static bool testFlg2 = true;
	if (idNumber == 2)
	{
		if(testFlg)
			ResourceManager::changeHierarchy(hierarchyMeshData, ModelChick,idNumber); 

		testFlg = false;
	}
	if (idNumber == 2)
	{
		if (testFlg2)
			ResourceManager::changeHierarchy(hierarchyMeshData, ModelChick,idNumber);

		testFlg2 = false;
	}
	*/

	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(myTransform.rotDeg.y));
	myTransform.rotDeg.y += 1;

	D3DXVECTOR3 cameraPos = Collision::getCameraTransform("Camera", 1)->pos;

//	rideBall(idNumber);
	setWorldMtxPos(myTransform.pos);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンエディット用更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::updateStageEdit()
{
	updateAnimation();
	setWorldMtxPos(myTransform.pos);


#if 0


#if _DEBUG


	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(rot.y));

	// フレーム更新
	UpdateFrameMatrices(hierarchyMeshData.frameRoot, &worldMtx);

	// アニメーション更新
	if (hierarchyMeshData.animCtrlPtrPtr)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - hierarchyMeshData.dwPrev) / 1000.0;
		hierarchyMeshData.dwPrev = dwNow;
		hierarchyMeshData.animCtrlPtrPtr->AdvanceTime(d, nullptr);
	}

#endif
	// コライダー更新
	pCollider->UpdateCollider(worldMtx, DEFAULT_BOXcolor);

#if _DEBUG
	PrintDebugProc("ぷれいや～～～X%f\n", worldMtx._41);
	PrintDebugProc("ぷれいや～～～Y%f\n", worldMtx._42);
	PrintDebugProc("ぷれいや～～～Z%f\n", worldMtx._43);
#endif

	// マテリアル色設定
	if (bHit)
		pCollider->figurePtr->SetMatcolor(HIT_BOX_color);
	else
		pCollider->figurePtr->SetMatcolor(DEFAULT_BOXcolor);

#endif

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Player::PlayerState Player::getState() const
{
	return playerStateType;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータスセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::setStatus(Player::PlayerState setStatus)
{
	playerStateType = setStatus;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スコアセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::setScore(INT nSetNum)
{
	score = nSetNum;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スコア加算
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::addScore()
{
	score ++;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スコア取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
INT Player::getScore() const
{
	return score;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス変更
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::changeStatus()
{
	switch (playerStateType)
	{
	case PlayerState::Stop:
		break;
	case PlayerState::velocity:
		myTransform.accele	= D3DXVECTOR3( 0.0f,0.0f,0.0f);
		myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		isGround	= true;
		break;
	case PlayerState::JumpUp:
		
		if (myTransform.velocity.y < 0.0f)
			playerStateType = PlayerState::JumpDown;
		
		isGround = false;
	
		break;
	case PlayerState::JumpDown:

		isGround = false;

		break;
	case PlayerState::Fall:
	{
		isGround = false;
	}
		break;
	case PlayerState::Dead:
		break;
	default:
		break;
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 状態遷移
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::changeState()
{
	if (playerStateType == PlayerState::JumpUp &&
		myTransform.velocity.y < 0.0f)
	{
		playerStateType = PlayerState::Fall;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::move()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 跳ね返り処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::rebound(size_t index)
{
	const Transform* player1TransformPtr = Collision::getTransform("Player", 0);
	const Transform* player2TransformPtr = Collision::getTransform("Player", 1);
	const Transform* ball1Transform = Collision::getTransform("Ball", 0);
	const Transform* ball2Transform = Collision::getTransform("Ball", 1);

	D3DXVECTOR3 playerToPlayer		= player1TransformPtr->pos - player2TransformPtr->pos;

	playerToPlayer.y = 0.0f;
	playerToPlayer *= 0.08f;

	D3DXVECTOR3 nvpe;
	D3DXVec3Normalize(&nvpe, &playerToPlayer);

	D3DXVECTOR3 pv;
	pv = moveVector + nvpe;

	myTransform.velocity += playerToPlayer;

	Collision::setVelocity("Player", 1, -playerToPlayer);
	myTransform.velocity.y = 0.0f;

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボール乗っかり
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::rideBall(size_t setIndex)
{
	switch (meshType)
	{
	case MeshObjType::NormalModel:
		myTransform.pos = Collision::getTransform("Ball", setIndex)->pos;
		myTransform.pos.y += meshDataObj.collitionBox.y;
		break;
	case MeshObjType::HierarchyModel:
		myTransform.pos = Collision::getTransform("Ball", setIndex)->pos;
		myTransform.pos.y += hierarchyMeshData.CollitionBox.y;
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 落下
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::fall(size_t checkIndex)
{
	if (!Collision::getRayHitData("Player", checkIndex)->isHit)
	{
		if (Collision::getRayHitData("Player", checkIndex)->length < 10.0f)
		{
			myTransform.velocity.y -= FallSpeed;
			DirectX3D::printDebug("落下中");
			fallFlg = false;
		}
	}
	else
	{
		if (fallFlg)
		{
			fallFlg = true;
			MyAudiere::getSe(0)->play();
		}

		const D3DXVECTOR3 cross = Collision::getCross();
		FLOAT ads = std::abs(std::abs(oldPos.y - cross.y));
	
	myTransform.pos = cross;
	//myTransform.pos.y += myTransform.collisionBox.y;
	
	oldPos = myTransform.pos;



	}

	DirectX3D::printDebug("くそおおおおおおおおおおおおおおおおお%f", Collision::getRayHitData("Player", checkIndex)->length);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// モデル変更
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::changeModel()
{
	if (idNumber != SelectManager::getSelect())
	{
		return;
	}

	if (Keyboard::getTrigger(DIK_RIGHTARROW))
	{
		currentModelType ++;
		currentModelType %= ResourceManager::MaxModelType;

		ResourceManager::setHierarchy(&hierarchyMeshData, SelectManager::getModelPass(currentModelType), idNumber);

	}
	if (Keyboard::getTrigger(DIK_LEFTARROW))
	{
		currentModelType --;
		currentModelType %= ResourceManager::MaxModelType;

		ResourceManager::setHierarchy(&hierarchyMeshData, SelectManager::getModelPass(currentModelType), idNumber);

	}



}

void Player::input()
{
	/*
	Xnum = 0.0f;
	Ynum = 0.0f;

	if (idNumber == 0)
	{
		if (Keyboard::getPress(DIK_D))
		{
			Xnum++;
		}
		if (Keyboard::getPress(DIK_A))
		{
			Xnum--;
		}
		if (Keyboard::getPress(DIK_S))
		{
			Ynum--;
		}
		if (Keyboard::getPress(DIK_W))
		{
			Ynum++;
		}
	}
	else if (idNumber == 1)
	{
		if (Keyboard::getPress(DIK_L))
		{
			Xnum++;
		}
		if (Keyboard::getPress(DIK_J))
		{
			Xnum--;
		}
		if (Keyboard::getPress(DIK_K))
		{
			Ynum--;
		}
		if (Keyboard::getPress(DIK_I))
		{
			Ynum++;
		}
	}
	*/
}

void Player::rotation(D3DXVECTOR3 destVec)
{

	D3DXVECTOR3 FowrdVec = getForwardVec();
	D3DXVECTOR3	RightVec = getRightVec();
	D3DXVECTOR3 Upvec = getUpVec();

	radRot = MyVector3::CalcAngleDegree(destVec, -FowrdVec);
	D3DXQUATERNION quatanion;

	if (radRot == 0.0f)
	{
		D3DXQuaternionRotationAxis(&quatanion, &Upvec, oldRadRot);
		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

		StartQuaternion = quatanion;
		rotCnt = 0.0f;
	}
	else
	{
		D3DXQuaternionRotationAxis(&destQua, &Upvec, radRot);

		D3DXQuaternionSlerp(&quatanion, &StartQuaternion, &destQua, rotCnt);
		rotCnt += 0.1f;

		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		oldRadRot = radRot;
	}

	if (rotCnt >= 1.0f)
	{
		rotCnt = 1.0f;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 準備用メイン更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::updateMainReady()
{
	updateAnimation();
	fall(0);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::updateGoal()
{
	updateAnimation();
	myTransform.velocity.x = 0.0f;
	myTransform.velocity.z = 0.0f;

	if (Collision::getRayHitData(tagName, 0)->isHit)
	{
		if (Collision::getRayHitData("Player", 0)->length < 0.0f)
		{
			myTransform.velocity.y = JumpSpeed;
			MyAudiere::getSe(1)->setVolume(0.1f);
			MyAudiere::getSe(1)->play();
		}
	}
	else
	{

	}

	DirectX3D::printDebug("VelocityY%f", myTransform.velocity.y);


	myTransform.pos += myTransform.velocity;
	myTransform.velocity.y -= 0.005f;

	DirectX3D::printDebug("myTransformY%f", myTransform.pos.y);

	setWorldMtxPos(myTransform.pos);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ゲーム中用更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::updatePlayng(D3DXVECTOR3 CameraForward)
{
	if (GameManager::getGameType() == GameManager::GameType::Playing)
	{
		MyAudiere::getSe(0)->setVolume(0.09f);
		static int repeatCnt = 35;

		if (repeatCnt > 35)
		{
			MyAudiere::getSe(0)->play();
			repeatCnt = 0;
		}
		repeatCnt ++;
	}

	oldPos = myTransform.pos;

	if (!isKeyInput)
	{
		inputVec.x = 0.0f;
		inputVec.y = 0.0f;
		inputVec.z = 0.0f;
	}


	D3DXVECTOR3 vv;

	fall(0);

	updateAnimation();

	if (!GameManager::isGameType(GameManager::GameType::Ready))
	{
		if (idNumber == 0)
		{
			if (Keyboard::getPress(DIK_D))
			{
				inputVec.x = SideSpeed;
				isKeyInput = true;
			}
			else
			{
				isKeyInput = false;
			}

			if (Keyboard::getPress(DIK_A))
			{
				inputVec.x = -SideSpeed;
				isKeyInput = true;
			}
			else
			{
				isKeyInput = false;
			}

			if (Keyboard::getPress(DIK_S))
			{
				inputVec.z = -SideSpeed;
				isKeyInput = true;
			}
			else
			{
				isKeyInput = false;
			}

			if (Keyboard::getPress(DIK_W))
			{
				inputVec.z = MoveSpeed;
				isKeyInput = true;
			}
			else
			{
				isKeyInput = false;
			}
		}
		else if (idNumber == 1)
		{
			if (Keyboard::getPress(DIK_L))
			{
				Xnum += MoveSpeed;
			}
			if (Keyboard::getPress(DIK_J))
			{
				Xnum -= MoveSpeed;
			}
			if (Keyboard::getPress(DIK_K))
			{
				Ynum -= MoveSpeed;
			}
			if (Keyboard::getPress(DIK_I))
			{
				Ynum += MoveSpeed;
			}
		}


		inputVec.z = MoveSpeed;
	}

	D3DXVECTOR3 CameraRight = D3DXVECTOR3(CameraForward.z, 0.0f, -CameraForward.x);

	if (GameManager::isGameType(GameManager::GameType::Ready))
	{
		CameraRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}

	moveVector = CameraRight * inputVec.x + CameraForward * inputVec.z;

	D3DXVECTOR3 UpVec = getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);

	if (GameManager::isGameType(GameManager::GameType::Ready))
	{
		D3DXQuaternionRotationAxis(&quatanion, &UpVec, D3DXToRadian(180.0f));
	}
	else
	{
		D3DXQuaternionRotationAxis(&quatanion, &UpVec, D3DXToRadian(0.0f));

	}
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
	//D3DXVec3Normalize(&moveVector, &moveVector);

	moveVector.y = 0.0f;

	myTransform.velocity += (moveVector);

	D3DXVec3Normalize(&vv, &inputVec);
	vv *= 0.001f;

	myTransform.velocity += vv;
	myTransform.pos += myTransform.velocity;

	if (idNumber == 0)
	{
		DirectX3D::printDebug("inputX :%f  IDNumber :%d \n", inputVec.x, idNumber);
		DirectX3D::printDebug("inputY :%f  IDNumber :%d \n", inputVec.y, idNumber);
		DirectX3D::printDebug("inputZ :%f  IDNumber :%d \n", inputVec.z, idNumber);

		DirectX3D::printDebug("xNum :%f \n", Xnum, idNumber);
		DirectX3D::printDebug("yNum :%f \n", Ynum, idNumber);
	}

	D3DXVECTOR3 fowrdVec = getForwardVec();
	D3DXVECTOR3	RightVec = getRightVec();
	D3DXVECTOR3 Upvec = getUpVec();

	inputVec.y = 0.0f;

	radRot = MyVector3::CalcAngleDegree(moveVector, -fowrdVec);

	D3DXQUATERNION quatanion;

	if (radRot == 0.0f)
	{
		D3DXQuaternionRotationAxis(&quatanion, &Upvec, oldRadRot);
		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

		StartQuaternion = quatanion;
		rotCnt = 0.0f;
	}
	else
	{
		D3DXQuaternionRotationAxis(&destQua, &Upvec, radRot);

		D3DXQuaternionSlerp(&quatanion, &StartQuaternion, &destQua, rotCnt);
		rotCnt += 0.1f;

		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		oldRadRot = radRot;
	}

	if (rotCnt >= 1.0f)
	{
		rotCnt = 1.0f;
	}

	if (GameManager::isGameType(GameManager::GameType::Ready))
	{
		myTransform.velocity.z = 0.0f;
	}
	else
	{
		myTransform.velocity.z = MoveSpeed;
		myTransform.velocity.x = 0.0f;
	}

	setWorldMtxPos(myTransform.pos);

}