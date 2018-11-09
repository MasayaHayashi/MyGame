//��������������������������������������������������������
// Camera.cpp
// �J�����N���X
// Author : MasayaHayashi
//��������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "camera.h"
#include "../SceneManager/SceneManager.h"
#include "input.h"
#include "C_Block.h"
#include "C_Player.h"
#include "C_Ready.h"
#include "C_Xinput.h"

#if _DEBUG
#include "debugproc.h"
#endif

// ===== �萔�E�}�N����` =====
#define MAX_PITCH_SPEED		(25.0f)	// �s�b�`�̍ő�l
#define ACCELE_PITCH_SPEED	(0.5f)	// 1�t���[���ɉ�������s�b�`�̒l
#define DECELE_PITCH_SPEED	(0.25f)	// 1�t���[���Ɍ�������s�b�`�̒l
#define ROT_SPEED (10.0f)

//��������������������������������
// �R���X�g���N�^
//��������������������������������
Camera::Camera()
{
	rotMoveCamera  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	moveCameraDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	numMovePitch	= 0.0f;
	rotPitchRadian = 0.0f;
	cameraUpDest	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	currentState	= CameraState::Type1Person;

	cameraFowerd	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	changeCamera	= true;
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
void Camera::finalize(C_PLAYER *pPlayer)
{

	// ���݂̃V�[���擾
	currentScene = getSceneManager()->getCurrentScene();


	// �V�[���ɉ����ĕ���
	switch (CurrentScene)
	{
	case C_SCENE_MANAGER::SCENE_TITLE:
		InitCamera_Title(pPlayer);
		break;
	case C_SCENE_MANAGER::SCENE_MAIN:
		InitCamera_GameMain(pPlayer);
		break;
	case C_SCENE_MANAGER::SCENE_RESULT:
		break;
	case C_SCENE_MANAGER::SCENE_STAGE_EDIT:
		InitCamera_StageEdit();
		break;
	default:
		break;
	}
}

//����������������������������������������
// �J����������
//����������������������������������������
void Camera::InitCamera()
{

	CameraPos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// �J�����̎��_
	CameraLook  = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �J�����̒����_
	CameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);									// �J�����̏����

	CameraPosDest = D3DXVECTOR3(0.0f, 0.0f, -9000.0f);								// �J�����̎��_�̖ړI�ʒu
//	CameraLookDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 1.7f, PlayerPos.z);		// �J�����̒����_�̖ړI�ʒu

																					//	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
																					//	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

																					//	ForwardVec *= 5.0f;
																					//	CameraPosDest -= ForwardVec;

	CameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = CameraPosDest.x - CameraLook.x;
	fVecZ = CameraPosDest.z - CameraLook.z;

	fLengthIntervalCamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

	fHeightCameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
	fHeightCameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

										// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);



}

//����������������������������������������
// �J�����������i�^�C�g���j
//����������������������������������������
void Camera::InitCamera_Title(C_PLAYER *pPlayer)
{
	// �v���C���[���擾
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

	CameraPos  = D3DXVECTOR3(0.0f, 5.0f, -19.0f);							// �J�����̎��_
	CameraLook = D3DXVECTOR3(PlayerPos.x, PlayerPos.y -0.7f, PlayerPos.z);	// �J�����̒����_
	CameraUp   = pPlayer->getUpVec();										// �J�����̏����

	CameraPosDest  = D3DXVECTOR3(0.0f, 0.0f, -9.0f);								// �J�����̎��_�̖ړI�ʒu
	CameraLookDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 1.7f, PlayerPos.z);		// �J�����̒����_�̖ړI�ʒu

//	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
//	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

