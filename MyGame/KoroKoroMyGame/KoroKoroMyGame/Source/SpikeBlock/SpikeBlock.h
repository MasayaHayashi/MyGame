//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SpikeBlock.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SPIKE_BLOCK_H
#define SPIKE_BLOCK_H

// ===== インクルード部 =====
#include "../SceneInterface/sceneBase.h"
#include "../MainObject/MainObject.h"

// ===== クラス定義 =====
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

	D3DXVECTOR3 destPos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	const std::string ModelFilePass		= "Data/Model/Block/block_spike1.x.x";
	const std::string TextureFilePass	= "Data/Texture/cube_tex.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

};

#endif 
