//����������������������������������������������������������������
// HeartObj.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef HEART_OBJ_H
#define HEART_OBJ_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"
#include "../SceneManager/SceneManager.h"

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O���錾 =====
class C_COLLIDER;

// ===== �\���̒�` =====

// ===== �񋓑̒�` =====

// ===== �N���X��` =====
class HeartObj final : public Pawn
{
public:
	HeartObj();
	~HeartObj();

	void initialize();
	void finalize();
	void update();
	void draw();

private:

#define TITLE_HART_ROT_SPEED (0.06f)

	FLOAT rotCnt = 0;

	void initializeTitleObj_Title();
	void initializeTitleObj_GameMain();
	void initializeTitleObj_Result();
	void UpdateTitleObj_Title();
	void UpdateTitleObj_GameMain();
	void UpdateTitleObj_Result();


	SceneManager::SceneState currentScene;
	C_COLLIDER *pCollider;
};

#endif 
