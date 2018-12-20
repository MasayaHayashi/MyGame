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

	const std::string TextureFilePass = "Data/Texture/GoldStar.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

	FLOAT rotCnt = 0;

	SceneManager::SceneState currentScene;
};

#endif 
