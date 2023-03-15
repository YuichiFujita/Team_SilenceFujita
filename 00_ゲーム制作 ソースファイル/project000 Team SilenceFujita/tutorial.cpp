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
#include "input.h"
#include "calculation.h"

#include "tutorial.h"

#include "2Deffect.h"
#include "2Dparticle.h"
#include "3Dnotation.h"
#include "ability.h"
#include "billboard.h"
#include "bomb.h"
#include "bonus.h"
#include "buildtimer.h"
#include "camera.h"
#include "Car.h"
#include "Combo.h"
#include "effect.h"
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
#include "police.h"
#include "praise.h"
#include "score.h"
#include "tiremark.h"
#include "velocity.h"
#include "weather.h"
#include "wind.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define LESSON_SETUP_TXT	"data\\TXT\\lesson.txt"	// �`���[�g���A���̃��b�X���Z�b�g�A�b�v�p�̃e�L�X�g�t�@�C���̑��΃p�X

#define MAX_TUTO		(9)			// �g�p����|���S����
#define END_TUTO_TIME	(120)		// �`���[�g���A���I���܂ł̗]�C�t���[��

#define TUTO_BG_POS_X	(970.0f)	// �`���[�g���A���̔w�i�̐�΍��W (x)
#define TUTO_BG_POS_Y	(125.0f)	// �`���[�g���A���̔w�i�̐�΍��W (y)
#define TUTO_BG_WIDTH	(300.0f)	// �`���[�g���A���̔w�i�̕� / 2 (��)
#define TUTO_BG_HEIGHT	(125.0f)	// �`���[�g���A���̔w�i�̕� / 2 (����)

#define TUTORIAL_POS_X	(990.0f)	// �`���[�g���A���̐�΍��W (x)
#define TUTORIAL_POS_Y	(120.0f)	// �`���[�g���A���̐�΍��W (y)
#define TUTORIAL_WIDTH	(300.0f)	// �`���[�g���A���̕� / 2 (��)
#define TUTORIAL_HEIGHT	(125.0f)	// �`���[�g���A���̕� / 2 (����)

#define TIPS_POS_X		(270.0f)	// �`���[�g���A���̔��l�̐�΍��W (x)
#define TIPS_POS_Y		(600.0f)	// �`���[�g���A���̔��l�̐�΍��W (y)
#define TIPS_WIDTH		(250.0f)	// �`���[�g���A���̔��l�̕� / 2 (��)
#define TIPS_HEIGHT		(100.0f)	// �`���[�g���A���̔��l�̕� / 2 (����)

#define RESET_POS_Z		(-2000.0f)	// �v���C���[�Đݒ莞�� z���W
#define LESSON_NUM_COM	(10)		// �R���{�̃��b�X���N���A�ɕK�v�ȃR���{��

#define TUTO_PAUSE_POS	(D3DXVECTOR3(85.0f, 255.0f, 0.0f))								// �|�[�Y�̐�΍��W
#define TUTO_PAUSE_SIZE	(D3DXVECTOR3(70.0f, 70.0f, 0.0f))								// �|�[�Y�̑傫��
#define TUTO_LET_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))	// �莆�̐�΍��W
#define TUTO_LET_SIZE	(D3DXVECTOR3(500.0f, 298.75f, 0.0f))							// �莆�̑傫��
#define TUTO_PAP_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 1020.0f, 0.0f))				// ��Ⳃ̏������W
#define TUTO_PAP_SIZE	(D3DXVECTOR3(500.0f, 298.75f, 0.0f))							// ��Ⳃ̑傫��
#define TUTO_CONT_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 665.0f, 0.0f))				// ����̏������W
#define TUTO_CONT_SIZE	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 50.0f, 0.0f))					// ����̑傫��

#define TUTO_LET_CHAN	(0.04f)		// �莆�̃��l�ύX��
#define TUTO_LET_STOP	(1.0f)		// �莆�̍ő僿�l
#define TUTO_FADE_CHAN	(0.02f)		// �t�F�[�h�̃��l�ύX��
#define TUTO_FADE_STOP	(0.6f)		// �t�F�[�h�̍ő僿�l

#define TUTO_PAP_MOVE	(0.5f)		// ��Ⳃ̈ʒu�̍X�V��
#define TUTO_PAP_STOP	(315.0f)	// ��Ⳃ̒�~�ʒu (y)

#define TUTO_CONT_CHAN			(0.008f)	// ��������̃��l�̕ύX��
#define TUTO_CONT_MAX_ALPHA		(1.0f)		// ��������̃��l�̍ő�l
#define TUTO_CONT_MIN_ALPHA		(0.2f)		// ��������̃��l�̍ŏ��l
#define TUTO_CONT_CHAN_RETURN	(0.06f)		// ��������̂��܂����̃��l�̕ύX��

#define TUTO_POLICE_POS	(D3DXVECTOR3(2000.0f, 0.0f, 3000.0f))	// �x�@�̃��b�X���̐ݒ�ʒu

//**********************************************************************************************************************
//	�񋓌^��` (TEXTURE_TUTORIAL)
//**********************************************************************************************************************
typedef enum
{
	TEXTURE_TUTORIAL_BG = 0,		// �w�i
	TEXTURE_TUTORIAL_LETTER,		// �莆
	TEXTURE_TUTORIAL_PAUSE,			// �|�[�Y�A�C�R��
	TEXTURE_TUTORIAL_CONTROL,		// ����\��
	TEXTURE_TUTORIAL_MAX,			// ���̗񋓌^�̑���
} TEXTURE_TUTORIAL;

//**********************************************************************************************************************
//	�񋓌^��` (LESSON_SETUP)
//**********************************************************************************************************************
typedef enum
{
	LESSON_SETUP_SLUMBOOST = 0,		// ���b�X��2 (�j�Ŏ���) �̓ǂݍ���
	LESSON_SETUP_FLYAWAY,			// ���b�X��3 (����U��) �̓ǂݍ���
	LESSON_SETUP_SILENCEWORLD,		// ���b�X��4 (�������E) �̓ǂݍ���
	LESSON_SETUP_COMBO,				// ���b�X��5 (�R���{)   �̓ǂݍ���
	LESSON_SETUP_MAX				// ���̗񋓌^�̑���
} LESSON_SETUP;

