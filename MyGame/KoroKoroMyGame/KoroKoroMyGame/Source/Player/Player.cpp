//������������������������������������������������������������������������������������������������������
// C_Player.cpp
// Author : Masaya Hayashi
//������������������������������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Player.h"
#include "../KeyBoard/Keyboard.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"

#include "C_Collider.h"
#include "C_MyVector3.h"
#include "collision.h"
#include "main.h"
#include "C_Xinput.h"

// ===== �萔�E�}�N����` =====
#define MODEL_NAME	 "data/MODEL/PenguinC.x"
#define MOVE_FORWARD_SPEED  (0.45f)		// �O�ւ̈ړ���
#define MOVE_SIDE_SPEED		(0.3f)		// ���ւ̈ړ���
#define VALUE_SCALE_SIZE	(1.0f)		// �g�嗦

#define BLOCK_POS_Y (1.84f)

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
C_PLAYER::C_PLAYER()
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
	strcpy_s(fileName, MODEL_NAME);

	// ���ʗp�^�O�ݒ�
	tagName		= TagType::Player;

	// ��ԏ�����
	PlayerState = PLAYER_STATE::TYPE_STOP;

	// �s�񏉊���
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	nScore = 0;
	OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
C_PLAYER::~C_PLAYER()
{

}

//����������������������������������������������
// �v���C���[������
//����������������������������������������������
void C_PLAYER::InitObject()
{
	SceneManager::SceneState currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		InitPlayer_Title();
		break;
	case SceneManager::SceneState::SceneMain:
		InitPlayer_GameMain();
		break;
	case SceneManager::SceneState::SceneResult:
		InitPlayer_Result();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		InitPlayer_SceneEdit();
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
void C_PLAYER::UninitObject()
{
	// �R���C�_�[���
	SAFE_DELETE(pCollider);

	// ���b�V�����
	destroyModelHierarchy();
}

//����������������������������������������������
// �v���C���[�X�V
//����������������������������������������������
void C_PLAYER::UpdateObject(D3DXVECTOR3 CameraForward)
{
	SceneManager::SceneState currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		UpdatePlayer_Title(CameraForward);
		break;
	case  SceneManager::SceneState::SceneMain:
		UpdatePlayer_GameMain(CameraForward);
		break;
	case  SceneManager::SceneState::SceneResult:
		UpdatePlayer_Result();
		break;
	case  SceneManager::SceneState::SceneStageEdit:
		UpdatePlayer_Title(CameraForward);
		break;
	default:
		break;
	}
}

//����������������������������������
// �v���C���[�`��
//����������������������������������
void C_PLAYER::DrawObject()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();
	
	// �`��
	Pawn::drawFrame(hierarchyMeshData.pFrameRoot);

	// �R���C�_�[�`��
	pCollider->DrawCollider();
}

//����������������������������������
// ���U���g�p�v���C���[�X�V
//����������������������������������
void C_PLAYER::UpdatePlayer_Result()
{

}

//��������������������������������������������
// �V�[���^�C�g���p�v���C���[������
//��������������������������������������������
void C_PLAYER::InitPlayer_Title()
{
	// �ʒu�A�ړ��ʁA�g�嗦������
	pos		= D3DXVECTOR3(0.0f, -0.01f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);

	SetDefaultValue();

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e��ϐ�������
	pD3DTexturePawn  = nullptr;
	pD3DXMesh	 = nullptr;
	pD3DXBuffMatPawn = nullptr;
	uNumMatPawn		 = 0;

	// X�t�@�C���̓ǂݍ���
	C_RESOURCE_MANAGER::MakeModelHierarchy(HierarchyMeshData, fileName, uMeshType);

	pos.y -= HierarchyMeshData.CollitionBox.y * 2;

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
	pCollider = NEW Collider(pos, HierarchyMeshData.CollitionBox);
	pCollider->InitCollider();

	// �\���t���O������
	pCollider->SetUsedFlg(false);

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	HierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, HierarchyMeshData.ppAnimSet[0]);
	HierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, HierarchyMeshData.ppAnimSet[2]);


	bUsed = true;

	TestVec = pos;


	TestVec.z -= 100.0f;
	TestVec.x -= 100.0f;
	TestVec.y += 0.0f;
	D3DXVec3Normalize(&TestVec, &TestVec);



	D3DXQuaternionRotationAxis(&StartQua, &GetUpVec(), 0);		// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &Quatanion);	// �N�H�[�^�j�I�������]�s��|�����킹
}

