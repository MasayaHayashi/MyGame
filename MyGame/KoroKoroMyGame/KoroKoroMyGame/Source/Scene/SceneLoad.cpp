//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneLoad.cpp
// シーンロード
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "SceneLoad.h"
#include "../DirectX3D/DirectX3D.h"
#include "../SceneManager/SceneManager.h"

/*
#include "C_LoadUiIcon.h"
#include "main.h"
#include "debugproc.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "C_TitleObj.h"
#include "C_FADE.h"
*/

// ===== 静的メンバ =====
bool C_SCENE_LOAD::finishedLoad; // ロード終了判定

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_SCENE_LOAD::C_SCENE_LOAD()
{
	finishedLoad = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_SCENE_LOAD::~C_SCENE_LOAD()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 別スレッド用ロード
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void InitNextResorce()
{
	std::mutex mutexObj;
	mutexObj.lock();

	SceneManager *pSceneManager = GetSceneManager();

	SceneManager::SceneState nextScene = SceneManager::getNextScene();

	SceneManager::SceneState NextScene = pSceneManager->GetNextScene();
	pSceneManager->SetCurrentScene(NextScene);	// 現在のシーンを次のシーンに上書き
	pSceneManager->SetScene(NextScene);

	// 次のシーンの初期化
	pSceneManager->InitScene();


	mutexObj.unlock();

	C_SCENE_LOAD::SetLoadFlg(true);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_LOAD::InitScene()
{
	// ロードアイコン初期化
	pLoadIcon = NEW C_LOAD_UI_ICON;
	pLoadIcon->InitObject();

	// カメラ初期化
	pCamera = NEW C_CAMERA;
	pCamera->InitCamera();

	// ライト初期化
	pLight = NEW C_LIGHT;
	pLight->InitLight();

	
	pTitleObj = NEW C_TITLE_OBJ;
	pTitleObj->InitObject();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_LOAD::UninitScene()
{
	// タイトルロゴ後処理
	pLoadIcon->UninitObject();
	SAFE_DELETE(pLoadIcon);

	pCamera->UninitCamera();
	SAFE_DELETE(pCamera);

	pLight->UninitLight();
	SAFE_DELETE(pLight);

	pTitleObj->UninitObject();
	SAFE_DELETE(pTitleObj);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_LOAD::UpdateLoad()
{
	// ロードオブジェクト更新
	pLoadIcon->UpdateObject();
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ロード開始
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_LOAD::EnableLoad()
{
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

#if 1

	// 新規スレッドを作成
	std::thread Thred(InitNextResorce);

	// ロード画面描画
	while (1)
	{
		dwCurrentTime = timeGetTime();

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
		{
			dwExecLastTime = dwCurrentTime;

			UpdateLoad();
			DrawScene();

			// ロード完了
			if (finishedLoad)
				break;
		}
	}
	Thred.join();

	finishedLoad = false;
#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_LOAD::DrawScene()
{
	// オブジェクト取得
	LPDIRECT3DDEVICE9 devicePtr = ();

	// バックバッファ＆Ｚバッファのクリア
	devicePtr->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(devicePtr->BeginScene()))
	{
		pLoadIcon->DrawObject();	// ロード

		pCamera->SetCamera();		// カメラセット

		// Direct3Dによる描画の終了
		devicePtr->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	devicePtr->Present(NULL, NULL, NULL, NULL);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ロードフラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_LOAD::SetLoadFlg(bool bSet)
{
	finishedLoad = bSet;
}