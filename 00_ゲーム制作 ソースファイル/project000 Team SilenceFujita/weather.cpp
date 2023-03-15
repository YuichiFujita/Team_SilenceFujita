//======================================================================================================================
//
//	�V�C���� [weather.cpp]
//	Author�F��������
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "weather.h"
#include "calculation.h"
#include "game.h"

#include "camera.h"
#include "player.h"
#include "flash.h"

//**********************************************************************************************************************
//	�}�N����`
//**********************************************************************************************************************
#define SNOW_TEXTURE	"data\\TEXTURE\\effect000.jpg"			// ��̃e�N�X�`��
#define THUNDER_FLASH_RADIUS	(100000.0f)						// ���̃t���b�V�������܂��̔��a
#define THUNDER_FLASH_ANGLE		(D3DXToRadian(90))				// ���̃t���b�V�������܂��̊p�x

#define RAIN_COL		(D3DXCOLOR(0.5f, 0.5f, 0.95f, 1.0f))	// �J�̒��_�J���[
#define SNOW_COL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// ��̒��_�J���[
#define THUNDER_COL		(D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f))		// ���̒��_�J���[

#define WEATHER_RAND	(50)		// �V�C�̃����_���͈̔�
#define SUNNY_RAND		(40)		// �����_���ł̐���͈̔�
#define RAIN_RAND		(44)		// �����_���ł̉J�͈̔�
#define SNOW_RAND		(48)		// �����_���ł̐�͈̔�
#define THUNDER_RAND	(49)		// �����_���ł̗��J�͈̔�

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
// �J
void InitRain(void);			// �J�̏���������
void UpdateRain(void);			// �J�̍X�V����
void DrawRain(void);			// �J�̕`�揈��

// ��
void InitSnow(void);			// ��̏���������
void UpdateSnow(void);			// ��̍X�V����
void DrawSnow(void);			// ��̕`�揈��

// ��
void InitThunder(void);			// ���̏���������
void UpdateThunder(void);		// ���̍X�V����

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9      g_apTextureWeather = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWeather  = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffThunder  = NULL;		// ���_�o�b�t�@�ւ̃|�C���^(��)

Rain g_aRain[MAX_RAIN];				// �J�̏��
Snow g_aSnow[MAX_SNOW];				// ��̏��
Thunder g_aThunder[MAX_THUNDER];	// ���̏��
WEATHERTYPE g_Weather;				// �V�C�̏��
int g_NumWeather;					// �~���Ă��鐔���擾����
int g_nThunderCount;				// ���̃J�E���g
MODE g_WeatherMode;					// ���[�h(�V�C��p)

