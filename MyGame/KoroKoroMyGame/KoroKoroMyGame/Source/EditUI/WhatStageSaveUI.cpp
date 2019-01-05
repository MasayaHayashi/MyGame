//������������������������������������������������
// WhatStageSaveUI.cpp
// Author : Masaya Hayashi
//������������������������������������������������

// ===== �C���N���[�h�� =====
#include "WhatStageSaveUI.h"
#include "../Application/Application.h"
#include <stdio.h>

// ===== �萔�E�}�N����` =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
WhatStageSaveUi::WhatStageSaveUi()
{
	vertexBoard.pos			= D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY, 0.0f);
	vertexBoard.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size		= D3DXVECTOR3(1024.0f, 768.0f, 0.0f);
	vertexBoard.radAngle	= 0.0f;
	posDestBoard			= D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY * 0.5f, 0.0f);
	isAlphaBlend			= false;
	vertexBoard.boardType = boardType::Polygon2d;
	strcpy_s(fileName, TexturePass.c_str());

	texPatternDivideX = 1;
	texPatternDivideY = 1;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	intervalChangePattern = 1;

	isUsed = false;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
WhatStageSaveUi::~WhatStageSaveUi()
{

}

//����������������������������������
// ������
//����������������������������������
void WhatStageSaveUi::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
	
	setVtx();
	setTexture();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void WhatStageSaveUi::finalize()
{
	ResourceManager::destroyAllMesh();
	ResourceManager::destroyAllTexture();
}

//������������������������������������
// �X�V
//������������������������������������
void WhatStageSaveUi::update()
{
	// ���ړ�
	moveCnt += 0.03f;
	vertexBoard.pos.x = sinf(moveCnt) * 11.0f + Application::ScreenCenterX;

	setVtx();
	setTexture();
}

//��������������������������������������
// �`��
//��������������������������������������
void WhatStageSaveUi::draw()
{
	Board::draw();
}