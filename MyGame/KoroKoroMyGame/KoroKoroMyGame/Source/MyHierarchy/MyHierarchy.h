//������������������������������������������������������������������������������
// MyHierarchy.h
// Author : MasayaHayashi
//������������������������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef MY_HIERARCHY_H
#define MY_HIERARCHY_H

// ===== �C���N���[�h�� =====
#include "d3dx9.h"

// ===== �\���̒�` =====

// �t���[���N���X
struct MYFRAME : public D3DXFRAME
{	// �ŏI�I�ȃ}�g���b�N�X���i�[���邽�߂̕ϐ���ǉ�.
	D3DXMATRIX	combinedTransformationMatrix;
};

// ���b�V���R���e�i
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{	// �e�N�X�`����ۑ����邽�߂̕ϐ���ǉ�
	LPDIRECT3DTEXTURE9*		ppTextures;
	// �����e�[�u��
	LPD3DXATTRIBUTERANGE	pAttributeTable;
	DWORD					NumAttributeGroups;
	// �ȉ��̓X�L�����b�V���p
	DWORD			dwWeight;	// �d�݂̐�
	DWORD			dwBone;		// �{�[���̐�
	LPD3DXBUFFER	pBoneBuffer;// �{�[���E�e�[�u��
	LPD3DXMATRIX*	ppBoneMatrix;// �{�[���̃��[���h�s��̐擪
	LPD3DXMATRIX	pBoneOffsetMatrix;// �t���[���Ƃ��Ẵ{�[���̃��[���h�s��
};

// ===== �N���X��` =====
class MyHierarchy final : public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(
		LPCSTR name,
		CONST D3DXMESHDATA* meshDataPtr,
		CONST D3DXMATERIAL* materialsPtr,
		CONST D3DXEFFECTINSTANCE* effectInstancesPtr,
		DWORD materials,
		CONST DWORD* adjacencyPtr,
		LPD3DXSKININFO skinInfoPtr,
		LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(LPD3DXFRAME frameToFreePtr);
	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER meshContainerBasePtr);

	MyHierarchy();		// �R���X�g���N�^
	void setDirectory(LPCTSTR pszDir);

private:
	TCHAR dir[_MAX_PATH];
};

#endif