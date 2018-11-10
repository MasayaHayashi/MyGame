//��������������������������������������������������������
// Camera.cpp
// �J�����N���X
// Author : MasayaHayashi
//��������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Camera.h"
#include "d3dx9.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../KeyBoard/Keyboard.h"
#include "../Player/Player.h"
#include "../Board/Board.h"
#include "../Application/Application.h"

/*
#include "C_Block.h"
#include "C_Player.h"
#include "C_Ready.h"
#include "C_Xinput.h"

#if _DEBUG
#include "debugproc.h"
#endif
*/

// ===== �萔�E�}�N����` =====
#define MAX_PITCH_SPEED		(25.0f)	// �s�b�`�̍ő�l
#define ACCELE_PITCH_SPEED	(0.5f)	// 1�t���[���ɉ�������s�b�`�̒l
#define DECELE_PITCH_SPEED	(0.25f)	// 1�t���[���Ɍ�������s�b�`�̒l

//��������������������������������
// �R���X�g���N�^
//��������������������������������
Camera::Camera()
{
	rotMovecamera  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	movecameraDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	numMovePitch	= 0.0f;
	rotPitchRadian = 0.0f;
	cameraUpDest	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	currentState	= cameraState::Type1Person;

	cameraFowerd	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	changecamera	= true;
	lerpCnt		= 0.0f;

	cameraMoveFade = MAIN_START_FADE;

	rot = 0;
	rotCnt = 0;
}

//��������������������������������
// �f�X�g���N�^
//��������������������������������
Camera::~Camera()
{

}

//����������������������������������������
// �J����������
//����������������������������������������
void Camera::finalize(Player *pPlayer)
{

	// ���݂̃V�[���擾
	currentScene = SceneManager::getCurrentSceneType();


	// �V�[���ɉ����ĕ���
	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		initializeTitle(pPlayer);
		break;
	case SceneManager::SceneState::SceneMain:
		initializeGameMain(pPlayer);
		break;
	case SceneManager::SceneState::SceneResult:
		break;
	case SceneManager::SceneState::SceneStageEdit:
		initializeStageEdit();
		break;
	default:
		break;
	}
}

//����������������������������������������
// �J����������
//����������������������������������������
void Camera::initialize()
{

	cameraPos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// �J�����̎��_
	cameraLook  = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �J�����̒����_
	cameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);									// �J�����̏����
	
	cameraPosDest = D3DXVECTOR3(0.0f, 0.0f, -9000.0f);								// �J�����̎��_�̖ړI�ʒu
//	cameraLookDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 1.7f, PlayerPos.z);		// �J�����̒����_�̖ړI�ʒu

																					//	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
																					//	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

																					//	ForwardVec *= 5.0f;
																					//	cameraPosDest -= ForwardVec;

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = cameraPosDest.x - cameraLook.x;
	fVecZ = cameraPosDest.z - cameraLook.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

	heightcameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
	heightcameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

										// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);



}

//����������������������������������������
// �J�����������i�^�C�g���j
//����������������������������������������
void Camera::initializeTitle(Player *pPlayer)
{
	// �v���C���[���擾
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

	cameraPos  = D3DXVECTOR3(0.0f, 5.0f, -19.0f);							// �J�����̎��_
	cameraLook = D3DXVECTOR3(PlayerPos.x, PlayerPos.y -0.7f, PlayerPos.z);	// �J�����̒����_
	cameraUp   = pPlayer->getUpVec();										// �J�����̏����

	cameraPosDest  = D3DXVECTOR3(0.0f, 0.0f, -9.0f);								// �J�����̎��_�̖ړI�ʒu
	cameraLookDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 1.7f, PlayerPos.z);		// �J�����̒����_�̖ړI�ʒu

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = cameraPosDest.x - cameraLook.x;
	fVecZ = cameraPosDest.z - cameraLook.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

	heightcameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
	heightcameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

	// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);



}

