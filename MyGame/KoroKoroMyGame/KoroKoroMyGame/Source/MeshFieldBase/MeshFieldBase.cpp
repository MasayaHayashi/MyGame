//����������������������������������������������������������������
// C_MeshField.cpp
// ���b�V���t�B�[���h�N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "MeshFieldBase.h"
#include "../DirectX3D/DirectX3D.h"

#define BLOCK_SPACE (0.0f)

D3DMATERIAL9 test;

#define SAFE_DELETE(p)       { if(p!=nullptr) { delete (p);     (p) = nullptr; } }
#define SAFE_RELEASE(p)      { if(p!=nullptr) { (p)->Release(); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; }}

//��������������������������������
// �R���X�g���N�^
//��������������������������������
C_MESH_FIELD_BASE::C_MESH_FIELD_BASE()
{
	PosField   = D3DXVECTOR3(0.0f,0.0f,0.0f);
	MoveField  = D3DXVECTOR3(0.0f,0.0f,0.0f);
	RotField   = D3DXVECTOR3(0.0f,0.0f,0.0f);
	ScaleField = D3DXVECTOR3(1.0f,1.0f,1.0f);

	pD3DTextureField  = nullptr;		// �e�N�X�`���ւ̃|�C���^
	pD3DXMeshField    = nullptr;		// ���b�V�����ւ̃|�C���^
	pD3DXBuffMatField = nullptr;		// �}�e���A�����ւ̃|�C���^
	pD3DIdxBuffField  = nullptr;
	pVtx			  = nullptr;
	bLoadMesh		  = false;

	texUV_SizeX			= 0.0f;
	currentAnimPattern  = 0;
	animPattern		= 0;
	texPatternDivideX	= 0;
	texPatternDivideY	= 0;
	texUV_SizeX			= 0;
	texUV_SizeY			= 0;


	test.Ambient.r = 1.0f;
	test.Ambient.g = 0.0f;
	test.Ambient.b = 0.0f;
	test.Ambient.a = 0.6f;
	
	test.Diffuse.r = 1.0f;
	test.Diffuse.g = 0.0f;
	test.Diffuse.b = 0.0f;
	test.Diffuse.a = 0.6f;
	
	test.Emissive.r = 1.0f;
	test.Emissive.g = 0.0f;
	test.Emissive.b = 0.0f;
	test.Emissive.a = 0.6f;
	
	test.Power = 0.5f;

	test.Specular.r = 1.0f;
	test.Specular.g = 0.0f;
	test.Specular.b = 0.0f;
	test.Specular.a = 1.0f;
}

//��������������������������������
// �f�X�g���N�^
//��������������������������������
C_MESH_FIELD_BASE::~C_MESH_FIELD_BASE()
{

}

//������������������������������������������
// �e�N�X�`���A�}�e���A���Ȃǂ̉������
//������������������������������������������
void C_MESH_FIELD_BASE::DestroyFieldMesh()
{
	// �e�N�X�`���A���b�V�����
	SAFE_RELEASE(pD3DTextureField);
	SAFE_RELEASE(pD3DXMeshField);
	SAFE_RELEASE(pD3DXBuffMatField);

	// ���_�����
	SAFE_DELETE(pVtx);
}

//������������������������������������
// X�t�@�C�����烂�f������
//������������������������������������
HRESULT C_MESH_FIELD_BASE::MakeModel()
{
	// ��O����
	if (!fileName)
		return E_FAIL;
	if (pD3DXMeshField)
		return E_FAIL;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM, devicePtr, nullptr, &pD3DXBuffMatField, nullptr, &nNumMatField, &pD3DXMeshField)))
	{
		return E_FAIL;
	}
	
	// �e��t���O�X�V
	bLoadMesh = true;

	// ���_���T�C�Y���擾
	dwNumVtx = pD3DXMeshField->GetNumVertices();	// ���_���擾
	pVtx = NEW MESH_VTX[dwNumVtx];
	LPVOID pVtxWork;

	// ���_�o�b�t�@���b�N
	pD3DXMeshField->LockVertexBuffer(D3DLOCK_READONLY, &pVtxWork);
	CopyMemory(pVtx, pVtxWork, sizeof(MESH_VTX) * dwNumVtx);

	// ���_�o�b�t�@�A�����b�N
//	pD3DXMeshField->UnlockVertexBuffer();

	// ���_�ő�T�C�Y�A�ŏ��T�C�Y�擾
	MaxVtx = pVtx[0].VtxPos;
	MinVtx = pVtx[0].VtxPos;
	for (WORD i = 1; i < dwNumVtx; i++)
	{
		// �ő�l�擾
		if(MaxVtx.x < pVtx[i].VtxPos.x)
			MaxVtx.x = pVtx[i].VtxPos.x;

		if (MaxVtx.y < pVtx[i].VtxPos.y)
			MaxVtx.y = pVtx[i].VtxPos.y;

		if (MaxVtx.z < pVtx[i].VtxPos.z)
			MaxVtx.z = pVtx[i].VtxPos.z;

		// �ŏ��l�擾
		if (MinVtx.x > pVtx[i].VtxPos.x)
			MinVtx.x = pVtx[i].VtxPos.x;

		if (MinVtx.y > pVtx[i].VtxPos.y)
			MinVtx.y = pVtx[i].VtxPos.y;

		if (MinVtx.z > pVtx[i].VtxPos.z)
			MinVtx.z = pVtx[i].VtxPos.z;
	}

	// �ő�A�ŏ��l���X�P�[���ɍ��킹��
