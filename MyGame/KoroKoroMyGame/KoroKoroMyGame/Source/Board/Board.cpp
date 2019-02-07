//����������������������������������������������������������������
// Board.cpp
// 2D�|���S���@�Q�[������UI�Ȃǈꖇ�̉摜�Ɏg�������
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Board.h"
#include "../Application/Application.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Camera/camera.h"
#include "../Collision/Collision.h"
#include "../MyVector3/MyVector3.h"

//��������������������������������
// �R���X�g���N�^
//��������������������������������
Board::Board()
{
	pD3DVtxBuff	= nullptr;
	pD3DTexture	= nullptr;
	vertexBoard.fade	= false;
	pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	size				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	posDestBoard		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tempQuaternion		= D3DXQUATERNION(0, 0, 0, 1);
	radAngle		= 0.0f;
	rotSpeed		= 0.0f;
	isAlphaBlend		= false;
	isvelocityUV			= false;
	isNarrow				= false;
	boardType			= boardType::Polygon2d;

	// �e�N�X�`���֘A
	texUV_SizeX			   = 0.0f;
	texUV_SizeY			   = 0.0f;
	velocityUV				   = D3DXVECTOR3 (0.0f,0.0f,0.0f);
	texPatternDivideX, texPatternDivideY = 0;
	animPattern		   = 0;
	intervalChangePattern = 0;
	animCnt			   = 0;
	currentAnimPattern	   = 1;
	color				   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//����������������������������������������������
// �R���X�g���N�^
//����������������������������������������������
Board::Board(size_t index)
{
	pD3DVtxBuff = nullptr;
	pD3DTexture = nullptr;
	vertexBoard.fade = false;
	pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	size		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	posDestBoard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tempQuaternion = D3DXQUATERNION(0, 0, 0, 1);
	radAngle = 0.0f;
	rotSpeed = 0.0f;
	isAlphaBlend = false;
	isvelocityUV = false;
	isNarrow = false;
	boardType = boardType::Polygon2d;

	// �e�N�X�`���֘A
	texUV_SizeX = 0.0f;
	texUV_SizeY = 0.0f;
	velocityUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	texPatternDivideX, texPatternDivideY = 0;
	animPattern = 0;
	intervalChangePattern = 0;
	animCnt = 0;
	currentAnimPattern = 1;
	color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	idNumber = index;
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
Board::~Board()
{

}

//����������������������������������������������
// �{�[�h������
//����������������������������������������������
void Board::initialize()
{
//	CreateTexture();	// �e�N�X�`������
	isUsed = false;
}

//����������������������������������������������
// �{�[�h�㏈��
//����������������������������������������������
void Board::finalize()
{
	// ���_�����
	ResourceManager::destroyVtx();
	ResourceManager::destroyAllTexture();
}

//����������������������������������������������
// �{�[�h�X�V
//����������������������������������������������
void Board::update()
{

}

//����������������������������������������������
// �{�[�h�`��
//����������������������������������������������
void Board::draw()
{
	if (!isUsed)
	{
		return;
	}
	if (!vertexBoard.pD3DVtxBuff)
	{
		return;
	}

	// �I�u�W�F�N�g�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	D3DXMATRIX mtxScale, mtxTranslate, mtxRoll,mtxRot;

	// �`��
	switch (vertexBoard.boardType)
	{
	case  boardType::Polygon2d:

			// ���e�X�g��L����
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, true);	// �A���t�@�e�X�g�I��
			devicePtr->SetRenderState(D3DRS_ALPHAREF, 5);				// 5�ȉ��͕`�悵�Ȃ�
			devicePtr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			devicePtr->SetStreamSource(0, vertexBoard.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			devicePtr->SetFVF(FVF_VERTEX_2D);

			devicePtr->SetTexture(0, texture.pD3DTexture);

			// �A���t�@�u�����h�ݒ�
			if (isAlphaBlend)
			{
				devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// �|���S���̕`��
			devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, Polygon);

			// �A���t�@�u�����h�ݒ��߂�
			devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// ���e�X�g�𖳌���
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		break;
	case boardType::Billboard:

			// ���e�X�g��L����
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			devicePtr->SetRenderState(D3DRS_ALPHAREF, 5);
			devicePtr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// ���C���e�B���O�𖳌��ɂ���
			devicePtr->SetRenderState(D3DRS_LIGHTING, FALSE);

			// �r���[�}�g���b�N�X���擾
			mtxTempView = Collision::getCameraTransform("Camera",0)->viewMtx;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&worldMtx);

			// �t�s������Ƃ߂�
			D3DXMatrixInverse(&worldMtx, nullptr, &mtxTempView);
			worldMtx._41 = 0.0f;
			worldMtx._42 = 0.0f;
			worldMtx._43 = 0.0f;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, vertexBoard.scale.x, vertexBoard.scale.y, vertexBoard.scale.z);
			D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, vertexBoard.pos.x, vertexBoard.pos.y, vertexBoard.pos.z);
			D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			devicePtr->SetStreamSource(0, vertexBoard.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			devicePtr->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			devicePtr->SetTexture(0, texture.pD3DTexture);
	
			// �A���t�@�u�����h
			if (isAlphaBlend)
				devicePtr->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			// �������������s��
			devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

			// �|���S���̕`��
			devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, Polygon);

			// ���C���e�B���O��L���ɂ���
			devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);

			// ���e�X�g�𖳌���
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		break;
	case boardType::Polygon3d:

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&worldMtx);

