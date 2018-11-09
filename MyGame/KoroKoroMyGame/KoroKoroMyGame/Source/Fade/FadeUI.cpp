//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_FADE.cpp
// フェードクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "FadeUI.h"
#include "../Application/Application.h"
#include "../SceneManager/SceneManager.h"
#include "../Scene/Load/SceneLoad.h"

// ===== 定数・マクロ定義 =====
#define TEXTURE_NAME		"data/TEXTURE/test.png"
#define TEXTURE_NAME_STAR	"data/TEXTURE/Background.png"
#define FADE_RATE (0.02f)

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
FadeUI::FadeUI()
{
	VertexBoard.bFade = true;
	CurentFadeType	= FadeType::FadeNone;
	CurrentColor	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//CurrentColor	= D3DXCOLOR(0.0f, 0.635f, 0.91f, 0.0f);
	//Color = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.0f);
	strcpy_s(szFileName, TEXTURE_NAME);
	VertexBoard.SizeBoard		 = D3DXVECTOR3(Application::ScreenWidth, Application::ScreenHeight, 0.0f);
	VertexBoard.PosBoard		 = D3DXVECTOR3(Application::ScreenWidth * 0.5f, Application::ScreenHeight * 0.5f, 0.0f);
	VertexBoard.ScaleBoard		 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	VertexBoard.fRadAngleBoard	 = D3DXToRadian(0);
	VertexBoard.BoardType		= POLYGON_2D;
	bAlphaBlendBoard = false;
	uNumber			 = 0;

	TexPatternDivideX		= 1;
	TexPatternDivideY		= 1;
	TexUV_SizeX				= 1.0f / TexPatternDivideX;
	TexUV_SizeY				= 1.0f / TexPatternDivideY;
	NumAnimPattern			= TexPatternDivideX * TexPatternDivideY;
	CurrentAnimPattern		= 1;
	nIntervalChangePattern	= 7;

	bUsed = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
FadeUI::~FadeUI()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::InitObject()
{
	strcpy_s(szFileName, TEXTURE_NAME);
//	Texture.pD3DTexture = NULL;
	//	CurentFadeType = FADE_IN;
	CurrentColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

	C_RESOURCE_MANAGER::MakeVertexBoard(VertexBoard,szFileName);	// 頂点情報生成
	C_RESOURCE_MANAGER::CreateTexture(Texture,szFileName);		// テクスチャ生成

	Texture.pD3DTexture = NULL;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::UninitObject()
{
//	C_BOARD::UninitObject();
	
	// 頂点情報解放
	C_RESOURCE_MANAGER::DestroyFadeVtx();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::UpdateObject()
{
	if (CurentFadeType != FADE_NONE)
	{// フェード処理中
		if (FADE_OUT == CurentFadeType)
		{
			// フェードアウト処理
			CurrentColor.a += FADE_RATE;		// α値を加算して画面を消していく

			if (CurrentColor.a >= 1.0f)
			{
				// フェードイン処理に切り替え
				CurrentColor.a = 1.0f;

				C_SCENE_MANAGER *pSceneManager = GetSceneManager();

				// シーン後処理
				pSceneManager->UninitScene();
			//	SAFE_DELETE(pCurrentScene);


				// ロード開始
				C_SCENE_LOAD *pSceneLoad = GetSceneLoad();
				pSceneLoad->EnableLoad();
				SetFade(FADE_IN);
			}

			// 色を設定
			SetColor(CurrentColor);
		}
		else if (FADE_IN == CurentFadeType)
		{
			// フェードイン処理
			CurrentColor.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせていく

			if (CurrentColor.a <= 0.0f)
			{
				// フェード処理終了
				CurrentColor.a = 0.0f;
				SetFade(FADE_NONE);
			}

			// 色を設定
			SetColor(CurrentColor);
		}
	}
	//SetTexture();
	SetVtxBoard();
	SetColor(CurrentColor);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::DrawObject()
{
	C_BOARD::DrawObject();
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 色を設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::SetColor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

	// 反射光の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	VertexBoard.pD3DVtxBuffBoard->Unlock();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェードの状態設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::SetFade(FADE_TYPE fade)
{
	CurentFadeType = fade;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェードの状態取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_FADE::FADE_TYPE C_FADE::GetFadeState()
{
	return CurentFadeType;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ワーク用シーンセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::SetWorkScene(C_SCENE_MANAGER::SCENE_STATE SetScene)
{
	WorkScene = SetScene;
}