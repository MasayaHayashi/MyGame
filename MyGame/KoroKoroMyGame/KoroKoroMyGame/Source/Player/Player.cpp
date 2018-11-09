//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_Player.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Player.h"
#include "../KeyBoard/Keyboard.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"

#include "C_Collider.h"
#include "C_MyVector3.h"
#include "collision.h"
#include "main.h"
#include "C_Xinput.h"

// ===== 定数・マクロ定義 =====
#define MODEL_NAME	 "data/MODEL/PenguinC.x"
#define MOVE_FORWARD_SPEED  (0.45f)		// 前への移動量
#define MOVE_SIDE_SPEED		(0.3f)		// 横への移動量
#define VALUE_SCALE_SIZE	(1.0f)		// 拡大率

#define BLOCK_POS_Y (1.84f)

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_PLAYER::C_PLAYER()
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
	strcpy_s(fileName, MODEL_NAME);

	// 識別用タグ設定
	tagName		= TagType::Player;

	// 状態初期化
	PlayerState = PLAYER_STATE::TYPE_STOP;

	// 行列初期化
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	nScore = 0;
	OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_PLAYER::~C_PLAYER()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::InitObject()
{
	SceneManager::SceneState currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		InitPlayer_Title();
		break;
	case SceneManager::SceneState::SceneMain:
		InitPlayer_GameMain();
		break;
	case SceneManager::SceneState::SceneResult:
		InitPlayer_Result();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		InitPlayer_SceneEdit();
		break;

	default:
		break;
	}

	// 共通処理
	nScore = 0;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::UninitObject()
{
	// コライダー解放
	SAFE_DELETE(pCollider);

	// メッシュ解放
	destroyModelHierarchy();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::UpdateObject(D3DXVECTOR3 CameraForward)
{
	SceneManager::SceneState currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		UpdatePlayer_Title(CameraForward);
		break;
	case  SceneManager::SceneState::SceneMain:
		UpdatePlayer_GameMain(CameraForward);
		break;
	case  SceneManager::SceneState::SceneResult:
		UpdatePlayer_Result();
		break;
	case  SceneManager::SceneState::SceneStageEdit:
		UpdatePlayer_Title(CameraForward);
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::DrawObject()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();
	
	// 描画
	Pawn::drawFrame(hierarchyMeshData.pFrameRoot);

	// コライダー描画
	pCollider->DrawCollider();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// リザルト用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::UpdatePlayer_Result()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンタイトル用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::InitPlayer_Title()
{
	// 位置、移動量、拡大率初期化
	pos		= D3DXVECTOR3(0.0f, -0.01f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);

	SetDefaultValue();

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 各種変数初期化
	pD3DTexturePawn  = nullptr;
	pD3DXMesh	 = nullptr;
	pD3DXBuffMatPawn = nullptr;
	uNumMatPawn		 = 0;

	// Xファイルの読み込み
	C_RESOURCE_MANAGER::MakeModelHierarchy(HierarchyMeshData, fileName, uMeshType);

	pos.y -= HierarchyMeshData.CollitionBox.y * 2;

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

	// コライダー初期化
	pCollider = NEW Collider(pos, HierarchyMeshData.CollitionBox);
	pCollider->InitCollider();

	// 表示フラグ初期化
	pCollider->SetUsedFlg(false);

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	HierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, HierarchyMeshData.ppAnimSet[0]);
	HierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, HierarchyMeshData.ppAnimSet[2]);


	bUsed = true;

	TestVec = pos;


	TestVec.z -= 100.0f;
	TestVec.x -= 100.0f;
	TestVec.y += 0.0f;
	D3DXVec3Normalize(&TestVec, &TestVec);



	D3DXQuaternionRotationAxis(&StartQua, &GetUpVec(), 0);		// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &Quatanion);	// クォータニオンから回転行列掛け合わせ
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンメイン用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::InitPlayer_GameMain()
{
	// 位置、移動量、拡大率初期化
	pos		= D3DXVECTOR3(0.0f, 1.84f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 180.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);
	PlayerState = PLAYER_STATE::TYPE_MOVE;

	SetDefaultValue();

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 各種変数初期化
	pD3DTexturePawn  = nullptr;
	pD3DXMesh	 = nullptr;
	pD3DXBuffMatPawn = nullptr;
	uNumMatPawn		 = 0;

	// Xファイルの読み込み
	C_RESOURCE_MANAGER::MakeModelHierarchy(HierarchyMeshData, fileName,uMeshType);
//	C_RESOURCE_MANAGER::CreateTexture(TextureData, szTexFileName);

	// モデル位置調整
	pos.y -= HierarchyMeshData.CollitionBox.y * 2;

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
	pCollider = NEW Collider(pos, HierarchyMeshData.CollitionBox);
	pCollider->InitCollider(pos, HierarchyMeshData.CollitionBox, CenterPos);

	// 表示フラグ初期化
	pCollider->SetUsedFlg(false);

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	HierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, HierarchyMeshData.ppAnimSet[1]);
	HierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, HierarchyMeshData.ppAnimSet[1]);

	PlayerState = PLAYER_STATE::TYPE_STOP;
	bIsGround	= true;

	/*

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	HierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, HierarchyMeshData.ppAnimSet[0]);
	HierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, HierarchyMeshData.ppAnimSet[2]);

	*/

	bUsed = true;

	TestVec = pos;


	TestVec.z -= 100.0f;
	TestVec.x -= 100.0f;
	TestVec.y += 0.0f;
	D3DXVec3Normalize(&TestVec, &TestVec);



	D3DXQuaternionRotationAxis(&StartQua, &GetUpVec(), 0);		// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &Quatanion);	// クォータニオンから回転行列掛け合わせ

}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンエディット用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::InitPlayer_SceneEdit()
{
	// 位置、移動量、拡大率初期化
	pos		= D3DXVECTOR3(0.0f, -0.01f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 各種変数初期化
	pD3DTexturePawn		= nullptr;
	pD3DXMesh		= nullptr;
	pD3DXBuffMatPawn	= nullptr;
	uNumMatPawn			= 0;

	// Xファイルの読み込み
	C_RESOURCE_MANAGER::MakeModelHierarchy(HierarchyMeshData, fileName,uMeshType);
//	C_RESOURCE_MANAGER::CreateTexture(TextureData, szTexFileName);
	
	// モデル回転
	pos.y -= HierarchyMeshData.CollitionBox.y * 2;
	
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
	pCollider = NEW Collider(pos, HierarchyMeshData.CollitionBox);
	pCollider->InitCollider();

	// 表示フラグ初期化
	pCollider->SetUsedFlg(false);


	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	HierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, HierarchyMeshData.ppAnimSet[0]);
	HierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, HierarchyMeshData.ppAnimSet[0]);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// リザルト用プレイヤー初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::InitPlayer_Result()
{
	pos		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 拡大率設定
	scale = D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pD3DTexturePawn		= nullptr;
	pD3DXMesh		= nullptr;
	pD3DXBuffMatPawn	= nullptr;
	uNumMatPawn			= 0;

	// Xファイルの読み込み
	MakeModel();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::InitStatus()
{
	C_3DPAWN::InitStatus();
	
	PlayerState = PLAYER_STATE::TYPE_STOP;
	bIsGround = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンタイトル用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::UpdatePlayer_Title(D3DXVECTOR3 CameraForward)
{
	C_XINPUT *pXinput = C_XINPUT::GetInstance();

	short Xnum = pXinput->GetThumbLX();
	short Ynum = pXinput->GetThumbLY();

	D3DXVECTOR3 CameraRight = D3DXVECTOR3(CameraForward.z,0.0f,-CameraForward.x);

	PrintDebugProc("xxxxxxxxxxxxxxxxxxxxx:%d\n", Xnum);
	PrintDebugProc("YYYYYYYYYYYYYYYYYYYYY:%d\n", Ynum);

	D3DXVec3Normalize(&TestVec, &TestVec);

//	TestVec.x = CameraRight.x * CameraForward.x +Xnum;
//	TestVec.z = CameraForward.z * CameraRight.z + Ynum;

	TestVec = CameraRight * Xnum + CameraForward * Ynum;
	
	D3DXVECTOR3 UpVec = GetUpVec();
	D3DXVec3Normalize(&UpVec,&UpVec);

	D3DXQuaternionRotationAxis(&Quatanion, &UpVec, 0);			// クォータニオンでの任意軸回転
	D3DXMatrixRotationQuaternion(&worldMtx, &Quatanion);	// クォータニオンから回転行列掛け合わせ
	D3DXVec3Normalize(&TestVec, &TestVec);

	TestVec.y = 0.0f;
	pos += TestVec * 0.3f;
	
	D3DXVECTOR3 FowrdVec = GetForwardVec();
	D3DXVECTOR3	RightVec = GetRightVec();
	D3DXVECTOR3 Upvec	 = GetUpVec();	
	
	fRadRot = C_MYVECTOR3::CalcAngleDegree(TestVec, -FowrdVec);
	D3DXQUATERNION quatanion;

	if (fRadRot == 0.0f)
	{
		D3DXQuaternionRotationAxis(&quatanion, &Upvec, fOldRadRot);
		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

		StartQua = quatanion;
		fCnt = 0.0f;

		PrintDebugProc("ああああああああああああ");
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

	PrintDebugProc("xxx%f\n", TestVec.x);
	PrintDebugProc("yyy%f\n", TestVec.y);
	PrintDebugProc("zzz%f\n", TestVec.z);

	PrintDebugProc("ddddddddddd%f",fCnt);

//	D3DXVec3Normalize(&TestVec, &TestVec);
//	D3DXVec3Normalize(&FowrdVec, &FowrdVec);


	/*

	FLOAT TestLength	= D3DXVec3Length(&TestVec);
	FLOAT FowedLength	= D3DXVec3Length(&FowrdVec);
	FLOAT RightLength	= D3DXVec3Length(&RightVec);

//	FLOAT fLength = sqrtf(TestVec.x * TestVec.x + FowrdVec.z * FowrdVec.z);
//	D3DXVec3Cross()

	FLOAT TestDot = D3DXVec3Dot(&TestVec, &FowrdVec);
	FLOAT RightDot = D3DXVec3Dot(&TestVec, &RightVec);

	TestDot /= (TestLength * FowedLength);
	FLOAT test		= acos(TestDot);
	test = D3DXToDegree(test);

	D3DXQUATERNION quatanion;
	
	if (RightDot >= 0.0f)
	{
		// 変化値が小さければ更新しない
		if (D3DXToDegree(RightDot) > 20.0f)
		{
			D3DXQuaternionRotationAxis(&quatanion, &GetUpVec(), D3DXToRadian(test));
			D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		}
	}
	else
	{
//		RightDot *= -1;

		// 変化値が小さければ処理しない
		if (D3DXToDegree(RightDot) > 20.0f)
		{
			D3DXQuaternionRotationAxis(&quatanion, &GetUpVec(), D3DXToRadian(-test));
			D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		}
	}


	PrintDebugProc("xxxx%f\n", FowrdVec.x);
	PrintDebugProc("yyyy%f\n", FowrdVec.y);
	PrintDebugProc("zzzz%f\n", FowrdVec.z);

	PrintDebugProc("RightDot%f\n", D3DXToDegree(test));


	PrintDebugProc("回転角度:%f", test);


	*/

	// 位置更新
	worldMtx._41 = pos.x;
	worldMtx._42 = pos.y;
	worldMtx._43 = pos.z;

//	FLOAT test = acos(TestDot);
	
//	test = D3DXToDegree(test);

	// フレーム更新
	UpdateFrameMatrices(HierarchyMeshData.pFrameRoot, &worldMtx);

	// アニメーション更新
	if (HierarchyMeshData.pAnimCtrl)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - HierarchyMeshData.dwPrev) / 1000.0;
		HierarchyMeshData.dwPrev = dwNow;
		HierarchyMeshData.pAnimCtrl->AdvanceTime(d, nullptr);
	}



	// コライダー更新
	pCollider->UpdateCollider(worldMtx,DEFAULT_BOXCOLOR);

#if _DEBUG
	PrintDebugProc("ぷれいや～～～X%f\n", pos.x);
	PrintDebugProc("ぷれいや～～～Y%f\n", pos.y);
	PrintDebugProc("ぷれいや～～～Z%f\n", pos.z);
#endif

	// マテリアル色設定
	if (bHit)
		pCollider->figurePtr->SetMatColor(HIT_BOX_COLOR);
	else
		pCollider->figurePtr->SetMatColor(DEFAULT_BOXCOLOR);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンメイン用プレイヤー更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::UpdatePlayer_GameMain(D3DXVECTOR3 CameraForward)
{
	PrintDebugProc("LangingXXX::::%f\n", DestLanding.x);
	PrintDebugProc("LangingYYY::::%f\n", DestLanding.y);
	PrintDebugProc("LangingZZZ::::%f\n", DestLanding.z);

	// ステータス変更処理
	ChangeStatus();

	// 状態遷移判定
//	ChangeState();

	if(PlayerState == PLAYER_STATE::TYPE_JUMP_UP)
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
		PlayerState = PLAYER_STATE::TYPE_STOP;
	else
		PlayerState = PLAYER_STATE::TYPE_MOVE;





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

	fRadRot = C_MYVECTOR3::CalcAngleDegree(TestVec, -FowrdVec);
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
			PlayerState = PLAYER_STATE::TYPE_JUMP_UP;
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
		PlayerState = PLAYER_STATE::TYPE_DEAD;

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
	PrintDebugProc("MovePawnY%f\n", move.y);
	PrintDebugProc("IsGround%d\n", bIsGround);
#endif


	// マテリアル色設定
	if (bHit)
		pCollider->figurePtr->SetMatColor(HIT_BOX_COLOR);
	else
		pCollider->figurePtr->SetMatColor(DEFAULT_BOXCOLOR);


	// フレーム更新
	UpdateFrameMatrices(HierarchyMeshData.pFrameRoot, &worldMtx);

	// アニメーション更新
	if (HierarchyMeshData.pAnimCtrl)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - HierarchyMeshData.dwPrev) / 400.0;
		HierarchyMeshData.dwPrev = dwNow;
		HierarchyMeshData.pAnimCtrl->AdvanceTime(d, nullptr);
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// シーンエディット用更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::UpdatePlayer_SceneEdit()
{

#if _DEBUG


	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(rot.y));

	// フレーム更新
	UpdateFrameMatrices(HierarchyMeshData.pFrameRoot, &worldMtx);

	// アニメーション更新
	if (HierarchyMeshData.pAnimCtrl)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - HierarchyMeshData.dwPrev) / 1000.0;
		HierarchyMeshData.dwPrev = dwNow;
		HierarchyMeshData.pAnimCtrl->AdvanceTime(d, nullptr);
	}