#if 0
		// ��]�𔽉f
		D3DXQuaternionIdentity(&quat);

		vecUpObj = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// �I�u�W�F�N�g�̏����
		vecUpField = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// �n�ʂ̖@��

														// �I�u�W�F�N�g�̏�����ƒn�ʂ̖@���̊O�ς����]�������߂�
		D3DXVec3Cross(&outVec, &vecUpObj, &vecUpField);

		// �I�u�W�F�N�g�̏�����ƒn�ʂ̖@���̂Ȃ��p�����߂�
		fDotProduct = D3DXVec3Dot(&vecUpObj, &vecUpField);
		fRot = acosf(fDotProduct / (sqrtf(vecUpObj.x * vecUpObj.x + vecUpObj.y * vecUpObj.y + vecUpObj.z * vecUpObj.z)
			* sqrtf(vecUpField.x * vecUpField.x + vecUpField.y * vecUpField.y + vecUpField.z * vecUpField.z)));

		// ��]���ƂȂ��p����N�H�[�^�j�I�������߁A��]�}�g���b�N�X���Z�o
		D3DXQuaternionRotationAxis(&quat, &outVec, fRot);
		D3DXMatrixRotationQuaternion(&mtxRot, &quat);
		D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);
#endif
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pos.x,
			pos.y,
			pos.z);
		D3DXMatrixMultiply(&worldMtx,
			&worldMtx, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

		// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		devicePtr->SetStreamSource(0, vertexBoard.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		devicePtr->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, texture.pD3DTexture);


		// �|���S���̕`��
		devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, Polygon);

		break;
	default:
		break;
	}
}