//����������������������������������������
// �J����������(�X�e�[�W�G�f�B�b�g)
//����������������������������������������
void Camera::initializeStageEdit()
{
	// �v���C���[���擾

	cameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);							// �J�����̎��_
	cameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̒����_
	cameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								// �J�����̏����

	cameraPosDest = D3DXVECTOR3(0.0f, 0.0f, -9.0f);								// �J�����̎��_�̖ړI�ʒu
	cameraLookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);		// �J�����̒����_�̖ړI�ʒu

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = cameraPosDest.x - cameraLook.x;
	fVecZ = cameraPosDest.z - cameraLook.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

	heightcameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
	heightcameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

	// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);

}

//����������������������������������������
// �J����������(�Q�[�����C��)
//����������������������������������������
void Camera::initializeGameMain(Player *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getPosition();

	fadePos[2]  = PlayerPos + D3DXVECTOR3(10.0f, PlayerPos.y  + 4.3f, 5.0f);
	fadePos[1]  = PlayerPos + D3DXVECTOR3(-4.0f, PlayerPos.y  + 4.3f, 5.0f);
	fadePos[0]  = D3DXVECTOR3(0.0f, 10.0f, -19.0f);


	// �v���C���[���擾
	cameraPos	= D3DXVECTOR3(0.0f, 10.0f, -19.0f);			// �J�����̎��_
	cameraLook	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̒����_
	cameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �J�����̏����

	cameraPosDest	= D3DXVECTOR3(0.0f, 0.0f, -9.0f);		// �J�����̎��_�̖ړI�ʒu
	cameraLookDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �J�����̒����_�̖ړI�ʒu


	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = cameraPosDest.x - cameraLook.x;
	fVecZ = cameraPosDest.z - cameraLook.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

	heightcameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
	heightcameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

										// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);
}

//����������������������������������������
// �J�����㏈��
//����������������������������������������
void Camera::finalize()
{
	
}

//����������������������������������������
// �J�����X�V����
//����������������������������������������
void Camera::update(Player *pPlayer,Board *pReadyUI)
{
	// ���݂̃V�[���擾
	currentScene = SceneManager::getCurrentSceneType();

	// �V�[���ɉ����ĕ���
	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		updateTitle(pPlayer);
		break;
	case SceneManager::SceneState::SceneMain:
		updateGameMain(pPlayer, pReadyUI);
		break;
	case SceneManager::SceneState::SceneResult:
		break;
	case SceneManager::SceneState::SceneStageEdit:
		break;
	default:
		break;
	}


}

