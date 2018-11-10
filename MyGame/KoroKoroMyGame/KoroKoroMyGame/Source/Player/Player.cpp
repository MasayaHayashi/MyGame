//������������������������������������������������������������������������������������������������������
// C_Player.cpp
// Author : Masaya Hayashi
//������������������������������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Player.h"
#include "../KeyBoard/Keyboard.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"

/*
#include "C_Collider.h"
#include "C_MyVector3.h"
#include "collision.h"
#include "main.h"
#include "C_Xinput.h"
*/

// ===== �萔�E�}�N����` =====
#define MOVE_FORWARD_SPEED  (0.45f)		// �O�ւ̈ړ���
#define MOVE_SIDE_SPEED		(0.3f)		// ���ւ̈ړ���
#define VALUE_SCALE_SIZE	(1.0f)		// �g�嗦

#define BLOCK_POS_Y (1.84f)

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
Player::Player()
{
	// �ʒu�E�����̏����ݒ�
	pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	// �g�嗦�ݒ�
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	setDefaultValue();

	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, ModelFilePass);

	// ���ʗp�^�O�ݒ�
	tagName		= TagType::Player;

	// ��ԏ�����
	playerStateType = PlayerState::Stop;

	// �s�񏉊���
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	nScore = 0;
	OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
		initializeGameMain();
		break;
	case SceneManager::SceneState::SceneResult:
		initializeResult();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		initializeSceneEdit();
		break;

	default:
		break;
	}

	// ���ʏ���
	nScore = 0;
}

//��������������������������������������������
// �v���C���[�㏈��
//��������������������������������������������
void Player::finalize()
{
	// �R���C�_�[���
//	SAFE_DELETE(pCollider);

	// ���b�V�����
	destroyModelHierarchy();
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
		UpdatePlayer_GameMain(CameraForward);
		break;
	case  SceneManager::SceneState::SceneResult:
		updateResult();
		break;
	case  SceneManager::SceneState::SceneStageEdit:
		updateTitle(CameraForward);
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
	const IDirect3DDevice9* devicePtr = DirectX3D::getDevice();
	
	// �`��
	Pawn::drawFrame(hierarchyMeshData.frameRoot);

	// �R���C�_�[�`��
//	pCollider->DrawCollider();
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
	pos		= D3DXVECTOR3(0.0f, -0.01f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);

	setDefaultValue();

	// �f�o�C�X�擾
	IDirect3DDevice9 * devicePtr = DirectX3D::getDevice();

	// �e��ϐ�������
	pD3DTexture  = nullptr;
	pD3DXMesh	 = nullptr;
	pD3DXBuffMat = nullptr;
	uNumMat		 = 0;

	// X�t�@�C���̓ǂݍ���
	ResourceManager::makeModelHierarchy(hierarchyMeshData, fileName, meshType);

	pos.y -= hierarchyMeshData.collitionBox.y * 2;

	// ��]
	D3DXMATRIX mRotX, mRotY, mRotZ;

	D3DXMatrixRotationY(&mRotY, rot.y);
	D3DXMatrixMultiply(&worldMtx, &worldMtx,&mRotY);

	// �g��
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// �ړ�
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	// �R���C�_�[������
	//pCollider = NEW Collider(pos, hierarchyMeshData.collitionBox);
	//pCollider->initializeCollider();

	// �\���t���O������
	//pCollider->setUsedFlg(false);

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	hierarchyMeshData.animCtrlPtr->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
	hierarchyMeshData.animCtrlPtr->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[2]);


	isUsed = true;

	TestVec = pos;


	TestVec.z -= 100.0f;
	TestVec.x -= 100.0f;
	TestVec.y += 0.0f;
	D3DXVec3Normalize(&TestVec, &TestVec);



	D3DXQuaternionRotationAxis(&StartQua, &getUpVec(), 0);		// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);	// �N�H�[�^�j�I�������]�s��|�����킹
}

//��������������������������������������������
// �V�[�����C���p�v���C���[������
//��������������������������������������������
void Player::initializeGameMain()
{
	// �ʒu�A�ړ��ʁA�g�嗦������
	pos		= D3DXVECTOR3(0.0f, 1.84f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 180.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);
	playerStateType = PlayerState::Move;

	setDefaultValue();

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e��ϐ�������
	pD3DTexture  = nullptr;
	pD3DXMesh	 = nullptr;
	pD3DXBuffMat = nullptr;
	uNumMat		 = 0;

	// X�t�@�C���̓ǂݍ���
	ResourceManager::makeModelHierarchy(hierarchyMeshData, fileName,meshType);
//	ResourceManager::CreateTexture(TextureData, texFileName);

	// ���f���ʒu����
	pos.y -= hierarchyMeshData.collitionBox.y * 2;

	// �g��
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(rot.y));

	// �ړ�
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	// �R���C�_�[������
//	pCollider = new Collider(pos, hierarchyMeshData.collitionBox);
//	pCollider->initializeCollider(pos, hierarchyMeshData.collitionBox, centerPos);

	// �\���t���O������
