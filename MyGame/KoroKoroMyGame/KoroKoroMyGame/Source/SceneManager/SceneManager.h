//����������������������������������������������������
// SceneManager.h
// Author : Masaya Hayashi
//����������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

// ===== �C���N���[�h�� =====
#include "../SceneInterface/sceneBase.h"
#include "d3dx9.h"
#include <memory>

// ===== �N���X�̑O���錾 =====
class FadeUI;

// ===== �N���X��` =====
class SceneManager final
{
public:

	SceneManager();
	~SceneManager();

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

	static void initialize();
	static bool create();
	void update();
	void updateFade();
	static void draw();
	void drawFade();
	static void finalize();

	static void changeScene(SceneState setScene);
	static void setNextScene(SceneState setNextScene);
	static void setCurrentScene(SceneState setSceneType);

	static C_SCENE_BASE*		getInstanse();
	static const SceneState		getCurrentSceneType();
	static const SceneState		getNextScene();
	const FadeUI*				getFade();

protected:

private:

	static constexpr INT DebugMoveOnFream = 10;
	static SceneState currentSceneType;
	static SceneState nextSceneType;

	static std::unique_ptr<SceneManager> sceneManagerInstancePtr;
	static std::unique_ptr<C_SCENE_BASE> currentScenePtr;
	static std::unique_ptr <FadeUI>		 fadePtr;

	bool	   debugMode;
	INT		   frameAdvanceCnt;				// �f�o�b�N�p�R�}����p�J�E���^
};

#endif