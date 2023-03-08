//========================================
//
//�R���{�̃��C������[Combo.cpp]
//Author ��������
//
//========================================
#include "main.h"
#include "Combo.h"
#include "score.h"
#include "value.h"

//�}�N����`
#define MAX_COMBO			(2)			// �g�p����|���S����

#define MAX_CONBOCOUNT		(99)		// �R���{�J�E���g�̍ő�
#define COMBOSTOP_CNT		(600)		// �R���{�̎~�܂�J�E���g
#define COMBO_CLEAR_CNT		(300)		// �R���{�������ɂȂ�n�߂�J�E���g
#define DIGIT_ONE			(1)			// 1���̋��E
#define DIGIT_TWO			(10)		// 2���̋��E

#define COM_COMBO_BG_POS	(D3DXVECTOR3(1138.0f, 583.0f, 0.0f))	// �R���{�̔w�i�̈ʒu
#define COM_SCORE_BG_POS	(D3DXVECTOR3( 918.0f, 573.0f, 0.0f))	// �R���{�X�R�A�̔w�i�̈ʒu
#define COM_COMBO_VAL_POS	(D3DXVECTOR3(1139.0f, 589.0f, 0.0f))	// �R���{�̒l�̈ʒu
#define COM_SCORE_VAL_POS	(D3DXVECTOR3(1026.0f, 576.0f, 0.0f))	// �R���{�X�R�A�̒l�̈ʒu

#define COM_COMBO_BG_SIZE	(D3DXVECTOR3( 100.0f,  50.0f, 0.0f))	// �R���{�̔w�i�T�C�Y
#define COM_SCORE_BG_SIZE	(D3DXVECTOR3( 150.0f,  50.0f, 0.0f))	// �R���{�X�R�A�̔w�i�T�C�Y
#define COM_COMBO_VAL_SIZE	(D3DXVECTOR3(  24.0f,  24.0f, 0.0f))	// �R���{�̒l�̃T�C�Y
#define COM_SCORE_VAL_SIZE	(D3DXVECTOR3(  19.0f,  19.0f, 0.0f))	// �R���{�X�R�A�̒l�̃T�C�Y

#define COM_DIGIT_PLUS		(14.0f)		// �R���{�̌��������̈ʒu���Z��
#define COM_COMBO_VAL_SPACE	(30.0f)		// �R���{�̒l�̋󔒃T�C�Y
#define COM_SCORE_VAL_SPACE	(26.0f)		// �R���{�X�R�A�̒l�̋󔒃T�C�Y

//�R���{�̃e�N�X�`��
typedef enum
{
	COMBOTEX_GROUND_COMBO = 0,	// �R���{���n
	COMBOTEX_GROUND_SCORE,		// �R���{�X�R�A���n
	COMBOTEX_MAX
}COMBOTEX;