//======================================================================================================================
//	�V�C�̏���������
//======================================================================================================================
void InitWeather(void)
{
	int nRandWeather;		// �V�C�̕ϐ�

	g_WeatherMode = GetMode();	// ���[�h
	RESULTSTATE resultState = GetResultState();	// �Q�[���̏��

	switch (g_WeatherMode)
	{
	case MODE_TITLE:

		// �V�C�𐰂�ɐݒ肷��
		g_Weather = WEATHERTYPE_SUNNY;

		break;			// �����o��

	case MODE_TUTORIAL:	// �`���[�g���A��

		// �V�C�𐰂�ɐݒ肷��
		g_Weather = WEATHERTYPE_SUNNY;

		break;			// �����o��

	case MODE_GAME:		// �Q�[��

#ifdef _DEBUG	// �f�o�b�O����
		// ����ɌŒ肷��
		nRandWeather = 0;
#else
		// �V��������_���ɐݒ�
		nRandWeather = rand() % WEATHER_RAND;
#endif
		if (nRandWeather <= SUNNY_RAND)
		{ // ����̏ꍇ
			// �V�C�𐰂�ɐݒ肷��
			g_Weather = WEATHERTYPE_SUNNY;
		}
		else if (nRandWeather <= RAIN_RAND)
		{ // �J�̏ꍇ
			// �V�C���J�ɐݒ肷��
			g_Weather = WEATHERTYPE_RAIN;
		}
		else if (nRandWeather <= SNOW_RAND)
		{ // ��̏ꍇ
			// �V�C���ɐݒ肷��
			g_Weather = WEATHERTYPE_SNOW;
		}
		else if (nRandWeather <= THUNDER_RAND)
		{ // ���J�̏ꍇ
			// �V�C�𗋉J�ɐݒ肷��
			g_Weather = WEATHERTYPE_THUNDER;
		}

		break;			// �����o��

	case MODE_RESULT:	// ���U���g

		if (resultState == RESULTSTATE_CLEAR)
		{ // �Q�[���N���A��Ԃ̏ꍇ
	
			// �V�C�𐰂�ɐݒ肷��
			g_Weather = WEATHERTYPE_SUNNY;
		}
		else if (resultState == RESULTSTATE_TIMEOVER
			 ||  resultState == RESULTSTATE_LIFEOVER)
		{ // �Q�[���I�[�o�[��Ԃ̏ꍇ
	
			// �V�C�𗋉J�ɐݒ肷��
			g_Weather = WEATHERTYPE_THUNDER;
		}

		break;

	case MODE_RANKING:	// �����L���O

		if (resultState == RESULTSTATE_CLEAR)
		{ // �Q�[���N���A��Ԃ̏ꍇ

			// �V�C�𐰂�ɐݒ肷��
			g_Weather = WEATHERTYPE_SUNNY;

		}
		else if (resultState == RESULTSTATE_TIMEOVER
			|| resultState == RESULTSTATE_LIFEOVER)
		{ // �Q�[���I�[�o�[��Ԃ̏ꍇ

		  // �V�C�𗋉J�ɐݒ肷��
			g_Weather = WEATHERTYPE_THUNDER;
		}
		break;			// �����o��
	}

	// ����������������
	g_NumWeather = 0;

	// ���̃J�E���g������������
	g_nThunderCount = 0;

	// �J�̏��̏�����
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{ // �J�̍ő�\�������J��Ԃ�

		g_aRain[nCntRain].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
		g_aRain[nCntRain].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
		g_aRain[nCntRain].fRadius = D3DXVECTOR2(0.0f, 0.0f);			// ���a
		g_aRain[nCntRain].bUse = false;									// �g�p��

	}

	// ��̏��̏�����
	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{ // ��̍ő�\�������J��Ԃ�

		g_aSnow[nCntSnow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
		g_aSnow[nCntSnow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
		g_aSnow[nCntSnow].fRadius = D3DXVECTOR2(0.0f, 0.0f);			// ���a
		g_aSnow[nCntSnow].bUse = false;									// �g�p��
	}

	// ���̏��̏�����
	for (int nCntThunder = 0; nCntThunder < MAX_THUNDER; nCntThunder++)
	{ // ���̍ő�\�����J��Ԃ�
		g_aThunder[nCntThunder].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		g_aThunder[nCntThunder].fRadius = D3DXVECTOR2(0.0f, 0.0f);		// ���a
		g_aThunder[nCntThunder].fShiftWidth = 0.0f;						// ���炷��
		g_aThunder[nCntThunder].nVariCount = 0;							// �J�E���g
		g_aThunder[nCntThunder].bUse = false;							// �g�p��
	}

	switch (g_Weather)
	{
	case WEATHERTYPE_SUNNY:		// ����

		break;					// �����o��

	case WEATHERTYPE_RAIN:		// �J

		// �J�̏���������
		InitRain();

		break;					// �����o��

	case WEATHERTYPE_SNOW:		// ��

		// ��̏���������
		InitSnow();

		break;					// �����o��

	case WEATHERTYPE_THUNDER:	// ���J

		// �J�̏���������
		InitRain();

		// ���̏���������
		InitThunder();

		break;					// �����o��
	}
}

//======================================================================================================================
// �J�̏���������
//======================================================================================================================
void InitRain(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, NULL, &g_apTextureWeather);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_RAIN,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWeather,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

	  // ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
// ��̏���������
//======================================================================================================================
void InitSnow(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, SNOW_TEXTURE, &g_apTextureWeather);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 4 * MAX_SNOW,		// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWeather,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_SNOW; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

	  // ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
// ���̏���������
//======================================================================================================================
void InitThunder(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^
	VERTEX_3D *pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	( // ����
		sizeof(VERTEX_3D) * 8 * MAX_THUNDER,	// �K�v���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,							// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffThunder,
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	���_���̏�����
	//------------------------------------------------------------------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffThunder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_THUNDER; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = THUNDER_COL;
		pVtx[1].col = THUNDER_COL;
		pVtx[2].col = THUNDER_COL;
		pVtx[3].col = THUNDER_COL;
		pVtx[4].col = THUNDER_COL;
		pVtx[5].col = THUNDER_COL;
		pVtx[6].col = THUNDER_COL;
		pVtx[7].col = THUNDER_COL;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^�̃|�C���^�� 8���i�߂�
		pVtx += 8;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffThunder->Unlock();
}

//======================================================================================================================
//	�V�C�̏I������
//======================================================================================================================
void UninitWeather(void)
{
	if (g_apTextureWeather != NULL)
	{ // �ϐ� (g_apTextureWeather) ��NULL�ł͂Ȃ��ꍇ

		g_apTextureWeather->Release();
		g_apTextureWeather = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffWeather != NULL)
	{ // �ϐ� (g_pVtxBuffWeather) ��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffWeather->Release();
		g_pVtxBuffWeather = NULL;
	}

	if (g_pVtxBuffThunder != NULL)
	{ //�ϐ�(g_pVtxBuffThunder)��NULL�ł͂Ȃ��ꍇ

		g_pVtxBuffThunder->Release();
		g_pVtxBuffThunder = NULL;
	}
}

//======================================================================================================================
//	�V�C�̍X�V����
//======================================================================================================================
void UpdateWeather(void)
{
	switch (g_Weather)
	{
	case WEATHERTYPE_SUNNY:		// ����

		break;					// �����o��

	case WEATHERTYPE_RAIN:		// �J

		// �J�̍X�V����
		UpdateRain();									

		break;					// �����o��

	case WEATHERTYPE_SNOW:		// ��

		// ��̍X�V����
		UpdateSnow();

		break;					// �����o��

	case WEATHERTYPE_THUNDER:	// ��

		// �J�̍X�V����
		UpdateRain();

		// ���̍X�V����
		UpdateThunder();

		break;					// �����o��
	}
}

//======================================================================================================================
// �J�̍X�V����
//======================================================================================================================
void UpdateRain(void)
{
	int nNumWeather = 0;	// �~���Ă��镨�̐�

	// �|�C���^��錾
	VERTEX_3D *pVtx;								// ���_���ւ̃|�C���^
	Player *pPlayer = GetPlayer();					// �v���C���[�̏��
	Camera *pCamera = GetCamera(CAMERATYPE_MAIN);	// �J�����̏��

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aRain[nCntWeather].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			switch (g_WeatherMode)
			{
			case MODE_GAME:		// �Q�[��

				// �v���C���[���̈ړ��ʂ𑫂�
				g_aRain[nCntWeather].pos.x += pPlayer->pos.x - pPlayer->oldPos.x;
				g_aRain[nCntWeather].pos.z += pPlayer->pos.z - pPlayer->oldPos.z;

				break;			// �����o��

			case MODE_RESULT:	// ���U���g

				// �v���C���[���̈ړ��ʂ𑫂�
				g_aRain[nCntWeather].pos.x += pCamera->posV.x - pCamera->posVOld.x;
				g_aRain[nCntWeather].pos.z += pCamera->posV.z - pCamera->posVOld.z;

				break;			// �����o��
			}

			// �~���Ă�����̂̑��������Z����
			nNumWeather++;

			// �ړ��̍X�V
			g_aRain[nCntWeather].pos -= g_aRain[nCntWeather].move;

			// ���a�̕␳
			if (g_aRain[nCntWeather].fRadius.x < 0.0f)
			{ // ���a�� 0.0f ��菬�����ꍇ

				// ���a�� 0.0f ����
				g_aRain[nCntWeather].fRadius.x = 0.0f;
			}

			// ���a�̕␳
			if (g_aRain[nCntWeather].fRadius.y < 0.0f)
			{ // ���a�� 0.0f ��菬�����ꍇ

				// ���a�� 0.0f ����
				g_aRain[nCntWeather].fRadius.y = 0.0f;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius.x, +g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius.x, +g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius.x, -g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius.x, -g_aRain[nCntWeather].fRadius.y, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = RAIN_COL;
			pVtx[1].col = RAIN_COL;
			pVtx[2].col = RAIN_COL;
			pVtx[3].col = RAIN_COL;

			switch (g_WeatherMode)
			{
			case MODE_GAME:		// �Q�[��

				if (g_aRain[nCntWeather].pos.y <= 0.0f)
				{ // �J���n�ʂɗ������Ƃ�
					// �g�p���Ȃ�
					g_aRain[nCntWeather].bUse = false;
				}

				break;			// �����o��

			case MODE_RESULT:	// ���U���g

				if (g_aRain[nCntWeather].pos.y <= RESULT_RAIN_LINE)
				{ // �J���n�ʂɗ������Ƃ�
					// �g�p���Ȃ�
					g_aRain[nCntWeather].bUse = false;
				}

				break;			// �����o��
			}
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWeather->Unlock();

	g_NumWeather = nNumWeather;	// ������������
}

//======================================================================================================================
// ��̍X�V����
//======================================================================================================================
void UpdateSnow(void)
{
	int nNumWeather = 0;	// �~���Ă��镨�̐�

	// �|�C���^��錾
	VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^
	Player *pPlayer = GetPlayer();		// �v���C���[�̏��

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_SNOW; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aSnow[nCntWeather].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			// �~���Ă�����̂̑��������Z����
			nNumWeather++;

			// �v���C���[���̈ړ��ʂ𑫂�
			g_aSnow[nCntWeather].pos.x += pPlayer->pos.x - pPlayer->oldPos.x;
			g_aSnow[nCntWeather].pos.z += pPlayer->pos.z - pPlayer->oldPos.z;

			// �ړ��̍X�V
			g_aSnow[nCntWeather].pos -= g_aSnow[nCntWeather].move;

			// ���a�̕␳
			if (g_aSnow[nCntWeather].fRadius.x < 0.0f)
			{ // ���a�� 0.0f ��菬�����ꍇ

				// ���a�� 0.0f ����
				g_aSnow[nCntWeather].fRadius.x = 0.0f;
			}

			// ���a�̕␳
			if (g_aSnow[nCntWeather].fRadius.y < 0.0f)
			{ // ���a�� 0.0f ��菬�����ꍇ

				// ���a�� 0.0f ����
				g_aSnow[nCntWeather].fRadius.y = 0.0f;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntWeather].fRadius.x, +g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aSnow[nCntWeather].fRadius.x, +g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntWeather].fRadius.x, -g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aSnow[nCntWeather].fRadius.x, -g_aSnow[nCntWeather].fRadius.y, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = SNOW_COL;
			pVtx[1].col = SNOW_COL;
			pVtx[2].col = SNOW_COL;
			pVtx[3].col = SNOW_COL;

			if (g_aSnow[nCntWeather].pos.y <= 0.0f)
			{ // �J���n�ʂɗ������Ƃ�
				// �g�p���Ȃ�
				g_aSnow[nCntWeather].bUse = false;
			}
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWeather->Unlock();

	g_NumWeather = nNumWeather;	// ������������
}

//======================================================================================================================
// ���̍X�V����
//======================================================================================================================
void UpdateThunder(void)
{
	int nNumWeather = 0;	// �~���Ă��镨�̐�

	// �|�C���^��錾
	VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffThunder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_THUNDER; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aThunder[nCntWeather].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			// �~���Ă�����̂̑��������Z����
			nNumWeather++;

			// �J�E���g�����Z����
			g_aThunder[nCntWeather].nVariCount++;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, +g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, +g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x + g_aThunder[nCntWeather].fShiftWidth, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x + g_aThunder[nCntWeather].fShiftWidth, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[4].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x - (g_aThunder[nCntWeather].fShiftWidth * 2), -(g_aThunder[nCntWeather].fRadius.y * 2), 0.0f);
			pVtx[5].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x - (g_aThunder[nCntWeather].fShiftWidth * 2), -(g_aThunder[nCntWeather].fRadius.y * 2), 0.0f);
			pVtx[6].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x + (g_aThunder[nCntWeather].fShiftWidth * 2), -(g_aThunder[nCntWeather].fRadius.y * 3), 0.0f);
			pVtx[7].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x + (g_aThunder[nCntWeather].fShiftWidth * 2), -(g_aThunder[nCntWeather].fRadius.y * 3), 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = THUNDER_COL;
			pVtx[1].col = THUNDER_COL;
			pVtx[2].col = THUNDER_COL;
			pVtx[3].col = THUNDER_COL;
			pVtx[4].col = THUNDER_COL;
			pVtx[5].col = THUNDER_COL;
			pVtx[6].col = THUNDER_COL;
			pVtx[7].col = THUNDER_COL;

			if (g_aThunder[nCntWeather].nVariCount >= THUNDER_COUNT)
			{ // ��莞�Ԍo�߂�����
				// �g�p���Ȃ�
				g_aThunder[nCntWeather].bUse = false;
			}
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 8;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffThunder->Unlock();

	g_NumWeather = nNumWeather;	// ������������
}

