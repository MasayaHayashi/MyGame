//������������������������������������������������������������������������������������������������������
// C_Player.cpp
// Author : Masaya Hayashi
//������������������������������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Player.h"
#include "../KeyBoard/Keyboard.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../MyVector3/MyVector3.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../Ball/BallObj.h"
#include "../Random/MyRandom.h"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
Player::Player()
{
	setDefaultValue();

	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, ModelPenchanPass);

	// ���ʗp�^�O�ݒ�
	tagName		= "Player";

	// ��ԏ�����
	playerStateType = PlayerState::Stop;

	// �s�񏉊���
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	score = 0;
}

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
Player::Player(D3DXVECTOR3 StartPos,UINT setNumber)
{
	// �ʒu�E�����̏����ݒ�
	myTransform.pos = (StartPos);
	myTransform.velocity = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	setDefaultValue();

	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, ModelPenchanPass);

	// ���ʗp�^�O�ݒ�
	tagName = "Player";

	// ��ԏ�����
	playerStateType = PlayerState::Stop;

	// �s�񏉊���
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	score = 0;

	idNumber = setNumber;
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
Player::~Player()
{

}

//����������������������������������������������
// �v���C���[������
//����������������������������������������������
void Player::initialize()
{
	SceneManager::SceneState currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		initializeTitle();
		break;
	case SceneManager::SceneState::SceneMain:
		if (idNumber == 0)
		{
			initializeGameMain(ModelPenchanPass);
		}
		else if(idNumber == 1)
		{
			initializeGameMain(ModelChick);
		}
		break;
	case SceneManager::SceneState::SceneResult:
		initializeResult();
	case SceneManager::SceneState::SceneSelect:
		initializeTitle();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		initializeSceneEdit();
		break;

	default:
		break;
	}

	// ���ʏ���
	score = 0;
}

//��������������������������������������������
// �v���C���[�㏈��
//��������������������������������������������
void Player::finalize()
{
	ResourceManager::destroyAllHierarchymesh();
}

//����������������������������������������������
// �v���C���[�X�V
//����������������������������������������������
void Player::update(D3DXVECTOR3 CameraForward)
{
	SceneManager::SceneState currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		updateTitle(CameraForward);
		break;
	case  SceneManager::SceneState::SceneMain:
		updateGameMain(CameraForward);
		break;
	case  SceneManager::SceneState::SceneResult:
		updateResult();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		updateSelect();
		break;
	case SceneManager::SceneState::SceneSelect:
		updateSelect();
		break;
	default:
		break;
	}
}

//����������������������������������
// �v���C���[�`��
//����������������������������������
void Player::draw()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	
	// �`��
	Pawn::drawFrame(hierarchyMeshData.pFrameRoot);

}

//����������������������������������
// ���U���g�p�v���C���[�X�V
//����������������������������������
void Player::updateResult()
{

}

//��������������������������������������������
// �V�[���^�C�g���p�v���C���[������
//��������������������������������������������
void Player::initializeTitle()
{
	// �ʒu�A�ړ��ʁA�g�嗦������
	myTransform.pos = (D3DXVECTOR3(-10.0f, -0.01f, -5.0f));

	setDefaultValue();

	// �f�o�C�X�擾
	IDirect3DDevice9 *devicePtr = DirectX3D::getDevice();

	// �e��ϐ�������
	pD3DTexture			 = nullptr;
	meshPtr				 = nullptr;
	materialBufferPtr	 = nullptr;
	numMat		 = 0;

	// X�t�@�C���̓ǂݍ���
	ResourceManager::makeModelHierarchy(hierarchyMeshData, fileName, "Player" , meshType);

	myTransform.pos.y -= hierarchyMeshData.CollitionBox.y * 2;

	// ��]
	D3DXMATRIX mRotX, mRotY, mRotZ;

	D3DXMatrixRotationY(&mRotY, myTransform.rotDeg.y);
	D3DXMatrixMultiply(&worldMtx, &worldMtx,&mRotY);

	// �g��
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, myTransform.scale.x, myTransform.scale.y, myTransform.scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// �ړ�
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	moveVector = myTransform.pos;

	moveVector.z -= 100.0f;
	moveVector.x -= 100.0f;
	moveVector.y += 0.0f;
	D3DXVec3Normalize(&moveVector, &moveVector);

	// �R���C�_�[������
	//pCollider = NEW Collider(pos, hierarchyMeshData.collitionBox);
	//pCollider->initializeCollider();

	// �\���t���O������
	//pCollider->setUsedFlg(false);

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	hierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	hierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
	hierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	hierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[2]);

	isUsed = true;

	D3DXQuaternionRotationAxis(&StartQuaternion, &getUpVec(), 0);		// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);	// �N�H�[�^�j�I�������]�s��|�����킹
}

