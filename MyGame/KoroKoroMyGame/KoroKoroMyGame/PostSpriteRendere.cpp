#include "PostSpriteRenderer.h"
#include "shader.h"

LPDIRECT3DVERTEXBUFFER9 SpriteRenderer::pD3DVtxBuff;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

SpriteRenderer::SpriteRenderer()
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (Vertex),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		nullptr)))										// nullptr�ɐݒ�

}
SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::draw()
{
	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D  *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(�E���)
	D3DXVECTOR3 Pos;
	Pos = pVtx[0].vtx;
	pVtx[0].vtx.x = -1.0f;
	pVtx[0].vtx.y = 1.0f;
	pVtx[0].vtx.z = 0.01f;

	Pos = pVtx[1].vtx;
	pVtx[1].vtx.x = 1.0f;
	pVtx[1].vtx.y = 1.0f;
	pVtx[1].vtx.z = 0.01f;

	Pos = pVtx[2].vtx;
	pVtx[2].vtx.x = -1.0f;
	pVtx[2].vtx.y = -1.0f;
	pVtx[2].vtx.z = 0.01f;

	Pos = pVtx[3].vtx;
	pVtx[3].vtx.x = 1.0f;
	pVtx[3].vtx.y = -1.0f;
	pVtx[3].vtx.z = 0.01f;


	// ���_�J���[�̐ݒ�(0�`255�̊�)
	D3DXCOLOR Col(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[0].diffuse = Col;
	pVtx[1].diffuse = Col;
	pVtx[2].diffuse = Col;
	pVtx[3].diffuse = Col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	pD3DVtxBuff->Unlock();

	auto pDevice = DirectX3D::getDevice();
	pDevice->SetStreamSource(0, pD3DVtxBuff, 0, sizeof(VERTEX_2D));

#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	// �e�N�X�`���̐ݒ�
	Shader::RenderStart();


	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	Shader::RenderEnd();
}