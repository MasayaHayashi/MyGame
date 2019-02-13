//����������������������������������������������
// DispDistance.cpp
// Autor : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "DispDistance.h"
#include "../Application/Application.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../GameManager/GameManager.h"
#include "../Collision/Collision.h"
#include "../MyVector3/MyVector3.h"
#include "../DirectX3D/DirectX3D.h"
#include <stdio.h>

//������������������������������������
// �R���X�g���N�^
//������������������������������������
DispDistance::DispDistance()
{

}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
DispDistance::~DispDistance()
{

}

//����������������������������������
// ������
//����������������������������������
void DispDistance::initialize()
{
	INT index = 0;

	for (auto & number : numberUi)
	{
		number.setSize(D3DXVECTOR3(621.0f * 0.4f, 606.0f * 0.4f, 0.0f));
		number.initialize();
		number.setNumber(index);
		number.setPosition(D3DXVECTOR3(Application::ScreenCenterX - number.getSize().x * 0.35f  * index + 1, 606.0f * 0.2f, 0.0f));
		index ++;
	}
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void DispDistance::finalize()
{
	for (auto & number : numberUi)
	{
		number.finalize();
	}
}

//������������������������������������
// �X�V
//������������������������������������
void DispDistance::update()
{
	
	D3DXVECTOR3 heartPos  = Collision::getTransform("heart.x", 0)->pos ;
	D3DXVECTOR3 playerPos = Collision::getTransform("Player", 0)->pos;
	D3DXVECTOR3 playerToHeart = heartPos - playerPos;


	FLOAT playerToHeartLength = MyVector3::getLength(playerToHeart);
	FLOAT heartLength = MyVector3::getLength(heartPos);
	
	
	FLOAT raito = playerToHeartLength / heartLength;
	raito *= 100;

	if (raito > 95.0f)
	{
		raito = 100.0f;
	}
	
	if (raito < 2.0f)
	{
		raito = 0.0f;
	}

	FLOAT distance = 100 - raito;

	FLOAT digit = 0.0f;

	for (size_t index = 0; index < numberUi.size(); index++)
	{
		digit = static_cast<INT>(distance) % 10;
		numberUi[index].changeNumber(static_cast<INT>(digit));
		distance /= 10;
	}
	
}

//��������������������������������������
// �`��
//��������������������������������������
void DispDistance::draw()
{
	for (auto & number : numberUi)
	{
		number.draw();
	}
}

//��������������������������������������
// �X�e�[�^�X������
//��������������������������������������
void DispDistance::initializeStatus()
{

}