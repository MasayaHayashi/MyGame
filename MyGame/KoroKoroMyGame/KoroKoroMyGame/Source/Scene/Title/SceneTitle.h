//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneTitle.h
// Author	 : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SCENE_TITLE
#define SCENE_TITLE

// ===== インクルード部 =====
#include "../../SceneInterface/sceneBase.h"
#include "../../Skydome/Skydome.h"
#include <memory>

// ===== 列挙体定義 =====
enum class UIType
{
	MainUI = 0,
	EditUI,
	DescUI,
};

// ===== クラスの前方宣言 =====
class MainField;
class Player;
class TitleUI;
class C_PARTICLE_BASE;
class HeartObj;
class Light;
class Camera;
class FadeUI;
class Board;

// ===== クラス定義 =====
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

//	Board					*pBoard[MaxUIType];					// UI関連

	bool					bChangeScene;						// シーン切り替え
	INT						nChangeSceneWaitCnt;				// 待機用カウンタ
	UINT					uSelectScene;

};

#endif