//����������������������������������������������
// �����������_���̃Z�b�g
//����������������������������������������������
void Board::setVtx()
{
	switch (boardType)
	{
	case boardType::Polygon2d:
		{
			// ���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D  *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�

			// ��]���S���W(�|���S���̒��S)
			D3DXVECTOR3 center = D3DXVECTOR3(vertexBoard.pos.x, vertexBoard.pos.y, 0.0f);

			// ����
			pVtx[0].vtx = center;
			pVtx[0].vtx.x += (-vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * cos(vertexBoard.radAngle) - (-vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * sin(vertexBoard.radAngle);
			pVtx[0].vtx.y += (-vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * sin(vertexBoard.radAngle) + (-vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * cos(vertexBoard.radAngle);

			// �E��
			pVtx[1].vtx = center;
			pVtx[1].vtx.x += (vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * cos(vertexBoard.radAngle) - (-vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * sin(vertexBoard.radAngle);
			pVtx[1].vtx.y += (vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * sin(vertexBoard.radAngle) + (-vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * cos(vertexBoard.radAngle);

			// ����
			pVtx[2].vtx = center;
			pVtx[2].vtx.x += (-vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * cos(vertexBoard.radAngle) - (vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * sin(vertexBoard.radAngle);
			pVtx[2].vtx.y += (-vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * sin(vertexBoard.radAngle) + (vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * cos(vertexBoard.radAngle);

			// �E��
			pVtx[3].vtx = center;
			pVtx[3].vtx.x += (vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * cos(vertexBoard.radAngle) - (vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * sin(vertexBoard.radAngle);
			pVtx[3].vtx.y += (vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * sin(vertexBoard.radAngle) + (vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * cos(vertexBoard.radAngle);

			// ���_�f�[�^���A�����b�N����
			vertexBoard.pD3DVtxBuff->Unlock();
			break;

		}
	case boardType::Billboard:
		{
			//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D * pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].vtx = D3DXVECTOR3(-vertexBoard.size.x, vertexBoard.size.y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(vertexBoard.size.x, vertexBoard.size.y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-vertexBoard.size.x, -vertexBoard.size.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(vertexBoard.size.x, -vertexBoard.size.y, 0.0f);

			// ���_�f�[�^���A�����b�N����
			vertexBoard.pD3DVtxBuff->Unlock();
			break;
		}
	case boardType::Polygon3d:
		{
			//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].vtx = D3DXVECTOR3(-vertexBoard.size.x, vertexBoard.size.y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(vertexBoard.size.x, size.y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-vertexBoard.size.x, -vertexBoard.size.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(vertexBoard.size.x, -vertexBoard.size.y, 0.0f);

			// ���_�f�[�^���A�����b�N����
			vertexBoard.pD3DVtxBuff->Unlock();
			break;
		}
	default:
		break;
	}
}

//��������������������������������������
// �e�N�X�`������
//��������������������������������������
bool Board::createTexture(TEXTURE_DATA &Texture)
{
	// ��O����
	if (!fileName)
		return false;
	if (size <= 0)
		return false;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, fileName, &Texture.pD3DTexture))
	{
		return true;
	}

	return false;
}

//��������������������������������������������
// ���_��񐶐�
//��������������������������������������������
bool Board::makevertexBoard()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr =  DirectX3D::getDevice();
	
	// ���_��񐶐�
	switch (boardType)
	{
	case boardType::Polygon2d:

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (Vertex),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			nullptr)))										// nullptr�ɐݒ�
		{
			return false;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			{
				// ���_���W�̐ݒ�
				pVtx[0].vtx = D3DXVECTOR3(vertexBoard.pos.x - vertexBoard.size.x * 0.5f, vertexBoard.pos.y + vertexBoard.size.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(vertexBoard.pos.x + vertexBoard.size.x * 0.5f, vertexBoard.pos.y + vertexBoard.size.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(vertexBoard.pos.x - vertexBoard.size.x * 0.5f, vertexBoard.pos.y - vertexBoard.size.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(vertexBoard.pos.x + vertexBoard.size.x * 0.5f, vertexBoard.pos.y - vertexBoard.size.y * 0.5f, 0.0f);

				// rhw�̐ݒ�
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			// ���_�f�[�^���A�����b�N����
			pD3DVtxBuff->Unlock();
		}
		break;
	case boardType::Billboard:
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&pD3DVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			nullptr)))						// nullptr�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vertexBoard.size.x, vertexBoard.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vertexBoard.size.x, vertexBoard.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vertexBoard.size.x, -vertexBoard.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vertexBoard.size.x, -vertexBoard.size.y, 0.0f);

		// �@���̐ݒ�
		if (size.z == 0.0f)
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		vertexBoard.pD3DVtxBuff->Unlock();
	}
		break;
	case boardType::Polygon3d:
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&vertexBoard.pD3DVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			nullptr)))						// nullptr�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vertexBoard.size.x, vertexBoard.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vertexBoard.size.x, vertexBoard.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vertexBoard.size.x, -vertexBoard.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vertexBoard.size.x, -vertexBoard.size.y, 0.0f);

		// �@���̐ݒ�
		if (size.z == 0.0f)
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		pD3DVtxBuff->Unlock();

		break;
	default:
		break;
	}

	return true;
}

//��������������������������������������������
// ���_��񐶐� (�@�����ݒ�)
//��������������������������������������������
bool Board::makevertexBoard(D3DXVECTOR3 nor)
{
	static constexpr INT Vertex = 4;

	LPDIRECT3DDEVICE9 devicePtr =  DirectX3D::getDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		nullptr)))						// nullptr�ɐݒ�
	{
		return false;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-size.x / 2.0f, 0.0f, -size.z);
	pVtx[1].vtx = D3DXVECTOR3(-size.x / 2.0f, 0.0f,  size.z);
	pVtx[2].vtx = D3DXVECTOR3( size.x / 2.0f, 0.0f, -size.z);
	pVtx[3].vtx = D3DXVECTOR3( size.x / 2.0f, 0.0f,  size.z);
	
	/*
	{
	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(size.x, 0.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(0.0f,size.y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(size.x,size.y, 0.0f);
	}
	*/

	// �@���̐ݒ�
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;

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

	// ���_�f�[�^���A�����b�N����
	pD3DVtxBuff->Unlock();


	return true;
}

//��������������������������������������������
// ���������e�N�X�`���̃Z�b�g
//��������������������������������������������
void Board::setTexture()
{
	switch (boardType)
	{
	case boardType::Polygon2d:
			// ���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D  *pVtx;

			FLOAT fPosXLeft, fPosXRight;
			FLOAT fPosYUp, fPosYDown;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// UV���W�̐ݒ�

			// �e���W�̌v�Z
			fPosXLeft = (FLOAT)(currentAnimPattern % texPatternDivideX) * texUV_SizeX;
			fPosXRight = fPosXLeft + texUV_SizeX;
			fPosYUp = (FLOAT)(currentAnimPattern / (animPattern / texPatternDivideY)) * texUV_SizeY;
			fPosYDown = fPosYUp + texUV_SizeY;

			pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

			// ���_�f�[�^���A�����b�N����
			vertexBoard.pD3DVtxBuff->Unlock();
		break;
	case boardType::Billboard:
	{
		// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D  *pVtx;

		FLOAT fPosXLeft, fPosXRight;
		FLOAT fPosYUp, fPosYDown;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �e���W�̌v�Z
		fPosXLeft = (FLOAT)(currentAnimPattern % texPatternDivideX) * texUV_SizeX;
		fPosXRight = fPosXLeft + texUV_SizeX;
		fPosYUp = (FLOAT)(currentAnimPattern / (animPattern / texPatternDivideY)) * texUV_SizeY;
		fPosYDown = fPosYUp + texUV_SizeY;

		pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
		pVtx[1].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
		pVtx[2].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
		pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

		// ���_�f�[�^���A�����b�N����
		vertexBoard.pD3DVtxBuff->Unlock();
	}
		break;
	case boardType::Polygon3d:
		break;
	default:
		break;
	}

}

//����������������������������������������������
// ���_�J���[�̕ύX
//����������������������������������������������
void Board::setcolor()
{
	switch (boardType)
	{
	case boardType::Polygon2d:
			// 2D�|���S���̂Ƃ�
			//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].diffuse = color;
			pVtx[1].diffuse = color;
			pVtx[2].diffuse = color;
			pVtx[3].diffuse = color;

			// ���_�f�[�^���A�����b�N����
			vertexBoard.pD3DVtxBuff->Unlock();
		break;
	case boardType::Billboard:
	{
		// 3D�|���S���̂Ƃ�
		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse = color;
		pVtx[1].diffuse = color;
		pVtx[2].diffuse = color;
		pVtx[3].diffuse = color;

		// ���_�f�[�^���A�����b�N����
		vertexBoard.pD3DVtxBuff->Unlock();
	}
		break;
	case boardType::Polygon3d:
	{
		// 3D�|���S���̂Ƃ�
		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse = color;
		pVtx[1].diffuse = color;
		pVtx[2].diffuse = color;
		pVtx[3].diffuse = color;

		// ���_�f�[�^���A�����b�N����
		vertexBoard.pD3DVtxBuff->Unlock();
	}
		break;
	default:
		break;
	}
}

//��������������������������������������������
// �X�e�[�^�X������
//��������������������������������������������
void Board::initializeStatus()
{
	vertexBoard.pos = D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY, 0.0f);
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	setVtx();
}

//��������������������������������������������
// �ʒu�擾
//��������������������������������������������
D3DXVECTOR3 Board::getPosition()
{
	return pos;
}

//��������������������������������������������
// �T�C�Y�擾
//��������������������������������������������
D3DXVECTOR3 Board::getSize()
{
	return size;
}

//��������������������������������������������
// �ԍ��Z�b�g
//��������������������������������������������
void Board::setNumber(BYTE nSetNumber)
{
	idNumber = nSetNumber;
}

//��������������������������������������������
// �A�j���[�V�����Z�b�g
//��������������������������������������������
void Board::setAnimation()
{
	animCnt ++;

	if ((animCnt % intervalChangePattern) == 0)
	{
		// �p�^�[���̐؂�ւ�
		currentAnimPattern = (currentAnimPattern + 1) % animPattern;

		//param->currentAnimPattern = nAnim;

		// �e�N�X�`�����W��؂�ւ�
		//SetTextureExplosion(0, currentAnimPattern);
		setTexture();
		//Setcolor(D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f));
		//SetvtxBoard();
	}
}

//��������������������������������������
// 
//��������������������������������������
void Board::setNarrowFlg(bool bSet)
{
	isNarrow = bSet;
}

//��������������������������������������
// �g�p�t���O�Z�b�g
//��������������������������������������
void Board::setUsedFlg(bool SetFlg)
{
	isUsed = SetFlg;
}

//��������������������������������������
// �e�N�X�`�����擾
//��������������������������������������
LPDIRECT3DTEXTURE9 Board::getTexture()
{
	return pD3DTexture;
}

//����������������������������������������������
// ���_��񐶐�
//����������������������������������������������
LPDIRECT3DVERTEXBUFFER9 Board::getVtxBuff()
{
	return pD3DVtxBuff;
}

//����������������������������������������������
// �g�p�t���O�擾
//����������������������������������������������
BOOL Board::getUsedFlg()
{
	return isUsed;
}

//����������������������������������������������
// �ʒu�Z�b�g
//����������������������������������������������
void Board::setPosition(D3DXVECTOR3 SetPos)
{
	vertexBoard.pos = SetPos;
}

//����������������������������������������������
// �ړ��ʃZ�b�g
//����������������������������������������������
void Board::setvelocity(D3DXVECTOR3 velocityNum)
{
	pos += velocityNum;
}

//����������������������������������������������
// ���f�����
//����������������������������������������������
void Board::destroyBoard()
{
//	SAFE_RELEASE(pD3DVtxBuff);	// ���_���
//	SAFE_RELEASE(pD3DTexture);	// �e�N�X�`�����
}

//����������������������������������������������
// �A�j���p�^�[���Z�b�g
//����������������������������������������������
void Board::setCurrentAnimPattern(INT nSetNum)
{
	currentAnimPattern = nSetNum;
}

//����������������������������������������������
// ��ԗp�J�n�ʒu�Z�b�g
//����������������������������������������������
void Board::setStartCurvePos(D3DXVECTOR3 SetStart,size_t index)
{
	curvePos[index] = SetStart;
}

//��������������������������������������������������
// �A�j���p�^�[���擾
//��������������������������������������������������
INT Board::getCurrentAnim()
{
	return currentAnimPattern;
}

//����������������������������������������������
// �������Ă��邩
//����������������������������������������������
bool Board::isHit(std::string keyName)
{
	const Transform* ball1Transform = Collision::getTransform(keyName, 0);
	const Transform* ball2Transform = Collision::getTransform(keyName, 1);

	D3DXVECTOR3 ballToBallVector = ball1Transform->pos - ball2Transform->pos;
	ballToBallVector.y = 0.0f;

	FLOAT length = MyVector3::getLength(ballToBallVector);

	if (length < Collision::HitLength)
	{
		return true;

	}
	else
	{
		return false;
	}
}


//������������������������������������������������
// 3D����2D�ւ̍��W�ϊ�
//������������������������������������������������
const D3DXVECTOR3 Board::checkUnProject(D3DXVECTOR3 worldPos,size_t index)
{
	D3DXMATRIX  viewMtx =		 Collision::getCameraTransform("Camera", 0)->viewMtx;
	D3DXMATRIX  projectionMtx =	 Collision::getCameraTransform("Camera", 0)->projectionMtx;

	D3DXVECTOR3 outVec;
	D3DXVECTOR3 outVec2;

	D3DXVec3TransformCoord(&outVec, &worldPos, &viewMtx);
	D3DXVec3TransformCoord(&outVec2, &outVec, &projectionMtx);

	outVec2.y *= -1;

	outVec2.x *= 0.5f;
	outVec2.y *= 0.5f;

	outVec2.x += 0.5f;
	outVec2.y += 0.5f;

	outVec2.x *= Application::ScreenWidth;
	outVec2.y *= Application::ScreenHeight;

	return outVec2;
}