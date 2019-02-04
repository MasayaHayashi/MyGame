//����������������������������������������������������������������
// ItemBase.h
// �A�C�e���p���N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef ITEM_BASE_H
#define ITEM_BASE_H

// ===== �C���N���[�h�� =====
#include "../MainObject/MainObject.h"
#include <map>

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O����` =====

// ===== �񋓑̒�` =====

// ===== �\���̒�` =====

// ===== �N���X��` =====
class ItemBase : public MainObject
{
public:
	ItemBase();
	ItemBase(std::string, std::string, size_t,GameObjectType, bool);
	virtual ~ItemBase();

	virtual void initialize();
	virtual void finalize();
	virtual void update();
	virtual void draw();
	virtual void draw(D3DXMATRIX,D3DXMATRIX);

protected:


private:

};

#endif
