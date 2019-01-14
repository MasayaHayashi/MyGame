//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// GameObjectBase.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef GAME_OBJECT_BASE
#define GAME_OBJECT_BASE

// ===== インクルード部 =====
#include "GameObjectBase.h"

enum class GameObjectType
{
	NormalBlockObj = 0,
	MoveBlockOBj,
	StarObj,
	GoalObj,
};

enum class ItemObjType
{
};

// ===== クラス定義 =====
class GameObjectBase
{
	public:
		static constexpr size_t MaxGameObjType = static_cast<size_t>(GameObjectType::GoalObj) + 1;


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