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
		FallMiss,
		Miss,
		Goal,
		Pause,
		GetItem,
	};

	static constexpr int HitStopTime = 60;

	static const bool isGameType(GameType setGameType);
	static void changeGameType(GameType setGameType);
	static const GameType getGameType();
	static void addNextStage();
	static int getStage();
	static void update();
protected:
	
private:
	static GameManager  instance;
	static GameType		currentGameType;
	static int			currentStage;

	static int			hitStopCnt;

	GameManager();
	~GameManager();

};

#endif