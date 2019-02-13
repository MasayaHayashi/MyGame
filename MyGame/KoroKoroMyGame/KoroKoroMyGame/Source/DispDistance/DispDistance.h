//������������������������������������������������
// DispDistance.h
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef DISP_DISTANCE_H
#define DISP_DISTANCE_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"
#include "../NumberUI/NumberUI.h"

// ===== �N���X��` =====
class DispDistance final : public Board
{
public:
	DispDistance();
	~DispDistance();

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();

	static constexpr INT MaxDigit = 3;

private:
	std::array<NumberUI, MaxDigit> numberUi;

	const std::string TexturePass = "Data/Texture/numbers.png";

};

#endif