//��������������������������������������������
// �V�[�����C���p�v���C���[������
//��������������������������������������������
void C_PLAYER::InitPlayer_GameMain()
{
	// �ʒu�A�ړ��ʁA�g�嗦������
	pos		= D3DXVECTOR3(0.0f, 1.84f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 180.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);
	PlayerState = PLAYER_STATE::TYPE_MOVE;

	SetDefaultValue();

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e��ϐ�������
	pD3DTexturePawn  = nullptr;
	pD3DXMesh	 = nullptr;
	pD3DXBuffMatPawn = nullptr;
	uNumMatPawn		 = 0;

	// X�t�@�C���̓ǂݍ���
	C_RESOURCE_MANAGER::MakeModelHierarchy(HierarchyMeshData, fileName,uMeshType);
//	C_RESOURCE_MANAGER::CreateTexture(TextureData, szTexFileName);

	// ���f���ʒu����
	pos.y -= HierarchyMeshData.CollitionBox.y * 2;

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
	pCollider = NEW Collider(pos, HierarchyMeshData.CollitionBox);
	pCollider->InitCollider(pos, HierarchyMeshData.CollitionBox, CenterPos);

	// �\���t���O������
	pCollider->SetUsedFlg(false);

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	HierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, HierarchyMeshData.ppAnimSet[1]);
	HierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, HierarchyMeshData.ppAnimSet[1]);

	PlayerState = PLAYER_STATE::TYPE_STOP;
	bIsGround	= true;

	/*

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	HierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, HierarchyMeshData.ppAnimSet[0]);
	HierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, HierarchyMeshData.ppAnimSet[2]);

	*/

	bUsed = true;

	TestVec = pos;


	TestVec.z -= 100.0f;
	TestVec.x -= 100.0f;
	TestVec.y += 0.0f;
	D3DXVec3Normalize(&TestVec, &TestVec);



	D3DXQuaternionRotationAxis(&StartQua, &GetUpVec(), 0);		// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &Quatanion);	// �N�H�[�^�j�I�������]�s��|�����킹

}


