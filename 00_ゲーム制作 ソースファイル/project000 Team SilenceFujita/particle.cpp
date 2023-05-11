//======================================================================================================================
//
//	�p�[�e�B�N������ [particle.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "particle.h"
#include "effect.h"
#include "player.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_PARTICLE	(128)	// �p�[�e�B�N���̍ő吔

//**********************************************************************************************************************
//	�\���̒�` (Particle)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;		// �ʒu
	D3DXCOLOR    col;		// �F
	PARTICLETYPE type;		// ���
	int          nSpawn;	// �G�t�F�N�g��
	int          nLife;		// ����
	bool         bUse;		// �g�p��
}Particle;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void ParticleDamage(Particle *pParticle);		// �_���[�W�G�t�F�N�g
void ParticleExplosion(Particle *pParticle);	// �����G�t�F�N�g
void ParticleBoost(Particle *pParticle);		// �u�[�X�g�G�t�F�N�g
void ParticleSpark(Particle *pParticle);		// �ΉԃG�t�F�N�g
void ParticleDust(Particle *pParticle);			// ���G�t�F�N�g
void ParticleRainSpray(Particle *pParticle);	// �J�̐����Ԃ��G�t�F�N�g
void ParticleItemLost(Particle *pParticle);		// �A�C�e�������G�t�F�N�g
void ParticlePlaySmoke(Particle *pParticle);	// �v���C���[�̍����G�t�F�N�g
void ParticleBreakArticle(Particle *pParticle);	// ������j�󂵂����̃G�t�F�N�g
void ParticleBreakObject(Particle *pParticle);	// �I�u�W�F�N�g��j�󂵂����̃G�t�F�N�g
void ParticlePlayerDeath(Particle *pParticle);	// �v���C���[�̎��S�����̃G�t�F�N�g
void ParticleObjectSmash(Particle *pParticle);	// �I�u�W�F�N�g�𐁂���΂������̃G�t�F�N�g
void ParticleExhaustGas(Particle *pParticle);	// �r�C�K�X�̃G�t�F�N�g
void ParticleShotBarrier(Particle *pParticle);	// �o���A���˃G�t�F�N�g

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
Particle g_aParticle[MAX_PARTICLE];	// �p�[�e�B�N���̏��

//======================================================================================================================
//	�p�[�e�B�N���̏���������
//======================================================================================================================
void InitParticle(void)
{
	// �p�[�e�B�N���̏��̏�����
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // �p�[�e�B�N���̍ő�\�������J��Ԃ�

		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F

		g_aParticle[nCntParticle].type   = PARTICLETYPE_NONE;	// ���
		g_aParticle[nCntParticle].nSpawn = 0;					// �G�t�F�N�g��
		g_aParticle[nCntParticle].nLife  = 0;					// ����
		g_aParticle[nCntParticle].bUse   = false;				// �g�p��
	}
}

//======================================================================================================================
//	�p�[�e�B�N���̏I������
//======================================================================================================================
void UninitParticle(void)
{
	
}

