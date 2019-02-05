//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Particle.cpp
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
typedef struct 
{
	D3DXVECTOR3 rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 計算用角度
	FLOAT		workSpeed = 1.0f;							// 計算用移動量
	FLOAT		margin = 1.0f;								// 計算用ふり幅
	D3DXVECTOR3 speed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		// 移動量
	D3DXVECTOR3 radAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 角度
	INT			lifeFream = 0;								// 生存時間
	FLOAT		moveCnt = 0.0f;								// 線形補間用カウンタ
} Particle2dData;

// ===== 列挙体定義 =====

// ===== クラス定義 =====
class Particle : public Board
{
public:
	Particle();
	virtual ~Particle();

	void initialize();
	void finalize();
	void update();
	void draw();

protected:
	static constexpr FLOAT Gravity = 0.04f;

	void make(D3DXVECTOR3 pos,D3DXVECTOR3 move);
	void destroy();
	void move();
	bool checkLength(FLOAT yPos);
	D3DXVECTOR3 moveVec = D3DXVECTOR3(0.0f,0.0f,0.0f);
	bool awake = false;



private:

	const std::string TextureFilePass = "Data/Texture/GoldParticle.png";
	static constexpr FLOAT TitleRotSpeed = 0.06f;


	Particle2dData my2DParticleData;
	FLOAT rotCnt = 0;

	SceneManager::SceneState currentScene;
};

#endif 