//��������������������������������������������
// �V�[���G�f�B�b�g�p�v���C���[������
//��������������������������������������������
void C_PLAYER::InitPlayer_SceneEdit()
{
	// �ʒu�A�ړ��ʁA�g�嗦������
	pos		= D3DXVECTOR3(0.0f, -0.01f, -5.0f);
	move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale	= D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e��ϐ�������
	pD3DTexturePawn		= nullptr;
	pD3DXMesh		= nullptr;
	pD3DXBuffMatPawn	= nullptr;
	uNumMatPawn			= 0;

	// X�t�@�C���̓ǂݍ���
	C_RESOURCE_MANAGER::MakeModelHierarchy(HierarchyMeshData, fileName,uMeshType);
//	C_RESOURCE_MANAGER::CreateTexture(TextureData, szTexFileName);
	
	// ���f����]
	pos.y -= HierarchyMeshData.CollitionBox.y * 2;
	
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
	pCollider = NEW Collider(pos, HierarchyMeshData.CollitionBox);
	pCollider->InitCollider();

	// �\���t���O������
	pCollider->SetUsedFlg(false);


	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	HierarchyMeshData.pAnimCtrl->GetTrackDesc(0, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(1, HierarchyMeshData.ppAnimSet[0]);
	HierarchyMeshData.pAnimCtrl->SetTrackDesc(1, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	HierarchyMeshData.pAnimCtrl->SetTrackAnimationSet(0, HierarchyMeshData.ppAnimSet[0]);
}

//����������������������������������
// ���U���g�p�v���C���[������
//����������������������������������
void C_PLAYER::InitPlayer_Result()
{
	pos		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot		 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g�嗦�ݒ�
	scale = D3DXVECTOR3(VALUE_SCALE_SIZE, VALUE_SCALE_SIZE, VALUE_SCALE_SIZE);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pD3DTexturePawn		= nullptr;
	pD3DXMesh		= nullptr;
	pD3DXBuffMatPawn	= nullptr;
	uNumMatPawn			= 0;

	// X�t�@�C���̓ǂݍ���
	MakeModel();
}

//����������������������������������������
// �X�e�[�^�X������
//����������������������������������������
void C_PLAYER::InitStatus()
{
	C_3DPAWN::InitStatus();
	
	PlayerState = PLAYER_STATE::TYPE_STOP;
	bIsGround = true;
}

//����������������������������������������
// �V�[���^�C�g���p�v���C���[�X�V
//����������������������������������������
void C_PLAYER::UpdatePlayer_Title(D3DXVECTOR3 CameraForward)
{
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
	
	fRadRot = C_MYVECTOR3::CalcAngleDegree(TestVec, -FowrdVec);
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

//	D3DXVec3Normalize(&TestVec, &TestVec);
//	D3DXVec3Normalize(&FowrdVec, &FowrdVec);


	/*

	FLOAT TestLength	= D3DXVec3Length(&TestVec);
	FLOAT FowedLength	= D3DXVec3Length(&FowrdVec);
	FLOAT RightLength	= D3DXVec3Length(&RightVec);

//	FLOAT fLength = sqrtf(TestVec.x * TestVec.x + FowrdVec.z * FowrdVec.z);
//	D3DXVec3Cross()

	FLOAT TestDot = D3DXVec3Dot(&TestVec, &FowrdVec);
	FLOAT RightDot = D3DXVec3Dot(&TestVec, &RightVec);

	TestDot /= (TestLength * FowedLength);
	FLOAT test		= acos(TestDot);
	test = D3DXToDegree(test);

	D3DXQUATERNION quatanion;
	
	if (RightDot >= 0.0f)
	{
		// �ω��l����������΍X�V���Ȃ�
		if (D3DXToDegree(RightDot) > 20.0f)
		{
			D3DXQuaternionRotationAxis(&quatanion, &GetUpVec(), D3DXToRadian(test));
			D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		}
	}
	else
	{
//		RightDot *= -1;

		// �ω��l����������Ώ������Ȃ�
		if (D3DXToDegree(RightDot) > 20.0f)
		{
			D3DXQuaternionRotationAxis(&quatanion, &GetUpVec(), D3DXToRadian(-test));
			D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);
		}
	}


	PrintDebugProc("xxxx%f\n", FowrdVec.x);
	PrintDebugProc("yyyy%f\n", FowrdVec.y);
	PrintDebugProc("zzzz%f\n", FowrdVec.z);

	PrintDebugProc("RightDot%f\n", D3DXToDegree(test));


	PrintDebugProc("��]�p�x:%f", test);


	*/

	// �ʒu�X�V
	worldMtx._41 = pos.x;
	worldMtx._42 = pos.y;
	worldMtx._43 = pos.z;

//	FLOAT test = acos(TestDot);
	
//	test = D3DXToDegree(test);

	// �t���[���X�V
	UpdateFrameMatrices(HierarchyMeshData.pFrameRoot, &worldMtx);

	// �A�j���[�V�����X�V
	if (HierarchyMeshData.pAnimCtrl)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - HierarchyMeshData.dwPrev) / 1000.0;
		HierarchyMeshData.dwPrev = dwNow;
		HierarchyMeshData.pAnimCtrl->AdvanceTime(d, nullptr);
	}



	// �R���C�_�[�X�V
	pCollider->UpdateCollider(worldMtx,DEFAULT_BOXCOLOR);

#if _DEBUG
	PrintDebugProc("�Ղꂢ��`�`�`X%f\n", pos.x);
	PrintDebugProc("�Ղꂢ��`�`�`Y%f\n", pos.y);
	PrintDebugProc("�Ղꂢ��`�`�`Z%f\n", pos.z);
#endif

	// �}�e���A���F�ݒ�
	if (bHit)
		pCollider->figurePtr->SetMatColor(HIT_BOX_COLOR);
	else
		pCollider->figurePtr->SetMatColor(DEFAULT_BOXCOLOR);

}

//��������������������������������������
// �V�[�����C���p�v���C���[�X�V
//��������������������������������������
void C_PLAYER::UpdatePlayer_GameMain(D3DXVECTOR3 CameraForward)
{
	PrintDebugProc("LangingXXX::::%f\n", DestLanding.x);
	PrintDebugProc("LangingYYY::::%f\n", DestLanding.y);
	PrintDebugProc("LangingZZZ::::%f\n", DestLanding.z);

	// �X�e�[�^�X�ύX����
	ChangeStatus();

	// ��ԑJ�ڔ���
//	ChangeState();

	if(PlayerState == PLAYER_STATE::TYPE_JUMP_UP)
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
		PlayerState = PLAYER_STATE::TYPE_STOP;
	else
		PlayerState = PLAYER_STATE::TYPE_MOVE;





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

	fRadRot = C_MYVECTOR3::CalcAngleDegree(TestVec, -FowrdVec);
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
			PlayerState = PLAYER_STATE::TYPE_JUMP_UP;
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
		PlayerState = PLAYER_STATE::TYPE_DEAD;

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
	PrintDebugProc("MovePawnY%f\n", move.y);
	PrintDebugProc("IsGround%d\n", bIsGround);
#endif


	// �}�e���A���F�ݒ�
	if (bHit)
		pCollider->figurePtr->SetMatColor(HIT_BOX_COLOR);
	else
		pCollider->figurePtr->SetMatColor(DEFAULT_BOXCOLOR);


	// �t���[���X�V
	UpdateFrameMatrices(HierarchyMeshData.pFrameRoot, &worldMtx);

	// �A�j���[�V�����X�V
	if (HierarchyMeshData.pAnimCtrl)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - HierarchyMeshData.dwPrev) / 400.0;
		HierarchyMeshData.dwPrev = dwNow;
		HierarchyMeshData.pAnimCtrl->AdvanceTime(d, nullptr);
	}
}

