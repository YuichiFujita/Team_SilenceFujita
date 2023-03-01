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
#define COMBO_GROUND_X			(100.0f)						// ���n�̔��a(X��)
#define COMBO_GROUND_Y			(50.0f)							// ���n�̔��a(Y��)
#define COMBO_NUMBER_X			(19.0f)							// �����̔��a(X��)
#define COMBO_NUMBER_Y			(19.0f)							// �����̔��a(Y��)
#define COMBO_NUMBER_SHIFT		(26.0f)							// �����̂��炷��

#define MAX_CONBOCOUNT			(99)							// �R���{�J�E���g�̍ő�
#define COMBOSTOP_CNT			(600)							// �R���{�̎~�܂�J�E���g
#define DIGIT_ONE				(1)								// 1���̋��E
#define DIGIT_TWO				(10)							// 2���̋��E

//�R���{�̃e�N�X�`��
typedef enum
{
	COMBOTEX_GROUND = 0,			//���n
	COMBOTEX_MAX
}COMBOTEX;

//�v���g�^�C�v�錾

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureCombo[COMBOTEX_MAX] = {};			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;					// ���_�o�b�t�@�ւ̃|�C���^

Combo g_Combo;													// �R���{�̏��
int g_nComboScore;												// �{���ł�����X�R�A
int g_nComboCount;												// �R���{���~�܂�܂ł̃J�E���g

//�e�N�X�`���t�@�C����
const char *c_apFilenameCombo[COMBOTEX_MAX] =
{
	"data/TEXTURE/ui007.png",								//���n
};

//===========================================
//�G�̏���������
//===========================================
void InitCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice;									//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���̏�����
	g_Combo.pos = D3DXVECTOR3(1150.0f, 600.0f, 0.0f);		// �ʒu
	g_Combo.nMagni		= 0;								// �{��
	g_Combo.nDigit		= 0;								// ����
	g_Combo.nMoveCount  = 0;								// �ړ��J�E���g
	g_Combo.bMoveUp		= true;								// ��Ɉړ����Ă��邩���Ɉړ����Ă��邩
	g_Combo.bUse		= false;							// �g�p��

	// �{���ł�����X�R�A��������
	g_nComboScore = 0;

	// �R���{���~�܂�܂ł̃J�E���g��������
	g_nComboCount = 0;

	for (int nCntTexture = 0; nCntTexture < COMBOTEX_MAX; nCntTexture++)
	{//�e�N�X�`���̐ݒ�
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCombo[nCntTexture],
			&g_pTextureCombo[nCntTexture]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCombo,
		NULL);

	VERTEX_2D * pVtx;											//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Combo.pos.x - COMBO_GROUND_X, g_Combo.pos.y - COMBO_GROUND_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Combo.pos.x + COMBO_GROUND_X, g_Combo.pos.y - COMBO_GROUND_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Combo.pos.x - COMBO_GROUND_X, g_Combo.pos.y + COMBO_GROUND_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Combo.pos.x + COMBO_GROUND_X, g_Combo.pos.y + COMBO_GROUND_Y, 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCombo->Unlock();
}

//===========================================
//�G�̏I������
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
//�G�̍X�V����
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

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Combo.pos.x - COMBO_GROUND_X, g_Combo.pos.y - COMBO_GROUND_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Combo.pos.x + COMBO_GROUND_X, g_Combo.pos.y - COMBO_GROUND_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Combo.pos.x - COMBO_GROUND_X, g_Combo.pos.y + COMBO_GROUND_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Combo.pos.x + COMBO_GROUND_X, g_Combo.pos.y + COMBO_GROUND_Y, 0.0f);

		if (g_nComboCount >= COMBOSTOP_CNT)
		{ // �J�E���g����萔�ɒB������

			// �R���{�̔{������(�X�R�A���Z)
			MagnificCombo(COMBO_INTERRUPTION);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCombo->Unlock();
}

//===========================================
//�v���C���[�̕`�揈��
//===========================================
void DrawCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffCombo,									//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D));									//���_���\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Combo.bUse == true)
	{//�g�p����Ă���ꍇ

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureCombo[COMBOTEX_GROUND]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);											//�`�悷��v���~�e�B�u��

		// ���l�̐ݒ菈��
		SetValue
		(
			g_Combo.pos,
			g_Combo.nMagni,
			MAX_CONBOCOUNT,
			COMBO_NUMBER_X,
			COMBO_NUMBER_Y,
			COMBO_NUMBER_SHIFT,
			1.0f);

		// ���l�̕`�揈��
		DrawValue
		(
			g_Combo.nDigit,
			VALUETYPE_RED
		);

		// ���l�̐ݒ菈��
		SetValue
		(
			D3DXVECTOR3(g_Combo.pos.x - 100.0f, g_Combo.pos.y, g_Combo.pos.z),
			g_nComboScore,
			VAL_MAX_SCORE,
			COMBO_NUMBER_X,
			COMBO_NUMBER_Y,
			COMBO_NUMBER_SHIFT,
			1.0f);

		// ���l�̕`�揈��
		DrawValue
		(
			VAL_SCO_DIGIT,
			VALUETYPE_NORMAL
		);
	}
}

//===========================================
// �R���{�̔{������
//===========================================
void MagnificCombo(int nMagni)
{
	if (nMagni <= COMBO_INTERRUPTION)
	{ // �{����-1�ȉ��������ꍇ

		// �X�R�A�̉��Z����
		AddScore(g_nComboScore * g_Combo.nMagni);

		// �{����0�ɂ���
		g_Combo.nMagni = 0;

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