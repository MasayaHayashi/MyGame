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

// ===== �O���錾 =====
class SceneManager;
enum class SceneState;

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

	FadeUI();
	~FadeUI();

	void initialize();
	void update();
	const void draw();
	void finalize();

	void setColor(D3DCOLOR col);
	void setFade(FadeType Fade);
	void setWorkScene(SceneManager::SceneState);
	
	FadeType getFadeState();


protected:
private:
	std::string TextureName = "data/TEXTURE/test.png";


	SceneManager::SceneState CurrentScene;
	FadeType  CurentFadeType;		// ���݂̃t�F�[�h���
	D3DXCOLOR CurrentColor;			// ���ݐF���
	SceneManager::SceneState WorkScene;
};

#endif