//======================================================================================================================
//	�V�C�̕`�揈��
//======================================================================================================================
void DrawWeather(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	switch (g_Weather)
	{
	case WEATHERTYPE_SUNNY:		// ����

		break;					// �����o��

	case WEATHERTYPE_RAIN:		// �J

		// �J�̕`�揈��
		DrawRain();

		break;					// �����o��

	case WEATHERTYPE_SNOW:		// ��

		// ��̕`�揈��
		DrawSnow();

		break;					// �����o��

	case WEATHERTYPE_THUNDER:	// ��

		// �J�̕`�揈��
		DrawRain();

		break;					// �����o��
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================================================================================================
// �J�̕`�揈��
//======================================================================================================================
void DrawRain(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;						// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aRain[nCntWeather].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

		  // ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aRain[nCntWeather].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aRain[nCntWeather].mtxWorld, NULL, &mtxView);	// �t�s������߂�
			g_aRain[nCntWeather].mtxWorld._41 = 0.0f;
			g_aRain[nCntWeather].mtxWorld._42 = 0.0f;
			g_aRain[nCntWeather].mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aRain[nCntWeather].pos.x, g_aRain[nCntWeather].pos.y, g_aRain[nCntWeather].pos.z);
			D3DXMatrixMultiply(&g_aRain[nCntWeather].mtxWorld, &g_aRain[nCntWeather].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aRain[nCntWeather].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWeather, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureWeather);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWeather * 4, 2);
		}
	}
}

