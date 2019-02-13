//������������������������������������������������
// StageEditUI.cpp
// �X�e�[�W�G�f�B�b�g�\�L
// Author : Masaya Hayashi
//������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../EditUI/StageEditUI.h"
#include "../Application/Application.h"
#include <stdio.h>	// �t�@�C�����o�͗p

// ===== �萔�E�}�N����` =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
StageEditUI::StageEditUI()
{
	strcpy_s(fileName, TexturePass.c_str());
	vertexBoard.pos = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenHeight, 0.0f);
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(640.0f, 400.0f, 0.0f);
	vertexBoard.radAngle = 0.0f;
	posDestBoard = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenCenterY * 1.35f, 0.0f);
	isAlphaBlend = false;
	vertexBoard.boardType = boardType::Polygon2d;

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
StageEditUI::~StageEditUI()
{

}

//����������������������������������
// ������
//����������������������������������
void StageEditUI::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void StageEditUI::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void StageEditUI::update()
{
	static FLOAT cnt = 0;

	// ���S�ړ�����
	vertexBoard.pos.y += 1.0f;

	// �I������
	if (vertexBoard.pos.y > posDestBoard.y)
	{
		vertexBoard.pos.y = posDestBoard.y;
	}

	// ���ړ�
	cnt += 0.03f;
	vertexBoard.pos.x = sinf(cnt) * 11.0f + Application::ScreenCenterX;

	setVtx();
	setTexture();
}

//��������������������������������������
// �`��
//��������������������������������������
void StageEditUI::draw()
{
	Board::draw();
}