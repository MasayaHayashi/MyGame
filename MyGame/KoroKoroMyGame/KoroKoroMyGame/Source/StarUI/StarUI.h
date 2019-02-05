//������������������������������������������������
// StarUI.h
// UI�p�̐��摜
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef STAR_UI_H
#define STAR_UI_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
class StarUI final : public Board
{
public:
	StarUI();
	~StarUI();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	bool awake = false;

	const std::string TextureFilePass = "Data/Texture/GoldStar.png";
	const FLOAT MoveSpeed = 1.5f;
};

#endif