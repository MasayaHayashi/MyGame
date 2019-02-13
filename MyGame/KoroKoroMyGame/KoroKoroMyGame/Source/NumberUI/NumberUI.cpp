//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// NumberUI.cpp
// Autor : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "NumberUI.h"
#include "../Application/Application.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../GameManager/GameManager.h"
#include <stdio.h>

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
NumberUI::NumberUI()
{
	vertexBoard.fade = false;
	strcpy_s(fileName, TexturePass.c_str());
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(0.4f, 0.4f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(621.0f, 606.0f, 0.0f);
	vertexBoard.pos = D3DXVECTOR3(Application::ScreenCenterX, vertexBoard.size.y * 0.5f, 0.0f);
	posDestBoard = D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY , 0.0f);
	isAlphaBlend = false;
	vertexBoard.boardType = boardType::Polygon2d;
	vertexBoard.radAngle = D3DXToRadian(0);

	texPatternDivideX = 5;
	texPatternDivideY = 2;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	currentAnimPattern = 3;
	intervalChangePattern = 10;

	isUsed = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
NumberUI::NumberUI(INT setNumber)
{
	number = setNumber;

	vertexBoard.fade = false;
	strcpy_s(fileName, TexturePass.c_str());
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(621.0f * 0.3f , 606.0f * 0.3f, 0.0f);
	vertexBoard.pos = D3DXVECTOR3(Application::ScreenCenterX + (number * vertexBoard.size.x  + 1), vertexBoard.size.y * 0.5f, 0.0f);
	posDestBoard = D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY, 0.0f);
	isAlphaBlend = false;
	vertexBoard.boardType = boardType::Polygon2d;
	vertexBoard.radAngle = D3DXToRadian(0);

	texPatternDivideX = 5;
	texPatternDivideY = 2;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	currentAnimPattern = 0;
	intervalChangePattern = 10;

	isUsed = true;

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
NumberUI::NumberUI(INT setNumber,FLOAT scale)
{
	number = setNumber;

	vertexBoard.fade = false;
	strcpy_s(fileName, TexturePass.c_str());
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(621.0f * scale, 606.0f * scale, 0.0f);
	vertexBoard.pos = D3DXVECTOR3(Application::ScreenCenterX + (number * vertexBoard.size.x + 1), vertexBoard.size.y * 0.5f, 0.0f);
	posDestBoard = D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY, 0.0f);
	isAlphaBlend = false;
	vertexBoard.boardType = boardType::Polygon2d;
	vertexBoard.radAngle = D3DXToRadian(0);

	texPatternDivideX = 5;
	texPatternDivideY = 2;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	currentAnimPattern = 0;
	intervalChangePattern = 10;

	isUsed = true;

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// サイズ設定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void NumberUI::setSize(D3DXVECTOR3 setSize)
{
	vertexBoard.size = setSize;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
NumberUI::~NumberUI()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void NumberUI::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createFadeTexture(texture, fileName);
	
	currentAnimPattern = 3;

	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void NumberUI::finalize()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void NumberUI::update()
{
	if (!isUsed)
	{
		return;
	}


	if (currentAnimPattern <= 0)
	{
		GameManager::changeGameType(GameManager::GameType::Playing);
		isUsed = false;
	}

	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void NumberUI::draw()
{
	Board::draw();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void NumberUI::initializeStatus()
{
	vertexBoard.pos = D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY + Application::ScreenCenterY * 0.5f, 0.0f);
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(0.2f, 0.2f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(621.0f, 606.0f, 0.0f);
	
	currentAnimPattern = 3;
	isUsed = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 数字変更
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool NumberUI::changeNumber(UINT setNumber)
{
	if (idNumber >= 10)
	{
		return false;
	}

	number = setNumber;
	currentAnimPattern = number;

	setTexture();
	setVtx();

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 位置変更
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void NumberUI::changePosition(D3DXVECTOR3 setPos)
{
	vertexBoard.pos = setPos;
}