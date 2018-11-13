//������������������������������������������������������������������������
// C_Collider.cpp
//
// �����F
// [Pawn]�N���X�Ɏ�������B�����蔻�����邽�߂̏������������N���X
// Unity��Collider�̃C���[�W
// 
// Author : Masaya Hayashi
//������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Collider.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Figure/Figure.h"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
Collider::Collider()
{
	Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//������������������������������������������������
// �R���X�g���N�^
// Pos  : �ʒu
// Size : �傫��
// �ʒu�A�T�C�Y�w��
//������������������������������������������������
Collider::Collider(D3DXVECTOR3 setPos ,D3DXVECTOR3 setSize)
{
	// �I�u�W�F�N�g������
	figurePtr.reset(NEW Figure());

	// �ʒu�A�T�C�Y������
	Pos   = setPos;
	Size  = setSize;
}


//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
Collider::~Collider()
{

}

//����������������������������������������������
// �R���C�_�[������
//����������������������������������������������
void Collider::initializeCollider()
{
	figurePtr->initializeFigure();
}

//����������������������������������������������
// �R���C�_�[������
//����������������������������������������������
void Collider::initializeCollider(D3DXVECTOR3 Pos,D3DXVECTOR3 CollisionSize,D3DXVECTOR3 CenterPos)
{
	figurePtr->initializeFigure(Pos,CollisionSize,CenterPos);
}

//��������������������������������������������
// �R���C�_�[�㏈��
//��������������������������������������������
void Collider::UninitializeCollider()
{

}

//����������������������������������������������
// �R���C�_�[�X�V
//����������������������������������������������
void Collider::UpdateCollider(D3DXMATRIX SetMtx,D3DXCOLOR Setcolor)
{
	mtxWorld = SetMtx;
	figurePtr->UpdateFigure(mtxWorld,Setcolor);
}

//����������������������������������
// �R���C�_�[�`��
//����������������������������������
void Collider::DrawCollider()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �`��
	figurePtr->DrawFigure();

}

//����������������������������������������������������������������������
// �g�p�t���O�Z�b�g
//����������������������������������������������������������������������
void Collider::setUsedFlg(bool bSet)
{
	isUsed = bSet;

	// �}�`�`��N���X�Ɏ󂯓n��
	figurePtr->setUsedFlg(bSet);
}

//����������������������������������������������
// �ʒu�Z�b�g
//����������������������������������������������
void Collider::setPosition(D3DXVECTOR3 SetPos)
{
	figurePtr->SetPos(SetPos);
}