//	MaxVtx.x *= ScaleField.x;
//	MaxVtx.y *= ScaleField.y;
//	MaxVtx.z *= ScaleField.z;

//	MinVtx.x *= ScaleField.x;
//	MinVtx.y *= ScaleField.y;
//	MinVtx.z *= ScaleField.z;

	// ���S���W�A�����蔻��擾
	CenterPos	 = (MaxVtx + MinVtx) * 0.5f;
	collitionBox = (MaxVtx - MinVtx) * 0.5f;
	
	// ���E���擾
	float dx = pVtx[0].VtxPos.x * ScaleField.x - CenterPos.x ;
	float dy = pVtx[0].VtxPos.y * ScaleField.y - CenterPos.y ;
	float dz = pVtx[0].VtxPos.z * ScaleField.z - CenterPos.z ;
	fCollisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < dwNumVtx; ++i)
	{
		dx = pVtx[i].VtxPos.x * ScaleField.x - CenterPos.x ;
		dy = pVtx[i].VtxPos.y * ScaleField.y - CenterPos.y ;
		dz = pVtx[i].VtxPos.z * ScaleField.z - CenterPos.z ;
		float fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (fCollisionRadus < fRadius)
			fCollisionRadus = fRadius;
	}

	CenterPos += PosField * 2;	// ���S���W�����[���h���W�n��

	return S_OK;
}

//��������������������������������������������������
// �t�B�[���h�̒��_��񐶐�
//��������������������������������������������������
HRESULT C_MESH_FIELD_BASE::MakeVertexField()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �u���b�N��
	nNumBlockX = nNumBlockX;
	nNumBlockZ = nNumBlockZ;

	// ���_��(�k�ރ|���S���p���l������)
	nNumVertex = nNumBlockX * nNumBlockZ * 4;

	// �C���f�b�N�X���̐ݒ�(�k�ރ|���S���p���l������)
	nNumVertexIndex = nNumBlockX * nNumBlockZ * (4 + 4);

	// �|���S����(�k�ރ|���S���p���l������)
	nNumPolygon = nNumBlockX * nNumBlockZ * (2 + 6);

	// �u���b�N�T�C�Y
	fSizeBlockX = fSizeBlockX;
	fSizeBlockZ = fSizeBlockZ;
	float halfSizeX = fSizeBlockX * 0.5f;
	float halfSizeZ = fSizeBlockZ * 0.5f;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuffField,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		nullptr)))						// nullptr�ɐݒ�
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if (FAILED(devicePtr->CreateIndexBuffer(sizeof(WORD) * nNumVertexIndex,	// �C���f�b�N�X�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,													// �C���f�b�N�X�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,														// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,													// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DIdxBuffField,													// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		nullptr)))																// nullptr�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
#if 0
		const float fSizeTexX = 1.0f / g_nNumBlockX;
		const float fSizeTexZ = 1.0f / g_nNumBlockZ;
#else
		const float fSizeTexX = 1.0f;
		const float fSizeTexZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < nNumBlockZ; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < nNumBlockX; nCntVtxX++, pVtx += 4)
			{
				D3DXVECTOR3 base = D3DXVECTOR3(-(nNumBlockX / 2.0f) * fSizeBlockX + nCntVtxX * (fSizeBlockX ) + halfSizeX,
								   0.0f,
								   (nNumBlockZ / 2.0f) * fSizeBlockZ - nCntVtxZ * (fSizeBlockZ ) - halfSizeZ);

				pVtx[0].vtx = D3DXVECTOR3(base.x - halfSizeX, 0.0f, base.z - halfSizeZ);	// ����O
				pVtx[1].vtx = D3DXVECTOR3(base.x - halfSizeX, 0.0f, base.z + halfSizeZ);	// ����
				pVtx[2].vtx = D3DXVECTOR3(base.x + halfSizeX, 0.0f, base.z - halfSizeZ);	// �E��O
				pVtx[3].vtx = D3DXVECTOR3(base.x + halfSizeX, 0.0f, base.z + halfSizeZ);	// �E��

				// �@���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);
				pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);
				pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);
				pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

			}
		}

		// ���_�f�[�^���A�����b�N����
		pD3DVtxBuffField->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		int nBlockNo = 0;

		for (int nCntVtxZ = 0; nCntVtxZ < nNumBlockZ; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < nNumBlockX; nCntVtxX++)
			{
				// �u���b�N����
				pIdx[nCntIdx] = nBlockNo;
				nCntIdx++;
				pIdx[nCntIdx] = nBlockNo + 1;
				nCntIdx++;
				pIdx[nCntIdx] = nBlockNo + 2;
				nCntIdx++;
				pIdx[nCntIdx] = nBlockNo + 3;
				nCntIdx++;

				// �k�ރ|���S������
				pIdx[nCntIdx] = pIdx[nCntIdx - 1];	// �Ō�̒��_
				nCntIdx++;

				nBlockNo += 4;

				pIdx[nCntIdx] = nBlockNo + 1;	// ���̎l�p�̍���
				nCntIdx++;

				pIdx[nCntIdx] = nBlockNo + 1;	// ���̎l�p�̍���2
				nCntIdx++;

				pIdx[nCntIdx] = nBlockNo;		// ���̎l�p�̍���O1
				nCntIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^�͈̔͂��A�����b�N����
		pD3DIdxBuffField->Unlock();
	}

	return S_OK;
}

