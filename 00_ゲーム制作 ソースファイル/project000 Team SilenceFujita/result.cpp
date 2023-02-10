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
#include "light.h"
#include "score.h"
#include "weather.h"

//�}�N����`
#define WHEEL_RADIUS		(300.0f)		// �^�C���̔��a
#define RESULT_FINISH_COUNT	(30)			// ���U���g���I������܂ł̃J�E���g
#define RESULT_SCORE_WIDTH	(20.0f)			// �l�̏c��
#define RESULT_SCORE_HEIGHT	(30.0f)			// �l�̉���
#define RESULT_SCORE_SHIFT	(50.0f)			// �l�̉���
#define ADD_DISP_SCORE		(1000)			// �X�R�A�̑�����

//�����L���O��ʂ̃e�N�X�`��
typedef enum
{
	RSL_WHEEL = 0,			// �^�C��
	RSL_MAX					// ���̗񋓌^�̑���
}RSLTEXTURE;

//**********************************************************************************************************************
//	�\���̒�` (WHEEL)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	float		fRot;		// ����
	float		fRotMove;	// �����̈ړ���
	float		fAngle;		// ����
	float		fLength;	// ����
}WHEEL;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureResult[RSL_MAX] = {};			// �e�N�X�`��(2����)�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultWheel = NULL;		// ��ʂ̒��_�o�b�t�@�ւ̃|�C���^(�^�C��)
int g_nResultCounter;										// ���U���g�J�E���^�[
WHEEL g_Wheel;												// �^�C���̏��
int g_nResultScore;											// ���U���g��ʂ̃X�R�A
int g_nDispRslScore;										// �\���p�̃X�R�A
bool g_bRslFade;											// ���U���g����J�ڂ��邩�ǂ���
D3DLIGHT9 g_RslLight;										// ���U���g�̃��C�g

//�e�N�X�`���t�@�C����
const char *c_apFilenameResult[RSL_MAX] =
{
	"data/TEXTURE/ResultWheel.png",							// �^�C��
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

	for (int nCntTex = 0; nCntTex < RSL_MAX; nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameResult[nCntTex],
			&g_apTextureResult[nCntTex]);
	}

	{ // ���̏�����
		// ���U���g�J�E���^�[������������
		g_nResultCounter = 0;

		g_Wheel.pos = D3DXVECTOR3(400.0f, 300.0f, 0.0f);		// �ʒu
		g_Wheel.fRot = 0.0f;									// ����
		g_Wheel.fLength = sqrtf(WHEEL_RADIUS * WHEEL_RADIUS + WHEEL_RADIUS * WHEEL_RADIUS) * 0.5f;			// ����
		g_Wheel.fAngle = atan2f(WHEEL_RADIUS, WHEEL_RADIUS);	// ����
		g_Wheel.fRotMove = 0.2f;								// �����̈ړ���

		g_nResultScore = 30000;				// ���U���g�̃X�R�A
		g_nDispRslScore = 0;				// �\���p�̃X�R�A
		g_bRslFade = false;					// ���U���g����J�ڂ��邩�ǂ���
	}

	{ // �^�C��
		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResultWheel,
			NULL);

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffResultWheel->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot - D3DX_PI + g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[0].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot - D3DX_PI + g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot + D3DX_PI - g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[1].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot + D3DX_PI - g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot - g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[2].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot - g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot + g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[3].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot + g_Wheel.fAngle) * g_Wheel.fLength;
		pVtx[3].pos.z = 0.0f;

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
		g_pVtxBuffResultWheel->Unlock();
	}

	// ���U���g�̏������S�̏���
	InitResultChunk();

	// �t�@�C�������[�h����S�̏���
	LoadFileChunk
	(
		false,
		false,
		true,
		true,
		false
	);
}

