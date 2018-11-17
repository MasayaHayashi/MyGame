//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// MainField.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MAIN_FIELD_H
#define MAIN_FIELD_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"

// ===== 定数・マクロ定義 =====

// ===== クラス定義 =====
class MainField : public Pawn
{
public:
	static constexpr FLOAT Size = 5300.0f;
	static constexpr FLOAT Radius = Size *0.5f;
	const std::string ModelFilePass   = "Data/Model/MainField/RockLayered_5.x";
	const std::string TextureFilePass = "Data/Model/Heart/heart.png";

	MainField();
	~MainField();
	void initialize();
	void finalize();
	void update();
	void draw();
	D3DXVECTOR3 getCenterPos() const;
	void setMaterialcolorEmissive(float fRed, float Green, float Blue, float Alpha);
	bool getCollisionCiecle(D3DXVECTOR3 CurrentPos, float) const;
	FLOAT getCollisionRadius() const;

private:
};

#endif
