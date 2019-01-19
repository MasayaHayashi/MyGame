//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// GameManager.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

// ===== インクルード部 =====
#include "../GameObjectBase/GameObjectBase.h"

// ===== クラスの前方宣言 =====

// ===== クラス定義 =====
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
