//����������������������������������������������������������������
// FadeUI.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef FADE_UI
#define FADE_UI

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"
#include "../SceneManager/SceneManager.h"

// ===== �萔�E�}�N����` =====

// ===== �N���X��` =====
class FadeUI final : public Board
{
public:
	enum class FadeType
	{
		FadeNone = 0,		// �����Ȃ����
		FadeIn,			// �t�F�[�h�C������
		FadeOut,			// �t�F�[�h�A�E�g����
		FadeComp,			// �t�F�[�h����
	};

	FadeUI();			// �R���X�g���N�^
	~FadeUI();			// �f�X�g���N�^

	void initialize();
	void Update();
	void Draw();
	void Uninit();

	void SetColor(D3DCOLOR col);
	void SetFade(FadeType Fade);
	void SetWorkScene(C_SCENE_MANAGER::SCENE_STATE);
	
	FadeType GetFadeState();


protected:
private:

	SceneManager::SCENE_STATE CurrentScene;
	FadeType  CurentFadeType;		// ���݂̃t�F�[�h���
	D3DXCOLOR CurrentColor;			// ���ݐF���
	SceneManager::SCENE_STATE WorkScene;
};

#endif
