//����������������������������������������������������
// SelectManager.h
// Author : Masaya Hayashi
//����������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SELECT_MANAGER_H
#define SELECT_MANAGER_H

// ===== �C���N���[�h�� =====
#include <memory>
#include <map>

// ===== �N���X�̑O���錾 =====

// ===== �N���X��` =====
class SelectManager final
{
public:

	SelectManager();
	~SelectManager();

	static void initialize();
	static void update();
	static void updateFade();
	static void draw();
	static void drawFade();
	static void finalize();

protected:

private:
	static std::unique_ptr<SelectManager> SelectManagerInstancePtr;
};

#endif