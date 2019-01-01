//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_StageEditUI.h
// ステージエディット表記
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef __C_STAGE_EDIT_UI_H__
#define __C_STAGE_EDIT_UI_H__

// ===== インクルード部 =====
#include "C_Board.h"
#include "C_SceneManager.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class C_STAGE_EDIT_UI : public C_BOARD
{
public:
	C_STAGE_EDIT_UI();		// コンストラクタ
	~C_STAGE_EDIT_UI();		// デストラクタ

	virtual void InitObject();
	virtual void UninitObject();
	virtual void UpdateObject();
	virtual void DrawObject();

private:

	C_SCENE_MANAGER::SCENE_STATE CurrentState;

};

#endif