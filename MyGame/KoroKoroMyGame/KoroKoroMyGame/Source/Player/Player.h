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

// ===== �N���X�̑O���錾 =====
class Collider;

// ===== �\���̒�` =====


enum HIT_COLLISION_TYPE
{
	RAY_TRUE = 0,
	RAY_FALSE,
	NONE,
};

// ===== �N���X��` =====
class Player final : public Pawn
{
public:
	Player();
	~Player();

	enum class PlayerState
	{
		Stop = 0,	
		Move,
		MoveHitWall,			// �ړ��ł��邪�ǂɓ������Ă���
		JumpUp,				// �W�����v�㏸��
		JumpDown,			// �W�����v���~��
		Fall,
		Dead
	};

	void initialize();
	void finalize();
	void update(D3DXVECTOR3);
	void draw();

	virtual void initializeStatus();

	void UpdatePlayer_SceneEdit();
	void UpdatePlayer_GameMain(D3DXVECTOR3);

	void setStatus(Player::PlayerState setStatus);
	PlayerState getState();
	D3DXVECTOR3 getMoveVec();

	void SetScore(INT);
	void AddScore();
	INT  GetScore();
private:
	//.. / Data / Model / Character / PenChan /
	const CHAR* ModelFilePass = "Data/Model/Character/PenChan/PenguinC.x";

	void initializeTitle();
	void initializeSceneEdit();
	void initializeGameMain();
	void initializeResult();
	void updateTitle(D3DXVECTOR3);
	void updateResult();


	void ChangeStatus();	// �X�e�[�^�X�ύX����
	void ChangeState();		// ��ԕύX����
	
	PlayerState				 playerStateType;

	INT nScore;

	D3DXVECTOR3		OldPos;
	D3DXVECTOR3		TestVec;
	D3DXQUATERNION	StartQua;

};

#endif 