//��������������������������������������������
// �V�[�����C���p�v���C���[������
//��������������������������������������������
void Player::initializeGameMain(CHAR *setFilePass)
{
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 180.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.scale	= D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);
	playerStateType = PlayerState::velocity;

	if (idNumber == 0)
	{
		myTransform.pos = D3DXVECTOR3(-5.0f, 5.0f, -5.0f);
	}
	else
	{
		myTransform.pos = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
	}

	Collision::registerList(&myTransform, "Player");

	setDefaultValue();

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture		  = nullptr;
	meshPtr			  = nullptr;
	materialBufferPtr = nullptr;
	numMat		 = 0;

	// X�t�@�C���̓ǂݍ���
	ResourceManager::makeModelHierarchy(hierarchyMeshData, setFilePass,"Player",meshType);

	// ���f���ʒu����
//	myTransform.pos.y -= hierarchyMeshData.CollitionBox.y * 2;

	// �g��
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, myTransform.scale.x, myTransform.scale.y, myTransform.scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(myTransform.rotDeg.y));

	// �ړ�
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	hierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);
	hierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	playerStateType = PlayerState::Stop;
	isGround	= true;
	isUsed = true;

	D3DXQuaternionRotationAxis(&StartQuaternion, &getUpVec(), 0);
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

}


