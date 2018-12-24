//����������������������������������������������
// Particle.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "../Particle/Particle.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Random/MyRandom.h"

// ===== �萔�E�}�N����` =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
Particle::Particle()
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
//	moveVec = D3DXVECTOR3())

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
Particle::~Particle()
{

}

//����������������������������������
// ������
//����������������������������������
void Particle::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
	

}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void Particle::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void Particle::update()
{
	if (isHit("Ball"))
	{
		vertexBoard.pos = Collision::getTransform("Ball",0)->pos;
		
		D3DXVECTOR3 cameraToBallVec = Collision::getCameraTransform("Camera",0)->pos - vertexBoard.pos;
		D3DXVec3Normalize(&cameraToBallVec,&cameraToBallVec);
		vertexBoard.pos += cameraToBallVec * 5;
	}

}

//��������������������������������������
// �`��
//��������������������������������������
void Particle::draw()
{
	Board::draw();
}

//��������������������������������������
// ����
//��������������������������������������
void Particle::make(D3DXVECTOR3 setPos,D3DXVECTOR3 setMove)
{
	isUsed = true;

	vertexBoard.pos = setPos;
	moveVec = setMove;
}

//������������������������������������������������������
// �j��
// ���_����e�N�X�`�����͎c�����܂ܕ`�悵�Ȃ�����
//������������������������������������������������������
void Particle::destroy()
{
	isUsed = false;
	vertexBoard.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//��������������������������������������
// �ړ�
//��������������������������������������
void Particle::move()
{
	if (!isUsed)
	{
		return;
	}

	vertexBoard.pos += moveVec;
	moveVec.y		-= Gravity;
}