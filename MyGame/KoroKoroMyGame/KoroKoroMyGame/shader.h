#ifndef SHADER_H
#define SHADER_H

#include "Source\DirectX3D\DirectX3D.h"
#include <wrl.h>

class Shader
{
public:
	Shader();
	~Shader();

	static HRESULT create();
	static ID3DXEffect* get();
	static void RenderStart();
	static void RenderEnd();

private:
	static Microsoft::WRL::ComPtr<ID3DXEffect> directXEffect;
};

#endif