//��������������������������������������������
// �J�����X�V(�^�C�g���V�[��)
//��������������������������������������������
void Camera::updateTitle(Pawn *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

	cameraLook = PlayerPos;
	cameraLook.y += 3.0f;

	cameraFowerd = cameraLook - cameraPos;

//	rotationcamera(pPlayer->getOffset());

	/*
#if 1 // ���`��ԏ���
	// ���ʏ���
	D3DXVec3Lerp(&cameraPos, &cameraPos, &cameraPosDest, fLerpCnt);
	D3DXVec3Normalize(&cameraUp, &cameraUp);

	// ���`��ԗp
	fLerpCnt += 0.0001f;
	if (fLerpCnt >= 1.0f)
	{
		bChangecamera = false;
		fLerpCnt = 1.0f;
	}
#endif

	/*
	// �ړ��ʂ̕␳
	fRotMovecamera += fMovecameraDest;
	*/

	float fVecX, fVecZ;
	fVecX = cameraPos.x - cameraLook.x;
	fVecZ = cameraPos.z - cameraLook.z;
	lengthIntervalcamera = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

#if _DEBUG

#if 0	// �f�o�b�O�p�J�����ړ�
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// ���O�ړ�
			cameraPos.x -= cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// ����ړ�
			cameraPos.x -= cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else
		{// ���ړ�
			cameraPos.x -= cosf(cameraRot.y) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y) * VALUE_MOVE_camera;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// �E�O�ړ�
			cameraPos.x += cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// �E��ړ�
			cameraPos.x += cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else
		{// �E�ړ�
			cameraPos.x += cosf(cameraRot.y) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y) * VALUE_MOVE_camera;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// �O�ړ�
		cameraPos.x += sinf(cameraRot.y) * VALUE_MOVE_camera;
		cameraPos.z += cosf(cameraRot.y) * VALUE_MOVE_camera;

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// ��ړ�
		cameraPos.x -= sinf(cameraRot.y) * VALUE_MOVE_camera;
		cameraPos.z -= cosf(cameraRot.y) * VALUE_MOVE_camera;

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// ���_����u���v
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI * 2.0f;
		}

		cameraPos.x = cameraLook.x - sinf(cameraRot.y) * lengthIntervalcamera;
		cameraPos.z = cameraLook.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_C))
	{// ���_����u�E�v
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI * 2.0f;
		}

		cameraPos.x = cameraLook.x - sinf(cameraRot.y) * lengthIntervalcamera;
		cameraPos.z = cameraLook.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// ���_�ړ��u��v
		cameraPos.y += VALUE_MOVE_camera;
	}
	if (getKeyboardPress(DIK_N))
	{// ���_�ړ��u���v
		cameraPos.y -= VALUE_MOVE_camera;
	}

	if (getKeyboardPress(DIK_Q))
	{// �����_����u���v
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI*2.0f;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_E))
	{// �����_����u�E�v
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI*2.0f;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_T))
	{// �����ړ��u��v
		cameraLook.y += VALUE_MOVE_camera;
	}
	if (getKeyboardPress(DIK_B))
	{// �����ړ��u���v
		cameraLook.y -= VALUE_MOVE_camera;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	cameraPos  = pPlayer->getOffset() - Look * 10.0f;
	cameraPos.y += 6.0f;
	cameraLook = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// �����_�̕␳
	cameraLook.x += (cameraLookDest.x - cameraLook.x) * RATE_CHASE_camera_L;
	cameraLook.y += (cameraLookDest.y - cameraLook.y) * 0.1f;
	cameraLook.z += (cameraLookDest.z - cameraLook.z) * RATE_CHASE_camera_L;

	// ���_�̕␳
	cameraPos.x += (cameraPosDest.x - cameraPos.x) * RATE_CHASE_camera_P;
	cameraPos.y += (cameraPosDest.y - cameraPos.y) * RATE_CHASE_camera_P;
	cameraPos.z += (cameraPosDest.z - cameraPos.z) * RATE_CHASE_camera_P;
	*/

	/*
	PrintDebugProc("�f�o�b�O�\��ON/OFF �F F1     �J�����A���f�����Z�b�g �F SPACE\n");
	PrintDebugProc("[�J�����̎��_  �F(%f : %f : %f)]\n", cameraPos.x, cameraPos.y, cameraPos.z);
	PrintDebugProc("[�J�����̒����_�F(%f : %f : %f)]\n", cameraLook.x, cameraLook.y, cameraLook.z);
	PrintDebugProc("[�J�����̌���  �F(%f)]\n", cameraRot.y);
	PrintDebugProc("\n");

	PrintDebugProc("*** ���_���� ***\n");
	PrintDebugProc("�O�ړ� : W\n");
	PrintDebugProc("��ړ� : S\n");
	PrintDebugProc("���ړ� : A\n");
	PrintDebugProc("�E�ړ� : D\n");
	PrintDebugProc("��ړ� : Y\n");
	PrintDebugProc("���ړ� : N\n");
	PrintDebugProc("������ : Z\n");
	PrintDebugProc("�E���� : C\n");
	PrintDebugProc("\n");

	PrintDebugProc("*** �����_���� ***\n");
	PrintDebugProc("��ړ� : T\n");
	PrintDebugProc("���ړ� : B\n");
	PrintDebugProc("������ : Q\n");
	PrintDebugProc("�E���� : E\n");
	PrintDebugProc("\n");
	*/
#endif

}