//	ForwardVec *= 5.0f;
//	CameraPosDest -= ForwardVec;

	CameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = CameraPosDest.x - CameraLook.x;
	fVecZ = CameraPosDest.z - CameraLook.z;

	fLengthIntervalCamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

	fHeightCameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
	fHeightCameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

	// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);





	//	
	//	if (getKeyboardPress(DIK_1))
	//	{
	//		bChangeCamera = true;
	//		CurrentState = TYPE_1PERSON;
	//	}
	//	if (getKeyboardPress(DIK_2))
	//	{
	//		bChangeCamera = true;
	//
	//		CurrentState = TYPE_3PERSON;
	//	}
	//
	//	switch (CurrentState)
	//	{
	//	case TYPE_3PERSON:
	//		D3DXVec3TransformCoord(&CameraPos, &D3DXVECTOR3(0.0f, 5.5f, -50.0f), pPlayer->getWorldMtx());
	//		CameraLook = D3DXVECTOR3(PlayerPos.x, PlayerPos.y, PlayerPos.z);			// �J�����̒����_	y 5000 z5000
	//		CameraUp = UpVec;				// �J�����̏����
	//		fLerpCnt = 0.0f;
	//		break;
	//	case TYPE_1PERSON:
	//		D3DXVec3TransformCoord(&CameraPosDest, &D3DXVECTOR3(0.0f, 2.5f, 12.0f), pPlayer->getWorldMtx());
	//		CameraLook = D3DXVECTOR3(PlayerPos.x, PlayerPos.y, PlayerPos.z) + ForwardVec * 50.0f;			// �J�����̒����_	y 5000 z5000
	//		CameraUp = UpVec;				// �J�����̏����
	//		break;
	//	default:
	//		break;
	//
	//	}
	//	// ���ʏ���
	//	D3DXVec3Lerp(&CameraPos, &CameraPos, &CameraPosDest, fLerpCnt);
	//	D3DXVec3Normalize(&CameraUp, &CameraUp);
	//
	//	// ���`��ԗp
	//	fLerpCnt += 0.01f;
	//	if (fLerpCnt >= 1.0f)
	//	{
	//		bChangeCamera = false;
	//		fLerpCnt = 1.0f;
	//	}
	//#if _DEBUG
	//	PrintDebugProc("X:%f", PlayerPos.x);
	//	PrintDebugProc("Y:%f", PlayerPos.y);
	//	PrintDebugProc("Z:%f", PlayerPos.z);
	//#endif
	//
	//	// �J�����U��
	//	if (nCntShakeFrame > 0)
	//	{
	//		nCntShakeFrame--;
	//
	//		// �T�C���J�[�u�p�x���X�V
	//		fRadShake += 2.0f * D3DX_PI / (float)DEFAULT_SHAKE_COUNT;
	//
	//		if (fRadShake >= D3DX_PI)
	//			fRadShake -= 2.0f * D3DX_PI;
	//
	//		// �I�����Ɋp�x��0�ɖ߂�
	//		if (0 == nCntShakeFrame)
	//			fRadShake = 0.0f;
	//
	//	}

	/*
	if (getKeyboardPress(DIK_LEFT))
	{// �Y�[���C��
	if (fLengthIntervalCamera > 100.0f)
	{
	fLengthIntervalCamera -= 1.0f;
	fHeightCameraP -= 0.35f;
	}
	}
	if (getKeyboardPress(DIK_RIGHT))
	{// �Y�[���A�E�g
	if (fLengthIntervalCamera < 300.0f)
	{
	fLengthIntervalCamera += 1.0f;
	fHeightCameraP += 0.35f;
	}
	}
	*/

	/*
	if (getKeyboardPress(DIK_Z))
	{// ���_����u���v
	rotCamera.y += VALUE_ROTATE_CAMERA;
	if (rotCamera.y > D3DX_PI)
	{
	rotCamera.y -= D3DX_PI * 2.0f;
	}

	posCameraP.x = posCameraL.x - sinf(rotCamera.y) * fLengthIntervalCamera;
	posCameraP.z = posCameraL.z - cosf(rotCamera.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_C))
	{// ���_����u�E�v
	rotCamera.y -= VALUE_ROTATE_CAMERA;
	if (rotCamera.y < -D3DX_PI)
	{
	rotCamera.y += D3DX_PI * 2.0f;
	}

	posCameraP.x = posCameraL.x - sinf(rotCamera.y) * fLengthIntervalCamera;
	posCameraP.z = posCameraL.z - cosf(rotCamera.y) * fLengthIntervalCamera;
	}
	*/

	/*
	// �����_�̕␳
	posCameraL.x += (posCameraLDest.x - posCameraL.x) * fRateChaseCameraL;
	posCameraL.y += (posCameraLDest.y - posCameraL.y) * 0.1f;
	posCameraL.z += (posCameraLDest.z - posCameraL.z) * fRateChaseCameraL;

	// ���_�̕␳
	posCameraP.x += (posCameraPDest.x - posCameraP.x) * fRateChaseCameraP;
	posCameraP.y += (posCameraPDest.y - posCameraP.y) * fRateChaseCameraP;
	posCameraP.z += (posCameraPDest.z - posCameraP.z) * fRateChaseCameraP;

	// �ړ��ʂ̕␳
	fRotMoveCamera += fMoveCameraDest;
	*/


}

