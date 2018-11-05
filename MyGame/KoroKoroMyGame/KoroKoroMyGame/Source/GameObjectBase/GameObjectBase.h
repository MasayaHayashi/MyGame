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

		virtual void InitObject();
		virtual void UninitObject();
		virtual void UpdateObject();
		virtual void DrawObject();

	protected:
		enum class TAG_TYPE
		{
			TAG_BASEOBJ = 0,
			TAG_PLAYER,		// ���@
			TAG_ENEMY,		// �G�@
			TAG_LAND,		// �n��
			MAX_TAG_TYPE,
		};

		TAG_TYPE	Tag;				// ���ʗp�^�O
	private:

};

#endif