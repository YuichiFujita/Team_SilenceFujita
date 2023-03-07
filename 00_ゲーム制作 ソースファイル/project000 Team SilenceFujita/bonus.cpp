//=======================================
//
// �{�[�i�X�̃��C������[Bonus.cpp]
// Author ��������
//
//=======================================
#include "bonus.h"

#include "Combo.h"
#include "score.h"
#include "game.h"
#include "Police.h"
#include "value.h"
#include "2Deffect.h"
#include "2Dparticle.h"

//**********************************************************************************************************************
// �}�N����`
//**********************************************************************************************************************
#define BONUS_MAX_VALUE		(9999)		// �{�[�i�X�̍ő�l
#define BONUS_WIDTH			(26.0f)		// �{�[�i�X�̕�
#define BONUS_HEIGHT		(26.0f)		// �{�[�i�X�̍���
#define BONUS_SHIFT			(37.0f)		// �{�[�i�X�̂��炷��
#define BONUS_ALPHA			(1.0f)		// �{�[�i�X�̓����x
#define PLUS_WIDTH			(30.0f)		// �v���X�̕�
#define PLUS_HEIGHT			(30.0f)		// �v���X�̍���
#define PLUS_SHIFT			(40.0f)		// �v���X�̂��炷��
#define BONUS_RIGHT_X		(1080.0f)	// �E�Ƀ{�[�i�X���o�����̍��W(X���W)
#define BONUS_LEFT_X		(40.0f)		// ���Ƀ{�[�i�X���o�����̍��W(X���W)
#define BONUS_RIGHT_Y		(400)		// �E�Ƀ{�[�i�X���o�����̍��W(Y���W)
#define BONUS_RIGHT_SHIFT	(120)		// �E�Ƀ{�[�i�X���o�����̂��炷���W(Y���W)
#define BONUS_LEFT_Y		(200)		// ���Ƀ{�[�i�X���o�����̍��W(Y���W)
#define BONUS_LEFT_SHIFT	(350)		// ���Ƀ{�[�i�X���o�����̂��炷���W(Y���W)
#define BONUS_SCORE_POS		(D3DXVECTOR3( 918.0f, 573.0f, 0.0f))		// �X�R�A�̈ʒu

#define BONUS_MOVE_MAGNI	(0.02f)		// �v���X�̈ړ��ʂ̔{��
#define BONUS_STATE_CNT		(120)		// ���Z��ԂɂȂ�܂ł̃J�E���g
#define BONUS_ADD_ALPHA		(0.02f)		// �{�[�i�X�̓����x�ɉ��Z����l

#define BONUS_EFFECT_LIFE	(8)			// �{�[�i�X�̃G�t�F�N�g�̎���
#define BONUS_EFFECT_RADIUS	(40.0f)		// �{�[�i�X�̃G�t�F�N�g�̔��a
#define BONUS_EFFECT_SUB	(4.0f)		// �{�[�i�X�̃G�t�F�N�g�̌����W��

#define BONUS_DIGIT_ONE		(9)			// 1���̋��E
#define BONUS_DIGIT_TWO		(99)		// 2���̋��E
#define BONUS_DIGIT_THREE	(999)		// 3���̋��E
#define BONUS_DIGIT_FOUR	(9999)		// 4���̋��E
#define BONUS_DIGIT_FIVE	(99999)		// 5���̋��E

//**********************************************************************************************************************
//	�{�[�i�X�e�N�X�`��(BONUSTEX)
//**********************************************************************************************************************
typedef enum
{
	BONUSTEX_PLUS = 0,		// �v���X�̃e�N�X�`��
	BONUSTEX_MAX			// ���̗񋓌^�̑���
}BONUSTEX;

//**********************************************************************************************************************
//�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBonus[BONUSTEX_MAX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBonus = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Bonus g_aBonus[MAX_BONUS];			// �{�[�i�X�̏��

//**********************************************************************************************************************
// �e�N�X�`���t�@�C����
//**********************************************************************************************************************
const char *c_apFilenameBonus[BONUSTEX_MAX] = 
{
	"data/TEXTURE/Plus.png",
};