//����������������������������������������
// �J����������(�X�e�[�W�G�f�B�b�g)
//����������������������������������������
void Camera::InitCamera_StageEdit()
{
	// �v���C���[���擾

	CameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);							// �J�����̎��_
	CameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̒����_
	CameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								// �J�����̏����

	CameraPosDest = D3DXVECTOR3(0.0f, 0.0f, -9.0f);								// �J�����̎��_�̖ړI�ʒu
	CameraLookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);		// �J�����̒����_�̖ړI�ʒu

																					//	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
																					//	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

																					//	ForwardVec *= 5.0f;
																					//	CameraPosDest -= ForwardVec;

	CameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = CameraPosDest.x - CameraLook.x;
	fVecZ = CameraPosDest.z - CameraLook.z;

	fLengthIntervalCamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

	fHeightCameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
	fHeightCameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

	// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);

}

//����������������������������������������
// �J����������(�Q�[�����C��)
//����������������������������������������
void Camera::InitCamera_GameMain(C_PLAYER *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getPosition();

	FadePos[2]  = PlayerPos + D3DXVECTOR3(10.0f, PlayerPos.y  + 4.3f, 5.0f);
	FadePos[1]  = PlayerPos + D3DXVECTOR3(-4.0f, PlayerPos.y  + 4.3f, 5.0f);
	FadePos[0]  = D3DXVECTOR3(0.0f, 10.0f, -19.0f);


	// �v���C���[���擾
	CameraPos	= D3DXVECTOR3(0.0f, 10.0f, -19.0f);			// �J�����̎��_
	CameraLook	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̒����_
	CameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �J�����̏����

	CameraPosDest	= D3DXVECTOR3(0.0f, 0.0f, -9.0f);		// �J�����̎��_�̖ړI�ʒu
	CameraLookDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �J�����̒����_�̖ړI�ʒu


	CameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = CameraPosDest.x - CameraLook.x;
	fVecZ = CameraPosDest.z - CameraLook.z;

	fLengthIntervalCamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

	fHeightCameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
	fHeightCameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

										// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);
}

//����������������������������������������
// �J�����㏈��
//����������������������������������������
void Camera::finalizeCamera()
{
	
}

//����������������������������������������
// �J�����X�V����
//����������������������������������������
void Camera::updateCamera(C_PLAYER *pPlayer,C_BOARD *pReadyUI)
{
	// ���݂̃V�[���擾
	CurrentScene = getSceneManager()->getCurrentScene();

	// �V�[���ɉ����ĕ���
	switch (CurrentScene)
	{
	case C_SCENE_MANAGER::SCENE_TITLE:
		updateCamera_Title(pPlayer);
		break;
	case C_SCENE_MANAGER::SCENE_MAIN:
		updateCamera_GameMain(pPlayer, pReadyUI);
		break;
	case C_SCENE_MANAGER::SCENE_RESULT:
		break;
	case C_SCENE_MANAGER::SCENE_STAGE_EDIT:
		break;
	default:
		break;
	}


}

