//����������������������������������������������������������������
// StarItem.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef STAR_ITEM_OBJ
#define STAR_ITEM_OBJ

// ===== �C���N���[�h�� =====
#include "../Item/ItemBase.h"
#include <map>

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O����` =====

// ===== �񋓑̒�` =====

// ===== �\���̒�` =====

// ===== �N���X��` =====
class StarItem final : public ItemBase
{
public:
	StarItem();
	StarItem(std::string, std::string, size_t,GameObjectType, bool);
	~StarItem();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX,D3DXMATRIX);

	const std::string getKeyName();

protected:

private:
};

#endif