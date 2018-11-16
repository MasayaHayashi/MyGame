//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// GameObjectBase.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef GAME_OBJECT_BASE
#define GAME_OBJECT_BASE

// ===== インクルード部 =====
#include "GameObjectBase.h"

// ===== クラス定義 =====
class GameObjectBase
{
	public:
		GameObjectBase();
		virtual ~GameObjectBase();

		virtual void initialize();
		virtual void finalize();
		virtual void update();
		virtual void draw();

	protected:

	private:

};

#endif