//��������������������������������������������
// �V�[���G�f�B�b�g�p�v���C���[������
//��������������������������������������������
void Player::initializeSceneEdit()
{
	// �ʒu�A�ړ��ʁA�g�嗦������
	myTransform.pos		= D3DXVECTOR3(0.0f, -0.01f, -5.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.scale	= D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e��ϐ�������
	pD3DTexture			= nullptr;
	meshPtr				= nullptr;
	materialBufferPtr	= nullptr;
	numMat				= 0;

	// X�t�@�C���̓ǂݍ���
	ResourceManager::makeModelHierarchy(hierarchyMeshData, fileName,"Player",meshType);
	
	// ���f����]
	myTransform.pos.y -= hierarchyMeshData.CollitionBox.y * 2;
	
	// ��]
	D3DXMATRIX mRotX, mRotY, mRotZ;

	D3DXMatrixRotationY(&mRotX, D3DXToRadian(0));
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mRotX);

	// �g��
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, myTransform.scale.x, myTransform.scale.y, myTransform.scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// �ړ�
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	// �R���C�_�[������
//	pCollider = NEW Collider(pos, hierarchyMeshData.collitionBox);
//	pCollider->initializeCollider();

	// �\���t���O������
//	pCollider->setUsedFlg(false);

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	hierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
//	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
	hierarchyMeshData.pAnimCtrl->SetTrackDesc(2, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
//	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[0]);


}

//����������������������������������
// ���U���g�p�v���C���[������
//����������������������������������
void Player::initializeResult()
{
	myTransform.pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g�嗦�ݒ�
	myTransform.scale = D3DXVECTOR3(ScaleSize, ScaleSize, ScaleSize);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture		= nullptr;
	meshPtr			= nullptr;
	materialBufferPtr	= nullptr;
	numMat			= 0;

	// X�t�@�C���̓ǂݍ���
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
}

//����������������������������������������
// �X�e�[�^�X������
//����������������������������������������
void Player::initializeStatus()
{
	Pawn::initializeStatus();
	
	playerStateType = PlayerState::Stop;
	isGround = true;
}

//����������������������������������������
// �V�[���^�C�g���p�v���C���[�X�V
//����������������������������������������
void Player::updateTitle(D3DXVECTOR3 CameraForward)
{
	if (Keyboard::getTrigger(DIK_2))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}
/*
	if (Keyboard::getRelease(DIK_2))
	{
		int a = 0;
	}
	*/

//	C_XINPUT *pXinput = C_XINPUT::GetInstance();

//	short Xnum = pXinput->GetThumbLX();
//	short Ynum = pXinput->GetThumbLY();

	D3DXVECTOR3 CameraRight = D3DXVECTOR3(CameraForward.z,0.0f,-CameraForward.x);

//	PrintDebugProc("xxxxxxxxxxxxxxxxxxxxx:%d\n", Xnum);
//	PrintDebugProc("YYYYYYYYYYYYYYYYYYYYY:%d\n", Ynum);

//	D3DXVec3Normalize(&TestVec, &TestVec);

//	TestVec.x = CameraRight.x * CameraForward.x +Xnum;
//	TestVec.z = CameraForward.z * CameraRight.z + Ynum;

//	testVec = CameraRight * Xnum + CameraForward * Ynum;
	
	/*
	D3DXVECTOR3 UpVec = getUpVec();
	D3DXVec3Normalize(&UpVec,&UpVec);

	D3DXQuaternionRotationAxis(&quatanion, &UpVec, 0);			// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);	// �N�H�[�^�j�I�������]�s��|�����킹
	D3DXVec3Normalize(&testVec, &testVec);

	testVec.y = 0.0f;
	pos += testVec * 0.3f;
	
	D3DXVECTOR3 FowrdVec = getForwardVec();
	D3DXVECTOR3	RightVec = getRightVec();
	D3DXVECTOR3 Upvec	 = getUpVec();	
	
	radRot = MyVector3::CalcAngleDegree(testVec, -FowrdVec);
	D3DXQUATERNION quatanion;
	*/
	/*
	if (radRot == 0.0f)
	{
		D3DXQuaternionRotationAxis(&quatanion, &Upvec, oldRadRot);
		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

		StartQuaternion = quatanion;
		fCnt = 0.0f;

		PrintDebugProc("������������������������");
	}
	else
	{
		D3DXQuaternionRotationAxis(&destQua, &Upvec, radRot);


		D3DXQuaternionSlerp(&quatanion, &StartQuaternion, &destQua, 1.0f);
		fCnt += 0.1f;

		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		oldRadRot = radRot;

	}

	
	if (fCnt >= 1.0f)
		fCnt = 1.0f;
	
	PrintDebugProc("xxx%f\n", testVec.x);
	PrintDebugProc("yyy%f\n", testVec.y);
	PrintDebugProc("zzz%f\n", testVec.z);
	PrintDebugProc("ddddddddddd%f",fCnt);
	*/

	// �ʒu�X�V
	worldMtx._41 = myTransform.pos.x;
	worldMtx._42 = myTransform.pos.y;
	worldMtx._43 = myTransform.pos.z;

	updateAnimation();



	// �R���C�_�[�X�V
//	pCollider->UpdateCollider(worldMtx,DEFAULT_BOXcolor);

#if _DEBUG
//	PrintDebugProc("�Ղꂢ��`�`�`X%f\n", pos.x);
//	PrintDebugProc("�Ղꂢ��`�`�`Y%f\n", pos.y);
//	PrintDebugProc("�Ղꂢ��`�`�`Z%f\n", pos.z);
#endif

	/*
	// �}�e���A���F�ݒ�
	if (bHit)
		pCollider->figurePtr->SetMatcolor(HIT_BOX_color);
	else
		pCollider->figurePtr->SetMatcolor(DEFAULT_BOXcolor);
	*/

}

