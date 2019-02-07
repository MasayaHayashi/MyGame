//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// MissUI.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "MissUI.h"
#include "../Application/Application.h"
#include "../DirectX3D/DirectX3D.h"
#include "../GameManager/GameManager.h"
#include <stdio.h>

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
MissUI::MissUI()
{
	strcpy_s(fileName, TextureFilePass.c_str());
	vertexBoard.pos				= D3DXVECTOR3(Application::ScreenCenterX, 0.0f, 0.0f);
	vertexBoard.rotDeg			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size			= D3DXVECTOR3(269.0f, 98.0f, 0.0f);
	posDestBoard				= D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY * 1.25f, 0.0f);
	isAlphaBlend				= false;
	vertexBoard.boardType		= boardType::Polygon2d;
	vertexBoard.radAngle		= 0.0f;

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
MissUI::~MissUI()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void MissUI::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void MissUI::finalize()
{
	Board::finalize();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void MissUI::update()
{

	if (GameManager::isGameType(GameManager::GameType::Miss) || 
		GameManager::isGameType(GameManager::GameType::FallMiss))
	{
		awake  = true;
		isUsed = true;
	}

	if (!awake)
	{
		return;
	}

	D3DXVec3Lerp(&vertexBoard.pos, &vertexBoard.pos, &posDestBoard, lerpCnt);
	lerpCnt += 0.001f;

	setVtx();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void MissUI::draw()
{
	Board::draw();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void MissUI::initializeStatus()
{
	lerpCnt = false;
	awake = false;
	isUsed = false;
}