//==========================================
//���_�\���̏���������
//==========================================
void InitBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < BONUSTEX_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBonus[nCnt],
			&g_pTextureBonus[nCnt]);
	}

	//���_�̏��̏�����
	for (int nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		// ���̏�����
		g_aBonus[nCntBonus].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_aBonus[nCntBonus].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_aBonus[nCntBonus].state = BONUSSTATE_FADE;					// ���
		g_aBonus[nCntBonus].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
		g_aBonus[nCntBonus].nScore = 0;									// ���_
		g_aBonus[nCntBonus].whereBonus = WHEREBONUS_RIGHT;				// �{�[�i�X�̍��E
		g_aBonus[nCntBonus].nStateCounter = 0;							// ��ԃJ�E���^�[
		g_aBonus[nCntBonus].nDigit = 0;									// ����
		g_aBonus[nCntBonus].bUse = false;								// �g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BONUS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBonus,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBonus->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aBonus[nCntBonus].col;
		pVtx[1].col = g_aBonus[nCntBonus].col;
		pVtx[2].col = g_aBonus[nCntBonus].col;
		pVtx[3].col = g_aBonus[nCntBonus].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBonus->Unlock();
}

//========================================
//���_�\���̏I������
//========================================
void UninitBonus(void)
{
	for (int nCntBonus = 0; nCntBonus < BONUSTEX_MAX; nCntBonus++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBonus[nCntBonus] != NULL)
		{
			g_pTextureBonus[nCntBonus]->Release();
			g_pTextureBonus[nCntBonus] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBonus != NULL)
	{
		g_pVtxBuffBonus->Release();
		g_pVtxBuffBonus = NULL;
	}
}

//========================================
//���_�\���̍X�V����
//========================================
void UpdateBonus(void)
{
	int nCntBonus;					//�񐔂̕ϐ���錾����

	D3DXVECTOR3 ScorePos = BONUS_SCORE_POS;		// �X�R�A�̈ʒu���擾

	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBonus->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		if (g_aBonus[nCntBonus].bUse == true)
		{ // ���_�\�����g�p����Ă���

			switch (g_aBonus[nCntBonus].state)
			{
			case BONUSSTATE_FADE:		// �t�F�[�h���

				// �F�����Z����
				g_aBonus[nCntBonus].col.a += BONUS_ADD_ALPHA;

				if (g_aBonus[nCntBonus].col.a >= 1.0f)
				{ // �����x��1.0f�ɂȂ����ꍇ

					// �����x��1.0f�ɂ���
					g_aBonus[nCntBonus].col.a = 1.0f;

					// �\����Ԃɂ���
					g_aBonus[nCntBonus].state = BONUSSTATE_DISP;
				}

				break;					// �����o��

			case BONUSSTATE_DISP:		// �\�����

				// ��ԃJ�E���^�[�����Z����
				g_aBonus[nCntBonus].nStateCounter++;

				if (g_aBonus[nCntBonus].nStateCounter >= BONUS_STATE_CNT)
				{ // ��ԃJ�E���^�[�����ȏ�ɂȂ�����

					// �ړ��ʂ��v�Z����
					g_aBonus[nCntBonus].move.x = (ScorePos.x - g_aBonus[nCntBonus].pos.x) * BONUS_MOVE_MAGNI;
					g_aBonus[nCntBonus].move.y = (ScorePos.y - g_aBonus[nCntBonus].pos.y) * BONUS_MOVE_MAGNI;

					// �����x��ݒ肷��
					g_aBonus[nCntBonus].col.a = 0.0f;

					// ���Z��Ԃɂ���
					g_aBonus[nCntBonus].state = BONUSSTATE_ADDSCORE;
				}

				break;					// �����o��

			case BONUSSTATE_ADDSCORE:	// �X�R�A���Z���

				if (g_aBonus[nCntBonus].whereBonus == WHEREBONUS_RIGHT)
				{ // �E���ɏo���ꍇ

					// �ʒu���ړ�������
					g_aBonus[nCntBonus].pos += g_aBonus[nCntBonus].move;

					if (g_aBonus[nCntBonus].pos.x <= ScorePos.x)
					{ // �X�R�A�̈ʒu���߂����ꍇ

						// �ʒu��␳����
						g_aBonus[nCntBonus].pos = ScorePos;

						// �g�p���Ȃ�
						g_aBonus[nCntBonus].bUse = false;

						// �R���{�̃X�R�A�̉��Z����
						AddComboScore(g_aBonus[nCntBonus].nScore);

						// 2D�p�[�e�B�N���̔���
						Set2DParticle
						( // ����
							g_aBonus[nCntBonus].pos,			// �ʒu
							D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f),	// �F
							PARTICLE2DTYPE_SCORE_FIRE,			// �ԉ�
							20,									// ������
							1									// ����
						);
					}
				}
				else
				{ // �����ɏo���ꍇ

					// �ʒu���ړ�������
					g_aBonus[nCntBonus].pos += g_aBonus[nCntBonus].move;

					if (g_aBonus[nCntBonus].pos.x >= ScorePos.x)
					{ // �X�R�A�̈ʒu���߂����ꍇ

						// �ʒu��␳����
						g_aBonus[nCntBonus].pos = ScorePos;

						// �g�p���Ȃ�
						g_aBonus[nCntBonus].bUse = false;

						// �R���{�̃X�R�A�̉��Z����
						AddComboScore(g_aBonus[nCntBonus].nScore);

						// 2D�p�[�e�B�N���̔���
						Set2DParticle
						( // ����
							g_aBonus[nCntBonus].pos,			// �ʒu
							D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f),	// �F
							PARTICLE2DTYPE_SCORE_FIRE,			// �ԉ�
							20,									// ������
							1									// ����
						);
					}
				}

				// 2D�p�[�e�B�N���𔭐�������
				Set2DEffect
				(
					g_aBonus[nCntBonus].pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f),
					BONUS_EFFECT_LIFE,
					BONUS_EFFECT_RADIUS,
					BONUS_EFFECT_SUB
				);

				break;					// �����o��
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aBonus[nCntBonus].col;
			pVtx[1].col = g_aBonus[nCntBonus].col;
			pVtx[2].col = g_aBonus[nCntBonus].col;
			pVtx[3].col = g_aBonus[nCntBonus].col;
		}
		pVtx += 4;			// ���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBonus->Unlock();
}

