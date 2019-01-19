//����������������������������������������������������������������
// GameManager.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

// ===== �C���N���[�h�� =====
#include "../GameObjectBase/GameObjectBase.h"

// ===== �N���X�̑O���錾 =====

// ===== �N���X��` =====
class GameManager final
{
public:
	void initialize();

	enum class GameType
	{
		Ready,
		Playing,
		Pause,
	};

	static const bool isGameType(GameType setGameType);
	static void changeGameType(GameType setGameType);
protected:
	
private:

	static GameType currentGameType;
	static GameManager instance;

	GameManager();
	~GameManager();

};

#endif
