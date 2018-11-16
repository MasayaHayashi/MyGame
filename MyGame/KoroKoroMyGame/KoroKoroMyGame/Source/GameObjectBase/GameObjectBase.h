//����������������������������������������������������������������
// GameObjectBase.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef GAME_OBJECT_BASE
#define GAME_OBJECT_BASE

// ===== �C���N���[�h�� =====
#include "GameObjectBase.h"

// ===== �N���X��` =====
class GameObjectBase
{
	public:
		GameObjectBase();
		virtual ~GameObjectBase();

		virtual void initialize();
		virtual void finalize();
		virtual void update();
		virtual void draw();

	protected:

	private:

};

#endif