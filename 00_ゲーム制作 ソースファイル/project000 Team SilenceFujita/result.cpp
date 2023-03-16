//========================================
//
//���U���g��ʂ̃��C������[Result.cpp]
//Author ��������
//
//========================================
#include "main.h"
#include "game.h"
#include "sound.h"
#include "result.h"
#include "input.h"
#include "Fade.h"
#include "calculation.h"

#include "value.h"
#include "camera.h"
#include "Car.h"
#include "Human.h"
#include "light.h"
#include "object.h"
#include "Police.h"
#include "score.h"
#include "shadow.h"
#include "weather.h"

//**********************************************************************************************************************
// �}�N����`
//**********************************************************************************************************************
#define RESULT_FINISH_COUNT		(30)			// ���U���g���I������܂ł̃J�E���g
#define RESULT_SCORE_WIDTH		(60.0f)			// �l�̏c��
#define RESULT_SCORE_HEIGHT		(60.0f)			// �l�̉���
#define RESULT_SCORE_SHIFT		(90.0f)			// �l�̂��炷��
#define RESULT_SCORE_POS		(D3DXVECTOR3(1000.0f, 560.0f, 0.0f))
#define RESULT_SCORE_BACK_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 550.0f, 0.0f))
#define RESULT_SCORE_BACK_SIZE	(D3DXVECTOR3(540.0f, 120.0f, 0.0f))

#define RESULT_WORD_RADIUS		(D3DXVECTOR3(540.0f, 75.0f, 0.0f))						// ���t�̔��a
#define RESULT_WORD_BACK_RADIUS	(D3DXVECTOR3(640.0f, 160.0f, 0.0f))						// ���t�̔w�i�̔��a
#define RESULT_WORD_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 180.0f, 0.0f))		// ���t�̈ʒu

#define RANK_CHANGE_CNT			(450)			// �����J�ڂ̃J�E���g
#define RANK_AFTERGLOW_CNT		(60)			// �X�R�A������̗]�C�̃J�E���g

//**********************************************************************************************************************
// ���U���g��ʂ̃e�N�X�`��
//**********************************************************************************************************************
typedef enum
{
	RSL_TEX_GAMECLEAR = 0,		// �Q�[���N���A
	RSL_TEX_GAMEOVER,			// �Q�[���I�[�o�[
	RSL_TEX_WORD_BACK,			// ���t�̔w�i
	RSL_TEX_SCORE_BACK,			// �X�R�A�̔w�i
	RSL_TEX_MAX					// ���̗񋓌^�̑���
}RSLTEXTURE;

//**********************************************************************************************************************
// ���U���g�\���̃|���S����
//**********************************************************************************************************************
typedef enum
{
	RSL_POLI_BACK = 0,		// �w�i�̃|���S��
	RSL_POLI_WORD,			// ���t�̃|���S��
	RSL_POLI_MAX			// ���̗񋓌^�̑���
}RSLPOLIGON;

//**********************************************************************************************************************
//	�\���̒�` (WORD)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ���S
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR2 radius;		// ���a
}RSLWORD;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************

//**********************************************************************************************************************
// �O���[�o���ϐ��錾
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureResult[RSL_TEX_MAX] = {};		// �e�N�X�`��(2����)�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultWord = NULL;		// ��ʂ̒��_�o�b�t�@�ւ̃|�C���^(���U���g�\���֌W)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultNumBack = NULL;		// ��ʂ̒��_�o�b�t�@�ւ̃|�C���^(���l�̔w�i)

int g_nFadeTitleCounter;									// �^�C�g���J�ڃJ�E���^�[
int g_nResultCounter;										// ���U���g�J�E���^�[

int g_nResultScore;											// ���U���g��ʂ̃X�R�A
int g_nDispRslScore;										// �\���p�̃X�R�A
int g_nAddDispScore;										// �\���p�̃X�R�A�̉��Z�{��

bool g_bRslFade;											// ���U���g����J�ڂ��邩�ǂ���

D3DLIGHT9 g_RslLight;										// ���U���g�̃��C�g
RESULTSTATE g_ResultState;									// �Q�[���I�����̏��
RSLWORD g_RslWord;											// �����̏��

//**********************************************************************************************************************
// �e�N�X�`���t�@�C����
//**********************************************************************************************************************
const char *c_apFilenameResult[RSL_TEX_MAX] =
{
	"data/TEXTURE/GAMECLEAR.tga",							// �Q�[���N���A
	"data/TEXTURE/GAMEOVER.tga",							// �Q�[���I�[�o�[
	"data/TEXTURE/ui008.tga",								// ���t�̔w�i
	"data/TEXTURE/ui009.tga",								// �X�R�A�̔w�i
};

//===============================
//���U���g��ʂ̏���������
//===============================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D * pVtx;										//���_���ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < RSL_TEX_MAX; nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameResult[nCntTex],
			&g_apTextureResult[nCntTex]);
	}

	{ // ���̏�����

		// �^�C�g���J�ڃJ�E���^�[������������
		g_nFadeTitleCounter = RANK_CHANGE_CNT;

		// ���U���g�J�E���^�[������������
		g_nResultCounter = 0;

		// ���t�̏���������
		g_RslWord.pos = RESULT_WORD_POS;					// ���S
		g_RslWord.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���

		g_bRslFade = false;					// ���U���g����J�ڂ��邩�ǂ���

		g_ResultState = GetResultState();	// �Q�[���̏��
	}

	if (g_ResultState == RESULTSTATE_TIMEOVER
	 || g_ResultState == RESULTSTATE_LIFEOVER)
	{ // �^�C���I�[�o�[�E���C�t�I�[�o�[��Ԃ������ꍇ

		// �X�R�A�̉��Z����
		AddScore(-(int)(GetScore() * 0.5f));
	}

	{ // ���U���g�\���n(ResultWord)

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RSL_POLI_MAX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResultWord,
			NULL);

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffResultWord->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < RSL_POLI_MAX; nCntVtx++)
		{
			switch (nCntVtx)
			{
			case RSL_POLI_WORD:		// ���t

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS.x, g_RslWord.pos.y - RESULT_WORD_RADIUS.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS.x, g_RslWord.pos.y - RESULT_WORD_RADIUS.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS.x, g_RslWord.pos.y + RESULT_WORD_RADIUS.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS.x, g_RslWord.pos.y + RESULT_WORD_RADIUS.y, 0.0f);

				break;

			case RSL_POLI_BACK:		// �w�i

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_BACK_RADIUS.x, g_RslWord.pos.y - RESULT_WORD_BACK_RADIUS.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_BACK_RADIUS.x, g_RslWord.pos.y - RESULT_WORD_BACK_RADIUS.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_BACK_RADIUS.x, g_RslWord.pos.y + RESULT_WORD_BACK_RADIUS.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_BACK_RADIUS.x, g_RslWord.pos.y + RESULT_WORD_BACK_RADIUS.y, 0.0f);

				break;
			}

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

			pVtx += 4;			// ���_�f�[�^��4���i�߂�
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffResultWord->Unlock();
	}

	{ // ���l�̔w�i(NumBack)

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResultNumBack,
			NULL);

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffResultNumBack->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(RESULT_SCORE_BACK_POS.x - RESULT_SCORE_BACK_SIZE.x, RESULT_SCORE_BACK_POS.y - RESULT_SCORE_BACK_SIZE.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(RESULT_SCORE_BACK_POS.x + RESULT_SCORE_BACK_SIZE.x, RESULT_SCORE_BACK_POS.y - RESULT_SCORE_BACK_SIZE.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RESULT_SCORE_BACK_POS.x - RESULT_SCORE_BACK_SIZE.x, RESULT_SCORE_BACK_POS.y + RESULT_SCORE_BACK_SIZE.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RESULT_SCORE_BACK_POS.x + RESULT_SCORE_BACK_SIZE.x, RESULT_SCORE_BACK_POS.y + RESULT_SCORE_BACK_SIZE.y, 0.0f);

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
		g_pVtxBuffResultNumBack->Unlock();
	}

	// ���U���g�̏������S�̏���
	InitResultChunk();

	if (g_ResultState == RESULTSTATE_CLEAR)
	{ // �Q�[���N���A��Ԃ̏ꍇ

		// �t�@�C�������[�h����S�̏���
		LoadFileChunk
		( // ����
			true,	// �Ԃ̃J�[�u
			true,	// �l�Ԃ̃J�[�u
			true,	// �X�e�[�W
			false,	// �����蔻��
			true,	// �e
			true,	// �I�u�W�F�N�g
			true,	// AI
			false	// �A�C�R��
		);
	}
	else if (g_ResultState == RESULTSTATE_TIMEOVER
		|| g_ResultState == RESULTSTATE_LIFEOVER)
	{ // �Q�[���I�[�o�[��Ԃ̏ꍇ

		// �t�@�C�������[�h����S�̏���
		LoadFileChunk
		( // ����
			false,	// �Ԃ̃J�[�u
			false,	// �l�Ԃ̃J�[�u
			true,	// �X�e�[�W
			false,	// �����蔻��
			true,	// �e
			true,	// �I�u�W�F�N�g
			false,	// AI
			false	// �A�C�R��
		);
	}

	//���C��BGM�̍Đ�
	if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
	{
		//�T�E���h�̍Đ��i�����L���O�I���܂Łj
		PlaySound(SOUND_LABEL_BGM_RESULT_000);
	}

	// �X�R�A�n�̏���������
	g_nResultScore = GetScore();		// ���U���g�̃X�R�A
	g_nAddDispScore = g_nResultScore / (RANK_CHANGE_CNT - RANK_AFTERGLOW_CNT);		// ���U���g�̃X�R�A�̉��Z�{��
	g_nDispRslScore = 0;				// �\���p�̃X�R�A
}

