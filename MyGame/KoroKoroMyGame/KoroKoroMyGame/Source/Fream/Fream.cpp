//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Fream.cpp
// タイトルUIクラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Fream.h"
#include "../Application/Application.h"
#include <stdio.h>	// ファイル入出力用

// ===== 定数・マクロ定義 =====

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Fream::Fream()
{
	strcpy_s(fileName, TexturePass.c_str());
	vertexBoard.pos			= D3DXVECTOR3(-30.0f, 0.0f, 20.0f);
	vertexBoard.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size		= D3DXVECTOR3(587.0f , 587.0f , 0.0f);
	vertexBoard.radAngle	= 0.0f;
	posDestBoard			= D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenCenterY * 0.25f, 0.0f);
	isAlphaBlend			= false;
	vertexBoard.boardType	= boardType::Billboard;

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
Fream::~Fream()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Fream::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Fream::finalize()
{
	Board::finalize();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Fream::update()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Fream::draw()
{
	Board::draw();
}