//======================================================================================================================
// ��̕`�揈��
//======================================================================================================================
void DrawSnow(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;						// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntWeather = 0; nCntWeather < MAX_SNOW; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aSnow[nCntWeather].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

		  // ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aSnow[nCntWeather].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aSnow[nCntWeather].mtxWorld, NULL, &mtxView);	// �t�s������߂�
			g_aSnow[nCntWeather].mtxWorld._41 = 0.0f;
			g_aSnow[nCntWeather].mtxWorld._42 = 0.0f;
			g_aSnow[nCntWeather].mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aSnow[nCntWeather].pos.x, g_aSnow[nCntWeather].pos.y, g_aSnow[nCntWeather].pos.z);
			D3DXMatrixMultiply(&g_aSnow[nCntWeather].mtxWorld, &g_aSnow[nCntWeather].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aSnow[nCntWeather].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWeather, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureWeather);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWeather * 4, 2);
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//======================================================================================================================
// ���̕`�揈��
//======================================================================================================================
void DrawThunder(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;						// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntWeather = 0; nCntWeather < MAX_THUNDER; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aThunder[nCntWeather].bUse == true)
		{ // �G�t�F�N�g���g�p����Ă���ꍇ

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aThunder[nCntWeather].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aThunder[nCntWeather].mtxWorld, NULL, &mtxView);	// �t�s������߂�
			g_aThunder[nCntWeather].mtxWorld._41 = 0.0f;
			g_aThunder[nCntWeather].mtxWorld._42 = 0.0f;
			g_aThunder[nCntWeather].mtxWorld._43 = 0.0f;

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aThunder[nCntWeather].pos.x, g_aThunder[nCntWeather].pos.y, g_aThunder[nCntWeather].pos.z);
			D3DXMatrixMultiply(&g_aThunder[nCntWeather].mtxWorld, &g_aThunder[nCntWeather].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aThunder[nCntWeather].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffThunder, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWeather * 8, 6);
		}
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================================================================================================
//	�J�̐ݒ菈��
//======================================================================================================================
void SetRain(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 fRadius)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_RAIN; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aRain[nCntWeather].bUse == false)
		{ // �G�t�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aRain[nCntWeather].pos = pos;			// �ʒu
			g_aRain[nCntWeather].move = move;		// �ړ���
			g_aRain[nCntWeather].fRadius = fRadius;	// ���a

			// �g�p���Ă����Ԃɂ���
			g_aRain[nCntWeather].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius.x, +g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius.x, +g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aRain[nCntWeather].fRadius.x, -g_aRain[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aRain[nCntWeather].fRadius.x, -g_aRain[nCntWeather].fRadius.y, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = RAIN_COL;
			pVtx[1].col = RAIN_COL;
			pVtx[2].col = RAIN_COL;
			pVtx[3].col = RAIN_COL;

			// �����𔲂���
			break;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
// ��̐ݒ菈��
//======================================================================================================================
void SetSnow(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 fRadius)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWeather->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_SNOW; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aSnow[nCntWeather].bUse == false)
		{ // �G�t�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aSnow[nCntWeather].pos = pos;			// �ʒu
			g_aSnow[nCntWeather].move = move;		// �ړ���
			g_aSnow[nCntWeather].fRadius = fRadius;	// ���a

			// �g�p���Ă����Ԃɂ���
			g_aSnow[nCntWeather].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntWeather].fRadius.x, +g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aSnow[nCntWeather].fRadius.x, +g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntWeather].fRadius.x, -g_aSnow[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aSnow[nCntWeather].fRadius.x, -g_aSnow[nCntWeather].fRadius.y, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = SNOW_COL;
			pVtx[1].col = SNOW_COL;
			pVtx[2].col = SNOW_COL;
			pVtx[3].col = SNOW_COL;

			// �����𔲂���
			break;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWeather->Unlock();
}

