//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SceneStageEdit.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SCENE_STAGE_EDIT_H
#define SCENE_STAGE_EDIT_H

// ===== インクルード部 =====
#include "../../SceneInterface/sceneBase.h"
#include "../../Edit/Editor/StageEditor.h"
#include <memory>

// ===== 定数・マクロ定義 =====

// ===== 列挙体定義 =====

// ===== クラスの前方宣言 =====
class Camera;
class Skydome;

// ===== クラス定義 =====
class SceneStageEdit : public SceneBase
{
public:
	SceneStageEdit();
	~SceneStageEdit();

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();

private:

	std::unique_ptr<Skydome>		skydomePtr;
	std::unique_ptr<StageEditor>	stageEditorPtr;
};

#endif