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
	curvePos[3] = D3DXVECTOR3(Application::ScreenWidth - (vertexBoard.size.x * idNumber), vertexBoard.size.y, 0.0f);

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
	if (!isUsed)
	{
		return;
	}

	size_t hitIndex = Collision::getHitIndex("star.x");

	if (Collision::getTransform("star.x", 0)->isHitAABB)
	{
		checkUnProject(Collision::getTransform("star.x",hitIndex)->pos);
	}

	curveCnt += 0.004f;

	if (curveCnt < 1.0f)
	{
		MyVector3::CalcBezierCurve(vertexBoard.pos, curvePos[0], curvePos[1], curvePos[2], curvePos[3], 1.0f, curveCnt);
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