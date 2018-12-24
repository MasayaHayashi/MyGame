//����������������������������������������������
// BackGround.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "BackGround.h"
#include "../Application/Application.h"
#include <stdio.h>	// �t�@�C�����o�͗p

// ===== �萔�E�}�N����` =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
BackGround::BackGround()
{
	strcpy_s(fileName, TexturePass.c_str());
	vertexBoard.pos			= D3DXVECTOR3(0.0f, -90.0f, 500.0f);
	vertexBoard.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size		= D3DXVECTOR3(587.0f , 587.0f * 0.5f , 0.0f);
	vertexBoard.radAngle	= 0.0f;
	posDestBoard			= D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenCenterY * 0.25f, 0.0f);
	isAlphaBlend			= false;
	vertexBoard.boardType	= boardType::Billboard;

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
BackGround::~BackGround()
{

}

//����������������������������������
// ������
//����������������������������������
void BackGround::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void BackGround::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void BackGround::update()
{

}

//��������������������������������������
// �`��
//��������������������������������������
void BackGround::draw()
{
	Board::draw();
}