//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Skydome.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SKYDOME_H
#define SKYDOME_H

// ===== インクルード部 =====
#include "../Pawn/Pawn.h"

// ===== クラス定義 =====
class Skydome final : public Pawn
{
public :
	Skydome();
	~Skydome();
	void initialize();
	void update();
	void draw();
	void finalize();

	HRESULT createTexture();
private:
	static constexpr FLOAT Scale = 7.0f;
	
	const std::string ModelFilePass		= "data/MODEL/Skydome/skydome.x";
	const std::string TextureFilePass	= "data/MODEL/Skydome/ocean.jpg";
	D3DXVECTOR3	textureSize;

};

#endif
