//����������������������������������������������������������������
// Collider.h
//
// �����F
// [Pawn]�N���X�Ɏ�������B�����蔻�����邽�߂̏������������N���X
// Unity��Collider�̃C���[�W
//
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef COLLIDER_H
#define COLLIDER_H

// ===== �C���N���[�h�� =====
#include "d3dx9.h"
#include <memory>

// ===== �N���X�̑O���錾 =====
class Figure;

// ===== �N���X��` =====
class Collider
{
public:
	Collider();				// �R���X�g���N�^
	Collider(D3DXVECTOR3 , D3DXVECTOR3);
	~Collider();				// �f�X�g���N�^

	void initializeCollider();
	void initializeCollider(D3DXVECTOR3 Pos, D3DXVECTOR3 CollisionSize, D3DXVECTOR3 CenterPos);
	void UninitializeCollider();
	void UpdateCollider(D3DXMATRIX,D3DXCOLOR);
	void DrawCollider();
	
	void setUsedFlg(bool);
	void setPosition(D3DXVECTOR3 SetPos);
	std::unique_ptr <Figure> figurePtr = nullptr;


private:


	bool							isUsed;				// �g�p�t���O
	D3DXVECTOR3						Pos;				// �ʒu
	D3DXVECTOR3						Size;				// �傫��
	INT								nNumVertex;			// �����_��
	INT								nNumVertexIndex;	// ���C���f�b�N�X��
	INT								nNumPolygon;		// ���|���S����
	LPDIRECT3DVERTEXBUFFER9			pCubeVtx;			// �����̒��_
	LPDIRECT3DTEXTURE9				pCubeTexture;		// �e�N�X�`��

	D3DXMATRIX						mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX						mtxTranslate;
	D3DXMATRIX						mtxRot;
	D3DXMATRIX						mtxScale;

	D3DXVECTOR3						Translate;
	D3DXVECTOR3						Rot;
	D3DXVECTOR3						Scale;

};

#endif