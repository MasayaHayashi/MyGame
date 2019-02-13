//����������������������������������������������������������������
// Field.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef FIELD_H
#define FIELD_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"
#include "../SceneManager/SceneManager.h"

// ===== �N���X�̑O���錾 =====

// ===== �\���̒�` =====

// ===== �񋓑̒�` =====

// ===== �N���X��` =====
class Field final : public Pawn
{
public:
	Field();
	~Field();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj);

private:

	const std::string ModelFilePass		= "Data/Model/Field/RockLayered_5.x.x";
	const std::string TextureFilePass	= "Data/Model/Heart/heart.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

	FLOAT rotCnt = 0;

	void initializeTitle();
	void initializeGameMain();
	void initializeResult();
	void updateTitle();
	void updateGameMain();
	void updateResult();


	SceneManager::SceneState currentScene;
};

#endif 
