//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// HeartObj.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef STAR_H
#define STAR_H

// ===== インクルード部 =====
#include "../Particle/Particle.h"
#include "../SceneManager/SceneManager.h"

// ===== 定数・マクロ定義 =====

// ===== クラスの前方宣言 =====

// ===== 構造体定義 =====

// ===== 列挙体定義 =====

// ===== クラス定義 =====
class StarParticle final : public Particle
{
public:
	StarParticle();
	~StarParticle();

	void initialize();
	void finalize();
	void update();
	void draw();
	void initializeStatus();

	static constexpr size_t MaxParticle = 100;
private:

	const std::string TextureFilePass = "Data/Texture/GoldStar.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;

	FLOAT rotCnt = 0;

	SceneManager::SceneState currentScene;
};

#endif 
