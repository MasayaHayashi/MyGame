//����������������������������������������������������������������
// C_MainField.cpp
// ���C���t�B�[���h�N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �萔�E�}�N����` =====
#define FILE_NAME	 "data/MODEL/RockLayered_5.x"
#define TEX_FILENAME "data/TEXTURE/land.tga"

// ===== �C���N���[�h�� =====
#include "MainField.h"
#include <string>
#if _DEBUG
//#include "debugproc.h"
#endif

//��������������������������������������������������
// �R���X�g���N�^
//��������������������������������������������������
C_MAIN_FIELD::C_MAIN_FIELD()
{
#if 0
	strcpy_s(pawnFileName, FILE_NAME);
	strcpy_s(texFileName, TEX_FILENAME);
	pos = D3DXVECTOR3(0.0f, -3.0f, 1.0f);
	
	scale = D3DXVECTOR3(0.7f, 0.1f, 0.5f);

	pD3DXMeshPawn = nullptr;

	isUsed = true;
#endif
}

//������������������������������������������������
// �f�X�g���N�^
//������������������������������������������������
C_MAIN_FIELD::~C_MAIN_FIELD()
{

}

//��������������������������������������
// �t�B�[���h������
//��������������������������������������
void C_MAIN_FIELD::initializeMeshField()
{
	// X�t�@�C���̓ǂݍ���
	ResourceManager::makeModel(meshData, fileName,meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//��������������������������������������
// �t�B�[���h�㏈��
//��������������������������������������
void C_MAIN_FIELD::UninitMeshField()
{
	// ���b�V�����
	ResourceManager::destroyAllMesh();

	// �e�N�X�`�����
	ResourceManager::destroyAllTexture();
}

//��������������������������������������
// �t�B�[���h�X�V
//��������������������������������������
void C_MAIN_FIELD::UpdateMeshField()
{

}

//��������������������������������������
// �t�B�[���h�`��
//��������������������������������������
void C_MAIN_FIELD::DrawMeshField()
{
	Pawn::drawObject();
//	ResourceManager *pResourceMgr = GetResourceManager();

}

//��������������������������������������
// ���S���W�擾
//��������������������������������������
D3DXVECTOR3 C_MAIN_FIELD::GetCenterPos()
{
	return centerPos;
}

//������������������������������������������
// �~��̃t�B�[���h�����蔻��擾
//������������������������������������������
bool C_MAIN_FIELD::GetCollisionCiecle(D3DXVECTOR3 CurrentPos,float Radius)
{
	if ((CurrentPos.x - centerPos.x) * (CurrentPos.x - centerPos.x) + (CurrentPos.z - centerPos.z) * (CurrentPos.z - centerPos.z) <= ((collisionRadus - Radius ) * (collisionRadus - Radius ) ))
		return false;

	return true;
}

//������������������������������������������
// �}�e���A���̐F�Z�b�g
//������������������������������������������
void C_MAIN_FIELD::SetMaterialcolorEmissive(float fRed, float Green, float Blue, float Alpha)
{
	D3DXMATERIAL *pD3DXMat;

	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMat->GetBufferPointer();

	pD3DXMat->MatD3D.Emissive.r = fRed;
	pD3DXMat->MatD3D.Emissive.g = Green;
	pD3DXMat->MatD3D.Emissive.b = Blue;
	pD3DXMat->MatD3D.Emissive.a = Alpha;
}

//������������������������������������������
// �����蔻��p�T�C�Y�擾
//������������������������������������������
float C_MAIN_FIELD::getCollisionRadius()
{
	return collisionRadus;
}