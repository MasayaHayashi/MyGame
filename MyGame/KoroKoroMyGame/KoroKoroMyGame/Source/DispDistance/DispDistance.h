//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// DispDistance.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef DISP_DISTANCE_H
#define DISP_DISTANCE_H

// ===== インクルード部 =====
#include "../Board/Board.h"
#include "../NumberUI/NumberUI.h"

// ===== クラス定義 =====
class DispDistance final : public Board
{
public:
	DispDistance();
	~DispDistance();

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();

	static constexpr INT MaxDigit = 3;

private:
	std::array<NumberUI, MaxDigit> numberUi;

	const std::string TexturePass = "Data/Texture/numbers.png";

};

#endif