//**********************************************************************************************************************
//	�񋓌^��` (TUTOSTAGSTATE)
//**********************************************************************************************************************
typedef enum
{
	TUTOSTAGSTATE_NONE = 0,			// �������Ȃ����
	TUTOSTAGSTATE_LET_ALPHA,		// �莆�̕\�����
	TUTOSTAGSTATE_FADE_ALPHA,		// �t�F�[�h�̕\�����
	TUTOSTAGSTATE_PAP_TAKE,			// ��Ⳃ̎��o�����
	TUTOSTAGSTATE_WAIT,				// �ҋ@���
	TUTOSTAGSTATE_PAP_RETURN,		// ��Ⳃ̂��܂����
	TUTOSTAGSTATE_MAX				// ���̗񋓌^�̑���
} TUTOSTAGSTATE;

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const char *apTextureTutorial[] =		// �`���[�g���A���e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\ui005.tga",			// �`���[�g���A���w�i�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tutorial000.tga",	// �莆�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tutorial001.tga",	// �|�[�Y�A�C�R���̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tutorial002.tga",	// ����\���̃e�N�X�`�����΃p�X
};

const int aNextLesson[] =	// ���b�X���̃J�E���^�[
{
	240,	// ���b�X��0 (�ړ�)     �̃��b�X���J�E���^�[
	180,	// ���b�X��1 (����)     �̃��b�X���J�E���^�[
	30,		// ���b�X��2 (�j�Ŏ���) �̃��b�X���J�E���^�[
	60,		// ���b�X��3 (����U��) �̃��b�X���J�E���^�[
	120,	// ���b�X��4 (�������E) �̃��b�X���J�E���^�[
	120,	// ���b�X��5 (�R���{)   �̃��b�X���J�E���^�[
	80,		// ���b�X��6 (�x�@)     �̃��b�X���J�E���^�[
	0,		// ���b�X��7 (�E�o)     �̃��b�X���J�E���^�[
};

const char *apTextureLesson[] =		// ���b�X���e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\lesson000.png",	// ���b�X��0 (�ړ�)     �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson001.png",	// ���b�X��1 (����)     �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson002.png",	// ���b�X��2 (�j�Ŏ���) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson003.png",	// ���b�X��3 (����U��) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson004.png",	// ���b�X��4 (�������E) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson005.png",	// ���b�X��5 (�R���{)   �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson006.png",	// ���b�X��6 (�x�@)     �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\lesson007.png",	// ���b�X��7 (�E�o)     �̃e�N�X�`�����΃p�X
};

const char *apTextureTips[] =		// ���l�e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\tips000.png",	// ���b�X��0 (�ړ�)     �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips001.png",	// ���b�X��1 (����)     �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips002.png",	// ���b�X��2 (�j�Ŏ���) �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips003.png",	// ���b�X��3 (����U��) �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips004.png",	// ���b�X��4 (�������E) �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips005.png",	// ���b�X��5 (�R���{)   �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips006.png",	// ���b�X��6 (�x�@)     �̔��l�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\tips007.png",	// ���b�X��7 (�E�o)     �̔��l�̃e�N�X�`�����΃p�X
};

const char *apTexturePaper[] =		// ��ⳃe�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\paper000.png",	// ���b�X��0 (�ړ�)     �̕�Ⳃ̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\paper001.png",	// ���b�X��1 (����)     �̕�Ⳃ̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\paper002.png",	// ���b�X��2 (�j�Ŏ���) �̕�Ⳃ̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\paper003.png",	// ���b�X��3 (����U��) �̕�Ⳃ̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\paper004.png",	// ���b�X��4 (�������E) �̕�Ⳃ̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\paper005.png",	// ���b�X��5 (�R���{)   �̕�Ⳃ̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\paper006.png",	// ���b�X��6 (�x�@)     �̕�Ⳃ̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\paper007.png",	// ���b�X��7 (�E�o)     �̕�Ⳃ̃e�N�X�`�����΃p�X
};

//**********************************************************************************************************************
//	�\���̒�` (Tutorial)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3   pos;				// ��Ⳃ̈ʒu
	TUTOSTAGSTATE state;			// ���o�̏��
	float         fMove;			// ��Ⳃ̈ړ���
	float         fAlphaLetter;		// �莆�̃��l
	float         fAlphaFade;		// �t�F�[�h�̃��l
	float         fAlphaControl;	// ����̃��l
	float         fChangeControl;	// ����̃��l�̕ύX��
}Tutorial;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
void UpdateTutorialUi(void);			// �`���[�g���A����UI�̍X�V����
void DrawTutorialUi(bool bBefore);		// �`���[�g���A����UI�̕`�揈��

bool CheckNextSlumBoost(void);			// �j�Ŏ����̃��b�X���I���̊m�F����
bool CheckNextFlyAway(void);			// ����U���̃��b�X���I���̊m�F����
bool CheckNextSilenceWorld(void);		// �������E�̃��b�X���I���̊m�F����

void AllFalseSlumBoost(void);			// �j�Ŏ����̃��b�X���I����̍폜����
void AllFalseFlyAway(void);				// ����U���̃��b�X���I����̍폜����
void AllFalseSilenceWorld(void);		// �������E�̃��b�X���I����̍폜����

void ResetPlayer(void);					// �v���C���[�̍Đݒ菈��

void TxtSetLesson(LESSON_SETUP lesson);	// ���b�X���̃Z�b�g�A�b�v����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureTutorial[TEXTURE_TUTORIAL_MAX] = {};	// �`���[�g���A���e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9      g_apTextureLesson[LESSON_MAX] = {};				// ���b�X���e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9      g_apTextureTips[LESSON_MAX] = {};				// ���l�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9      g_apTexturePaper[LESSON_MAX] = {};				// ��ⳃe�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

