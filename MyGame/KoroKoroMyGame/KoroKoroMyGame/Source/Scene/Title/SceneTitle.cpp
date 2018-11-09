//��������������������������������������������������������������������������������������������������
// SceneTitle.cpp
// Author	 : MasayaHayashi
//��������������������������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "SceneTitle.h"
#include "../../Light/Light.h"
#include "../../Player/Player.h"
#include "../../camera/camera.h"
/*
#include ""
#include "C_MainField.h"
#include "C_camera.h"
#include "C_Light.h"
#include "input.h"
#include "C_ParticleBase.h"
#include "C_Player.h"
#include "C_TitleObj.h"
#include "C_FADE.h"
#include "C_SceneMainUI.h"
#include "C_StageEditUI.h"
#include "debugproc.h"
#include "C_TitleDesc.h"
#include <stdio.h>	// �t�@�C�����o�͗p
*/

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneTitle::SceneTitle()
{
	bChangeScene = false;
	nChangeSceneWaitCnt = 0;
	uSelectScene = 0;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SceneTitle::~SceneTitle()
{

}

//����������������������������������
// ������
//����������������������������������
void SceneTitle::initialize()
{
	// ���C�g������
	pLight = new Light;
	pLight->InitLight();

	// �v���C���[������
	pPlayer = new C_PLAYER;
	pPlayer->InitObject();

	// �J����������
	pcamera = new camera;
	pcamera->initialize(pPlayer);

	// �X�J�C�h�[��������
	pSkydome = NEW C_SKYDOME;
	pSkydome->InitObject();

	// �t�B�[���h������
	pField = NEW C_MAIN_FIELD;
	pField->InitMeshField();

	// �^�C�g��UI������
	pTitleUI = NEW C_TITLE_UI;
	pTitleUI->InitObject();

	// �^�C�g���I�u�W�F�N�g������
	pTitleObj = NEW C_TITLE_OBJ;
	pTitleObj->InitObject();

	// �p�[�e�B�N��������
	for (INT i = 0; i < MAX_PARTICLE; i++)
	{
		pParticle[i] = NEW C_PARTICLE_BASE(i);
		pParticle[i]->InitObject();
	}

	// 
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		switch (BoardCnt)
		{
		case UI_MAIN:
			pBoard[BoardCnt] = NEW C_SCENE_MAIN_UI;
			break;
		case UI_EDIT:
			pBoard[BoardCnt] = NEW C_STAGE_EDIT_UI;
			break;
		case UI_DESC:
			pBoard[BoardCnt] = NEW C_TITLE_DESCR_UI;
			break;
		default:
			break;
		}
		pBoard[BoardCnt]->InitObject();
	}

	pBoard[UI_MAIN]->setUsedFlg(false);

}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void SceneTitle::finalizeScene()
{

	// �J�����㏈��
	pcamera->finalizecamera();
	SAFE_DELETE(pcamera);

	// ���C�g�㏈��
	pLight->finalizeLight();
	SAFE_DELETE(pLight);

	// �v���C���[�㏈��
	pPlayer->finalizeObject();
	SAFE_DELETE(pPlayer);

	// �X�J�C�h�[���㏈��
	pSkydome->finalizeObject();
	SAFE_DELETE(pSkydome);

	// �t�B�[���h�㏈��
	pField->finalizeMeshField();
	SAFE_DELETE(pField);

	// �^�C�g��UI�㏈��
	pTitleUI->finalizeObject();
	SAFE_DELETE(pTitleUI);

	// �^�C�g���I�u�W�F�N�g�㏈��
	pTitleObj->finalizeObject();
	SAFE_DELETE(pTitleObj);

	// �p�[�e�B�N���㏈��
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		pParticle[i]->finalizeObject();
		SAFE_DELETE(pParticle[i]);
	}

	// UI�㏈��
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->finalizeObject();
		SAFE_DELETE(pBoard[BoardCnt]);
	}


}

//������������������������������������
// �X�V
//������������������������������������
void SceneTitle::updateScene()
{
	D3DXVECTOR3 cameraFowerd = pcamera->getcameraFowerd();

	// �^�C�g��UI�X�V
	pTitleUI->updateObject();

	// �p�[�e�B�N���X�V
	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->updateObject();

	// �v���C���[�X�V
	pPlayer->updateObject(cameraFowerd);

	// �^�C�g���I�u�W�F�N�g�X�V
	pTitleObj->updateObject();

	// �J�����X�V
	pcamera->updatecamera_Title(pPlayer);

	// �V�[���J��
	if (getKeyboardTrigger(DIK_F1))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

		bChangeScene = true;
	}
	if (getKeyboardTrigger(DIK_F2))
	{
//		PlaySound(SOUND_SE_CHANGE_SCENE);
		getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

		bChangeScene = true;
	}

	if (getKeyboardTrigger(DIK_SPACE))
	{
		if (uSelectScene == 0)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_STAGE_EDIT);

			bChangeScene = true;
		}
		else if (uSelectScene == 1)
		{
//			PlaySound(SOUND_SE_CHANGE_SCENE);
			getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);

			bChangeScene = true;
		}
	}

	if (getKeyboardPress(DIK_A))
	{
		pBoard[UI_MAIN]->setUsedFlg(false);
		pBoard[UI_EDIT]->setUsedFlg(true);
		uSelectScene = 0;
	}
	if (getKeyboardPress(DIK_D))
	{
		pBoard[UI_MAIN]->setUsedFlg(true);
		pBoard[UI_EDIT]->setUsedFlg(false);
		uSelectScene = 1;
	}

	// UI�X�V
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->updateObject();
	}


	/*
	// �V�[���J��
	if (bChangeScene)
		nChangeSceneWaitCnt++;
	if(nChangeSceneWaitCnt > 120)
	*/

}

//��������������������������������������
// �`��
//��������������������������������������
void SceneTitle::drawScene()
{
	// �v���C���[�`��
	pPlayer->drawObject();

	// �X�J�C�h�[���`��
	pSkydome->drawObject();

	// �t�B�[���h�`��
	pField->drawMeshField();

	// �^�C�g�����S�`��
	pTitleUI->drawObject();

	// �^�C�g���I�u�W�F�N�g�`��
	pTitleObj->drawObject();

	// �J�����Z�b�g
	pcamera->setcamera();

		
	// �p�[�e�B�N���`��
	for (int i = 0; i < MAX_PARTICLE; i++)
		pParticle[i]->drawObject();

	// UI�`��
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
		pBoard[BoardCnt]->drawObject();
	
}

//��������������������������������������
// �X�e�[�^�X������
//��������������������������������������
void SceneTitle::InitStatus()
{

}


//����������������������������������
// �J�����擾
//����������������������������������
const camera* SceneTitle::getcamera()
{
	return pcamera;
}