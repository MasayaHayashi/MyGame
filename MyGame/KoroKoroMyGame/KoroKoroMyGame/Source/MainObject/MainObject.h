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
class MainObject final : public Pawn
{
public:
	MainObject();
	MainObject(std::string, std::string,size_t,bool);
	~MainObject();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX,D3DXMATRIX);

	const std::string getKeyName();

protected:
	
private:

	std::map<std::string, std::string>	modelPasses;
	std::map<std::string, std::string>	texturePasses;

	const std::string passName = "Data/Model/Block/";


//	const std::string ModelPass		= "Data/Model/Block/flatAndHill.x";

};

#endif
