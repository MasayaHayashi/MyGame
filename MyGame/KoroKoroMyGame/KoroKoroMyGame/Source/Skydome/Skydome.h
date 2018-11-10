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
	void update();
	void draw();
	void finalize();

	HRESULT createTexture();
private:
	CHAR fileName[256];
	D3DXVECTOR3	textureSize;

};

#endif
