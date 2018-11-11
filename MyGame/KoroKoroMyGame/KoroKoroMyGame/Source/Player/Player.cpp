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

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Player::Player()
{
	// 位置・向きの初期設定
	pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	// 拡大率設定
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	setDefaultValue();

	// ファイルパス設定
	strcpy_s(fileName, ModelFilePass);

	// 識別用タグ設定
	tagName		= TagType::Player;

	// 状態初期化
	playerStateType = PlayerState::Stop;

	// 行列初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	score = 0;
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
		break;
	case SceneManager::SceneState::SceneStageEdit:
		initializeSceneEdit();
		break;

	default:
		break;
	}

	// 共通処理
	score = 0;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::finalize()
{
	// コライダー解放
//	SAFE_DELETE(pCollider);

	// メッシュ解放
	destroyModelHierarchy();
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
	case  SceneManager::SceneState::SceneStageEdit:
		updateTitle(CameraForward);
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
	Pawn::drawFrame(hierarchyMeshData.frameRoot);

	// コライダー描画
//	pCollider->DrawCollider();
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
	pos		= D3DXVECTOR3(0.0f, -0.01f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);

	setDefaultValue();

	// デバイス取得
	IDirect3DDevice9 * devicePtr = DirectX3D::getDevice();

	// 各種変数初期化
	pD3DTexture  = nullptr;
	pD3DXMesh	 = nullptr;
	pD3DXBuffMat = nullptr;
	numMat		 = 0;

	// Xファイルの読み込み
	ResourceManager::makeModelHierarchy(hierarchyMeshData, fileName, meshType);

	pos.y -= hierarchyMeshData.collitionBox.y * 2;

	// 回転
	D3DXMATRIX mRotX, mRotY, mRotZ;

	D3DXMatrixRotationY(&mRotY, rot.y);
	D3DXMatrixMultiply(&worldMtx, &worldMtx,&mRotY);

	// 拡大
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// 移動
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);


	testVec = pos;

	testVec.z -= 100.0f;
	testVec.x -= 100.0f;
	testVec.y += 0.0f;
	D3DXVec3Normalize(&testVec, &testVec);

	// コライダー初期化
	//pCollider = NEW Collider(pos, hierarchyMeshData.collitionBox);
	//pCollider->initializeCollider();

	// 表示フラグ初期化
	//pCollider->setUsedFlg(false);

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	hierarchyMeshData.animCtrlPtr->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
	hierarchyMeshData.animCtrlPtr->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[2]);

	isUsed = true;

	D3DXQuaternionRotationAxis(&startQuaternion, &getUpVec(), 0);		// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);	// クォータニオンから回転行列掛け合わせ
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンメイン用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::initializeGameMain()
{
	// 位置、移動量、拡大率初期化
	pos		= D3DXVECTOR3(0.0f, 1.84f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 180.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);
	playerStateType = PlayerState::Move;

	setDefaultValue();

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 各種変数初期化
	pD3DTexture  = nullptr;
	pD3DXMesh	 = nullptr;
	pD3DXBuffMat = nullptr;
	numMat		 = 0;

	// Xファイルの読み込み
	ResourceManager::makeModelHierarchy(hierarchyMeshData, fileName,meshType);
//	ResourceManager::CreateTexture(TextureData, texFileName);

	// モデル位置調整
	pos.y -= hierarchyMeshData.collitionBox.y * 2;

	// 拡大
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(rot.y));

	// 移動
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	// コライダー初期化
//	pCollider = new Collider(pos, hierarchyMeshData.collitionBox);
//	pCollider->initializeCollider(pos, hierarchyMeshData.collitionBox, centerPos);

	// 表示フラグ初期化