//======================================
//���U���g��ʂ̏I������
//======================================
void UninitResult(void)
{
	//�T�E���h�̒�~
	StopSound();

	// ���U���g�̏I���S�̏���
	UninitResultChunk();			

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
	if (g_pVtxBuffResultWheel != NULL)
	{
		g_pVtxBuffResultWheel->Release();
		g_pVtxBuffResultWheel = NULL;
	}
}

//=======================================
//���U���g��ʂ̍X�V����
//=======================================
void UpdateResult(void)
{
	FADE pFade = GetFade();									//�t�F�[�h�̏�Ԃ��擾����
	VERTEX_2D * pVtx;										//���_���ւ̃|�C���^

	// �V�C�̐ݒ菈��
	SetWeather();

	// �V�C�̍X�V����
	UpdateWeather();

	// �J�����̍X�V����
	UpdateCamera();

	// ���C�g�̍X�V����
	UpdateLight();

	if (g_bRslFade == true)
	{ // �t�F�[�h���Ȃ��ꍇ
		// ���U���g��ʂɂȂ����u�ԉ��Z���n�߂�
		g_nResultCounter++;
	}

	if (g_nDispRslScore <= g_nResultScore)
	{ // �\���X�R�A���ق�Ƃ̃X�R�A�ɂȂ�܂�
		// �\���X�R�A�����Z����
		g_nDispRslScore += ADD_DISP_SCORE;
	}

	if (g_nDispRslScore > g_nResultScore)
	{ // �\���X�R�A���{���̃X�R�A�𒴂����ꍇ
		// �X�R�A���Œ肷��
		g_nDispRslScore = g_nResultScore;
	}

	// �������ړ�������
	g_Wheel.fRot += g_Wheel.fRotMove;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultWheel->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot - D3DX_PI + g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[0].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot - D3DX_PI + g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot + D3DX_PI - g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[1].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot + D3DX_PI - g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot - g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[2].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot - g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Wheel.pos.x + sinf(g_Wheel.fRot + g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[3].pos.y = g_Wheel.pos.y + cosf(g_Wheel.fRot + g_Wheel.fAngle) * g_Wheel.fLength;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultWheel->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_SPACE) == true
		|| GetJoyKeyTrigger(JOYKEY_A, 0) == true || GetJoyKeyTrigger(JOYKEY_B, 0) == true
		|| GetJoyKeyTrigger(JOYKEY_X, 0) == true || GetJoyKeyTrigger(JOYKEY_Y, 0) == true)
	{ // ����̑��삪�s��ꂽ�ꍇ
		// �t�F�[�h����
		g_bRslFade = true;

		// �X�R�A��\������
		g_nDispRslScore = g_nResultScore;

		if (g_nResultCounter >= RESULT_FINISH_COUNT)
		{ // �J�E���g����萔�ɒB�����ꍇ
			if (pFade == FADE_NONE)
			{ // �t�F�[�h���Ă��Ȃ��ꍇ

				// �^�C�g���Ɉڍs
				SetFade(MODE_TITLE);
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

	//{ // �^�C��
	//	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//	pDevice->SetStreamSource(0,
	//		g_pVtxBuffResultWheel,						//���_�o�b�t�@�ւ̃|�C���^
	//		0,
	//		sizeof(VERTEX_2D));							//���_���\���̂̃T�C�Y

	//	//�e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, g_apTextureResult[RSL_WHEEL]);

	//	//�|���S���̕`��
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
	//		0,											//�`�悷��ŏ��̒��_�C���f�b�N�X
	//		2);											//�`�悷��v���~�e�B�u��
	//}

	//------------------------------------------------------------------------------------------------------------------
	//	���l�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// ���l�̐ݒ�
	SetValue
	( // ����
		D3DXVECTOR3
		( // ����
			SCREEN_WIDTH * 0.5f,	// �ʒu (x)
			SCREEN_HEIGHT * 0.5f,	// �ʒu (y)
			0.0f					// �ʒu (z)
		),
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