//��������������������������������������������
// �J�����X�V(�^�C�g���V�[��)
//��������������������������������������������
void Camera::updateCamera_Title(Pawn *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

	CameraLook = PlayerPos;
	CameraLook.y += 3.0f;

	CameraFowerd = CameraLook - CameraPos;

	RotationCamera(pPlayer->getOffset());

	/*
#if 1 // ���`��ԏ���
	// ���ʏ���
	D3DXVec3Lerp(&CameraPos, &CameraPos, &CameraPosDest, fLerpCnt);
	D3DXVec3Normalize(&CameraUp, &CameraUp);

	// ���`��ԗp
	fLerpCnt += 0.0001f;
	if (fLerpCnt >= 1.0f)
	{
		bChangeCamera = false;
		fLerpCnt = 1.0f;
	}
#endif

	/*
	// �ړ��ʂ̕␳
	fRotMoveCamera += fMoveCameraDest;
	*/

	float fVecX, fVecZ;
	fVecX = CameraPos.x - CameraLook.x;
	fVecZ = CameraPos.z - CameraLook.z;
	fLengthIntervalCamera = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

#if _DEBUG

#if 1	// �f�o�b�O�p�J�����ړ�
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// ���O�ړ�
			CameraPos.x -= cosf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else if (getKeyboardPress(DIK_S))
		{// ����ړ�
			CameraPos.x -= cosf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else
		{// ���ړ�
			CameraPos.x -= cosf(CameraRot.y) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// �E�O�ړ�
			CameraPos.x += cosf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else if (getKeyboardPress(DIK_S))
		{// �E��ړ�
			CameraPos.x += cosf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else
		{// �E�ړ�
			CameraPos.x += cosf(CameraRot.y) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// �O�ړ�
		CameraPos.x += sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		CameraPos.z += cosf(CameraRot.y) * VALUE_MOVE_CAMERA;

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// ��ړ�
		CameraPos.x -= sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		CameraPos.z -= cosf(CameraRot.y) * VALUE_MOVE_CAMERA;

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// ���_����u���v
		CameraRot.y += VALUE_ROTATE_CAMERA;
		if (CameraRot.y > D3DX_PI)
		{
			CameraRot.y -= D3DX_PI * 2.0f;
		}

		CameraPos.x = CameraLook.x - sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraPos.z = CameraLook.z - cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_C))
	{// ���_����u�E�v
		CameraRot.y -= VALUE_ROTATE_CAMERA;
		if (CameraRot.y < -D3DX_PI)
		{
			CameraRot.y += D3DX_PI * 2.0f;
		}

		CameraPos.x = CameraLook.x - sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraPos.z = CameraLook.z - cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// ���_�ړ��u��v
		CameraPos.y += VALUE_MOVE_CAMERA;
	}
	if (getKeyboardPress(DIK_N))
	{// ���_�ړ��u���v
		CameraPos.y -= VALUE_MOVE_CAMERA;
	}

	if (getKeyboardPress(DIK_Q))
	{// �����_����u���v
		CameraRot.y -= VALUE_ROTATE_CAMERA;
		if (CameraRot.y < -D3DX_PI)
		{
			CameraRot.y += D3DX_PI*2.0f;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_E))
	{// �����_����u�E�v
		CameraRot.y += VALUE_ROTATE_CAMERA;
		if (CameraRot.y > D3DX_PI)
		{
			CameraRot.y -= D3DX_PI*2.0f;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_T))
	{// �����ړ��u��v
		CameraLook.y += VALUE_MOVE_CAMERA;
	}
	if (getKeyboardPress(DIK_B))
	{// �����ړ��u���v
		CameraLook.y -= VALUE_MOVE_CAMERA;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	CameraPos  = pPlayer->getOffset() - Look * 10.0f;
	CameraPos.y += 6.0f;
	CameraLook = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// �����_�̕␳
	CameraLook.x += (CameraLookDest.x - CameraLook.x) * RATE_CHASE_CAMERA_L;
	CameraLook.y += (CameraLookDest.y - CameraLook.y) * 0.1f;
	CameraLook.z += (CameraLookDest.z - CameraLook.z) * RATE_CHASE_CAMERA_L;

	// ���_�̕␳
	CameraPos.x += (CameraPosDest.x - CameraPos.x) * RATE_CHASE_CAMERA_P;
	CameraPos.y += (CameraPosDest.y - CameraPos.y) * RATE_CHASE_CAMERA_P;
	CameraPos.z += (CameraPosDest.z - CameraPos.z) * RATE_CHASE_CAMERA_P;
	*/

	PrintDebugProc("�f�o�b�O�\��ON/OFF �F F1     �J�����A���f�����Z�b�g �F SPACE\n");
	PrintDebugProc("[�J�����̎��_  �F(%f : %f : %f)]\n", CameraPos.x, CameraPos.y, CameraPos.z);
	PrintDebugProc("[�J�����̒����_�F(%f : %f : %f)]\n", CameraLook.x, CameraLook.y, CameraLook.z);
	PrintDebugProc("[�J�����̌���  �F(%f)]\n", CameraRot.y);
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
#endif
}

//������������������������������������������������
// �J�����X�V(�X�e�[�W�G�f�B�b�g�V�[��)
//������������������������������������������������
void Camera::updateCamera_StageEdit(const D3DXVECTOR3 &Pos)
{
	D3DXVECTOR3 BlockPos = Pos;

	CameraPosDest = D3DXVECTOR3(BlockPos.x, BlockPos.y + 20.0f, BlockPos.z - 20.0f);

	CameraLookDest = BlockPos;

	static float fcnt = 0.0f;
	fcnt += 0.0001f;
	D3DXVec3Lerp(&CameraPos,  &CameraPos,		  &CameraPosDest,  fcnt);
	D3DXVec3Lerp(&CameraLook, &CameraLook,		  &CameraLookDest, fcnt);
	if (fcnt > 0.2f)
		fcnt = 0.0f;
#if _DEBUG

#if 0	// �f�o�b�O�p�J�����ړ�
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// ���O�ړ�
			CameraPos.x -= cosf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else if (getKeyboardPress(DIK_S))
		{// ����ړ�
			CameraPos.x -= cosf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else
		{// ���ړ�
			CameraPos.x -= cosf(CameraRot.y) * VALUE_MOVE_CAMERA;
			CameraPos.z += sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// �E�O�ړ�
			CameraPos.x += cosf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else if (getKeyboardPress(DIK_S))
		{// �E��ړ�
			CameraPos.x += cosf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
		}
		else
		{// �E�ړ�
			CameraPos.x += cosf(CameraRot.y) * VALUE_MOVE_CAMERA;
			CameraPos.z -= sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// �O�ړ�
		CameraPos.x += sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		CameraPos.z += cosf(CameraRot.y) * VALUE_MOVE_CAMERA;

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// ��ړ�
		CameraPos.x -= sinf(CameraRot.y) * VALUE_MOVE_CAMERA;
		CameraPos.z -= cosf(CameraRot.y) * VALUE_MOVE_CAMERA;

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// ���_����u���v
		CameraRot.y += VALUE_ROTATE_CAMERA;
		if (CameraRot.y > D3DX_PI)
		{
			CameraRot.y -= D3DX_PI * 2.0f;
		}

		CameraPos.x = CameraLook.x - sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraPos.z = CameraLook.z - cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_C))
	{// ���_����u�E�v
		CameraRot.y -= VALUE_ROTATE_CAMERA;
		if (CameraRot.y < -D3DX_PI)
		{
			CameraRot.y += D3DX_PI * 2.0f;
		}

		CameraPos.x = CameraLook.x - sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraPos.z = CameraLook.z - cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// ���_�ړ��u��v
		CameraPos.y += VALUE_MOVE_CAMERA;
	}
	if (getKeyboardPress(DIK_N))
	{// ���_�ړ��u���v
		CameraPos.y -= VALUE_MOVE_CAMERA;
	}

	if (getKeyboardPress(DIK_Q))
	{// �����_����u���v
		CameraRot.y -= VALUE_ROTATE_CAMERA;
		if (CameraRot.y < -D3DX_PI)
		{
			CameraRot.y += D3DX_PI*2.0f;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_E))
	{// �����_����u�E�v
		CameraRot.y += VALUE_ROTATE_CAMERA;
		if (CameraRot.y > D3DX_PI)
		{
			CameraRot.y -= D3DX_PI*2.0f;
		}

		CameraLook.x = CameraPos.x + sinf(CameraRot.y) * fLengthIntervalCamera;
		CameraLook.z = CameraPos.z + cosf(CameraRot.y) * fLengthIntervalCamera;
	}
	if (getKeyboardPress(DIK_T))
	{// �����ړ��u��v
		CameraLook.y += VALUE_MOVE_CAMERA;
	}
	if (getKeyboardPress(DIK_B))
	{// �����ړ��u���v
		CameraLook.y -= VALUE_MOVE_CAMERA;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	CameraPos  = pPlayer->getOffset() - Look * 10.0f;
	CameraPos.y += 6.0f;
	CameraLook = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// �����_�̕␳
	CameraLook.x += (CameraLookDest.x - CameraLook.x) * RATE_CHASE_CAMERA_L;
	CameraLook.y += (CameraLookDest.y - CameraLook.y) * 0.1f;
	CameraLook.z += (CameraLookDest.z - CameraLook.z) * RATE_CHASE_CAMERA_L;

	// ���_�̕␳
	CameraPos.x += (CameraPosDest.x - CameraPos.x) * RATE_CHASE_CAMERA_P;
	CameraPos.y += (CameraPosDest.y - CameraPos.y) * RATE_CHASE_CAMERA_P;
	CameraPos.z += (CameraPosDest.z - CameraPos.z) * RATE_CHASE_CAMERA_P;
	*/

	PrintDebugProc("�f�o�b�O�\��ON/OFF �F F1     �J�����A���f�����Z�b�g �F SPACE\n");
	PrintDebugProc("[�J�����̎��_  �F(%f : %f : %f)]\n", CameraPos.x, CameraPos.y, CameraPos.z);
	PrintDebugProc("[�J�����̒����_�F(%f : %f : %f)]\n", CameraLook.x, CameraLook.y, CameraLook.z);
	PrintDebugProc("[�J�����̌���  �F(%f)]\n", CameraRot.y);
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
#endif
}

//������������������������������������������������
// �J�����X�V(�Q�[�����C��)
//������������������������������������������������
void Camera::updateCamera_GameMain(C_PLAYER *pPlayer,C_BOARD *pReadyUI)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);


	switch (CameraMoveFade)
	{
		case MAIN_TUTORIAL:
			{

			CameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);		// �J�����̎��_
			CameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̒����_


			}
			break;

		case MAIN_START_FADE:
			{
				UINT Indx = pReadyUI->getCurrentAnim();

				CameraPos = FadePos[Indx - 1];
				CameraLook = pPlayer->getOffset();

				if (Indx == 0)
				{
					CameraMoveFade = MAIN_NORMAL;
					CameraPos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);		// �J�����̎��_
					CameraLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̒����_
				}
			}
		break;
		case MAIN_NORMAL:
			{
				D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

				CameraPosDest = D3DXVECTOR3(PlayerPos.x, PlayerPos.y + 4.3f, PlayerPos.z - 9.0f);
				CameraLookDest = PlayerPos + D3DXVECTOR3(0.0f, 0.0f, 30.0f);

				static float fcnt = 0.0f;
				fcnt += 0.001f;
				D3DXVec3Lerp(&CameraPos, &CameraPos, &CameraPosDest, fcnt);
				D3DXVec3Lerp(&CameraLook, &CameraLook, &CameraLookDest, fcnt);

				if (fcnt > 1.0f)
					fcnt = 1.0f;

			}

			break;
		case MAIN_GOAL_FADE:
		{

			D3DXVECTOR3 PlayerPos = pPlayer->getOffset();

			D3DXVECTOR3 VecLength = CameraPos - PlayerPos;
			FLOAT Length = sqrtf(PlayerPos.x * PlayerPos.x + PlayerPos.z * PlayerPos.z);

			nRotCnt += ROT_SPEED;
			RotMove(&PlayerPos, 10.0f);

			CameraPos.y = 10.0f;
			CameraLook = PlayerPos;
			CameraLook.y = 0.0f;

		}
		break;
	default:
		break;
	}


	CameraLook = PlayerPos;
	CameraLook.y += 3.0f;

	CameraFowerd = CameraLook - CameraPos;

	RotationCamera(pPlayer->getOffset());

}

//����������������������������������������������������������������
// 
//����������������������������������������������������������������
void Camera::RotMove(D3DXVECTOR3* pVecCenter, FLOAT fRadius)
{
	D3DXMATRIX matRotation;	// �|�����킹�p��]�s��
	fRot = nRotCnt / 1000.0f;

	//���_�ɔ��a�𑫂��������̍��W��p��
	D3DXVECTOR3 vecTarget(-fRadius, 0.0f, -fRadius);

	//���_�𒆐S�Ƃ�����]�i�I�C���[��]�j�̍s������
	D3DXMatrixRotationY(&matRotation, fRot);
	D3DXVec3TransformCoord(&vecTarget, &vecTarget, &matRotation);

	//�Ō�ɖ{���̍��W�i��]�Ώۂ̍��W�j�𑫂����킹
	D3DXVec3Add(&vecTarget, &vecTarget, pVecCenter);
	CameraPos.x = vecTarget.x;
	CameraPos.z = vecTarget.z;
}

//������������������������������������������������
// �J�����̐ݒ菈��
//������������������������������������������������
void Camera::setCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&mtxView,
		&CameraPos	,		// �J�����̎��_
		&CameraLook,		// �J�����̒����_
		&CameraUp);			// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->setTransform(D3DTS_VIEW, &mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,			// ����p
		VIEW_ASPECT,		// �A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->setTransform(D3DTS_PROJECTION, &mtxProjection);
	
