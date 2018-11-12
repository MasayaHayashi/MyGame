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
	static constexpr FLOAT Scale = 7.0f;
	
	const std::string ModelFilePass		= "data/MODEL/Skydome/skydome.x";
	const std::string TextureFilePass	= "data/MODEL/Skydome/ocean.jpg";
	D3DXVECTOR3	textureSize;

};

#endif