//======================================================================================================================
//	�p�[�e�B�N���̍X�V����
//======================================================================================================================
void UpdateParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // �p�[�e�B�N���̍ő�\�������J��Ԃ�

		if (g_aParticle[nCntParticle].bUse == true)
		{ // �p�[�e�B�N�����g�p����Ă���ꍇ

			// ���������炷
			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{ // �������s�����ꍇ

				// �g�p����Ă��Ȃ���Ԃɂ���
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//======================================================================================================================
//	�p�[�e�B�N���̕`�揈��
//======================================================================================================================
void DrawParticle(void)
{
	
}

//======================================================================================================================
//	�p�[�e�B�N���̐ݒ菈��
//======================================================================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLETYPE type, int nSpawn, int nLife)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // �p�[�e�B�N���̍ő�\�������J��Ԃ�

		if (g_aParticle[nCntParticle].bUse == false)
		{ // �p�[�e�B�N�����g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aParticle[nCntParticle].pos    = pos;		// �ʒu
			g_aParticle[nCntParticle].col    = col;		// �F
			g_aParticle[nCntParticle].type   = type;	// ���
			g_aParticle[nCntParticle].nSpawn = nSpawn;	// �G�t�F�N�g��
			g_aParticle[nCntParticle].nLife  = nLife;	// ����

			// �g�p���Ă����Ԃɂ���
			g_aParticle[nCntParticle].bUse = true;

			switch (g_aParticle[nCntParticle].type)
			{ // �p�[�e�B�N���̎�ނ��Ƃ̐ݒ�
			case PARTICLETYPE_EXPLOSION:

				// �����G�t�F�N�g
				ParticleExplosion(&g_aParticle[nCntParticle]);

				// �����𔲂���
				break;

			case PARTICLETYPE_DAMAGE:

				// �_���[�W�G�t�F�N�g
				ParticleDamage(&g_aParticle[nCntParticle]);

				// �����𔲂���
				break;

			case PARTICLETYPE_BOOST:

				// �u�[�X�g�G�t�F�N�g
				ParticleBoost(&g_aParticle[nCntParticle]);

				// �����𔲂���
				break;

			case PARTICLETYPE_SPARK:

				// �ΉԃG�t�F�N�g
				ParticleSpark(&g_aParticle[nCntParticle]);

				// �����𔲂���
				break;

			case PARTICLETYPE_DUST:

				// ���G�t�F�N�g
				ParticleDust(&g_aParticle[nCntParticle]);

				// �����𔲂���
				break;

			case PARTICLETYPE_RAINSPRAY:

				// �����Ԃ��G�t�F�N�g
				ParticleRainSpray(&g_aParticle[nCntParticle]);

				// �����𔲂���
				break;

			case PARTICLETYPE_ITEM_LOST:

				// �A�C�e�������G�t�F�N�g
				ParticleItemLost(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_PLAY_SMOKE:

				// �v���C���[�̍����G�t�F�N�g
				ParticlePlaySmoke(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_BREAK_ARTICLE:

				// ������j�󂵂����̃G�t�F�N�g
				ParticleBreakArticle(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_BREAK_OBJECT:

				// �I�u�W�F�N�g��j�󂵂����̃G�t�F�N�g
				ParticleBreakObject(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_PLAY_DEATH:

				// �v���C���[�̎��S�����̃G�t�F�N�g
				ParticlePlayerDeath(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_OBJECT_SMASH:

				// �I�u�W�F�N�g�𐁂���΂������̃G�t�F�N�g
				ParticleObjectSmash(&g_aParticle[nCntParticle]);

				break;

			case PARTICLETYPE_EXHAUST_GAS:

				// �r�C�K�X�̃G�t�F�N�g
				ParticleExhaustGas(&g_aParticle[nCntParticle]);

				break;
				
			case PARTICLETYPE_SHOT_BARRIER:

				// �o���A���˃G�t�F�N�g
				ParticleShotBarrier(&g_aParticle[nCntParticle]);

				break;
			}

			// �����𔲂���
			break;
		}
	}
}

//======================================================================================================================
//	�p�[�e�B�N���̍폜
//======================================================================================================================
void AllFalseParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // �p�[�e�B�N���̍ő�\�������J��Ԃ�

		// �g�p���Ă��Ȃ���Ԃɂ���
		g_aParticle[nCntParticle].bUse = false;
	}
}

//======================================================================================================================
//	�_���[�W�G�t�F�N�g
//======================================================================================================================
void ParticleDamage(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;	// �G�t�F�N�g�̈ړ��ʂ̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 9.0f;
		move.y *= 9.0f;
		move.z *= 9.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			30,				// ����
			90.0f,			// ���a
			2.0f,			// ���Z�� (���a)
			EFFECTTYPE_NONE	// ���̑�
		);
	}
}

//======================================================================================================================
//	�����G�t�F�N�g
//======================================================================================================================
void ParticleExplosion(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;	// �G�t�F�N�g�̈ړ��ʂ̑���p
	int nLife;			// �G�t�F�N�g�̎����̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 4.0f;
		move.y *= 3.0f;
		move.z *= 4.0f;

		// �����������_���Őݒ�
		nLife = rand() % 30 + 50;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			nLife,			// ����
			70.0f,			// ���a
			1.5f,			// ���Z�� (���a)
			EFFECTTYPE_NONE	// ���̑�
		);
	}
}