//	pCollider->setUsedFlg(false);

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	hierarchyMeshData.animCtrlPtr->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[1]);
	hierarchyMeshData.animCtrlPtr->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[1]);

	playerStateType = PlayerState::Stop;
	isGround	= true;

	/*

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	hierarchyMeshData.animCtrlPtrPtr->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	hierarchyMeshData.animCtrlPtrPtr->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
	hierarchyMeshData.animCtrlPtrPtr->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	hierarchyMeshData.animCtrlPtrPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[2]);

	*/

	isUsed = true;

	D3DXQuaternionRotationAxis(&startQuaternion, &getUpVec(), 0);		// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);	// クォータニオンから回転行列掛け合わせ

}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンエディット用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::initializeSceneEdit()
{
	// 位置、移動量、拡大率初期化
	pos		= D3DXVECTOR3(0.0f, -0.01f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// 各種変数初期化
	pD3DTexture		= nullptr;
	pD3DXMesh		= nullptr;
	pD3DXBuffMat	= nullptr;
	numMat			= 0;

	// Xファイルの読み込み
	ResourceManager::makeModelHierarchy(hierarchyMeshData, fileName,meshType);
	
	// モデル回転
	pos.y -= hierarchyMeshData.collitionBox.y * 2;
	
	// 回転
	D3DXMATRIX mRotX, mRotY, mRotZ;

	D3DXMatrixRotationY(&mRotX, D3DXToRadian(0));
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mRotX);

	// 拡大
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// 移動
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	// コライダー初期化
//	pCollider = NEW Collider(pos, hierarchyMeshData.collitionBox);
//	pCollider->initializeCollider();

	// 表示フラグ初期化
//	pCollider->setUsedFlg(false);


	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	hierarchyMeshData.animCtrlPtr->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
	hierarchyMeshData.animCtrlPtr->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[0]);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// リザルト用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::initializeResult()
{
	pos		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 拡大率設定
	scale = D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);

	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture		= nullptr;
	pD3DXMesh		= nullptr;
	pD3DXBuffMat	= nullptr;
	numMat			= 0;

	// Xファイルの読み込み
	makeModel();
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

		startQuaternion = quatanion;
		fCnt = 0.0f;

		PrintDebugProc("ああああああああああああ");
	}
	else
	{
		D3DXQuaternionRotationAxis(&destQua, &Upvec, radRot);


		D3DXQuaternionSlerp(&quatanion, &startQuaternion, &destQua, 1.0f);
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

	// 位置更新
	worldMtx._41 = pos.x;
	worldMtx._42 = pos.y;
	worldMtx._43 = pos.z;

	// フレーム更新
	updateFrameMatrices(hierarchyMeshData.frameRoot, &worldMtx);

	// アニメーション更新
	if (hierarchyMeshData.animCtrlPtr)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - hierarchyMeshData.dwPrev) / 1000.0;
		hierarchyMeshData.dwPrev = dwNow;
		hierarchyMeshData.animCtrlPtr->AdvanceTime(d, nullptr);
	}



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
#if 0

	PrintDebugProc("LangingXXX::::%f\n", DestLanding.x);
	PrintDebugProc("LangingYYY::::%f\n", DestLanding.y);
	PrintDebugProc("LangingZZZ::::%f\n", DestLanding.z);

	// ステータス変更処理
	changeStatus();

	// 状態遷移判定
