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
	CHAR fileName[256];
	D3DXVECTOR3	textureSize;

};

#endif