//����������������������������������������������
// �V�[���G�f�B�b�g�p�X�V
//����������������������������������������������
void C_PLAYER::UpdatePlayer_SceneEdit()
{

#if _DEBUG


	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(rot.y));

	// �t���[���X�V
	UpdateFrameMatrices(HierarchyMeshData.pFrameRoot, &worldMtx);

	// �A�j���[�V�����X�V
	if (HierarchyMeshData.pAnimCtrl)
	{
		DWORD dwNow = timeGetTime();
		DOUBLE d = (dwNow - HierarchyMeshData.dwPrev) / 1000.0;
		HierarchyMeshData.dwPrev = dwNow;
		HierarchyMeshData.pAnimCtrl->AdvanceTime(d, nullptr);
	}

#endif
	// �R���C�_�[�X�V
	pCollider->UpdateCollider(worldMtx, DEFAULT_BOXCOLOR);

#if _DEBUG
	PrintDebugProc("�Ղꂢ��`�`�`X%f\n", worldMtx._41);
	PrintDebugProc("�Ղꂢ��`�`�`Y%f\n", worldMtx._42);
	PrintDebugProc("�Ղꂢ��`�`�`Z%f\n", worldMtx._43);
#endif

	// �}�e���A���F�ݒ�
	if (bHit)
		pCollider->figurePtr->SetMatColor(HIT_BOX_COLOR);
	else
		pCollider->figurePtr->SetMatColor(DEFAULT_BOXCOLOR);

}

//��������������������������������������������������
// �X�e�[�^�X�擾
//��������������������������������������������������
PLAYER_STATE::PLAYER_STATE C_PLAYER::GetState()
{
	return PlayerState;
}

//��������������������������������������������������
// �X�e�[�^�X�Z�b�g
//��������������������������������������������������
void C_PLAYER::SetStatus(PLAYER_STATE::PLAYER_STATE SetStatus)
{
	PlayerState = SetStatus;
}

//��������������������������������
// �X�R�A�Z�b�g
//��������������������������������
void C_PLAYER::SetScore(INT nSetNum)
{
	nScore = nSetNum;
}

//��������������������������������
// �X�R�A���Z
//��������������������������������
void C_PLAYER::AddScore()
{
	nScore ++;
}

//��������������������������������
// �X�R�A�擾
//��������������������������������
INT C_PLAYER::GetScore()
{
	return nScore;
}

//��������������������������������
// �X�e�[�^�X�ύX
//��������������������������������
void C_PLAYER::ChangeStatus()
{
	switch (PlayerState)
	{
	case PLAYER_STATE::TYPE_STOP:
		break;
	case PLAYER_STATE::TYPE_MOVE:
		AccelePawn	= D3DXVECTOR3( 0.0f,0.0f,0.0f);
		move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bIsGround	= true;
		break;
	case PLAYER_STATE::TYPE_JUMP_UP:
		
		if (move.y < 0.0f)
			PlayerState = PLAYER_STATE::TYPE_JUMP_DOWN;

		bIsGround = false;
	
		break;
	case PLAYER_STATE::TYPE_JUMP_DOWN:

		bIsGround = false;

		break;
	case PLAYER_STATE::TYPE_FALL:
	{
		bIsGround = false;

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
	case PLAYER_STATE::TYPE_DEAD:
		break;
	default:
		break;
	}

	// ���ʏ���

}

//��������������������������������
// ��ԑJ��
//��������������������������������
void C_PLAYER::ChangeState()
{
	if (PlayerState == PLAYER_STATE::TYPE_JUMP_UP &&
		move.y < 0.0f)
	{
		PlayerState = PLAYER_STATE::TYPE_FALL;
	}
}

//��������������������������������������
// �v���C���[�ړ������x�N�g���擾
//��������������������������������������
D3DXVECTOR3 C_PLAYER::GetMoveVec()
{
	return TestVec;
}