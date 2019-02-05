//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_StageEditor.cpp
// ステージクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef STAGE_EDITOR_H
#define STAGE_EDITOR_H

// ===== インクルード部 =====
#include "../../Pawn/Pawn.h"
#include "../../Board/Board.h"
#include "../../SceneManager/SceneManager.h"
#include "../../SceneInterface/sceneBase.h"
#include "../../MainObject/MainObject.h"
#include <array>
#include <fstream>


// ===== 定数・マクロ定義 =====
#define MAX_STAGE     (4)
#define MAX_FILE_NAME (256)
#define STAGE_TUTORIAL_FILE_NAME "StageTutorial.bin"
#define STAGE_1_FILE_NAME		 "Stage1.bin"
#define STAGE_2_FILE_NAME		 "Stage2.bin"
#define STAGE_3_FILE_NAME		 "Stage3.bin"

// ===== クラスの前方参照 =====
class Player;
class Light;

// ===== 列挙体定義 =====
enum EDITOR_UI_TYPE
{
	UI_WTHAT_STAGE_SAVE = 0,		// どのステージにセーブしますか
	UI_WTHAT_STAGE_LOAD,			// どのステージをロードしますか
	UI_STAGE,				// Stage
	UI_NUMBERS,				// 数字
	UI_SAVE_OK,				// セーブ完了
	UI_STAGE_EDIT,
	UI_DESCR,				// 説明
};

enum class EDIT_MODE_TYPE
{
	MODE_EDIT = 0,		// エディットモード
	MODE_SAVE_CONF,		// セーブ確認モード
};

enum class STAGE_SELECT_TYPE
{
	STAGE_SELECT1 = 0,
	STAGE_SELECT2,
	STAGE_SELECT3,
	STAGE_CANCEL,
};

enum class BoardObjType
{
	WhatSave = 0,
};

// ===== 構造体定義 =====

// ===== クラス定義 =====
class StageEditor final : public Pawn
{
public:
	StageEditor();		// コンストラクタ
	~StageEditor();		// デストラクタ

	void initialize();
	void finalize();
	void update();
	void draw();

	void updateSelectIndex();
	
private:
	static constexpr UINT	MaxGameObj = 512;
	static constexpr size_t MaxBoardObj = static_cast<size_t>(BoardObjType::WhatSave) + 1;

	const std::string StagePassName = "Stage";

	void saveStageData(size_t stageNumber);
	void loadStageData(size_t stageNumber);
	void checkSaveData();		// ステージ情報書き込みの確認

	void move();
	void move(INT input);
	void place();

	void deleteObj();		// オブジェクト削除
	void rotObj();			// オブジェクト回転
	void changeSelectObj();	// 選択オブジェクト切り替え

	std::unique_ptr<Light>	lightPtr;
	std::unique_ptr<Camera> cameraPtr;
	std::unique_ptr<Player> playerPtr;

	std::array<std::list<std::unique_ptr<Board>>,MaxBoardObj>						boardObjPtr;
	std::array<std::vector<MainObject*>,GameObjectBase::MaxGameObjType>				gameObjPtr;

	void creteGameObj(size_t objType);
	void createBoard(size_t setSize);

	INT selectGameObjType	= 0;
	INT selectGameObjIndex	= 0;

	const std::string StageFilePass = "Stage";

	std::vector<std::string> stagePass;

	/*
	template<class T>
	void creteGamesObj(T dataType, size_t size);
	*/

//	Board 					*pUIObj[MAX_EDITOR_UI_TYPE];

	//C_COLLISION				*pCollision;								// 衝突用
	//C_PAUSE					*pPause;									// ポーズ用

	UINT					uSaveStage;									// 保存先ステージ番号

	UINT					uExportBlockCnt[2];			// 出力用カウンタ
	UINT					uSelectObjNum[2];			// 現在の選択されているオブジェクトの識別番号
	CHAR					szStageFileName[MAX_STAGE][MAX_FILE_NAME];	// ステージ読み込みのファイル名
	
	FILE					*fp;										// ファイルポインタ
	errno_t 				nErrorFopen;								// ファイルオープンフラグ
	ExportData				exportWorkData;								// 書き出し用データ保存場所

	bool					bPause;
	bool					isSelectSaveStage;							// セーブステージを選んだか
	UINT					uSelectMode;								// 現在の選択モード
};

#endif