//�v���g�^�C�v�錾

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9      g_pTextureCombo[COMBOTEX_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

Combo g_Combo;			// �R���{�̏��
int   g_nComboScore;	// �{���ł�����X�R�A
int   g_nComboCount;	// �R���{���~�܂�܂ł̃J�E���g

//�e�N�X�`���t�@�C����
const char *c_apFilenameCombo[COMBOTEX_MAX] =
{
	"data/TEXTURE/ui006.png",	// �R���{���n
	"data/TEXTURE/ui007.png",	// �R���{�X�R�A���n
};

//===========================================
//�R���{�̏���������
//===========================================
void InitCombo(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;							// ���_���ւ̃|�C���^

	// ���̏�����
	g_Combo.nMagni     = 0;		// �{��
	g_Combo.nDigit     = 0;		// ����
	g_Combo.nMoveCount = 0;		// �ړ��J�E���g
	g_Combo.fAlpha	   = 1.0f;	// �����x
	g_Combo.bMoveUp    = true;	// ��Ɉړ����Ă��邩���Ɉړ����Ă��邩
	g_Combo.bUse       = false;	// �g�p��

	// �{���ł�����X�R�A��������
	g_nComboScore = 0;

	// �R���{���~�܂�܂ł̃J�E���g��������
	g_nComboCount = 0;

	for (int nCntTexture = 0; nCntTexture < COMBOTEX_MAX; nCntTexture++)
	{ // �e�N�X�`���̐ݒ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apFilenameCombo[nCntTexture], &g_pTextureCombo[nCntTexture]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_COMBO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCombo,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	//---------------------------------------
	//	�R���{�̔w�i�̏�����
	//---------------------------------------
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(COM_COMBO_BG_POS.x - COM_COMBO_BG_SIZE.x, COM_COMBO_BG_POS.y - COM_COMBO_BG_SIZE.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(COM_COMBO_BG_POS.x + COM_COMBO_BG_SIZE.x, COM_COMBO_BG_POS.y - COM_COMBO_BG_SIZE.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(COM_COMBO_BG_POS.x - COM_COMBO_BG_SIZE.x, COM_COMBO_BG_POS.y + COM_COMBO_BG_SIZE.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(COM_COMBO_BG_POS.x + COM_COMBO_BG_SIZE.x, COM_COMBO_BG_POS.y + COM_COMBO_BG_SIZE.y, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//---------------------------------------
	//	�R���{�X�R�A�̔w�i�̏�����
	//---------------------------------------
	//���_���W�̐ݒ�
	pVtx[4].pos = D3DXVECTOR3(COM_SCORE_BG_POS.x - COM_SCORE_BG_SIZE.x, COM_SCORE_BG_POS.y - COM_SCORE_BG_SIZE.y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(COM_SCORE_BG_POS.x + COM_SCORE_BG_SIZE.x, COM_SCORE_BG_POS.y - COM_SCORE_BG_SIZE.y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(COM_SCORE_BG_POS.x - COM_SCORE_BG_SIZE.x, COM_SCORE_BG_POS.y + COM_SCORE_BG_SIZE.y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(COM_SCORE_BG_POS.x + COM_SCORE_BG_SIZE.x, COM_SCORE_BG_POS.y + COM_SCORE_BG_SIZE.y, 0.0f);

	//rhw�̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCombo->Unlock();
}

//===========================================
//�R���{�̏I������
//===========================================
void UninitCombo(void)
{
	int nCntCombo;

	for (nCntCombo = 0; nCntCombo < COMBOTEX_MAX; nCntCombo++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureCombo[nCntCombo] != NULL)
		{
			g_pTextureCombo[nCntCombo]->Release();
			g_pTextureCombo[nCntCombo] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCombo != NULL)
	{
		g_pVtxBuffCombo->Release();
		g_pVtxBuffCombo = NULL;
	}
}

//===========================================
//�R���{�̍X�V����
//===========================================
void UpdateCombo(void)
{
	if (g_Combo.nMagni > 0)
	{ // �{����0��蒴�߂��Ă����ꍇ

		// �g�p����
		g_Combo.bUse = true;
	}
	else
	{ // �{����0�ȉ��̏ꍇ

		// �g�p���Ă��Ȃ�
		g_Combo.bUse = false;
	}

	VERTEX_2D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Combo.bUse == true)
	{//�g�p���Ă����ꍇ

		// �R���{�̎~�܂�J�E���g�����Z
		g_nComboCount++;

		if (g_nComboCount >= COMBO_CLEAR_CNT)
		{ // �J�E���g�����ȏ�ɂȂ����ꍇ

			// �����x�����炵�Ă���
			g_Combo.fAlpha -= (1.0f / (COMBOSTOP_CNT - COMBO_CLEAR_CNT));

			if (g_Combo.fAlpha <= 0.0f)
			{ // �����x��0.0f�ȉ��ɂȂ����ꍇ

				// �����x��␳����
				g_Combo.fAlpha = 0.0f;
			}
		}

		if (g_nComboCount >= COMBOSTOP_CNT)
		{ // �J�E���g����萔�ɒB������

			// �����x��␳����
			g_Combo.fAlpha = 0.0f;

			// �R���{�̔{������(�X�R�A���Z)
			MagnificCombo(COMBO_INTERRUPTION);
		}

		for (int nCnt = 0; nCnt < MAX_COMBO; nCnt++)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Combo.fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Combo.fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Combo.fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Combo.fAlpha);

			pVtx += 4;		// ���_�f�[�^��4���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCombo->Unlock();
}

//===========================================
//�R���{�̕`�揈��
//===========================================
void DrawCombo(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;	// �����Ƃ̔{���ʒu�̐ݒ�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffCombo,									//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D));									//���_���\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Combo.bUse == true)
	{//�g�p����Ă���ꍇ

		//-----------------------------------
		//	�w�i�̕`��
		//-----------------------------------
		for (int nCntCombo = 0; nCntCombo < MAX_COMBO; nCntCombo++)
		{ // �g�p����|���S�������J��Ԃ�

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureCombo[nCntCombo]);

			// �|���S���̕`��
			pDevice->DrawPrimitive
			( // ����
				D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
				nCntCombo * 4,			// �`�悷��ŏ��̒��_�C���f�b�N�X
				2						// �`�悷��v���~�e�B�u��
			);
		}
		
		//-----------------------------------
		//	���l�̕`��
		//-----------------------------------
		// �����Ƃ̔{���ʒu�̐ݒ�
		pos = COM_COMBO_VAL_POS;
		pos.x += (g_Combo.nDigit == 2) ? COM_DIGIT_PLUS : 0;	// 2���̏ꍇ�ʒu�����炷

		// ���l�̐ݒ菈��
		SetValue
		( // ����
			pos,					// �ʒu
			g_Combo.nMagni,			// �l
			MAX_CONBOCOUNT,			// �l�̍ő�l
			COM_COMBO_VAL_SIZE.x,	// ����
			COM_COMBO_VAL_SIZE.y,	// �c��
			COM_COMBO_VAL_SPACE,	// ���l�Ԃ̕�
			g_Combo.fAlpha			// ���l
		);

		// ���l�̕`�揈��
		DrawValue(g_Combo.nDigit, VALUETYPE_RED);

		// ���l�̐ݒ菈��
		SetValue
		( // ����
			COM_SCORE_VAL_POS,		// �ʒu
			g_nComboScore,			// �l
			VAL_MAX_SCORE,			// �l�̍ő�l
			COM_SCORE_VAL_SIZE.x,	// ����
			COM_SCORE_VAL_SIZE.y,	// �c��
			COM_SCORE_VAL_SPACE,	// ���l�Ԃ̕�
			g_Combo.fAlpha			// ���l
		);

		// ���l�̕`�揈��
		DrawValue(VAL_SCO_DIGIT, VALUETYPE_NORMAL);
	}
}

//===========================================
// �R���{�̔{������
//===========================================
void MagnificCombo(int nMagni)
{
	if (nMagni <= COMBO_INTERRUPTION)
	{ // �{����-1�ȉ��������ꍇ

		if (g_Combo.nMagni <= 0)
		{ // �{����0�ȉ��������ꍇ

			// �{����1�ɐݒ肷��
			g_Combo.nMagni = 1;
		}

		// �X�R�A�̉��Z����
		AddScore(g_nComboScore * g_Combo.nMagni);

		// �{����0�ɂ���
		g_Combo.nMagni = 0;

		// �R���{�{�����g�p���Ȃ�
		g_Combo.bUse = false;

		// �{���ł�����X�R�A��������
		g_nComboScore = 0;

		// �R���{�̎~�܂�J�E���g��������
		g_nComboCount = 0;
	}
	else
	{ // �{����0�������������ꍇ

		// �{�������Z����
		g_Combo.nMagni += nMagni;

		if (g_Combo.nMagni >= MAX_CONBOCOUNT)
		{ // �{�����ő吔�ȏゾ�����ꍇ

			// �R���{�̔{����␳����
			g_Combo.nMagni = MAX_CONBOCOUNT;
		}

		if (g_Combo.nMagni >= DIGIT_TWO)
		{ // 10�ȏゾ�����ꍇ

			// ������ݒ肷��
			g_Combo.nDigit = 2;
		}
		else if (g_Combo.nMagni >= DIGIT_ONE)
		{ // 1�ȏゾ�����ꍇ

			// ������ݒ肷��
			g_Combo.nDigit = 1;
		}

		// �R���{�̎~�܂�J�E���g��������
		g_nComboCount = 0;

		// �����x��������
		g_Combo.fAlpha = 1.0f;
	}
}

//===========================================
// �R���{�{���̃X�R�A�̉��Z����
//===========================================
void AddComboScore(int nScore)
{
	if (g_nComboScore < VAL_MAX_SCORE)
	{ // ���݂̃X�R�A�̒l���X�R�A�̍ő�l��菬�����ꍇ

		// �X�R�A�Ɉ����̒l�����Z
		g_nComboScore += nScore;

		if (g_nComboScore > VAL_MAX_SCORE)
		{ // ���݂̃X�R�A�̒l���X�R�A�̍ő�l���傫���ꍇ

			// �X�R�A��␳
			g_nComboScore = VAL_MAX_SCORE;
		}
	}
}