//======================================================================================================================
//	�u�[�X�g�G�t�F�N�g
//======================================================================================================================
void ParticleBoost(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;	// �G�t�F�N�g�̈ړ��ʂ̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 5.0f;
		move.y *= 5.0f;
		move.z *= 5.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			6,				// ����
			26.0f,			// ���a
			0.2f,			// ���Z�� (���a)
			EFFECTTYPE_NONE	// ���̑�
		);
	}
}

//======================================================================================================================
// �ΉԃG�t�F�N�g
//======================================================================================================================
void ParticleSpark(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 shiftPos;	// �G�t�F�N�g�̂��炷�ʒu
	D3DXVECTOR3 move;		// �G�t�F�N�g�̈ړ��ʂ̑���p
	float fRadius;			// �G�t�F�N�g�̔��a�̑���p
	int nLife;				// �G�t�F�N�g�̎����̑���p
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 5.0f;
		move.y *= 9.0f;
		move.z *= 5.0f;

		// ���炷�ʒu��ݒ肷��
		shiftPos.x = (float)((rand() % 600) / 100.0f);
		shiftPos.y = (float)((rand() % 600) / 100.0f);
		shiftPos.z = (float)((rand() % 600) / 100.0f);

		// ���a�������_���ɐݒ�
		fRadius = (float)((rand() % 900) / 100.0f) + (pPlayer->move.x * 0.1f);

		// �����������_���ɐݒ�
		nLife = rand() % 5 + 10;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos + shiftPos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			nLife,			// ����
			fRadius,		// ���a
			0.2f,			// ���Z�� (���a)
			EFFECTTYPE_SPARK// �Ή�
		);
	}
}

//======================================================================================================================
// ���G�t�F�N�g
//======================================================================================================================
void ParticleDust(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;		// �G�t�F�N�g�̈ړ��ʂ̑���p
	float fRadius;			// �G�t�F�N�g�̔��a�̑���p
	int nLife;				// �G�t�F�N�g�̎����̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = D3DX_PI;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= (float)(rand() % 5 - 2);
		move.y *= 4.0f;
		move.z *= (float)(rand() % 5 - 2);

		// ���a�������_���ɐݒ�
		fRadius = (float)(rand() % 200 + 50.0f);

		// ������ݒ�
		nLife = (rand() % 20) + 160;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos,		// �ʒu
			move,				// �ړ���
			pParticle->col,		// �F
			nLife,				// ����
			fRadius,			// ���a
			0.1f,				// ���Z�� (���a)
			EFFECTTYPE_NONE		// �Ή�
		);
	}
}

//======================================================================================================================
// �J�̐����Ԃ��G�t�F�N�g
//======================================================================================================================
void ParticleRainSpray(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 shiftPos;	// �G�t�F�N�g�̂��炷�ʒu
	D3DXVECTOR3 move;		// �G�t�F�N�g�̈ړ��ʂ̑���p
	float fRadius;			// �G�t�F�N�g�̔��a�̑���p
	int nLife;				// �G�t�F�N�g�̎����̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 4.0f;
		move.y *= 8.0f;
		move.z *= 4.0f;

		// ���炷�ʒu��ݒ肷��
		shiftPos.x = (float)((rand() % 800) / 100.0f);
		shiftPos.y = (float)((rand() % 800) / 100.0f);
		shiftPos.z = (float)((rand() % 800) / 100.0f);

		// ���a�������_���ɐݒ�
		fRadius = (float)((rand() % 600) / 100.0f);

		// �����������_���ɐݒ�
		nLife = rand() % 5 + 10;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos + shiftPos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			nLife,			// ����
			fRadius,		// ���a
			0.2f,			// ���Z�� (���a)
			EFFECTTYPE_RAINSPRAY		// �Ή�
		);
	}
}

