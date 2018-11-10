//����������������������������������������������������������������
// C_MeshField.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef MESHFIELD_BASE_H
#define MESHFIELD_BASE_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"

class C_MESH_FIELD_BASE
{
public:
	C_MESH_FIELD_BASE();
	~C_MESH_FIELD_BASE();

	virtual	void initializeMeshField()   = 0;
	virtual	void UninitMeshField() = 0;
	virtual	void UpdateMeshField() = 0;
	virtual	void DrawMeshField();		// �t�B�[���h�`��
	void CreateTexture();				// �e�N�`������
	HRESULT MakeModel();				// X�t�@�C������n�`����
	void DestroyFieldMesh();
	void SetTexture();

protected:
	virtual HRESULT MakeVertexField();		// �t�B�[���h���_��񐶐�
	float fSizeBlockX, fSizeBlockZ;			// 1�u���b�N�̑傫��
	int nNumBlockX, nNumBlockZ;				// ���u���b�N��
	int nNumVertex;							// �����_��
	int nNumVertexIndex;					// ���C���f�b�N�X��
	int nNumPolygon;						// ���|���S����
	
	INT currentAnimPattern;
	INT animPattern;
	INT texPatternDivideX, texPatternDivideY;
	FLOAT texUV_SizeX, texUV_SizeY;

	char	fileName[256];					// ���b�V���̃t�@�C����
	char	texFileName[256];					// �e�N�X�`���̃t�@�C����

	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffField;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBuffField;	// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	LPDIRECT3DTEXTURE9	pD3DTextureField;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pD3DXMeshField;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pD3DXBuffMatField;		// �}�e���A�����ւ̃|�C���^
	DWORD				nNumMatField;			// �}�e���A�����̐�
	D3DXMATRIX			mtxWorldField;			// ���[���h�}�g���b�N�X

	D3DXVECTOR3			PosField;				// ���݂̈ʒu
	D3DXVECTOR3			MoveField;				// �|�[���ړ���
	D3DXVECTOR3			RotField;				// ���݂̌���
	D3DXVECTOR3			ScaleField;				// �g�嗦
	D3DXVECTOR3			CenterPos;				// ���S���W
	D3DXVECTOR3			collitionBox;			// �����蔻��p�{�b�N�X

	D3DXVECTOR3			MaxVtx;					// �ő咸�_�ʒu
	D3DXVECTOR3			MinVtx;					// �ŏ����_�ʒu

	BOOL				bLoadMesh;				// X�t�@�C������n�`��ǂݍ��ނ��A����Œn�`����邩
	FLOAT				fCollisionRadus;		// �����蔻��p���a

private:
	DWORD				dwNumVtx;				// ���_��
	MESH_VTX			*pVtx;					// ���_���ւ̃A�N�Z�X�p�|�C���^
};

#endif
