//����������������������������������������������
// TitleDisp.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "TitleDisp.h"
#include "../Application/Application.h"
#include <stdio.h>

// ===== �萔�E�}�N����` =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
TitleDisp::TitleDisp()
{
	strcpy_s(fileName, TextureFilePass.c_str());
	vertexBoard.pos					= D3DXVECTOR3(0.0f, Application::ScreenCenterY * 1.25f, 0.0f);
	vertexBoard.rotDeg				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size				= D3DXVECTOR3(1024.0f * 0.3f, 768.0f * 0.3f, 0.0f);
	vertexBoard.radAngle = 0.0f;
	posDestBoard				 = D3DXVECTOR3(Application::ScreenCenterX * 0.35f, Application::ScreenCenterY * 1.25f, 0.0f);
	isAlphaBlend				 = false;
	vertexBoard.boardType		 = boardType::Polygon2d;

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
TitleDisp::~TitleDisp()
{

}

//����������������������������������
// ������
//����������������������������������
void TitleDisp::initialize()
{
//	ResourceManager *pResourceMgr = GetResourceManager();

	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void TitleDisp::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void TitleDisp::update()
{
	static FLOAT cnt = 0;
	
	// ���S�ړ�����
	vertexBoard.pos.y += MoveSpeed;

	// �I������
	if (vertexBoard.pos.y > posDestBoard.y)
	{
		vertexBoard.pos.y = posDestBoard.y;
	}

	// ���ړ�
	cnt += 0.001f;

	D3DXVec3Lerp(&vertexBoard.pos, &vertexBoard.pos, &posDestBoard, cnt);

	setVtx();
	setTexture();
}

//��������������������������������������
// �`��
//��������������������������������������
void TitleDisp::draw()
{
	Board::draw();
}