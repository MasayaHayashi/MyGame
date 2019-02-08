//������������������������������������������
// Shader.h
// Author : Masaya Hayashi
//������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SHADER_H
#define SHADER_H

// ===== �C���N���[�h�� =====
#include "../DirectX3D/DirectX3D.h"
#include <wrl.h>

// ===== �N���X��` =====
class Shader final
{
public:
	Shader();
	~Shader();

	static HRESULT  make(std::string keyName);
	static ID3DXEffect* get();
	static void RenderStart();
	static void RenderEnd();

private:
	static Microsoft::WRL::ComPtr<ID3DXEffect> directXEffect;
};

#endif