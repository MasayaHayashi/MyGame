//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_WhatStageSaveUI.h
// セーブステージ選択用UI
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef WHAT_STAGE_SAVE_UI_H
#define WHAT_STAGE_SAVE_UI_H

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class WhatStageSaveUi final : public Board
{
public:
	WhatStageSaveUi();		// コンストラクタ
	~WhatStageSaveUi();		// デストラクタ

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	const std::string TexturePass = "data/TEXTURE/WhatStageLoad.png";
	FLOAT moveCnt = 0;
};

#endif