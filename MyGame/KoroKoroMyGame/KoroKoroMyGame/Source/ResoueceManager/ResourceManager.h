//����������������������������������������������������������������
// C_ResourceManager.h
// ���\�[�X�Ǘ�(���b�V���A�e�N�X�`���Ǘ�)
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// ===== �C���N���[�h�� =====
#include "d3dx9.h"
#include "../Mesh/Mesh.h"
#include "../MyHierarchy/MyHierarchy.h"
#include <vector>

// ===== �N���X�̑O���錾 =====

// ===== �\���̒�` =====
typedef struct
{
	CHAR				szMeshFileName[256];	// �t�@�C����
	LPD3DXMESH			pD3DXMesh;				// ���b�V�����
	LPD3DXBUFFER		pD3DXBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD				numMat;					// �}�e���A�����̐�
	D3DXVECTOR3			maxVtx;					// �ő咸�_�ʒu
	D3DXVECTOR3			minVtx;					// �ŏ����_�ʒu
	D3DXVECTOR3			centerPos;				// ���f���̒��S���W
	D3DXVECTOR3			collitionBox;			// �����蔻��p�T�C�Y
	FLOAT				collisionRadus;			// �����蔻��p�T�C�Y

	DWORD				dwNumVtx;				// ���_��
	DWORD				dwNumTriangles;			// �O�p�`�̐� (���_ * 3)
	DWORD				dwNumIndx;				// �C���f�b�N�X�o�b�t�@�̐�
	DWORD				dwAttrNum;				// �����l
	D3DXATTRIBUTERANGE*	pAttr;					// �����l

	MESH_VTX			*pVtx;					// ���_���ւ̃A�N�Z�X�p�|�C���^
	WORD				*pIndx;					// �C���f�b�N�X�o�b�t�@�A�N�Z�X�p�|�C���^
	
} MESH_DATA;	// ���b�V�����

typedef struct
{
	CHAR						meshFileName[256];		// �t�@�C����
	LPD3DXMESH					pD3DXMesh;				// ���b�V�����
	LPD3DXBUFFER				pD3DXBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD						numMat;					// �}�e���A�����̐�
	D3DXVECTOR3					maxVtx;					// �ő咸�_�ʒu
	D3DXVECTOR3					minVtx;					// �ŏ����_�ʒu
	D3DXVECTOR3					centerPos;				// ���f���̒��S���W
	D3DXVECTOR3					collitionBox;			// �����蔻��p�T�C�Y
	FLOAT						collisionRadus;			// �����蔻��p�T�C�Y

	LPD3DXFRAME					frameRoot;				// ���[�g �t���[�� �I�u�W�F�N�g
	LPD3DXANIMATIONCONTROLLER	animCtrl;				// �A�j���[�V���� �R���g���[�� �I�u�W�F�N�g
	UINT						numAnimSet;				// �A�j���[�V���� �Z�b�g��
	LPD3DXANIMATIONSET*			ppAnimSet;				// �A�j���[�V���� �Z�b�g
	MyHierarchy					hierarchy;				// �K�w�������m��/����N���X
	DWORD						dwPrev;					// ���O�̎���
} HIERARCHY_MESH_DATA;	// �K�w�\���p���b�V�����

typedef struct
{
	CHAR				texFileName[256];		// �ǂݍ��ރt�@�C����
	LPDIRECT3DTEXTURE9	pD3DTexture;			// �e�N�X�`���ւ̃|�C���^
} TEXTURE_DATA;	// �e�N�X�`�����

typedef struct
{
	CHAR					name[256];				// ���ʖ�
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffBoard;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	UINT					boardType;				// �{�[�h�̎��
	D3DXVECTOR3				posBoard;				// �ʒu
	D3DXVECTOR3				sizeBoard;				// �傫��
	D3DXVECTOR3				scaleBoard;				// �g�嗦
	D3DXVECTOR3				rotBoard;				// ��]�p�x
	FLOAT					radAngleBoard;			// 2D�p��]�p�x
	bool					fade;					// �t�F�[�h�p
} VERTEX_BOARD_DATA;

// ===== �N���X��` =====
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// ����
	static HRESULT MakeModel(MESH_DATA &MeshData, CHAR *pszFilename,UINT &uMeshType);
	static HRESULT CreateTexture(TEXTURE_DATA &TextureData,CHAR *pszFilename);
	static HRESULT MakeModelHierarchy(HIERARCHY_MESH_DATA &HierarchyMedhData, CHAR *pszFilename, UINT &MeshType);
	static HRESULT MakeVertexBoard(VERTEX_BOARD_DATA &VtxBordData, CHAR *pszFilename);

	// ���
	static bool DestroyMesh(CHAR *pszChakNeme);
	static bool DestroyAllMesh();	// �S���b�V�����폜
	static bool DestroyHierarchyMesh(CHAR *pszChakNeme);
	static bool DestroyAllHierarchyMesh();
	static bool DestroyAllTexture();
	static bool DestroyVtx();
	static bool DestroyFadeVtx();
	
	static void CreateFadeTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename);

	// �C���X�^���X
	static C_RESOURCE_MANAGER *pInstance;

private:
#define FVF_TVERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

	C_RESOURCE_MANAGER();

	static std::vector<MESH_DATA*>				mesh;			
	static std::vector<HIERARCHY_MESH_DATA*>	hierarchyMesh;
	static std::vector<TEXTURE_DATA*>			texture;		
	static std::vector<TEXTURE_DATA*>			fadeTexture;	
	static std::vector<VERTEX_BOARD_DATA*>		vtxBoard;		
	static std::vector<VERTEX_BOARD_DATA*>		vtxFadeBoard;

	static bool CheckExisting(CHAR *pszChakNeme, MESH_DATA &meshData);
	static bool CheckExisting(CHAR *pszChakNeme, TEXTURE_DATA &textureData);
	static bool CheckExisting(CHAR *pszChakNeme, VERTEX_BOARD_DATA &textureData);

static void SetTime(DOUBLE dTime, CHAR *pszFilename);

static 	HRESULT AllocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename);
static 	HRESULT AllocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename);
static	void CalcCollision(LPD3DXFRAME pFrame, CHAR *pszFilename);
static	void CalcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename);
static	void CalcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename);
static	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

static 	bool makeVtx(VERTEX_BOARD_DATA&);		// �ʏ�
static 	bool makeVtxFade(VERTEX_BOARD_DATA&);	// �t�F�[�h�p

static	void createNormalTexture(TEXTURE_DATA&,CHAR*);

};


#endif