//������������������������������������������������
// �J�����X�V(�X�e�[�W�G�f�B�b�g�V�[��)
//������������������������������������������������
void Camera::updateStageEdit(const D3DXVECTOR3 &Pos)
{
	D3DXVECTOR3 BlockPos = Pos;

	cameraPosDest = D3DXVECTOR3(BlockPos.x, BlockPos.y + 20.0f, BlockPos.z - 20.0f);

	cameraLookDest = BlockPos;

	static float fcnt = 0.0f;
	fcnt += 0.0001f;
	D3DXVec3Lerp(&cameraPos,  &cameraPos,		  &cameraPosDest,  fcnt);
	D3DXVec3Lerp(&cameraLook, &cameraLook,		  &cameraLookDest, fcnt);
	if (fcnt > 0.2f)
		fcnt = 0.0f;
#if _DEBUG

#if 0	// �f�o�b�O�p�J�����ړ�
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// ���O�ړ�
			cameraPos.x -= cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// ����ړ�
			cameraPos.x -= cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else
		{// ���ړ�
			cameraPos.x -= cosf(cameraRot.y) * VALUE_MOVE_camera;
			cameraPos.z += sinf(cameraRot.y) * VALUE_MOVE_camera;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// �E�O�ړ�
			cameraPos.x += cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// �E��ړ�
			cameraPos.x += cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_camera;
		}
		else
		{// �E�ړ�
			cameraPos.x += cosf(cameraRot.y) * VALUE_MOVE_camera;
			cameraPos.z -= sinf(cameraRot.y) * VALUE_MOVE_camera;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// �O�ړ�
		cameraPos.x += sinf(cameraRot.y) * VALUE_MOVE_camera;
		cameraPos.z += cosf(cameraRot.y) * VALUE_MOVE_camera;

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// ��ړ�
		cameraPos.x -= sinf(cameraRot.y) * VALUE_MOVE_camera;
		cameraPos.z -= cosf(cameraRot.y) * VALUE_MOVE_camera;

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// ���_����u���v
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI * 2.0f;
		}

		cameraPos.x = cameraLook.x - sinf(cameraRot.y) * lengthIntervalcamera;
		cameraPos.z = cameraLook.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_C))
	{// ���_����u�E�v
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI * 2.0f;
		}

		cameraPos.x = cameraLook.x - sinf(cameraRot.y) * lengthIntervalcamera;
		cameraPos.z = cameraLook.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// ���_�ړ��u��v
		cameraPos.y += VALUE_MOVE_camera;
	}
	if (getKeyboardPress(DIK_N))
	{// ���_�ړ��u���v
		cameraPos.y -= VALUE_MOVE_camera;
	}

	if (getKeyboardPress(DIK_Q))
	{// �����_����u���v
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI*2.0f;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_E))
	{// �����_����u�E�v
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI*2.0f;
		}

		cameraLook.x = cameraPos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		cameraLook.z = cameraPos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_T))
	{// �����ړ��u��v
		cameraLook.y += VALUE_MOVE_camera;
	}
	if (getKeyboardPress(DIK_B))
	{// �����ړ��u���v
		cameraLook.y -= VALUE_MOVE_camera;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	cameraPos  = pPlayer->getOffset() - Look * 10.0f;
	cameraPos.y += 6.0f;
	cameraLook = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// �����_�̕␳
	cameraLook.x += (cameraLookDest.x - cameraLook.x) * RATE_CHASE_camera_L;
	cameraLook.y += (cameraLookDest.y - cameraLook.y) * 0.1f;
	cameraLook.z += (cameraLookDest.z - cameraLook.z) * RATE_CHASE_camera_L;

	// ���_�̕␳
	cameraPos.x += (cameraPosDest.x - cameraPos.x) * RATE_CHASE_camera_P;
	cameraPos.y += (cameraPosDest.y - cameraPos.y) * RATE_CHASE_camera_P;
	cameraPos.z += (cameraPosDest.z - cameraPos.z) * RATE_CHASE_camera_P;
	*/
	/*
	PrintDebugProc("�f�o�b�O�\��ON/OFF �F F1     �J�����A���f�����Z�b�g �F SPACE\n");
	PrintDebugProc("[�J�����̎��_  �F(%f : %f : %f)]\n", cameraPos.x, cameraPos.y, cameraPos.z);
	PrintDebugProc("[�J�����̒����_�F(%f : %f : %f)]\n", cameraLook.x, cameraLook.y, cameraLook.z);
	PrintDebugProc("[�J�����̌���  �F(%f)]\n", cameraRot.y);
	PrintDebugProc("\n");

	PrintDebugProc("*** ���_���� ***\n");
	PrintDebugProc("�O�ړ� : W\n");
	PrintDebugProc("��ړ� : S\n");
	PrintDebugProc("���ړ� : A\n");
	PrintDebugProc("�E�ړ� : D\n");
	PrintDebugProc("��ړ� : Y\n");
	PrintDebugProc("���ړ� : N\n");
	PrintDebugProc("������ : Z\n");
	PrintDebugProc("�E���� : C\n");
	PrintDebugProc("\n");

	PrintDebugProc("*** �����_���� ***\n");
	PrintDebugProc("��ړ� : T\n");
	PrintDebugProc("���ړ� : B\n");
	PrintDebugProc("������ : Q\n");
	PrintDebugProc("�E���� : E\n");
	PrintDebugProc("\n");
	*/
#endif
}