//======================================================================================================================
// ���̐ݒ菈��
//======================================================================================================================
void SetThunder(D3DXVECTOR3 pos, D3DXVECTOR2 fRadius)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	Player *pPlayer = GetPlayer();		// �v���C���[�̏��

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffThunder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWeather = 0; nCntWeather < MAX_THUNDER; nCntWeather++)
	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�

		if (g_aThunder[nCntWeather].bUse == false)
		{ // �G�t�F�N�g���g�p����Ă��Ȃ��ꍇ

			// ��������
			g_aThunder[nCntWeather].pos = pos;			// �ʒu
			g_aThunder[nCntWeather].fRadius = fRadius;	// ���a
			g_aThunder[nCntWeather].nVariCount = 0;		// �J�E���g

			// ���炷���������_���ŕς���
			g_aThunder[nCntWeather].fShiftWidth = (float)(rand() % 300 + 100.0f);

			if (CollisionSector(pPlayer->pos, g_aThunder[nCntWeather].pos, pPlayer->rot.y, THUNDER_FLASH_RADIUS, THUNDER_FLASH_ANGLE) == true)
			{ // ���E���ɗ����������ꍇ

				// �t���b�V���̐ݒ菈��
				SetFlash(WEATHER_ALPHA,REV_WEATHER_ALPHA);
			}

			// �g�p���Ă����Ԃɂ���
			g_aThunder[nCntWeather].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, +g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, +g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[4].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[5].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[6].pos = D3DXVECTOR3(-g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);
			pVtx[7].pos = D3DXVECTOR3(+g_aThunder[nCntWeather].fRadius.x, -g_aThunder[nCntWeather].fRadius.y, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = THUNDER_COL;
			pVtx[1].col = THUNDER_COL;
			pVtx[2].col = THUNDER_COL;
			pVtx[3].col = THUNDER_COL;
			pVtx[4].col = THUNDER_COL;
			pVtx[5].col = THUNDER_COL;
			pVtx[6].col = THUNDER_COL;
			pVtx[7].col = THUNDER_COL;

			// �����𔲂���
			break;
		}

		// ���_�f�[�^�̃|�C���^�� 4���i�߂�
		pVtx += 8;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffThunder->Unlock();
}

