//��������������������������������������������������������
// Camera.cpp
// �J�����N���X
// Author : MasayaHayashi
//��������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Camera.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../KeyBoard/Keyboard.h"
#include "../Player/Player.h"
#include "../Board/Board.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"
#include "../GameManager/GameManager.h"

//��������������������������������
// �R���X�g���N�^
//��������������������������������
Camera::Camera()
{
	rotvelocityCamera  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	velocityCameraDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	numvelocityPitch	= 0.0f;
	rotPitchRadian = 0.0f;
	cameraUpDest	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	cameraFowerd	= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	changeCamera	= true;
	lerpCnt		= 0.0f;


	rot = 0;
	rotCnt = 0;

	Collision::registerList(&myTransform, "Camera");
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
		
		break;
	case SceneManager::SceneState::SceneMain:
		
		break;
	case SceneManager::SceneState::SceneResult:
		break;
	case SceneManager::SceneState::SceneStageEdit:
		
		break;
	default:
		break;
	}
}

#if _DEBUG

//����������������������������������������
//
//����������������������������������������
void Camera::debugMove()
{
	if (Keyboard::getPress(DIK_W))
	{
		myTransform.pos.y += 0.05f;
	}
	if (Keyboard::getPress(DIK_A))
	{
		myTransform.pos.x -= 0.05f;
	}
	if (Keyboard::getPress(DIK_S))
	{
		myTransform.pos.y -= 0.05f;
	}
	if (Keyboard::getPress(DIK_D))
	{
		myTransform.pos.x += 0.05f;
	}

	DirectX3D::printDebug("myTransform.pos.x%f\n", myTransform.pos.x);
	DirectX3D::printDebug("myTransform.pos.y%f\n", myTransform.pos.y);
	DirectX3D::printDebug("myTransform.pos.z%f\n", myTransform.pos.z);
}

#endif

//����������������������������������������
// �J����������
//����������������������������������������
void Camera::initialize()
{

	myTransform.pos	= D3DXVECTOR3(0.0f, 0.0f, -5.0f);							// �J�����̎��_
	myTransform.look  = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �J�����̒����_
	cameraUp	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);									// �J�����̏����
	
	myTransform.posDest = D3DXVECTOR3(0.0f, 0.0f, -9000.0f);

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

//	heightcameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
//	heightcameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

	// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);


	const Transform playerTransform = *Collision::getTransform("Player").front();

	fadePos[2] = playerTransform.pos + D3DXVECTOR3(10.0f, playerTransform.pos.y + 4.3f, 5.0f);
	fadePos[1] = playerTransform.pos + D3DXVECTOR3(-4.0f, playerTransform.pos.y + 4.3f, 5.0f);
	fadePos[0] = D3DXVECTOR3(0.0f, 10.0f, -19.0f);


}

//����������������������������������������
// �J�����������i�^�C�g���j
//����������������������������������������
void Camera::initializeTitle()
{
	myTransform.pos  = D3DXVECTOR3(0.0f, 3.0f, -19.0f);
	myTransform.look = D3DXVECTOR3(0.0f,0.0f,0.0f);
	myTransform.up   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	myTransform.posDest  = D3DXVECTOR3(0.0f, 0.0f, -9.0f);
	myTransform.lookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	FLOAT fVecX, fVecZ;
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);

	D3DXMatrixIdentity(&mtxView);



}

//����������������������������������������
// �J����������(�X�e�[�W�G�f�B�b�g)
//����������������������������������������
void Camera::initializeStageEdit()
{
	// �v���C���[���擾

	myTransform.pos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);							// �J�����̎��_
	myTransform.look = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̒����_
	cameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								// �J�����̏����

	myTransform.posDest = D3DXVECTOR3(0.0f, -0.0f, -9.0f);								// �J�����̎��_�̖ړI�ʒu
	myTransform.lookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);		// �J�����̒����_�̖ړI�ʒu

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

//	heightcameraP = CHASE_HEIGHT_P;	// �ǐՎ��̎��_�̍���
//	heightcameraL = CHASE_HEIGHT_L;	// �ǐՎ��̒����_�̍���

	// �s�񏉊���
	D3DXMatrixIdentity(&mtxView);

}

