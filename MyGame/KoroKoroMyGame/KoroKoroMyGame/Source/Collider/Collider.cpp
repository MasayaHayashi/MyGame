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
	figurePtr.reset(new Figure());

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
void Collider::InitCollider()
{
	figurePtr->InitFigure();
}

//����������������������������������������������
// �R���C�_�[������
//����������������������������������������������
void Collider::InitCollider(D3DXVECTOR3 Pos,D3DXVECTOR3 CollisionSize,D3DXVECTOR3 CenterPos)
{
	figurePtr->InitFigure(Pos,CollisionSize,CenterPos);
}

//��������������������������������������������
// �R���C�_�[�㏈��
//��������������������������������������������
void Collider::UnInitCollider()
{

}

//����������������������������������������������
// �R���C�_�[�X�V
//����������������������������������������������
void Collider::UpdateCollider(D3DXMATRIX SetMtx,D3DXCOLOR SetColor)
{
	mtxWorld = SetMtx;
	figurePtr->UpdateFigure(mtxWorld,SetColor);
}

//����������������������������������
// �R���C�_�[�`��
//����������������������������������
void Collider::DrawCollider()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// �`��
	figurePtr->DrawFigure();

}

//����������������������������������������������������������������������
// �g�p�t���O�Z�b�g
//����������������������������������������������������������������������
void Collider::SetUsedFlg(bool bSet)
{
	bUsed = bSet;

	// �}�`�`��N���X�Ɏ󂯓n��
	figurePtr->SetUsedFlg(bSet);
}

//����������������������������������������������
// �ʒu�Z�b�g
//����������������������������������������������
void Collider::SetPosition(D3DXVECTOR3 SetPos)
{
	figurePtr->SetPos(SetPos);
}