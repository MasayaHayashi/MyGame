//����������������������������������������������
// C_TitleUI.cpp
// ���[�h��ʗp�A�C�R���N���X
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "MyLoadIcon.h"
#include "../Application/Application.h"
#include "../ResoueceManager/ResourceManager.h"
#include <stdio.h>	// �t�@�C�����o�͗p

// ===== �萔�E�}�N����` =====
#define TEX_FILENAME "data/TEXTURE/Loadeffect.png"

//������������������������������������
// �R���X�g���N�^
//������������������������������������
MyLoadIcon::MyLoadIcon()
{
	vertexBoard.fade = true;
	strcpy_s(fileName, TEX_FILENAME);
	vertexBoard.pos = D3DXVECTOR3( Application::ScreenCenterX,Application::ScreenCenterY, 0.0f);
	vertexBoard.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(512.0f * 0.5f, 512.0f * 0.5f, 0.0f);
	posDestBoard = D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY * 0.5f, 0.0f);
	isAlphaBlend = false;
	vertexBoard.boardType = boardType::Polygon2d;
	vertexBoard.radAngle = D3DXToRadian(0);

	texPatternDivideX = 1;
	texPatternDivideY = 1;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	intervalChangePattern = 1;

	isUsed = true;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
MyLoadIcon::~MyLoadIcon()
{

}

//����������������������������������
// ������
//����������������������������������
void MyLoadIcon::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);

	setVtx();
	setTexture();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void MyLoadIcon::finalize()
{

}

//������������������������������������
// �X�V
//������������������������������������
void MyLoadIcon::update()
{
	vertexBoard.radAngle += D3DXToRadian(2);

	setVtx();
}

//��������������������������������������
// �`��
//��������������������������������������
void MyLoadIcon::draw()
{
	Board::draw();
}