//======================================================================================================================
// �A�C�e�������G�t�F�N�g
//======================================================================================================================
void ParticleItemLost(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;	// �G�t�F�N�g�̈ړ��ʂ̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 2.0f;
		move.y *= 2.0f;
		move.z *= 2.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			34,				// ����
			30.0f,			// ���a
			0.5f,			// ���Z�� (���a)
			EFFECTTYPE_NONE	// ���̑�
		);
	}
}

//======================================================================================================================
// �v���C���[�̍����G�t�F�N�g
//======================================================================================================================
void ParticlePlaySmoke(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;		// �G�t�F�N�g�̈ړ��ʂ̑���p
	float fRadius;			// �G�t�F�N�g�̔��a�̑���p
	int nLife;				// �G�t�F�N�g�̎����̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = D3DX_PI;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= (float)(rand() % 5 - 2);
		move.y *= (float)(rand() % 5 + 2);
		move.z *= (float)(rand() % 5 - 2);

		// ���a�������_���ɐݒ�
		fRadius = (float)(rand() % 30 + 100.0f);

		// ������ݒ�
		nLife = (rand() % 20) + 30;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			D3DXVECTOR3
			(
				pParticle->pos.x + (rand() % 80 - 40),
				pParticle->pos.y,
				pParticle->pos.z + (rand() % 80 - 40)
			),		// �ʒu
			move,				// �ړ���
			pParticle->col,		// �F
			nLife,				// ����
			fRadius,			// ���a
			-4.0f,				// ���Z�� (���a)
			EFFECTTYPE_PLAY_SMOKE	// �v���C���[�̍���
		);
	}
}

//======================================================================================================================
// ������j�󂵂����̃G�t�F�N�g
//======================================================================================================================
void ParticleBreakArticle(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;	// �G�t�F�N�g�̈ړ��ʂ̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 25.0f;
		move.y *= 25.0f;
		move.z *= 25.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			40,				// ����
			110.0f,			// ���a
			-1.0f,			// ���Z�� (���a)
			EFFECTTYPE_BREAKOBJECT	// ���̑�
		);
	}
}

//======================================================================================================================
// �I�u�W�F�N�g��j�󂵂����̃G�t�F�N�g
//======================================================================================================================
void ParticleBreakObject(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;	// �G�t�F�N�g�̈ړ��ʂ̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

	  // �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 25.0f;
		move.y *= 25.0f;
		move.z *= 25.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			40,				// ����
			220.0f,			// ���a
			-2.0f,			// ���Z�� (���a)
			EFFECTTYPE_BREAKOBJECT	// ���̑�
		);
	}
}

//======================================================================================================================
// �v���C���[�̎��S�����̃G�t�F�N�g
//======================================================================================================================
void ParticlePlayerDeath(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 shiftPos;	// �G�t�F�N�g�̂��炷�ʒu
	D3DXVECTOR3 move;		// �G�t�F�N�g�̈ړ��ʂ̑���p
	float fRadius;			// �G�t�F�N�g�̔��a�̑���p
	int nLife;				// �G�t�F�N�g�̎����̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 8.0f;
		move.y *= 8.0f;
		move.z *= 8.0f;

		// ���炷�ʒu��ݒ肷��
		shiftPos.x = (float)((rand() % 600) / 100.0f);
		shiftPos.y = (float)((rand() % 600) / 100.0f);
		shiftPos.z = (float)((rand() % 600) / 100.0f);

		// ���a�������_���ɐݒ�
		fRadius = (float)((rand() % 600) / 100.0f) + 80.0f;

		// �����������_���ɐݒ�
		nLife = rand() % 7 + 20;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos + shiftPos,	// �ʒu
			move,						// �ړ���
			pParticle->col,				// �F
			nLife,						// ����
			fRadius,					// ���a
			0.4f,						// ���Z�� (���a)
			EFFECTTYPE_EXPLOSION		// ����
		);
	}
}