Tutorial      g_tutorial;				// �`���[�g���A���̏��
TUTORIALSTATE g_tutorialState;			// �`���[�g���A���̏��
int           g_nLessonState;			// ���b�X���̏��
int           g_nCounterTutorialState;	// �`���[�g���A���̏�ԊǗ��J�E���^�[
int           g_nCounterLessonState;	// ���b�X���̏�ԊǗ��J�E���^�[
bool          g_bTutorialEnd;			// ���[�h�̑J�ڏ�
bool          g_bComboClear;			// �R���{�̃��b�X���̃N���A����

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

	// ���b�X���E���l�E��ⳃe�N�X�`���̓ǂݍ���
	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureLesson[nCntTutorial], &g_apTextureLesson[nCntTutorial]);
		D3DXCreateTextureFromFile(pDevice, apTextureTips[nCntTutorial], &g_apTextureTips[nCntTutorial]);
		D3DXCreateTextureFromFile(pDevice, apTexturePaper[nCntTutorial], &g_apTexturePaper[nCntTutorial]);
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
	g_tutorialState         = TUTORIALSTATE_NORMAL;			// �`���[�g���A���̏��
	g_nLessonState          = LESSON_00;					// ���b�X���̏��
	g_nCounterTutorialState = 0;							// �`���[�g���A���̏�ԊǗ��J�E���^�[
	g_nCounterLessonState   = 0;							// ���b�X���̏�ԊǗ��J�E���^�[
	g_bTutorialEnd          = false;						// ���[�h�̑J�ڏ�
	g_bComboClear           = false;						// �R���{�̃��b�X���̃N���A����

	// �`���[�g���A���̏���������
	g_tutorial.pos            = TUTO_PAP_POS;				// ��Ⳃ̈ʒu
	g_tutorial.state          = TUTOSTAGSTATE_LET_ALPHA;	// ���o�̏��
	g_tutorial.fMove          = 0.0f;						// ��Ⳃ̈ړ���
	g_tutorial.fAlphaLetter   = 0.0f;						// �莆�̃��l
	g_tutorial.fAlphaFade     = 0.0f;						// �t�F�[�h�̃��l
	g_tutorial.fAlphaControl  = 0.0f;						// ����̃��l
	g_tutorial.fChangeControl = TUTO_CONT_CHAN;				// ����̃��l�̕ύX��

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
	pVtx[8].pos  = D3DXVECTOR3(TIPS_POS_X - TIPS_WIDTH, TIPS_POS_Y - TIPS_HEIGHT, 0.0f);
	pVtx[9].pos  = D3DXVECTOR3(TIPS_POS_X + TIPS_WIDTH, TIPS_POS_Y - TIPS_HEIGHT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(TIPS_POS_X - TIPS_WIDTH, TIPS_POS_Y + TIPS_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(TIPS_POS_X + TIPS_WIDTH, TIPS_POS_Y + TIPS_HEIGHT, 0.0f);

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
	pVtx[12].pos = D3DXVECTOR3(TIPS_POS_X - TIPS_WIDTH, TIPS_POS_Y - TIPS_HEIGHT, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(TIPS_POS_X + TIPS_WIDTH, TIPS_POS_Y - TIPS_HEIGHT, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(TIPS_POS_X - TIPS_WIDTH, TIPS_POS_Y + TIPS_HEIGHT, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(TIPS_POS_X + TIPS_WIDTH, TIPS_POS_Y + TIPS_HEIGHT, 0.0f);

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

	//------------------------------------------------------------------------------------------------------------------
	//	�|�[�Y�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[16].pos = D3DXVECTOR3(TUTO_PAUSE_POS.x - TUTO_PAUSE_SIZE.x, TUTO_PAUSE_POS.y - TUTO_PAUSE_SIZE.y, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(TUTO_PAUSE_POS.x + TUTO_PAUSE_SIZE.x, TUTO_PAUSE_POS.y - TUTO_PAUSE_SIZE.y, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(TUTO_PAUSE_POS.x - TUTO_PAUSE_SIZE.x, TUTO_PAUSE_POS.y + TUTO_PAUSE_SIZE.y, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(TUTO_PAUSE_POS.x + TUTO_PAUSE_SIZE.x, TUTO_PAUSE_POS.y + TUTO_PAUSE_SIZE.y, 0.0f);

	// rhw �̐ݒ�
	pVtx[16].rhw = 1.0f;
	pVtx[17].rhw = 1.0f;
	pVtx[18].rhw = 1.0f;
	pVtx[19].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[16].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[17].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[18].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[19].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	�莆�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[20].pos = D3DXVECTOR3(TUTO_LET_POS.x - TUTO_LET_SIZE.x, TUTO_LET_POS.y - TUTO_LET_SIZE.y, 0.0f);
	pVtx[21].pos = D3DXVECTOR3(TUTO_LET_POS.x + TUTO_LET_SIZE.x, TUTO_LET_POS.y - TUTO_LET_SIZE.y, 0.0f);
	pVtx[22].pos = D3DXVECTOR3(TUTO_LET_POS.x - TUTO_LET_SIZE.x, TUTO_LET_POS.y + TUTO_LET_SIZE.y, 0.0f);
	pVtx[23].pos = D3DXVECTOR3(TUTO_LET_POS.x + TUTO_LET_SIZE.x, TUTO_LET_POS.y + TUTO_LET_SIZE.y, 0.0f);

	// rhw �̐ݒ�
	pVtx[20].rhw = 1.0f;
	pVtx[21].rhw = 1.0f;
	pVtx[22].rhw = 1.0f;
	pVtx[23].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[20].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
	pVtx[21].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
	pVtx[22].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
	pVtx[23].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[22].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[23].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	�t�F�[�h�̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[24].pos = D3DXVECTOR3(0.0f,         0.0f,          0.0f);
	pVtx[25].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f,          0.0f);
	pVtx[26].pos = D3DXVECTOR3(0.0f,         SCREEN_HEIGHT, 0.0f);
	pVtx[27].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw �̐ݒ�
	pVtx[24].rhw = 1.0f;
	pVtx[25].rhw = 1.0f;
	pVtx[26].rhw = 1.0f;
	pVtx[27].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[24].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
	pVtx[25].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
	pVtx[26].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
	pVtx[27].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);

	//------------------------------------------------------------------------------------------------------------------
	//	��Ⳃ̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[28].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
	pVtx[29].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
	pVtx[30].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);
	pVtx[31].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);

	// rhw �̐ݒ�
	pVtx[28].rhw = 1.0f;
	pVtx[29].rhw = 1.0f;
	pVtx[30].rhw = 1.0f;
	pVtx[31].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[28].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[29].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[30].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[31].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[28].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[29].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[30].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[31].tex = D3DXVECTOR2(1.0f, 1.0f);

	//------------------------------------------------------------------------------------------------------------------
	//	����̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_���W��ݒ�
	pVtx[32].pos = D3DXVECTOR3(TUTO_CONT_POS.x - TUTO_CONT_SIZE.x, TUTO_CONT_POS.y - TUTO_CONT_SIZE.y, 0.0f);
	pVtx[33].pos = D3DXVECTOR3(TUTO_CONT_POS.x + TUTO_CONT_SIZE.x, TUTO_CONT_POS.y - TUTO_CONT_SIZE.y, 0.0f);
	pVtx[34].pos = D3DXVECTOR3(TUTO_CONT_POS.x - TUTO_CONT_SIZE.x, TUTO_CONT_POS.y + TUTO_CONT_SIZE.y, 0.0f);
	pVtx[35].pos = D3DXVECTOR3(TUTO_CONT_POS.x + TUTO_CONT_SIZE.x, TUTO_CONT_POS.y + TUTO_CONT_SIZE.y, 0.0f);

	// rhw �̐ݒ�
	pVtx[32].rhw = 1.0f;
	pVtx[33].rhw = 1.0f;
	pVtx[34].rhw = 1.0f;
	pVtx[35].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[32].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
	pVtx[33].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
	pVtx[34].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
	pVtx[35].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[32].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[33].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[34].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[35].tex = D3DXVECTOR2(1.0f, 1.0f);

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

	// �����\���̏�����
	Init3DNotation();

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

	// �J�߂̏�����
	InitPraise();

	// �t�@�C�������[�h����S�̏���
	LoadFileChunk
	( // ����
		false,	// �Ԃ̃J�[�u
		false,	// �l�Ԃ̃J�[�u
		true,	// �X�e�[�W
		true,	// �����蔻��
		true,	// �e
		true,	// �I�u�W�F�N�g
		false,	// AI
		false	// �A�C�R��
	);

	//���C��BGM�̍Đ�
	if (GetSoundType(SOUND_TYPE_MAIN_BGM) == true)
	{
		// �T�E���h�̍Đ��i�`���[�g���A��BGM�j
		PlaySound(SOUND_LABEL_BGM_TUTORIAL_000);
	}
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

	// ���b�X���E���l�E��ⳃe�N�X�`���̔j��
	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureLesson[nCntTutorial] != NULL)
		{ // �ϐ� (g_apTextureLesson) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureLesson[nCntTutorial]->Release();
			g_apTextureLesson[nCntTutorial] = NULL;
		}

		if (g_apTextureTips[nCntTutorial] != NULL)
		{ // �ϐ� (g_apTextureTips) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureTips[nCntTutorial]->Release();
			g_apTextureTips[nCntTutorial] = NULL;
		}

		if (g_apTexturePaper[nCntTutorial] != NULL)
		{ // �ϐ� (g_apTexturePaper) ��NULL�ł͂Ȃ��ꍇ

			g_apTexturePaper[nCntTutorial]->Release();
			g_apTexturePaper[nCntTutorial] = NULL;
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

	// �����\���̏I��
	Uninit3DNotation();

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

	// �J�߂̏I��
	UninitPraise();

	// �T�E���h�̒�~
	StopSound();
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
	case LESSON_02:	// ���b�X��2 (�j�Ŏ���)

		if (CheckNextSlumBoost() == true)
		{ // ���̃��b�X���ɑJ�ډ\�ȏꍇ

			// ���b�X���̏�Ԃ̉��Z
			AddLessonState();
		}

		// �����𔲂���
		break;

	case LESSON_03:	// ���b�X��3 (����U��)

		if (CheckNextFlyAway() == true)
		{ // ���̃��b�X���ɑJ�ډ\�ȏꍇ

			// ���b�X���̏�Ԃ̉��Z
			AddLessonState();
		}

		// �����𔲂���
		break;

	case LESSON_04:	// ���b�X��4 (�������E)

		if (CheckNextSilenceWorld() == true)
		{ // ���̃��b�X���ɑJ�ډ\�ȏꍇ

			// ���b�X���̏�Ԃ̉��Z
			AddLessonState();
		}

		// �����𔲂���
		break;

	case LESSON_05:	// ���b�X��5 (�R���{)

		if (g_bComboClear == true)
		{ // �R���{�̃��b�X�����N���A����̏ꍇ

			// ���b�X���̏�Ԃ̉��Z
			AddLessonState();
		}
		else
		{ // �R���{�̃��b�X�����N���A����ł͂Ȃ��ꍇ

			if (GetCurrentCombo() >= LESSON_NUM_COM)
			{ // ���݂̃R���{�����l�ȏ�̏ꍇ

				// �R���{�̃��b�X�����N���A����ɂ���
				g_bComboClear = true;
			}
		}

		// �����𔲂���
		break;

	case LESSON_06:	// ���b�X��6 (�x�@)

		// ����

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
	case TUTORIALSTATE_SKIP:		// �X�L�b�v���

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
	if (g_tutorial.state == TUTOSTAGSTATE_NONE)
	{ // �������Ȃ���Ԃ̏ꍇ

		// ���C�g�̍X�V
		UpdateLight();

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

		// �����\���̍X�V
		Update3DNotation();

		// �x�@�̃`���[�g���A���X�V
		UpdateTutorialPolice();

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
	else
	{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

		// �J�����̍X�V
		UpdateCamera();
	}
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

	// �����\���̕`��
	Draw3DNotation();

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

	// �x�@�̃`���[�g���A���`��
	DrawTutorialPolice();

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
	DrawTutorialUi(false);

	// �{�[�i�X�̕`��
	DrawBonus();

	// 2D�G�t�F�N�g�̕`��
	Draw2DEffect();

	// 2D�p�[�e�B�N���̕`��
	Draw2DParticle();

	// �`���[�g���A����UI�̕`��
	DrawTutorialUi(true);
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

			case LESSON_02:	// ���b�X��2 (�j�Ŏ���)

				// ���b�X���̃Z�b�g�A�b�v
				TxtSetLesson(LESSON_SETUP_SLUMBOOST);

				// �����𔲂���
				break;

			case LESSON_03:	// ���b�X��3 (����U��)

				// �j�Ŏ����̃��b�X���I����̍폜
				AllFalseSlumBoost();

				// ���b�X���̃Z�b�g�A�b�v
				TxtSetLesson(LESSON_SETUP_FLYAWAY);

				// �����𔲂���
				break;

			case LESSON_04:	// ���b�X��4 (�������E)

				// ����U���̃��b�X���I����̍폜
				AllFalseFlyAway();

				// ���b�X���̃Z�b�g�A�b�v
				TxtSetLesson(LESSON_SETUP_SILENCEWORLD);

				// �����𔲂���
				break;

			case LESSON_05:	// ���b�X��5 (�R���{)

				// �������E�̃��b�X���I����̍폜
				AllFalseSilenceWorld();

				// ���b�X���̃Z�b�g�A�b�v
				TxtSetLesson(LESSON_SETUP_COMBO);

				// �����𔲂���
				break;

			case LESSON_06:	// ���b�X��6 (�x�@)

				// �j�Ŏ����̃��b�X���I����̍폜
				AllFalseSlumBoost();

				// ����U���̃��b�X���I����̍폜
				AllFalseFlyAway();

				// �������E�̃��b�X���I����̍폜
				AllFalseSilenceWorld();

				// �x�@�̃`���[�g���A���̐ݒ�
				SetTutorialPolice(TUTO_POLICE_POS);

				// �����𔲂���
				break;

			case LESSON_07:	// ���b�X��7 (�E�o)

				// �Q�[�g�̑S�J��
				AllOpenGate();

				// �����𔲂���
				break;
			}

			// �v���C���[�̍Đݒ�
			ResetPlayer();

			// �G�t�F�N�g�̍폜
			AllFalseEffect();

			// �p�[�e�B�N���̍폜
			AllFalseParticle();

			// ���̑S����
			WindAllClear();

			// ���o�̏�Ԃ��莆�̕\����ԂɕύX
			g_tutorial.state = TUTOSTAGSTATE_LET_ALPHA;

			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_SCORE_000);	// SE (����00)
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
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_tutorial.state)
	{ // ��Ԃ��Ƃ̏���
	case TUTOSTAGSTATE_NONE:		// �������Ȃ����

		if (GetKeyboardTrigger(DIK_P) == true
		||  GetJoyKeyTrigger(JOYKEY_START, 0) == true)
		{ // ��Ⳃ����o�����삪�s��ꂽ�ꍇ

			if (g_tutorialState == TUTORIALSTATE_NORMAL)
			{ // �`���[�g���A�����ʏ��Ԃ̏ꍇ

				// �莆�̕\����Ԃɂ���
				g_tutorial.state = TUTOSTAGSTATE_LET_ALPHA;

				// �T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_TUTORIAL_PAGE_000);		// SE (�`���[�g���A�����߂���SE_000)
			}
		}

		// �����𔲂���
		break;

	case TUTOSTAGSTATE_LET_ALPHA:	// �莆�̕\�����

		// �莆�̃��l�����Z
		g_tutorial.fAlphaLetter += TUTO_LET_CHAN;

		if (g_tutorial.fAlphaLetter >= TUTO_LET_STOP)
		{ // �莆�̃��l�����l�ȏ�̏ꍇ

			// �莆�̃��l��␳
			g_tutorial.fAlphaLetter = TUTO_LET_STOP;

			// �t�F�[�h�̕\����Ԃɂ���
			g_tutorial.state = TUTOSTAGSTATE_FADE_ALPHA;
		}

		// ���_�J���[�̐ݒ�
		pVtx[20].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
		pVtx[21].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
		pVtx[22].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
		pVtx[23].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);

		// �����𔲂���
		break;

	case TUTOSTAGSTATE_FADE_ALPHA:	// �t�F�[�h�̕\�����

		// �t�F�[�h�̃��l�����Z
		g_tutorial.fAlphaFade += TUTO_FADE_CHAN;
		
		if (g_tutorial.fAlphaFade >= TUTO_FADE_STOP)
		{ // �t�F�[�h�̃��l�����l�ȏ�̏ꍇ

			// �t�F�[�h�̃��l��␳
			g_tutorial.fAlphaFade = TUTO_FADE_STOP;

			// ��Ⳃ̎��o����Ԃɂ���
			g_tutorial.state = TUTOSTAGSTATE_PAP_TAKE;

			// ���ʉ��̍Đ�
			if (GetSoundType(SOUND_TYPE_SE) == true)
			{
				// �T�E���h�i�`���[�g���A���̃y�[�W���߂��鉹�j���Đ�
				PlaySound(SOUND_LABEL_SE_TUTORIAL_PAGE_000);
			}
		}

		// ���_�J���[�̐ݒ�
		pVtx[24].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
		pVtx[25].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
		pVtx[26].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
		pVtx[27].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);

		// �����𔲂���
		break;

	case TUTOSTAGSTATE_PAP_TAKE:	// ��Ⳃ̎��o�����

		// ��Ⳃ̈ʒu���Z�ʂ�ݒ�
		g_tutorial.fMove += TUTO_PAP_MOVE;

		// ��Ⳃ̈ʒu�����Z
		g_tutorial.pos.y -= g_tutorial.fMove;

		if (g_tutorial.pos.y <= TUTO_PAP_STOP)
		{ // ��Ⳃ̈ʒu�����l�ȉ��̏ꍇ

			// ��Ⳃ̈ʒu��␳
			g_tutorial.pos.y = TUTO_PAP_STOP;

			// ��Ⳃ̈ʒu���Z�ʂ�������
			g_tutorial.fMove = 0;

			// �ҋ@��Ԃɂ���
			g_tutorial.state = TUTOSTAGSTATE_WAIT;
		}

		// ���_���W��ݒ�
		pVtx[28].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
		pVtx[29].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
		pVtx[30].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);
		pVtx[31].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);

		// �����𔲂���
		break;

	case TUTOSTAGSTATE_WAIT:		// �ҋ@���

		if (GetKeyboardTrigger(DIK_SPACE) == true
		||  GetJoyKeyTrigger(JOYKEY_A, 0) == true)
		{ // ��Ⳃ����܂����삪�s��ꂽ�ꍇ

			// ��Ⳃ̂��܂���Ԃɂ���
			g_tutorial.state = TUTOSTAGSTATE_PAP_RETURN;

			// ���ʉ��̍Đ�
			if (GetSoundType(SOUND_TYPE_SE) == true)
			{
				// �T�E���h�i�`���[�g���A���̃y�[�W���߂��鉹�j���Đ�
				PlaySound(SOUND_LABEL_SE_TUTORIAL_PAGE_000);
			}
		}

		if (GetKeyboardTrigger(DIK_P) == true
		||  GetJoyKeyTrigger(JOYKEY_START, 0) == true)
		{ // �^�C�g���J�ڂ̑��삪�s��ꂽ�ꍇ

			if (g_bTutorialEnd == false)
			{ // �J�ڐݒ肪����Ă��Ȃ��ꍇ

				// �J�ڐݒ肪���ꂽ��Ԃɂ���
				g_bTutorialEnd = true;

				// ��Ⳃ̂��܂���Ԃɂ���
				g_tutorial.state = TUTOSTAGSTATE_PAP_RETURN;

				// ���ʉ��̍Đ�
				if (GetSoundType(SOUND_TYPE_SE) == true)
				{
					// �T�E���h�i�`���[�g���A���̃y�[�W���߂��鉹�j���Đ�
					PlaySound(SOUND_LABEL_SE_TUTORIAL_PAGE_000);
				}

				// �Q�[����ʂ̏�Ԑݒ�
				SetTutorialState(TUTORIALSTATE_SKIP, END_TUTO_TIME);	// �X�L�b�v���
			}
		}

		// ��������̃��l��ύX
		g_tutorial.fAlphaControl += g_tutorial.fChangeControl;

		if (g_tutorial.fAlphaControl < TUTO_CONT_MIN_ALPHA
		||  g_tutorial.fAlphaControl > TUTO_CONT_MAX_ALPHA)
		{ // �����x���͈͊O�̏ꍇ

			// �ϐ��̕����𔽓]
			g_tutorial.fChangeControl *= -1.0f;

			// �����x�̕␳
			g_tutorial.fAlphaControl = (g_tutorial.fAlphaControl < TUTO_CONT_MIN_ALPHA) ? TUTO_CONT_MIN_ALPHA : TUTO_CONT_MAX_ALPHA;
		}

		// ���_�J���[�̐ݒ�
		pVtx[32].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
		pVtx[33].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
		pVtx[34].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
		pVtx[35].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);

		// �����𔲂���
		break;

	case TUTOSTAGSTATE_PAP_RETURN:	// ��Ⳃ̂��܂����

		// �莆�̓�����
		if (g_tutorial.fAlphaLetter > 0.0f)
		{ // �莆�̃��l�����l���傫���ꍇ

			// �莆�̃��l�����Z
			g_tutorial.fAlphaLetter -= TUTO_LET_CHAN;

			if (g_tutorial.fAlphaLetter <= 0.0f)
			{ // �莆�̃��l�����l�ȉ��̏ꍇ

				// �莆�̃��l��␳
				g_tutorial.fAlphaLetter = 0.0f;
			}

			// ���_�J���[�̐ݒ�
			pVtx[20].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
			pVtx[21].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
			pVtx[22].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
			pVtx[23].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaLetter);
		}

		// �t�F�[�h�̓�����
		if (g_tutorial.fAlphaFade > 0.0f)
		{ // �t�F�[�h�̃��l�����l���傫���ꍇ

			// �t�F�[�h�̃��l�����Z
			g_tutorial.fAlphaFade -= TUTO_FADE_CHAN;

			if (g_tutorial.fAlphaFade <= 0.0f)
			{ // �t�F�[�h�̃��l�����l�ȉ��̏ꍇ

				// �t�F�[�h�̃��l��␳
				g_tutorial.fAlphaFade = 0.0f;
			}

			// ���_�J���[�̐ݒ�
			pVtx[24].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
			pVtx[25].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
			pVtx[26].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
			pVtx[27].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_tutorial.fAlphaFade);
		}

		// ��������̓�����
		if (g_tutorial.fAlphaControl > 0.0f)
		{ // ��������̃��l�����l���傫���ꍇ

			// ��������̃��l�����Z
			g_tutorial.fAlphaControl -= TUTO_CONT_CHAN_RETURN;

			if (g_tutorial.fAlphaControl <= 0.0f)
			{ // ��������̃��l�����l�ȉ��̏ꍇ

				// ��������̃��l��␳
				g_tutorial.fAlphaControl = 0.0f;

				// ����̃��l�̕ύX�ʂ�������
				g_tutorial.fChangeControl = TUTO_CONT_CHAN;
			}

			// ���_�J���[�̐ݒ�
			pVtx[32].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
			pVtx[33].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
			pVtx[34].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
			pVtx[35].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_tutorial.fAlphaControl);
		}

		// ��Ⳃ̈ړ�
		if (g_tutorial.pos.y < TUTO_PAP_POS.y)
		{ // ��Ⳃ̈ʒu�����l��菬�����ꍇ

			// ��Ⳃ̈ʒu���Z�ʂ�ݒ�
			g_tutorial.fMove += TUTO_PAP_MOVE;

			// ��Ⳃ̈ʒu�����Z
			g_tutorial.pos.y += g_tutorial.fMove;
		}
		else if (g_tutorial.pos.y >= TUTO_PAP_POS.y)
		{ // ��Ⳃ̈ʒu�����l�ȏ�̏ꍇ

			// ��Ⳃ̈ʒu��␳
			g_tutorial.pos.y = TUTO_PAP_POS.y;

			// ��Ⳃ̈ʒu���Z�ʂ�������
			g_tutorial.fMove = 0;

			if (g_tutorial.fAlphaLetter  <= 0.0f
			&&  g_tutorial.fAlphaFade    <= 0.0f
			&&  g_tutorial.fAlphaControl <= 0.0f)
			{ // �S�Ẵ��l��������؂��Ă���ꍇ

				// �������Ȃ���Ԃɂ���
				g_tutorial.state = TUTOSTAGSTATE_NONE;
			}
		}

		// ���_���W��ݒ�
		pVtx[28].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
		pVtx[29].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y - TUTO_PAP_SIZE.y, 0.0f);
		pVtx[30].pos = D3DXVECTOR3(g_tutorial.pos.x - TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);
		pVtx[31].pos = D3DXVECTOR3(g_tutorial.pos.x + TUTO_PAP_SIZE.x, g_tutorial.pos.y + TUTO_PAP_SIZE.y, 0.0f);

		// �����𔲂���
		break;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();
}

