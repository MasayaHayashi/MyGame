//����������������������������������������������
// StarUI.cpp
// �X�^�[UI(�I�u�W�F�N�g�ł͂Ȃ��{�[�h�p)
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "StarUI.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"
#include "../GameManager/GameManager.h"
#include "../MyVector3/MyVector3.h"
#include <stdio.h>

// ===== �萔�E�}�N����` =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
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

//������������������������������������
// �f�X�g���N�^
//������������������������������������
StarUI::~StarUI()
{

}

//����������������������������������
// ������
//����������������������������������
void StarUI::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void StarUI::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
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

//��������������������������������������
// �`��
//��������������������������������������
void StarUI::draw()
{
	Board::draw();
}