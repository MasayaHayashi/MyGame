//����������������������������������������������������������������
// Figure.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Figure.h"
#include "../Mesh/Mesh.h"
#include "../DirectX3D/DirectX3D.h"
#include "../KeyBoard/Keyboard.h"
#include "../SceneManager/SceneManager.h"

//��������������������������������
// �R���X�g���N�^
//��������������������������������
Figure::Figure()
{
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxTranslate);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxScale);

	Translate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Rot		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scale	  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	cubeTexture  = nullptr;
	cubeVtx		 = nullptr;
	pD3DXBuffMat = nullptr;
	pD3DXMesh	 = nullptr;
	lNumMat		 = 0;

	Material.MatD3D.Ambient.r = 1.0f;
	Material.MatD3D.Ambient.g = 0.0f;
	Material.MatD3D.Ambient.b = 0.0f;
	Material.MatD3D.Ambient.a = 0.0f;

	Material.MatD3D.Diffuse.r = 1.0f;
	Material.MatD3D.Diffuse.g = 0.0f;
	Material.MatD3D.Diffuse.b = 0.0f;
	Material.MatD3D.Diffuse.a = 0.0f;

	Material.MatD3D.Emissive.r = 0.0f;
	Material.MatD3D.Emissive.g = 1.0f;
	Material.MatD3D.Emissive.b = 0.0f;
	Material.MatD3D.Emissive.a = 0.0f;

	Material.MatD3D.Power = 1.0f;

	Material.MatD3D.Specular.r = 0.0f;
	Material.MatD3D.Specular.g = 1.0f;
	Material.MatD3D.Specular.b = 1.0f;
	Material.MatD3D.Specular.a = 0.0f;

	isUsed = true;
}

//������������������������������������
// �R���X�g���N�^ (����: Pawn)
// �ʒu�A�T�C�Y�w��
// ���_��񐶐�
//������������������������������������
Figure::Figure(D3DXVECTOR3 SetPos, D3DXVECTOR3 SetSize)
{
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxTranslate);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxScale);

	Translate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Rot		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scale	  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	cubeVtx		= nullptr;
	cubeTexture = nullptr;
	pD3DXBuffMat = nullptr;
	pD3DXMesh	 = nullptr;
	lNumMat		 = 0;

	Material.MatD3D.Ambient.r = 1.0f;
	Material.MatD3D.Ambient.g = 0.0f;
	Material.MatD3D.Ambient.b = 0.0f;
	Material.MatD3D.Ambient.a = 0.0f;

	Material.MatD3D.Diffuse.r = 1.0f;
	Material.MatD3D.Diffuse.g = 0.0f;
	Material.MatD3D.Diffuse.b = 0.0f;
	Material.MatD3D.Diffuse.a = 0.0f;

	Material.MatD3D.Emissive.r = 0.0f;
	Material.MatD3D.Emissive.g = 1.0f;
	Material.MatD3D.Emissive.b = 0.0f;
	Material.MatD3D.Emissive.a = 0.0f;

	Material.MatD3D.Power = 1.0f;

	Material.MatD3D.Specular.r = 0.0f;
	Material.MatD3D.Specular.g = 1.0f;
	Material.MatD3D.Specular.b = 1.0f;
	Material.MatD3D.Specular.a = 0.0f;

	pBBox = nullptr;
	isUsed = true;

	// �ʒu�A�T�C�Y�Z�b�g
	Pos	  = SetPos;
	Size  = SetSize;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	color = D3DXCOLOR(DeaultBoxcolor);

	D3DXCreateBox(devicePtr, Size.x * 2.0f, Size.y * 2.0f, Size.z * 2.0f, &pBBox, nullptr);
	
}

//��������������������������������
// �f�X�g���N�^
//��������������������������������
Figure::~Figure()
{

}

//����������������������������������������
// ������
//����������������������������������������
void Figure::initializeFigure()
{
	D3DXMatrixTranslation(&mtxWorld, Pos.x, Pos.y * 2, Pos.z);

}

//����������������������������������������
// ������
//����������������������������������������
void Figure::initializeFigure(D3DXVECTOR3 SetPos, D3DXVECTOR3 SetSize,D3DXVECTOR3 SetCenterPos)
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	Pos		  = SetPos;
	Size	  = SetSize;
	CenterPos = SetCenterPos;

	color = D3DXCOLOR(DeaultBoxcolor);

	D3DXCreateBox(devicePtr, Size.x * 2.0f, Size.y * 2.0f, Size.z * 2.0f, &pBBox, nullptr);
}

//������������������������������������
// �X�V
//������������������������������������
void Figure::UpdateFigure(D3DXMATRIX SetMtx, D3DXCOLOR Setcolor)
{
	D3DXMATRIX trans;

	mtxWorld = SetMtx;
	D3DXMatrixTranslation(&trans, 0.0f, Size.y, 0.0f);
	mtxWorld *= trans;
}

