//����������������������������������������������������������
// Light.h
// Author : MasayaHayashi
//����������������������������������������������������������

// ===== �C���N���[�h�� ====
#include "../Linklib.h"
#include "d3dx9.h"
#include <memory>

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O���錾 =====
class Camera;

// ===== �N���X��` =====
class Light
{
public:
	HRESULT initialize();
	void finalize();
	void UpdateLight(Camera *pCamera);
	void SetLight(D3DLIGHTTYPE LightType, D3DXCOLOR Diffusecolor, D3DXCOLOR Ambientcolor, D3DXVECTOR3 vecDir, BYTE LightID);
	BYTE* GetLightID();
private:
	static constexpr UINT MaxLight = 3;

	BYTE LightID;

	std::unique_ptr<D3DLIGHT9> lightPtrArray[MaxLight];


	D3DLIGHT9 *pLight[MaxLight];
	D3DXVECTOR3 vecDir;		// ���C�g�̕����̃x�N�g��
};
