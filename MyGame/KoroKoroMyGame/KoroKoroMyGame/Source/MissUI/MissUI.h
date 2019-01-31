//������������������������������������������������
// MssUI.h
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef MISS_UI
#define MISS_UI

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
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