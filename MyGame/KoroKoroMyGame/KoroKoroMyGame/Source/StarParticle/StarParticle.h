//����������������������������������������������������������������
// HeartObj.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef STAR_H
#define STAR_H

// ===== �C���N���[�h�� =====
#include "../Particle/Particle.h"
#include "../SceneManager/SceneManager.h"

// ===== �萔�E�}�N����` =====

// ===== �N���X�̑O���錾 =====

// ===== �\���̒�` =====

// ===== �񋓑̒�` =====

// ===== �N���X��` =====
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
