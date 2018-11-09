//����������������������������������������������������������������
// Skydome.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SKYDOME_H
#define SKYDOME_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"

// ===== �N���X��` =====
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
