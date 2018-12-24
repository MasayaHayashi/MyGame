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
#include <string>

enum class MeshObjType
{
	NormalModel,
	HierarchyModel,	
};

// ===== �N���X�̑O���錾 =====
class Collider;

// ===== �\���̒�` =====
typedef struct
{
	D3DXVECTOR3			pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3			velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3			accele		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3			rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3			rotDegDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3			scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3			cross		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3			normal		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
} Transform;

typedef struct
{
	D3DXVECTOR3 hitRay	 = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 hitCross = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool isHitAABB		 = false;
	bool isHitRay		 = false;
} HitData;

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
	virtual void initialize(std::string fileName);
	virtual void update();
	virtual void updateAnimation();
	virtual void draw();
	virtual void draw(D3DXMATRIX,D3DXMATRIX);
	virtual void draw(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTex, LPD3DXBUFFER pBuff, DWORD numMat);
	virtual void finalize();

	virtual void initializeStatus();	// �e��X�e�[�^�X�������l�ɂ���

	void drawObjectLocal();

	// �Q�b�^�[
	D3DXMATRIX*			getWorldMtx();			// ���[���h�s��擾
	D3DXVECTOR3			getUpVec();				// ������̃x�N�g���擾
	D3DXVECTOR3			getForwardVec();		// �O�����̃x�N�g���擾
	D3DXVECTOR3			getRightVec();			// �������̃x�N�g���擾
	D3DXVECTOR3			getPosition();			// �ʒu�擾
	D3DXVECTOR3			getRotation();			// ��]�擾
	D3DXVECTOR3			getvelocity();				// �ړ��X�s�[�h�擾
	D3DXVECTOR3			getOffset();			// ���[���h�s�񂩂�I�t�Z�b�g�l���擾
	D3DXVECTOR3			getCenterPos();			// ���S���W�擾
	D3DXVECTOR3			getCollisionBox();		// �����蔻��p�T�C�Y�擾
	D3DXVECTOR3			getColliderPos();		// �����蔻��p�ʒu�擾		
	FLOAT				getCollisionRadius();	// �����蔻��p���a�擾
	std::string			getTag();				// �^�O�擾
	DWORD				getVertexNum();			// ���_�̐��擾
	DWORD				getIndxNum();			// �C���f�b�N�X�o�b�t�@�̐��擾
	DWORD				getTriangleNum();		// �O�p�`�̐��擾
	MESH_VTX*			getVtxAcess();			// ���_���A�N�Z�X�p�|�C���^�擾		
	WORD*				getIndxAcess();			// �C���f�b�N�X�o�b�t�@�A�N�Z�X�p�|�C���^�擾
	bool				getIsGround();			// �n�ʏ��Ԏ擾
	bool				isUsedShader();			// �V�F�[�_�[�g�p

	LPD3DXMESH	 getMesh();
	LPD3DXBUFFER getMat();
	DWORD		 getMatNum();

	LPDIRECT3DVERTEXBUFFER9 getVtxBuff();		// ���_���擾
	LPDIRECT3DTEXTURE9		getTexture();		// �e�N�X�`���擾
	BOOL					getUsedFlg();		// �g�p�t���O�擾

	HRESULT AllocBoneMatrix(LPD3DXMESHCONTAINER);
	HRESULT AllocAllBoneMatrix(LPD3DXFRAME);

	void setTime(DOUBLE);
	void changePosition();
	void drawFrame(LPD3DXFRAME);
	void RenderMeshContainer(LPD3DXMESHCONTAINER, LPD3DXFRAME );

	// �Z�b�^�[
	void setMesh(LPD3DXMESH, LPD3DXBUFFER, DWORD);
	void setMaterialcolorEmissive(float, float, float, float);	// �}�e���A���̎��Ȕ����l�Z�b�g
	void setUsedFlg(bool);										// �g�p�t���O�Z�b�g
	void setvelocitySpeed(D3DXVECTOR3);							// �ړ��ʃZ�b�g
	void setOffset(D3DXVECTOR3 setOffset);						// �s���̈ʒu�Z�b�g
	void setNumber(UINT);										// ���ʔԍ��Z�b�g
	void setPosition(D3DXVECTOR3);								// �ʒu�Z�b�g
	void setTranslate(D3DXVECTOR3);								// �ʒu�Z�b�g
	void setRotation(D3DXVECTOR3);								// ��]�Z�b�g
	void setIsGround(bool );									// �n�ʏ㔻��Z�b�g
	void setDefaultValue();										// �����l�Z�b�g
	void setDestLanding(D3DXVECTOR3 setLanding);				// ���n�ʒu�Z�b�g
	void setHitIndex(INT);										// ���������I�u�W�F�N�g�̃C���f�b�N�X���Z�b�g

	void setAnimChange(UINT, UINT);

	void setPawn(Pawn);

	void setWorldMtxPos(const D3DXVECTOR3 setPos);

	bool isHit(std::string keyName);