//======================================================================================================================
// �I�u�W�F�N�g�𐁂���΂������̃G�t�F�N�g
//======================================================================================================================
void ParticleObjectSmash(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;	// �G�t�F�N�g�̈ړ��ʂ̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 10.0f;
		move.y *= 10.0f;
		move.z *= 10.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			20,				// ����
			30.0f,			// ���a
			-3.0f,			// ���Z�� (���a)
			EFFECTTYPE_BREAKOBJECT	// ���̑�
		);
	}
}

//======================================================================================================================
// �r�C�K�X�̃G�t�F�N�g
//======================================================================================================================
void ParticleExhaustGas(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;		// �G�t�F�N�g�̈ړ��ʂ̑���p
	float fRadius;			// �G�t�F�N�g�̔��a�̑���p
	int nLife;				// �G�t�F�N�g�̎����̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		//// �x�N�g���𐳋K��
		//D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x = sinf(GetPlayer()->rot.y) * (float)((rand() % 4));
		move.y = (float)((rand() % 5 + 3) * 0.01f);
		move.z = cosf(GetPlayer()->rot.y) * (float)((rand() % 4));

		// ���a�������_���ɐݒ�
		fRadius = (float)(rand() % 10 + 30);

		// ������ݒ�
		nLife = (rand() % 10) + 10;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			D3DXVECTOR3
			(
				pParticle->pos.x,
				pParticle->pos.y,
				pParticle->pos.z
			),		// �ʒu
			move,				// �ړ���
			pParticle->col,		// �F
			nLife,				// ����
			fRadius,			// ���a
			-4.0f,				// ���Z�� (���a)
			EFFECTTYPE_BREAKOBJECT	// �j��̉�
		);
	}
}

//======================================================================================================================
//	�o���A���˃G�t�F�N�g
//======================================================================================================================
void ParticleShotBarrier(Particle *pParticle)
{
	// �ϐ���錾
	D3DXVECTOR3 move;	// �G�t�F�N�g�̈ړ��ʂ̑���p

	for (int nCntAppear = 0; nCntAppear < pParticle->nSpawn; nCntAppear++)
	{ // �p�[�e�B�N���� 1F�Ő��������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f);
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f);
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f);

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ���Z
		move.x *= 4.0f;
		move.y *= 4.0f;
		move.z *= 4.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect
		( // ����
			pParticle->pos,	// �ʒu
			move,			// �ړ���
			pParticle->col,	// �F
			30,				// ����
			40.0f,			// ���a
			1.2f,			// ���Z�� (���a)
			EFFECTTYPE_NONE	// ���̑�
		);
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
//**********************************************************************************************************************
//	�p�[�e�B�N���̑����擾����
//**********************************************************************************************************************
int GetNumParticle(void)
{
	// �ϐ���錾
	int nNumParticle = 0;	// �p�[�e�B�N���̑����̊m�F�p

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{ // �p�[�e�B�N���̍ő�\�������J��Ԃ�

		if (g_aParticle[nCntParticle].bUse == true)
		{ // �p�[�e�B�N�����g�p����Ă���ꍇ

			// �J�E���^�[�����Z
			nNumParticle++;
		}
	}

	// �ϐ��̒l��Ԃ�
	return nNumParticle;	// �p�[�e�B�N���̑���
}
#endif