//======================================
//���U���g��ʂ̏I������
//======================================
void UninitResult(void)
{
	////�T�E���h�̒�~
	//StopSound();

	// ���U���g�̏I���S�̏���
	UninitResultChunk();

	for (int nReslCount = 0; nReslCount < RSL_TEX_MAX; nReslCount++)
	{//�j��
		//�e�N�X�`���̔j��
		if (g_apTextureResult[nReslCount] != NULL)
		{
			g_apTextureResult[nReslCount]->Release();
			g_apTextureResult[nReslCount] = NULL;
		}
	}

	{ // �o�b�t�@�̔j��

		// �o�b�t�@�̔j��(ResultWord)
		if (g_pVtxBuffResultWord != NULL)
		{
			g_pVtxBuffResultWord->Release();
			g_pVtxBuffResultWord = NULL;
		}

		// �o�b�t�@�̔j��(NumBack)
		if (g_pVtxBuffResultNumBack != NULL)
		{
			g_pVtxBuffResultNumBack->Release();
			g_pVtxBuffResultNumBack = NULL;
		}
	}
}

//=======================================
//���U���g��ʂ̍X�V����
//=======================================
void UpdateResult(void)
{
	// �|�C���^��錾
	FADE pFade = GetFade();	//�t�F�[�h�̏�Ԃ��擾����

	// �V�C�̐ݒ菈��
	SetWeather();

	// �V�C�̍X�V����
	UpdateWeather();

	// �J�����̍X�V����
	UpdateCamera();

	// �Ԃ̍X�V����
	UpdateCar();

	// �I�u�W�F�N�g�̍X�V����
	UpdateObject();

	// �x�@�̍X�V����
	UpdatePolice();

	// �l�Ԃ̍X�V����
	UpdateHuman();

	// ���C�g�̍X�V����
	UpdateLight();

	// �e�̍X�V����
	UpdateShadow();

	if (g_bRslFade == true)
	{ // �t�F�[�h���Ȃ��ꍇ

		// ���U���g��ʂɂȂ����u�ԉ��Z���n�߂�
		g_nResultCounter++;
	}

	if (g_nDispRslScore < g_nResultScore)
	{ // �\���X�R�A���ق�Ƃ̃X�R�A�ɂȂ�܂�

		// �\���X�R�A�����Z����
		g_nDispRslScore += g_nAddDispScore;
	}

	if (g_nDispRslScore >= g_nResultScore)
	{ // �\���X�R�A���{���̃X�R�A�𒴂����ꍇ

		// �X�R�A���Œ肷��
		g_nDispRslScore = g_nResultScore;
	}

	// �^�C�}�[�����Z
	g_nFadeTitleCounter--;

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
	||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
	||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true
	||  g_nFadeTitleCounter <= 0)
	{ // ����̑��삪�s��ꂽ�ꍇ (�������͎��Ԍo��)

		// �t�F�[�h����
		g_bRslFade = true;

		// �X�R�A��\������
		g_nDispRslScore = g_nResultScore;

		if (g_nResultCounter >= RESULT_FINISH_COUNT)
		{ // �J�E���g����萔�ɒB�����ꍇ

			if (pFade == FADE_NONE)
			{ // �t�F�[�h���Ă��Ȃ��ꍇ

				// �^�C�g���Ɉڍs
				SetFade(MODE_RANKING);
			}
		}
	}
}