//��������������������������������������
// �V�[�����C���p�v���C���[�X�V
//��������������������������������������
void Player::updateGameMain(D3DXVECTOR3 CameraForward)
{
	Xnum -= 0.0004f;
	Ynum -= 0.0004f;

	if (Xnum <= 0.0f)
	{
		Xnum = 0.0f;
	}
	if (Ynum <= 0.0f)
	{
		Ynum = 0.0f;
	}

	updateAnimation();

	if (isHit("Ball"))
	{
		rebound(idNumber);
	}

	rideBall(idNumber);

	if (idNumber == 0 && !isHit("Ball"))
	{
		if (Keyboard::getPress(DIK_D))
		{
			Xnum += MoveSpeed;
		}
		if (Keyboard::getPress(DIK_A))
		{
			Xnum -= MoveSpeed;
		}
		if (Keyboard::getPress(DIK_S))
		{
			Ynum -= MoveSpeed;
		}
		if (Keyboard::getPress(DIK_W))
		{
			Ynum += MoveSpeed;
		}
	}
	else if (idNumber == 1 && !isHit("Ball"))
	{
		if (Keyboard::getPress(DIK_L))
		{
			Xnum += MoveSpeed;
		}
		if (Keyboard::getPress(DIK_J))
		{
			Xnum -= MoveSpeed;
		}
		if (Keyboard::getPress(DIK_K))
		{
			Ynum -= MoveSpeed;
		}
		if (Keyboard::getPress(DIK_I))
		{
			Ynum += MoveSpeed;
		}
	}

	fall(idNumber);

	D3DXVECTOR3 CameraRight = D3DXVECTOR3(CameraForward.z, 0.0f, -CameraForward.x);
	moveVector = CameraRight * Xnum + CameraForward * Ynum;

	D3DXVECTOR3 UpVec = getUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);

	D3DXQuaternionRotationAxis(&quatanion, &UpVec, 0);
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
	D3DXVec3Normalize(&moveVector, &moveVector);

	moveVector.y = 0.0f;

	myTransform.velocity += (moveVector * 0.1f);
	DirectX3D::printDebug("velocityXXXXXXXXXXXXXX%f", myTransform.velocity.x);

	Collision::setVelocity("Ball", idNumber, myTransform.velocity);

	if (myTransform.velocity.x > MaxSpeed)
	{
		myTransform.velocity.x = MaxSpeed;
	}
	if (myTransform.velocity.x < -MaxSpeed)
	{
		myTransform.velocity.x = -MaxSpeed;
	}

	if (myTransform.velocity.z > MaxSpeed)
	{
		myTransform.velocity.z = MaxSpeed;
	}
	if (myTransform.velocity.z < -MaxSpeed)
	{
		myTransform.velocity.z = -MaxSpeed;
	}

	myTransform.pos += myTransform.velocity;
	myTransform.velocity *= 0.964f;

	D3DXVECTOR3 FowrdVec = getForwardVec();
	D3DXVECTOR3	RightVec = getRightVec();
	D3DXVECTOR3 Upvec = getUpVec();

	radRot = MyVector3::CalcAngleDegree(moveVector, -FowrdVec);
	D3DXQUATERNION quatanion;

	if (radRot == 0.0f)
	{
		D3DXQuaternionRotationAxis(&quatanion, &Upvec, oldRadRot);
		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

		StartQuaternion = quatanion;
		rotCnt = 0.0f;
	}
	else
	{
		D3DXQuaternionRotationAxis(&destQua, &Upvec, radRot);

		D3DXQuaternionSlerp(&quatanion, &StartQuaternion, &destQua, rotCnt);
		rotCnt += 0.1f;

		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		oldRadRot = radRot;
	}

	if (rotCnt >= 1.0f)
	{
		rotCnt = 1.0f;
	}

	setWorldMtxPos(myTransform.pos);