//======================================================================================================================
// �V�C�̎擾����
//======================================================================================================================
WEATHERTYPE GetWeather(void)
{
	// �V�C�̏�Ԃ�Ԃ�
	return g_Weather;
}

//======================================================================================================================
//�J���~�点�鏈��
//======================================================================================================================
void WeatherRain(void)
{
	D3DXVECTOR3 Camerapos = GetCamera(CAMERATYPE_MAIN)->posV;	// �J�����̎��_
	D3DXVECTOR3 Camerarot = GetCamera(CAMERATYPE_MAIN)->rot;	// �J�����̌���
	D3DXVECTOR3 posRain;	// �J�̍~��ʒu
	float moveRain;			// �J�̈ړ���

	for (int nCnt = 0; nCnt < RAIN_GENERATE; nCnt++)
	{
		// �J�̈ʒu��ݒ肷��
		posRain.x = Camerapos.x + (sinf(Camerarot.y) * SHIFT_RAIN) + (rand() % RAIN_RANGE - (int)(RAIN_RANGE * 0.5f));
		posRain.y = Camerapos.y + RAIN_HEIGHT;
		posRain.z = Camerapos.z + (cosf(Camerarot.y) * SHIFT_RAIN) + (rand() % RAIN_RANGE - (int)(RAIN_RANGE * 0.5f));

		// ���x��ݒ肷��
		moveRain = (rand() % RAIN_MOVE_RANGE) + RAIN_MOVE_LEAST;

		// �J�̐ݒ菈��
		SetRain
		(
			posRain,									// �ʒu
			D3DXVECTOR3(0.0f, moveRain, 0.0f),			// �ړ���
			D3DXVECTOR2(RAIN_RADIUS_X,RAIN_RADIUS_Y)	// ���a
		);
	}
}