//=====================================
//���_�\���̕`�揈��
//=====================================
void DrawBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBonus;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffBonus,										// ���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D));										// ���_���\���̂̃T�C�Y

	for (nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		if (g_aBonus[nCntBonus].bUse == true)
		{ // ���_�\�����g�p����Ă���

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBonus[BONUSTEX_PLUS]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntBonus,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u��
		}
	}

	for (nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
	{
		if (g_aBonus[nCntBonus].bUse == true)
		{ // ���_�\�����g�p����Ă���

			// ���l�̐ݒ菈��
			SetValue
			(
				D3DXVECTOR3
				(
					g_aBonus[nCntBonus].pos.x + (g_aBonus[nCntBonus].nDigit * PLUS_SHIFT),
					g_aBonus[nCntBonus].pos.y,
					0.0f
				),
				g_aBonus[nCntBonus].nScore,
				BONUS_MAX_VALUE,
				BONUS_WIDTH,
				BONUS_HEIGHT,
				BONUS_SHIFT,
				g_aBonus[nCntBonus].col.a
			);

			// ���l�̕`�揈��
			DrawValue(g_aBonus[nCntBonus].nDigit, VALUETYPE_NORMAL);
		}
	}
}

//========================================
//���_�\���̐ݒ菈��
//========================================
void SetBonus(int nBonus)
{
	VERTEX_2D * pVtx;					// ���_���ւ̃|�C���^
	D3DXVECTOR3 posBonus;				// �{�[�i�X�̈ʒu

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBonus->Lock(0, 0, (void**)&pVtx, 0);

	if (GetGameState() == GAMESTATE_NORMAL)
	{ // �Q�[���̐i�s���ʏ��Ԃ̏ꍇ

		for (int nCntBonus = 0; nCntBonus < MAX_BONUS; nCntBonus++)
		{
			if (g_aBonus[nCntBonus].bUse == false)
			{ // ���_�\�����g�p����Ă��Ȃ��ꍇ

				// �����̓��_�𑝂₷
				GetReinforce()->nBonus += 1;

				// �R���{�̔{������
				MagnificCombo(1);

				// �E�ɏo�������ɏo�����������_���ŎZ�o����
				g_aBonus[nCntBonus].whereBonus = (WHEREBONUS)(rand() % WHEREBONUS_MAX);

				switch (g_aBonus[nCntBonus].whereBonus)
				{
				case WHEREBONUS_RIGHT:	// �E�ɏo��

					// �{�[�i�X�̈ʒu��ݒ肷��
					posBonus.x = BONUS_RIGHT_X;
					posBonus.y = (float)(rand() % BONUS_RIGHT_SHIFT) + BONUS_RIGHT_Y;

					break;				// �����o��

				case WHEREBONUS_LEFT:	// ���ɏo��

					// �{�[�i�X�̈ʒu��ݒ肷��
					posBonus.x = BONUS_LEFT_X;
					posBonus.y = (float)(rand() % BONUS_LEFT_SHIFT) + BONUS_LEFT_Y;

					break;				// �����o��
				}

				// ���̐ݒ�
				g_aBonus[nCntBonus].pos = D3DXVECTOR3(posBonus.x, posBonus.y, 0.0f);	// �ʒu
				g_aBonus[nCntBonus].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
				g_aBonus[nCntBonus].state = BONUSSTATE_FADE;					// ���
				g_aBonus[nCntBonus].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	// �F
				g_aBonus[nCntBonus].nStateCounter = 0;							// ��ԃJ�E���^�[

				// �X�R�A��ݒ肷��
				g_aBonus[nCntBonus].nScore = nBonus;

				if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_ONE)
				{ // �X�R�A��9�ȏゾ�����ꍇ

					// ������ݒ肷��
					g_aBonus[nCntBonus].nDigit = 1;
				}
				else if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_TWO)
				{ // �X�R�A��99�ȏゾ�����ꍇ

					// ������ݒ肷��
					g_aBonus[nCntBonus].nDigit = 2;
				}
				else if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_THREE)
				{ // �X�R�A��999�ȏゾ�����ꍇ

					// ������ݒ肷��
					g_aBonus[nCntBonus].nDigit = 3;
				}
				else if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_FOUR)
				{ // �X�R�A��9999�ȏゾ�����ꍇ

					// ������ݒ肷��
					g_aBonus[nCntBonus].nDigit = 4;
				}
				else if (g_aBonus[nCntBonus].nScore <= BONUS_DIGIT_FIVE)
				{ // �X�R�A��99999�ȏゾ�����ꍇ

					// ������ݒ肷��
					g_aBonus[nCntBonus].nDigit = 5;
				}

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y - PLUS_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x - PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBonus[nCntBonus].pos.x + PLUS_WIDTH, g_aBonus[nCntBonus].pos.y + PLUS_HEIGHT, 0.0f);

				//���_�J���[�̐ݒ�
				pVtx[0].col = g_aBonus[nCntBonus].col;
				pVtx[1].col = g_aBonus[nCntBonus].col;
				pVtx[2].col = g_aBonus[nCntBonus].col;
				pVtx[3].col = g_aBonus[nCntBonus].col;

				//�g�p���Ă����Ԃɂ���
				g_aBonus[nCntBonus].bUse = true;

				// 2D�p�[�e�B�N���̔���
				Set2DParticle
				( // ����
					g_aBonus[nCntBonus].pos,			// �ʒu
					D3DXCOLOR(0.8f, 0.5f, 0.0f, 1.0f),	// �F
					PARTICLE2DTYPE_BONUS_FIRE,			// �ԉ�
					20,									// ������
					1									// ����
				);

				//�����o��
				break;
			}

			pVtx += 4;			//���_�f�[�^��4���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBonus->Unlock();
}

//========================================
// �{�[�i�X�̎擾����
//========================================
Bonus *GetBonus(void)
{
	// �{�[�i�X�̏���Ԃ�
	return &g_aBonus[0];
}