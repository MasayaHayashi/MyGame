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