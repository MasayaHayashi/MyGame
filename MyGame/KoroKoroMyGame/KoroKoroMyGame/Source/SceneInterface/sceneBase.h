//����������������������������������������������������
// C_SceneBase.h
// Author : Masaya Hayashi
//����������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef C_SCENEBASE_H
#define C_SCENEBASE_H

// ===== �N���X�̑O���錾 =====
class C_CAMERA;

// ===== �C���N���[�h�� =====
#include "../Audio/\audiere.h"
#include "d3dx9.h"

// ===== �񋓑̒�` =====
enum class GAME_STATE
{
	GAME_READY = 0,	// �������
	GAME_TUTORIAL,	// �`���[�g���A���\��
	GAME_NORMAL,	// �ʏ���
	GAME_PAUSE,		// �|�[�Y���
	GAME_GOAL,		// �S�[�����
	GAME_MISS,		// �������
	MAX_GAME_STATE,
};

// ===== �N���X��` =====
class C_SCENE_BASE
{
public:
	C_SCENE_BASE();
	virtual ~C_SCENE_BASE();

	virtual void InitScene()   = 0;
	virtual void finalizeScene() = 0;
	virtual void updateScene() = 0;
	virtual void drawScene()   = 0;

	virtual void InitStatus()  = 0;		// �e��X�e�[�^�X���Z�b�g

	// �Q�b�^�[�֘A
	virtual C_CAMERA* getCamera() = 0;

	GAME_STATE getGameState();

protected:
	audiere::OutputStreamPtr bgmPtr;
	audiere::SoundEffectPtr	 sePtr;

	UINT gameState;
	
private:
};

#endif