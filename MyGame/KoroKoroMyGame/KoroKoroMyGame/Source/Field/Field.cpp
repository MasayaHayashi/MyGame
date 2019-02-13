//����������������������������������������������������������������
// Field.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Field.h"
#include "../KeyBoard/Keyboard.h"
#include "../Collider/Collider.h"
#include "../DirectX3D/DirectX3D.h"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
Field::Field()
{
	// �ʒu�E�����̏����ݒ�
	myTransform.pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g�嗦�ݒ�
	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	// �s�񏉊���
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	isFieldObject = false;
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
Field::~Field()
{

}

//����������������������������������������������
// �v���C���[������
//����������������������������������������������
void Field::initialize()
{
	// ���݂̃V�[���擾
	currentScene = SceneManager::getCurrentSceneType();

	// �V�[�����Ƃɕ���
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
		initializeTitle();
		break;

	default:
		break;
	}
}

//��������������������������������������������
// �v���C���[�㏈��
//��������������������������������������������
void Field::finalize()
{
	ResourceManager::destroyAllMesh();
}

//����������������������������������������������
// �v���C���[�X�V
//����������������������������������������������
void Field::update()
{
	// ���݂̃V�[���擾
	currentScene = SceneManager::getCurrentSceneType();

	switch (currentScene)
	{
	case SceneManager::SceneState::SceneTitle:
		updateTitle();
		break;
	case SceneManager::SceneState::SceneMain:
		updateGameMain();
		break;
	case SceneManager::SceneState::SceneResult:
		updateResult();
		break;
	case SceneManager::SceneState::SceneStageEdit:
		break;
	default:

		break;
	}
}

//��������������������������������������
// �`��
//��������������������������������������
void Field::draw()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();


	// �`��
	Pawn::drawObjectLocal();

	// �R���C�_�[�`��
//	pCollider->DrawCollider();
}

//����������������������������������������
// �`��
//����������������������������������������
void Field::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();


	// �`��
	Pawn::drawObjectLocal(mtxView, mtxProj);

}

//������������������������������������������
// ���U���g�p�v���C���[�X�V
//������������������������������������������
void Field::updateResult()
{

}

//��������������������������������������������
// �V�[���^�C�g���p�v���C���[������
//��������������������������������������������
void Field::initializeTitle()
{
	// �ʒu�A�ړ��ʁA�g�嗦������
	myTransform.pos		= D3DXVECTOR3(-8.5f, -2.0f, 0.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e��ϐ�������
	pD3DTexture	 = nullptr;
	meshPtr	 = nullptr;
	materialBufferPtr = nullptr;
	numMat		 = 0;

	// X�t�@�C���̓ǂݍ���
//	ResourceManager *pResourceMgr = GetResourceManager();

//	ResourceManager::MakevertexBoard(vertexBoard, fileName);
//	ResourceManager::CreateTexture(Texture, fileName);

	ResourceManager::makeModel(meshDataObj, fileName,meshType);
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

	myTransform.isUsed = true;

	// �R���C�_�[������
//	pCollider = NEW C_COLLIDER(pos, MeshData.collitionBox);
//	pCollider->initializeCollider();
//	pCollider->setUsedFlg(false);
}

//������������������������������������������������
// �V�[�����C���p�v���C���[GameMain
//������������������������������������������������
void Field::initializeGameMain()
{
	myTransform.pos		= D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
	myTransform.rotDegDest = D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture	 = nullptr;
	meshPtr	 = nullptr;
	materialBufferPtr = nullptr;
	numMat		 = 0;

	// X�t�@�C���̓ǂݍ���
//	ResourceManager *pResourceMgr = GetResourceManager();


	ResourceManager::makeModel(meshDataObj, fileName,meshType);
	ResourceManager::createTexture(textureData, texFileName);
}


//��������������������������������������
// ���U���g�p�v���C���[������
//��������������������������������������
void Field::initializeResult()
{
	myTransform.pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.rotDegDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g�嗦�ݒ�
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	pD3DTexture			= nullptr;
	meshPtr				= nullptr;
	materialBufferPtr	= nullptr;
	numMat				= 0;

	ResourceManager::makeModel(meshDataObj, fileName,meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//����������������������������������������
// �V�[���^�C�g���p�v���C���[�X�V
//����������������������������������������
void Field::updateTitle()
{
	// ��]���ݒ�
	D3DXVECTOR3 rotAxis = D3DXVECTOR3(1.0f,5.0f,2.0f);
	
	// �N�H�[�^�j�I���ɂ���]
	rotCnt += TitleRotSpeed;
	D3DXQuaternionRotationAxis(&quatanion, &rotAxis, rotCnt);		// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);			// �N�H�[�^�j�I�������]�s��|�����킹

	// �R���C�_�[�X�V
//	pCollider->UpdateCollider(worldMtx, DEFAULT_BOXcolor);
}

//��������������������������������������
// �V�[�����C���p�v���C���[�X�V
//��������������������������������������
void Field::updateGameMain()
{
	// ��]���ݒ�
	D3DXVECTOR3 rotAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �N�H�[�^�j�I���ɂ���]
	rotCnt += TitleRotSpeed;
	D3DXQuaternionRotationAxis(&quatanion, &rotAxis, rotCnt);		// �N�H�[�^�j�I���ł̔C�ӎ���]
	D3DXMatrixRotationQuaternion(&worldMtx, &quatanion);			// �N�H�[�^�j�I�������]�s��|�����킹
}