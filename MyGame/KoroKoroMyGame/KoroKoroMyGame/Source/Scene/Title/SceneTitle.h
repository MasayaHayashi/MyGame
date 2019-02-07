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
class Light;
class Collision;
class Skydome;
class Player;
class MainField;
class TitleUI;
class HeartObj;
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