//	D3DXVec3Unproject()
//	pDevice->getViewport()	// �r���[�|�[�g�}�g���b�N�X�擾
}

//��������������������������������������������������
// �J������]
//��������������������������������������������������
void Camera::RotationCamera(D3DXVECTOR3 Center)
{
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


	D3DXVECTOR3 CameraForwrd = getCameraFowerd();

	RotMove(&Center, 5.0f);

	// ��]����
	nRotCnt += CAMERA_ROT_SPEED * RightStickX;


	D3DXVec3Normalize(&CameraForwrd, &CameraForwrd);


	CameraPos.y += RightStickY;
	if(RightStickY > 0.0f)
		CameraPos.y -= 0.1f;
	else if(RightStickY < 0.0f)
		CameraPos.y += 0.1f;

}

//��������������������������������������������������
// �J�����̌����擾
//��������������������������������������������������
D3DXVECTOR3 Camera::getCameraRot()
{
	return CameraRot;
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
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	pDevice->getViewport(&ViePort);

	return &ViePort;
}

//����������������������������������������
// �J�����ʒu�擾
//����������������������������������������
D3DXVECTOR3 Camera::getCameraPos()
{
	return CameraPos;
}

//����������������������������������������
// �J���������_�擾
//����������������������������������������
D3DXVECTOR3 Camera::getCameraLook()
{
	return CameraLook;
}

//����������������������������������������
// �J�����A�b�v�x�N�g���擾
//����������������������������������������
D3DXVECTOR3* Camera::getCameraUp()
{
	return &CameraUp;
}

//����������������������������������������
// �J�����̑O�x�N�g���擾
//����������������������������������������
D3DXVECTOR3 Camera::getCameraFowerd()
{
	D3DXVECTOR3 CameraFowerd = CameraLook - CameraPos;
	D3DXVec3Normalize(&CameraFowerd, &CameraFowerd);
	
	return CameraFowerd;
}

//����������������������������������������
// �J�����X�e�[�g�Z�b�g
//����������������������������������������
void Camera::setCameraState(CAMERA_MOVE_TYPE setState)
{
	CameraMoveFade = setState;
}