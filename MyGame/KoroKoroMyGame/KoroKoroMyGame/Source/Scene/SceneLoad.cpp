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

	SceneManager *pSceneManager = getSceneManager();

	SceneManager::SceneState nextScene = SceneManager::getNextScene();

	SceneManager::SceneState NextScene = pSceneManager->getNextScene();
	pSceneManager->setCurrentScene(NextScene);	// 現在のシーンを次のシーンに上書き
	pSceneManager->setScene(NextScene);

	// 次のシーンの初期化
	pSceneManager->InitScene();


	mutexObj.unlock();

	C_SCENE_LOAD::setLoadFlg(true);

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
	pLight = NEW Light;
	pLight->InitLight();

	
	pTitleObj = NEW C_TITLE_OBJ;
	pTitleObj->InitObject();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_LOAD::finalizeScene()
{
	// タイトルロゴ後処理
	pLoadIcon->finalizeObject();
	SAFE_DELETE(pLoadIcon);

	pCamera->finalizeCamera();
	SAFE_DELETE(pCamera);

	pLight->finalizeLight();
	SAFE_DELETE(pLight);

	pTitleObj->finalizeObject();
	SAFE_DELETE(pTitleObj);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_LOAD::updateLoad()
{
	// ロードオブジェクト更新
	pLoadIcon->updateObject();
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

	dwExecLastTime = dwFPSLastTime = timegetTime();
	dwCurrentTime = dwFrameCount = 0;

#if 1

	// 新規スレッドを作成
	std::thread Thred(InitNextResorce);

	// ロード画面描画
	while (1)
	{
		dwCurrentTime = timegetTime();

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
		{
			dwExecLastTime = dwCurrentTime;

			updateLoad();
			drawScene();

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
void C_SCENE_LOAD::drawScene()
{
	// オブジェクト取得
	LPDIRECT3DDEVICE9 devicePtr = ();

	// バックバッファ＆Ｚバッファのクリア
	devicePtr->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(devicePtr->BeginScene()))
	{
		pLoadIcon->drawObject();	// ロード

		pCamera->setCamera();		// カメラセット

		// Direct3Dによる描画の終了
		devicePtr->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	devicePtr->Present(nullptr, nullptr, nullptr, nullptr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ロードフラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_SCENE_LOAD::setLoadFlg(bool bset)
{
	finishedLoad = bset;
}