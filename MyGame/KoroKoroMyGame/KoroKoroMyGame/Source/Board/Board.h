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
enum class  BoardType
{
	Polygon2d = 0,
	Billboard,
	Polygon3d,
};

// ===== �N���X��` =====
class Board final : public GameObjectBase
{
public:
	Board();
	virtual ~Board();
	virtual void initialize();
	virtual void update();
	virtual const void draw();
	virtual void initializeStatus();
	virtual void finalize();

	bool makeVertexBoard();	
	bool makeVertexBoard(D3DXVECTOR3);		// ���_��񐶐�
	void setTexture();						// UV���W�Z�b�g
	void setVtxBoard();						// ���_���Z�b�g
	void setColor();						// ���_�J���[�Z�b�g
	bool createTexture(TEXTURE_DATA &Texture);

	void setNumber(BYTE);					// �g�p�ԍ��Z�b�g
	void setAnimation();					// �A�j���[�V�����Z�b�g
	void setPosition(D3DXVECTOR3);			// �ʒu�Z�b�g
	void setUsedFlg(bool Setflg);			// �g�p�t���O�Z�b�g
	void setMoveNum(D3DXVECTOR3 MoveNum);	// �ړ��ʃZ�b�g
	void setCnt(FLOAT fSet);				// �J�E���^�Z�b�g
	void setCnt(INT nSet);					// �J�E���^�Z�b�g
	void setCurrentAnimPattern(INT nAnim);	// �A�j���[�V�����Z�b�g
	void setStartCurvePos(D3DXVECTOR3 );	// �x�W�F�Ȑ��A�G���~�[�g�Ȑ��p�X�^�[�g�ʒu�Z�b�g
	void setNarrowFlg(bool bSet);
	void destroyBoard();					// �|���S�����


	// �Q�b�^�[
	LPDIRECT3DTEXTURE9		getTexture();		// �e�N�X�`����񐶐�
	LPDIRECT3DVERTEXBUFFER9 getVtxBuff();		// ���_��񐶐�
	BOOL					getUsedFlg();		// �g�p�t���O�擾 
	D3DXVECTOR3				getPosition();		// ���W�擾
	D3DXVECTOR3				getSize();			// �傫���擾
	FLOAT					getCurveCnt();		// ���`��ԗp�J�E���^�擾
	UINT					getCurrentAnim();	// �A�j���p�^�[���擾

	// �Z�b�^�[

protected:
	static constexpr INT MaxLerp = 4;

	VERTEX_BOARD_DATA		vertexBoard;		// ���_���f�[�^
	TEXTURE_DATA			texture;			// �e�N�X�`���f�[�^

	CHAR					fileName[256];	// �ǂݍ��ރt�@�C����
	LPDIRECT3DTEXTURE9		pD3DTextureBoard;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffBoard;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuffBoard;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				posBoard;			// �ʒu
	D3DXVECTOR3				sizeBoard;			// �傫��
	D3DXVECTOR3				rotBoard;			// ��]
	D3DXVECTOR3				scaleBoard;			// �g�嗦
	D3DXVECTOR3				posDestBoard;		// �ړI�ʒu
	D3DXVECTOR3				moveBoard;			// �ړ���
	D3DXVECTOR3				curvePos[MaxLerp];	// �x�W�F�Ȑ��A�G���~�[�g�Ȑ��p�x�N�g��(�O����)
	D3DXMATRIX				worldMtxBoard;		// ���[���h�s��
	D3DXCOLOR				color;				// �F
	D3DMATERIAL9			mat;				// �}�e���A��
	FLOAT					radAngleBoard;		// �|���S���̉�]�p�x(���W�A���l)
	FLOAT					rotSpeedBoard;		// �|���S���̉�]���x
	bool					alphaBlendBoard;	// �e�N�X�`���̉��Z�����ݒ�
	BoardType				boardType;			// 2D�|���S�����A�r���{�[�h���̎���

	// �e�N�X�`���֘A
	INT						texPatternDivideX;		// �e�N�X�`����������X
	INT						texPatternDivideY;		// �e�N�X�`����������Y
	INT						animPattern;			// �A�j���[�V�����̃p�^�[����
	INT						intervalChangePattern;	// �A�j���[�V�����̐؂�ւ��t���[����
	INT						animCnt;				// �A�j���[�V�����J�E���^�[
	INT						currentAnimPattern;		// �A�j���[�V�����p�^�[���ԍ�
	FLOAT					texUV_SizeX;			// �e�N�X�`���؂�ւ��pUV���WX
	FLOAT					texUV_SizeY;			// �e�N�X�`���؂�ւ��pUV���WY

	FLOAT					fCnt;					// ���`��ԂȂǂɎg�p����J�E���^(���������_��)
	INT						nCnt;					// ���`��ԂȂǂɎg�p����J�E���^(����)


	BYTE					number;				// ���ʔԍ�
	bool					narrow;				// �k������
	bool					used;				// �g�p�t���O
private:

	D3DXMATRIX				mtxTempView;			// �t�s������߂邽�߂̃r���[�s��
	D3DXQUATERNION			tempQuaternion;			// ���[�J�����W�n�ŉ�]�����邽�߂̃N�H�[�^�j�I��
	bool					isMoveUV;				// UV���W�𓮂������ǂ���
	INT						nUVCnt;					// UV���W�ړ��p�J�E���^
	D3DXVECTOR3				MoveUV;					// UV���W�ړ��p

};

#endif