//	pCollider->setUsedFlg(false);

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	hierarchyMeshData.animCtrlPtr->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[1]);
	hierarchyMeshData.animCtrlPtr->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[1]);

	playerStateType = PlayerState::Stop;
	isGround	= true;

	/*

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	hierarchyMeshData.animCtrlPtrPtr->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	hierarchyMeshData.animCtrlPtrPtr->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
	hierarchyMeshData.animCtrlPtrPtr->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	hierarchyMeshData.animCtrlPtrPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[2]);

	*/

	isUsed = true;

	TestVec = pos;


	TestVec.z -= 100.0f;
	TestVec.x -= 100.0f;
	TestVec.y += 0.0f;
	D3DXVec3Normalize(&TestVec, &TestVec);



	D3DXQuaternionRotationAxis(&StartQua, &getUpVec(), 0);		// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);	// �N�H�[�^�j�I�������]�s��|�����킹

}


//��������������������������������������������
// �V�[���G�f�B�b�g�p�v���C���[������
//��������������������������������������������
void Player::initializeSceneEdit()
{
	// �ʒu�A�ړ��ʁA�g�嗦������
	pos		= D3DXVECTOR3(0.0f, -0.01f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e��ϐ�������
	pD3DTexture		= nullptr;
	pD3DXMesh		= nullptr;
	pD3DXBuffMat	= nullptr;
	uNumMat			= 0;

	// X�t�@�C���̓ǂݍ���
	ResourceManager::makeModelHierarchy(hierarchyMeshData, fileName,meshType);
//	ResourceManager::CreateTexture(TextureData, texFileName);
	
	// ���f����]
	pos.y -= hierarchyMeshData.collitionBox.y * 2;
	
	// ��]
	D3DXMATRIX mRotX, mRotY, mRotZ;

	D3DXMatrixRotationY(&mRotX, D3DXToRadian(0));
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mRotX);

	// �g��
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// �ړ�
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translate);

	// �R���C�_�[������
//	pCollider = NEW Collider(pos, hierarchyMeshData.collitionBox);
//	pCollider->initializeCollider();

	// �\���t���O������
//	pCollider->setUsedFlg(false);


	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	hierarchyMeshData.animCtrlPtr->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(1, hierarchyMeshData.ppAnimSet[0]);
	hierarchyMeshData.animCtrlPtr->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[0]);
}

//����������������������������������
// ���U���g�p�v���C���[������
//����������������������������������
void Player::initializeResult()
{
	pos		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g�嗦�ݒ�
	scale = D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture		= nullptr;
	pD3DXMesh		= nullptr;
	pD3DXBuffMat	= nullptr;
	uNumMat			= 0;

	// X�t�@�C���̓ǂݍ���
	makeModel();
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
#if 0
	C_XINPUT *pXinput = C_XINPUT::GetInstance();

	short Xnum = pXinput->GetThumbLX();
	short Ynum = pXinput->GetThumbLY();

	D3DXVECTOR3 CameraRight = D3DXVECTOR3(CameraForward.z,0.0f,-CameraForward.x);

	PrintDebugProc("xxxxxxxxxxxxxxxxxxxxx:%d\n", Xnum);
	PrintDebugProc("YYYYYYYYYYYYYYYYYYYYY:%d\n", Ynum);

	D3DXVec3Normalize(&TestVec, &TestVec);

//	TestVec.x = CameraRight.x * CameraForward.x +Xnum;
//	TestVec.z = CameraForward.z * CameraRight.z + Ynum;

	TestVec = CameraRight * Xnum + CameraForward * Ynum;
	
	D3DXVECTOR3 UpVec = GetUpVec();
	D3DXVec3Normalize(&UpVec,&UpVec);

	D3DXQuaternionRotationAxis(&Quatanion, &UpVec, 0);			// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &Quatanion);	// �N�H�[�^�j�I�������]�s��|�����킹
	D3DXVec3Normalize(&TestVec, &TestVec);

	TestVec.y = 0.0f;
	pos += TestVec * 0.3f;
	
	D3DXVECTOR3 FowrdVec = GetForwardVec();
	D3DXVECTOR3	RightVec = GetRightVec();
	D3DXVECTOR3 Upvec	 = GetUpVec();	
	
	fRadRot = MyVector3::CalcAngleDegree(TestVec, -FowrdVec);
	D3DXQUATERNION quatanion;

	if (fRadRot == 0.0f)
	{
		D3DXQuaternionRotationAxis(&quatanion, &Upvec, fOldRadRot);
		D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);

		StartQua = quatanion;
		fCnt = 0.0f;

		PrintDebugProc("������������������������");
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

	PrintDebugProc("xxx%f\n", TestVec.x);
	PrintDebugProc("yyy%f\n", TestVec.y);
	PrintDebugProc("zzz%f\n", TestVec.z);

	PrintDebugProc("ddddddddddd%f",fCnt);

	// �ʒu�X�V
	worldMtx._41 = pos.x;
	worldMtx._42 = pos.y;
	worldMtx._43 = pos.z;

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



	// �R���C�_�[�X�V
	pCollider->UpdateCollider(worldMtx,DEFAULT_BOXcolor);

#if _DEBUG
	PrintDebugProc("�Ղꂢ��`�`�`X%f\n", pos.x);
	PrintDebugProc("�Ղꂢ��`�`�`Y%f\n", pos.y);
	PrintDebugProc("�Ղꂢ��`�`�`Z%f\n", pos.z);
#endif

	// �}�e���A���F�ݒ�
	if (bHit)
		pCollider->figurePtr->SetMatcolor(HIT_BOX_color);
	else
		pCollider->figurePtr->SetMatcolor(DEFAULT_BOXcolor);
#endif

}

