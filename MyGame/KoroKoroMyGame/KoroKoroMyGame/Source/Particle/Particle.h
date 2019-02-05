//����������������������������������������������������������������
// Particle.cpp
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
typedef struct 
{
	D3DXVECTOR3 rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v�Z�p�p�x
	FLOAT		workSpeed = 1.0f;							// �v�Z�p�ړ���
	FLOAT		margin = 1.0f;								// �v�Z�p�ӂ蕝
	D3DXVECTOR3 speed = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		// �ړ���
	D3DXVECTOR3 radAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �p�x
	INT			lifeFream = 0;								// ��������
	FLOAT		moveCnt = 0.0f;								// ���`��ԗp�J�E���^
} Particle2dData;

// ===== �񋓑̒�` =====

// ===== �N���X��` =====
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
