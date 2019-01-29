//����������������������������������������������������������������
// Board.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef BOARD_H
#define BOARD_H

// ===== �C���N���[�h�� =====
#include "../GameObjectBase/GameObjectBase.h"
#include "../ResoueceManager/ResourceManager.h"

// ===== �񋓑̒�` =====
enum class  boardType
{
	Polygon2d = 0,
	Billboard,
	Polygon3d,
};

// ===== �N���X��` =====
class Board : public GameObjectBase
{
public:
	Board();
	virtual ~Board();
	virtual void initialize();
	virtual void update();
	virtual void draw();
	virtual void initializeStatus();
	virtual void finalize();

	bool makevertexBoard();	
	bool makevertexBoard(D3DXVECTOR3);		// ���_��񐶐�
	void setTexture();						// UV���W�Z�b�g
	void setVtx();							// ���_���Z�b�g
	void setcolor();						// ���_�J���[�Z�b�g
	bool createTexture(TEXTURE_DATA &Texture);

	void setNumber(BYTE);					// �g�p�ԍ��Z�b�g
	void setAnimation();					// �A�j���[�V�����Z�b�g
	void setPosition(D3DXVECTOR3);			// �ʒu�Z�b�g
	void setUsedFlg(bool setflg);			// �g�p�t���O�Z�b�g
	void setvelocity(D3DXVECTOR3 velocityNum);		// �ړ��ʃZ�b�g
	void setCurrentAnimPattern(INT nAnim);	// �A�j���[�V�����Z�b�g
	void setStartCurvePos(D3DXVECTOR3 );	// �x�W�F�Ȑ��A�G���~�[�g�Ȑ��p�X�^�[�g�ʒu�Z�b�g
	void setNarrowFlg(bool bset);
	void destroyBoard();					// �|���S�����

	bool isHit(std::string keyName);
	// �Q�b�^�[
	LPDIRECT3DTEXTURE9		getTexture();		// �e�N�X�`����񐶐�
	LPDIRECT3DVERTEXBUFFER9 getVtxBuff();		// ���_��񐶐�
	BOOL					getUsedFlg();		// �g�p�t���O�擾 
	D3DXVECTOR3				getPosition();		// ���W�擾
	D3DXVECTOR3				getSize();			// �傫���擾
	INT						getCurrentAnim();	// �A�j���p�^�[���擾

protected:
	static constexpr INT MaxLerp = 4;
	static constexpr INT Vertex  = 4;
	static constexpr INT Polygon = 2;

	VERTEX_BOARD_DATA		vertexBoard;		// ���_���f�[�^
	TEXTURE_DATA			texture;			// �e�N�X�`���f�[�^

	CHAR				fileName[256];		// �ǂݍ��ރt�@�C����
	CHAR				textureFileName[256];
	LPDIRECT3DTEXTURE9		pD3DTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				pos;			// �ʒu
	D3DXVECTOR3				size;			// �傫��
	D3DXVECTOR3				rot;			// ��]
	D3DXVECTOR3				scale;			// �g�嗦
	D3DXVECTOR3				posDestBoard;		// �ړI�ʒu
	D3DXVECTOR3				curvePos[MaxLerp];	// �x�W�F�Ȑ��A�G���~�[�g�Ȑ��p�x�N�g��(�O����)
	D3DXMATRIX				worldMtx;		// ���[���h�s��
	D3DXCOLOR				color;				// �F
	D3DMATERIAL9			mat;				// �}�e���A��
	FLOAT					radAngle;		// �|���S���̉�]�p�x(���W�A���l)
	FLOAT					rotSpeed;		// �|���S���̉�]���x
	bool					isAlphaBlend;	// �e�N�X�`���̉��Z�����ݒ�
	boardType				boardType;			// 2D�|���S�����A�r���{�[�h���̎���

	// �e�N�X�`���֘A
	INT						texPatternDivideX;		// �e�N�X�`����������X
	INT						texPatternDivideY;		// �e�N�X�`����������Y
	INT						animPattern;			// �A�j���[�V�����̃p�^�[����
	INT						intervalChangePattern;	// �A�j���[�V�����̐؂�ւ��t���[����
	INT						animCnt;				// �A�j���[�V�����J�E���^�[
	INT						currentAnimPattern;		// �A�j���[�V�����p�^�[���ԍ�
	FLOAT					texUV_SizeX;			// �e�N�X�`���؂�ւ��pUV���WX
	FLOAT					texUV_SizeY;			// �e�N�X�`���؂�ւ��pUV���WY

	BYTE					idNumber;				// ���ʔԍ�
	bool					isNarrow;				// �k������
	bool					isUsed;
private:

	D3DXMATRIX				mtxTempView;			// �t�s������߂邽�߂̃r���[�s��
	D3DXQUATERNION			tempQuaternion;			// ���[�J�����W�n�ŉ�]�����邽�߂̃N�H�[�^�j�I��
	bool					isvelocityUV;				// UV���W�𓮂������ǂ���
	INT						nUVCnt;					// UV���W�ړ��p�J�E���^
	D3DXVECTOR3				velocityUV;					// UV���W�ړ��p

};

#endif
