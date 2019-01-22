//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Countdown.cpp
// Autor : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Countdown.h"
#include "../Application/Application.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../GameManager/GameManager.h"
#include <stdio.h>	// ファイル入出力用

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Countdown::Countdown()
{
	vertexBoard.fade = true;
	strcpy_s(fileName, TexturePass.c_str());
	vertexBoard.pos = D3DXVECTOR3( Application::ScreenCenterX ,Application::ScreenCenterY + Application::ScreenCenterY * 0.5f, 0.0f);
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(0.2f, 0.2f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(621.0f, 606.0f, 0.0f);
	posDestBoard = D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY * 0.5f, 0.0f);
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

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Countdown::~Countdown()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Countdown::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createFadeTexture(texture, fileName);

	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Countdown::finalize()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Countdown::update()
{
	if (!isUsed)
	{
		return;
	}

	changeNumberCnt--;

	if (!(changeNumberCnt % ChangeFrame))
	{
		if (currentAnimPattern >= 0)
		{
			currentAnimPattern --;
			changeNumberCnt = ChangeFrame;
		}
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
void Countdown::draw()
{
	Board::draw();
}