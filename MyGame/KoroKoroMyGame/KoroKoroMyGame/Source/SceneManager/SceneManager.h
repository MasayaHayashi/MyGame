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
#include <map>


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
		SceneMain,
		SceneStageEdit,
		SceneResult,
	};

	enum class FadeState
	{
		FadeNone = 0,
		FadeIn,
		FadeOut,
	};
	static bool create();

	static void initialize();
	static void update();
	static void updateFade();
	static void draw();
	static void drawFade();
	static void finalize();

	static void changeScene(SceneState setScene);
	static void setNextScene(SceneState setNextScene);
	static void setCurrentScene(SceneState setSceneType);

	static SceneBase*			getInstanse();
	static const SceneState		getCurrentSceneType();
	static const SceneState		getNextScene();
	const FadeUI*				getFade() const;

protected:

private:

	void loadSettingFile();
	void makeStartScene();
	void changeStringToUint();

	static constexpr INT DebugMoveOnFream = 10;
	static SceneState currentSceneType;
	static SceneState nextSceneType;

	static std::unique_ptr<SceneManager> sceneManagerInstancePtr;
	static std::unique_ptr<SceneBase>	 currentScenePtr;
	static std::unique_ptr <FadeUI>		 fadePtr;

	std::map<std::string, SceneState> sceneTypeMap =
	{
		{ "SceneTitle" ,     SceneState::SceneTitle },
		{ "SceneMain",		 SceneState::SceneMain },
		{ "SceneStageEdit",	 SceneState::SceneStageEdit },
		{ "SceneResult",	 SceneState::SceneResult },
	};

	TCHAR  startScene[256];

	static bool	   debugMode;
	static INT	   frameAdvanceCnt;
};

#endif