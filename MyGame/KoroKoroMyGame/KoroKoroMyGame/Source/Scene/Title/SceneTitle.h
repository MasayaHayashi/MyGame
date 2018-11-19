//��������������������������������������������������������������
// SceneTitle.h
// Author	 : Masaya Hayashi
//��������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SCENE_TITLE
#define SCENE_TITLE

// ===== �C���N���[�h�� =====
#include "../../SceneInterface/sceneBase.h"
#include "../../Skydome/Skydome.h"
#include <memory>

// ===== �񋓑̒�` =====
enum class UIType
{
	MainUI = 0,
	EditUI,
	DescUI,
};

// ===== �N���X�̑O���錾 =====
class MainField;
class Player;
class TitleUI;
class C_PARTICLE_BASE;
class HeartObj;
class Light;
class Camera;
class FadeUI;
class Board;

// ===== �N���X��` =====
class SceneTitle final : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

	void initialize();
	void update();
	void draw();
	void finalize();

	void initializeStatus();

	Camera* getCamera();

private:
	static constexpr UINT MaxUIType = static_cast<UINT>(UIType::DescUI) + 1;

	std::unique_ptr <Camera>		cameraPtr   = nullptr;
	std::unique_ptr <Light>		    lightPtr    = nullptr;
	std::unique_ptr <Skydome>		skydomePtr  = nullptr;
	std::unique_ptr <Player>		playerPtr   = nullptr;
	std::unique_ptr <MainField>		fieldPtr    = nullptr;
	std::unique_ptr <TitleUI>		titleUiPtr  = nullptr;
	std::unique_ptr <HeartObj>		heartObjPtr = nullptr;

//	Board					*pBoard[MaxUIType];					// UI�֘A

	bool					bChangeScene;						// �V�[���؂�ւ�
	INT						nChangeSceneWaitCnt;				// �ҋ@�p�J�E���^
	UINT					uSelectScene;

};

#endif