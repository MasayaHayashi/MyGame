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
	rotvelocityCamera	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	velocityCameraDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	numvelocityPitch	= 0.0f;
	rotPitchRadian		= 0.0f;
	cameraUpDest		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	cameraFowerd		= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	changeCamera		= true;
	lerpCnt				= 0.0f;

	rotCnt = 0;

	D3DXMatrixIdentity(&myTransform.viewMtx);

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
// �f�o�b�O�p�ړ�
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

	FLOAT fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

	const Transform playerTransform = *Collision::getTransform("Player",0);

	fadePos[2] = playerTransform.pos + D3DXVECTOR3( 4.0f, playerTransform.pos.y + 0.3f, -20.0f);
	fadePos[1] = playerTransform.pos + D3DXVECTOR3(-4.0f, playerTransform.pos.y + 0.3f, -20.0f);
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
}

//����������������������������������������
// �J����������(�X�e�[�W�G�f�B�b�g)
//����������������������������������������
void Camera::initializeStageEdit()
{
	myTransform.pos = D3DXVECTOR3(0.0f, 10.0f, -19.0f);							// �J�����̎��_
	myTransform.look = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �J�����̒����_
	cameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								// �J�����̏����

	myTransform.posDest = D3DXVECTOR3(0.0f, -0.0f, -9.0f);								// �J�����̎��_�̖ړI�ʒu
	myTransform.lookDest = D3DXVECTOR3(0.0f,0.0f,0.0f);		// �J�����̒����_�̖ړI�ʒu

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	FLOAT fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���

}

//����������������������������������������
// �J����������(�Q�[�����C��)
//����������������������������������������
void Camera::initializeMain(Player *pPlayer)
{
	D3DXVECTOR3 playerPos = pPlayer->getPosition();

	fadePos[0]  = playerPos + D3DXVECTOR3(0.0f, 3.0f, -20.0f);
	fadePos[1]  = Collision::getTransform("Player", 0)->pos + D3DXVECTOR3(-5.5f,  -4.3f, 7.0f);
	fadePos[2]  = Collision::getTransform("Player", 0)->pos + D3DXVECTOR3( 2.0f,  -4.3f, 7.0f);

	myTransform.pos		= D3DXVECTOR3(0.0f, 10.0f, -20.0f);		
	myTransform.look	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	cameraUp			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);		

	myTransform.posDest		= D3DXVECTOR3(0.0f, -1.0f, -5.0f);	
	myTransform.lookDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	

	cameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					

	FLOAT fVecX, fVecZ;			// �J�������璍���_�܂ł̋���
	fVecX = myTransform.posDest.x - myTransform.look.x;
	fVecZ = myTransform.posDest.z - myTransform.look.z;

	lengthIntervalcamera = sqrt(fVecX * fVecX + fVecZ * fVecZ);		// �J�����̎��_�ƒ����_�̋���
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
void Camera::update(Player &pPlayer,INT countdown)
{
	currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		updateGameMain(pPlayer, countdown);
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
	myTransform.fowerd = myTransform.look - myTransform.pos;

	myTransform.posDest = PlayerPos - ForwardVec * 10;
	
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
}

//������������������������������������������������
// �J�����X�V(�X�e�[�W�G�f�B�b�g�V�[��)
//������������������������������������������������
void Camera::updateStageEdit(std::string keyName,UINT selectIndex)
{
	D3DXVECTOR3 blockPos = Collision::getTransform(keyName, selectIndex)->pos;
	static D3DXVECTOR3 forword = D3DXVECTOR3(0.0f,0.0f,0.0f);

	myTransform.posDest = D3DXVECTOR3(blockPos.x, blockPos.y + 40.0f, blockPos.z - 30.0f) + forword;
	myTransform.look = blockPos;

	static float fcnt = 0.0f;
	fcnt += 0.0001f;
	D3DXVec3Lerp(&myTransform.pos,  &myTransform.pos,	&myTransform.posDest,  fcnt);
	D3DXVec3Lerp(&myTransform.look, &myTransform.look,	&myTransform.lookDest, fcnt);


	D3DXVECTOR3 forwd = myTransform.pos - blockPos;
	D3DXVec3Normalize(&forwd, &forwd);

	if (Keyboard::getPress(DIK_I))
	{
		forword += forwd ;
	}

	if (Keyboard::getPress(DIK_K))
	{
		forword -= forwd ;
	}

//	rotation(blockPos, 10.0f);
//	editInput(blockPos);

	if (fcnt > 0.3f)
	{
		fcnt = 0.0f;
	}
}

