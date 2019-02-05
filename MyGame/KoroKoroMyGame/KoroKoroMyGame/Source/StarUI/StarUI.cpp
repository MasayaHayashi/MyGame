//����������������������������������������������
// StarUI.cpp
// �X�^�[UI
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "StarUI.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"
#include "../GameManager/GameManager.h"
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

	isUsed = true;
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
//	ResourceManager *pResourceMgr = GetResourceManager();

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

	static FLOAT cnt = 0;
	
	// ���S�ړ�����
	vertexBoard.pos.y += MoveSpeed;

	// �I������
	if (vertexBoard.pos.y > posDestBoard.y)
	{
		vertexBoard.pos.y = posDestBoard.y;
	}

	if (Collision::getTransform("star.x", 0)->isHitAABB)
	{
		//GameManager::changeGameType(GameManager::GameType::GetItem);
		vertexBoard.pos.x =- 5.0f;
	}

	// ���ړ�
	cnt += 0.03f;
//	vertexBoard.pos.x = sinf(cnt) * 11.0f + Application::ScreenCenterX;
	
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