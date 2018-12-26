//����������������������������������������������������
// SceneManager.cpp
// Author : Masaya Hayashi
//����������������������������������������������������

// ===== �C���N���[�h�� =====
#include "SceneManager.h"
#include "../CrtDebug/CrtDebug.h"
#include "../KeyBoard/Keyboard.h"
#include "../Scene/Title/SceneTitle.h"
#include "../Scene/Main/SceneMain.h"
#include "../Scene/Select/SceneSelect.h"
#include "../Scene/Result/C_SceneResult.h"
#include "../Fade/FadeUI.h"

// ===== �ÓI�����o =====
std::unique_ptr<SceneManager>	SceneManager::SceneManagerInstancePtr(nullptr);
std::unique_ptr<SceneBase>		SceneManager::currentScenePtr(nullptr);
std::unique_ptr<FadeUI>			SceneManager::fadePtr(nullptr);

SceneManager::SceneState	  SceneManager::nextSceneType	 = SceneManager::SceneState::SceneTitle;
SceneManager::SceneState	  SceneManager::currentSceneType = SceneManager::SceneState::SceneTitle;

bool	SceneManager::debugMode		  = false;
INT		SceneManager::frameAdvanceCnt = DebugvelocityOnFream;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneManager::SceneManager()
{
	debugMode		= false;
	frameAdvanceCnt = DebugvelocityOnFream;

	loadSettingFile();
	makeStartScene();

	fadePtr.reset(NEW FadeUI);
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SceneManager::~SceneManager()
{
	fadePtr->finalize();
}

//������������������������������������������
// �C���X�^���X����
//������������������������������������������
bool SceneManager::create()
{
	if (SceneManagerInstancePtr.get() == nullptr)
	{
		SceneManagerInstancePtr.reset(NEW SceneManager());
		return true;
	}
	else
	{
		return false;
	}
}

//������������������������������������������
// �V�[��������
//������������������������������������������
void SceneManager::initialize()
{
	fadePtr->initialize();

	// �V�[��������
	currentScenePtr->initialize();
}

//������������������������������������������
// �V�[���I������
//������������������������������������������
void SceneManager::finalize()
{
	// �V�[���㏈��
	currentScenePtr->finalize();
}

//������������������������������������������
// �V�[���X�V
//������������������������������������������
void SceneManager::update()
{
	// �f�o�b�O���[�h�ؑւ�
	if (Keyboard::getTrigger(DIK_V))
	{
		debugMode = !debugMode;
	}

	// �f�o�b�O���[�h����
	if (debugMode)
	{
		if (Keyboard::getPress(DIK_SPACE))
		{
			frameAdvanceCnt--;
		}

		if (frameAdvanceCnt <= 0)
		{
			frameAdvanceCnt = DebugvelocityOnFream;
			currentScenePtr->update();
		}
	}
	else
	{
		// �V�[���X�V
		currentScenePtr->update();
	}
}

//������������������������������������������
// �t�F�[�h�X�V
//������������������������������������������
void SceneManager::updateFade()
{
	fadePtr->update();
}

//������������������������������������������
// �V�[���`��
//������������������������������������������
void SceneManager::draw()
{
	// �V�[���`��
	currentScenePtr->draw();
}

//������������������������������������������
// �t�F�[�h�V�[���`��
//������������������������������������������
void  SceneManager::drawFade()
{
	if (fadePtr->getFadeState() != FadeUI::FadeType::FadeNone)
	{
		// �t�F�[�h�`��
		fadePtr->draw();
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
		currentScenePtr.reset(NEW SceneTitle());
		break;
		
	case SceneState::SceneMain:
		currentScenePtr.reset(NEW SceneMain());
		break;
	case SceneState::SceneSelect:
		currentScenePtr.reset(NEW SceneSelect());
		break;
	/*
	case SceneState::SceneResult:
		delete currentScenePtr.get();
		currentScenePtr = NEW SceneResult();
		break;
	case SceneState::SceneStageEdit:
		delete currentScenePtr.get();
		currentScenePtr = NEW SceneStageEdit();
		break;
		*/
	// ��O����
	default:
		break;
	}
	
}

//��������������������������������������������������
// ���݂̃V�[���̃C���X�^���X�擾
//��������������������������������������������������
SceneBase* SceneManager::getInstanse()
{
	return currentScenePtr.get();
}

//��������������������������������������������������
// ���݂̃V�[��enum�擾
//��������������������������������������������������
const SceneManager::SceneState SceneManager::getCurrentSceneType()
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
	fadePtr->setFade(FadeUI::FadeType::FadeOut);
}

//����������������������������������������������������������������������������
// ���̃V�[���擾(���[�h�V�[���p)
//����������������������������������������������������������������������������
const SceneManager::SceneState SceneManager::getNextScene()
{
	return nextSceneType;
}

//����������������������������������������������������������������������������
// �t�F�[�h�擾
//����������������������������������������������������������������������������
const FadeUI* SceneManager::getFade() const
{
	return fadePtr.get();
}

//��������������������������������������������������
// ini�t�@�C�����[�h
//��������������������������������������������������
void SceneManager::loadSettingFile()
{
	DWORD  ret;
	ret = GetPrivateProfileString("Scene", "StartScene", "�����ς�", StartScene, sizeof(StartScene),"Data/debugSetting.ini");

	if (!ret)
	{
		MessageBox(nullptr, TEXT("�V�[���ǂݍ��݃G���["), TEXT("Error"), MB_ICONERROR);
	}
}

//��������������������������������������������������
// �����V�[���ݒ�
//��������������������������������������������������
void SceneManager::makeStartScene()
{
	switch (sceneTypeMap[StartScene])
	{
	case SceneState::SceneTitle :
		currentScenePtr.reset(NEW SceneTitle());
		break;
	case SceneState::SceneMain	:
		currentScenePtr.reset(NEW SceneMain());
		break;
	case SceneState::SceneStageEdit: 
		break;
	case SceneState::SceneSelect:
		currentScenePtr.reset(NEW SceneSelect());
		break;
	case SceneState::SceneResult:


		break;
	default:
		break;
	}

	currentSceneType = sceneTypeMap[StartScene];
	nextSceneType = sceneTypeMap[StartScene];
}

//��������������������������������������������������
// �����񂩂�UINT�ւ̕ϊ�
//��������������������������������������������������
void SceneManager::changeStringToUint()
{
	
}