//������������������������������������������������
// �J�����X�V(�Q�[�����C��)
//������������������������������������������������
void Camera::updateGameMain(Player &pPlayer,INT countDown)
{
	GameManager::GameType currentGameType = GameManager::getGameType();

	switch (currentGameType)
	{
		case GameManager::GameType::Ready:
			updateGameMainReady(pPlayer, countDown);
			break;
		case GameManager::GameType::Miss:
			updateGameMainMiss();
			break;
		case GameManager::GameType::Playing:
			updateGameMainPlay(pPlayer,countDown);
			break;
		case GameManager::GameType::FallMiss:
			updateFallMiss();
		default:
			break;
	}

}

//������������������������������������������������
// ����
//������������������������������������������������
void Camera::updateGameMainReady(Player &pPlayer, INT countDown)
{
//	myTransform.look = pPlayer.getPosition();
	D3DXVECTOR3 playerToCamera = myTransform.pos - pPlayer.getPosition();

//	myTransform.pos = myTransform.pos + playerToCamera;

	myMoveType		= MoveStateType::StartFade;

	if (countDown <= 0)
	{
		countDown = 0;
	}

	DirectX3D::printDebug("XXX%f\n", myTransform.pos.x);
	DirectX3D::printDebug("YYY%f\n", myTransform.pos.y);
	DirectX3D::printDebug("ZZZ%f\n", myTransform.pos.z);

	myTransform.pos = fadePos[countDown - 1];
	myTransform.look = Collision::getTransform("Player", 0)->pos;
}

//����������������������������������������������
// ���s
//����������������������������������������������
void Camera::updateGameMainMiss()
{
	myTransform.pos = Collision::getTransform("Player", 0)->pos - myTransform.fowerd * 20;
	myTransform.pos.y += 15.0f;
	myTransform.look = Collision::getTransform("Player", 0)->pos;
}

//����������������������������������������������
// �v���C���X�V
//����������������������������������������������
void Camera::updateGameMainPlay(Player &pPlayer,Board countDown)
{
	D3DXVECTOR3 PlayerPos		= pPlayer.getOffset();
	D3DXVECTOR3 ForwardVec		= pPlayer.getForwardVec();
	D3DXVECTOR3 UpVec			= pPlayer.getUpVec();

	D3DXVec3Normalize(&UpVec, &UpVec);
	D3DXVec3Normalize(&ForwardVec, &ForwardVec);
/*
	if (GameManager::isGameType(GameManager::GameType::Ready))
	{
		myTransform.pos = pPlayer.getPosition();
		myTransform.pos.y += 2.0f;
		myTransform.pos.z -= 10.0f;
		myTransform.look = pPlayer.getPosition();
	}*/



	myTransform.posDest = Collision::getTransform("Player", 0)->pos - *D3DXVec3Normalize(&cameraFowerd, &cameraFowerd) * 6.0f;
	myTransform.posDest.y += 4.0f;
	myTransform.lookDest = Collision::getTransform("Player", 0)->pos + *D3DXVec3Normalize(&cameraFowerd, &cameraFowerd) * 10.0f;

	lerpCnt += 0.02f;

	if (lerpCnt > 1.0f)
	{
		lerpCnt = 1.0f;
	}

	D3DXVec3Lerp(&myTransform.pos, &myTransform.pos, &myTransform.posDest, lerpCnt);
	D3DXVec3Lerp(&myTransform.look, &myTransform.look, &myTransform.lookDest, lerpCnt);

	DirectX3D::printDebug("�v���C���[�ʒu%f\n", Collision::getTransform("Player", 0)->pos.z);

	DirectX3D::printDebug("�J�����ʒuZ:%f\n", myTransform.pos.z);
	DirectX3D::printDebug("�J���������_Z:%f\n", myTransform.lookDest.z);

}

