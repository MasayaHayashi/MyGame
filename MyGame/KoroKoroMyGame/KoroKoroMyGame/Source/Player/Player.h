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

	void updateSceneEdit();
	void updateGameMain(D3DXVECTOR3);

	void setStatus(Player::PlayerState setStatus);
	PlayerState getState()   const;

	void setScore(INT);
	void addScore();
	INT  getScore() const;
private:
	const CHAR* ModelFilePass = "Data/Model/Character/PenChan/PenguinC.x";
	static constexpr FLOAT MoveForwardSpeed = 0.45f;
	static constexpr FLOAT MoveSideSpeed	= 0.3f;
	static constexpr FLOAT ScaleSize		= 1.0f;

	void initializeTitle();
	void initializeSceneEdit();
	void initializeGameMain();
	void initializeResult();
	void updateTitle(D3DXVECTOR3);
	void updateResult();

	void changeStatus();	// �X�e�[�^�X�ύX����
	void changeState();		// ��ԕύX����
	
	D3DXVECTOR3		testVec;

	PlayerState				 playerStateType;

	INT score;

	D3DXVECTOR3		oldPos;
	D3DXQUATERNION	startQuaternion;

};

#endif 
