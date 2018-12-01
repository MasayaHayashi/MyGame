//
// SceneTitle.h
// Author	 : Masaya Hayashi
//

// ===== ½dCN[hh~ =====
#ifndef SCENE_TITLE
#define SCENE_TITLE

// ===== CN[h =====
#include "../../SceneInterface/sceneBase.h"
#include "../../Skydome/Skydome.h"
#include <memory>

// ===== ñÌè` =====
enum class UIType
{
	MainUI = 0,
	EditUI,
	DescUI,
};

// ===== NXÌOûé¾ =====
class MainField;
class Player;
class TitleUI;
class C_PARTICLE_BASE;
class HeartObj;
class Light;
class Camera;
class FadeUI;
class Board;
class Texture;

// ===== NXè` =====
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
	std::unique_ptr <Texture>		texturePtr  = nullptr;

//	Board					*pBoard[MaxUIType];					// UIÖA

	bool					bChangeScene;						// V[ØèÖ¦
	INT						nChangeSceneWaitCnt;				// Ò@pJE^
	UINT					uSelectScene;

};

#endif