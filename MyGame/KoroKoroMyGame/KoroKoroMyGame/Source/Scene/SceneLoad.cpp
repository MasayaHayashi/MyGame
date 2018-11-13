//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneLoad.cpp
// シーンロード
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "SceneLoad.h"
#include "../DirectX3D/DirectX3D.h"
#include "../SceneManager/SceneManager.h"
#include "Load\SceneLoad.h"

/*
#include "C_LoadUiIcon.h"
#include "main.h"
#include "debugproc.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "C_TitleObj.h"
#include "FadeUI.h"
*/

// ===== 静的メンバ =====
bool SceneLoad::finishedLoad;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneLoad::SceneLoad()
{
	finishedLoad = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneLoad::~SceneLoad()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 別スレッド用ロード
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void initializeNextResorce()
{
	std::mutex mutexObj;
	mutexObj.lock();

	SceneManager::SceneState nextScene = SceneManager::getNextScene(); SceneManager::getNextScene();

	SceneManager::SceneState NextScene = SceneManager::getNextScene();
	SceneManager::setCurrentScene(NextScene);	// 現在のシーンを次のシーンに上書き
	SceneManager::changeScene(NextScene);

	// 次のシーンの初期化
	SceneManager::initialize();


	mutexObj.unlock();

	SceneLoad::setLoadFlg(true);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::initialize()
{
	/*
	// ロードアイコン初期化
	pLoadIcon = NEW C_LOAD_UI_ICON;
	pLoadIcon->initialize();

	// カメラ初期化
	pCamera = NEW C_CAMERA;
	pCamera->initializeCamera();

	// ライト初期化
	pLight = NEW Light;
	pLight->initializeLight();

	
	pTitleObj = NEW HeartObj;
	pTitleObj->initialize();
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::finalize()
{
	/*
	// タイトルロゴ後処理
	pLoadIcon->finalizeObject();
	SAFE_DELETE(pLoadIcon);

	pCamera->finalizeCamera();
	SAFE_DELETE(pCamera);

	pLight->finalizeLight();
	SAFE_DELETE(pLight);

	pTitleObj->finalizeObject();
	SAFE_DELETE(pTitleObj);
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::update()
{
	// ロードオブジェクト更新
//	pLoadIcon->updateObject();
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ロード開始
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::enable()
{
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

#if 1

	// 新規スレッドを作成
	std::thread Thred(initializeNextResorce);

	// ロード画面描画
	while (1)
	{
		dwCurrentTime = timeGetTime();

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
		{
			dwExecLastTime = dwCurrentTime;

			update();
			draw();

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
void SceneLoad::draw()
{
	// オブジェクト取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// バックバッファ＆Ｚバッファのクリア
	devicePtr->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(devicePtr->BeginScene()))
	{
		//pLoadIcon->drawObject();	// ロード

		//pCamera->setCamera();		// カメラセット

		// Direct3Dによる描画の終了
		devicePtr->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	devicePtr->Present(nullptr, nullptr, nullptr, nullptr);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ロードフラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::setLoadFlg(bool bset)
{
	finishedLoad = bset;
}