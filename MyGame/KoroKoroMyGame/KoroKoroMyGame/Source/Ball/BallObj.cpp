//����������������������������������������������������������������
// BallObj.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "BallObj.h"
#include "../KeyBoard/Keyboard.h"
#include "../Collider/Collider.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Collision/Collision.h"

/*
#include "C_MyVector3.h"
#include "collision.h"

#if _DEBUG
#include "debugproc.h"
#endif
*/
// ===== �萔�E�}�N����` =====


//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
BallObj::BallObj()
{
	// �e��N���X������
	pCollider = nullptr;

	// �ʒu�E�����̏����ݒ�
	myTransform.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g�嗦�ݒ�
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	isShader = true;

	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	// �s�񏉊���
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);
}

//��������������������������������������
// �R���X�g���N�^
//��������������������������������������
BallObj::BallObj(UINT setIndex)
{
	// �e��N���X������
	pCollider = nullptr;

	// �ʒu�E�����̏����ݒ�
	myTransform.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g�嗦�ݒ�
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	isShader = true;

	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	// �s�񏉊���
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	idNumber = setIndex;
}


//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
BallObj::~BallObj()
{

	// �e��N���X���
	//	SAFE_DELETE(pCollider);
}

//����������������������������������������������
// �v���C���[������
//����������������������������������������������
void BallObj::initialize()
{
	// ���݂̃V�[���擾
	currentScene = SceneManager::getCurrentSceneType();

	// �V�[�����Ƃɕ���
	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		initializeTitleObj_Title();
		break;
	case SceneManager::SceneState::SceneMain:
		initializeTitleObj_GameMain();
		break;
	case SceneManager::SceneState::SceneResult:
		initializeTitleObj_Result();
		break;
	case SceneManager::SceneState::SceneSelect:
		initializeTitleObj_Title();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		initializeTitleObj_Title();
		break;

	default:
		break;
	}
}

//��������������������������������������������
// �v���C���[�㏈��
//��������������������������������������������
void BallObj::finalize()
{
	ResourceManager::destroyAllMesh();
}

//����������������������������������������������
// �v���C���[�X�V
//����������������������������������������������
void BallObj::update(D3DXVECTOR3 pos, D3DXVECTOR3 rotVec)
{
	// ���݂̃V�[���擾
	currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		UpdateTitleObj_Title();
		break;
	case SceneManager::SceneState::SceneMain:
		updateGameMain(pos,rotVec);
		break;
	case SceneManager::SceneState::SceneResult:
		UpdateTitleObj_Result();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		UpdateTitleObj_Title();
		break;
	default:

		break;
	}
}

//��������������������������������������
// �v���C���[�`��
//��������������������������������������
void BallObj::draw()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �`��
	Pawn::drawObjectLocal();

	// �R���C�_�[�`��
	//	pCollider->DrawCollider();
}

//������������������������������������������
// ���U���g�p�v���C���[�X�V
//������������������������������������������
void BallObj::UpdateTitleObj_Result()
{

}

//��������������������������������������������
// �V�[���^�C�g���p�v���C���[������
//��������������������������������������������
void BallObj::initializeTitleObj_Title()
{
	// �ʒu�A�ړ��ʁA�g�嗦������
	myTransform.pos = D3DXVECTOR3(-0.0, 2.0f, 0.0f);
	myTransform.velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e��ϐ�������
	pD3DTexture = nullptr;
	meshPtr = nullptr;
	materialBufferPtr = nullptr;
	numMat = 0;

	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	// ��]
	D3DXMATRIX mRotX, mRotY, mRotZ;

	D3DXMatrixRotationY(&mRotX, D3DXToRadian(0));
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mRotX);

	// �g��
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, myTransform.scale.x, myTransform.scale.y, myTransform.scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// �ړ�
	D3DXMATRIX mtxTranslate;
	D3DXMatrixTranslation(&mtxTranslate, myTransform.pos.x, myTransform.pos.y, myTransform.pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxTranslate);

	isUsed = true;

	// �R���C�_�[������
	//	pCollider = NEW C_COLLIDER(pos, MeshData.collitionBox);
	//	pCollider->initializeCollider();
	//	pCollider->setUsedFlg(false);
}

//������������������������������������������������
// �V�[�����C���p�v���C���[GameMain
//������������������������������������������������
void BallObj::initializeTitleObj_GameMain()
{
	myTransform.pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity	 = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	myTransform.rotDeg		 = D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
	myTransform.rotDegDest	 = D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
	myTransform.scale		 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	if (idNumber == 1)
	{
		myTransform.pos.x = 5.0f;
		myTransform.pos.y = 0.0f;
		myTransform.pos.z = 0.0f;
	}

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture			= nullptr;
	meshPtr				= nullptr;
	materialBufferPtr	= nullptr;
	numMat = 0;

	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	myTransform.pos.y = meshDataObj.collitionBox.y * 2;

	tagName = "Ball";
	Collision::registerList(&myTransform, tagName);
	
//	move(idNumber);

}

//��������������������������������������
// ���U���g�p�v���C���[������
//��������������������������������������
void BallObj::initializeTitleObj_Result()
{
	myTransform.pos		   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg	   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g�嗦�ݒ�
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture = nullptr;
	meshPtr = nullptr;
	materialBufferPtr = nullptr;
	numMat = 0;

	// X�t�@�C���̓ǂݍ���

	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//����������������������������������������
// �V�[���^�C�g���p�v���C���[�X�V
//����������������������������������������
void BallObj::UpdateTitleObj_Title()
{
	// ��]���ݒ�
	D3DXVECTOR3 RotAxis = D3DXVECTOR3(1.0f, 5.0f, 2.0f);

	// �N�H�[�^�j�I���ɂ���]
	rotCnt += TitleRotSpeed;
	D3DXQuaternionRotationAxis(&quatanion, &RotAxis, rotCnt);		// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);	// �N�H�[�^�j�I�������]�s��|�����킹

															// �R���C�_�[�X�V
															//	pCollider->UpdateCollider(worldMtx, DEFAULT_BOXcolor);
}

//��������������������������������������
// �V�[�����C���p�X�V
//��������������������������������������
void BallObj::updateGameMain(D3DXVECTOR3 pos,D3DXVECTOR3 rotVec)
{
	setWorldMtxPos(myTransform.pos);

	myTransform.pos += myTransform.velocity;
}

//������������������������������������������
// �ړ�
//������������������������������������������
void BallObj::move(const D3DXVECTOR3 moveVector)
{
	D3DXVECTOR3 playerPos = Collision::getTransform("Player", idNumber)->pos;

	myTransform.pos		= playerPos;
	myTransform.pos.y	= playerPos.y - 0.95f;

	setWorldMtxPos(myTransform.pos);
}