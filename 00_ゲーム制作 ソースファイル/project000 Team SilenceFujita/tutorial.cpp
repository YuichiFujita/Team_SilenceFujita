//======================================================================================================================
//
//	�`���[�g���A����ʏ��� [tutorial.cpp]
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
#include "calculation.h"

#include "tutorial.h"

#include "2Deffect.h"
#include "2Dparticle.h"
#include "3DValue.h"
#include "ability.h"
#include "billboard.h"
#include "bomb.h"
#include "bonus.h"
#include "buildtimer.h"
#include "camera.h"
#include "Car.h"
#include "Combo.h"
#include "effect.h"
#include "flash.h"
#include "gate.h"
#include "icon.h"
#include "junk.h"
#include "life.h"
#include "light.h"
#include "meshdome.h"
#include "meshcylinder.h"
#include "meshfield.h"
#include "meshwall.h"
#include "particle.h"
#include "shadow.h"
#include "object.h"
#include "player.h"
#include "Police.h"
#include "score.h"
#include "tiremark.h"
#include "velocity.h"
#include "weather.h"
#include "wind.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define LESSON_SETUP_TXT	"data\\TXT\\lesson.txt"	// �`���[�g���A���̃��b�X���Z�b�g�A�b�v�p�̃e�L�X�g�t�@�C���̑��΃p�X

#define MAX_TUTO		(4)			// �g�p����|���S����
#define END_TUTO_TIME	(120)		// �`���[�g���A���I���܂ł̗]�C�t���[��

#define TUTO_BG_POS_X	(910.0f)	// �`���[�g���A���̔w�i�̐�΍��W (x)
#define TUTO_BG_POS_Y	(160.0f)	// �`���[�g���A���̔w�i�̐�΍��W (y)
#define TUTO_BG_WIDTH	(360.0f)	// �`���[�g���A���̔w�i�̕� / 2 (��)
#define TUTO_BG_HEIGHT	(160.0f)	// �`���[�g���A���̔w�i�̕� / 2 (����)

#define TUTORIAL_POS_X	(930.0f)	// �`���[�g���A���̐�΍��W (x)
#define TUTORIAL_POS_Y	(150.0f)	// �`���[�g���A���̐�΍��W (y)
#define TUTORIAL_WIDTH	(360.0f)	// �`���[�g���A���̕� / 2 (��)
#define TUTORIAL_HEIGHT	(150.0f)	// �`���[�g���A���̕� / 2 (����)

#define TIPS_POS_X		(200.0f)	// �`���[�g���A���̔��l�̐�΍��W (x)
#define TIPS_POS_Y		(520.0f)	// �`���[�g���A���̔��l�̐�΍��W (y)

#define RESET_POS_Z		(-2000.0f)	// �v���C���[�Đݒ莞�� z���W

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_TUTORIAL)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_TUTORIAL_BG = 0,		// �w�i
	TEXTURE_TUTORIAL_SLUM,			// ���b�X��4 (�j�Ŏ���) �̔��l
	TEXTURE_TUTORIAL_FLY,			// ���b�X��5 (����U��) �̔��l
	TEXTURE_TUTORIAL_SILENCE,		// ���b�X��6 (�������E) �̔��l
	TEXTURE_TUTORIAL_EXIT,			// ���b�X��7 (�E�o)     �̔��l
	TEXTURE_TUTORIAL_MAX,			// ���̗񋓌^�̑���
} TEXTURE_TUTORIAL;

//**********************************************************************************************************************
//	�񋓌^��` (LESSON_SETUP)
//**********************************************************************************************************************
typedef enum
{
	LESSON_SETUP_SLUMBOOST = 0,		// ���b�X��4 (�j�Ŏ���) �̓ǂݍ���
	LESSON_SETUP_FLYAWAY,			// ���b�X��5 (����U��) �̓ǂݍ���
	LESSON_SETUP_SILENCEWORLD,		// ���b�X��6 (�������E) �̓ǂݍ���
	LESSON_SETUP_MAX				// ���̗񋓌^�̑���
} LESSON_SETUP;

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureTutorial[] =	// �`���[�g���A���e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\ui005.png",		// �`���[�g���A���w�i�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips000.png",	// ���b�X��4 (�j�Ŏ���) �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips001.png",	// ���b�X��5 (����U��) �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips002.png",	// ���b�X��6 (�������E) �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips003.png",	// ���b�X��7 (�E�o)     �̔��l�̃e�N�X�`�����΃p�X
};

const int aNextLesson[] =	// ���b�X���̃J�E���^�[
{
	240,	// ���b�X��0 (�ړ�)     �̃��b�X���J�E���^�[
	150,	// ���b�X��1 (����)     �̃��b�X���J�E���^�[
	30,		// ���b�X��2 (��~)     �̃��b�X���J�E���^�[
	60,		// ���b�X��3 (���_�ύX) �̃��b�X���J�E���^�[
	30,		// ���b�X��4 (�j�Ŏ���) �̃��b�X���J�E���^�[
	60,		// ���b�X��5 (����U��) �̃��b�X���J�E���^�[
	120,	// ���b�X��6 (�������E) �̃��b�X���J�E���^�[
	0,		// ���b�X��7 (�E�o)     �̃��b�X���J�E���^�[
};

