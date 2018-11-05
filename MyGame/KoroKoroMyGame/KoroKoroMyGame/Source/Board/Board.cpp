//����������������������������������������������������������������
// Board.cpp
// 2D�|���S���@�Q�[������UI�Ȃǈꖇ�̉摜�Ɏg�������
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Board.h"
#include "../SceneManager/SceneManager.h"
#include "C_Camera.h"

//��������������������������������
// �R���X�g���N�^
//��������������������������������
Board::Board()
{
	pD3DVtxBuffBoard	= nullptr;
	pD3DTextureBoard	= nullptr;
	VertexBoard.bFade   = false;
	PosBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ScaleBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SizeBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MoveBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PosDestBoard		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	TempQuaternion		= D3DXQUATERNION(0, 0, 0, 1);
	fRadAngleBoard		= 0.0f;
	fRotSpeedBoard		= 0.0f;
	bAlphaBlendBoard	= false;
	bUVMove				= false;
	narrow				= false;
	boardType			= BoardType::Polygon2d;

	fCnt = 0.0f;

	// �e�N�X�`���֘A
	TexUV_SizeX			   = 0.0f;
	TexUV_SizeY			   = 0.0f;
	MoveUV				   = D3DXVECTOR3 (0.0f,0.0f,0.0f);
	TexPatternDivideX, TexPatternDivideY = 0;
	NumAnimPattern		   = 0;
	nIntervalChangePattern = 0;
	nAnimCnt			   = 0;
	CurrentAnimPattern	   = 1;
	Color				   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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
	bUsed = false;
}

//����������������������������������������������
// �{�[�h�㏈��
//����������������������������������������������
void Board::finalize()
{
	// ���_�����
	C_RESOURCE_MANAGER::DestroyVtx();
	C_RESOURCE_MANAGER::DestroyAllTexture();
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
const void Board::draw()
{
	// ��O����
	if (!used)
		return;
	if (!vertexBoard.pD3DVtxBuffBoard)
		return;

	// �I�u�W�F�N�g�擾
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	D3DXMATRIX mtxScale, mtxTranslate, mtxRoll,mtxRot;

	C_CAMERA *pCamera = NULL;

	// �`��
	switch (VertexBoard.BoardType)
	{
	case POLYGON_2D:

			// ���e�X�g��L����
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, true);	// �A���t�@�e�X�g�I��
			devicePtr->SetRenderState(D3DRS_ALPHAREF, 5);				// 5�ȉ��͕`�悵�Ȃ�
			devicePtr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			devicePtr->SetStreamSource(0, VertexBoard.pD3DVtxBuffBoard, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			devicePtr->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			//if(Texture.pD3DTexture)
			devicePtr->SetTexture(0, Texture.pD3DTexture);

			// �A���t�@�u�����h�ݒ�
			if (bAlphaBlendBoard)
				devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// �e�N�X�`���̐ݒ�i�t���[�������j
			//	devicePtr->SetTexture(0,pD3DTexture);

			// �|���S���̕`��i�t���[�������j
			devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// �A���t�@�u�����h�ݒ��߂�
			devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// ���e�X�g�𖳌���
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		break;
	case BILLBOARD:

			pCamera = GetSceneManager()->GetInstanse()->GetCamera();

			// ���e�X�g��L����
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			devicePtr->SetRenderState(D3DRS_ALPHAREF, 5);
			devicePtr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// ���C���e�B���O�𖳌��ɂ���
			devicePtr->SetRenderState(D3DRS_LIGHTING, FALSE);


			// �r���[�}�g���b�N�X���擾
			mtxTempView = pCamera->GetMtxView();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&WorldMtxBoard);

			// �|���S���𐳖ʂɌ�����

			// �t�s������Ƃ߂�
			D3DXMatrixInverse(&WorldMtxBoard, NULL, &mtxTempView);
			WorldMtxBoard._41 = 0.0f;
			WorldMtxBoard._42 = 0.0f;
			WorldMtxBoard._43 = 0.0f;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, VertexBoard.ScaleBoard.x, VertexBoard.ScaleBoard.y, VertexBoard.ScaleBoard.z);
			D3DXMatrixMultiply(&WorldMtxBoard, &WorldMtxBoard, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, VertexBoard.PosBoard.x, VertexBoard.PosBoard.y, VertexBoard.PosBoard.z);
			D3DXMatrixMultiply(&WorldMtxBoard, &WorldMtxBoard, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			devicePtr->SetTransform(D3DTS_WORLD, &WorldMtxBoard);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			devicePtr->SetStreamSource(0, VertexBoard.pD3DVtxBuffBoard, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			devicePtr->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			devicePtr->SetTexture(0, Texture.pD3DTexture);
	
			// �A���t�@�u�����h
			if (bAlphaBlendBoard)
				devicePtr->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			// �������������s��
			devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

			// �|���S���̕`��
			devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// ���C���e�B���O��L���ɂ���
			devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);

			// ���e�X�g�𖳌���
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// �A���t�@�u�����h�ݒ��߂�
	//		devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		break;
	case POLYGON_3D:

	//	D3DXMATRIX mtxRot, mtxTranslate;
		/*
		// ���Z����
		devicePtr->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// ���� = �]����(DEST) - �]����(SRC)
		devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// Z��r�Ȃ�
		//	devicePtr->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&WorldMtxBoard);

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
		D3DXMatrixRotationYawPitchRoll(&mtxRot, RotBoard.y, RotBoard.x, RotBoard.z);
		D3DXMatrixMultiply(&WorldMtxBoard, &WorldMtxBoard, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, PosBoard.x,
			PosBoard.y,
			PosBoard.z);
		D3DXMatrixMultiply(&WorldMtxBoard,
			&WorldMtxBoard, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		devicePtr->SetTransform(D3DTS_WORLD, &WorldMtxBoard);

		// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		devicePtr->SetStreamSource(0, VertexBoard.pD3DVtxBuffBoard, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		devicePtr->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, Texture.pD3DTexture);

		// �|���S���̕`��
		devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, NULL, NUM_POLYGON);

		/*
		// �ʏ�u�����h
		devicePtr->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
		devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// Z��r����
		devicePtr->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		*/
		break;
	default:
		break;
	}
}

