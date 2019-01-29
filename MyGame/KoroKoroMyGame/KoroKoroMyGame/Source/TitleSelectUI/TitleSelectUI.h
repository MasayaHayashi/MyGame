//������������������������������������������������
// C_TitleUI.h
// �^�C�g���p�I�u�W�F�N�g�N���X
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef TITLE_SELECT_UI
#define TITLE_SELECT_UI

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
class TitleSelectUI final : public Board
{
public:
	TitleSelectUI();
	~TitleSelectUI();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	const std::string TextureFilePass = "Data/Texture/ScceneMain.png";
	const FLOAT MoveSpeed = 1.5f;
};

#endif