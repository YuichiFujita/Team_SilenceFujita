//======================================================================================================================
//
//	�^�C�g����ʏ��� [title.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "title.h"

#include "calculation.h"
#include "light.h"
#include "camera.h"
#include "icon.h"
#include "Human.h"
#include "weather.h"
#include "shadow.h"
#include "Car.h"
#include "Police.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_TITLE			(2)			// �g�p����|���S����

#define TITLE_POS_X			(640.0f)	// �^�C�g���̒�~���̐�΍��W (x)
#define TITLE_POS_Y			(250.0f)	// �^�C�g���̒�~���̐�΍��W (y)
#define TITLE_WIDTH			(300.0f)	// �^�C�g���̉��� / 2
#define TITLE_HEIGHT		(120.0f)	// �^�C�g���̏c�� / 2

#define START_HEIGHT_UP		(500.0f)						// �X�^�[�g�̑��΍��W �� (y)
#define START_HEIGHT_DOWN	(START_HEIGHT_UP + 110.0f)		// �X�^�[�g�̑��΍��W �� (y)

#define TITLE_MOVE			(2.0f)		// �^�C�g���̈ړ���
#define TIT_FIRST_ALPHA		(0.65f)		// �w�i�̏����̓����x
#define TIT_CHANGE_ALPHA	(0.003f)	// �w�i�̓����x�̕ύX�l
#define TIT_ALPHA_HIGH		(0.8f)		// �s�����x�̍ő�l
#define TIT_ALPHA_LOW		(0.5f)		// �����x�̍ő�l