const char *apTextureLesson[] =		// ���b�X���e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\lesson000.png",	// ���b�X��0 (�ړ�)     �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson001.png",	// ���b�X��1 (����)     �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson002.png",	// ���b�X��2 (��~)     �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson003.png",	// ���b�X��3 (���_�ύX) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson004.png",	// ���b�X��4 (�j�Ŏ���) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson005.png",	// ���b�X��5 (����U��) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson006.png",	// ���b�X��6 (�������E) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson007.png",	// ���b�X��7 (�E�o)     �̃e�N�X�`�����΃p�X
};

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void UpdateTutorialUi(void);			// �`���[�g���A����UI�̍X�V����
void DrawTutorialUi(void);				// �`���[�g���A����UI�̕`�揈��

bool CheckNextSlumBoost(void);			// �j�Ŏ����̃��b�X���I���̊m�F����
bool CheckNextFlyAway(void);			// ����U���̃��b�X���I���̊m�F����
bool CheckNextSilenceWorld(void);		// �������E�̃��b�X���I���̊m�F����
void AllFalseSilenceWorld(void);		// �������E�̃��b�X���I����̍폜����

void ResetPlayer(void);					// �v���C���[�̍Đݒ菈��

void TxtSetLesson(LESSON_SETUP lesson);	// ���b�X���̃Z�b�g�A�b�v����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureTutorial[TEXTURE_TUTORIAL_MAX] = {};	// �`���[�g���A���e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9      g_apTextureLesson[LESSON_MAX] = {};				// ���b�X���e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

TUTORIALSTATE g_tutorialState;			// �`���[�g���A���̏��
int           g_nLessonState;			// ���b�X���̏��
int           g_nCounterTutorialState;	// �`���[�g���A���̏�ԊǗ��J�E���^�[
int           g_nCounterLessonState;	// ���b�X���̏�ԊǗ��J�E���^�[
bool          g_bTutorialEnd;			// ���[�h�̑J�ڏ�

