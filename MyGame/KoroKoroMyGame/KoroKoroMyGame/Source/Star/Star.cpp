//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Star.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "../Star/Star.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"

// ===== 定数・マクロ定義 =====

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Star::Star()
{
	strcpy_s(fileName, TextureFilePass.c_str());

	color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	vertexBoard.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(99.0f * 0.004f, 94.0f * 0.004f, 0.0f);
	isAlphaBlend = true;
	vertexBoard.boardType = boardType::Billboard;
	vertexBoard.radAngle = D3DXToRadian(0);

	texPatternDivideX		= 1;
	texPatternDivideY		= 1;
	texUV_SizeX				= 1.0f / texPatternDivideX;
	texUV_SizeY				= 1.0f / texPatternDivideY;
	animPattern				= texPatternDivideX * texPatternDivideY;
	intervalChangePattern	= 1;

	isUsed = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Star::~Star()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Star::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
	

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Star::finalize()
{
	Board::finalize();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Star::update()
{
	if (isHit("Ball"))
	{
		vertexBoard.pos = Collision::getTransform("Ball",0)->pos;
		vertexBoard.pos.z -= 2.0f;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Star::draw()
{
	Board::draw();
}