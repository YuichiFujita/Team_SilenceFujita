//======================================================================================================================
//
//	�p�[�e�B�N���w�b�_�[ [particle.h]
//	Author�F���c�E��
//
//======================================================================================================================
#ifndef _PARTICLE_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _PARTICLE_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define SPAWN_PARTICLE_EXPLOSION	(8)		// �p�[�e�B�N�� (����) �̃G�t�F�N�g��
#define SPAWN_PARTICLE_DAMAGE		(10)	// �p�[�e�B�N�� (�_���[�W) �̃G�t�F�N�g��
#define SPAWN_PARTICLE_DUST			(4)		// �p�[�e�B�N���i���j�̃G�t�F�N�g��
#define SPAWN_PARTICLE_RAINSPRAY	(8)		// �p�[�e�B�N���i�����Ԃ��j�̃G�t�F�N�g��
#define SPAWN_PARTICLE_SMOKING		(1)		// �p�[�e�B�N���i�^�o�R�̉��j�̃G�t�F�N�g��
#define SPAWN_PARTICLE_ITEM_LOST	(10)	// �p�[�e�B�N���i�A�C�e�������j�̃G�t�F�N�g��
#define SPAWN_PARTICLE_PLAY_SMOKE	(20)	// �p�[�e�B�N���i�v���C���[�̍����j�̃G�t�F�N�g��
#define SPAWN_PARTICLE_BREAKARTICLE	(15)	// �p�[�e�B�N���i�����̔j�󎞂̉��j�̃G�t�F�N�g��
#define SPAWN_PARTICLE_BREAKOBJECT	(15)	// �p�[�e�B�N���i�I�u�W�F�N�g�̔j�󎞂̉��j�̃G�t�F�N�g��
#define SPAWN_PARTICLE_PLAYERDEATH	(20)	// �p�[�e�B�N���i�v���C���[�̎��S�����j�̃G�t�F�N�g��
#define SPAWN_PARTICLE_SHOT_BARRIER	(6)		// �p�[�e�B�N�� (�o���A����) �̃G�t�F�N�g��

//**********************************************************************************************************************
//	�񋓌^��` (PARTICLETYPE)
//**********************************************************************************************************************
typedef enum
{
	PARTICLETYPE_NONE = 0,		// �p�[�e�B�N�� (�������Ȃ�)
	PARTICLETYPE_EXPLOSION,		// �p�[�e�B�N�� (����)
	PARTICLETYPE_DAMAGE,		// �p�[�e�B�N�� (�_���[�W)
	PARTICLETYPE_BOOST,			// �p�[�e�B�N�� (�u�[�X�g)
	PARTICLETYPE_SPARK,			// �p�[�e�B�N���i�Ήԁj
	PARTICLETYPE_DUST,			// �p�[�e�B�N�� (��)
	PARTICLETYPE_RAINSPRAY,		// �p�[�e�B�N���i���򖗁j
	PARTICLETYPE_ITEM_LOST,		// �p�[�e�B�N���i�A�C�e�������j
	PARTICLETYPE_PLAY_SMOKE,	// �p�[�e�B�N���i�v���C���[�̍����j
	PARTICLETYPE_BREAK_ARTICLE,	// �p�[�e�B�N���i�����̔j�󎞁j
	PARTICLETYPE_BREAK_OBJECT,	// �p�[�e�B�N���i�I�u�W�F�N�g�̔j�󎞁j
	PARTICLETYPE_PLAY_DEATH,	// �p�[�e�B�N���i�v���C���[�̎��S�����j
	PARTICLETYPE_OBJECT_SMASH,	// �p�[�e�B�N���i�I�u�W�F�N�g�𐁂���΂������j
	PARTICLETYPE_SHOT_BARRIER,	// �p�[�e�B�N���i�o���A���ˁj
	PARTICLETYPE_MAX,			// ���̗񋓌^�̑���
} PARTICLETYPE;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void InitParticle(void);		// �p�[�e�B�N���̏���������
void UninitParticle(void);		// �p�[�e�B�N���̏I������
void UpdateParticle(void);		// �p�[�e�B�N���̍X�V����
void DrawParticle(void);		// �p�[�e�B�N���̕`�揈��

void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nSpawn, int nLife);		// �p�[�e�B�N���̐ݒ菈��

void AllFalseParticle(void);	// �p�[�e�B�N���̍폜

//**********************************************************************************************************************
//	�v���g�^�C�v�錾 (�f�o�b�O�p)
//**********************************************************************************************************************
int GetNumParticle(void);		// �p�[�e�B�N���̑����擾����

#endif