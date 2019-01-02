//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_WhatStageLoadUI.h
// どのステージにしますか
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef __C_WHAT_STAGE_LOAD_UI_H__
#define __C_WHAT_STAGE_LOAD_UI_H__

// ===== インクルード部 =====
#include "C_Board.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class C_WHAT_STAGE_LOAD_UI : public C_BOARD
{
public:
	C_WHAT_STAGE_LOAD_UI();		// コンストラクタ
	~C_WHAT_STAGE_LOAD_UI();		// デストラクタ

	void InitObject();
	void UninitObject();
	void UpdateObject();
	void DrawObject();

private:

};

#endif