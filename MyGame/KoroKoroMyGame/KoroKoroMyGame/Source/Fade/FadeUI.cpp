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
//#define TEXTURE_NAME		"data/TEXTURE/test.png"
#define TEXTURE_NAME_STAR	"data/TEXTURE/Background.png"
#define FADE_RATE (0.02f)

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
FadeUI::FadeUI()
{
	vertexBoard.fade = true;
	CurentFadeType	= FadeType::FadeNone;
	CurrentColor	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//CurrentColor	= D3DXCOLOR(0.0f, 0.635f, 0.91f, 0.0f);
	//Color = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.0f);
	strcpy_s(fileName, &TextureName.front());
	vertexBoard.size		 = D3DXVECTOR3(Application::ScreenWidth, Application::ScreenHeight, 0.0f);
	vertexBoard.pos		 = D3DXVECTOR3(Application::ScreenWidth * 0.5f, Application::ScreenHeight * 0.5f, 0.0f);
	vertexBoard.scaleBoard		 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.radAngleBoard	 = D3DXToRadian(0);
	vertexBoard.boardType		= POLYGON_2D;
	alphaBlendBoard = false;
	number			 = 0;

	texPatternDivideX		= 1;
	texPatternDivideY		= 1;
	texUV_SizeX				= 1.0f / texPatternDivideX;
	texUV_SizeY				= 1.0f / texPatternDivideY;
	animPattern				= texPatternDivideX * texPatternDivideY;
	currentAnimPattern		= 1;
	intervalChangePattern	= 7;

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
void C_FADE::initialize()
{
	strcpy_s(szFileName, TEXTURE_NAME);
//	Texture.pD3DTexture = nullptr;
	//	CurentFadeType = FADE_IN;
	CurrentColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

	ResourceManager::makevertexBoard(vertexBoard,szFileName);	// 頂点情報生成
	ResourceManager::CreateTexture(Texture,szFileName);		// テクスチャ生成

	Texture.pD3DTexture = nullptr;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::finalizeObject()
{
//	C_BOARD::finalizeObject();
	
	// 頂点情報解放
	ResourceManager::destroyFadeVtx();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::updateObject()
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

				C_SCENE_MANAGER *pSceneManager = getSceneManager();

				// シーン後処理
				pSceneManager->finalizeScene();
			//	SAFE_DELETE(pcurrentScene);


				// ロード開始
				C_SCENE_LOAD *pSceneLoad = getSceneLoad();
				pSceneLoad->EnableLoad();
				setFade(FADE_IN);
			}

			// 色を設定
			setColor(CurrentColor);
		}
		else if (FADE_IN == CurentFadeType)
		{
			// フェードイン処理
			CurrentColor.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせていく

			if (CurrentColor.a <= 0.0f)
			{
				// フェード処理終了
				CurrentColor.a = 0.0f;
				setFade(FADE_NONE);
			}

			// 色を設定
			setColor(CurrentColor);
		}
	}
	//setTexture();
	setvtxBoard();
	setColor(CurrentColor);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::drawObject()
{
	C_BOARD::drawObject();
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 色を設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::setColor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	vertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

	// 反射光の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	vertexBoard.pD3DVtxBuffBoard->Unlock();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェードの状態設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::setFade(FADE_TYPE fade)
{
	CurentFadeType = fade;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェードの状態取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_FADE::FADE_TYPE C_FADE::getFadeState()
{
	return CurentFadeType;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ワーク用シーンセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_FADE::setWorkScene(C_SCENE_MANAGER::SCENE_STATE setScene)
{
	WorkScene = setScene;
}