//����������������������������������������������
// �����������_���̃Z�b�g
//����������������������������������������������
void Board::SetVtxBoard()
{
	switch (BoardType)
	{
	case POLYGON_2D:
		{
			// ���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D  *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�

			// ��]���S���W(�|���S���̒��S)
			D3DXVECTOR3 center = D3DXVECTOR3(VertexBoard.PosBoard.x, VertexBoard.PosBoard.y, 0.0f);

			// ����
			pVtx[0].vtx = center;
			pVtx[0].vtx.x += (-VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * cos(VertexBoard.fRadAngleBoard) - (-VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * sin(VertexBoard.fRadAngleBoard);
			pVtx[0].vtx.y += (-VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * sin(VertexBoard.fRadAngleBoard) + (-VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * cos(VertexBoard.fRadAngleBoard);

			// �E��
			pVtx[1].vtx = center;
			pVtx[1].vtx.x += (VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * cos(VertexBoard.fRadAngleBoard) - (-VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * sin(VertexBoard.fRadAngleBoard);
			pVtx[1].vtx.y += (VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * sin(VertexBoard.fRadAngleBoard) + (-VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * cos(VertexBoard.fRadAngleBoard);

			// ����
			pVtx[2].vtx = center;
			pVtx[2].vtx.x += (-VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * cos(VertexBoard.fRadAngleBoard) - (VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * sin(VertexBoard.fRadAngleBoard);
			pVtx[2].vtx.y += (-VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * sin(VertexBoard.fRadAngleBoard) + (VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * cos(VertexBoard.fRadAngleBoard);

			// �E��
			pVtx[3].vtx = center;
			pVtx[3].vtx.x += (VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * cos(VertexBoard.fRadAngleBoard) - (VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * sin(VertexBoard.fRadAngleBoard);
			pVtx[3].vtx.y += (VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * sin(VertexBoard.fRadAngleBoard) + (VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * cos(VertexBoard.fRadAngleBoard);

			// ���_�f�[�^���A�����b�N����
			VertexBoard.pD3DVtxBuffBoard->Unlock();
			break;

		}
	case BILLBOARD:
		{
			//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D * pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);

			// ���_�f�[�^���A�����b�N����
			VertexBoard.pD3DVtxBuffBoard->Unlock();
			break;
		}
	case POLYGON_3D:
		{
			//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, SizeBoard.y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);

			// ���_�f�[�^���A�����b�N����
			VertexBoard.pD3DVtxBuffBoard->Unlock();
			break;
		}
	case MAX_BOARD_TYPE:
		break;
	default:
		break;
	}
}

//��������������������������������������
// �e�N�X�`������
//��������������������������������������
bool Board::CreateTexture(TEXTURE_DATA &Texture)
{
	// ��O����
	if (!szFileName)
		return false;
	if (SizeBoard <= 0)
		return false;

	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, szFileName, &Texture.pD3DTexture))
		return true;

	return false;
}

//��������������������������������������������
// ���_��񐶐�
//��������������������������������������������
bool Board::MakeVertexBoard()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();
	
	// ���_��񐶐�
	switch (BoardType)
	{
	case POLYGON_2D:

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&pD3DVtxBuffBoard,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))										// NULL�ɐݒ�
		{
			return false;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			{
				// ���_���W�̐ݒ�
				pVtx[0].vtx = D3DXVECTOR3(VertexBoard.PosBoard.x - VertexBoard.SizeBoard.x * 0.5f, VertexBoard.PosBoard.y + VertexBoard.SizeBoard.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(VertexBoard.PosBoard.x + VertexBoard.SizeBoard.x * 0.5f, VertexBoard.PosBoard.y + VertexBoard.SizeBoard.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(VertexBoard.PosBoard.x - VertexBoard.SizeBoard.x * 0.5f, VertexBoard.PosBoard.y - VertexBoard.SizeBoard.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(VertexBoard.PosBoard.x + VertexBoard.SizeBoard.x * 0.5f, VertexBoard.PosBoard.y - VertexBoard.SizeBoard.y * 0.5f, 0.0f);

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
			pD3DVtxBuffBoard->Unlock();
		}
		break;
	case BILLBOARD:
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&pD3DVtxBuffBoard,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);

		// �@���̐ݒ�
		if (SizeBoard.z == 0.0f)
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
		VertexBoard.pD3DVtxBuffBoard->Unlock();
	}
		break;
	case POLYGON_3D:
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&VertexBoard.pD3DVtxBuffBoard,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);

		// �@���̐ݒ�
		if (SizeBoard.z == 0.0f)
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
		pD3DVtxBuffBoard->Unlock();

		break;
	default:
		break;
	}

	return true;
}

//��������������������������������������������
// ���_��񐶐� (�@�����ݒ�)
//��������������������������������������������
bool Board::MakeVertexBoard(D3DXVECTOR3 nor)
{
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuffBoard,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return false;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);
	
	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
	pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f,  SizeBoard.z);
	pVtx[2].vtx = D3DXVECTOR3( SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
	pVtx[3].vtx = D3DXVECTOR3( SizeBoard.x / 2.0f, 0.0f,  SizeBoard.z);
	
	/*
	{
	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
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
	pD3DVtxBuffBoard->Unlock();


	return true;
}

//��������������������������������������������
// ���������e�N�X�`���̃Z�b�g
//��������������������������������������������
void Board::SetTexture()
{
	switch (BoardType)
	{
	case POLYGON_2D:
			// ���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D  *pVtx;

			FLOAT fPosXLeft, fPosXRight;
			FLOAT fPosYUp, fPosYDown;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			// UV���W�̐ݒ�

			// �e���W�̌v�Z
			fPosXLeft = (FLOAT)(CurrentAnimPattern % TexPatternDivideX) * TexUV_SizeX;
			fPosXRight = fPosXLeft + TexUV_SizeX;
			fPosYUp = (FLOAT)(CurrentAnimPattern / (NumAnimPattern / TexPatternDivideY)) * TexUV_SizeY;
			fPosYDown = fPosYUp + TexUV_SizeY;

			pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

			// ���_�f�[�^���A�����b�N����
			VertexBoard.pD3DVtxBuffBoard->Unlock();
		break;
	case BILLBOARD:
	{
		// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D  *pVtx;

		FLOAT fPosXLeft, fPosXRight;
		FLOAT fPosYUp, fPosYDown;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		// �e���W�̌v�Z
		fPosXLeft = (FLOAT)(CurrentAnimPattern % TexPatternDivideX) * TexUV_SizeX;
		fPosXRight = fPosXLeft + TexUV_SizeX;
		fPosYUp = (FLOAT)(CurrentAnimPattern / (NumAnimPattern / TexPatternDivideY)) * TexUV_SizeY;
		fPosYDown = fPosYUp + TexUV_SizeY;

		pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
		pVtx[1].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
		pVtx[2].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
		pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

		// ���_�f�[�^���A�����b�N����
		VertexBoard.pD3DVtxBuffBoard->Unlock();
	}
		break;
	case POLYGON_3D:
		break;
	default:
		break;
	}

}

//����������������������������������������������
// ���_�J���[�̕ύX
//����������������������������������������������
void Board::SetColor()
{
	switch (BoardType)
	{
	case POLYGON_2D:
			// 2D�|���S���̂Ƃ�
			//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].diffuse = Color;
			pVtx[1].diffuse = Color;
			pVtx[2].diffuse = Color;
			pVtx[3].diffuse = Color;

			// ���_�f�[�^���A�����b�N����
			VertexBoard.pD3DVtxBuffBoard->Unlock();
		break;
	case BILLBOARD:
	{
		// 3D�|���S���̂Ƃ�
		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse = Color;
		pVtx[1].diffuse = Color;
		pVtx[2].diffuse = Color;
		pVtx[3].diffuse = Color;

		// ���_�f�[�^���A�����b�N����
		VertexBoard.pD3DVtxBuffBoard->Unlock();
	}
		break;
	case POLYGON_3D:
	{
		// 3D�|���S���̂Ƃ�
		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse = Color;
		pVtx[1].diffuse = Color;
		pVtx[2].diffuse = Color;
		pVtx[3].diffuse = Color;

		// ���_�f�[�^���A�����b�N����
		VertexBoard.pD3DVtxBuffBoard->Unlock();
	}
		break;
	case MAX_BOARD_TYPE:
		break;
	default:
		break;
	}
}

//��������������������������������������������
// �X�e�[�^�X������
//��������������������������������������������
void Board::InitStatus()
{
	VertexBoard.PosBoard = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	VertexBoard.RotBoard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	VertexBoard.ScaleBoard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetVtxBoard();
}

//��������������������������������������������
// �ʒu�擾
//��������������������������������������������
D3DXVECTOR3 Board::GetPosition()
{
	return PosBoard;
}

//��������������������������������������������
// �T�C�Y�擾
//��������������������������������������������
D3DXVECTOR3 Board::GetSize()
{
	return SizeBoard;
}

//��������������������������������������������
// �ԍ��Z�b�g
//��������������������������������������������
void Board::SetNumber(BYTE nSetNumber)
{
	uNumber = nSetNumber;
}

//��������������������������������������������
// �A�j���[�V�����Z�b�g
//��������������������������������������������
void Board::SetAnimation()
{
	nAnimCnt ++;

	if ((nAnimCnt % nIntervalChangePattern) == 0)
	{
		// �p�^�[���̐؂�ւ�
		CurrentAnimPattern = (CurrentAnimPattern + 1) % NumAnimPattern;

		//param->currentAnimPattern = nAnim;

		// �e�N�X�`�����W��؂�ւ�
		//SetTextureExplosion(0, CurrentAnimPattern);
		SetTexture();
		//SetColor(D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f));
		//SetVtxBoard();
	}
}

//��������������������������������������
// �k�����Ă���g�p�t���O��false��
//��������������������������������������
void Board::SetNarrowFlg(bool bSet)
{
	bNarrow = bSet;
}

//��������������������������������������
// �g�p�t���O�Z�b�g
//��������������������������������������
void Board::SetUsedFlg(bool SetFlg)
{
	bUsed = SetFlg;
}

//��������������������������������������
// �e�N�X�`�����擾
//��������������������������������������
LPDIRECT3DTEXTURE9 Board::GetTexture()
{
	return pD3DTextureBoard;
}

//����������������������������������������������
// ���_��񐶐�
//����������������������������������������������
LPDIRECT3DVERTEXBUFFER9 Board::GetVtxBuff()
{
	return pD3DVtxBuffBoard;
}

//����������������������������������������������
// �g�p�t���O�擾
//����������������������������������������������
BOOL Board::GetUsedFlg()
{
	return bUsed;
}

//����������������������������������������������
// �ʒu�Z�b�g
//����������������������������������������������
void Board::SetPosition(D3DXVECTOR3 SetPos)
{
	VertexBoard.PosBoard = SetPos;
}

//����������������������������������������������
// �ړ��ʃZ�b�g
//����������������������������������������������
void Board::SetMoveNum(D3DXVECTOR3 MoveNum)
{
	PosBoard += MoveNum;
}

//����������������������������������������������
// ���f�����
//����������������������������������������������
void Board::DestroyBoard()
{
	SAFE_RELEASE(pD3DVtxBuffBoard);	// ���_���
	SAFE_RELEASE(pD3DTextureBoard);	// �e�N�X�`�����
}

//����������������������������������������������
// �J�E���^�Z�b�g
//����������������������������������������������
void Board::SetCnt(FLOAT fSet)
{
	fCnt = fSet;
}

//����������������������������������������������
// �J�E���^�Z�b�g
//����������������������������������������������
void Board::SetCnt(INT nSet)
{
	nCnt = nSet;
}

//����������������������������������������������
// �A�j���p�^�[���Z�b�g
//����������������������������������������������
void Board::SetCurrentAnimPattern(INT nSetNum)
{
	CurrentAnimPattern = nSetNum;
}

//����������������������������������������������
// ��ԗp�J�n�ʒu�Z�b�g
//����������������������������������������������
void Board::SetStartCurvePos(D3DXVECTOR3 SetStart)
{
	CurvePos[0] = SetStart;
}

//��������������������������������������������������
// ���`��ԗp�J�E���^�擾
//��������������������������������������������������
FLOAT Board::GetCurveCnt()
{
	return fCnt;
}

//��������������������������������������������������
// �A�j���p�^�[���擾
//��������������������������������������������������
UINT Board::GetCurrentAnim()
{
	return CurrentAnimPattern;
}