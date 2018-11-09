//������������������������������������������������������������������������������������������������������
// C_Player.cpp
// Author : Masaya Hayashi
//������������������������������������������������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef PLAYER_H
#define PLAYER_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"
#include "../SceneManager/SceneManager.h"

// ===== �萔�E�}�N����` =====
#define PLAYER_SIDE_MOVE (5)

// ===== �N���X�̑O���Q�� =====
class Collider;

// ===== �\���̒�` =====

// ===== �񋓑̒�` =====

	enum PLAYER_STATE
	{
		TYPE_STOP = 0,				// ��~
		TYPE_MOVE,					// �ړ��\
		TYPE_MOVE_HIT_WALL,			// �ړ��ł��邪�ǂɓ������Ă���
		TYPE_JUMP_UP,				// �W�����v�㏸��
		TYPE_JUMP_DOWN,				// �W�����v���~��
		TYPE_FALL,					// ������(���S��)
		TYPE_DEAD,					// ���S
		MAX_PLAYER_STATE,
	};


enum HIT_COLLISION_TYPE
{
	RAY_TRUE = 0,
	RAY_FALSE,
	NONE,
};

// ===== �N���X��` =====
class C_PLAYER final : public Pawn
{
public:
	C_PLAYER();
	~C_PLAYER();

	void InitObject();
	void UninitObject();
	void UpdateObject(D3DXVECTOR3);
	void DrawObject();

	virtual void InitStatus();

	void UpdatePlayer_SceneEdit();
	void UpdatePlayer_GameMain(D3DXVECTOR3);

	void SetStatus(PLAYER_STATE SetStatus);
	PLAYER_STATE GetState();
	D3DXVECTOR3 GetMoveVec();

	void SetScore(INT);
	void AddScore();
	INT  GetScore();
private:

	void InitPlayer_Title();
	void InitPlayer_SceneEdit();
	void InitPlayer_GameMain();
	void InitPlayer_Result();
	void UpdatePlayer_Title(D3DXVECTOR3);
	void UpdatePlayer_Result();


	void ChangeStatus();	// �X�e�[�^�X�ύX����
	void ChangeState();		// ��ԕύX����

	
	PLAYER_STATE				 PlayerState;	// �v���C���[�̏��



	INT nScore;					// �X�R�A

	D3DXVECTOR3		OldPos;
	D3DXVECTOR3		TestVec;
	D3DXQUATERNION	StartQua;

};

#endif 