//������������������������������������������
// �`��
//������������������������������������������
void Figure::DrawFigure()
{
	// ��O����
	if (!isUsed)
		return;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	
	devicePtr->SetTransform(D3DTS_WORLD, &mtxWorld);

	devicePtr->SetTexture(0, nullptr);

	ZeroMemory(&Material, sizeof(Material));

	Material.MatD3D.Diffuse = color;
	devicePtr->SetMaterial(&Material.MatD3D);

	pBBox->DrawSubset(0);
	
}

//��������������������������������
// �㏈��
//��������������������������������
void Figure::UninitFigure()
{

}

//����������������������������������������������������������������������
// �����̒��_��񐶐�
//����������������������������������������������������������������������
HRESULT Figure::MakeVtxCube(D3DXVECTOR3 Pos, D3DXVECTOR3 Size)
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	nNumVertex = 36;// * 2;	// ���_��

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVertex ,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&cubeVtx,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		nullptr)))						// nullptr�ɐݒ�
	{
		return E_FAIL;
	}

	{
		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		cubeVtx->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		
		// �O����
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);

		pVtx[3].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[4].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z - Size.z);
		pVtx[5].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);

		// �E����
		pVtx[6].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[7].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[8].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		pVtx[9].vtx  = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[10].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[11].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		// ������
		pVtx[12].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z - Size.z);
		pVtx[13].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[14].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		pVtx[15].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[16].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[17].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		// ������
		pVtx[18].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[19].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[20].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		pVtx[21].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[22].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[23].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z - Size.z);
		
		// ������
		pVtx[24].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[25].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[26].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);

		pVtx[27].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[28].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y - Size.y, Pos.z + Size.z);
		pVtx[29].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y - Size.y, Pos.z + Size.z);
		
		// �����
		pVtx[30].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[31].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[32].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z - Size.z);
		
		pVtx[33].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z + Size.z);
		pVtx[34].vtx = D3DXVECTOR3(Pos.x + Size.x, Pos.y + Size.y, Pos.z - Size.z);
		pVtx[35].vtx = D3DXVECTOR3(Pos.x - Size.x, Pos.y + Size.y, Pos.z - Size.z);
		

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		
		pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[8].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[9].nor = D3DXVECTOR3( 1.0f,  0.0f, 0.0f);
		pVtx[10].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pVtx[11].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		
		pVtx[12].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[13].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[14].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[15].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[16].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[17].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		
		pVtx[18].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[19].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[20].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[21].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[22].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[23].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		
		pVtx[24].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[25].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[26].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[27].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[28].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[29].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		
		pVtx[30].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[31].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[32].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[33].nor = D3DXVECTOR3(0.0f, -1.0f, 0.3f);
		pVtx[34].nor = D3DXVECTOR3(0.0f, -1.0f, 0.3f);
		pVtx[35].nor = D3DXVECTOR3(0.0f, -1.0f, 0.3f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);

		pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[4].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[5].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.6f, 0.8f);
		
		pVtx[6].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.8f);
		pVtx[7].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
		pVtx[8].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);

		pVtx[9].diffuse  = D3DXCOLOR(0.0f, 0.6f, 1.0f, 0.8f);
		pVtx[10].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f);
		pVtx[11].diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[12].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[13].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[14].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		pVtx[15].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[16].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[17].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[18].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[19].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[20].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[21].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[22].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[23].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[24].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[25].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[26].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		pVtx[27].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.8f);
		pVtx[28].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[29].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		
		pVtx[30].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f);
		pVtx[31].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[32].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		pVtx[33].diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.8f);
		pVtx[34].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[35].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
	}
}

void Figure::CreateTexture()
{

}

//��������������������������������������������
// �ʒu�擾
//��������������������������������������������
D3DXVECTOR3 Figure::GetPos()
{
	return D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
}

//����������������������������������������������
// �ʒu�Z�b�g
//����������������������������������������������
void Figure::SetPos(D3DXVECTOR3 Pos)
{
	mtxWorld._41 = Pos.x;
	mtxWorld._42 = Pos.y;
	mtxWorld._43 = Pos.z;
}


//��������������������������������������������
// �T�C�Y�擾
//��������������������������������������������
D3DXVECTOR3 Figure::GetSize()
{
	return Size;
}

//��������������������������������������������
// �g�p�t���O�Z�b�g�p
//��������������������������������������������
void Figure::setUsedFlg(bool bSet)
{
	isUsed = bSet;
}

//��������������������������������������������
// �}�e���A���F�Z�b�g
//��������������������������������������������
void Figure::SetMatcolor(D3DCOLOR Setcolor)
{
	color = Setcolor;
}