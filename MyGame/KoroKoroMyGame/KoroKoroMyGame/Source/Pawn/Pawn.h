//����������������������������������������������������������������
// Pawn.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef PAWN_H
#define PAWN_H

// ===== �C���N���[�h�� =====
#include "d3dx9.h"
#include "../GameObjectBase/GameObjectBase.h"
#include "../Mesh/Mesh.h"
#include "../MyHierarchy/MyHierarchy.h"
#include "../ResoueceManager/ResourceManager.h"
#include <vector>
#include <memory>

// ===== �񋓑̒�` =====
enum GAME_OBJ_TYPE
{
	NORMAL_BLOCK_OBJ = 0,	// �ʏ�u���b�N
	MOVE_BLOCK_OBJ,			// �ړ��u���b�N
	STAR_OBJ,				// ��
	GOAL_OBJ,				// �S�[��
	MAX_GAME_OBJ_TYPE,
};	// �I�u�W�F�N�g�̎��

enum class MeshObjType
{
	NormalModel,
	HierarchyModel,	
};

// ===== �萔�E�}�N����` =====
#define MAX_BLOCK_TYPE (2)

// ===== �\���̒�` =====
typedef struct
{
	D3DXVECTOR3				ScaleData;	// �g��
	D3DXVECTOR3				RotData;	// ��]
	D3DXVECTOR3				PosData;	// ���W
	GAME_OBJ_TYPE			ObjType;	// �ǂ̃I�u�W�F�N�g��
	UINT					uNumber;	// ���ʔԍ�
	bool					bUsed;		// �g�p����Ă��邩
} EXPORT_GAMEOBJ_DATA;		// �o�͗p�f�[�^

// ===== �N���X�̑O���錾 =====
class Collider;

// ===== �N���X��` =====
class Pawn : public GameObjectBase
{
public:
	Pawn();
	Pawn(UINT);
	virtual ~Pawn();

	// ���ʗp�񋓑̒�`
	enum class TagType
	{
		Player = 0,
		Enemy,
	};

	virtual void initialize();
	virtual void updateObject();
	virtual void drawObject();
	virtual void drawObject(D3DXMATRIX,D3DXMATRIX);
	virtual void drawObject(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTex, LPD3DXBUFFER pBuff, DWORD uNumMat);
	virtual void finalize();

	virtual void initializeStatus();	// �e��X�e�[�^�X�������l�ɂ���

	HRESULT makeModel();				// ���f������
	HRESULT makeModelHierarchy();		// �K�w�\���p���f���ǂݍ���
	HRESULT createTexture();			// �e�N�X�`������

	void destroyResorceModel();			// ���f���̉��
	void destroyModelHierarchy();		// �K�w�\���p���f�����
	void destroyModel();				// ���f���̉��
	void destroyTexture();				// �e�N�X�`�����
	void destroyResorceTexture();		// �e�N�X�`�����

	void drawObjectLocal();

	// �Q�b�^�[
	D3DXMATRIX*			getWorldMtx();			// ���[���h�s��擾
	D3DXVECTOR3			getUpVec();				// ������̃x�N�g���擾
	D3DXVECTOR3			getForwardVec();		// �O�����̃x�N�g���擾
	D3DXVECTOR3			getRightVec();			// �������̃x�N�g���擾
	D3DXVECTOR3			getPosition();			// �ʒu�擾
	D3DXVECTOR3			getRotation();			// ��]�擾
	D3DXVECTOR3			getMove();				// �ړ��X�s�[�h�擾
	D3DXVECTOR3			getOffset();			// ���[���h�s�񂩂�I�t�Z�b�g�l���擾
	D3DXVECTOR3			getCenterPos();			// ���S���W�擾
	D3DXVECTOR3			getCollisionBox();		// �����蔻��p�T�C�Y�擾
	D3DXVECTOR3			getColliderPos();		// �����蔻��p�ʒu�擾		
	FLOAT				getCollisionRadius();	// �����蔻��p���a�擾
	TagType				getTag();				// �^�O�擾
	DWORD				getVertexNum();			// ���_�̐��擾
	DWORD				getIndxNum();			// �C���f�b�N�X�o�b�t�@�̐��擾
	DWORD				getTriangleNum();		// �O�p�`�̐��擾
	MESH_VTX*			getVtxAcess();			// ���_���A�N�Z�X�p�|�C���^�擾		
	WORD*				getIndxAcess();			// �C���f�b�N�X�o�b�t�@�A�N�Z�X�p�|�C���^�擾
	bool				getIsGround();			// �n�ʏ��Ԏ擾

