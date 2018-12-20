//����������������������������������������������
// Star.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "../Star/Star.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"

// ===== �萔�E�}�N����` =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
Star::Star()
{
	strcpy_s(fileName, TextureFilePass.c_str());

	color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	vertexBoard.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size = D3DXVECTOR3(99.0f * 0.004f, 94.0f * 0.004f, 0.0f);
	isAlphaBlend = true;
	vertexBoard.boardType = boardType::Billboard;
	vertexBoard.radAngle = D3DXToRadian(0);

	texPatternDivideX		= 1;
	texPatternDivideY		= 1;
	texUV_SizeX				= 1.0f / texPatternDivideX;
	texUV_SizeY				= 1.0f / texPatternDivideY;
	animPattern				= texPatternDivideX * texPatternDivideY;
	intervalChangePattern	= 1;

	isUsed = true;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
Star::~Star()
{

}

//����������������������������������
// ������
//����������������������������������
void Star::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
	

}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void Star::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void Star::update()
{
	if (isHit("Ball"))
	{
		vertexBoard.pos = Collision::getTransform("Ball",0)->pos;
		vertexBoard.pos.z -= 2.0f;
	}
}

//��������������������������������������
// �`��
//��������������������������������������
void Star::draw()
{
	Board::draw();
}