//������������������������������������������������
// StageEditUI.h
// �X�e�[�W�G�f�B�b�g�\�L
// Author : Masaya Hayashi
//������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef STAGE_EDIT_UI
#define STAGE_EDIT_UI

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"
#include "../SceneManager/SceneManager.h"

// ===== �N���X�̑O����` =====

// ===== �N���X��` =====
class StageEditUI final : public Board
{
public:
	StageEditUI();		// �R���X�g���N�^
	~StageEditUI();		// �f�X�g���N�^

	virtual void initialize();
	virtual void finalize();
	virtual void update();
	virtual void draw();

private:
	const std::string TexturePass = "data/Texture/EditMode.png";

	SceneManager::SceneState currentState;

};

#endif