#define TRANS_CNT			(900)		// �����J�ڂ���܂ł̃J�E���g

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureTitle[] =			// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\title000.png",		// �^�C�g���̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\title001.png",		// �X�^�[�g�̃e�N�X�`�����΃p�X
};

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_TITLE)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_TITLE_TITLE = 0,			// �^�C�g��
	TEXTURE_TITLE_START,				// �X�^�[�g
	TEXTURE_TITLE_MAX,					// ���̗񋓌^�̑���
} TEXTURE_TITLE;

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_TITLE)
//**********************************************************************************************************************
typedef enum
{
	TRANSITION_TITLE_LOGO = 0,			// ���S���
	TRANSITION_TITLE_RANKING,			// �����L���O
	TRANSITION_TITLE_MAX,				// ���̗񋓌^�̑���
} TRANSITION_TITLE;

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureTitle[TEXTURE_TITLE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

D3DXVECTOR3 g_posTitle;		// �^�C�g���̐�΍��W
TITLESTATE  g_stateTitle;	// �^�C�g���̏��
float g_fAlphaTitle;		// �w�i�̓����x
float g_fChangeTitle;		// �����x�̕ύX�l
int g_nTransCount;			// �J�ڃJ�E���g
int g_nTransMode = TRANSITION_TITLE_LOGO;			// �J�ڂ��郂�[�h
bool g_bTitleSound;			// �^�C�g���̃T�E���h�̗L��

//======================================================================================================================
//	�^�C�g����ʂ̏���������
//======================================================================================================================
void InitTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�^�C�g���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D         *pVtx;					// ���_���ւ̃|�C���^

	for (int nCntTitle = 0; nCntTitle < TEXTURE_TITLE_MAX; nCntTitle++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureTitle[nCntTitle], &g_apTextureTitle[nCntTitle]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_TITLE,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	// �O���[�o���ϐ���������
	g_posTitle     = D3DXVECTOR3(TITLE_POS_X, 0.0f, 0.0f);	// �^�C�g���̐�΍��W��������
	g_stateTitle   = TITLESTATE_MOVE;						// �^�C�g���̏�Ԃ��ړ��ɏ�����
	g_fAlphaTitle  = TIT_FIRST_ALPHA;						// �w�i�̓����x��������
	g_fChangeTitle = TIT_CHANGE_ALPHA;						// �����x�̕ύX�l��������
	g_nTransCount  = 0;										// �����J�ڂ���܂ł̃J�E���g

	if (g_bTitleSound != true)
	{//�^�C�g���T�E���h���g�p������Ȃ�
		g_bTitleSound = false;
	}

	// �J�ڂ��郂�[�h��ς���
	g_nTransMode = (g_nTransMode + 1) % TRANSITION_TITLE_MAX;

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	�^�C�g���A�C�R���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	�X�^�[�g�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(0.0f        , START_HEIGHT_UP,   0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, START_HEIGHT_UP,   0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f        , START_HEIGHT_DOWN, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, START_HEIGHT_DOWN, 0.0f);

	// rhw �̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	3D�}�b�v�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���\�������̑S�̏����i3D�}�b�v�j
	InitAllAroundChunk();

	// �A�C�R���̏�����
	InitIcon();

	// �t�@�C�������[�h����S�̏���
	LoadFileChunk
	( // ����
		true,	// �Ԃ̃J�[�u
		true,	// �l�Ԃ̃J�[�u
		true,	// �X�e�[�W
		false,	// �����蔻��
		true,	// �e
		true,	// �I�u�W�F�N�g
		true	// AI
	);
}

//======================================================================================================================
//	�^�C�g����ʂ̏I������
//======================================================================================================================
void UninitTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�^�C�g���̏I��
	//------------------------------------------------------------------------------------------------------------------
	// �e�N�X�`���̔j��
	for (int nCntTitle = 0; nCntTitle < TEXTURE_TITLE_MAX; nCntTitle++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureTitle[nCntTitle] != NULL)
		{ // �ϐ� (g_apTextureTitle) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureTitle[nCntTitle]->Release();
			g_apTextureTitle[nCntTitle] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{ // �ϐ� (g_pVtxBuffTitle) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	3D�}�b�v�̏I��
	//------------------------------------------------------------------------------------------------------------------
	// ���\�I���̑S�̏����i3D�}�b�v�j
	UninitAllAroundChunk();

	// �A�C�R���̏I��
	UninitIcon();

	//------------------------------------------------------------------------------------------------------------------
	//	�T�E���h�̏I��
	//------------------------------------------------------------------------------------------------------------------
	if (g_bTitleSound == false)
	{//�^�C�g���̃T�E���h���g���Ă��Ȃ�
		// �T�E���h�̒�~
		StopSound();
	}
}

//======================================================================================================================
//	�^�C�g����ʂ̍X�V����
//======================================================================================================================
void UpdateTitle(void)
{
	// �J�ڂ܂ł̃J�E���g�����Z����
	g_nTransCount++;

	//------------------------------------------------------------------------------------------------------------------
	//	�^�C�g���̍X�V
	//------------------------------------------------------------------------------------------------------------------
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// ���o�̍X�V
	switch (g_stateTitle)
	{ // ��Ԃ̊m�F
	case TITLESTATE_MOVE:	// �ړ���Ԃ̏ꍇ

		// �^�C�g���̐�΍��W���ړ� (��)
		g_posTitle.y += TITLE_MOVE;

		if (g_posTitle.y >= TITLE_POS_Y)
		{ // �^�C�g����������؂����ꍇ

			// y���W��␳
			g_posTitle.y = TITLE_POS_Y;

			// ��Ԃ��������Ȃ��ɐݒ�
			g_stateTitle = TITLESTATE_NONE;

			//���C��BGM�̍Đ�
			if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
			{
				if (g_bTitleSound == false)
				{//�^�C�g���T�E���h���g�p���Ă��Ȃ��Ƃ�

					//�^�C�g���T�E���h���g�p
					g_bTitleSound = true;

					// �T�E���h�̍Đ��i�^�C�g���̃��C��BGM�j
					PlaySound(SOUND_LABEL_BGM_TITLE_000);
				}
			}
		}

		// �����𔲂���
		break;

	case TITLESTATE_NONE:	// �������Ă��Ȃ���Ԃ̏ꍇ

		if (GetFade() == FADE_NONE)
		{ // �t�F�[�h���Ă��Ȃ��ꍇ

			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
			||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
			||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
			{ // ����̑��삪�s��ꂽ�ꍇ

				//// �T�E���h�̍Đ�
				//PlaySound(SOUND_LABEL_SE_DECISION_00);	// ���艹00 (�V�X�e��)

				//�^�C�g���T�E���h���g�p���~
				g_bTitleSound = false;

				// ���[�h�I�� (�Q�[����ʂɈڍs)
				SetFade(MODE_GAME);
			}

			if (g_fAlphaTitle >= TIT_ALPHA_HIGH || g_fAlphaTitle <= TIT_ALPHA_LOW)
			{ // �����x���s�����A�܂��͓����ɂȂ����ꍇ

				// �ϓ��ʂ𔽓]������
				g_fChangeTitle *= -1.0f;	// �}���t�]������
			}

			// �����x�����Z
			g_fAlphaTitle += g_fChangeTitle;

			// ���_�J���[�̍X�V
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaTitle);
		}
		else
		{ // �t�F�[�h���̏ꍇ

			// ���_�J���[�̍X�V
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, TIT_ALPHA_HIGH);
		}

		// �����𔲂���
		break;
	}

	// ���o�̃X�L�b�v
	if (g_stateTitle != TITLESTATE_NONE && GetFade() == FADE_NONE)
	{ // �^�C�g�����������Ȃ���Ԃł͂Ȃ����A�t�F�[�h���Ă��Ȃ��ꍇ

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		||  GetJoyKeyTrigger(JOYKEY_A, 0)  == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		||  GetJoyKeyTrigger(JOYKEY_X, 0)  == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
		{ // �X�L�b�v���삪�s��ꂽ�ꍇ

			// y���W��␳
			g_posTitle.y = TITLE_POS_Y;

			// ��Ԃ��������Ȃ��ɐݒ�
			g_stateTitle = TITLESTATE_NONE;

			//���C��BGM�̍Đ�
			if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
			{
				if (g_bTitleSound == false)
				{//�^�C�g���T�E���h���g�p���Ă��Ȃ��Ƃ�

					//�^�C�g���T�E���h���g�p
					g_bTitleSound = true;

					// �T�E���h�̍Đ��i�^�C�g���̃��C��BGM�j
					PlaySound(SOUND_LABEL_BGM_TITLE_000);
				}
			}
		}
	}

	// ���_���W���X�V
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y - TITLE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X - TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, g_posTitle.y + TITLE_HEIGHT, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	3D�}�b�v�̍X�V
	//------------------------------------------------------------------------------------------------------------------
	// �V�C�̍X�V����
	UpdateWeather();

	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	// �Ԃ̍X�V����
	UpdateCar();

	// �l�Ԃ̍X�V����
	UpdateHuman();

	//�@�p�g�J�[�̍X�V����
	UpdatePolice();

	//------------------------------------------------------------------------------------------------------------------
	//	�����J��
	//------------------------------------------------------------------------------------------------------------------
	if (g_nTransCount >= TRANS_CNT)
	{ // ��莞�Ԍo������
		if (GetFade() == FADE_NONE)
		{ // �t�F�[�h���Ă��Ȃ��ꍇ

			switch (g_nTransMode)
			{ // ���[�h�Ŕ��f����

			case TRANSITION_TITLE_LOGO:		// ���S��ʂ̏ꍇ

				//�^�C�g���̏�Ԃ�J�ڂɕύX
				g_stateTitle = TITLESTATE_FADE;

				// ���S��ʂɑJ��
				SetFade(MODE_LOGO);

				// �����𔲂���
				break;

			case TRANSITION_TITLE_RANKING:	// �����L���O��ʂ̏ꍇ

				//�^�C�g���̏�Ԃ�J�ڂɕύX
				g_stateTitle = TITLESTATE_FADE;

				// �����L���O��ʂɑJ��
				SetFade(MODE_RANKING);

				// �����𔲂���
				break;
			}
		}
	}
}

//======================================================================================================================
//	�^�C�g����ʂ̕`�揈��
//======================================================================================================================
void DrawTitle(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	3D�}�b�v�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// ���\�`��̑S�̏���
	DrawAllAroundChunk();

	//------------------------------------------------------------------------------------------------------------------
	//	�^�C�g���̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{ // �g�p����|���S�������J��Ԃ�

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureTitle[nCntTitle]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}


}

//======================================================================================================================
//	�^�C�g���̏�Ԃ̎擾����
//======================================================================================================================
TITLESTATE GetTitleState(void)
{
	return g_stateTitle;
}