#endif
	// コライダー更新
	pCollider->UpdateCollider(worldMtx, DEFAULT_BOXCOLOR);

#if _DEBUG
	PrintDebugProc("ぷれいや～～～X%f\n", worldMtx._41);
	PrintDebugProc("ぷれいや～～～Y%f\n", worldMtx._42);
	PrintDebugProc("ぷれいや～～～Z%f\n", worldMtx._43);
#endif

	// マテリアル色設定
	if (bHit)
		pCollider->figurePtr->SetMatColor(HIT_BOX_COLOR);
	else
		pCollider->figurePtr->SetMatColor(DEFAULT_BOXCOLOR);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
PLAYER_STATE::PLAYER_STATE C_PLAYER::GetState()
{
	return PlayerState;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータスセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::SetStatus(PLAYER_STATE::PLAYER_STATE SetStatus)
{
	PlayerState = SetStatus;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スコアセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::SetScore(INT nSetNum)
{
	nScore = nSetNum;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スコア加算
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::AddScore()
{
	nScore ++;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スコア取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
INT C_PLAYER::GetScore()
{
	return nScore;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス変更
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::ChangeStatus()
{
	switch (PlayerState)
	{
	case PLAYER_STATE::TYPE_STOP:
		break;
	case PLAYER_STATE::TYPE_MOVE:
		AccelePawn	= D3DXVECTOR3( 0.0f,0.0f,0.0f);
		move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bIsGround	= true;
		break;
	case PLAYER_STATE::TYPE_JUMP_UP:
		
		if (move.y < 0.0f)
			PlayerState = PLAYER_STATE::TYPE_JUMP_DOWN;

		bIsGround = false;
	
		break;
	case PLAYER_STATE::TYPE_JUMP_DOWN:

		bIsGround = false;

		break;
	case PLAYER_STATE::TYPE_FALL:
	{
		bIsGround = false;

	//	PlayerState = TYPE_MOVE;
		/*
		pos = DestLanding;
		bIsGround = true;
		AccelePawn.y = 0.0f;
		move.y = 0.0f;
		PlayerState = TYPE_MOVE;
		*/
	}
		break;
	case PLAYER_STATE::TYPE_DEAD:
		break;
	default:
		break;
	}

	// 共通処理

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 状態遷移
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_PLAYER::ChangeState()
{
	if (PlayerState == PLAYER_STATE::TYPE_JUMP_UP &&
		move.y < 0.0f)
	{
		PlayerState = PLAYER_STATE::TYPE_FALL;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// プレイヤー移動方向ベクトル取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 C_PLAYER::GetMoveVec()
{
	return TestVec;
}