//��������������������������������������������
// �t�B�[���h�`��
//��������������������������������������������
void C_MESH_FIELD_BASE::DrawMeshField()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	if (!bLoadMesh)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorldField);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, RotField.y, RotField.x, RotField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, PosField.x, PosField.y, PosField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

		// �k���𔽉f
		D3DXMatrixTranslation(&mtxTranslate, ScaleField.x, ScaleField.y, ScaleField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		devicePtr->SetTransform(D3DTS_WORLD, &mtxWorldField);

		// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		devicePtr->SetStreamSource(0, pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		devicePtr->SetIndices(pD3DIdxBuffField);

		// ���_�t�H�[�}�b�g�̐ݒ�
		devicePtr->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, pD3DTextureField);

		// �|���S���̕`��
		devicePtr->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nNumVertex, 0, nNumPolygon);
	}
	else
	{
		/*
		LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
		D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
		D3DXMATERIAL *pD3DXMat;
		D3DMATERIAL9 matDef;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorldField);

		// �g��k��
		D3DXMatrixScaling(&mtxScale, ScaleField.x, ScaleField.y, ScaleField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxScale);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, RotField.y, RotField.x, RotField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, PosField.x, PosField.y, PosField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		devicePtr->SetTransform(D3DTS_WORLD, &mtxWorldField);

		// ���݂̃}�e���A�����擾
		devicePtr->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMatField->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < static_cast<INT>nNumMatField; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			devicePtr->SetTexture(0, pD3DTextureField);

			// �`��
			pD3DXMeshField->DrawSubset(nCntMat);
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		devicePtr->SetMaterial(&matDef);
		*/
		LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
		D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
		D3DXMATERIAL *pD3DXMat;
		D3DMATERIAL9 matDef;

		// ���C�e�B���O���Ȃ�
		devicePtr->SetRenderState(D3DRS_LIGHTING, FALSE);

		// Z�o�b�t�@�X�V�𖳌�
		devicePtr->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorldField);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScale, ScaleField.x, ScaleField.y, ScaleField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxScale);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, PosField.x, PosField.y, PosField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		devicePtr->SetTransform(D3DTS_WORLD, &mtxWorldField);
		
		// ���݂̃}�e���A�����擾
		devicePtr->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMatField->GetBufferPointer();

		for (INT nCntMat = 0; nCntMat < static_cast<INT>(nNumMatField); nCntMat++)
		{
			// �}�e���A���̐ݒ�
			devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			devicePtr->SetTexture(0, pD3DTextureField);

			// �`��
			pD3DXMeshField->DrawSubset(nCntMat);
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		devicePtr->SetMaterial(&matDef);

		// ���ɖ߂�
		devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);
		devicePtr->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

}

//��������������������������������������������
// �e�N�X�`������
//��������������������������������������������
void C_MESH_FIELD_BASE::CreateTexture()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(devicePtr,					// �f�o�C�X�ւ̃|�C���^
		texFileName,			// �t�@�C���̖��O
		&pD3DTextureField);	// �ǂݍ��ރ������[
}

void C_MESH_FIELD_BASE::SetTexture()
{		// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D  *pVtx;

	float fPosXLeft, fPosXRight;
	float fPosYUp, fPosYDown;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	// �e���W�̌v�Z
	fPosXLeft = (float)(currentAnimPattern % texPatternDivideX) * texUV_SizeX;
	fPosXRight = fPosXLeft + texUV_SizeX;
	fPosYUp = (float)(currentAnimPattern / (animPattern / texPatternDivideY)) * texUV_SizeY;
	fPosYDown = fPosYUp + texUV_SizeY;

	pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
	pVtx[1].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
	pVtx[2].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
	pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

	// ���_�f�[�^���A�����b�N����
	pD3DVtxBuffField->Unlock();
}