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
#include <unordered_map>
#include <memory>
#include <wrl.h>

// ===== �O���錾 =====
enum class boardType;
enum class MeshObjType;

// ===== �\���̒�` =====
typedef struct
{
	CHAR				meshFileName[256];	// �t�@�C����
	Microsoft::WRL::ComPtr<ID3DXMesh>	meshPtr;
	Microsoft::WRL::ComPtr<ID3DXBuffer> materialBufferPtr;
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

	D3DXATTRIBUTERANGE  *attrPtr;
	MESH_VTX			*vertexPtr;
	WORD				*indexPtr;
} MeshData;

typedef struct
{
	CHAR						meshFileName[256];			// �t�@�C����
	LPD3DXMESH					pD3DXMesh;				// ���b�V�����
	LPD3DXBUFFER				pD3DXBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD						uNumMat;				// �}�e���A�����̐�
	D3DXVECTOR3					MaxVtx;					// �ő咸�_�ʒu
	D3DXVECTOR3					MinVtx;					// �ŏ����_�ʒu
	D3DXVECTOR3					CenterPos;				// ���f���̒��S���W
	D3DXVECTOR3					CollitionBox;			// �����蔻��p�T�C�Y
	FLOAT						fCollisionRadus;		// �����蔻��p�T�C�Y

	LPD3DXFRAME					pFrameRoot;				// ���[�g �t���[�� �I�u�W�F�N�g
	LPD3DXANIMATIONCONTROLLER	pAnimCtrl;				// �A�j���[�V���� �R���g���[�� �I�u�W�F�N�g
	UINT						uNumAnimSet;			// �A�j���[�V���� �Z�b�g��
	LPD3DXANIMATIONSET*			ppAnimSet;				// �A�j���[�V���� �Z�b�g
	MyHierarchy					Hierarchy;				// �K�w�������m��/����N���X
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
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	boardType				boardType;				// �{�[�h�̎��
	D3DXVECTOR3				pos;					// �ʒu
	D3DXVECTOR3				size;					// �傫��
	D3DXVECTOR3				scale;				// �g�嗦
	D3DXVECTOR3				rotDeg;				// ��]�p�x
	FLOAT					radAngle;			// 2D�p��]�p�x
	bool					fade;					// �t�F�[�h�p
} VERTEX_BOARD_DATA;

// ===== �N���X��` =====
class ResourceManager final
{
public:
	ResourceManager();
	~ResourceManager();

	static constexpr CHAR* ModelPenchanPass		= "Data/Model/Character/PenChan/PenguinC.x";
	static constexpr CHAR* ModelPenNoHahaPass	= "Data/Model/Character/PenNoHaha/PenguinA.x";
	static constexpr CHAR* ModelChick			= "Data/Model/Character/Chick/Chick.x";

	static constexpr UINT MaxModelType = 3;

	// ����
	static HRESULT makeModel(MeshData &MeshData, CHAR *pszFilename, MeshObjType &uMeshType);
	static HRESULT createTexture(TEXTURE_DATA &TextureData,CHAR *pszFilename);
	static HRESULT makeModelHierarchy(HIERARCHY_MESH_DATA &HierarchyMedhData, CHAR *pszFilename, std::string keyName, MeshObjType &MeshType,UINT index);
	static HRESULT makevertexBoard(VERTEX_BOARD_DATA &VtxBordData, CHAR *pszFilename);
	static HRESULT makeModelHierarchyResouce(HIERARCHY_MESH_DATA &setHierarchyMeshData, CHAR *setFilename, std::string keyName, MeshObjType &meshType,UINT index);
	static HRESULT makeHierarchyResouce(HIERARCHY_MESH_DATA& meshData, std::string setName, CHAR* keyName, UINT index);


	// ���
	static bool destroyMesh(CHAR *pszChakNeme);
	static bool destroyAllMesh();	// �S���b�V�����폜
	static bool destroyHierarchymesh(CHAR *pszChakNeme,std::string keyName);
	static bool destroyAllHierarchymesh();
	static bool destroyAllTexture();
	static bool destroyVtx();
	static bool destroyFadeVtx();
	static bool destroyAllResouce(size_t index);

	static void createFadeTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename);

	static bool createInstance();
	static void changeHierarchy(HIERARCHY_MESH_DATA &changeData,CHAR* modelName,UINT index);

	static void setHierarchy(HIERARCHY_MESH_DATA *meshData, std::string keyName,UINT index);

	static std::unique_ptr<ResourceManager> instancePtr;

private:
#define FVF_TVERTEX	    (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

	static std::vector<MeshData*>											meshes;
	static std::vector<HIERARCHY_MESH_DATA*>								hierarchyMesh;
	static std::unordered_map<std::string ,std::vector<HIERARCHY_MESH_DATA*>>	resoueceMesh;

	static std::vector<TEXTURE_DATA*>			texture;
	static std::vector<TEXTURE_DATA*>			fadeTexture;
	static std::vector<VERTEX_BOARD_DATA*>		vtxBoard;
	static std::vector<VERTEX_BOARD_DATA*>		vtxFadeBoard;

	static bool checkExisting(CHAR *pszChakNeme, MeshData &meshData);
	static bool checkExisting(CHAR *pszChakNeme, TEXTURE_DATA &textureData);
	static bool checkExisting(CHAR *pszChakNeme, VERTEX_BOARD_DATA &textureData);

	static void setTime(DOUBLE dTime, CHAR *pszFilename, std::string keyName, UINT index);
	
	static 	HRESULT allocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename,std::string keyName,UINT index);
	static 	HRESULT allocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename,std::string keyName,UINT index);
	static	void calcCollision(LPD3DXFRAME pFrame, CHAR *pszFilename,std::string,UINT index);
	static	void calcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename,std::string keyName,UINT index);
	static	void calcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename,std::string keyName,UINT index);
	static	void updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
	static  void destroyResouceMesh(std::string keyName,UINT index);
	
	
	static 	bool makeVtx(VERTEX_BOARD_DATA&);		// �ʏ�
	static 	bool makeVtxFade(VERTEX_BOARD_DATA&);	// �t�F�[�h�p
	
	static	void createNormalTexture(TEXTURE_DATA&,CHAR*);

};


#endif