//======================================================================================================================
//	�`���[�g���A����ʂ̏���������
//======================================================================================================================
void InitTutorial(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�`���[�g���A���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D        *pVtx;						// ���_���ւ̃|�C���^

	// �`���[�g���A���e�N�X�`���̓ǂݍ���
	for (int nCntTutorial = 0; nCntTutorial < TEXTURE_TUTORIAL_MAX; nCntTutorial++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureTutorial[nCntTutorial], &g_apTextureTutorial[nCntTutorial]);
	}

	// ���b�X���e�N�X�`���̓ǂݍ���
	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureLesson[nCntTutorial], &g_apTextureLesson[nCntTutorial]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_TUTO,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL
	);

	// �O���[�o���ϐ���������
	g_tutorialState         = TUTORIALSTATE_NORMAL;		// �`���[�g���A���̏��
	g_nLessonState          = LESSON_00;				// ���b�X���̏��
	g_nCounterTutorialState = 0;						// �`���[�g���A���̏�ԊǗ��J�E���^�[
	g_nCounterLessonState   = 0;						// ���b�X���̏�ԊǗ��J�E���^�[
	g_bTutorialEnd          = false;					// ���[�h�̑J�ڏ�

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------------------------------------------------------------------------------------------------
	//	���b�X���̔w�i�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(TUTO_BG_POS_X - TUTO_BG_WIDTH, TUTO_BG_POS_Y - TUTO_BG_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTO_BG_POS_X + TUTO_BG_WIDTH, TUTO_BG_POS_Y - TUTO_BG_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTO_BG_POS_X - TUTO_BG_WIDTH, TUTO_BG_POS_Y + TUTO_BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTO_BG_POS_X + TUTO_BG_WIDTH, TUTO_BG_POS_Y + TUTO_BG_HEIGHT, 0.0f);

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
	//	���b�X���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[4].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	���l�̔w�i�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[8].pos  = D3DXVECTOR3(TIPS_POS_X - TUTORIAL_WIDTH, TIPS_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(TIPS_POS_X + TUTORIAL_WIDTH, TIPS_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(TIPS_POS_X - TUTORIAL_WIDTH, TIPS_POS_Y + TUTORIAL_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(TIPS_POS_X + TUTORIAL_WIDTH, TIPS_POS_Y + TUTORIAL_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[8].rhw  = 1.0f;
	pVtx[9].rhw  = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[8].col  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[9].col  = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[10].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	pVtx[11].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[8].tex  = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex  = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	���l�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[12].pos = D3DXVECTOR3(TIPS_POS_X - TUTORIAL_WIDTH, TIPS_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(TIPS_POS_X + TUTORIAL_WIDTH, TIPS_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(TIPS_POS_X - TUTORIAL_WIDTH, TIPS_POS_Y + TUTORIAL_HEIGHT, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(TIPS_POS_X + TUTORIAL_WIDTH, TIPS_POS_Y + TUTORIAL_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();

	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// �V�C�̏���������
	InitWeather();

	// �e�̏�����
	InitShadow();

	// �A�C�R���̏�����
	InitIcon();

	// �v���C���[�̏�����
	InitPlayer();

	// �x�@�̏�����
	InitPolice();

	// �I�u�W�F�N�g�̏�����
	InitObject();

	// �Ԃ̏�����
	InitCar();

	// �l�Ԃ̏�����
	InitHuman();

	// �Q�[�g�̏�����
	InitGate();

	// ���ꂫ�̏�����
	InitJunk();

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// ���b�V���h�[���̏�����
	InitMeshDome();

	// ���b�V���V�����_�[�̏�����
	InitMeshCylinder();

	// ���b�V���t�B�[���h�̏�����
	InitMeshField();

	// ���b�V���E�H�[���̏�����
	InitMeshWall();

	// �r���{�[�h�̏�����
	InitBillboard();

	// �{�[�i�X�̏�����
	InitBonus();

	// �Č��z�^�C�}�[�̏�����
	InitBuildtimer();

	// 3D�̐��l�̏�����
	Init3DValue();

	// 2D�G�t�F�N�g�̏�����
	Init2DEffect();

	// 2D�p�[�e�B�N���̏�����
	Init2DParticle();

	// �^�C�����̏�����
	InitTireMark();

	// �����@�̏�����
	InitWind();

	// ���e�̏�����
	InitBomb();

	// �R���{�̏�����
	InitCombo();

	// �G�t�F�N�g�̏�����
	InitEffect();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �̗̓o�[�̏�����
	InitLife();

	// �\�̓o�[�̏�����
	InitAbility();

	// ���x�o�[�̏�����
	InitVelocity();

	// �X�R�A�̏�����
	InitScore();

	// �t���b�V���̏�����
	InitFlash();

	// �t�@�C�������[�h����S�̏���
	LoadFileChunk
	( // ����
		false,	// �Ԃ̃J�[�u
		false,	// �l�Ԃ̃J�[�u
		true,	// �X�e�[�W
		true,	// �����蔻��
		true,	// �e
		false,	// �I�u�W�F�N�g
		false	// AI
	);

	//// �T�E���h�̍Đ�
	//PlaySound(SOUND_LABEL_BGM_001);	// BGM (�`���[�g���A�����)
}

//======================================================================================================================
//	�`���[�g���A����ʂ̏I������
//======================================================================================================================
void UninitTutorial(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	�`���[�g���A���̏I��
	//------------------------------------------------------------------------------------------------------------------
	// �`���[�g���A���e�N�X�`���̔j��
	for (int nCntTutorial = 0; nCntTutorial < TEXTURE_TUTORIAL_MAX; nCntTutorial++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureTutorial[nCntTutorial] != NULL)
		{ // �ϐ� (g_apTextureTutorial) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureTutorial[nCntTutorial]->Release();
			g_apTextureTutorial[nCntTutorial] = NULL;
		}
	}

	// ���b�X���e�N�X�`���̔j��
	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureLesson[nCntTutorial] != NULL)
		{ // �ϐ� (g_apTextureLesson) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureLesson[nCntTutorial]->Release();
			g_apTextureLesson[nCntTutorial] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{ // �ϐ� (g_pVtxBuffTutorial) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�g�p�����\�[�X�t�@�C���̏I��
	//------------------------------------------------------------------------------------------------------------------
	// �V�C�̏I������
	UninitWeather();

	// �e�̏I��
	UninitShadow();

	// �A�C�R���̏I��
	UninitIcon();

	// �v���C���[�̏I��
	UninitPlayer();

	// �x�@�̏I��
	UninitPolice();

	// �I�u�W�F�N�g�̏I��
	UninitObject();

	// �Ԃ̏I��
	UninitCar();

	// �l�Ԃ̏I��
	UninitHuman();

	// �Q�[�g�̏I��
	UninitGate();

	// ���ꂫ�̏I��
	UninitJunk();

	// �J�����̏I��
	UninitCamera();

	// ���C�g�̏I��
	UninitLight();

	// ���b�V���h�[���̏I��
	UninitMeshDome();

	// ���b�V���V�����_�[�̏I��
	UninitMeshCylinder();

	// ���b�V���t�B�[���h�̏I��
	UninitMeshField();

	// ���b�V���E�H�[���̏I��
	UninitMeshWall();

	// �r���{�[�h�̏I��
	UninitBillboard();

	// �{�[�i�X�̏I��
	UninitBonus();

	// �Č��z�^�C�}�[�̏I��
	UninitBuildtimer();

	// 3D�̐��l�̏I��
	Uninit3DValue();

	// 2D�G�t�F�N�g�̏I��
	Uninit2DEffect();

	// 2D�p�[�e�B�N���̏I��
	Uninit2DParticle();

	// �^�C�����̏I��
	UninitTireMark();

	// �����@�̏I��
	UninitWind();

	// ���e�̏I��
	UninitBomb();

	// �R���{�̏I��
	UninitCombo();

	// �G�t�F�N�g�̏I��
	UninitEffect();

	// �p�[�e�B�N���̏I��
	UninitParticle();

	// �̗̓o�[�̏I��
	UninitLife();

	// �\�̓o�[�̏I��
	UninitAbility();

	// ���x�o�[�̏I��
	UninitVelocity();

	// �X�R�A�̏I��
	UninitScore();

	// �t���b�V���̏I��
	UninitFlash();

	//// �T�E���h�̒�~
	//StopSound();
}

//======================================================================================================================
//	�`���[�g���A����ʂ̍X�V����
//======================================================================================================================
void UpdateTutorial(void)
{
	if (g_bTutorialEnd == false)
	{ // �J�ڐݒ肪����Ă��Ȃ��ꍇ

		if (GetExit().bExit == true)
		{ // �Q�[�g����E�o�����ꍇ

			// �J�ڐݒ肪���ꂽ��Ԃɂ���
			g_bTutorialEnd = true;

			// �Q�[����ʂ̏�Ԑݒ�
			SetTutorialState(TUTORIALSTATE_END, END_TUTO_TIME);	// �I�����
		}
	}

	// �`���[�g���A����UI�̍X�V
	UpdateTutorialUi();

	switch (g_nLessonState)
	{ // ���b�X�����Ƃ̏���
	case LESSON_04:	// ���b�X��4 (�j�Ŏ���)

		if (CheckNextSlumBoost() == true)
		{ // ���̃��b�X���ɑJ�ډ\�ȏꍇ

			// ���b�X���̏�Ԃ̉��Z
			AddLessonState();
		}

		// �����𔲂���
		break;

	case LESSON_05:	// ���b�X��5 (����U��)

		if (CheckNextFlyAway() == true)
		{ // ���̃��b�X���ɑJ�ډ\�ȏꍇ

			// ���b�X���̏�Ԃ̉��Z
			AddLessonState();
		}

		// �����𔲂���
		break;

	case LESSON_06:	// ���b�X��6 (�������E)

		if (CheckNextSilenceWorld() == true)
		{ // ���̃��b�X���ɑJ�ډ\�ȏꍇ

			// ���b�X���̏�Ԃ̉��Z
			AddLessonState();
		}

		// �����𔲂���
		break;

	case LESSON_07:	// ���b�X��7 (�E�o)

		// ����

		// �����𔲂���
		break;
	}

	switch (g_tutorialState)
	{ // ��Ԃ��Ƃ̏���
	case TUTORIALSTATE_NORMAL:		// �ʏ���

		// ����

		// �����𔲂���
		break;

	case TUTORIALSTATE_END:			// �I�����

		if (g_nCounterTutorialState > 0)
		{ // �J�E���^�[�� 0���傫���ꍇ

			// �J�E���^�[�����Z
			g_nCounterTutorialState--;
		}
		else
		{ // �J�E���^�[�� 0�ȉ��̏ꍇ

			// ���[�h�I�� (�^�C�g����ʂɈڍs)
			SetFade(MODE_TITLE);
		}

		// �����𔲂���
		break;
	}

	//------------------------------------------------------------------------------------------------------------------
	//	�g�p����\�[�X�t�@�C���̍X�V
	//------------------------------------------------------------------------------------------------------------------
	// ���C�g�̍X�V
	UpdateLight();

	// �t���b�V���̍X�V����
	UpdateFlash();

	// ���b�V���h�[���̍X�V
	UpdateMeshDome();

	// ���b�V���V�����_�[�̍X�V
	UpdateMeshCylinder();

	// ���b�V���t�B�[���h�̍X�V
	UpdateMeshField();

	// ���b�V���E�H�[���̍X�V
	UpdateMeshWall();

	// �����@�̍X�V
	UpdateWind();

	// ���e�̍X�V
	UpdateBomb();

	// �v���C���[�̃`���[�g���A���X�V
	UpdateTutorialPlayer();

	// �J�����̍X�V
	UpdateCamera();

	// �^�C�����̍X�V
	UpdateTireMark();

	// �x�@�̍X�V
	UpdatePolice();

	// �I�u�W�F�N�g�̍X�V
	UpdateObject();

	// ���ꂫ�̍X�V
	UpdateJunk();

	// �Ԃ̍X�V����
	UpdateCar();

	// �l�Ԃ̍X�V
	UpdateHuman();

	// �Q�[�g�̍X�V
	UpdateGate();

	// �G�t�F�N�g�̍X�V
	UpdateEffect();

	// �p�[�e�B�N���̍X�V
	UpdateParticle();

	// 2D�G�t�F�N�g�̍X�V
	Update2DEffect();

	// 2D�p�[�e�B�N���̍X�V
	Update2DParticle();

	// �r���{�[�h�̍X�V
	UpdateBillboard();

	// �Č��z�^�C�}�[�̍X�V
	UpdateBuildtimer();

	// 3D�̐��l�̍X�V
	Update3DValue();

	// �̗̓o�[�̍X�V
	UpdateLife();

	// �\�̓o�[�̍X�V
	UpdateAbility();

	// ���x�o�[�̍X�V
	UpdateVelocity();

	// �X�R�A�̍X�V
	UpdateScore();

	// �R���{�̍X�V
	UpdateCombo();

	// �{�[�i�X�̍X�V����
	UpdateBonus();

	// �e�̍X�V
	UpdateShadow();
}

//======================================================================================================================
//	�`���[�g���A����ʂ̕`�揈��
//======================================================================================================================
void DrawTutorial(void)
{
	//------------------------------------------------------------------------------------------------------------------
	//	���C���J�����̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_MAIN);

	// ���b�V���h�[���̕`��
	DrawMeshDome();

	// ���b�V���V�����_�[�̕`��
	DrawMeshCylinder();

	// ���b�V���t�B�[���h�̕`��
	DrawMeshField(false);

	// ���b�V���E�H�[���̕`��
	DrawMeshWall();

	// �e�̕`��
	DrawShadow();

	// �v���C���[�̕`��
	DrawPlayer();

	// �^�C�����̕`��
	DrawTireMark();

	// �Č��z�^�C�}�[�̕`��
	DrawBuildtimer();

	// �I�u�W�F�N�g�̕`��
	DrawObject();

	// ���ꂫ�̕`��
	DrawJunk();

	// �x�@�̕`��
	DrawPolice();

	// �Ԃ̕`�揈��
	DrawCar();

	// �l�Ԃ̕`��
	DrawHuman();

	// �Q�[�g�̕`��
	DrawGate();

	// �r���{�[�h�̕`��
	DrawBillboard();

	// �����@�̕`��
	DrawWind();

	// ���e�̕`��
	DrawBomb();

	// �G�t�F�N�g�̕`��
	DrawEffect();

	// �p�[�e�B�N���̕`��
	DrawParticle();

	//------------------------------------------------------------------------------------------------------------------
	//	UI�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �J�����̐ݒ�
	SetCamera(CAMERATYPE_UI);

	// �̗̓o�[�̕`��
	DrawLife();

	// �\�̓o�[�̕`��
	DrawAbility();

	// ���x�o�[�̕`��
	DrawVelocity();

	// �R���{�̕`��
	DrawCombo();

	// �X�R�A�̕`��
	DrawScore();

	// �`���[�g���A����UI�̕`��
	DrawTutorialUi();

	// �{�[�i�X�̕`��
	DrawBonus();

	// 2D�G�t�F�N�g�̕`��
	Draw2DEffect();

	// 2D�p�[�e�B�N���̕`��
	Draw2DParticle();

	// �t���b�V���̕`��
	DrawFlash();
}

//======================================================================================================================
//	���b�X���̏�Ԃ̉��Z����
//======================================================================================================================
void AddLessonState(void)
{
	if (g_nLessonState < LESSON_MAX - 1)
	{ // ���݂̃��b�X�������b�X���̍ő吔 -1��菬�����ꍇ

		// ���b�X���J�E���^�[�����Z
		g_nCounterLessonState++;

		if (g_nCounterLessonState >= aNextLesson[g_nLessonState])
		{ // ���b�X���J�E���^�[�����̃��b�X���Ɉڍs����l�ȏ�ɂȂ����ꍇ

			// �J�E���^�[��������
			g_nCounterLessonState = 0;

			// ���b�X�������Z (���̃��b�X���ɐi�߂�)
			g_nLessonState++;

			switch (g_nLessonState)
			{ // ���b�X�����Ƃ̏���
			case LESSON_00:	// ���b�X��0 (�ړ�)

				// ����

				// �����𔲂���
				break;

			case LESSON_01:	// ���b�X��1 (����)

				// ����

				// �����𔲂���
				break;

			case LESSON_02:	// ���b�X��2 (��~)

				// ����

				// �����𔲂���
				break;

			case LESSON_03:	// ���b�X��3 (���_�ύX)

				// ����

				// �����𔲂���
				break;

			case LESSON_04:	// ���b�X��4 (�j�Ŏ���)

				// ���b�X���̃Z�b�g�A�b�v
				TxtSetLesson(LESSON_SETUP_SLUMBOOST);

				// �����𔲂���
				break;

			case LESSON_05:	// ���b�X��5 (����U��)

				// ���b�X���̃Z�b�g�A�b�v
				TxtSetLesson(LESSON_SETUP_FLYAWAY);

				// �����𔲂���
				break;

			case LESSON_06:	// ���b�X��6 (�������E)

				// ���b�X���̃Z�b�g�A�b�v
				TxtSetLesson(LESSON_SETUP_SILENCEWORLD);

				// �����𔲂���
				break;

			case LESSON_07:	// ���b�X��7 (�E�o)

				// �������E�̃��b�X���I����̍폜����
				AllFalseSilenceWorld();

				// �Q�[�g�̑S�J������
				AllOpenGate();

				// �����𔲂���
				break;
			}

			// �t���b�V���̐ݒ�
			SetFlash(REV_TUTORIAL_ALPHA);

			// �v���C���[�̍Đݒ�
			ResetPlayer();

			//// �T�E���h�̍Đ�
			//PlaySound(SOUND_LABEL_SE_DEC_00);	// SE (����00)
		}
	}
}

//======================================================================================================================
//	�`���[�g���A����ʂ̏�Ԑݒ菈��
//======================================================================================================================
void SetTutorialState(TUTORIALSTATE state, int nCounter)
{
	// �����̏�Ԃ���
	g_tutorialState = state;

	// �����̗]�C����
	g_nCounterTutorialState = nCounter;
}

//======================================================================================================================
//	���b�X���̏�Ԃ̎擾����
//======================================================================================================================
LESSON GetLessonState(void)
{
	// ���b�X���̏�Ԃ�Ԃ�
	return (LESSON)g_nLessonState;
}

//======================================================================================================================
//	�`���[�g���A���̏�Ԃ̎擾����
//======================================================================================================================
TUTORIALSTATE GetTutorialState(void)
{
	// �`���[�g���A���̏�Ԃ�Ԃ�
	return g_tutorialState;
}

//======================================================================================================================
//	�`���[�g���A����UI�̍X�V����
//======================================================================================================================
void UpdateTutorialUi(void)
{

}

//======================================================================================================================
//	�`���[�g���A����UI�̕`�揈��
//======================================================================================================================
void DrawTutorialUi(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//------------------------------------------------------------------------------------------------------------------
	//	���b�X���̔w�i�̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_BG]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//------------------------------------------------------------------------------------------------------------------
	//	���b�X���̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureLesson[g_nLessonState]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	if (g_nLessonState >= LESSON_04)
	{ // ���݂̃��b�X�������b�X��4 (�j�Ŏ���) �ȍ~�̏ꍇ

		//--------------------------------------------------------------------------------------------------------------
		//	���l�̔w�i�̕`��
		//--------------------------------------------------------------------------------------------------------------
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);

		//--------------------------------------------------------------------------------------------------------------
		//	���l�̕`��
		//--------------------------------------------------------------------------------------------------------------
		switch (g_nLessonState)
		{ // ���b�X�����Ƃ̏���
		case LESSON_04:	// ���b�X��4 (�j�Ŏ���)

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_SLUM]);

			// �����𔲂���
			break;

		case LESSON_05:	// ���b�X��5 (����U��)

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_FLY]);

			// �����𔲂���
			break;

		case LESSON_06:	// ���b�X��6 (�������E)

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_SILENCE]);

			// �����𔲂���
			break;

		case LESSON_07:	// ���b�X��7 (�E�o)

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_EXIT]);

			// �����𔲂���
			break;
		}

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
	}
}