//����������������������������������������
// �J����������(�Q�[�����C��)
//����������������������������������������
void Camera::initializeMain(Player *pPlayer)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getPosition();

	fadePos[0]  = PlayerPos + D3DXVECTOR3(0.0f, 3.0f, -20.0f);
	fadePos[1]  = Collision::getTransform("Player", 0)->pos + D3DXVECTOR3(-0.5f, 4.0f, -12.0f);
	fadePos[2]  = Collision::getTransform("Player", 0)->pos + D3DXVECTOR3(0.0f,4.0f,-7.0f);


	myTransform.pos		= D3DXVECTOR3(0.0f, 30.0f, -20.0f);			// �J�����̎��_
	myTransform.look	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̒����_
	cameraUp			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �J�����̏����

	myTransform.posDest		= D3DXVECTOR3(0.0f, -1.0f, -5.0f);		// �J�����̎��_�̖ړI�ʒu
	myTransform.lookDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �J�����̒����_�̖ړI�ʒu


	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �J�����̉�]��

	FLOAT fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

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
void Camera::update(Player *pPlayer,Board &countdown)
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
		updateGameMain(pPlayer,countdown);
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
	D3DXVECTOR3 PlayerPos  = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec	   = pPlayer->getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

	myTransform.look = PlayerPos;
	myTransform.look.y += 10.2f;
	myTransform.fowerd = myTransform.look - myTransform.pos;
	
#if 1 // ���`��ԏ���
	// ���ʏ���
	D3DXVec3Lerp(&myTransform.pos, &myTransform.pos, &myTransform.posDest, lerpCnt);
	D3DXVec3Normalize(&cameraUp, &cameraUp);

	// ���`��ԗp
	lerpCnt += 0.0001f;
	if (lerpCnt >= 1.0f)
	{
		changeCamera = false;
		lerpCnt = 1.0f;
	}
#endif

	
	// �ړ��ʂ̕␳
	rotvelocityCamera += velocityCameraDest;

	FLOAT vecX, vecZ;
	vecX = myTransform.pos.x - myTransform.look.x;
	vecZ = myTransform.pos.z - myTransform.look.z;
	lengthIntervalcamera = sqrtf(vecX * vecX + vecZ * vecZ);

#if _DEBUG

#if 0	// �f�o�b�O�p�J�����ړ�
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// ���O�ړ�
			myTransform.pos.x -= cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// ����ړ�
			myTransform.pos.x -= cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else
		{// ���ړ�
			myTransform.pos.x -= cosf(cameraRot.y) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y) * VALUE_velocity_camera;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// �E�O�ړ�
			myTransform.pos.x += cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// �E��ړ�
			myTransform.pos.x += cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else
		{// �E�ړ�
			myTransform.pos.x += cosf(cameraRot.y) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y) * VALUE_velocity_camera;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// �O�ړ�
		myTransform.pos.x += sinf(cameraRot.y) * VALUE_velocity_camera;
		myTransform.pos.z += cosf(cameraRot.y) * VALUE_velocity_camera;

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// ��ړ�
		myTransform.pos.x -= sinf(cameraRot.y) * VALUE_velocity_camera;
		myTransform.pos.z -= cosf(cameraRot.y) * VALUE_velocity_camera;

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// ���_����u���v
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI * 2.0f;
		}

		myTransform.pos.x = myTransform.look.x - sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.pos.z = myTransform.look.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_C))
	{// ���_����u�E�v
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI * 2.0f;
		}

		myTransform.pos.x = myTransform.look.x - sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.pos.z = myTransform.look.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// ���_�ړ��u��v
		myTransform.pos.y += VALUE_velocity_camera;
	}
	if (getKeyboardPress(DIK_N))
	{// ���_�ړ��u���v
		myTransform.pos.y -= VALUE_velocity_camera;
	}

	if (getKeyboardPress(DIK_Q))
	{// �����_����u���v
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI*2.0f;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_E))
	{// �����_����u�E�v
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI*2.0f;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_T))
	{// �����ړ��u��v
		myTransform.look.y += VALUE_velocity_camera;
	}
	if (getKeyboardPress(DIK_B))
	{// �����ړ��u���v
		myTransform.look.y -= VALUE_velocity_camera;
	}
#endif
#endif

}

