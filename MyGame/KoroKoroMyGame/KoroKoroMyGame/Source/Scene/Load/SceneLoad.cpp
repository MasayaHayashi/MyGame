//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneLoad.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "SceneLoad.h"
#include "../../SceneManager/SceneManager.h"
#include "../../DirectX3D/DirectX3D.h"
#include "../../Camera/Camera.h"
#include "../../Light/Light.h"
#include "../../LoadIcon/MyLoadIcon.h"

/*
#include "C_LoadUiIcon.h"
#include "main.h"
#include "debugproc.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "C_TitleObj.h"
#include "C_FADE.h"
*/

// ===== グローバル変数宣言 =====
std::mutex *pMutex;
bool SceneLoad::isFinishedLoad; // ロード終了判定

std::unique_ptr<Camera>			SceneLoad::cameraPtr;
std::unique_ptr<Light>			SceneLoad::lightPtr;
std::unique_ptr<MyLoadIcon>		SceneLoad::myLoadIConPtr;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneLoad::SceneLoad()
{
	pMutex = NEW std::mutex;
	cameraPtr.reset(NEW Camera());
	lightPtr.reset(NEW Light());
	myLoadIConPtr.reset(NEW MyLoadIcon());

	isFinishedLoad = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SceneLoad::~SceneLoad()
{
	delete pMutex;
	pMutex = nullptr;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 別スレッド用ロード
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void InitNextResorce()
{
	pMutex->lock();

	SceneManager::SceneState nextScene = SceneManager::getNextScene();

//	C_SCENE_MANAGER::SCENE_STATE NextScene = pSceneManager->GetNextScene();

	SceneManager::setCurrentScene(nextScene);
	SceneManager::changeScene(nextScene);

//	pSceneManager->SetCurrentScene(NextScene);	// 現在のシーンを次のシーンに上書き
//	pSceneManager->SetScene(NextScene);

	SceneManager::getInstanse()->initialize();

	pMutex->unlock();

	SceneLoad::setLoadFlg(true);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::initialize()
{
	cameraPtr->initialize();
	lightPtr->initialize();
	myLoadIConPtr->initialize();


	/*
	pTitleObj = NEW C_TITLE_OBJ;
	pTitleObj->InitObject();
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::finalize()
{
	/*
	// タイトルロゴ後処理
	pLoadIcon->UninitObject();
	SAFE_DELETE(pLoadIcon);

	pCamera->UninitCamera();
	SAFE_DELETE(pCamera);

	pLight->UninitLight();
	SAFE_DELETE(pLight);

	pTitleObj->UninitObject();
	SAFE_DELETE(pTitleObj);
	*/
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::update()
{
	// ロードオブジェクト更新
	myLoadIConPtr->update();
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
	std::thread Thred(InitNextResorce);

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
			if (isFinishedLoad)
			{
				break;
			}
		}
	}
	Thred.join();

	isFinishedLoad = false;
#endif
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::draw()
{
	// オブジェクト取得
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		myLoadIConPtr->draw();	// ロード

		cameraPtr->setCamera();		// カメラセット

		// Direct3Dによる描画の終了
		pDevice->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	pDevice->Present(NULL, NULL, NULL, NULL);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ロードフラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SceneLoad::setLoadFlg(bool bSet)
{
	isFinishedLoad = bSet;
}