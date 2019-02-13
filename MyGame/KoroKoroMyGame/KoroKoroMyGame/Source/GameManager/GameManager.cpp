//����������������������������������������������������������������
// GameManager.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "GameManager.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../SceneManager/SceneManager.h"

// ===== �ÓI�����o =====
GameManager				GameManager::instance;
GameManager::GameType	GameManager::currentGameType;
INT						GameManager::currentStage = 1;
INT						GameManager::hitStopCnt   = 0;
INT						GameManager::restartCnt   = 0;
bool					GameManager::restart	  = false;
bool					GameManager::gameClear	  = false;

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
GameManager::GameManager()
{

}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
GameManager::~GameManager()
{

}

//����������������������������������������������
// ������
//����������������������������������������������
void GameManager::initialize()
{
	restart		= false;
	gameClear	= false;
	restartCnt = 0;
}

//����������������������������������������������
// �X�V
//����������������������������������������������
void GameManager::update()
{
	if (isGameType(GameType::GetItem))
	{
		hitStopCnt++;

		if (hitStopCnt > HitStopTime)
		{
			hitStopCnt = 0;
			GameManager::changeGameType(GameType::Playing);
		}
	}


	if (GameManager::isGameType(GameManager::GameType::Miss) || 
		GameManager::isGameType(GameManager::GameType::FallMiss))
	{
		restartCnt++;

		if (restartCnt > RestartFream)
		{
			restart = true;
			GameManager::changeGameType(GameManager::GameType::Ready);
			restartCnt = 0;
		}
	}


	if (GameManager::isGameType(GameManager::GameType::Goal))
	{
		restartCnt++;

		if (restartCnt > RestartFream)
		{
			GameManager::addNextStage();
			SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
			restartCnt = 0;
		}
	}
}

//����������������������������������������������
// �Q�[����Ԏ擾
//����������������������������������������������
const bool GameManager::isGameType(GameType setGameType)
{
	if (currentGameType == setGameType)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//����������������������������������������������
// �Q�[����Ԏ擾
//����������������������������������������������
const GameManager::GameType GameManager::getGameType()
{
	return currentGameType;
}

//����������������������������������������������
// �Q�[����ԕύX
//����������������������������������������������
void GameManager::changeGameType(GameType setGameType)
{
	currentGameType = setGameType;
}

//����������������������������������������������
// ���̃X�e�[�W��
//����������������������������������������������
void GameManager::addNextStage()
{
	currentStage ++;
}
//��������������������������������������������
// �X�e�[�W�擾
//��������������������������������������������
INT GameManager::getStage()
{
	return currentStage;
}

//��������������������������������������������
// �Q�[���Ǘ�
//��������������������������������������������
bool GameManager::isRestart()
{
	return restart;
}

//��������������������������������������������
// �Q�[���N���A
//��������������������������������������������
bool GameManager::isGameClear()
{
	return gameClear;
}

//��������������������������������������������
// �X�e�[�^�X������
//��������������������������������������������
void GameManager::initializeStatus()
{
	restartCnt = 0;
}