//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// GameObjectBase.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef GAME_OBJECT_BASE
#define GAME_OBJECT_BASE

// ===== インクルード部 =====
#include "GameObjectBase.h"

// ===== 列挙体定義 =====
enum class GameObjType
{
	NormalBlockObj = 0,
	velocityBlockObj,
	StarObj,
	GoalObj,
};

// ===== クラス定義 =====
class GameObjectBase
{
	public:
		GameObjectBase();
		virtual ~GameObjectBase();

		virtual void initialize() = 0;
		virtual void finalize() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;

	protected:

	private:

};

#endif