//��������������������������������������
// �V�[�����C���p�v���C���[�X�V
//��������������������������������������
void Player::UpdatePlayer_GameMain(D3DXVECTOR3 CameraForward)
{
#if 0

	PrintDebugProc("LangingXXX::::%f\n", DestLanding.x);
	PrintDebugProc("LangingYYY::::%f\n", DestLanding.y);
	PrintDebugProc("LangingZZZ::::%f\n", DestLanding.z);

	// �X�e�[�^�X�ύX����
	ChangeStatus();

	// ��ԑJ�ڔ���
//	ChangeState();

	if(PlayerState == PlayerState::TYPE_JUMP_UP)
		bIsGround = false;

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

	if(Xnum == 0.0f && Ynum == 0.0f)
		PlayerState = PlayerState::Stop;
	else
		PlayerState = PlayerState::TYPE_MOVE;





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
	D3DXVECTOR3 Upvec	 = GetUpVec();

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
	if(uCurrentState == GAME_NORMAL)
		if ( pXinput->GetButtonTriger(XINPUT_GAMEPAD_A) || GetKeyboardTrigger(DIK_SPACE) && bIsGround)
		{
			PlayerState = PlayerState::TYPE_JUMP_UP;
			AccelePawn.y = 0.555f;
		}

	// �d�͉��Z����
	if (!bIsGround)
		AccelePawn.y = -0.025f;

	// �ړ����f
	move += AccelePawn;

	if (move.y <= -0.3f)
		move.y = -0.3f;

	pos += move;

	if (pos.y < -6.0f && !bIsGround)
		PlayerState = PlayerState::TYPE_DEAD;

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
	PrintDebugProc("moveY%f\n", move.y);
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

//����������������������������������������������
// �V�[���G�f�B�b�g�p�X�V
//����������������������������������������������
void Player::UpdatePlayer_SceneEdit()
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
Player::PlayerState Player::getState()
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
void Player::SetScore(INT nSetNum)
{
	nScore = nSetNum;
}

//��������������������������������
// �X�R�A���Z
//��������������������������������
void Player::AddScore()
{
	nScore ++;
}

//��������������������������������
// �X�R�A�擾
//��������������������������������
INT Player::GetScore()
{
	return nScore;
}

//��������������������������������
// �X�e�[�^�X�ύX
//��������������������������������
void Player::ChangeStatus()
{
	switch (playerStateType)
	{
	case PlayerState::Stop:
		break;
	case PlayerState::Move:
		accele	= D3DXVECTOR3( 0.0f,0.0f,0.0f);
		move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		isGround	= true;
		break;
	case PlayerState::JumpUp:
		
		if (move.y < 0.0f)
			playerStateType = PlayerState::JumpDown;

		isGround = false;
	
		break;
	case PlayerState::JumpDown:

		isGround = false;

		break;
	case PlayerState::Fall:
	{
		isGround = false;

	//	PlayerState = TYPE_MOVE;
		/*
		pos = DestLanding;
		bIsGround = true;
		AccelePawn.y = 0.0f;
		move.y = 0.0f;
		PlayerState = TYPE_MOVE;
		*/
	}
		break;
	case PlayerState::Dead:
		break;
	default:
		break;
	}

	// ���ʏ���

}

//��������������������������������
// ��ԑJ��
//��������������������������������
void Player::ChangeState()
{
	if (playerStateType == PlayerState::JumpUp &&
		move.y < 0.0f)
	{
		playerStateType = PlayerState::Fall;
	}
}

//��������������������������������������
// �v���C���[�ړ������x�N�g���擾
//��������������������������������������
D3DXVECTOR3 Player::getMoveVec()
{
	return TestVec;
}