#if _DEBUG
	void debugMove();

	static constexpr FLOAT DebugMoveSpeed = 0.05f;
#endif
protected:
	std::unique_ptr <Collider> colliderPtr = nullptr;

	MeshData				meshDataObj;
	HIERARCHY_MESH_DATA		hierarchyMeshData;
	TEXTURE_DATA			textureData;

	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffPawn;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  pD3DIdxBuffPawn;	// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	LPDIRECT3DTEXTURE9	pD3DTexture;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			meshPtr;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		materialBufferPtr;		// �}�e���A�����ւ̃|�C���^
	DWORD				numMat;				// �}�e���A�����̐�
	
	D3DXMATRIX			worldMtx;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			translateMtx;		// ���s�ړ��p���[���h�}�g���b�N�X
	D3DXMATRIX			rotMtx;				// ��]�}�g���b�N�X

	CHAR				texFileName[256];	// �ǂݍ��ރt�@�C����

	Transform			myTransform;
	HitData				myHitData;

	D3DXVECTOR3			destLanding;			// �ړI���n�ʒu
	D3DXVECTOR3			maxVtx;					// �ő咸�_�ʒu
	D3DXVECTOR3			minVtx;					// �ŏ����_�ʒu
	D3DXVECTOR3			centerPos;				// ���f���̒��S���W
	D3DXVECTOR3			collitionBox;			// �����蔻��p�T�C�Y
	D3DXVECTOR3			collisionSize;			// �����蔻��p�̑傫��

	D3DXVECTOR3			defPos;					// �����l�̈ʒu
	D3DXVECTOR3			defvelocity;				// �����l�̈ړ���
	D3DXVECTOR3			defrot;					// �����l�̌���
	D3DXVECTOR3			defScale;				// �����l�̊g�嗦

	D3DXQUATERNION		quatanion;				// �C�ӎ��̉�]�p�N�H�[�^�j�I��
	D3DXQUATERNION		destQua;				// �ړI�N�I�[�^�j�I��

	CHAR				fileName[256];			// ���f���̃t�@�C����
	FLOAT				speed;					// ���x
	FLOAT				radRot;					// ��]�p�x
	FLOAT				oldRadRot;				// �O�̉�]�p�x
	FLOAT				collisionRadus;			// �����蔻��p�T�C�Y
	bool				isShader;				// �V�F�[�_�[�g�p
	bool				isGround;				// �n�ʂ̏ォ
	bool				isUsed;					// �g�p����

	std::string			tagName;				// ���ʗp�^�O
	UINT				idNumber;				// �����̎��ʗp�Y�����ԍ�

	D3DXVECTOR3			StartPos;				// �J�n�ʒu (��ԗp)
	D3DXVECTOR3			endPos;					// �I���ʒu (��ԗp)
	INT					hitIndex;				// �Փ˔���p

	bool				endAnim;				// �A�j���[�V�������I��������
	DOUBLE				maxTrackTime;			// �A�j���[�V�����̃g���b�N�ő厞��

	UINT				curSelectAnim;			// ���ݑI�𒆂̃A�j���[�V�����ԍ�

	// �K�w�\���p
	LPD3DXFRAME					frameRoot;		// ���[�g �t���[�� �I�u�W�F�N�g
	LPD3DXANIMATIONCONTROLLER	animCtrlPtr;	// �A�j���[�V���� �R���g���[�� �I�u�W�F�N�g
	UINT						numAnimset;		// �A�j���[�V���� �Z�b�g��
	LPD3DXANIMATIONSET*			ppAnimSet;		// �A�j���[�V���� �Z�b�g
	MyHierarchy					Hierarchy;		// �K�w�������m��/����N���X
	DWORD						dwPrev;			// ���O�̎���
	

	GameObjType					objType;
	MeshObjType					meshType;				// ���b�V���̎��
	UINT						currentAnim;			// �A�j���[�V����
private:


#define FVF_TVERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

	void updateFrameMatrices(LPD3DXFRAME, LPD3DXMATRIX);

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

	FLOAT				colorAlpha;				// �A���t�@�l�ύX�p�ϐ�
};

#endif