//����������������������������������������������������������������
// SpikeBlock.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SPIKE_BLOCK_H
#define SPIKE_BLOCK_H

// ===== �C���N���[�h�� =====
#include "../SceneInterface/sceneBase.h"
#include "../MainObject/MainObject.h"

// ===== �N���X��` =====
class SpikeBlock final : public MainObject
{
public:
	SpikeBlock();
	SpikeBlock(std::string modelPass, std::string texturePass, size_t setNumber, GameObjectType setGameObj, bool setFieldModel);
	~SpikeBlock();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX, D3DXMATRIX);

	void initializeStatus();
private:

	const std::string ModelFilePass		= "Data/Model/Block/block_spike1.x.x";
	const std::string TextureFilePass	= "Data/Texture/cube_tex.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

};

#endif 