//=======================================
//���U���g��ʂ̕`�揈��
//=======================================
void DrawResult(void)
{
	// ���U���g�̕`��S�̏���
	DrawResultChunk();

	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	{ // ���U���g�\���n(ResultWord)

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuffResultWord,						//���_�o�b�t�@�ւ̃|�C���^
			0,
			sizeof(VERTEX_2D));							//���_���\���̂̃T�C�Y

		for (int nCntDraw = 0; nCntDraw < RSL_POLI_MAX; nCntDraw++)
		{
			switch (nCntDraw)
			{
			case RSL_POLI_BACK:			// �w�i

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureResult[RSL_TEX_WORD_BACK]);

				break;

			case RSL_POLI_WORD:			// ���t

				if (g_ResultState == RESULTSTATE_CLEAR)
				{ // �Q�[���N���A��Ԃ̏ꍇ

					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_apTextureResult[RSL_TEX_GAMECLEAR]);
				}
				else if (g_ResultState == RESULTSTATE_TIMEOVER
					|| g_ResultState == RESULTSTATE_LIFEOVER)
				{ // �Q�[���I�[�o�[��Ԃ̏ꍇ

					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_apTextureResult[RSL_TEX_GAMEOVER]);
				}

				break;
			}

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				4 * nCntDraw,							//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);										//�`�悷��v���~�e�B�u��
		}
	}

	{ // �X�R�A�̔w�i(NumBack)

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuffResultNumBack,					//���_�o�b�t�@�ւ̃|�C���^
			0,
			sizeof(VERTEX_2D));							//���_���\���̂̃T�C�Y

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureResult[RSL_TEX_SCORE_BACK]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);											//�`�悷��v���~�e�B�u��
	}

	//------------------------------------------------------------------------------------------------------------------
	//	���l�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// ���l�̐ݒ�
	SetValue
	( // ����
		RESULT_SCORE_POS,			// �ʒu
		g_nDispRslScore,			// �l
		VAL_MAX_SCORE,				// �l�̍ő�l
		RESULT_SCORE_WIDTH,			// ����
		RESULT_SCORE_HEIGHT,		// �c��
		RESULT_SCORE_SHIFT,			// ���l�Ԃ̕�
		1.0f						// ���l
	);

	// ���l�̕`��
	DrawValue(VAL_SCO_DIGIT, VALUETYPE_NORMAL);
}