#if 0

	PrintDebugProc("LangingXXX::::%f\n", DestLanding.x);
	PrintDebugProc("LangingYYY::::%f\n", DestLanding.y);
	PrintDebugProc("LangingZZZ::::%f\n", DestLanding.z);

	// �X�e�[�^�X�ύX����
	changeStatus();

	// ��ԑJ�ڔ���
//	changeState();

	if (PlayerState == PlayerState::TYPE_JUMP_UP)
	{
		bIsGround = false;
	}

	C_SCENE_MANAGER *pScene = GetSceneManager();
	GAME_STATE uCurrentState = pScene->GetInstanse()->GetGameState();

	C_XINPUT *pXinput = C_XINPUT::GetInstance();

	short Xnum = pXinput->GetThumbLX();
	short Ynum = pXinput->GetThumbLY();

	if (GetKeyboardPress(DIK_D))
		Xnum++;
	if (GetKeyboardPress(DIK_A))
		Xnum--;
	if (GetKeyboardPress(DIK_S))
		Ynum--;
	if (GetKeyboardPress(DIK_W))
		Ynum++;

	if (Xnum == 0.0f && Ynum == 0.0f)
		PlayerState = PlayerState::Stop;
	else
		PlayerState = PlayerState::TYPE_velocity;

	D3DXVECTOR3 CameraRight = D3DXVECTOR3(CameraForward.z, 0.0f, -CameraForward.x);

	D3DXVec3Normalize(&TestVec, &TestVec);

	TestVec = CameraRight * Xnum + CameraForward * Ynum;

	D3DXVECTOR3 UpVec = GetUpVec();
	D3DXVec3Normalize(&UpVec, &UpVec);

	D3DXQuaternionRotationAxis(&Quatanion, &UpVec, 0);			// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &Quatanion);	// �N�H�[�^�j�I�������]�s��|�����킹
	D3DXVec3Normalize(&TestVec, &TestVec);

	TestVec.y = 0.0f;
	pos += TestVec * 0.3f;

	D3DXVECTOR3 FowrdVec = GetForwardVec();
	D3DXVECTOR3	RightVec = GetRightVec();
	D3DXVECTOR3 Upvec = GetUpVec();

	fRadRot = MyVector3::CalcAngleDegree(TestVec, -FowrdVec);
	D3DXQUATERNION quatanion;

	if (fRadRot == 0.0f)
	{
		D3DXQuaternionRotationAxis(&quatanion, &Upvec, fOldRadRot);
		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

		StartQua = quatanion;
		fCnt = 0.0f;
	}
	else
	{
		D3DXQuaternionRotationAxis(&DestQua, &Upvec, fRadRot);

		D3DXQuaternionSlerp(&quatanion, &StartQua, &DestQua, fCnt);
		fCnt += 0.1f;

		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		fOldRadRot = fRadRot;

	}

	if (fCnt >= 1.0f)
		fCnt = 1.0f;

	// �W�����v
	if (uCurrentState == GAME_NORMAL)
		if (pXinput->GetButtonTriger(XINPUT_GAMEPAD_A) || GetKeyboardTrigger(DIK_SPACE) && bIsGround)
		{
			PlayerState = PlayerState::TYPE_JUMP_UP;
			AccelePawn.y = 0.555f;
		}

	// �d�͉��Z����
	if (!bIsGround)
	{
		AccelePawn.y = -0.025f;
	}
	// �ړ����f
	velocity += AccelePawn;

	if (velocity.y <= -0.3f)
	{
		velocity.y = -0.3f;
	}
	pos += velocity;

	if (pos.y < -6.0f && !bIsGround)
	{
		PlayerState = PlayerState::TYPE_DEAD;
	}

	worldMtx._41 = pos.x;
	worldMtx._42 = pos.y;
	worldMtx._43 = pos.z;

