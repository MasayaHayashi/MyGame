//����������������������������������������������������������
// C_Camera.h
// Author : MasayaHayashi
// �J�����N���X
//����������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef C_CAMERA_H
#define C_CAMERA_H

// ===== �C���N���[�h�� ====
#include "d3dx9.h"
#include "../SceneManager/SceneManager.h"
#include "../Camera/camera.h"
#include <vector>

// ===== �萔�E�}�N����` =====
#define	VIEW_ANGLE			(D3DXToRadian(55.0f))	// ����p
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z			(1.0f)					// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(70000.0f)				// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA	(2.0f)					// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.003809f)		// �J�����̉�]��

#define	INTERVAL_CAMERA_L			(12.5f)			// ���f���̎����̐�܂ł̋���
#define	INTERVAL_CAMERA_L_MOVE		(20.5f)			// ���f���̎����̐�܂ł̈ړ���

#define	CHASE_HEIGHT_P		(100.0f)				// �ǐՎ��̎��_�̍���
#define	CHASE_HEIGHT_L		(10.0f)					// �ǐՎ��̒����_�̍���

#define RATE_CHASE_CAMERA_P	(0.3f)					// �J�������_�ւ̕␳�W��
#define RATE_CHASE_CAMERA_L	(0.2f)					// �J���������_�ւ̕␳�W��

#define MAX_CAMERA_FADE_POS (3)

// ===== �񋓑̒�` =====
enum CAMERA_MOVE_TYPE
{
	MAIN_TUTORIAL,
	MAIN_START_FADE,
	MAIN_NORMAL,
	MAIN_GOAL_FADE,
	MAX_CAMERA_MOVE_TYPE,
};

// ===== �N���X�̑O���錾 =====
class SceneManager;
class Pawn;

class C_PLAYER;
class C_GAMEOBJ_BASE;
class C_BOARD;

// ===== �N���X��` =====
class Camera
{
public:
	Camera();
	~Camera();

	void initialize();
	void finalize(C_PLAYER* pPlayer);
	void finalize();
	void update(C_PLAYER *,C_BOARD *);
	void updateTitle(Pawn*);
	void updateStageEdit(const D3DXVECTOR3&);
	void updateGameMain(C_PLAYER*,C_BOARD*);
	void setState(CAMERA_MOVE_TYPE setState);


	void setCamera();

	D3DXVECTOR3  getRot();
	D3DXVECTOR3  getPos();
	D3DXVECTOR3  getLook();
	D3DXVECTOR3* getUp();

	D3DXMATRIX    getProjectionMtx();	// �v���W�F�N�V�����}�g���b�N�X�擾
	D3DXMATRIX    getMtxView();			// �r���[�}�g���b�N�X�擾
	D3DVIEWPORT9* getViwPort();			// �r���[�|�[�g�}�g���b�N�X�擾

	D3DXVECTOR3 getCameraFowerd();

private:
	enum class CameraState
	{
		Type3Person,
		Type1Person,
		TypePoint
	};

	static constexpr FLOAT CameraRotSpeed = 22.0f;

	void initTitle(C_PLAYER * pPlayer);		// �^�C�g����ʃJ����������
	void initStageEdit();					// �X�e�[�W�G�f�B�b�g�p�J����������
	void initGameMain(C_PLAYER *pPlayer);	// �Q�[�����C���p������

	void rotationCamera(D3DXVECTOR3 Center);


	D3DXVECTOR3 fadePos[MAX_CAMERA_FADE_POS];
	D3DXVECTOR3 fadeLook[MAX_CAMERA_FADE_POS];

	void RotMove(D3DXVECTOR3* pVecCenter, FLOAT fRadius);

	CAMERA_MOVE_TYPE cameraMoveFade;

	SceneManager::SceneState currentScene;  // �V�[�����ʗp
	D3DXVECTOR3		cameraPos;				// �J�����̎��_
	D3DXVECTOR3		cameraLook;				// �J�����̒����_
	D3DXVECTOR3		cameraUp;				// �J�����̏����

	D3DXVECTOR3		cameraPosDest;			// �J�����̎��_�̖ړI�ʒu
	D3DXVECTOR3		cameraLookDest;			// �J�����̒����_�̖ړI�ʒu
	D3DXVECTOR3		cameraUpDest;			// �J�����̏�����ړI�ʒu

	D3DXVECTOR3		cameraRot;				// �J�����̉�]�p�x
	FLOAT			lengthIntervalCamera;	// �J�����̎��_�ƒ����_�̋���
	FLOAT			heightCameraP;			// �J�����̎��_�̍���
	FLOAT			heightCameraL;			// �J�����̒����_�̍���
	D3DXVECTOR3		cameraFowerd;			// �J�����̑O�����x�N�g��
	D3DXMATRIX		mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		mtxRot;					// �J������]�s��
	D3DXMATRIX		mtxMove;				// �J�����ړ��s��

	FLOAT			radShake;				// ��ʂ�h�炷�ۂɗp����R�T�C���J�[�u�p�̊p�x
	INT				cntShakeFrame;			// �J������h�炵�Ă���t���[���̃J�E���^
	D3DXVECTOR3		moveCamera;				// �J�������W�̈ړ����x			

	FLOAT			rotRadian;				// ��]�p�x(���W�A��)
	FLOAT			rotPitchRadian;		// ��]�p�x(���W�A��)

	FLOAT			numMovePitch;			// �s�b�`�̈ړ���
	D3DXVECTOR3		rotMoveCamera;			// �p�x����̈ړ������x�N�g��
	D3DXVECTOR3		moveCameraDest;		// �J�����̈ړ���

	D3DXVECTOR3		movePitchTest;	

	D3DXVECTOR3		rotWorkX;
	D3DXVECTOR3		rotWorkY;
	D3DXVECTOR3		rotWorkZ;

	D3DXMATRIX mW, mY, mX, mZ;
	D3DXVECTOR3 vX;
	D3DXVECTOR3 vY;
	D3DXVECTOR3 vZ;
	D3DXVECTOR3 vP;

	D3DXMATRIX rotCameraX;
	D3DXMATRIX rotCameraZ;

	D3DVIEWPORT9 viewPort;

	CameraState currentState;
	bool changeCamera;
	FLOAT lerpCnt;	// ���`��ԗp�J�E���^

	INT   rotCnt;
	FLOAT rot;
};

#endif