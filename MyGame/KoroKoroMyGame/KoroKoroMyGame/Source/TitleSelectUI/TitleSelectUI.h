//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_TitleUI.h
// タイトル用オブジェクトクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef TITLE_SELECT_UI
#define TITLE_SELECT_UI

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class TitleSelectUI final : public Board
{
public:
	TitleSelectUI();
	~TitleSelectUI();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	const std::string TextureFilePass = "Data/Texture/ScceneMain.png";
	const FLOAT MoveSpeed = 1.5f;
};

#endif