//======================================================================================================================
//	�j�Ŏ����̃��b�X���I���̊m�F����
//======================================================================================================================
bool CheckNextSlumBoost(void)
{
	// �ϐ���錾
	bool bNext = true;		// ���̃��b�X���J�ڏ�

	// �|�C���^��錾
	Object *pObject = GetObjectData();		// �I�u�W�F�N�g�̏��
	Junk   *pJunk   = GetJunkData();		// ���ꂫ�̏��

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (pObject->bUse == true)
		{ // �I�u�W�F�N�g���g�p����Ă���ꍇ

			if (pObject->nBreakType == BREAKTYPE_ON)
			{ // �j��\�I�u�W�F�N�g�̏ꍇ

				// ���̃��b�X���J�ڂ��ł��Ȃ���Ԃɂ���
				bNext = false;

				// �����𔲂���
				break;
			}
		}
	}

	for (int nCntJunk = 0; nCntJunk < MAX_JUNK; nCntJunk++, pJunk++)
	{ // ���ꂫ�̍ő�\�������J��Ԃ�

		if (pJunk->bUse == true)
		{ // ���ꂫ���g�p����Ă���ꍇ

			// ���̃��b�X���J�ڂ��ł��Ȃ���Ԃɂ���
			bNext = false;

			// �����𔲂���
			break;
		}
	}

	// ���̃��b�X���J�ڏ󋵂�Ԃ�
	return bNext;
}

