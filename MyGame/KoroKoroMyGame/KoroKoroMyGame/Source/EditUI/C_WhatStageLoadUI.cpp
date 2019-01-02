//������������������������������������������������
// C_WhatStageLoadUI.cpp
// �Z�[�u�X�e�[�W�I��pUI
// Author : Masaya Hayashi
//������������������������������������������������

// ===== �C���N���[�h�� =====
#include "C_WhatStageLoadUI.h"
#include <stdio.h>	// �t�@�C�����o�͗p

// ===== �萔�E�}�N����` =====
#define TEX_FILENAME "data/TEXTURE/whatStageSave.png"

//������������������������������������
// �R���X�g���N�^
//������������������������������������
C_WHAT_STAGE_LOAD_UI::C_WHAT_STAGE_LOAD_UI()
{
	strcpy_s(szFileName, TEX_FILENAME);
	VertexBoard.PosBoard = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	VertexBoard.RotBoard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	VertexBoard.ScaleBoard = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	VertexBoard.SizeBoard = D3DXVECTOR3(1024.0f, 768.0f, 0.0f);
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

	bUsed = false;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
C_WHAT_STAGE_LOAD_UI::~C_WHAT_STAGE_LOAD_UI()
{

}

//����������������������������������
// ������
//����������������������������������
void C_WHAT_STAGE_LOAD_UI::InitObject()
{
	C_RESOURCE_MANAGER *pResourceMgr = GetResourceManager();

	pResourceMgr->MakeVertexBoard(VertexBoard, szFileName);
	pResourceMgr->CreateTexture(Texture, szFileName);

	SetVtxBoard();
	SetTexture();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void C_WHAT_STAGE_LOAD_UI::UninitObject()
{
	C_BOARD::UninitObject();
}

//������������������������������������
// �X�V
//������������������������������������
void C_WHAT_STAGE_LOAD_UI::UpdateObject()
{
	// ���ړ�
	fCnt += 0.03f;
	VertexBoard.PosBoard.x = sinf(fCnt) * 11.0f + SCREEN_CENTER_X;

	SetVtxBoard();
	SetTexture();
}

//��������������������������������������
// �`��
//��������������������������������������
void C_WHAT_STAGE_LOAD_UI::DrawObject()
{
	C_BOARD::DrawObject();
}