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

// ===== 列挙体定義 =====
enum class UIType
{
	MainUI = 0,
	EditUI,
	DescUI,
};

// ===== クラスの前方宣言 =====
class C_MAIN_FIELD;
class Player;
class TitleUI;
class C_PARTICLE_BASE;
class HeartObj;
class Light;
class Camera;
class FadeUI;
class Board;

// ===== クラス定義 =====
class SceneTitle final : public C_SCENE_BASE
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

	Camera					*pCamera;							// カメラ
	Light					*pLight;							// ライト

	Skydome					*pSkydome;							// スカイドーム


	Player				*pPlayer;
	C_MAIN_FIELD			*pField;
	TitleUI					*pTitleUI;							// タイトルUI
	HeartObj				*pTitleObj;							// タイトルオブジェクト
	Board					*pBoard[MaxUIType];					// UI関連
//	C_PARTICLE_BASE			*pParticle[MAX_PARTICLE];			// パーティクル

	bool					bChangeScene;						// シーン切り替え
	INT						nChangeSceneWaitCnt;				// 待機用カウンタ
	UINT					uSelectScene;

};

#endif