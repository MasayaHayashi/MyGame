//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// HeartObj.cpp
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef PARTICLE_H
#define PARTICLE_H

// ===== インクルード部 =====
#include "../Board/Board.h"
#include "../SceneManager/SceneManager.h"
#include <random>

// ===== 定数・マクロ定義 =====

// ===== クラスの前方宣言 =====

// ===== 構造体定義 =====

// ===== 列挙体定義 =====

// ===== クラス定義 =====
class Particle : public Board
{
public:
	Particle();
	~Particle();

	void initialize();
	void finalize();
	void update();
	void draw();

protected:
	static constexpr FLOAT Gravity = 0.04f;

	void make(D3DXVECTOR3 pos,D3DXVECTOR3 move);
	void destroy();
	void move();
	D3DXVECTOR3 moveVec = D3DXVECTOR3(0.0f,0.0f,0.0f);

private:

	const std::string TextureFilePass = "Data/Texture/GoldParticle.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;



	FLOAT rotCnt = 0;

	SceneManager::SceneState currentScene;
};

#endif 
