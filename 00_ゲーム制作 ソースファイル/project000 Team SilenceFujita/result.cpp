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

//�}�N����`
#define NUM_SCOREPOLY				(9)						// �X�R�A�̃|���S����
#define NUM_RSL_PLACE				(8)						// �X�R�A�̌�
#define DESTROY_NUMBER				(130)					// �f�X�g���C���[�{�[�i�X�����̂ɕK�v�Ȑ��l
#define NUMBER_POS_X				(480.0f)				// ���l�̒��S(X��)
#define NUMBER_POS_Y				(470.0f)				// ���l�̒��S(Y��)
#define INIT_NUMBER					(0)						// �ԍ��̏������̐��l
#define NODAMAGE_BONUS_COUNT		(0)						// �m�[�_���[�W�{�[�i�X�̏����̐��l
#define ND_SETDASCORE_POS_X			(360.0f)				// �m�[�_���[�W�̓��_�\���̍��W(X��)
#define ND_SETDASCORE_POS_Y			(-25.0f)				// �m�[�_���[�W�̓��_�\���̍��W(Y��)
#define NOPREVENT_BONUS_COUNT		(0)						// �m�[�e����{�[�i�X�̏����̐��l
#define NP_SETDASCORE_POS_X			(1075.0f)				// �e����Ȃ������{�[�i�X�̓��_�\���̍��W(X��)
#define NP_SETDASCORE_POS_Y			(-25.0f)				// �e����Ȃ������{�[�i�X�̓��_�\���̍��W(Y��)
#define DEST_SETDASCORE_POS_X		(360.0f)				// �f�X�g���C�{�[�i�X�̓��_�\���̍��W(X��)
#define DEST_SETDASCORE_POS_Y		(55.0f)					// �f�X�g���C�{�[�i�X�̓��_�\���̍��W(Y��)
#define PERFECT_SETDASCORE_POS_X	(1075.0f)				// �p�[�t�F�N�g�{�[�i�X�̓��_�\���̍��W(X��)
#define PERFECT_SETDASCORE_POS_Y	(55.0f)					// �p�[�t�F�N�g�{�[�i�X�̓��_�\���̍��W(Y��)
#define ADD_SCORE					(10000)					// �ǉ�����Ă����X�R�A
#define SOUND_ADD_SCORE_COUNT		(5)						// �T�E���h����J�E���g
#define SET_RANKING_COUNT			(300)					// �����L���O��ʂɈڍs����J�E���g
#define NUMBER_TEXTURE_PATTERN		(0.1f)					// ���l�̃e�N�X�`���̃p�^�[��

//�����L���O��ʂ̃e�N�X�`��
typedef enum
{
	RSL_SCREEN = 0,			//�w�i
	RSL_MAX
}RSLTEXTURE;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureResult[RSL_MAX] = {};			// �e�N�X�`��(2����)�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			// ��ʂ̒��_�o�b�t�@�ւ̃|�C���^
int g_nResultCounter;										// ���U���g�J�E���^�[

//�e�N�X�`���t�@�C����
const char *c_apFilenameResult[RSL_MAX] =
{
	"data/TEXTURE/Forest.jpg",						//�w�i
};

//===============================
//���U���g��ʂ̏���������
//===============================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;								//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < RSL_MAX; nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameResult[nCntTex],
			&g_apTextureResult[nCntTex]);
	}

	// ���U���g�J�E���^�[������������
	g_nResultCounter = 0;

	{//�w�i
		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResult,
			NULL);

		VERTEX_2D * pVtx;										//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
		g_pVtxBuffResult->Unlock();
	}
}

//======================================
//���U���g��ʂ̏I������
//======================================
void UninitResult(void)
{
	//�T�E���h�̒�~
	StopSound();

	for (int nReslCount = 0; nReslCount < RSL_MAX; nReslCount++)
	{//�j��
		//�e�N�X�`���̔j��
		if (g_apTextureResult[nReslCount] != NULL)
		{
			g_apTextureResult[nReslCount]->Release();
			g_apTextureResult[nReslCount] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//=======================================
//���U���g��ʂ̍X�V����
//=======================================
void UpdateResult(void)
{
	FADE pFade = GetFade();									//�t�F�[�h�̏�Ԃ��擾����

	//���U���g��ʂɂȂ����u�ԉ��Z���n�߂�
	g_nResultCounter++;

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		|| GetJoyKeyTrigger(JOYKEY_A, 0) == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		|| GetJoyKeyTrigger(JOYKEY_X, 0) == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true
		|| g_nResultCounter >= SET_RANKING_COUNT)
	{ // ����̑��삪�s��ꂽ�ꍇ

		if (pFade == FADE_NONE)
		{//�t�F�[�h���Ă��Ȃ��ꍇ

			//�^�C�g���Ɉڍs
			SetFade(MODE_TITLE);
		}
	}
}

//=======================================
//���U���g��ʂ̕`�揈��
//=======================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	{//�w�i
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuffResult,							//���_�o�b�t�@�ւ̃|�C���^
			0,
			sizeof(VERTEX_2D));							//���_���\���̂̃T�C�Y

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureResult[RSL_SCREEN]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);											//�`�悷��v���~�e�B�u��
	}
}