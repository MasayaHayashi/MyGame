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
#include <vector>

// ===== �萔�E�}�N����` =====
#define MAX_GAME_OBJ (5000)

// ===== �N���X�̑O����` =====
class C_COLLIDER;

// ===== �񋓑̒�` =====

// ===== �\���̒�` =====

// ===== �N���X��` =====
class Block final : public Pawn
{
public:
	Block();
	Block(UINT);
	~Block();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX,D3DXMATRIX);

	void SetSelectNum(UINT);	// ���݂̑I��ԍ�
//	EXPORT_BLOCK_DATA*	GetExportData();			// �����o���p�f�[�^�擾
//	void SetExportData(EXPORT_BLOCK_DATA SetData);	// �o�͗p�f�[�^�Z�b�g

protected:
	//void UpdateExportData();					// �o�͗p�f�[�^�X�V

//	EXPORT_BLOCK_DATA	ExportData;				// �����o���ۂ̕ۑ��p�f�[�^
//	BLOCK_TYPE			BlockType;
//	C_COLLIDER		    *pCollider;
	UINT				uCurrentSelect;
private:

	const std::string ModelPass		= "data/MODEL/box.x";
	const std::string TexturePass	= "data/TEXTURE/IceStone.png";


};

#endif
