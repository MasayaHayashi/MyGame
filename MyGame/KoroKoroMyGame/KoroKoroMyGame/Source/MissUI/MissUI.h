//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// MssUI.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MISS_UI
#define MISS_UI

// ===== インクルード部 =====
#include "../Board/Board.h"

// ===== クラスの前方定義 =====

// ===== クラス定義 =====
class MissUI final : public Board
{
public:
	MissUI();
	~MissUI();

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();

private:
	const std::string TextureFilePass = "Data/Texture/Miss.png";

	FLOAT lerpCnt = 0;
};

#endif