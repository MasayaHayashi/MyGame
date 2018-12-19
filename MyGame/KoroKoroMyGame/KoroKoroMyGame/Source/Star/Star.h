//����������������������������������������������������������������
// HeartObj.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef STAR_H
#define STAR_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"
#include "../SceneManager/SceneManager.h"

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O���錾 =====

// ===== �\���̒�` =====

// ===== �񋓑̒�` =====

// ===== �N���X��` =====
class Star final : public Board
{
public:
	Star();
	~Star();

	void initialize();
	void finalize();
	void update();
	void draw();

private:

	const std::string ModelFilePass = "Data/Model/Heart/heart.x";
	const std::string TextureFilePass = "Data/Texture/GoldStar.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

	FLOAT rotCnt = 0;

	void initializeTitleObj_Title();
	void initializeTitleObj_GameMain();
	void initializeTitleObj_Result();
	void UpdateTitleObj_Title();
	void UpdateTitleObj_GameMain();
	void UpdateTitleObj_Result();

	SceneManager::SceneState currentScene;
};

#endif 