//������������������������������������������������
// �J�����X�V(�Q�[�����C��)
//������������������������������������������������
void Camera::updateGameMain(Player *pPlayer,Board *pReadyUI)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);


	switch (cameraMoveFade)
	{
		case MAIN_TUTORIAL:
			{

			cameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);		// �J�����̎��_
			cameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̒����_


			}
			break;

		case MAIN_START_FADE:
			{
				UINT Indx = pReadyUI->getCurrentAnim();

				cameraPos = fadePos[Indx - 1];
				cameraLook = pPlayer->getOffset();

				if (Indx == 0)
				{
					cameraMoveFade = MAIN_NORMAL;
					cameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);		// �J�����̎��_
					cameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̒����_
				}
			}
		break;
		case MAIN_NORMAL:
			{
				D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

				cameraPosDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 4.3f, PlayerPos.z - 9.0f);
				cameraLookDest = PlayerPos + D3DXVECTOR3(0.0f, 0.0f, 30.0f);

				static float fcnt = 0.0f;
				fcnt += 0.001f;
				D3DXVec3Lerp(&cameraPos, &cameraPos, &cameraPosDest, fcnt);
				D3DXVec3Lerp(&cameraLook, &cameraLook, &cameraLookDest, fcnt);

				if (fcnt > 1.0f)
					fcnt = 1.0f;

			}

			break;
		case MAIN_GOAL_FADE:
		{

			D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

			D3DXVECTOR3 VecLength = cameraPos - PlayerPos;
			FLOAT Length = sqrtf(PlayerPos.x * PlayerPos.x + PlayerPos.z * PlayerPos.z);

			rotCnt += RotSpeed;
			RotMove(&PlayerPos, 10.0f);

			cameraPos.y = 10.0f;
			cameraLook = PlayerPos;
			cameraLook.y = 0.0f;

		}
		break;
	default:
		break;
	}


	cameraLook = PlayerPos;
	cameraLook.y += 3.0f;

	cameraFowerd = cameraLook - cameraPos;

//	rotationcamera(pPlayer->getOffset());

}

//����������������������������������������������������������������
// 
//����������������������������������������������������������������
void Camera::RotMove(D3DXVECTOR3* pVecCenter, FLOAT fRadius)
{
	D3DXMATRIX matRotation;	// �|�����킹�p��]�s��
	rot = rotCnt / 1000.0f;

	//���_�ɔ��a�𑫂��������̍��W��p��
	D3DXVECTOR3 vecTarget(-fRadius, 0.0f, -fRadius);

	//���_�𒆐S�Ƃ�����]�i�I�C���[��]�j�̍s������
	D3DXMatrixRotationY(&matRotation, rot);
	D3DXVec3TransformCoord(&vecTarget, &vecTarget, &matRotation);

	//�Ō�ɖ{���̍��W�i��]�Ώۂ̍��W�j�𑫂����킹
	D3DXVec3Add(&vecTarget, &vecTarget, pVecCenter);
	cameraPos.x = vecTarget.x;
	cameraPos.z = vecTarget.z;
}

