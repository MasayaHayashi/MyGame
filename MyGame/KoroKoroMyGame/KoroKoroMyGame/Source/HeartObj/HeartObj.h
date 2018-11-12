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

	const std::string ModelFilePass		= "Data/Model/Heart/heart.x";
	const std::string TextureFilePass	= "Data/Model/Heart/heart.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

#define MODEL_NAME	 "data/MODEL/Cute/heart.x"
#define TEXTURE_NAME "data/TEXTURE/heart.png"
#define VALUE_SCALE_SIZE (1.0f)		// �g�嗦
#define ROT_SPEED (50.0f)			// ��]���x

	

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
