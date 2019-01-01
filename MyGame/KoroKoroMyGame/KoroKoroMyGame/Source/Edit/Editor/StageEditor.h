//����������������������������������������������������������������
// C_StageEditor.cpp
// �X�e�[�W�N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef STAGE_EDITOR_H
#define STAGE_EDITOR_H

// ===== �C���N���[�h�� =====
#include "../../Pawn/Pawn.h"
#include "../../Board/Board.h"
#include "../../SceneManager/SceneManager.h"
#include "../../SceneInterface/sceneBase.h"
#include "../../Block/Block.h"
#include <array>


// ===== �萔�E�}�N����` =====
#define MAX_STAGE     (4)
#define MAX_FILE_NAME (256)
#define STAGE_TUTORIAL_FILE_NAME "StageTutorial.bin"
#define STAGE_1_FILE_NAME		 "Stage1.bin"
#define STAGE_2_FILE_NAME		 "Stage2.bin"
#define STAGE_3_FILE_NAME		 "Stage3.bin"

// ===== �N���X�̑O���Q�� =====
class Player;
class Light;

// ===== �񋓑̒�` =====
enum EDITOR_UI_TYPE
{
	UI_WTHAT_STAGE_SAVE = 0,		// �ǂ̃X�e�[�W�ɃZ�[�u���܂���
	UI_WTHAT_STAGE_LOAD,			// �ǂ̃X�e�[�W�����[�h���܂���
	UI_STAGE,				// Stage
	UI_NUMBERS,				// ����
	UI_SAVE_OK,				// �Z�[�u����
	UI_STAGE_EDIT,
	UI_DESCR,				// ����
	MAX_EDITOR_UI_TYPE,
};

enum EDIT_MODE_TYPE
{
	MODE_EDIT = 0,		// �G�f�B�b�g���[�h
	MODE_SAVE_CONF,		// �Z�[�u�m�F���[�h
	MAX_EDIT_MODE_TYPE,
};	// ���[�h���

enum STAGE_SELECT_TYPE
{
	STAGE_SELECT1 = 0,
	STAGE_SELECT2,
	STAGE_SELECT3,
	STAGE_CANCEL,
	MAX_STAGE_SELECT_TYPE,
};	// �I�����ꂽ�X�e�[�W


// ===== �\���̒�` =====

// ===== �N���X��` =====
class StageEditor final : public Pawn
{
public:
	StageEditor();		// �R���X�g���N�^
	~StageEditor();		// �f�X�g���N�^

	void initialize();
	void finalize();
	void update();
	void draw();

	Camera* getCamera();		// �J�����擾
	
private:
	static constexpr UINT MaxGameObj = 1000;

	void saveStageData();		// �X�e�[�W��񏑂�����
	void loadStageData(UINT);	// �X�e�[�W���ǂݍ���
	void checkSaveData();		// �X�e�[�W��񏑂����݂̊m�F

	void moveObj();			// �I�u�W�F�N�g�ړ�
	void deleteObj();		// �I�u�W�F�N�g�폜
	void rotObj();			// �I�u�W�F�N�g��]
	void changeSelectObj();	// �I���I�u�W�F�N�g�؂�ւ�

	std::unique_ptr<Light>	lightPtr;
	std::unique_ptr<Camera> cameraPtr;
	std::unique_ptr<Player> playerPtr;


	std::array<std::list<Pawn*>,GameObjectBase::MaxGameObjType> gameObjPtr;

	void creteGameObj(size_t objType);

	/*
	template<class T>
	void creteGamesObj(T dataType, size_t size);
	*/

	Board 					*pUIObj[MAX_EDITOR_UI_TYPE];

	//C_COLLISION				*pCollision;								// �Փ˗p
	//C_PAUSE					*pPause;									// �|�[�Y�p

	UINT					uSaveStage;									// �ۑ���X�e�[�W�ԍ�

	UINT					uExportBlockCnt[2];			// �o�͗p�J�E���^
	UINT					CurrentSelectType;							// ���݂̑I���I�u�W�F�N�g�̎��
	UINT					uSelectObjNum[2];			// ���݂̑I������Ă���I�u�W�F�N�g�̎��ʔԍ�
	CHAR					szStageFileName[MAX_STAGE][MAX_FILE_NAME];	// �X�e�[�W�ǂݍ��݂̃t�@�C����
	
	FILE					*fp;										// �t�@�C���|�C���^
	errno_t 				nErrorFopen;								// �t�@�C���I�[�v���t���O
	ExportData				ExportWorkData;								// �����o���p�f�[�^�ۑ��ꏊ

	bool					bPause;
	bool					bIsSelectSaveStage;							// �Z�[�u�X�e�[�W��I�񂾂�
	UINT					uSelectMode;								// ���݂̑I�����[�h
};

#endif