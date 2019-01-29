//����������������������������������������������
// StarParticle.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "../StarParticle/StarParticle.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Random/MyRandom.h"
#include "../GameManager/GameManager.h"

// ===== �萔�E�}�N����` =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
StarParticle::StarParticle()
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
	moveVec = D3DXVECTOR3(MyRandom::get(-1.0f,1.0f), MyRandom::get(0.3f,0.5f), 0.0f);

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
StarParticle::~StarParticle()
{

}

//����������������������������������
// ������
//����������������������������������
void StarParticle::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void StarParticle::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void StarParticle::update()
{

	if (GameManager::getGameType() == GameManager::GameType::Miss)
	{
		if (!awake)
		{
			awake  = true;
			isUsed = true;
			vertexBoard.pos = Collision::getTransform("Player", 0)->pos;
			vertexBoard.velocity = D3DXVECTOR3 (MyRandom::get(-1.0f,1.0f), MyRandom::get(0.2f, 2.0f), MyRandom::get(-1.0f, 1.0f));
		}
	}

	if (!isUsed)
	{
		return;
	}
	
	vertexBoard.pos += vertexBoard.velocity;

	/*
	if(orBelow(-6.0f))
	{
		destroy();
	}
	*/
	
}

//��������������������������������������
// �`��
//��������������������������������������
void StarParticle::draw()
{
	Board::draw();
}