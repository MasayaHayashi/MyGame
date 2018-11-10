//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Light.h
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 ====
#include "../Linklib.h"
#include "d3dx9.h"
#include <memory>

// ===== 定数・マクロ定義 =====

// ===== クラスの前方宣言 =====
class Camera;

// ===== クラス定義 =====
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
	D3DXVECTOR3 vecDir;		// ライトの方向のベクトル
};
