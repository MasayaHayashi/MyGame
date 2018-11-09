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

		virtual void InitObject();
		virtual void UninitObject();
		virtual void UpdateObject();
		virtual void DrawObject();

	protected:
		enum class TAG_TYPE
		{
			TAG_BASEOBJ = 0,
			TAG_PLAYER,		// 自機
			TAG_ENEMY,		// 敵機
			TAG_LAND,		// 地面
			MAX_TAG_TYPE,
		};

		TAG_TYPE	Tag;				// 識別用タグ
	private:

};

#endif