	LPD3DXMESH	 getMesh();
	LPD3DXBUFFER getMat();
	DWORD		 getMatNum();


	LPDIRECT3DVERTEXBUFFER9 getVtxBuff();		// ���_���擾
	LPDIRECT3DTEXTURE9		getTexture();		// �e�N�X�`���擾
	BOOL					getUsedFlg();		// �g�p�t���O�擾

	HRESULT AllocBoneMatrix(LPD3DXMESHCONTAINER);
	HRESULT AllocAllBoneMatrix(LPD3DXFRAME);

	void setTime(DOUBLE);
	void updateFrameMatrices(LPD3DXFRAME , LPD3DXMATRIX );
	void drawFrame(LPD3DXFRAME);
	void RenderMeshContainer(LPD3DXMESHCONTAINER, LPD3DXFRAME );

	// �Z�b�^�[
	void setMesh(LPD3DXMESH, LPD3DXBUFFER, DWORD);
	void setMaterialColorEmissive(float, float, float, float);	// �}�e���A���̎��Ȕ����l�Z�b�g
	void setUsedFlg(bool);										// �g�p�t���O�Z�b�g
	void setMoveSpeed(D3DXVECTOR3);								// �ړ��ʃZ�b�g
	void setOffset(D3DXVECTOR3 setOffset);						// �s���̈ʒu�Z�b�g
	void setNumber(UINT);										// ���ʔԍ��Z�b�g
	void setPosition(D3DXVECTOR3);								// �ʒu�Z�b�g
	void setTranslate(D3DXVECTOR3);								// �ʒu�Z�b�g
	void setRotation(D3DXVECTOR3);								// ��]�Z�b�g
	void setHitFlg(bool );										// �Փ˔���Z�b�g
	void setIsGround(bool );									// �n�ʏ㔻��Z�b�g
	void setRayToTriangleInfo(D3DXVECTOR3 , D3DXVECTOR3 );		// ���C�ƎO�p�`�̂����蔻��p�l�Z�b�g
	void setDefaultValue();										// �����l�Z�b�g
	void setDestLanding(D3DXVECTOR3 setLanding);				// ���n�ʒu�Z�b�g
	void setHitIndex(INT);										// ���������I�u�W�F�N�g�̃C���f�b�N�X���Z�b�g

	void setAnimChange(UINT, UINT);				// �A�j���[�V�����؂�ւ�

	void updateExportData();
	void setExportData(EXPORT_GAMEOBJ_DATA);
	EXPORT_GAMEOBJ_DATA* getExportData();
protected:
	std::unique_ptr <Collider> colliderPtr = nullptr;

	MESH_DATA				meshData;
	HIERARCHY_MESH_DATA		hierarchymeshData;
	TEXTURE_DATA			textureData;

	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffPawn;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBuffPawn;	// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	LPDIRECT3DTEXTURE9	pD3DTexture;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			pD3DXMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pD3DXBuffMat;		// �}�e���A�����ւ̃|�C���^
	DWORD				uNumMat;			// �}�e���A�����̐�
	
	D3DXMATRIX			worldMtx;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			translateMtx;		// ���s�ړ��p���[���h�}�g���b�N�X
	D3DXMATRIX			rotMtx;				// ��]�}�g���b�N�X

	CHAR				texFileName[256];	// �ǂݍ��ރt�@�C����

	D3DXVECTOR3			pos;					// ���݂̈ʒu
	D3DXVECTOR3			move;					// �ړ���
	D3DXVECTOR3			accele;					// �����x
	D3DXVECTOR3			rot;					// ���݂̌���
	D3DXVECTOR3			rotDest;				// �ړI�̌���
	D3DXVECTOR3			scale;					// �g�嗦
	D3DXVECTOR3			destLanding;			// �ړI���n�ʒu
	D3DXVECTOR3			maxVtx;					// �ő咸�_�ʒu
	D3DXVECTOR3			minVtx;					// �ŏ����_�ʒu
	D3DXVECTOR3			centerPos;				// ���f���̒��S���W
	D3DXVECTOR3			collitionBox;			// �����蔻��p�T�C�Y
	D3DXVECTOR3			collisionSize;			// �����蔻��p�̑傫��

