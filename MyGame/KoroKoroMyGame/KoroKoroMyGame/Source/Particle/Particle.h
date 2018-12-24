//����������������������������������������������������������������
// HeartObj.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef PARTICLE_H
#define PARTICLE_H

// ===== �C���N���[�h�� =====
#include "../Board/Board.h"
#include "../SceneManager/SceneManager.h"
#include <random>

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O���錾 =====

// ===== �\���̒�` =====

// ===== �񋓑̒�` =====

// ===== �N���X��` =====
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
