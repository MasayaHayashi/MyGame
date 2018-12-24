//��������������������������������������������������������������������������������������������������
// SceneTitle.cpp
// Author	 : MasayaHayashi
//��������������������������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "SceneTitle.h"
#include "../../Light/Light.h"
#include "../../Player/Player.h"
#include "../../Camera/camera.h"
#include "../../Skydome/Skydome.h"
#include "../../MainField/MainField.h"
#include "../../TitleUI/TitleUI.h"
#include "../../HeartObj/HeartObj.h"

/*
#include "C_MainField.h"
#include "C_Camera.h"
#include "C_Light.h"
#include "input.h"
#include "C_StarBase.h"
#include "C_Player.h"
#include "C_TitleObj.h"
#include "FadeUI.h"
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

	lightPtr.reset(NEW Light());
	cameraPtr.reset(NEW Camera());
	skydomePtr.reset(NEW Skydome());
	playerPtr.reset(NEW Player());
	fieldPtr.reset(NEW MainField());
	titleUiPtr.reset(NEW TitleUI());
	heartObjPtr.reset(NEW HeartObj());
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
	playerPtr->initialize();
	lightPtr->initialize();
	cameraPtr->initializeTitle();
	skydomePtr->initialize();
	fieldPtr->initialize();
	titleUiPtr->initialize();
	heartObjPtr->initialize();

	/*
	// �p�[�e�B�N��������
	for (INT i = 0; i < MAX_Star; i++)
	{
		pStar[i] = NEW C_Star_BASE(i);
		pStar[i]->initialize();
	}
	*/
	/*

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
		pBoard[BoardCnt]->initialize();
	}

	pBoard[UI_MAIN]->setUsedFlg(false);
	*/

}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void SceneTitle::finalize()
{
	cameraPtr->finalize();
	playerPtr->finalize();

	/*
	// �J�����㏈��
	pCamera->finalizeCamera();
	SAFE_DELETE(pCamera);

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
	for (int i = 0; i < MAX_Star; i++)
	{
		pStar[i]->finalizeObject();
		SAFE_DELETE(pStar[i]);
	}

	// UI�㏈��
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
	{
		pBoard[BoardCnt]->finalizeObject();
		SAFE_DELETE(pBoard[BoardCnt]);
	}

	*/
}

//������������������������������������
// �X�V
//������������������������������������
void SceneTitle::update()
{
	playerPtr->update(SceneManager::getInstanse()->getCamera()->getFowerd());
	heartObjPtr->update();
	skydomePtr->update();
	fieldPtr->update();
	cameraPtr->updateTitle(playerPtr.get());

	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}

	/*
	D3DXVECTOR3 CameraFowerd = pCamera->getCameraFowerd();

	// �^�C�g��UI�X�V
	pTitleUI->updateObject();

	// �p�[�e�B�N���X�V
	for (INT i = 0; i < MAX_Star; i++)
		pStar[i]->updateObject();

	// �v���C���[�X�V
	pPlayer->updateObject(CameraFowerd);

	// �^�C�g���I�u�W�F�N�g�X�V
	pTitleObj->updateObject();

	// �J�����X�V
	pCamera->updateCamera_Title(pPlayer);

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
	*/

}

//��������������������������������������
// �`��
//��������������������������������������
void SceneTitle::draw()
{
	playerPtr->draw();
	heartObjPtr->draw();
	skydomePtr->draw();
	fieldPtr->draw();

	// �X�J�C�h�[���`��
//	pSkydome->draw();

	/*
	// �t�B�[���h�`��
	pField->drawMeshField();

	// �^�C�g�����S�`��
	pTitleUI->drawObject();

	// �^�C�g���I�u�W�F�N�g�`��
	pTitleObj->drawObject();

	*/

	// �J�����Z�b�g
	cameraPtr->setCamera();

	/*
	// �p�[�e�B�N���`��
	for (int i = 0; i < MAX_Star; i+)+
		pStar[i]->drawObject();

	// UI�`��
	for (INT BoardCnt = 0; BoardCnt < MAX_UI_TYPE; BoardCnt++)
		pBoard[BoardCnt]->drawObject();
	*/
}

//��������������������������������������
// �X�e�[�^�X������
//��������������������������������������
void SceneTitle::initializeStatus()
{

}


//����������������������������������
// �J�����擾
//����������������������������������
Camera* SceneTitle::getCamera()
{
	return cameraPtr.get();
}