	D3DXVECTOR3			defPos;					// �����l�̈ʒu
	D3DXVECTOR3			defMove;				// �����l�̈ړ���
	D3DXVECTOR3			defrot;					// �����l�̌���
	D3DXVECTOR3			defScale;				// �����l�̊g�嗦

	D3DXQUATERNION		quatanion;				// �C�ӎ��̉�]�p�N�H�[�^�j�I��
	D3DXQUATERNION		destQua;				// �ړI�N�I�[�^�j�I��


	CHAR				fileName[256];			// ���f���̃t�@�C����
	FLOAT				speed;					// ���x
	FLOAT				radRot;					// ��]�p�x
	FLOAT				oldRadRot;				// �O�̉�]�p�x
	FLOAT				collisionRadus;			// �����蔻��p�T�C�Y
	bool				isHit;					// �������Ă��邩�ǂ���
	bool				isGround;				// �n�ʂ̏ォ
	bool				isUsed;					// �g�p����

	TagType				tagName;				// ���ʗp�^�O
	UINT				idNumber;				// �����̎��ʗp�Y�����ԍ�

	D3DXVECTOR3			cross;					// Ray�ƎO�p�`�̔���p�q�b�g�ʒu
	D3DXVECTOR3			normal;					// Ray�ƎO�p�`�̔���p�@���x�N�g��
	
	D3DXVECTOR3			startPos;				// �J�n�ʒu (��ԗp)
	D3DXVECTOR3			endPos;					// �I���ʒu (��ԗp)
	INT					hitIndex;				// �Փ˔���p

	bool				endAnim;				// �A�j���[�V�������I��������
	DOUBLE				maxTrackTime;			// �A�j���[�V�����̃g���b�N�ő厞��

	UINT				curSelectAnim;			// ���ݑI�𒆂̃A�j���[�V�����ԍ�

	// �K�w�\���p
	LPD3DXFRAME					pFrameRoot;		// ���[�g �t���[�� �I�u�W�F�N�g
	LPD3DXANIMATIONCONTROLLER	animCtrl;		// �A�j���[�V���� �R���g���[�� �I�u�W�F�N�g
	UINT						numAnimset;	// �A�j���[�V���� �Z�b�g��
	LPD3DXANIMATIONSET*			ppAnimSet;		// �A�j���[�V���� �Z�b�g
	MyHierarchy					Hierarchy;		// �K�w�������m��/����N���X
	DWORD						dwPrev;			// ���O�̎���
	
	EXPORT_GAMEOBJ_DATA			exportData;				// �����o���ۂ̕ۑ��p�f�[�^
	GAME_OBJ_TYPE				objType;				// �I�u�W�F�N�g�̎��
	MeshObjType					meshType;				// ���b�V���̎��
	UINT						currentAnim;			// �A�j���[�V����
private:

#define FVF_TVERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


	void calcCollision(LPD3DXFRAME);			// �K�w�\���p�A���E��/���E�{�b�N�X�擾
	void calcCollisionFrame(LPD3DXFRAME);
	void calcCollisionMeshContainer(LPD3DXMESHCONTAINER , LPD3DXFRAME );

	D3DXMATRIX			mtxInverse;				// �ŏ����p�t�s��
	D3DXMATRIX			mtxOldTranslate;		// 1�t���[���O�̕��s�ړ�

	// �Փ˔���p
	DWORD				dwNumVtx;				// ���_��
	DWORD				dwNumTriangles;			// �O�p�`�̐� (���_ * 3)
	DWORD				dwNumIndx;				// �C���f�b�N�X�o�b�t�@�̐�
	DWORD				dwAttrNum;				// �����l
	D3DXATTRIBUTERANGE*	pAttr;					// �����l

	MESH_VTX			*pVtx;					// ���_���ւ̃A�N�Z�X�p�|�C���^
	WORD				*pIndx;					// �C���f�b�N�X�o�b�t�@�A�N�Z�X�p�|�C���^
	FLOAT				colorAlpha;				// �A���t�@�l�ύX�p�ϐ�

	CHAR				*pSzPointer;			// �ǂݍ��݊m�F�p������ 
	CHAR				*szToken;				// �ǂݍ��񂾃g�[�N��

	D3DMATERIAL9*		pMaterial;				// �}�e���A�����
	LPDIRECT3DTEXTURE9*	ppTexture;				// �e�N�X�`�����
};

#endif