//������������������������������������������������
// �J�����X�V(�X�e�[�W�G�f�B�b�g�V�[��)
//������������������������������������������������
void Camera::updateStageEdit(std::string keyName,UINT selectIndex)
{
	D3DXVECTOR3 BlockPos = Collision::getTransform(keyName, selectIndex)->pos;

	myTransform.posDest = D3DXVECTOR3(BlockPos.x, BlockPos.y + 40.0f, BlockPos.z - 30.0f);

	myTransform.lookDest = BlockPos;

	static float fcnt = 0.0f;
	fcnt += 0.0001f;
	D3DXVec3Lerp(&myTransform.pos,  &myTransform.pos,		  &myTransform.posDest,  fcnt);
	D3DXVec3Lerp(&myTransform.look, &myTransform.look,		  &myTransform.lookDest, fcnt);

	if (fcnt > 0.2f)
	{
		fcnt = 0.0f;
	}

#if _DEBUG

#if 0	// �f�o�b�O�p�J�����ړ�
	if (getKeyboardPress(DIK_A))
	{
		if (getKeyboardPress(DIK_W))
		{// ���O�ړ�
			myTransform.pos.x -= cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// ����ړ�
			myTransform.pos.x -= cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else
		{// ���ړ�
			myTransform.pos.x -= cosf(cameraRot.y) * VALUE_velocity_camera;
			myTransform.pos.z += sinf(cameraRot.y) * VALUE_velocity_camera;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_D))
	{
		if (getKeyboardPress(DIK_W))
		{// �E�O�ړ�
			myTransform.pos.x += cosf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y - D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else if (getKeyboardPress(DIK_S))
		{// �E��ړ�
			myTransform.pos.x += cosf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y + D3DX_PI * 0.25f) * VALUE_velocity_camera;
		}
		else
		{// �E�ړ�
			myTransform.pos.x += cosf(cameraRot.y) * VALUE_velocity_camera;
			myTransform.pos.z -= sinf(cameraRot.y) * VALUE_velocity_camera;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_W))
	{// �O�ړ�
		myTransform.pos.x += sinf(cameraRot.y) * VALUE_velocity_camera;
		myTransform.pos.z += cosf(cameraRot.y) * VALUE_velocity_camera;

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	else if (getKeyboardPress(DIK_S))
	{// ��ړ�
		myTransform.pos.x -= sinf(cameraRot.y) * VALUE_velocity_camera;
		myTransform.pos.z -= cosf(cameraRot.y) * VALUE_velocity_camera;

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}

	if (getKeyboardPress(DIK_Z))
	{// ���_����u���v
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI * 2.0f;
		}

		myTransform.pos.x = myTransform.look.x - sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.pos.z = myTransform.look.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_C))
	{// ���_����u�E�v
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI * 2.0f;
		}

		myTransform.pos.x = myTransform.look.x - sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.pos.z = myTransform.look.z - cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_Y))
	{// ���_�ړ��u��v
		myTransform.pos.y += VALUE_velocity_camera;
	}
	if (getKeyboardPress(DIK_N))
	{// ���_�ړ��u���v
		myTransform.pos.y -= VALUE_velocity_camera;
	}

	if (getKeyboardPress(DIK_Q))
	{// �����_����u���v
		cameraRot.y -= VALUE_ROTATE_camera;
		if (cameraRot.y < -D3DX_PI)
		{
			cameraRot.y += D3DX_PI*2.0f;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_E))
	{// �����_����u�E�v
		cameraRot.y += VALUE_ROTATE_camera;
		if (cameraRot.y > D3DX_PI)
		{
			cameraRot.y -= D3DX_PI*2.0f;
		}

		myTransform.look.x = myTransform.pos.x + sinf(cameraRot.y) * lengthIntervalcamera;
		myTransform.look.z = myTransform.pos.z + cosf(cameraRot.y) * lengthIntervalcamera;
	}
	if (getKeyboardPress(DIK_T))
	{// �����ړ��u��v
		myTransform.look.y += VALUE_velocity_camera;
	}
	if (getKeyboardPress(DIK_B))
	{// �����ړ��u���v
		myTransform.look.y -= VALUE_velocity_camera;
	}
