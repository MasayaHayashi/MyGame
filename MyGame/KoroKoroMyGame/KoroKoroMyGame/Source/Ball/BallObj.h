//����������������������������������������������������������������
// HeartObj.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef BALL_OBJ_H
#define BALL_OBJ_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"
#include "../SceneManager/SceneManager.h"

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O���錾 =====
class C_COLLIDER;

// ===== �\���̒�` =====

// ===== �񋓑̒�` =====

// ===== �N���X��` =====
class BallObj final : public Pawn
{
public:
	BallObj();
	BallObj(UINT setIndex);
	~BallObj();

	void initialize();
	void finalize();
	void update(D3DXVECTOR3 pos, D3DXVECTOR3 rotVec);
	void draw();

private:

	const std::string ModelFilePass	  = "Data/Model/Ball/Beachball.x";
	const std::string TextureFilePass = "Data/Model/Ball/Beachball_Albedo.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

	FLOAT rotCnt = 0;

	void initializeTitleObj_Title();
	void initializeTitleObj_GameMain();
	void initializeTitleObj_Result();

	void initializeScelect();
	void updateScelect();


	void UpdateTitleObj_Title();
	void updateGameMain(D3DXVECTOR3 pos, D3DXVECTOR3 rotVec);
	void UpdateTitleObj_Result();
	void move(const D3DXVECTOR3 moveVector);


	SceneManager::SceneState currentScene;
	C_COLLIDER *pCollider;
};

#endif 
