//=======================================
//
// ���X�g�J�E���g����[lastcount.cpp]
// Author ��������
//
//=======================================
#include "main.h"
#include "lastcount.h"
#include "timer.h"
#include "value.h"

//�}�N����`
#define LASTCOUNT_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))			// ���X�g�J�E���g�̈ʒu
#define LASTCOUNT_INIT_SIZE			(D3DXVECTOR3(10.0f, 10.0f, 0.0f))										// ���X�g�J�E���g�̔��a
#define LASTCOUNT_SIZE				(D3DXVECTOR3(100.0f, 100.0f, 0.0f))										// ���X�g�J�E���g�̔��a
#define LASTCOUNT_ALPHA				(0.7f)				// ���X�g�J�E���g�̓����x
#define LASTCOUNT_ADD_SIZE			(5.0f)				// ���X�g�J�E���g�̒ǉ��T�C�Y
#define LASTCOUNT_TIME				(10)				// ���X�g�J�E���g���g����悤�ɂȂ鎞��
#define LASTCOUNT_CLEAR_ADDITION	(0.05f)				// ���X�g�J�E���g�̓����x�̉��Z
#define LASTCOUNT_CLEAR_INTERVAL	(5)					// ���X�g�J�E���g�������Ȃ�Ԋu
#define LASTCOUNT_INIT_ALPHA		(0.0f)				// ���X�g�J�E���g�̏������l
#define LASTCOUNT_SHIFT				(130.0f)			// ���X�g�J�E���g�̂��炷��

//�O���[�o���ϐ�
LASTCOUNT g_LastCount;									//���X�g�J�E���g�̏��

//==========================================
// ���X�g�J�E���g�̏���������
//==========================================
void InitLastCount(void)
{
	// ��������������
	g_LastCount.pos = LASTCOUNT_POS;				// �ʒu
	g_LastCount.size = LASTCOUNT_INIT_SIZE;			// ���a
	g_LastCount.nCount = 0;							// �J�E���g
	g_LastCount.nTime = 0;							// ����
	g_LastCount.nTimeOld = -1;						// �O��̎���
	g_LastCount.nDigit = 2;							// ����
	g_LastCount.fClear = LASTCOUNT_INIT_ALPHA;		// �����x
	g_LastCount.bUse = false;						// �g�p��
}

//========================================
// ���X�g�J�E���g�̏I������
//========================================
void UninitLastCount(void)
{
	
}

//========================================
// ���X�g�J�E���g�̍X�V����
//========================================
void UpdateLastCount(void)
{
	// �ϐ���錾
	g_LastCount.nTime = GetTime();		// ���Ԃ��擾

	if (g_LastCount.nTime <= LASTCOUNT_TIME)
	{ // ��莞�Ԃ��߂�����

		// ���X�g�J�E���g���g���Ă��邱�Ƃɂ���
		g_LastCount.bUse = true;

		if (g_LastCount.nTimeOld != g_LastCount.nTime)
		{ // ���Ԃ��O��̎��ԂƈႤ�ꍇ

			// �O��̐��l��ݒ肷��
			g_LastCount.nTimeOld = g_LastCount.nTime;

			// �����x��ݒ肷��
			g_LastCount.fClear = LASTCOUNT_ALPHA;

			// ���a��ݒ肷��
			g_LastCount.size = LASTCOUNT_INIT_SIZE;

			if (g_LastCount.nTime >= LASTCOUNT_TIME)
			{ // ���Ԃ�2���̏ꍇ

				// �ʒu��ݒ肷��
				g_LastCount.pos = D3DXVECTOR3
				(
					LASTCOUNT_POS.x + (LASTCOUNT_SHIFT * 0.5f),
					LASTCOUNT_POS.y,
					LASTCOUNT_POS.z
				);

				// ������ݒ肷��
				g_LastCount.nDigit = 2;
			}
			else
			{ // ���Ԃ�1���̏ꍇ

				// �ʒu��ݒ肷��
				g_LastCount.pos = LASTCOUNT_POS;

				// ������ݒ肷��
				g_LastCount.nDigit = 1;
			}
		}
	}

	if (g_LastCount.bUse == true)
	{ // ���X�g�J�E���g���g�p����Ă���ꍇ

		// �J�E���g�����Z����
		g_LastCount.nCount++;

		if ((g_LastCount.nCount % LASTCOUNT_CLEAR_INTERVAL) == 0)
		{ // 5F�o�߂�����

			// �����x�����Z����
			g_LastCount.fClear -= LASTCOUNT_CLEAR_ADDITION;

			if (g_LastCount.fClear <= 0.0f)
			{ // �����x��0.0f�ȉ��ɂȂ����ꍇ

				// �����x��␳����
				g_LastCount.fClear = 0.0f;
			}
		}

		// ���a�����Z����
		g_LastCount.size.x += LASTCOUNT_ADD_SIZE;
		g_LastCount.size.y += LASTCOUNT_ADD_SIZE;

		if (g_LastCount.size.x > LASTCOUNT_SIZE.x)
		{ // ���a����萔�𒴂����ꍇ

			// ���a��ݒ肷��
			g_LastCount.size = LASTCOUNT_SIZE;
		}
	}
}

//=====================================
//���X�g�J�E���g�̕`�揈��
//=====================================
void DrawLastCount(void)
{
	// ���l�̐ݒ�
	SetValue
	( // ����
		g_LastCount.pos,		// �ʒu
		g_LastCount.nTime,		// �l
		LASTCOUNT_TIME,			// �l�̍ő�l
		g_LastCount.size.x,		// ����
		g_LastCount.size.y,		// �c��
		LASTCOUNT_SHIFT,		// ���l�Ԃ̕�
		g_LastCount.fClear		// ���l
	);

	// ���l�̕`��
	DrawValue(g_LastCount.nDigit, VALUETYPE_BURNING);
}