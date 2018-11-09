//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneManager.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

// ===== インクルード部 =====
#include "../SceneInterface/sceneBase.h"
#include "d3dx9.h"
#include <memory>

// ===== クラスの前方宣言 =====
class SceneTitle;
class SceneMain;
class FadeUI;

// ===== クラス定義 =====
class SceneManager
{
public:
	enum class SceneState
	{
		SceneTitle = 0,
		SceneStageEdit,
		SceneMain,
		SceneResult,
	};

	enum class FadeState
	{
		FadeNone = 0,
		FadeIn,
		FadeOut,
	};

	SceneManager();
	~SceneManager();

	void initialize();
	void update();
	void updateFade();
	void draw();
	void drawFade();
	void finalize();

	void changeScene(SceneState setScene);
	void setNextScene(SceneState setNextScene);
	void setCurrentScene(SceneState setSceneType);

	C_SCENE_BASE*	   getInstanse();
	static SceneState  getCurrentSceneType();
	SceneState		   getNextScene();
	FadeUI*			   getFade();

protected:

private:
	static constexpr INT DebugMoveOnFream = 10;

	static SceneState currentSceneType;

	static std::unique_ptr<SceneManager> gameManagerInstancePtr;


	SceneState nextSceneType;

	std::unique_ptr<C_SCENE_BASE> currentScenePtr;
	std::unique_ptr <FadeUI>	  fadePtr;

	bool	   debugMode;					// デバッグモード管理
	INT		   frameAdvanceCnt;				// デバック用コマ送り用カウンタ
};

#endif