#if _DEBUG
	PrintDebugProc("XXX%f\n", TestVec.x);
	PrintDebugProc("YYY%f\n", TestVec.y);
	PrintDebugProc("ZZZ%f\n", TestVec.z);
	PrintDebugProc("�Ղꂢ��`�`�`X%f\n", worldMtx._41);
	PrintDebugProc("�Ղꂢ��`�`�`Y%f\n", worldMtx._42);
	PrintDebugProc("�Ղꂢ��`�`�`Z%f\n", worldMtx._43);
	PrintDebugProc("�Ղꂢ��`���傤����%d\n", PlayerState);
	PrintDebugProc("velocityY%f\n", velocity.y);
	PrintDebugProc("IsGround%d\n", bIsGround);
#endif


	// �}�e���A���F�ݒ�
	if (bHit)
		pCollider->figurePtr->SetMatcolor(HIT_BOX_color);
	else
		pCollider->figurePtr->SetMatcolor(DEFAULT_BOXcolor);


	// �t���[���X�V
	UpdateFrameMatrices(hierarchyMeshData.frameRoot, &worldMtx);

	// �A�j���[�V�����X�V
	if (hierarchyMeshData.animCtrlPtrPtr)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - hierarchyMeshData.dwPrev) / 400.0;
		hierarchyMeshData.dwPrev = dwNow;
		hierarchyMeshData.animCtrlPtrPtr->AdvanceTime(d, nullptr);
	}

#endif

}

//��������������������������������������
// �V�[���Z���N�g�p�X�V
//��������������������������������������
void Player::updateSelect()
{
	updateAnimation();

	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(myTransform.rotDeg.y));
	myTransform.rotDeg.y += 1;

	D3DXVECTOR3 cameraPos = Collision::getCameraTransform("Camera", 1)->pos;

	myTransform.pos = cameraPos;
	myTransform.pos.z += 8.0f;
	myTransform.pos.x -= 4.0f;

	rideBall(0);
	setWorldMtxPos(myTransform.pos);
}

//����������������������������������������������
// �V�[���G�f�B�b�g�p�X�V
//����������������������������������������������
void Player::updateSceneEdit()
{
#if 0


#if _DEBUG


	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(rot.y));

	// �t���[���X�V
	UpdateFrameMatrices(hierarchyMeshData.frameRoot, &worldMtx);

	// �A�j���[�V�����X�V
	if (hierarchyMeshData.animCtrlPtrPtr)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - hierarchyMeshData.dwPrev) / 1000.0;
		hierarchyMeshData.dwPrev = dwNow;
		hierarchyMeshData.animCtrlPtrPtr->AdvanceTime(d, nullptr);
	}

#endif
	// �R���C�_�[�X�V
	pCollider->UpdateCollider(worldMtx, DEFAULT_BOXcolor);

#if _DEBUG
	PrintDebugProc("�Ղꂢ��`�`�`X%f\n", worldMtx._41);
	PrintDebugProc("�Ղꂢ��`�`�`Y%f\n", worldMtx._42);
	PrintDebugProc("�Ղꂢ��`�`�`Z%f\n", worldMtx._43);
#endif

	// �}�e���A���F�ݒ�
	if (bHit)
		pCollider->figurePtr->SetMatcolor(HIT_BOX_color);
	else
		pCollider->figurePtr->SetMatcolor(DEFAULT_BOXcolor);

#endif

}

//��������������������������������������������������
// �X�e�[�^�X�擾
//��������������������������������������������������
Player::PlayerState Player::getState() const
{
	return playerStateType;
}

//��������������������������������������������������
// �X�e�[�^�X�Z�b�g
//��������������������������������������������������
void Player::setStatus(Player::PlayerState setStatus)
{
	playerStateType = setStatus;
}

//��������������������������������
// �X�R�A�Z�b�g
//��������������������������������
void Player::setScore(INT nSetNum)
{
	score = nSetNum;
}

//��������������������������������
// �X�R�A���Z
//��������������������������������
void Player::addScore()
{
	score ++;
}

//��������������������������������
// �X�R�A�擾
//��������������������������������
INT Player::getScore() const
{
	return score;
}

