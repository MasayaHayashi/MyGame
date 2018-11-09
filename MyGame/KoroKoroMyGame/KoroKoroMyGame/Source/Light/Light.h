//����������������������������������������������������������
// Light.h
// Author : MasayaHayashi
//����������������������������������������������������������

// ===== �C���N���[�h�� ====
#include "../Linklib.h"
#include "d3dx9.h"
#include <memory>

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O����` =====
class camera;

// ===== �N���X��` =====
class Light
{
public:
	HRESULT InitLight();
	void UninitLight();
	void UpdateLight(camera *pcamera);
	void SetLight(D3DLIGHTTYPE LightType, D3DXCOLOR DiffuseColor, D3DXCOLOR AmbientColor, D3DXVECTOR3 vecDir, BYTE LightID);
	BYTE* GetLightID();
private:
	static constexpr UINT MaxLight = 3;

	BYTE LightID;

	std::unique_ptr<D3DLIGHT9> lightPtrArray[MaxLight];


	D3DLIGHT9 *pLight[MaxLight];
	D3DXVECTOR3 vecDir;		// ���C�g�̕����̃x�N�g��
};
