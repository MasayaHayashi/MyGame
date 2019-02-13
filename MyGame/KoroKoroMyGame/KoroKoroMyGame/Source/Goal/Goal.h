//����������������������������������������������������������������
// Block.h
// �u���b�N�N���X(���N���X)
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef GOAL_H
#define GOAL_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"
#include "../GameObjectBase/GameObjectBase.h"
#include "../MainObject/MainObject.h"
#include <map>

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O����` =====

// ===== �񋓑̒�` =====

// ===== �\���̒�` =====

// ===== �N���X��` =====
class Goal final : public MainObject
{
public:
	Goal();
	Goal(std::string keyName, std::string textureName, size_t setNumber);
	~Goal();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX,D3DXMATRIX);

	const std::string Goal::getKeyName();

protected:
	
private:

	std::map<std::string, std::string> modelPasses;

	const std::string TexturePass	= "Data/Texture/double_1.png";

};

#endif