//======================================================================================================================
//	����U���̃��b�X���I���̊m�F����
//======================================================================================================================
bool CheckNextFlyAway(void)
{
	// �ϐ���錾
	bool bNext = true;		// ���̃��b�X���J�ڏ�

	// �|�C���^��錾
	Human *pHuman = GetHumanData();		// �l�Ԃ̏��

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++, pHuman++)
	{ // �l�Ԃ̍ő�\�������J��Ԃ�

		if (pHuman->bUse == true)
		{ // �l�Ԃ��g�p����Ă���ꍇ

			// ���̃��b�X���J�ڂ��ł��Ȃ���Ԃɂ���
			bNext = false;

			// �����𔲂���
			break;
		}
	}

	// ���̃��b�X���J�ڏ󋵂�Ԃ�
	return bNext;
}

//======================================================================================================================
//	�������E�̃��b�X���I���̊m�F����
//======================================================================================================================
bool CheckNextSilenceWorld(void)
{
	// �ϐ���錾
	bool bNext = false;		// ���̃��b�X���J�ڏ�

	// �|�C���^��錾
	Barrier *pBarrier = GetBarrierData();	// �o���A�̏��

	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
	{ // �o���A�̍ő�\�������J��Ԃ�

		if (pBarrier->bUse == true)
		{ // �o���A���g�p����Ă���ꍇ

			if (pBarrier->state == BARRIERSTATE_SET)
			{ // �o���A��������Ԃ̏ꍇ

				// ���̃��b�X���J�ڂ��ł����Ԃɂ���
				bNext = true;

				// �����𔲂���
				break;
			}
		}
	}

	// ���̃��b�X���J�ڏ󋵂�Ԃ�
	return bNext;
}