//======================================================================================================================
//	�`���[�g���A����UI�̕`�揈��
//======================================================================================================================
void DrawTutorialUi(bool bBefore)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (bBefore == false)
	{ // ��̕`��̏ꍇ

		//--------------------------------------------------------------------------------------------------------------
		//	���b�X���̔w�i�̕`��
		//--------------------------------------------------------------------------------------------------------------
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_BG]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//--------------------------------------------------------------------------------------------------------------
		//	���b�X���̕`��
		//--------------------------------------------------------------------------------------------------------------
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureLesson[g_nLessonState]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

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
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureTips[g_nLessonState]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);

		//--------------------------------------------------------------------------------------------------------------
		//	�|�[�Y�̕`��
		//--------------------------------------------------------------------------------------------------------------
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_PAUSE]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
	}
	else
	{ // �O�̕`��̏ꍇ

		if (g_tutorial.state != TUTOSTAGSTATE_NONE)
		{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

			//----------------------------------------------------------------------------------------------------------
			//	�莆�̕`��
			//----------------------------------------------------------------------------------------------------------
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_LETTER]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);

			//----------------------------------------------------------------------------------------------------------
			//	�t�F�[�h�̕`��
			//----------------------------------------------------------------------------------------------------------
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 24, 2);

			//----------------------------------------------------------------------------------------------------------
			//	��Ⳃ̕`��
			//----------------------------------------------------------------------------------------------------------
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTexturePaper[g_nLessonState]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 28, 2);

			//----------------------------------------------------------------------------------------------------------
			//	����̕`��
			//----------------------------------------------------------------------------------------------------------
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTutorial[TEXTURE_TUTORIAL_CONTROL]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 32, 2);
		}
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

			if (pObject->nCollisionType != COLLISIONTYPE_NONE)
			{ // �����蔻�肪����I�u�W�F�N�g�̏ꍇ

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
	bool bNext = true;			// ���̃��b�X���J�ڏ�

	// �|�C���^��錾
	Car *pCar = GetCarData();	// �Ԃ̏��

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++, pCar++)
	{ // �Ԃ̍ő�\�������J��Ԃ�

		if (pCar->bUse == true)
		{ // �Ԃ��g�p����Ă���ꍇ

			if (GetBarrierState(pCar) != BARRIERSTATE_SET)
			{ // �Ԃ̃o���A��Ԃ�������Ԃł͂Ȃ��ꍇ

				// ���̃��b�X���J�ڂ��ł����Ԃɂ���
				bNext = false;

				// �����𔲂���
				break;
			}
		}
	}

	// ���̃��b�X���J�ڏ󋵂�Ԃ�
	return bNext;
}