//��������������������������������
// �X�e�[�^�X�ύX
//��������������������������������
void Player::changeStatus()
{
	switch (playerStateType)
	{
	case PlayerState::Stop:
		break;
	case PlayerState::velocity:
		myTransform.accele	= D3DXVECTOR3( 0.0f,0.0f,0.0f);
		myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		isGround	= true;
		break;
	case PlayerState::JumpUp:
		
		if (myTransform.velocity.y < 0.0f)
			playerStateType = PlayerState::JumpDown;

		isGround = false;
	
		break;
	case PlayerState::JumpDown:

		isGround = false;

		break;
	case PlayerState::Fall:
	{
		isGround = false;
	}
		break;
	case PlayerState::Dead:
		break;
	default:
		break;
	}

}

//��������������������������������
// ��ԑJ��
//��������������������������������
void Player::changeState()
{
	if (playerStateType == PlayerState::JumpUp &&
		myTransform.velocity.y < 0.0f)
	{
		playerStateType = PlayerState::Fall;
	}
}

//��������������������������������
// 
//��������������������������������
void Player::move()
{

}

//��������������������������������
// ���˕Ԃ菈��
//��������������������������������
void Player::rebound(size_t index)
{
	const Transform* player1TransformPtr = Collision::getTransform("Player", 0);
	const Transform* player2TransformPtr = Collision::getTransform("Player", 1);
	const Transform* ball1Transform = Collision::getTransform("Ball", 0);
	const Transform* ball2Transform = Collision::getTransform("Ball", 1);

	D3DXVECTOR3 playerToPlayer		= player1TransformPtr->pos - player2TransformPtr->pos;

	playerToPlayer.y = 0.0f;
	playerToPlayer *= 0.08f;

	D3DXVECTOR3 nvpe;
	D3DXVec3Normalize(&nvpe, &playerToPlayer);

	D3DXVECTOR3 pv;
	pv = moveVector + nvpe;

	myTransform.velocity += playerToPlayer;

	Collision::setVelocity("Player", 1, -playerToPlayer);
	myTransform.velocity.y = 0.0f;

}

//����������������������������������������
// �{�[���������
//����������������������������������������
void Player::rideBall(size_t setIndex)
{
	switch (meshType)
	{
	case MeshObjType::NormalModel:
		myTransform.pos = Collision::getTransform("Ball", setIndex)->pos;
		myTransform.pos.y += meshDataObj.collitionBox.y;
		break;
	case MeshObjType::HierarchyModel:
		myTransform.pos = Collision::getTransform("Ball", setIndex)->pos;
		myTransform.pos.y += hierarchyMeshData.CollitionBox.y;
		break;
	}
}

//��������������������������������������
// ����
//��������������������������������������
void Player::fall(size_t checkIndex)
{
	if (!Collision::getRayHitData("Player", checkIndex)->isHit)
	{
		if (idNumber == checkIndex)
		{
			myTransform.velocity.y -= FallSpeed;
		}
	}
	else
	{
		if (idNumber == checkIndex)
		{
			myTransform.velocity.y = 0.0f;
		}
	}
}

void Player::input()
{
	/*
	Xnum = 0.0f;
	Ynum = 0.0f;

	if (idNumber == 0)
	{
		if (Keyboard::getPress(DIK_D))
		{
			Xnum++;
		}
		if (Keyboard::getPress(DIK_A))
		{
			Xnum--;
		}
		if (Keyboard::getPress(DIK_S))
		{
			Ynum--;
		}
		if (Keyboard::getPress(DIK_W))
		{
			Ynum++;
		}
	}
	else if (idNumber == 1)
	{
		if (Keyboard::getPress(DIK_L))
		{
			Xnum++;
		}
		if (Keyboard::getPress(DIK_J))
		{
			Xnum--;
		}
		if (Keyboard::getPress(DIK_K))
		{
			Ynum--;
		}
		if (Keyboard::getPress(DIK_I))
		{
			Ynum++;
		}
	}
	*/
}