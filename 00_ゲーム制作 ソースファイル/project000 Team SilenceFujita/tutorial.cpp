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
#include "Police.h"
#include "score.h"
#include "tiremark.h"
#include "velocity.h"
#include "weather.h"
#include "wind.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define MAX_TUTORIAL		(1)			// �g�p����|���S����
#define END_TUTORIAL_TIME	(120)		// �`���[�g���A���I���܂ł̗]�C�t���[��

#define TUTORIAL_POS_X		(910.0f)	// �`���[�g���A���̐�΍��W (x)
#define TUTORIAL_POS_Y		(160.0f)	// �`���[�g���A���̐�΍��W (y)
#define TUTORIAL_WIDTH		(360.0f)	// �`���[�g���A���̕� / 2 (��)
#define TUTORIAL_HEIGHT		(160.0f)	// �`���[�g���A���̕� / 2 (����)

//**********************************************************************************************************************
//	�R���X�g��`
//**********************************************************************************************************************
const int aNextLesson[] =	// ���b�X���̃J�E���^�[
{
	0,		// ���b�X��0 (�ړ�)     �̃��b�X���J�E���^�[
	0,		// ���b�X��1 (����)     �̃��b�X���J�E���^�[
	0,		// ���b�X��2 (��~)     �̃��b�X���J�E���^�[
	0,		// ���b�X��3 (���_�ύX) �̃��b�X���J�E���^�[
	0,		// ���b�X��4 (�j�Ŏ���) �̃��b�X���J�E���^�[
	0,		// ���b�X��5 (����U��) �̃��b�X���J�E���^�[
	0,		// ���b�X��6 (�������E) �̃��b�X���J�E���^�[
	0,		// ���b�X��7 (�E�o)     �̃��b�X���J�E���^�[
};

const char *apTextureTutorial[] =		// �e�N�X�`���̑��΃p�X
{
	"data\\TEXTURE\\ui005.png",	// ���b�X��0 (�ړ�)     �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\ui005.png",	// ���b�X��1 (����)     �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\ui005.png",	// ���b�X��2 (��~)     �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\ui005.png",	// ���b�X��3 (���_�ύX) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\ui005.png",	// ���b�X��4 (�j�Ŏ���) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\ui005.png",	// ���b�X��5 (����U��) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\ui005.png",	// ���b�X��6 (�������E) �̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\ui005.png",	// ���b�X��7 (�E�o)     �̃e�N�X�`�����΃p�X
};

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureTutorial[LESSON_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				// ���_�o�b�t�@�ւ̃|�C���^

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

	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, apTextureTutorial[nCntTutorial], &g_apTextureTutorial[nCntTutorial]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_2D) * 4 * MAX_TUTORIAL,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// ���_�t�H�[�}�b�g
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

	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y - TUTORIAL_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_WIDTH, TUTORIAL_POS_Y + TUTORIAL_HEIGHT, 0.0f);

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
	// �e�N�X�`���̔j��
	for (int nCntTutorial = 0; nCntTutorial < LESSON_MAX; nCntTutorial++)
	{ // �g�p����e�N�X�`�������J��Ԃ�

		if (g_apTextureTutorial[nCntTutorial] != NULL)
		{ // �ϐ� (g_apTextureTutorial) ��NULL�ł͂Ȃ��ꍇ

			g_apTextureTutorial[nCntTutorial]->Release();
			g_apTextureTutorial[nCntTutorial] = NULL;
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
			SetTutorialState(TUTORIALSTATE_END, END_TUTORIAL_TIME);	// �I�����
		}
	}

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

		// ����

		// �����𔲂���
		break;

	case LESSON_05:	// ���b�X��5 (����U��)

		// ����

		// �����𔲂���
		break;

	case LESSON_06:	// ���b�X��6 (�������E)

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
	//	�g�p����\�[�X�t�@�C���̕`��
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

	// �X�R�A�̕`��
	DrawScore();

	// �{�[�i�X�̕`��
	DrawBonus();

	// 2D�G�t�F�N�g�̕`��
	Draw2DEffect();

	// 2D�p�[�e�B�N���̕`��
	Draw2DParticle();

	//------------------------------------------------------------------------------------------------------------------
	//	�`���[�g���A���̕`��
	//------------------------------------------------------------------------------------------------------------------
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureTutorial[g_nLessonState]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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

				// ����

				// �����𔲂���
				break;

			case LESSON_05:	// ���b�X��5 (����U��)

				// ����

				// �����𔲂���
				break;

			case LESSON_06:	// ���b�X��6 (�������E)

				// ����

				// �����𔲂���
				break;

			case LESSON_07:	// ���b�X��7 (�E�o)

				// ����

				// �����𔲂���
				break;
			}

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
TUTORIALSTATE GetLessonState(void)
{
	// ���b�X���̏�Ԃ�Ԃ�
	return (TUTORIALSTATE)g_nLessonState;

}