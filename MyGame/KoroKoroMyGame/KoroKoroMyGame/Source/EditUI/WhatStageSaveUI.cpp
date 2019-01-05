//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// WhatStageSaveUI.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "WhatStageSaveUI.h"
#include "../Application/Application.h"
#include <stdio.h>

// ===== 定数・マクロ定義 =====

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
WhatStageSaveUi::WhatStageSaveUi()
{
	vertexBoard.pos			= D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY, 0.0f);
	vertexBoard.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size		= D3DXVECTOR3(1024.0f, 768.0f, 0.0f);
	vertexBoard.radAngle	= 0.0f;
	posDestBoard			= D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY * 0.5f, 0.0f);
	isAlphaBlend			= false;
	vertexBoard.boardType = boardType::Polygon2d;
	strcpy_s(fileName, TexturePass.c_str());

	texPatternDivideX = 1;
	texPatternDivideY = 1;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	intervalChangePattern = 1;

	isUsed = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
WhatStageSaveUi::~WhatStageSaveUi()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void WhatStageSaveUi::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
	
	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void WhatStageSaveUi::finalize()
{
	ResourceManager::destroyAllMesh();
	ResourceManager::destroyAllTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void WhatStageSaveUi::update()
{
	// 横移動
	moveCnt += 0.03f;
	vertexBoard.pos.x = sinf(moveCnt) * 11.0f + Application::ScreenCenterX;

	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void WhatStageSaveUi::draw()
{
	Board::draw();
}