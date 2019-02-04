//����������������������������������������������������������������
// Block.h
// �u���b�N�N���X(���N���X)
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef BLOCK_H
#define BLOCK_H

// ===== �C���N���[�h�� =====
#include "../Pawn/Pawn.h"
#include "../GameObjectBase/GameObjectBase.h"
#include <map>

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O����` =====
class C_COLLIDER;

// ===== �񋓑̒�` =====

// ===== �\���̒�` =====

// ===== �N���X��` =====
class MainObject : public Pawn
{
public:
	MainObject();
	MainObject(std::string, std::string, size_t,GameObjectType, bool);
	virtual ~MainObject();

	virtual void initialize();
	virtual void finalize();
	virtual void update();
	virtual void draw();
	virtual void draw(D3DXMATRIX,D3DXMATRIX);

	const std::string getKeyName();

protected:
	std::map<std::string, std::string>	texturePasses;
	std::map<std::string, std::string>	modelPasses;
	
	const std::string passName = "Data/Model/Block/";

private:




//	const std::string ModelPass		= "Data/Model/Block/flatAndHill.x";

};

#endif
