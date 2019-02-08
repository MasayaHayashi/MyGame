//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// StarUI.cpp
// スターUI(オブジェクトではなくボード用)
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "StarUI.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"
#include "../GameManager/GameManager.h"
#include "../MyVector3/MyVector3.h"
#include <stdio.h>

// ===== 定数・マクロ定義 =====

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
StarUI::StarUI()
{
	strcpy_s(fileName, TextureFilePass.c_str());
	vertexBoard.pos					= D3DXVECTOR3(Application::ScreenCenterX * 0.5f, 0.0f, 0.0f);
	vertexBoard.rotDeg				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size				= D3DXVECTOR3(330.0f * 0.4f, 320.0f * 0.4f, 0.0f);
	vertexBoard.radAngle			= 0.0f;
	posDestBoard					= D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenCenterY * 0.25f, 0.0f);
	isAlphaBlend					= false;
	vertexBoard.boardType			= boardType::Polygon2d;

	texPatternDivideX = 1;
	texPatternDivideY = 1;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	intervalChangePattern = 1;

	curvePos[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	curvePos[1] = D3DXVECTOR3(Application::ScreenCenterX * 0.25f, Application::ScreenCenterY, 0.0f);
	curvePos[2] = D3DXVECTOR3(Application::ScreenWidth - vertexBoard.size.x, vertexBoard.size.y, 0.0f);
	curvePos[3] = D3DXVECTOR3(Application::ScreenWidth - (vertexBoard.size.x * 0.5f * idNumber + 1), vertexBoard.size.y, 0.0f);

	isUsed = false;
}

StarUI::StarUI(size_t indexNumber)
{
	strcpy_s(fileName, TextureFilePass.c_str());
	vertexBoard.pos = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, 0.0f, 0.0f);
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(330.0f * 0.2f, 320.0f * 0.2f, 0.0f);
	vertexBoard.radAngle = 0.0f;
	posDestBoard = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenCenterY * 0.25f, 0.0f);
	isAlphaBlend = false;
	idNumber = indexNumber;

	vertexBoard.boardType = boardType::Polygon2d;

	texPatternDivideX = 1;
	texPatternDivideY = 1;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	intervalChangePattern = 1;

	curvePos[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	curvePos[1] = D3DXVECTOR3(Application::ScreenCenterX * 0.25f, Application::ScreenCenterY, 0.0f);
	curvePos[2] = D3DXVECTOR3(Application::ScreenWidth - (vertexBoard.size.x * 0.5f * (idNumber + 1)) - 10.0f, vertexBoard.size.y, 0.0f);
	curvePos[3] = D3DXVECTOR3(Application::ScreenWidth - (vertexBoard.size.x * 0.5f * (idNumber + 1)) - 10.0f, vertexBoard.size.y, 0.0f);

	isUsed = false;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
StarUI::~StarUI()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StarUI::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StarUI::finalize()
{
	Board::finalize();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StarUI::update()
{
	const INT hitIndex = Collision::getHitIndex("star.x");

	if (hitIndex >= 0)
	{
		awake = true;
		isUsed = true;
	}

	if (!awake)
	{
		return;
	}

	/*
	if (hitIndex > 0)
	{
		checkUnProject(Collision::getTransform("star.x", hitIndex)->pos, 0);
		checkUnProject(Collision::getTransform("star.x", hitIndex)->pos,1);
	}

	curveCnt += 0.004f;

	if (curveCnt < 1.0f)
	{
		MyVector3::CalcBezierCurve(vertexBoard.pos, curvePos[0], curvePos[1], curvePos[2], curvePos[3], 1.0f, curveCnt);
	}
	*/

	if (hitIndex >= 0)
	{
		D3DXVECTOR3   size2D = checkUnProject(Collision::getTransform("star.x", hitIndex)->pos, hitIndex);
		curvePos[0]	= size2D - D3DXVECTOR3(vertexBoard.size.x * 2.0f, 0.0f, 0.0f);
	}

	MyVector3::CalcBezierCurve(vertexBoard.pos, curvePos[0], curvePos[1], curvePos[2], curvePos[3], 1.0f, curveCnt);

	if (curveCnt < 1.0f)
	{
		vertexBoard.radAngle += D3DXToRadian(4);
		curveCnt += 0.007f;
	}

	setVtx();
	setTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void StarUI::draw()
{
	Board::draw();
}