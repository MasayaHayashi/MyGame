//������������������������������������������������
// C_StageEditUI.cpp
// �X�e�[�W�G�f�B�b�g�\�L
// Author : Masaya Hayashi
//������������������������������������������������

// ===== �C���N���[�h�� =====
#include "C_StageEditUI.h"
#include <stdio.h>	// �t�@�C�����o�͗p

// ===== �萔�E�}�N����` =====
#define TEX_FILENAME "data/TEXTURE/EDIT_MODE.png"

//������������������������������������
// �R���X�g���N�^
//������������������������������������
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

//������������������������������������
// �f�X�g���N�^
//������������������������������������
C_STAGE_EDIT_UI::~C_STAGE_EDIT_UI()
{

}

//����������������������������������
// ������
//����������������������������������
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

//����������������������������������
// �^�C�g���I������
//����������������������������������
void C_STAGE_EDIT_UI::UninitObject()
{
	C_BOARD::UninitObject();
}

//������������������������������������
// �X�V
//������������������������������������
void C_STAGE_EDIT_UI::UpdateObject()
{
	C_SCENE_MANAGER *pSceneManager = GetSceneManager();
	CurrentState = pSceneManager->GetCurrentScene();

	if (CurrentState == C_SCENE_MANAGER::SCENE_STAGE_EDIT)
	{
		// ���ړ�
		fCnt += 0.03f;
		VertexBoard.PosBoard.x = sinf(fCnt) * 11.0f + SCREEN_CENTER_X;

		SetVtxBoard();
		SetTexture();
		SetColor();
	}
	else if(CurrentState == C_SCENE_MANAGER::SCENE_TITLE)
	{
		// ���ړ�
		fCnt += 0.03f;
		VertexBoard.PosBoard.x = sinf(fCnt) * 11.0f + SCREEN_CENTER_X;

		SetVtxBoard();
		SetTexture();
		SetColor();
	}
}

//��������������������������������������
// �`��
//��������������������������������������
void C_STAGE_EDIT_UI::DrawObject()
{
	C_BOARD::DrawObject();
}