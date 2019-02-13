//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_SceneMain.h
// シーンメインクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "../../SceneInterface/sceneBase.h"
#include "../../DirectX3D/DirectX3D.h"
#include "../../Pawn/Pawn.h"
#include <memory>
#include <list>
#include <array>

// ===== 列挙体定義 =====

// ===== クラスの前方宣言 =====
class Light;
class Camera;
class Player;
class Collision;
class BallObj;
class Board;
class Skydome;

// ===== クラス定義 =====
class SceneMain final : public SceneBase
{
public:
	SceneMain();
	~SceneMain();

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();
	bool loadStageData(size_t stageNumber);

private:

	enum class GameState
	{
		Ready = 0,
		Tutorial,
		Normal,
		Pause,
		Goal,
		Miss,
	};

	const std::string StageFilePass = "Stage";

	static constexpr UINT	MaxGameObj	 = 512;
	ExportData				exportWorkData;
	size_t					selectGameObjIndex;

	void creteGameObj(size_t objType);
	void createItem(size_t index);

	static constexpr INT ReStartTime = 120;

	const std::string StageTutorialPass = "StageTutorial.bin";
	const std::string Stage1Pass		= "Stage1.bin";
	const std::string Stage2Pass		= "Stage2.bin";
	const std::string Stage3Pass		= "Stage3.bin";

	std::unique_ptr<Light>		lightPtr;
	std::unique_ptr<Camera>		cameraPtr;
	std::unique_ptr<Collision>	collisionPtr;

	std::list<std::unique_ptr<Player>>														playeresPtr;
	std::list<std::unique_ptr<Skydome>>														skyDomePtr;
	std::list<std::unique_ptr<Board>>														boardObjectesPtr;
	std::array<std::list<std::unique_ptr<Pawn>>, GameObjectBase::MaxGameObjType>			gameObjPtr;


};