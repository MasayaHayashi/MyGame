//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_TitleUI.cpp
// タイトルUIクラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "TitleUI.h"
#include "../Application/Application.h"
#include <stdio.h>

// ===== 定数・マクロ定義 =====

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
TitleUI::TitleUI()
{
	strcpy_s(fileName, TextureFilePass.c_str());
	vertexBoard.pos					 = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, 0.0f, 0.0f);
	vertexBoard.rotDeg					 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale					 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size					 = D3DXVECTOR3(921.0f, 177.0f, 0.0f);
	vertexBoard.radAngle = 0.0f;
	posDestBoard				 = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenCenterY * 0.25f, 0.0f);
	isAlphaBlend			 = false;
	vertexBoard.boardType		 = boardType::Polygon2d;

	texPatternDivideX = 1;
	texPatternDivideY = 1;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	intervalChangePattern = 1;

	isUsed = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
TitleUI::~TitleUI()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void TitleUI::initialize()
{
//	ResourceManager *pResourceMgr = GetResourceManager();

	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void TitleUI::finalize()
{
	Board::finalize();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void TitleUI::update()
{
	static FLOAT cnt = 0;
	
	// ロゴ移動処理
	vertexBoard.pos.y += MoveSpeed;

	// 終了判定
	if (vertexBoard.pos.y > posDestBoard.y)
		vertexBoard.pos.y = posDestBoard.y;

	// 横移動
	cnt += 0.03f;
	vertexBoard.pos.x = sinf(cnt) * 11.0f + Application::ScreenCenterX;
	
	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void TitleUI::draw()
{
	Board::draw();
}