//======================================================================================================================
//	�j�Ŏ����̃��b�X���I����̍폜����
//======================================================================================================================
void AllFalseSlumBoost(void)
{
	// �|�C���^��錾
	Object *pObject = GetObjectData();	// �I�u�W�F�N�g�̏��

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
	{ // �I�u�W�F�N�g�̍ő�\�������J��Ԃ�

		if (pObject->nCollisionType != COLLISIONTYPE_NONE)
		{ // �����蔻�肪����I�u�W�F�N�g�̏ꍇ

			// �I�u�W�F�N�g���g�p���Ă��Ȃ���Ԃɂ���
			pObject->bUse = false;
		}
	}

	// �e�̍X�V
	UpdateShadow();		// �e�̍폜�p

	// �����\���̍X�V
	Update3DNotation();	// �����\���̍폜�p
}

//======================================================================================================================
//	����U���̃��b�X���I����̍폜����
//======================================================================================================================
void AllFalseFlyAway(void)
{
	// �|�C���^��錾
	Human *pHuman = GetHumanData();		// �l�Ԃ̏��

	for (int nCntHuman = 0; nCntHuman < MAX_HUMAN; nCntHuman++, pHuman++)
	{ // �l�Ԃ̍ő�\�������J��Ԃ�

		// �l�Ԃ��g�p���Ă��Ȃ���Ԃɂ���
		pHuman->bUse = false;
	}

	// �e�̍X�V
	UpdateShadow();		// �e�̍폜�p

	// �����\���̍X�V
	Update3DNotation();	// �����\���̍폜�p
}

