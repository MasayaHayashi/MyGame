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
	void updateObject();
	void drawObject();
	void finalize();

	HRESULT CreateTexture();
private:
	char textureFileName[256];
	D3DXVECTOR3	textureSize;

};

#endif
