//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// DirectX3D.cpp
// DirectX9.0c用クラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "DirectX3D.h"
#include "Application.h"

// ===== 静的メンバ変数 =====
//std::unique_ptr<LPDIRECT3DDEVICE9> DirectX3D::pDirectXDevice(new LPDIRECT3DDEVICE9);


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
DirectX3D::DirectX3D()
{
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
DirectX3D::~DirectX3D()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT DirectX3D::init(HWND& wnd)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	directXObj = Direct3DCreate9(D3D_SDK_VERSION);

	if (directXObj == nullptr)
		return E_FAIL;

	// 現在のディスプレイモードを取得
	directXObj->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);


	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount  = 1;								// バックバッファの数
	d3dpp.BackBufferWidth  = (Application::ScreenWidth);	// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = (Application::ScreenHeight);	// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;					// カラーモードの指定
	d3dpp.SwapEffect	   = D3DSWAPEFFECT_DISCARD;			// 映像信号に同期してフリップする
	d3dpp.Windowed		   = isWindowMode;
	d3dpp.EnableAutoDepthStencil = TRUE;					// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// デプスバッファとして16bitを使う

	if (isWindowMode)
	{
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	// 描画と頂点処理をハードウェアで行なう
	if (FAILED( (directXObj)->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		wnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp, &directXDevice)))
	{
		// 上記の設定が失敗したら描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED( directXObj->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			wnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &directXDevice )))
		{
			// 上記の設定が失敗したら描画と頂点処理をCPUで行なう
			if (FAILED( directXObj->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				wnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &directXDevice )))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}


	//シェーダーを読み込み
	if (FAILED(D3DXCreateEffectFromFile( directXDevice, "Data/FX/Min.fx", nullptr, nullptr, 0, nullptr, &effectObj, nullptr)))
	{
		MessageBox(nullptr, "シェーダーファイル読み込み失敗", "", MB_OK);
		return E_FAIL;
	}

	// レンダーステートパラメータの設定	
	directXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// 裏面をカリング
	directXDevice->SetRenderState(D3DRS_ZENABLE, TRUE);					// Zバッファを使用
	directXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	directXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	directXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	directXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);			// 法線を常に正規化する設定
	
	directXDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	directXDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	directXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	directXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定
	
	directXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	directXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	directXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	
	/*
	// ステージ番号初期化
	g_StageNum = 0;	// チュートリアルステージ

	// 入力の初期化処理
	InitInput(hInstance, hWnd);

	// Xinput生成
	C_XINPUT::Create();

	// Xinput初期化
	g_pXinput = C_XINPUT::GetInstance();
	g_pXinput->InitXinput();

#if _DEBUG
		// デバッグ表示の初期化処理
		InitDebugProc();
#endif

	// サウンド処理の初期化
	g_Audio = NEW C_AUDIO();

	// シーン初期化
	g_pSceneManager = NEW C_SCENE_MANAGER();
	g_pSceneManager->InitScene();

	// ロードシーン初期化
	g_pSceneLoad = NEW C_SCENE_LOAD;
	g_pSceneLoad->InitScene();

	return S_OK;
	*/

	return E_FAIL;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const void DirectX3D::draw()
{
	
	// バックバッファ＆Ｚバッファのクリア
	directXDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(directXDevice->BeginScene()))
	{
		// シーン描画
	//	pSceneManager->DrawScene();

		// フェード描画
	//	g_pSceneManager->DrawFadeScene();

		// デバッグ表示の描画処理
#if _DEBUG
		//if (g_bDispDebug)
		//	DrawDebugProc();
#endif


		// Direct3Dによる描画の終了
		directXDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	directXDevice->Present(nullptr, nullptr, nullptr, nullptr);
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デバッグ初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT DirectX3D::initDebugProc()
{
	HRESULT hr;

	// 情報表示用フォントを設定
	hr = D3DXCreateFont(directXDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		 OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &pD3DXFont);

	// 情報クリア
	memset(debug, 0, sizeof debug);
	

	return E_FAIL;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デバッグ更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void DirectX3D::updateDebugProc()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デバッグ描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const void DirectX3D::drawDebugProc()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デバッグ後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void DirectX3D::finalizeDebugProc()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デバイス取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LPDIRECT3DDEVICE9 DirectX3D::getDevice()
{
	//return pDirectXDevice;

	return nullptr;
}