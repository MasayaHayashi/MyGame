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
	static void initialize();
	static void initializeStatus();

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
	static constexpr int RestartFream = 110;

	static const bool isGameType(GameType setGameType);
	static void  changeGameType(GameType setGameType);
	static const GameType getGameType();
	static void  addNextStage();
	static int   getStage();
	static void  update();
	static bool  isRestart();
	static bool  isGameClear();
protected:
	
private:
	static GameManager  instance;
	static GameType		currentGameType;
	static int			currentStage;

	static int			hitStopCnt;

	static int			restartCnt;
	static bool			restart;
	static bool			gameClear;
	GameManager();
	~GameManager();

};

#endif