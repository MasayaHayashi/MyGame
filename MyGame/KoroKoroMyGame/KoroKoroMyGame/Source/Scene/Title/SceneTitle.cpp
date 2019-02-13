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
#include "../../TitleUI/TitleUI.h"
#include "../../TitleSelectUI/TitleSelectUI.h"
#include "../../TitleDisp/TitleDisp.h"
#include "../../Xinput/Xinput.h"
#include "../../EditUI/StageEditUI.h"
#include "../../Audio/MyAudiere.h"

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
	boardPtres.push_back(std::unique_ptr<Board>(NEW TitleSelectUI()));
	boardPtres.push_back(std::unique_ptr<Board>(NEW TitleUI()));

	lightPtr.reset(NEW Light());
	cameraPtr.reset(NEW Camera());
	skydomePtr.reset(NEW Skydome());
	playerPtr.reset(NEW Player(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0));
	fieldPtr.reset(NEW MainField());
	titleUiPtr.reset(NEW TitleUI());
	heartObjPtr.reset(NEW HeartObj());

	boardPtres.push_back(std::unique_ptr<TitleDisp>( NEW TitleDisp()) );
	boardPtres.push_back(std::unique_ptr<Board>	   ( NEW StageEditUI()));
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


	for (const auto &boardPtr : boardPtres)
	{
		boardPtr->initialize();
	}


	MyAudiere::getBgm(0)->setRepeat(true);
	MyAudiere::getBgm(0)->setVolume(0.05f);
	MyAudiere::getBgm(0)->play();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void SceneTitle::finalize()
{
	cameraPtr->finalize();
	playerPtr->finalize();

	skydomePtr->finalize();
	heartObjPtr->finalize();

	for (const auto &boardPtr : boardPtres)
	{
		boardPtr->finalize();
	}

	collisionPtr->finalize("Player");

	MyAudiere::getBgm(0)->setRepeat(true);
	MyAudiere::getBgm(0)->setVolume(0.1f);
	MyAudiere::getBgm(0)->stop();
}

//������������������������������������
// �X�V
//������������������������������������
void SceneTitle::update()
{

	playerPtr->update(Collision::getCameraTransform("Camera", 0)->fowerd);
	heartObjPtr->update();
	skydomePtr->update();
	fieldPtr->update();
	Xinput::update();
	DirectX3D::printDebug("isss%d", Xinput::isLeftLS());
	DirectX3D::printDebug("right%d", Xinput::isRightLS());
	DirectX3D::printDebug("yyyyyyyyyyyy%d", Xinput::getThumbLX());

	cameraPtr->updateTitle(playerPtr.get());

	if (Keyboard::getPress(DIK_1) || 
		Xinput::getButton_A_Triger())
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}

	if (Keyboard::getPress(DIK_2))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneSelect);
	}

	if (Keyboard::getPress(DIK_3))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneStageEdit);
	}

	if (Keyboard::getPress(DIK_A))
	{
		boardPtres.back()->setUsedFlg(true);
		boardPtres.front()->setUsedFlg(false);
		changeMode = SceneManager::SceneState::SceneStageEdit;
	}

	if (Keyboard::getPress(DIK_D))
	{
		boardPtres.back()->setUsedFlg(false);
		boardPtres.front()->setUsedFlg(true);
		changeMode = SceneManager::SceneState::SceneMain;
	}

	if (Keyboard::getTrigger(DIK_SPACE))
	{
		SceneManager::setNextScene(changeMode);
	}

	for (const auto &boardPtr : boardPtres)
	{
		boardPtr->update();
	}

}

//��������������������������������������
// �`��
//��������������������������������������
void SceneTitle::draw()
{
	playerPtr->draw();
	heartObjPtr->draw(cameraPtr->getMtxView(), cameraPtr->getProjectionMtx());
	skydomePtr->draw(cameraPtr->getMtxView(), cameraPtr->getProjectionMtx());
	fieldPtr->draw();

	for (const auto &boardPtr : boardPtres)
	{
		boardPtr->draw();
	}

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