//======================================================================================================================
// ����~�点�鏈��
//======================================================================================================================
void WeatherSnow(void)
{
	D3DXVECTOR3 Camerapos = GetCamera(CAMERATYPE_MAIN)->posV;	// �J�����̎��_
	D3DXVECTOR3 Camerarot = GetCamera(CAMERATYPE_MAIN)->rot;	// �J�����̌���
	D3DXVECTOR3 posSnow;	// ��̍~��ʒu
	float moveSnow;			// ��̈ړ���

	for (int nCnt = 0; nCnt < SNOW_GENERATE; nCnt++)
	{
		// �J�̈ʒu��ݒ肷��
		posSnow.x = Camerapos.x + (sinf(Camerarot.y) * SHIFT_SNOW) + (rand() % SNOW_RANGE - (int)(SNOW_RANGE * 0.5f));
		posSnow.y = Camerapos.y + SNOW_HEIGHT;
		posSnow.z = Camerapos.z + (cosf(Camerarot.y) * SHIFT_SNOW) + (rand() % SNOW_RANGE - (int)(SNOW_RANGE * 0.5f));

		// ���x��ݒ肷��
		moveSnow = (float)(rand() % SNOW_MOVE_RANGE) + SNOW_MOVE_LEAST;

		// ��̐ݒ菈��
		SetSnow
		(
			posSnow,									// �ʒu
			D3DXVECTOR3(0.0f, moveSnow, 0.0f),			// �ړ���
			D3DXVECTOR2(SNOW_RADIUS_X, SNOW_RADIUS_Y)	// ���a
		);
	}
}

