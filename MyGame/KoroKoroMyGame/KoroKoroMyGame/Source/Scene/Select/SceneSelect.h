//������������������������������������������������
// SceneSelect.h
// �V�[���Z���N�g
// Author : Masaya Hayashi
//������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../../SceneInterface/sceneBase.h"
#include "../../DirectX3D/DirectX3D.h"
#include "../../SelectManager/SelectManager.h"
#include <memory>
#include <list>

// ===== �񋓑̒�` =====

// ===== �N���X�̑O���錾 =====
class Light;
class Camera;
class Player;
class Pawn;
class Board;
class BallObj;

// ===== �N���X��` =====
class SceneSelect : public SceneBase
{
public:
	SceneSelect();
	~SceneSelect();

	void initialize();
	void finalize();
	void update();
	void draw();

	void initializeStatus();

	Camera* getCamera();

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

	void checkUnProject(INT);
	void checkCollision();
	void setScore();

	static constexpr INT ReStartTime = 120;

	std::unique_ptr<Light>		lightPtr;
	std::unique_ptr<Camera>		cameraPtr;

	std::list<std::unique_ptr<Player>>	 playeresPtr;
	std::list<std::unique_ptr<Pawn>>	 gameObjectesPtr;
	std::list<std::unique_ptr<Board>>	 boardObjectesPtr;

	GameState				currentGameState;
	static UINT				currentStage;
	static UINT				prevScore;
};