#endif

	/*
	D3DXVECTOR3 Look;
	D3DXVec3Normalize(&Look, &pPlayer->getForwardVec());
	myTransform.pos  = pPlayer->getOffset() - Look * 10.0f;
	myTransform.pos.y += 6.0f;
	myTransform.look = pPlayer->getOffset() + Look * 50.0f;
	*/

	/*
	// �����_�̕␳
	myTransform.look.x += (myTransform.lookDest.x - myTransform.look.x) * RATE_CHASE_camera_L;
	myTransform.look.y += (myTransform.lookDest.y - myTransform.look.y) * 0.1f;
	myTransform.look.z += (myTransform.lookDest.z - myTransform.look.z) * RATE_CHASE_camera_L;

	// ���_�̕␳
	myTransform.pos.x += (myTransform.posDest.x - myTransform.pos.x) * RATE_CHASE_camera_P;
	myTransform.pos.y += (myTransform.posDest.y - myTransform.pos.y) * RATE_CHASE_camera_P;
	myTransform.pos.z += (myTransform.posDest.z - myTransform.pos.z) * RATE_CHASE_camera_P;
	*/
	/*
	PrintDebugProc("�f�o�b�O�\��ON/OFF �F F1     �J�����A���f�����Z�b�g �F SPACE\n");
	PrintDebugProc("[�J�����̎��_  �F(%f : %f : %f)]\n", myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	PrintDebugProc("[�J�����̒����_�F(%f : %f : %f)]\n", myTransform.look.x, myTransform.look.y, myTransform.look.z);
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
void Camera::updateGameMain(Player *pPlayer,Board &countDown)
{
	D3DXVECTOR3 PlayerPos = pPlayer->getOffset();
	D3DXVECTOR3 ForwardVec = pPlayer->getForwardVec();
	D3DXVECTOR3 UpVec = pPlayer->getUpVec();

	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);

	if (GameManager::isGameType(GameManager::GameType::Ready))
	{
		myTransform.pos = pPlayer->getPosition();
		myTransform.pos.y += 7.0f;
		myTransform.pos.z -= 10.0f;
		myTransform.look = pPlayer->getPosition();
	}
	currentFadeType = countDown.getCurrentAnim() - 1;

	if (currentFadeType <= 0)
	{
		currentFadeType = 0;
	}

	if (GameManager::isGameType(GameManager::GameType::Ready))
	{
		myTransform.pos = fadePos[currentFadeType];
		myTransform.look = Collision::getTransform("Player", 0)->pos;
	}

	if (GameManager::isGameType(GameManager::GameType::Playing))
	{
		myTransform.posDest = Collision::getTransform("Player", 0)->pos - *D3DXVec3Normalize(&cameraFowerd, &cameraFowerd) * 6.0f;
		myTransform.posDest.y += 4.0f;
		myTransform.lookDest = Collision::getTransform("Player", 0)->pos + *D3DXVec3Normalize(&cameraFowerd, &cameraFowerd) * 10.0f;
		//myTransform.pos -= cameraFowerd;

		static float fcnt = 0.0f;
		fcnt += 0.02f;

		if (fcnt > 1.0f)
		{
			fcnt = 1.0f;
		}

		D3DXVec3Lerp(&myTransform.pos, &myTransform.pos, &myTransform.posDest, fcnt);
		D3DXVec3Lerp(&myTransform.look, &myTransform.look, &myTransform.lookDest, fcnt);

		DirectX3D::printDebug("�v���C���[�ʒu%f\n", Collision::getTransform("Player", 0)->pos.z);

		DirectX3D::printDebug("�J�����ʒuZ:%f\n", myTransform.pos.z);
		DirectX3D::printDebug("�J���������_Z:%f\n", myTransform.lookDest.z);

	}

	/*
		else
	{


		cameraPosDest = D3DXVECTOR3(0.0f,4.3f,  9.0f);
		cameraLookDest =  D3DXVECTOR3(0.0f, 0.0f, 30.0f);

		static float fcnt = 0.0f;
		fcnt += 0.001f;
		D3DXVec3Lerp(&cameraPos, &cameraPos, &cameraPosDest, fcnt);
		D3DXVec3Lerp(&cameraLook, &cameraLook, &cameraLookDest, fcnt);
	}
	*/
	myMoveType = MoveStateType::StartFade;
}

//����������������������������������������������������������������
// 
//����������������������������������������������������������������
void Camera::Rotvelocity(D3DXVECTOR3* pVecCenter, FLOAT fRadius)
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
	myTransform.pos.x = vecTarget.x;
	myTransform.pos.z = vecTarget.z;
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
		&myTransform.pos,			// �J�����̎��_
		&myTransform.look,			// �J�����̒����_
		&myTransform.up);			// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_VIEW, &mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		ViewAngle,			// ����p
		static_cast<FLOAT> (Application::ScreenWidth) / static_cast<FLOAT>(Application::ScreenHeight),		// �A�X�y�N�g��
		ViewNearZ,		// �r���[���ʂ�NearZ�l
		ViewFarZ);		// �r���[���ʂ�FarZ�l

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

	Rotvelocity(&Center, 5.0f);

	// ��]����
//	rotCnt += camera_ROT_SPEED * RightStickX;


//	D3DXVec3Normalize(&cameraForwrd, &cameraForwrd);


//	myTransform.pos.y += RightStickY;
	/*
	if (RightStickY > 0.0f)
	{
		myTransform.pos.y -= 0.1f;
	}
	else if (RightStickY < 0.0f)
	{
		myTransform.pos.y += 0.1f;
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
	return myTransform.pos;
}

//����������������������������������������
// �J���������_�擾
//����������������������������������������
D3DXVECTOR3 Camera::getLook()
{
	return myTransform.look;
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
	D3DXVECTOR3 cameraFowerd = myTransform.look - myTransform.pos;
	D3DXVec3Normalize(&cameraFowerd, &cameraFowerd);
	
	return cameraFowerd;
}

//����������������������������������������
// �J�����X�e�[�g�Z�b�g
//����������������������������������������
void Camera::setState(MoveStateType setState)
{
	myMoveType = setState;
}