//����������������������������������������������������������������
// ��]
//����������������������������������������������������������������
void Camera::rotation(D3DXVECTOR3* pVecCenter, FLOAT fRadius)
{
	D3DXMATRIX matRotation;	// �|�����킹�p��]�s��
	INT rot = rotCnt / 1000;

	//���_�ɔ��a�𑫂��������̍��W��p��
	D3DXVECTOR3 vecTarget(-fRadius, 0.0f, -fRadius);

	//���_�𒆐S�Ƃ�����]�i�I�C���[��]�j�̍s������
	D3DXMatrixRotationY(&matRotation, static_cast<FLOAT>(rot));
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
	D3DXMatrixIdentity(&myTransform.viewMtx);
	
	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&myTransform.viewMtx,
		&myTransform.pos,			// �J�����̎��_
		&myTransform.look,			// �J�����̒����_
		&myTransform.up);			// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_VIEW, &myTransform.viewMtx);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&myTransform.projectionMtx);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&myTransform.projectionMtx,
		ViewAngle,			// ����p
		static_cast<FLOAT> (Application::ScreenWidth) / static_cast<FLOAT>(Application::ScreenHeight),		// �A�X�y�N�g��
		ViewNearZ,		// �r���[���ʂ�NearZ�l
		ViewFarZ);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_PROJECTION, &myTransform.projectionMtx);
}

//��������������������������������������������������
// �J������]
//��������������������������������������������������
void Camera::rotationCamera(D3DXVECTOR3 Center)
{

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
	return myTransform.viewMtx;
}

//����������������������������������������������������
// �v���W�F�N�V�����}�g���b�N�X�擾
//����������������������������������������������������
D3DXMATRIX Camera::getProjectionMtx()
{
	return myTransform.projectionMtx;
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

//����������������������������������������
// ��]
//����������������������������������������
void Camera::rotation(D3DXVECTOR3 center, FLOAT radius)
{
	D3DXMATRIX matRotation;	// �|�����킹�p��]�s��
	FLOAT rot = rotCnt / 1000.0f;

	//���_�ɔ��a�𑫂��������̍��W��p��
	D3DXVECTOR3 vecTarget(-radius, 0.0f, -radius);

	//���_�𒆐S�Ƃ�����]�i�I�C���[��]�j�̍s������
	D3DXMatrixRotationY(&matRotation, rot);
	D3DXVec3TransformCoord(&vecTarget, &vecTarget, &matRotation);

	//�Ō�ɖ{���̍��W�i��]�Ώۂ̍��W�j�𑫂����킹
	D3DXVec3Add(&vecTarget, &vecTarget, &center);
	myTransform.pos.x = vecTarget.x;
	myTransform.pos.z = vecTarget.z;

	rotCnt++;
}

//��������������������������������
// �X�e�[�^�X������
//��������������������������������
void Camera::initializeStatus()
{
	lerpCnt = 0;
}

//��������������������������������
// �J�����p����
//��������������������������������
void Camera::editInput(D3DXVECTOR3 blockPos)
{
	D3DXVECTOR3 forwd = myTransform.pos - blockPos;
	D3DXVec3Normalize(&forwd, &forwd);

	if (Keyboard::getPress(DIK_I))
	{
		myTransform.look += forwd;
	}

	if (Keyboard::getPress(DIK_K))
	{
		myTransform.look -= forwd;
	}

}

//��������������������������������
// ����
//��������������������������������
void Camera::updateFallMiss()
{
	myTransform.look = Collision::getTransform("Player", 0)->pos;

}