//======================================================================================================================
//	�������E�̃��b�X���I����̍폜����
//======================================================================================================================
void AllFalseSilenceWorld(void)
{
	// �|�C���^��錾
	Car         *pCar     = GetCarData();			// �Ԃ̏��
	Barrier     *pBarrier = GetBarrierData();		// �o���A�̏��
	BarrierInfo *pBarInfo = GetBarrierInfoData();	// �o���A�̂܂Ƃ܂�̏��

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

	for (int nCntBarInfo = 0; nCntBarInfo < MAX_BARINFO; nCntBarInfo++, pBarInfo++)
	{ // �o���A�̂܂Ƃ܂�̍ő�\�������J��Ԃ�

		// �o���A�̂܂Ƃ܂���g�p���Ă��Ȃ���Ԃɂ���
		pBarInfo->bUse = false;
	}

	// �e�̍X�V
	UpdateShadow();		// �e�̍폜�p

	// �����\���̍X�V
	Update3DNotation();	// �����\���̍폜�p
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

	// �{�̏��̏�����
	pPlayer->move          = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	pPlayer->rot           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	pPlayer->moveRot       = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����ύX��
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
	pPlayer->bomb.state           = ATTACKSTATE_NONE;			// �U�����
	pPlayer->bomb.nCounterState   = BOMB_WAIT_CNT;				// �U���Ǘ��J�E���^�[
	pPlayer->bomb.nCounterControl = 0;							// ����Ǘ��J�E���^�[
	pPlayer->bomb.bShot           = false;						// ���ˑҋ@��

	// ���̑����@
	SetWindSound(false);

	// �e�̈ʒu�ݒ�
	SetPositionShadow(pPlayer->nShadowID, pPlayer->pos, pPlayer->rot, NONE_SCALE);
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
	int			nJudgeType;		// �P���̎�ނ̑���p
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
			case LESSON_SETUP_SLUMBOOST:	// ���b�X��2 (�j�Ŏ���) �̓ǂݍ���

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
								else if (strcmp(&aString[0], "JUDGE") == 0)
								{ // �ǂݍ��񂾕����� JUDGE �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%d", &nJudgeType);		// �P����Ԃ�ǂݍ���
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
							SetObject(pos, rot, scale, &aMat[0], nType, nBreakType, nShadowType, nCollisionType, stateRot, APPEARSTATE_NONE, nJudgeType);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_OBJECT") != 0);			// �ǂݍ��񂾕����� END_SETLESSON_OBJECT �ł͂Ȃ��ꍇ���[�v
				}

				// �����𔲂���
				break;

			case LESSON_SETUP_FLYAWAY:		// ���b�X��3 (����U��) �̓ǂݍ���

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
								else if (strcmp(&aString[0], "ROT") == 0)
								{ // �ǂݍ��񂾕����� ROT �̏ꍇ
									fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
									fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
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
							SetHuman(pos, D3DXToRadian(rot), nWalk, bRecur, type);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_HUMAN") != 0);			// �ǂݍ��񂾕����� END_SETLESSON_HUMAN �ł͂Ȃ��ꍇ���[�v
				}

				// �����𔲂���
				break;

			case LESSON_SETUP_SILENCEWORLD:	// ���b�X��4 (�������E) �̓ǂݍ���

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
							SetCar(pos, D3DXToRadian(rot), nWalk, type);
						}
					} while (strcmp(&aString[0], "END_SETLESSON_CAR") != 0);			// �ǂݍ��񂾕����� END_SETLESSON_CAR �ł͂Ȃ��ꍇ���[�v
				}

				// �����𔲂���
				break;

			case LESSON_SETUP_COMBO:		// ���b�X��5 (�R���{) �̓ǂݍ���

				if (strcmp(&aString[0], "SETLESSON_COMBO") == 0)
				{ // �ǂݍ��񂾕����� SETLESSON_COMBO �̏ꍇ

					do
					{ // �ǂݍ��񂾕����� END_SETLESSON_COMBO �ł͂Ȃ��ꍇ���[�v

						// �t�@�C�����當�����ǂݍ���
						fscanf(pFile, "%s", &aString[0]);

						//----------------------------------------------------------------------------------------------
						//	�I�u�W�F�N�g�̐ݒ�
						//----------------------------------------------------------------------------------------------
						if (strcmp(&aString[0], "SETCOMBO_OBJECT") == 0)
						{ // �ǂݍ��񂾕����� SETCOMBO_OBJECT �̏ꍇ

							do
							{ // �ǂݍ��񂾕����� END_SETCOMBO_OBJECT �ł͂Ȃ��ꍇ���[�v

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
										else if (strcmp(&aString[0], "JUDGE") == 0)
										{ // �ǂݍ��񂾕����� JUDGE �̏ꍇ
											fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
											fscanf(pFile, "%d", &nJudgeType);		// �P����Ԃ�ǂݍ���
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
									SetObject(pos, rot, scale, &aMat[0], nType, nBreakType, nShadowType, nCollisionType, stateRot, APPEARSTATE_NONE, nJudgeType);
								}
							} while (strcmp(&aString[0], "END_SETCOMBO_OBJECT") != 0);			// �ǂݍ��񂾕����� END_SETCOMBO_OBJECT �ł͂Ȃ��ꍇ���[�v
						}

						//----------------------------------------------------------------------------------------------
						//	�l�Ԃ̐ݒ�
						//----------------------------------------------------------------------------------------------
						else if (strcmp(&aString[0], "SETCOMBO_HUMAN") == 0)
						{ // �ǂݍ��񂾕����� SETCOMBO_HUMAN �̏ꍇ

							do
							{ // �ǂݍ��񂾕����� END_SETCOMBO_HUMAN �ł͂Ȃ��ꍇ���[�v

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
										else if (strcmp(&aString[0], "ROT") == 0)
										{ // �ǂݍ��񂾕����� ROT �̏ꍇ
											fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
											fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// ������ǂݍ���
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
									SetHuman(pos, D3DXToRadian(rot), nWalk, bRecur, type);
								}
							} while (strcmp(&aString[0], "END_SETCOMBO_HUMAN") != 0);			// �ǂݍ��񂾕����� END_SETCOMBO_HUMAN �ł͂Ȃ��ꍇ���[�v
						}

						//----------------------------------------------------------------------------------------------
						//	�Ԃ̐ݒ�
						//----------------------------------------------------------------------------------------------
						else if (strcmp(&aString[0], "SETCOMBO_CAR") == 0)
						{ // �ǂݍ��񂾕����� SETCOMBO_CAR �̏ꍇ

							do
							{ // �ǂݍ��񂾕����� END_SETCOMBO_CAR �ł͂Ȃ��ꍇ���[�v

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
									SetCar(pos, D3DXToRadian(rot), nWalk, type);
								}
							} while (strcmp(&aString[0], "END_SETCOMBO_CAR") != 0);				// �ǂݍ��񂾕����� END_SETCOMBO_CAR �ł͂Ȃ��ꍇ���[�v
						}
					} while (strcmp(&aString[0], "END_SETLESSON_COMBO") != 0);					// �ǂݍ��񂾕����� END_SETLESSON_COMBO �ł͂Ȃ��ꍇ���[�v
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