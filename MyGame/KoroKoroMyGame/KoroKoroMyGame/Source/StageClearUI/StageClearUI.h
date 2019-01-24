//����������������������������������������������������������������
// StageClearUI.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef STAGE_CLEAR_UI
#define STAGE_CLEAR_UI

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"
#include "../SceneManager/SceneManager.h"

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O���錾 =====

// ===== �\���̒�` =====

// ===== �񋓑̒�` =====

// ===== �N���X��` =====
class StageClearUI final : public Board
{
public:
	StageClearUI();
	~StageClearUI();

	void initialize();
	void finalize();
	void update();
	void draw();

private:

	const std::string TextureFilePass = "Data/Texture/stageClear.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

	FLOAT rotCnt = 0;

	SceneManager::SceneState currentScene;
};

#endif 
