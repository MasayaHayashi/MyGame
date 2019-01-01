//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_StageEditUI.cpp
// ステージエディット表記
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "C_StageEditUI.h"
#include <stdio.h>	// ファイル入出力用

// ===== 定数・マクロ定義 =====
#define TEX_FILENAME "data/TEXTURE/EDIT_MODE.png"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_STAGE_EDIT_UI::C_STAGE_EDIT_UI()
{
	strcpy_s(szFileName, TEX_FILENAME);
	VertexBoard.RotBoard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	VertexBoard.ScaleBoard = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	VertexBoard.SizeBoard = D3DXVECTOR3(640, 400.0f, 0.0f);
	VertexBoard.PosBoard = D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 100.0f, 0.0f);
	Color = D3DXCOLOR(0.4f, 0.5f, 1.0f, 1.0f);

	VertexBoard.fRadAngleBoard = 0.0f;
	PosDestBoard = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y * 0.5f, 0.0f);
	bAlphaBlendBoard = false;
	VertexBoard.BoardType = POLYGON_2D;

	TexPatternDivideX = 1;
	TexPatternDivideY = 1;
	TexUV_SizeX = 1.0f / TexPatternDivideX;
	TexUV_SizeY = 1.0f / TexPatternDivideY;
	NumAnimPattern = TexPatternDivideX * TexPatternDivideY;
	nIntervalChangePattern = 1;

	bUsed = true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_STAGE_EDIT_UI::~C_STAGE_EDIT_UI()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_STAGE_EDIT_UI::InitObject()
{
	C_SCENE_MANAGER *pSceneManager = GetSceneManager();
	CurrentState = pSceneManager->GetCurrentScene();



	if (CurrentState == C_SCENE_MANAGER::SCENE_STAGE_EDIT)
	{
		C_RESOURCE_MANAGER::MakeVertexBoard(VertexBoard, szFileName);
		C_RESOURCE_MANAGER::CreateTexture(Texture, szFileName);

		SetVtxBoard();
		SetTexture();
		SetColor();
	}
	else if (CurrentState == C_SCENE_MANAGER::SCENE_TITLE)
	{
		VertexBoard.PosBoard = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_HEIGHT - 100.0f, 0.0f);

		C_RESOURCE_MANAGER::MakeVertexBoard(VertexBoard, szFileName);
		C_RESOURCE_MANAGER::CreateTexture(Texture, szFileName);

		SetVtxBoard();
		SetTexture();
		SetColor();
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// タイトル終了処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_STAGE_EDIT_UI::UninitObject()
{
	C_BOARD::UninitObject();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_STAGE_EDIT_UI::UpdateObject()
{
	C_SCENE_MANAGER *pSceneManager = GetSceneManager();
	CurrentState = pSceneManager->GetCurrentScene();

	if (CurrentState == C_SCENE_MANAGER::SCENE_STAGE_EDIT)
	{
		// 横移動
		fCnt += 0.03f;
		VertexBoard.PosBoard.x = sinf(fCnt) * 11.0f + SCREEN_CENTER_X;

		SetVtxBoard();
		SetTexture();
		SetColor();
	}
	else if(CurrentState == C_SCENE_MANAGER::SCENE_TITLE)
	{
		// 横移動
		fCnt += 0.03f;
		VertexBoard.PosBoard.x = sinf(fCnt) * 11.0f + SCREEN_CENTER_X;

		SetVtxBoard();
		SetTexture();
		SetColor();
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_STAGE_EDIT_UI::DrawObject()
{
	C_BOARD::DrawObject();
}