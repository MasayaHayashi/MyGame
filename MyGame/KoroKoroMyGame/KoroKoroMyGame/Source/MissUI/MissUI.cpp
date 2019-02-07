//����������������������������������������������
// MissUI.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "MissUI.h"
#include "../Application/Application.h"
#include "../DirectX3D/DirectX3D.h"
#include "../GameManager/GameManager.h"
#include <stdio.h>

//������������������������������������
// �R���X�g���N�^
//������������������������������������
MissUI::MissUI()
{
	strcpy_s(fileName, TextureFilePass.c_str());
	vertexBoard.pos				= D3DXVECTOR3(Application::ScreenCenterX, 0.0f, 0.0f);
	vertexBoard.rotDeg			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size			= D3DXVECTOR3(269.0f, 98.0f, 0.0f);
	posDestBoard				= D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY * 1.25f, 0.0f);
	isAlphaBlend				= false;
	vertexBoard.boardType		= boardType::Polygon2d;
	vertexBoard.radAngle		= 0.0f;

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
MissUI::~MissUI()
{
	
}

//����������������������������������
// ������
//����������������������������������
void MissUI::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void MissUI::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void MissUI::update()
{

	if (GameManager::isGameType(GameManager::GameType::Miss) || 
		GameManager::isGameType(GameManager::GameType::FallMiss))
	{
		awake  = true;
		isUsed = true;
	}

	if (!awake)
	{
		return;
	}

	D3DXVec3Lerp(&vertexBoard.pos, &vertexBoard.pos, &posDestBoard, lerpCnt);
	lerpCnt += 0.001f;

	setVtx();
}

//��������������������������������������
// �`��
//��������������������������������������
void MissUI::draw()
{
	Board::draw();
}

//��������������������������������������
// �X�e�[�^�X������
//��������������������������������������
void MissUI::initializeStatus()
{
	lerpCnt = false;
	awake = false;
	isUsed = false;
}