//======================================================================================================================
//	�������E�̃��b�X���I����̍폜����
//======================================================================================================================
void AllFalseSilenceWorld(void)
{
	// �|�C���^��錾
	Car     *pCar     = GetCarData();		// �Ԃ̏��
	Barrier *pBarrier = GetBarrierData();	// �o���A�̏��

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++, pCar++)
	{ // �Ԃ̍ő�\�������J��Ԃ�

		// �Ԃ��g�p���Ă��Ȃ���Ԃɂ���
		pCar->bUse = false;
	}

	for (int nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
	{ // �o���A�̍ő�\�������J��Ԃ�

		// �o���A���g�p���Ă��Ȃ���Ԃɂ���
		pBarrier->bUse = false;
	}
}

//======================================================================================================================
//	�v���C���[�̍Đݒ菈��
//======================================================================================================================
void ResetPlayer(void)
{
	// �|�C���^��錾
	Player *pPlayer = GetPlayer();	// �v���C���[�̏��

	// �ʒu��ݒ�
	pPlayer->pos    = D3DXVECTOR3(0.0f, 0.0f, RESET_POS_Z);		// ���݂̈ʒu
	pPlayer->oldPos = D3DXVECTOR3(0.0f, 0.0f, RESET_POS_Z);		// �O��̈ʒu

	// ������ݒ�
	pPlayer->rot     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���݂̌���
	pPlayer->destRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ڕW�̌���

	// �{�̏��̏�����
	pPlayer->move          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	pPlayer->state         = ACTIONSTATE_NORMAL;				// �v���C���[�̏��
	pPlayer->nLife         = PLAY_LIFE;							// �̗�
	pPlayer->nCounterState = 0;									// ��ԊǗ��J�E���^�[
	pPlayer->bMove         = false;								// �ړ���
	pPlayer->bJump         = false;								// �W�����v��
	pPlayer->nCameraState  = PLAYCAMESTATE_NORMAL;				// �J�����̏��
	pPlayer->bCameraFirst  = false;								// ��l�̃J�����̏�

	// �u�[�X�g�̏��̏�����
	pPlayer->boost.plusMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ǉ��ړ���
	pPlayer->boost.state    = BOOSTSTATE_NONE;					// �������
	pPlayer->boost.nCounter = 0;								// �����Ǘ��J�E���^�[

	// ���̏��̏�����
	pPlayer->wind.bUseWind     = false;							// ���̎g�p��
	pPlayer->wind.nCircleCount = 0;								// �ǂ��ɏo����
	pPlayer->wind.nCount       = 0;								// �����o���J�E���g
	pPlayer->wind.rot          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����o������

	// ���e�̏��̏�����
	pPlayer->bomb.state    = ATTACKSTATE_NONE;					// �U�����
	pPlayer->bomb.nCounterState = BOMB_WAIT_CNT;						// �U���Ǘ��J�E���^�[
}

