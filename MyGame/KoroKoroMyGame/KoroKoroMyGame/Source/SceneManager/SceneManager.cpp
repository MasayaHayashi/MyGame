//
// C_GameSceneManager.cpp
// Q[V[ΜΗNX
// Author : Masaya Hayashi
//

// ===== CN[h =====
#include "SceneManager.h"
#include "../KeyBoard/Keyboard.h"
#include "../Scene/Main/C_SceneMain.h"
#include "../Fade/FadeUI.h"

// ===== ΓIo =====
std::unique_ptr<SceneManager> SceneManager::gameManagerInstancePtr;
SceneManager::SceneState SceneManager::currentSceneType;

//
// RXgN^
//
SceneManager::SceneManager()
{
	debugMode		= false;
	frameAdvanceCnt = DebugMoveOnFream;

	// C_SCENE_STAGE_EDIT
	// SCENE_STAGE_EDIT

	// C_SCENE_MAIN
	// SCENE_MAIN

	// C_SCENE_TITLE
	// SCENE_TITLE


	// V[έθϊ»
	currentScenePtr.reset(new SceneMain());
	currentSceneType	= SceneState::SceneMain;
	nextSceneType		= SceneState::SceneTitle;

	// tF[hϊ»
	fadePtr.reset(new FadeUI);
	fadePtr->InitObject();
}

//
// fXgN^
//
SceneManager::~SceneManager()
{
	fadePtr->finalizeObject();
}

//
// V[ϊ»
//
void SceneManager::initialize()
{
	// V[ϊ»
	currentScenePtr->InitScene();
}

//
// V[IΉ
//
void SceneManager::finalize()
{
	// V[γ
	currentScenePtr->finalizeScene();
}

//
// V[XV
//
void SceneManager::update()
{
	// fobO[hΨΦ¦
	if (Keyboard::getTrigger(DIK_V))
		debugMode = !debugMode;

	// fobO[h»θ
	if (debugMode)
	{
		if(Keyboard::getPress(DIK_SPACE))
			frameAdvanceCnt --;

		if (frameAdvanceCnt <= 0)
		{
			frameAdvanceCnt = DebugMoveOnFream;
			currentScenePtr->updateScene();
		}
	}
	else
		// V[XV
		currentScenePtr->updateScene();
}

//
// tF[hXV
//
void SceneManager::updateFade()
{
	fadePtr->updateObject();
}

//
// V[`ζ
//
void SceneManager::draw()
{
	// V[`ζ
	currentScenePtr->drawScene();
}

//
// tF[hV[`ζ
//
void  SceneManager::drawFade()
{
	if (fadePtr->getFadeState() != FadeUI::FadeType::FadeNone)
	{
		// tF[h`ζ
		fadePtr->drawObject();
	}
}

//
// V[Zbg
//
void SceneManager::changeScene(SceneState Scene)
{
	switch (Scene)
	{
	case SceneState::SceneTitle:
		delete currentScenePtr.get();
		currentScenePtr = new SceneTitle();
		break;
	case SceneState::SceneMain:
		delete currentScenePtr.get();
		currentScenePtr = new SceneMain();
		break;
	case SceneState::SceneResult:
		delete currentScenePtr.get();
		currentScenePtr = new SceneResult();
		break;
	case SceneState::SceneStageEdit:
		delete currentScenePtr.get();
		currentScenePtr = new SceneStageEdit();
		break;
	// αO
	default:
		break;
	}
}

//
// »έΜV[ΜCX^XζΎ
//
C_SCENE_BASE* SceneManager::getInstanse()
{
	return currentScenePtr.get();
}

//
// »έΜV[enumζΎ
//
SceneManager::SceneState SceneManager::getCurrentSceneType()
{
	return currentSceneType;
}

//
// O©ηΜV[πZbg([hV[p)
//
void SceneManager::setCurrentScene(SceneState setStateType)
{
	currentSceneType = setStateType;
}

//
// ΜV[πZbg
//
void SceneManager::setNextScene(SceneState setNextSceneType)
{
	nextSceneType = setNextSceneType;
	fadePtr->setFade(FadeUI::FadeType::FadeOut);
}

//
// ΜV[ζΎ([hV[p)
//
SceneManager::SceneState SceneManager::getNextScene()
{
	return nextSceneType;
}

//
// tF[hζΎ
//
FadeUI* SceneManager::getFade()
{
	return fadePtr.get();
}