//======================================================================================================================
// �����~�点�鏈��
//======================================================================================================================
void WeatherThunder(void)
{
	D3DXVECTOR3 posThunder;						// ���̍~��ʒu
	float rotThunder;							// ���̍~���Ă������
	int nThunderGene;							// ���Y��

	switch (g_WeatherMode)
	{
	case MODE_GAME:			// �Q�[��

		// ���̊Ԋu��ݒ肷��
		nThunderGene = THUNDER_INTERVAL;

		break;				// �����o��

	case MODE_RESULT:		// ���U���g

		// ���̊Ԋu��ݒ肷��
		nThunderGene = THUNDER_RSL_INTERVAL;

		break;				// �����o��
	case MODE_RANKING:		// �����L���O

		// ���̊Ԋu��ݒ肷��
		nThunderGene = THUNDER_RSL_INTERVAL;

		break;				// �����o��
	}

	if (g_nThunderCount % nThunderGene == 0)
	{ // ���̍~��Ԋu���o�߂����ꍇ

		// ���̈ʒu��ݒ肷��
		posThunder.x = (float)(rand() % THUNDER_RANGE - (int)(THUNDER_RANGE * 0.5f));
		posThunder.y = THUNDER_HEIGHT;
		posThunder.z = (float)(rand() % THUNDER_RANGE - (int)(THUNDER_RANGE * 0.5f));

		// �p�x�����
		rotThunder = atan2f(posThunder.x - 0.0f, posThunder.z - 0.0f);

		posThunder.x += sinf(rotThunder) * SHIFT_THUNDER;
		posThunder.z += cosf(rotThunder) * SHIFT_THUNDER;

		// ���̐ݒ菈��
		SetThunder
		(
			posThunder,										// �ʒu
			D3DXVECTOR2(THUNDER_RADIUS_X, THUNDER_RADIUS_Y)	// ���a
		);
	}
}

//======================================================================================================================
// �V�C�̐ݒ菈��
//======================================================================================================================
void SetWeather(void)
{
	switch (g_Weather)
	{
	case WEATHERTYPE_SUNNY:		// ����

		break;					// �����o��

	case WEATHERTYPE_RAIN:		// �J

		// �J���~�点�鏈��
		WeatherRain();

		break;					// �����o��

	case WEATHERTYPE_SNOW:		// ��

		// ����~�点�鏈��
		WeatherSnow();

		break;					// �����o��

	case WEATHERTYPE_THUNDER:	// ��

		// �����~�点��Ԋu�����Z����
		g_nThunderCount++;

		// �J���~�点�鏈��
		WeatherRain();

		// �����~�点�鏈��
		WeatherThunder();		

		break;					// �����o��
	}
}

//======================================================================================================================
// �J�̑����擾����
//======================================================================================================================
int GetNumWeather(void)
{
	// �~���Ă��鑍����Ԃ�
	return g_NumWeather;
}


#ifdef _DEBUG	// �f�o�b�O����
//======================================================================================================================
//	�f�o�b�O�����ꗗ
//======================================================================================================================
////**********************************************************************************************************************
////	�G�t�F�N�g�̑����擾����
////**********************************************************************************************************************
//int GetNumWeather(void)
//{
//	// �ϐ���錾
//	int nNumWeather = 0;	// �G�t�F�N�g�̑����̊m�F�p
//
//	for (int nCntWeather = 0; nCntWeather < MAX_EFFECT; nCntWeather++)
//	{ // �G�t�F�N�g�̍ő�\�������J��Ԃ�
//
//		if (g_aWeather[nCntWeather].bUse == true)
//		{ // �G�t�F�N�g���g�p����Ă���ꍇ
//
//		  // �J�E���^�[�����Z
//			nNumWeather++;
//		}
//	}
//
//	// �ϐ��̒l��Ԃ�
//	return nNumWeather;	// �G�t�F�N�g�̑���
//}
#endif