//======================================================================================================================
//	���b�X���̃Z�b�g�A�b�v����
//======================================================================================================================
void TxtSetLesson(LESSON_SETUP lesson)
{
	// �ϐ���錾
	int         nEnd;			// �e�L�X�g�ǂݍ��ݏI���̊m�F�p
	D3DXVECTOR3 pos;			// �ʒu�̑���p
	D3DXVECTOR3 rot;			// �����̑���p
	D3DXVECTOR3 scale;			// �g�嗦�̑���p
	D3DXCOLOR   col;			// �F�̑���p
	D3DXVECTOR2 radius;			// ���a�̑���p
	int         nType;			// ��ނ̑���p
	int         nBreakType;		// �����̎�ނ̑���p
	int         nShadowType;	// �e�̎�ނ̑���p
	int         nCollisionType;	// �����蔻��̎�ނ̑���p
	int         nNumMat;		// �}�e���A�����̑���p
	ROTSTATE    stateRot;		// �������
	int			nWalk;			// �����^�C�v�̕ϐ�
	bool		bRecur;			// �����̕ϐ�
	int			type;			// ���

	// �ϐ��z���錾
	char         aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p
	D3DXMATERIAL aMat[MAX_MATERIAL];	// �}�e���A���̏��̑���p

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(LESSON_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			switch (lesson)
			{ // ���b�X�����Ƃ̏���
			case LESSON_SETUP_SLUMBOOST:	// ���b�X��4 (�j�Ŏ���) �̓ǂݍ���

				if (strcmp(&aString[0], "SETLESSON_OBJECT") == 0)
				{ // �ǂݍ��񂾕����� SETLESSON_OBJECT �̏ꍇ

					do
					{ // �ǂݍ��񂾕����� END_SETLESSON_OBJECT �ł͂Ȃ��ꍇ���[�v

						// �t�@�C�����當�����ǂݍ���
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "SET_OBJECT") == 0)
						{ // �ǂݍ��񂾕����� SET_OBJECT �̏ꍇ

							do
							{ // �ǂݍ��񂾕����� END_SET_OBJECT �ł͂Ȃ��ꍇ���[�v

								// �t�@�C�����當�����ǂݍ���
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "POS") == 0)
								{ // �ǂݍ��񂾕����� POS �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);		// �ʒu��ǂݍ���
								}
								else if (strcmp(&aString[0], "ROT") == 0)
								{ // �ǂݍ��񂾕����� ROT �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);		// ������ǂݍ���
								}
								else if (strcmp(&aString[0], "SCALE") == 0)
								{ // �ǂݍ��񂾕����� SCALE �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%f%f%f", &scale.x, &scale.y, &scale.z);	// �g�嗦��ǂݍ���
								}
								else if (strcmp(&aString[0], "TYPE") == 0)
								{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &nType);			// �I�u�W�F�N�g�̎�ނ�ǂݍ���
								}
								else if (strcmp(&aString[0], "BREAKTYPE") == 0)
								{ // �ǂݍ��񂾕����� BREAKTYPE �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &nBreakType);		// �����̎�ނ�ǂݍ���
								}
								else if (strcmp(&aString[0], "SHADOWTYPE") == 0)
								{ // �ǂݍ��񂾕����� SHADOWTYPE �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &nShadowType);		// �e�̎�ނ�ǂݍ���
								}
								else if (strcmp(&aString[0], "COLLTYPE") == 0)
								{ // �ǂݍ��񂾕����� COLLTYPE �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &nCollisionType);	// �����蔻��̎�ނ�ǂݍ���
								}
								else if (strcmp(&aString[0], "COLLROT") == 0)
								{ // �ǂݍ��񂾕����� COLLROT �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &stateRot);			// ������Ԃ�ǂݍ���
								}
								else if (strcmp(&aString[0], "NUMMAT") == 0)
								{ // �ǂݍ��񂾕����� NUMMAT �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &nNumMat);			// �}�e���A������ǂݍ���

									for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
									{ // �}�e���A�������J��Ԃ�

										fscanf(pFile, "%s", &aString[0]);	// �}�e���A���̔ԍ���ǂݍ��� (�s�v)
										fscanf(pFile, "%s", &aString[0]);	// �}�e���A���̗v�f��ǂݍ��� (�s�v)
										fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)

										// �g�U����ǂݍ���
										fscanf(pFile, "%f%f%f%f",
										&aMat[nCntMat].MatD3D.Diffuse.r,
										&aMat[nCntMat].MatD3D.Diffuse.g,
										&aMat[nCntMat].MatD3D.Diffuse.b,
										&aMat[nCntMat].MatD3D.Diffuse.a);

										fscanf(pFile, "%s", &aString[0]);	// �}�e���A���̔ԍ���ǂݍ��� (�s�v)
										fscanf(pFile, "%s", &aString[0]);	// �}�e���A���̗v�f��ǂݍ��� (�s�v)
										fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)

										// ������ǂݍ���
										fscanf(pFile, "%f%f%f%f",
										&aMat[nCntMat].MatD3D.Ambient.r,
										&aMat[nCntMat].MatD3D.Ambient.g,
										&aMat[nCntMat].MatD3D.Ambient.b,
										&aMat[nCntMat].MatD3D.Ambient.a);
									}
								}

							} while (strcmp(&aString[0], "END_SET_OBJECT") != 0);		// �ǂݍ��񂾕����� END_SET_OBJECT �ł͂Ȃ��ꍇ���[�v

							// �I�u�W�F�N�g�̐ݒ�
							SetObject(pos, rot, scale, &aMat[0], nType, nBreakType, nShadowType, nCollisionType, stateRot, APPEARSTATE_SLOWLY);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_OBJECT") != 0);			// �ǂݍ��񂾕����� END_SETLESSON_OBJECT �ł͂Ȃ��ꍇ���[�v
				}

				// �����𔲂���
				break;

			case LESSON_SETUP_FLYAWAY:		// ���b�X��5 (����U��) �̓ǂݍ���

				if (strcmp(&aString[0], "SETLESSON_HUMAN") == 0)
				{ // �ǂݍ��񂾕����� SETLESSON_HUMAN �̏ꍇ
					do
					{ // �ǂݍ��񂾕����� END_SETLESSON_HUMAN �ł͂Ȃ��ꍇ���[�v

						// �t�@�C�����當�����ǂݍ���
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "SET_HUMAN") == 0)
						{ // �ǂݍ��񂾕����� SET_HUMAN �̏ꍇ

							do
							{ // �ǂݍ��񂾕����� END_SET_HUMAN �ł͂Ȃ��ꍇ���[�v

								// �t�@�C�����當�����ǂݍ���
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "POS") == 0)
								{ // �ǂݍ��񂾕����� POS �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// �ʒu��ǂݍ���
								}
								else if (strcmp(&aString[0], "WALK") == 0)
								{ // �ǂݍ��񂾕����� WALK �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &nWalk);						// �����̃^�C�v��ǂݍ���
								}
								else if (strcmp(&aString[0], "RECUR") == 0)
								{ // �ǂݍ��񂾕����� RECUR �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%s", &aString[0]);					// �����󋵂�ǂݍ���

									if (strcmp(&aString[0], "TRUE") == 0)
									{ // �ǂݍ��񂾕����� TRUE �̏ꍇ

										// ��������
										bRecur = true;
									}
									else
									{ // �ǂݍ��񂾕����� FALSE �̏ꍇ

										// �������Ȃ�
										bRecur = false;
									}
								}
								else if (strcmp(&aString[0], "TYPE") == 0)
								{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &type);							// ��ނ�ǂݍ���
								}

							} while (strcmp(&aString[0], "END_SET_HUMAN") != 0);		// �ǂݍ��񂾕����� END_SET_HUMAN �ł͂Ȃ��ꍇ���[�v

							// �l�Ԃ̐ݒ�
							SetHuman(pos, nWalk, bRecur, type);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_HUMAN") != 0);			// �ǂݍ��񂾕����� END_SETLESSON_HUMAN �ł͂Ȃ��ꍇ���[�v
				}

				// �����𔲂���
				break;

			case LESSON_SETUP_SILENCEWORLD:	// ���b�X��6 (�������E) �̓ǂݍ���

				if (strcmp(&aString[0], "SETLESSON_CAR") == 0)
				{ // �ǂݍ��񂾕����� SETLESSON_CAR �̏ꍇ
					do
					{ // �ǂݍ��񂾕����� END_SETLESSON_CAR �ł͂Ȃ��ꍇ���[�v

						// �t�@�C�����當�����ǂݍ���
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "SET_CAR") == 0)
						{ // �ǂݍ��񂾕����� SET_CAR �̏ꍇ

							do
							{ // �ǂݍ��񂾕����� END_SET_CAR �ł͂Ȃ��ꍇ���[�v

								// �t�@�C�����當�����ǂݍ���
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "POS") == 0)
								{ // �ǂݍ��񂾕����� POS �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// �ʒu��ǂݍ���
								}
								else if (strcmp(&aString[0], "ROT") == 0)
								{ // �ǂݍ��񂾕����� ROT �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
								}
								else if (strcmp(&aString[0], "WALK") == 0)
								{ // �ǂݍ��񂾕����� WALK �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &nWalk);						// �ړ��̃^�C�v��ǂݍ���
								}
								else if (strcmp(&aString[0], "TYPE") == 0)
								{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &type);							// ��ނ�ǂݍ���
								}

							} while (strcmp(&aString[0], "END_SET_CAR") != 0);			// �ǂݍ��񂾕����� END_SET_CAR �ł͂Ȃ��ꍇ���[�v

							// �Ԃ̐ݒ�
							SetCar(pos, rot, nWalk, type);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_CAR") != 0);			// �ǂݍ��񂾕����� END_SETLESSON_CAR �ł͂Ȃ��ꍇ���[�v
				}

				// �����𔲂���
				break;
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "���b�X���t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}