//������������������������������������������������
// �J�����̐ݒ菈��
//������������������������������������������������
void Camera::setCamera()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);
	
	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&mtxView,
		&cameraPos	,		// �J�����̎��_
		&cameraLook,		// �J�����̒����_
		&cameraUp);			// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_VIEW, &mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,			// ����p
		static_cast<FLOAT> (Application::ScreenWidth) / static_cast<FLOAT>(Application::ScreenHeight),		// �A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_PROJECTION, &mtxProjection);
	
//	D3DXVec3Unproject()
//	devicePtr->getViewport()	// �r���[�|�[�g�}�g���b�N�X�擾
}

//��������������������������������������������������
// �J������]
//��������������������������������������������������
void Camera::rotationCamera(D3DXVECTOR3 Center)
{
	/*
	C_XINPUT *pXinput = C_XINPUT::getInstance();

	short RightStickX = pXinput->getThumbRX();
	short RightStickY = pXinput->getThumbRY();

	RightStickX /= (32767 * 0.5f);
	
	if (getKeyboardPress(DIK_L))
		RightStickX ++;
	if (getKeyboardPress(DIK_J))
		RightStickX --;
	if (getKeyboardPress(DIK_K))
		RightStickY --;
	if (getKeyboardPress(DIK_I))
		RightStickY ++;

	*/
//	D3DXVECTOR3 cameraForwrd = GetCameraFowerd();

	RotMove(&Center, 5.0f);

	// ��]����
//	rotCnt += camera_ROT_SPEED * RightStickX;


//	D3DXVec3Normalize(&cameraForwrd, &cameraForwrd);


//	cameraPos.y += RightStickY;
	/*
	if (RightStickY > 0.0f)
	{
		cameraPos.y -= 0.1f;
	}
	else if (RightStickY < 0.0f)
	{
		cameraPos.y += 0.1f;
	}
	*/
}

//��������������������������������������������������
// �J�����̌����擾
//��������������������������������������������������
D3DXVECTOR3 Camera::getRot()
{
	return cameraRot;
}

//������������������������������������������������������������������������������
// �r���[�}�g���b�N�X�̎擾
//������������������������������������������������������������������������������
D3DXMATRIX Camera::getMtxView()
{
	return mtxView;
}

//����������������������������������������������������
// �v���W�F�N�V�����}�g���b�N�X�擾
//����������������������������������������������������
D3DXMATRIX Camera::getProjectionMtx()
{
	return mtxProjection;
}

//����������������������������������������������������
// �r���[�|�[�g�}�g���b�N�X�擾
//����������������������������������������������������
D3DVIEWPORT9* Camera::getViwPort()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	devicePtr->GetViewport(&viewPort);

	return &viewPort;
}

//����������������������������������������
// �J�����ʒu�擾
//����������������������������������������
D3DXVECTOR3 Camera::getPos()
{
	return cameraPos;
}

//����������������������������������������
// �J���������_�擾
//����������������������������������������
D3DXVECTOR3 Camera::getLook()
{
	return cameraLook;
}

//����������������������������������������
// �J�����A�b�v�x�N�g���擾
//����������������������������������������
D3DXVECTOR3* Camera::getUp()
{
	return &cameraUp;
}

//����������������������������������������
// �J�����̑O�x�N�g���擾
//����������������������������������������
D3DXVECTOR3 Camera::getFowerd()
{
	D3DXVECTOR3 cameraFowerd = cameraLook - cameraPos;
	D3DXVec3Normalize(&cameraFowerd, &cameraFowerd);
	
	return cameraFowerd;
}

//����������������������������������������
// �J�����X�e�[�g�Z�b�g
//����������������������������������������
void Camera::setState(camera_MOVE_TYPE setState)
{
	cameraMoveFade = setState;
}