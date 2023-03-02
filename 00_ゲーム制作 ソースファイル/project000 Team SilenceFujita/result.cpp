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
#define WHEEL_RADIUS			(300.0f)		// �^�C���̔��a
#define RESULT_FINISH_COUNT		(30)			// ���U���g���I������܂ł̃J�E���g
#define RESULT_SCORE_WIDTH		(70.0f)			// �l�̏c��
#define RESULT_SCORE_HEIGHT		(100.0f)			// �l�̉���
#define RESULT_SCORE_SHIFT		(110.0f)		// �l�̉���
#define ADD_DISP_SCORE			(1000)			// �X�R�A�̑�����

#define RESULT_WORD_RADIUS_X	(500.0f)		// ���t�̔��a(X��)
#define RESULT_WORD_RADIUS_Y	(100.0f)		// ���t�̔��a(Y��)

#define TIT_CHANGE_CNT			(600)			// �����J�ڂ̃J�E���g

//**********************************************************************************************************************
// ���U���g��ʂ̃e�N�X�`��
//**********************************************************************************************************************
typedef enum
{
	RSL_WHEEL = 0,			// �^�C��
	RSL_GAMECLEAR,			// �Q�[���N���A
	RSL_GAMEOVER,			// �Q�[���I�[�o�[
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
void WheelUpdateRsl(void);			// �^�C���̍X�V����
void RslWordUpdateRsl(void);		// ���t�̍X�V����

//**********************************************************************************************************************
// �O���[�o���ϐ��錾
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureResult[RSL_MAX] = {};			// �e�N�X�`��(2����)�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultWheel = NULL;		// ��ʂ̒��_�o�b�t�@�ւ̃|�C���^(�^�C��)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultWord = NULL;		// ��ʂ̒��_�o�b�t�@�ւ̃|�C���^(�^�C��)
int g_nFadeTitleCounter;									// �^�C�g���J�ڃJ�E���^�[
int g_nResultCounter;										// ���U���g�J�E���^�[
WHEEL g_Wheel;												// �^�C���̏��
int g_nResultScore;											// ���U���g��ʂ̃X�R�A
int g_nDispRslScore;										// �\���p�̃X�R�A
bool g_bRslFade;											// ���U���g����J�ڂ��邩�ǂ���
D3DLIGHT9 g_RslLight;										// ���U���g�̃��C�g
RESULTSTATE g_ResultState;									// �Q�[���I�����̏��
RSLWORD g_RslWord;											// �����̏��

//**********************************************************************************************************************
// �e�N�X�`���t�@�C����
//**********************************************************************************************************************
const char *c_apFilenameResult[RSL_MAX] =
{
	"data/TEXTURE/ResultWheel.png",							// �^�C��
	"data/TEXTURE/GAMECLEAR.png",							// �Q�[���N���A
	"data/TEXTURE/GAMEOVER.png",							// �Q�[���I�[�o�[
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
		// �^�C�g���J�ڃJ�E���^�[������������
		g_nFadeTitleCounter = TIT_CHANGE_CNT;

		// ���U���g�J�E���^�[������������
		g_nResultCounter = 0;

		// �^�C���̏���������
		g_Wheel.pos = D3DXVECTOR3(400.0f, 300.0f, 0.0f);		// �ʒu
		g_Wheel.fRot = 0.0f;									// ����
		g_Wheel.fLength = sqrtf(WHEEL_RADIUS * WHEEL_RADIUS + WHEEL_RADIUS * WHEEL_RADIUS) * 0.5f;			// ����
		g_Wheel.fAngle = atan2f(WHEEL_RADIUS, WHEEL_RADIUS);	// ����
		g_Wheel.fRotMove = 0.2f;								// �����̈ړ���

		// ���t�̏���������
		g_RslWord.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 150.0f, 0.0f);		// ���S
		g_RslWord.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���

		g_nDispRslScore = 0;				// �\���p�̃X�R�A
		g_bRslFade = false;					// ���U���g����J�ڂ��邩�ǂ���

		g_ResultState = GetResultState();	// �Q�[���̏��
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

	{ // ���t
		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffResultWord,
			NULL);

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffResultWord->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS_X, g_RslWord.pos.y - RESULT_WORD_RADIUS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS_X, g_RslWord.pos.y - RESULT_WORD_RADIUS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS_X, g_RslWord.pos.y + RESULT_WORD_RADIUS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS_X, g_RslWord.pos.y + RESULT_WORD_RADIUS_Y, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (g_ResultState == RESULTSTATE_CLEAR)
		{ // �Q�[���N���A��Ԃ̏ꍇ

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
		}
		else if (g_ResultState == RESULTSTATE_TIMEOVER
			|| g_ResultState == RESULTSTATE_LIFEOVER)
		{ // �Q�[���I�[�o�[��Ԃ̏ꍇ

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.7f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.7f, 1.0f);

			// �X�R�A�̉��Z����
			AddScore(-(int)(GetScore() * 0.5f));
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffResultWord->Unlock();
	}

	g_nResultScore = GetScore();		// ���U���g�̃X�R�A

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
			true	// AI
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
			false	// AI
		);
	}

	////�T�E���h�̍Đ��i�����L���O�I���܂Łj��AnarchyCarsBGM
	//PlaySound(SOUND_LABEL_BGM_RESULT_000);

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

	if (g_pVtxBuffResultWord != NULL)
	{
		g_pVtxBuffResultWord->Release();
		g_pVtxBuffResultWord = NULL;
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

	// �^�C���̍X�V����
	WheelUpdateRsl();

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
		g_nDispRslScore += ADD_DISP_SCORE;
	}
	else
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

	{ // ���t
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuffResultWord,						//���_�o�b�t�@�ւ̃|�C���^
			0,
			sizeof(VERTEX_2D));							//���_���\���̂̃T�C�Y


		if (g_ResultState == RESULTSTATE_CLEAR)
		{ // �Q�[���N���A��Ԃ̏ꍇ
	
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureResult[RSL_GAMECLEAR]);
		}
		else if (g_ResultState == RESULTSTATE_TIMEOVER
			 ||  g_ResultState == RESULTSTATE_LIFEOVER)
		{ // �Q�[���I�[�o�[��Ԃ̏ꍇ
	
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureResult[RSL_GAMEOVER]);
		}

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
		D3DXVECTOR3
		( // ����
			1100.0f,				// �ʒu (x)
			500.0f,					// �ʒu (y)
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

//=======================================
// �^�C���̍X�V����
//=======================================
void WheelUpdateRsl(void)
{
	VERTEX_2D * pVtx;										//���_���ւ̃|�C���^

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
}

//=======================================
// ���t�̍X�V����
//=======================================
void RslWordUpdateRsl(void)
{
	VERTEX_2D * pVtx;										//���_���ւ̃|�C���^

	//g_RslWord.pos.x += 

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultWord->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS_X, g_RslWord.pos.y - RESULT_WORD_RADIUS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS_X, g_RslWord.pos.y - RESULT_WORD_RADIUS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_RslWord.pos.x - RESULT_WORD_RADIUS_X, g_RslWord.pos.y + RESULT_WORD_RADIUS_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_RslWord.pos.x + RESULT_WORD_RADIUS_X, g_RslWord.pos.y + RESULT_WORD_RADIUS_Y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultWord->Unlock();
}