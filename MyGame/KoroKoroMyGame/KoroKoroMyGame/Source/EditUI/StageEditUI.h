//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// StageEditUI.h
// ステージエディット表記
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef STAGE_EDIT_UI
#define STAGE_EDIT_UI

// ===== インクルード部 =====
#include "../Board/Board.h"
#include "../SceneManager/SceneManager.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class StageEditUI final : public Board
{
public:
	StageEditUI();		// コンストラクタ
	~StageEditUI();		// デストラクタ

	virtual void initialize();
	virtual void finalize();
	virtual void update();
	virtual void draw();

private:
	const std::string TexturePass = "data/Texture/EditMode.png";

	SceneManager::SceneState currentState;

};

#endif