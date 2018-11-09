//����������������������������������������������������
// C_GameSceneManager.cpp
// �Q�[���V�[���̊Ǘ��N���X
// Author : Masaya Hayashi
//����������������������������������������������������

// ===== �C���N���[�h�� =====
#include "SceneManager.h"
#include "../KeyBoard/Keyboard.h"
#include "../Scene/Main/C_SceneMain.h"
#include "../Fade/FadeUI.h"

// ===== �ÓI�����o =====
std::unique_ptr<SceneManager> SceneManager::gameManagerInstancePtr;
SceneManager::SceneState SceneManager::currentSceneType;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneManager::SceneManager()
{
	debugMode		= false;
	frameAdvanceCnt = DebugMoveOnFream;

	// C_SCENE_STAGE_EDIT
	// SCENE_STAGE_EDIT

	// C_SCENE_MAIN
	// SCENE_MAIN

	// C_SCENE_TITLE
	// SCENE_TITLE


	// �V�[���ݒ菉����
	currentScenePtr.reset(new SceneMain());
	currentSceneType	= SceneState::SceneMain;
	nextSceneType		= SceneState::SceneTitle;

	// �t�F�[�h������
	fadePtr.reset(new FadeUI);
	fadePtr->InitObject();
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SceneManager::~SceneManager()
{
	fadePtr->UninitObject();
}

//������������������������������������������
// �V�[��������
//������������������������������������������
void SceneManager::initialize()
{
	// �V�[��������
	currentScenePtr->InitScene();
}

//������������������������������������������
// �V�[���I������
//������������������������������������������
void SceneManager::finalize()
{
	// �V�[���㏈��
	currentScenePtr->UninitScene();
}

//������������������������������������������
// �V�[���X�V
//������������������������������������������
void SceneManager::update()
{
	// �f�o�b�O���[�h�ؑւ�
	if (Keyboard::getTrigger(DIK_V))
		debugMode = !debugMode;

	// �f�o�b�O���[�h����
	if (debugMode)
	{
		if(Keyboard::getPress(DIK_SPACE))
			frameAdvanceCnt --;

		if (frameAdvanceCnt <= 0)
		{
			frameAdvanceCnt = DebugMoveOnFream;
			currentScenePtr->UpdateScene();
		}
	}
	else
		// �V�[���X�V
		currentScenePtr->UpdateScene();
}

//������������������������������������������
// �t�F�[�h�X�V
//������������������������������������������
void SceneManager::updateFade()
{
	fadePtr->UpdateObject();
}

//������������������������������������������
// �V�[���`��
//������������������������������������������
void SceneManager::draw()
{
	// �V�[���`��
	currentScenePtr->DrawScene();
}

//������������������������������������������
// �t�F�[�h�V�[���`��
//������������������������������������������
void  SceneManager::drawFade()
{
	if (fadePtr->GetFadeState() != FadeUI::FadeType::FadeNone)
	{
		// �t�F�[�h�`��
		fadePtr->DrawObject();
	}
}

//��������������������������������������������������������
// �V�[���Z�b�g
//��������������������������������������������������������
void SceneManager::changeScene(SceneState Scene)
{
	switch (Scene)
	{
	case SceneState::SceneTitle:
		delete currentScenePtr.get();
		currentScenePtr = new SceneTitle();
		break;
	case SceneState::SceneMain:
		delete currentScenePtr.get();
		currentScenePtr = new SceneMain();
		break;
	case SceneState::SceneResult:
		delete currentScenePtr.get();
		currentScenePtr = new SceneResult();
		break;
	case SceneState::SceneStageEdit:
		delete currentScenePtr.get();
		currentScenePtr = new SceneStageEdit();
		break;
	// ��O����
	default:
		break;
	}
}

//��������������������������������������������������
// ���݂̃V�[���̃C���X�^���X�擾
//��������������������������������������������������
C_SCENE_BASE* SceneManager::getInstanse()
{
	return currentScenePtr.get();
}

//��������������������������������������������������
// ���݂̃V�[��enum�擾
//��������������������������������������������������
SceneManager::SceneState SceneManager::getCurrentSceneType()
{
	return currentSceneType;
}

//����������������������������������������������������������������������������
// �O�����玟�̃V�[�����Z�b�g(���[�h�V�[���p)
//����������������������������������������������������������������������������
void SceneManager::setCurrentScene(SceneState setStateType)
{
	currentSceneType = setStateType;
}

//����������������������������������������������������
// ���̃V�[�����Z�b�g
//����������������������������������������������������
void SceneManager::setNextScene(SceneState setNextSceneType)
{
	nextSceneType = setNextSceneType;
	fadePtr->SetFade(FadeUI::FadeType::FadeOut);
}

//����������������������������������������������������������������������������
// ���̃V�[���擾(���[�h�V�[���p)
//����������������������������������������������������������������������������
SceneManager::SceneState SceneManager::getNextScene()
{
	return nextSceneType;
}

//����������������������������������������������������������������������������
// �t�F�[�h�擾
//����������������������������������������������������������������������������
FadeUI* SceneManager::getFade()
{
	return fadePtr.get();
}