//	changeState();

	if(PlayerState == PlayerState::TYPE_JUMP_UP)
		bIsGround = false;

	C_SCENE_MANAGER *pScene = GetSceneManager();
	GAME_STATE uCurrentState = pScene->GetInstanse()->GetGameState();
	
	C_XINPUT *pXinput = C_XINPUT::GetInstance();

	short Xnum = pXinput->GetThumbLX();
	short Ynum = pXinput->GetThumbLY();

	
	if (GetKeyboardPress(DIK_D))
		Xnum++;
	if (GetKeyboardPress(DIK_A))
		Xnum--;
	if (GetKeyboardPress(DIK_S))
		Ynum--;
	if (GetKeyboardPress(DIK_W))
		Ynum++;

	if(Xnum == 0.0f && Ynum == 0.0f)
		PlayerState = PlayerState::Stop;
	else
		PlayerState = PlayerState::TYPE_MOVE;





	D3DXVECTOR3 CameraRight = D3DXVECTOR3(CameraForward.z, 0.0f, -CameraForward.x);

	D3DXVec3Normalize(&TestVec, &TestVec);

	TestVec = CameraRight * Xnum + CameraForward * Ynum;

	D3DXVECTOR3 UpVec = GetUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);

	D3DXQuaternionRotationAxis(&Quatanion, &UpVec, 0);			// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &Quatanion);	// クォータニオンから回転行列掛け合わせ
	D3DXVec3Normalize(&TestVec, &TestVec);

	TestVec.y = 0.0f;
	pos += TestVec * 0.3f;

	D3DXVECTOR3 FowrdVec = GetForwardVec();
	D3DXVECTOR3	RightVec = GetRightVec();
	D3DXVECTOR3 Upvec	 = GetUpVec();

	fRadRot = MyVector3::CalcAngleDegree(TestVec, -FowrdVec);
	D3DXQUATERNION quatanion;

	if (fRadRot == 0.0f)
	{
		D3DXQuaternionRotationAxis(&quatanion, &Upvec, fOldRadRot);
		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

		StartQua = quatanion;
		fCnt = 0.0f;
	}
	else
	{
		D3DXQuaternionRotationAxis(&DestQua, &Upvec, fRadRot);

		D3DXQuaternionSlerp(&quatanion, &StartQua, &DestQua, fCnt);
		fCnt += 0.1f;

		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		fOldRadRot = fRadRot;

	}

	if (fCnt >= 1.0f)
		fCnt = 1.0f;

	// ジャンプ
	if(uCurrentState == GAME_NORMAL)
		if ( pXinput->GetButtonTriger(XINPUT_GAMEPAD_A) || GetKeyboardTrigger(DIK_SPACE) && bIsGround)
		{
			PlayerState = PlayerState::TYPE_JUMP_UP;
			AccelePawn.y = 0.555f;
		}

	// 重力加算処理
	if (!bIsGround)
		AccelePawn.y = -0.025f;

	// 移動反映
	move += AccelePawn;

	if (move.y <= -0.3f)
		move.y = -0.3f;

	pos += move;

	if (pos.y < -6.0f && !bIsGround)
		PlayerState = PlayerState::TYPE_DEAD;

	worldMtx._41 = pos.x;
	worldMtx._42 = pos.y;
	worldMtx._43 = pos.z;

#if _DEBUG
	PrintDebugProc("XXX%f\n", TestVec.x);
	PrintDebugProc("YYY%f\n", TestVec.y);
	PrintDebugProc("ZZZ%f\n", TestVec.z);
	PrintDebugProc("ぷれいや～～～X%f\n", worldMtx._41);
	PrintDebugProc("ぷれいや～～～Y%f\n", worldMtx._42);
	PrintDebugProc("ぷれいや～～～Z%f\n", worldMtx._43);
	PrintDebugProc("ぷれいや～じょうたい%d\n", PlayerState);
	PrintDebugProc("moveY%f\n", move.y);
	PrintDebugProc("IsGround%d\n", bIsGround);
#endif


	// マテリアル色設定
	if (bHit)
		pCollider->figurePtr->SetMatcolor(HIT_BOX_color);
	else
		pCollider->figurePtr->SetMatcolor(DEFAULT_BOXcolor);


	// フレーム更新
	UpdateFrameMatrices(hierarchyMeshData.frameRoot, &worldMtx);

	// アニメーション更新
	if (hierarchyMeshData.animCtrlPtrPtr)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - hierarchyMeshData.dwPrev) / 400.0;
		hierarchyMeshData.dwPrev = dwNow;
		hierarchyMeshData.animCtrlPtrPtr->AdvanceTime(d, nullptr);
	}

#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンエディット用更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Player::updateSceneEdit()
{
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
	case PlayerState::Move:
		accele	= D3DXVECTOR3( 0.0f,0.0f,0.0f);
		move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		isGround	= true;
		break;
	case PlayerState::JumpUp:
		
		if (move.y < 0.0f)
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
		move.y < 0.0f)
	{
		playerStateType = PlayerState::Fall;
	}
}