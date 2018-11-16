//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// FadeUI.cpp
// フェードクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "FadeUI.h"
#include "../Application/Application.h"
#include "../SceneManager/SceneManager.h"
#include "../Scene/Load/SceneLoad.h"

// ===== 定数・マクロ定義 =====
#define TEXTURE_NAME		"Data/Texture/fade.png"
#define TEXTURE_NAME_STAR	"data/TEXTURE/Background.png"
#define FADE_RATE (0.02f)

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
FadeUI::FadeUI()
{
	vertexBoard.fade = true;
	CurentFadeType	= FadeType::FadeNone;
	Currentcolor	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//Currentcolor	= D3DXCOLOR(0.0f, 0.635f, 0.91f, 0.0f);
	//color = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.0f);
	strcpy_s(fileName, &TextureName.front());
	vertexBoard.size		 = D3DXVECTOR3(static_cast<FLOAT>( Application::ScreenWidth), static_cast<FLOAT>(Application::ScreenHeight), 0.0f);
	vertexBoard.pos			 = D3DXVECTOR3(static_cast<FLOAT>( Application::ScreenWidth) * 0.5f, static_cast<FLOAT>(Application::ScreenHeight) * 0.5f, 0.0f);
	vertexBoard.scale		 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.radAngle	 = D3DXToRadian(0);
	vertexBoard.boardType		= boardType::Polygon2d;
	isAlphaBlend = false;
	idNumber			 = 0;

	texPatternDivideX		= 1;
	texPatternDivideY		= 1;
	texUV_SizeX				= 1.0f / texPatternDivideX;
	texUV_SizeY				= 1.0f / texPatternDivideY;
	animPattern				= texPatternDivideX * texPatternDivideY;
	currentAnimPattern		= 1;
	intervalChangePattern	= 7;

	isUsed = true;
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
void FadeUI::initialize()
{
	strcpy_s(fileName, TEXTURE_NAME);
//	Texture.pD3DTexture = nullptr;
	//	CurentFadeType = FADE_IN;
	Currentcolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

	ResourceManager::makevertexBoard(vertexBoard,fileName);	// 頂点情報生成
	ResourceManager::createTexture(texture,fileName);		// テクスチャ生成

	texture.pD3DTexture = nullptr;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void FadeUI::finalize()
{
//	Board::finalizeObject();
	
	// 頂点情報解放
	ResourceManager::destroyFadeVtx();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェード更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void FadeUI::update()
{
	if (CurentFadeType != FadeUI::FadeType::FadeNone)
	{// フェード処理中
		if (FadeUI::FadeType::FadeOut == CurentFadeType)
		{
			// フェードアウト処理
			Currentcolor.a += FADE_RATE;		// α値を加算して画面を消していく

			if (Currentcolor.a >= 1.0f)
			{
				// フェードイン処理に切り替え
				Currentcolor.a = 1.0f;

				// シーン後処理
				SceneManager::finalize();
			//	SAFE_DELETE(pCurrentScene);

				SceneLoad::enable();
				setFade(FadeUI::FadeType::FadeIn);
			}

			// 色を設定
			setcolor(Currentcolor);
		}
		else if (FadeUI::FadeType::FadeIn == CurentFadeType)
		{
			// フェードイン処理
			Currentcolor.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせていく

			if (Currentcolor.a <= 0.0f)
			{
				// フェード処理終了
				Currentcolor.a = 0.0f;
				setFade(FadeUI::FadeType::FadeNone);
			}

			// 色を設定
			setcolor(Currentcolor);
		}
	}
	setTexture();
	setVtx();
	setcolor(Currentcolor);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void FadeUI::draw()
{
	Board::draw();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 色を設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void FadeUI::setcolor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 反射光の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	vertexBoard.pD3DVtxBuff->Unlock();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェードの状態設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void FadeUI::setFade(FadeType fade)
{
	CurentFadeType = fade;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フェードの状態取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
FadeUI::FadeType FadeUI::getFadeState()
{
	return CurentFadeType;
}

/*
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ワーク用シーンセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void FadeUI::setWorkScene(C_SCENE_MANAGER::SCENE_STATE setScene)
{
	WorkScene = setScene;
}
*/