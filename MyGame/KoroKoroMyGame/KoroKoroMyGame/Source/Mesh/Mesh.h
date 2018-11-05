//����������������������������������������������
// Mesh.h
// Author : MasayaHayashi
//����������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef MESH_H
#define MESH_H

// ===== �C���N���[�h�� =====
#include "d3dx9.h"

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

// ���_���p�\����
typedef struct
{
	D3DXVECTOR3	VtxPos;	// ���W
	D3DXVECTOR3	Normal;	// �@��
	float		u, v;

} MESH_VTX;


#endif // !MESH_H
