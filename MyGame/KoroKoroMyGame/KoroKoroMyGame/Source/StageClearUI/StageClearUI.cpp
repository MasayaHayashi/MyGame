//����������������������������������������������
// StageClearUI.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "../StageClearUI/StageClearUI.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"
#include "../DirectX3D/DirectX3D.h"
#include "../GameManager/GameManager.h"

//������������������������������������
// �R���X�g���N�^
//������������������������������������
StageClearUI::StageClearUI()
{
	strcpy_s(fileName, TextureFilePass.c_str());

	color					= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	vertexBoard.pos			= D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	vertexBoard.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size		= D3DXVECTOR3(99.0f * 0.004f, 94.0f * 0.004f, 0.0f);
	isAlphaBlend			= true;
	vertexBoard.boardType	= boardType::Billboard;
	vertexBoard.radAngle	= D3DXToRadian(0);

	texPatternDivideX		= 1;
	texPatternDivideY		= 1;
	texUV_SizeX				= 1.0f / texPatternDivideX;
	texUV_SizeY				= 1.0f / texPatternDivideY;
	animPattern				= texPatternDivideX * texPatternDivideY;
	intervalChangePattern	= 1;

	isUsed					= false;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
StageClearUI::~StageClearUI()
{

}

//����������������������������������
// ������
//����������������������������������
void StageClearUI::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void StageClearUI::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void StageClearUI::update()
{
	if (GameManager::isGameType(GameManager::GameType::Goal))
	{
		isUsed = true;
	}

}

//��������������������������������������
// �`��
//��������������������������������������
void StageClearUI::draw()
{
	Board::draw();
}