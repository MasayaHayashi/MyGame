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

// ===== ñÌè` =====
enum class UIType
{
	MainUI = 0,
	EditUI,
	DescUI,
};

// ===== NXÌOûè` =====
class C_MAIN_FIELD;
class C_PLAYER;
class C_TITLE_UI;
class C_PARTICLE_BASE;
class C_TITLE_OBJ;
class C_CAMERA;
class Light;
class C_FADE;
class Board;

// ===== NXè` =====
class SceneTitle final : public C_SCENE_BASE
{
public:
	SceneTitle();
	~SceneTitle();

	void initialize();
	void update();
	const void draw();
	void finalize();

	void initStatus();

	C_CAMERA* getCamera();


private:
	static constexpr UINT MaxUIType = static_cast<UINT>(UIType::DescUI) + 1;

	C_CAMERA				*pCamera;							// J
	Light					*pLight;							// Cg

	Skydome					*pSkydome;							// XJCh[


	C_PLAYER				*pPlayer;
	C_MAIN_FIELD			*pField;
	C_TITLE_UI				*pTitleUI;							// ^CgUI
	C_TITLE_OBJ				*pTitleObj;							// ^CgIuWFNg
	Board					*pBoard[MaxUIType];					// UIÖA
	C_PARTICLE_BASE			*pParticle[MAX_PARTICLE];			// p[eBN

	bool					bChangeScene;						// V[ØèÖ¦
	INT						nChangeSceneWaitCnt;				// Ò@pJE^
	UINT					uSelectScene;

};

#endif