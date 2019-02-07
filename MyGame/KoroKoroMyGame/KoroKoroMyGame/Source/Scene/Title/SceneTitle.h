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
class Light;
class Collision;
class Skydome;
class Player;
class MainField;
class TitleUI;
class HeartObj;
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

private:
	static constexpr UINT MaxUIType = static_cast<UINT>(UIType::DescUI) + 1;

	std::unique_ptr <Camera>		cameraPtr   = nullptr;
	std::unique_ptr <Light>		    lightPtr    = nullptr;
	std::unique_ptr <Skydome>		skydomePtr  = nullptr;
	std::unique_ptr <Player>		playerPtr   = nullptr;
	std::unique_ptr <MainField>		fieldPtr    = nullptr;
	std::unique_ptr <TitleUI>		titleUiPtr  = nullptr;
	std::unique_ptr <HeartObj>		heartObjPtr = nullptr;

	std::list<std::unique_ptr<Board>> boardPtres;
	std::unique_ptr<Collision>		  collisionPtr;
};

#endif