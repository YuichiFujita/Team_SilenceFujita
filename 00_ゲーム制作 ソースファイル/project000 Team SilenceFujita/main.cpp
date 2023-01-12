//============================================================
//
//	���C������ [main.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "input.h"
#include "model.h"
#include "sound.h"
#include "fade.h"

#include "title.h"
#include "game.h"

#ifdef _DEBUG	// �f�o�b�O����
#include "camera.h"
#include "effect.h"
#include "particle.h"
#include "Editmain.h"
#include "Edit.h"
#include "EditBillboard.h"
#include "SoundDJ.h"
#endif

//************************************************************
//	�}�N����`
//************************************************************
#define CLASS_NAME		"WindowClass"		// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME		"None"				// �E�C���h�E�̖��O (�L���v�V�����ɕ\��)

#ifdef _DEBUG	// �f�o�b�O����
#define DEBUG_PRINT		(1280)				// �f�o�b�O�\���̕�����̍Œ�
#endif

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�C���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);							// ����������
void Uninit(void);		// �I������
void Update(void);		// �X�V����
void Draw(void);		// �`�揈��

#ifdef _DEBUG	// �f�o�b�O����
void InitDebug(void);	// �f�o�b�O�̏���������
void UninitDebug(void);	// �f�o�b�O�̏I������
void UpdateDebug(void);	// �f�o�b�O�̍X�V����
void DrawDebug(void);	// �f�o�b�O�̕`�揈��
#endif

//************************************************************
//	�O���[�o���ϐ�
//************************************************************
LPDIRECT3D9       g_pD3D = NULL;			// Direct3D �I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		// Direct3D �f�o�C�X�ւ̃|�C���^

MODE g_mode;					// ���[�h�؂�ւ��p
StageLimit g_stageLimit;		// �X�e�[�W�̈ړ��͈�

#ifdef _DEBUG	// �f�o�b�O����
LPD3DXFONT g_pFont;		// �t�H���g�ւ̃|�C���^
int  g_nCountFPS;		// FPS �J�E���^
bool g_bDispDebug;		// �f�o�b�O�\�L�� ON / OFF
#endif

//============================================================
//	���C���֐�
//============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	// �ϐ���錾
	WNDCLASSEX wcex =
	{ // �����l
		sizeof(WNDCLASSEX),					// WNDCLASSEX �̃������T�C�Y
		CS_CLASSDC,							// �E�C���h�E�̃X�^�C��
		WindowProc,							// �E�C���h�E�v���V�[�W��
		0,									// 0 �ɂ��� (�ʏ�͎g�p���Ȃ�)
		0,									// 0 �ɂ��� (�ʏ�͎g�p���Ȃ�)
		hInstance,							// �C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),		// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			// �N���C�A���g�̈�̔w�i�F
		NULL,								// ���j���[�o�[
		CLASS_NAME,							// �E�C���h�E�N���X�̖��O
		LoadIcon(NULL, IDI_APPLICATION)		// �t�@�C���̃A�C�R��
	};

	// ��ʃT�C�Y
	RECT rect = 
	{ // �����l
		0,					// �E�C���h�E�̍��� X���W
		0,					// �E�C���h�E�̍��� Y���W
		SCREEN_WIDTH,		// �E�C���h�E�̕�
		SCREEN_HEIGHT		// �E�C���h�E�̍���
	};

	// �E�C���h�E�p
	HWND hWnd;				// �E�C���h�E�n���h�� (���ʎq)
	MSG  msg;				// ���b�Z�[�W���i�[����ϐ�

	// �������x�p
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExecLastTime;	// �Ō�ɏ�����������

#ifdef _DEBUG	// �f�o�b�O����
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō�� FPS ���v����������
#endif

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�𐶐�
	hWnd = CreateWindowEx
	( // ����
		0,							// �g���E�C���h�E�X�^�C��
		CLASS_NAME,					// �E�C���h�E�N���X�̖��O
		WINDOW_NAME,				// �E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,				// �E�C���h�E�̍��� X ���W
		CW_USEDEFAULT,				// �E�C���h�E�̍��� Y ���W
		(rect.right - rect.left),	// �E�C���h�E�̕�
		(rect.bottom - rect.top),	// �E�C���h�E�̍���
		NULL,						// �e�E�C���h�E�̃n���h��
		NULL,						// ���j���[�n���h���܂��͎q�E�C���h�E ID
		hInstance,					// �C���X�^���X�n���h��
		NULL						// �E�C���h�E�쐬�f�[�^
	);

	// ����\��ݒ�
	timeBeginPeriod(1);

	//�����_���֐��̎���쐬
	srand((unsigned int)time(0));

	// �������x�p
	dwCurrentTime  = 0;
	dwExecLastTime = timeGetTime();	// ���ݎ������擾 (�ۑ�)

#ifdef _DEBUG	// �f�o�b�O����
	dwFrameCount  = 0;
	dwFPSLastTime = timeGetTime();	// ���ݎ������擾 (�ۑ�)

	g_pFont      = NULL;			// �t�H���g�ւ̃|�C���^��������
	g_nCountFPS  = 0;				// FPS �J�E���^��������
	g_bDispDebug = true;			// �f�o�b�O�\�L�����Ȃ���Ԃɂ���
#endif

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);		// �E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);				// �N���C�A���g�̈���X�V

	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{ // ���������������s�����ꍇ

		// �l��Ԃ�
		return -1;
	}

	// ���b�Z�[�W���[�v
	while (1)
	{ // �������[�v

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ // Windows �̏���

			if (msg.message == WM_QUIT)
			{ // WM_QUIT ���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���

				// �����𔲂���
				break;
			}
			else
			{ // WM_QUIT ���b�Z�[�W���󂯎���Ă��Ȃ��ꍇ

				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);		// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);		// �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{ // DirectX �̏���

			// ���ݎ������擾
			dwCurrentTime = timeGetTime();

#ifdef _DEBUG	// �f�o�b�O����
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5�b�o��

				// FPS ���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				// FPS �𑪒肵��������ۑ�
				dwFPSLastTime = dwCurrentTime;

				// �t���[���J�E���g���N���A
				dwFrameCount = 0;
			}
#endif

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{ // 60����1�b�o��

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				// �����J�n�̎��� [���ݎ���] ��ۑ�
				dwExecLastTime = dwCurrentTime;

#ifdef _DEBUG	// �f�o�b�O����
				// �t���[���J�E���g�����Z
				dwFrameCount++;
#endif
			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �l��Ԃ�
	return (int)msg.wParam;
}

//============================================================
//	�E�C���h�E�v���V�[�W��
//============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �ϐ���錾
	int nID;	// �Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_KEYDOWN:	// �L�[���͌��m����

		switch (wParam)
		{
		case VK_ESCAPE:		// [ESC] �L�[�������ꂽ�ꍇ

			// YES / NO �̃��b�Z�[�WBOX��\��
			nID = MessageBox(hWnd, "�{���ɏI�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{ // YES �������ꂽ�ꍇ

				// �E�C���h�E��j������ (WM_DESTROY ���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}

			// �������甲����
			break;
		}

		// �������甲����
		break;

	case WM_CLOSE:		// �E�C���h�E���I������m�F����

		// YES / NO �̃��b�Z�[�WBOX��\��
		nID = MessageBox(hWnd, "�{���ɏI�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{ // YES �������ꂽ�ꍇ

			// �E�C���h�E��j������ (WM_DESTROY ���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{ // NO �������ꂽ�ꍇ

			// �l��Ԃ�
			return 0;
		}

		// �������甲����
		break;

	case WM_DESTROY:	// �E�C���h�E���I�����鏈��

		// WM_QUIT ���b�Z�[�W�𑗂�
		PostQuitMessage(0);

		// �������甲����
		break;
	}

	// �l��Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//============================================================
//	����������
//============================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �ϐ���錾
	D3DDISPLAYMODE        d3ddm;	// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D �I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{ // �I�u�W�F�N�g�̐����Ɏ��s�����ꍇ

		// ���s�̒l��Ԃ�
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{ // �f�B�X�v���C���[�h�̎擾�Ɏ��s�����ꍇ

		// ���s�̒l��Ԃ�
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�f�o�C�X�̐ݒ�
	//--------------------------------------------------------
	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// �p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth            = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y (��)
	d3dpp.BackBufferHeight           = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y (����)
	d3dpp.BackBufferFormat           = d3ddm.Format;				// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount            = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;		// �_�u���o�b�t�@�̐؂�ւ� (�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil     = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ��� 16bit ���g��
	d3dpp.Windowed                   = bWindow;						// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	//--------------------------------------------------------
	//	Direct3D �f�o�C�X�̐���
	//--------------------------------------------------------
	// [��(���݂̐ݒ�) �`�揈���ƒ��_�������n�[�h�E�F�A�ōs��]
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{ // DirectX �� CPU �̃X�y�b�N���m�F���A���\���Ⴉ�����ꍇ

		// [�� (���݂̐ݒ�) �`�揈���̓n�[�h�E�F�A�A���_������ CPU ���s��]
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{ // DirectX �� �n�[�h�E�F�A �̃X�y�b�N���m�F���A���\���Ⴉ�����ꍇ

			// [�� (���݂̐ݒ�) �`�揈���ƒ��_������ CPU �ōs��]
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{ // DirectX �� CPU �ƃn�[�h�E�F�A�̃X�y�b�N���m�F���A���\���g�����ɂȂ�Ȃ������ꍇ

				// ���s�̒l��Ԃ�
				return E_FAIL;
			}
		}
	}

	//--------------------------------------------------------
	//	�X�e�[�g�̐ݒ�
	//--------------------------------------------------------
	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ� (�e�N�X�`���̊g�k��Ԃ̐ݒ�)
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ� (�e�N�X�`���̃A���t�@�u�����h�̐ݒ�)
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//--------------------------------------------------------
	//	�ϐ��̏�����
	//--------------------------------------------------------
	g_mode = MODE_TITLE;	// ���[�h���^�C�g���ɏ�����

	// �X�e�[�W�̈ړ��͈͂�������
	g_stageLimit.fNear  = 0.0f;		// �ړ��̐����ʒu (��O)
	g_stageLimit.fFar   = 0.0f;		// �ړ��̐����ʒu (��)
	g_stageLimit.fRight = 0.0f;		// �ړ��̐����ʒu (�E)
	g_stageLimit.fLeft  = 0.0f;		// �ړ��̐����ʒu (��)
	g_stageLimit.fField = 0.0f;		// �ړ��̐����ʒu (�n��)

	//--------------------------------------------------------
	//	����������
	//--------------------------------------------------------
	// ���̓f�o�C�X�̏�����
	if (FAILED(InitInput(hInstance, hWnd)))
	{ // ���̓f�o�C�X�̏������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���f���̏�����
	if (FAILED(InitModel()))
	{ // ���f���̏������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �T�E���h�̏�����
	InitSound(hWnd);

	// �t�F�[�h�̏�����
	InitFade(g_mode);

#ifdef _DEBUG	// �f�o�b�O����
	// �T�E���hDJ�̏�����
	InitSoundDJ(hWnd);

	// �f�o�b�O�̏�����
	InitDebug();
#endif

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void Uninit(void)
{
	// ���̓f�o�C�X�̏I��
	UninitInput();

	// ���f���̏I��
	UninitModel();

	// �T�E���h�̏I��
	UninitSound();

	// �t�F�[�h�̏I��
	UninitFade();

	// �^�C�g����ʂ̏I��
	UninitTitle();

	// �Q�[����ʂ̏I��
	UninitGame();

	// Direct3D �f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{ // Direct3D �f�o�C�X�� NULL �ł͂Ȃ��ꍇ

		// Direct3D �f�o�C�X��j������
		g_pD3DDevice->Release();

		// Direct3D �f�o�C�X�� NULL �ɂ���
		g_pD3DDevice = NULL;
	}

	// Direct3D �I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{ // Direct3D �I�u�W�F�N�g�� NULL �ł͂Ȃ��ꍇ

		// Direct3D �I�u�W�F�N�g��j������
		g_pD3D->Release();

		// Direct3D �I�u�W�F�N�g�� NULL �ɂ���
		g_pD3D = NULL;
	}

#ifdef _DEBUG	// �f�o�b�O����
	// �T�E���hDJ�̏I��
	UninitSoundDJ();

	// �f�o�b�O�̏I��
	UninitDebug();
#endif
}

//============================================================
//	�X�V����
//============================================================
void Update(void)
{
	// ���̓f�o�C�X�̍X�V
	UpdateInput();

	// ���ꂼ��̉�ʂ̍X�V
	switch (g_mode)
	{ // �I������
	case MODE_TITLE:	// �^�C�g�����

		// �^�C�g����ʂ̍X�V
		UpdateTitle();

		// �������甲����
		break;

	case MODE_GAME:		// �Q�[�����

		// �Q�[����ʂ̍X�V
		UpdateGame();

		// �������甲����
		break;
	}

	// �t�F�[�h�̍X�V
	UpdateFade();

#ifdef _DEBUG	// �f�o�b�O����
	// �f�o�b�O�̍X�V
	UpdateDebug();
#endif
}

//============================================================
//	�`�揈��
//============================================================
void Draw(void)
{
	// ��ʃN���A (�o�b�N�o�b�t�@�� Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear
	( // ����
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),				// ��ʃN���A���̐F
		1.0f,
		0
	);

	// �`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{ // �`��J�n�����������ꍇ

		switch (g_mode)
		{ // �I������
		case MODE_TITLE:	// �^�C�g�����

			// �^�C�g����ʂ̕`��
			DrawTitle();

			// �������甲����
			break;

		case MODE_GAME:		// �Q�[�����

			// �Q�[����ʂ̕`��
			DrawGame();

			// �������甲����
			break;
		}

		// �t�F�[�h�̕`��
		DrawFade();

#ifdef _DEBUG	// �f�o�b�O����
		// �f�o�b�O�̕`��
		DrawDebug();
#endif

		// �`��I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//============================================================
//	���[�h�̐ݒ�
//============================================================
void SetMode(MODE mode)
{
	//--------------------------------------------------------
	//	���݂̉�� (���[�h) �̏I������
	//--------------------------------------------------------
	switch (g_mode)
	{
	case MODE_TITLE:	// �^�C�g�����

		// �^�C�g����ʂ̏I��
		UninitTitle();

		// �������甲����
		break;

	case MODE_GAME:		// �Q�[�����

		// �Q�[����ʂ̏I��
		UninitGame();

		// �������甲����
		break;
	}

	//--------------------------------------------------------
	//	�V������� (���[�h) �̏���������
	//--------------------------------------------------------
	switch (mode)
	{
	case MODE_TITLE:	// �^�C�g�����

		// �^�C�g����ʂ̏�����
		InitTitle();

		// �������甲����
		break;

	case MODE_GAME:		// �Q�[�����

		// �Q�[����ʂ̏�����
		InitGame();

		// �������甲����
		break;
	}

	// ���[�h��ݒ肷��
	g_mode = mode;		// ���݂̃��[�h�ɐV�������[�h�̒l����
}

//============================================================
//	���[�h�̎擾����
//============================================================
MODE GetMode(void)
{
	// ���݂̃��[�h��Ԃ�
	return g_mode;
}

//============================================================
//	�X�e�[�W�̈ړ��͈͂̐ݒ菈��
//============================================================
void SetLimitStage(StageLimit limit)
{
	// �����̈ړ��͈͂���
	g_stageLimit.fNear  = limit.fNear;		// �ړ��̐����ʒu (��O)
	g_stageLimit.fFar   = limit.fFar;		// �ړ��̐����ʒu (��)
	g_stageLimit.fRight = limit.fRight;		// �ړ��̐����ʒu (�E)
	g_stageLimit.fLeft  = limit.fLeft;		// �ړ��̐����ʒu (��)
	g_stageLimit.fField = limit.fField;		// �ړ��̐����ʒu (�n��)
}

//============================================================
//	�X�e�[�W�̈ړ��͈͂̎擾����
//============================================================
StageLimit GetLimitStage(void)
{
	// �X�e�[�W�̈ړ��͈͂�Ԃ�
	return g_stageLimit;
}

//============================================================
//	�f�o�C�X�̎擾
//============================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	// �f�o�C�X�̃A�h���X��Ԃ�
	return g_pD3DDevice;
}

//============================================================
//	�X�e�[�W�̃Z�b�g�A�b�v����
//============================================================
void TxtSetStage(void)
{
	// �ϐ���錾
	StageLimit  stageLimit;		// �X�e�[�W�̈ړ��͈͂̑���p
	int         nEnd;			// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;				// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(STAGE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			//------------------------------------------------
			//	�ړ��͈͂̐ݒ�
			//------------------------------------------------
			if (strcmp(&aString[0], "STAGE_LIMITSET") == 0)
			{ // �ǂݍ��񂾕����� STAGE_LIMITSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STAGE_LIMITSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NEAR") == 0)
					{ // �ǂݍ��񂾕����� NEAR �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fNear);		// ��O�̐����ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "FAR") == 0)
					{ // �ǂݍ��񂾕����� FAR �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fFar);		// ���̐����ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "RIGHT") == 0)
					{ // �ǂݍ��񂾕����� RIGHT �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fRight);	// �E�̐����ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "LEFT") == 0)
					{ // �ǂݍ��񂾕����� LEFT �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fLeft);		// ���̐����ʒu��ǂݍ���
					}
					else if (strcmp(&aString[0], "FIELD") == 0)
					{ // �ǂݍ��񂾕����� FIELD �̏ꍇ
						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &stageLimit.fField);	// �n�ʂ̐����ʒu��ǂݍ���
					}
				} while (strcmp(&aString[0], "END_STAGE_LIMITSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_LIMITSET �ł͂Ȃ��ꍇ���[�v

				// �X�e�[�W�̈ړ��͈͂̐ݒ�
				SetLimitStage(stageLimit);
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�X�e�[�W�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

#ifdef _DEBUG	// �f�o�b�O����
//==============================================
//�G�f�B�b�g�I�u�W�F�N�g���[�h�̃f�o�b�O�\��
//==============================================
void DrawDebugEditObject(void)
{
	RECT rect = { 0,108,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[512];
	EditObject *edit = GetEditObject();				//�G�f�B�b�g�I�u�W�F�N�g�̏����擾����

	//������ɑ��
	sprintf(&aStr[0], "====================================================\n�G�f�B�b�g�I�u�W�F�N�g�̈ʒu[%.4f,%.4f,%.4f]"
		"\n�G�f�B�b�g�I�u�W�F�N�g�̌���[%.4f]\n�G�f�B�b�g�I�u�W�F�N�g�̎��:%d"
		"\n�}�e���A���̐F:[%.2f, %.2f, %.2f]\n�I�u�W�F�N�g�̊g�嗦[%.4f,%.4f,%.4f]"
		, edit->pos.x, edit->pos.y, edit->pos.z, edit->rot.y, edit->nType
		, edit->EditMaterial[edit->nType][edit->nCntMaterial].MatD3D.Diffuse.r, edit->EditMaterial[edit->nType][edit->nCntMaterial].MatD3D.Diffuse.g, edit->EditMaterial[edit->nType][edit->nCntMaterial].MatD3D.Diffuse.b
		, edit->scale.x, edit->scale.y, edit->scale.z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================
//�G�f�B�b�g�r���{�[�h���[�h�̃f�o�b�O�\��
//==============================================
void DrawDebugEditBillboard(void)
{
	RECT rect = { 0,108,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[512];
	EditBillboard *Editbillboard = GetEditBillboard();					//�G�f�B�b�g�r���{�[�h�̏����擾����

	//������ɑ��
	sprintf(&aStr[0], "====================================================\n�G�f�B�b�g�I�u�W�F�N�g�̈ʒu[%.4f,%.4f,%.4f]\n"
		"�G�f�B�b�g�I�u�W�F�N�g�̎��:%d\n�A�j���[�V����:%d�@(0=No,1=Yes)\n�A�j���[�V�����v���C:%d�@(0=Stop,1=Replay)"
		"\n�A�j���[�V�����p�^�[����:%d\n�A�j���[�V�����J�E���^�[:%d\n�F:[%.2f,%.2f,%.2f]\n�r���{�[�h�̊g�嗦:[%.4f,%.4f]"
		, Editbillboard->pos.x, Editbillboard->pos.y, Editbillboard->pos.z, Editbillboard->nType, Editbillboard->EditAnim.bAnim, Editbillboard->bAnimReplay, Editbillboard->EditAnim.nAnimPattern, Editbillboard->EditAnim.nAnimCounter, Editbillboard->col.r,
		Editbillboard->col.g, Editbillboard->col.b, Editbillboard->Radius.x, Editbillboard->Radius.y);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================
//�G�f�B�b�g�I�u�W�F�N�g�������
//==============================================
void DrawDebugControlObject(void)
{
	RECT rect = { 950,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[600];

	//������ɑ��
	sprintf(&aStr[0], "�I�u�W�F�N�g�̎�ޕύX:1\n�����̃��Z�b�g:2\n�g�嗦�̃��Z�b�g:3\n�S�̂̊g��:4\n�S�̂̏k��:5\n�ݒu����X�^�C���̕ύX:6\n"
		"�I�u�W�F�N�g�̍폜:9\n�I�u�W�F�N�g�̐ݒu:0\n"
		"�I�u�W�F�N�g�̈ړ�:W/A/S/D\n�I�u�W�F�N�g�̌����ύX:Q/E\n�I�u�W�F�N�g��X���̊g��k��:U/J\n�I�u�W�F�N�g��Y���̊g��k��:I/K\n�I�u�W�F�N�g��Z���̊g��k��:O/L\n"
		"�}�e���A����R�l�̕ύX:LSHIFT+R/F\n�}�e���A����G�l�̕ύX:LSHIFT+T/G\n�}�e���A����B�l�̕ύX:LSHIFT+Y/H\n"
		"�}�e���A���̃��Z�b�gR�l:LSHIFT+V\n�}�e���A���̃��Z�b�gG�l:LSHIFT+B\n�}�e���A���̃��Z�b�gB�l:LSHIFT+N\n"
		"�}�e���A���̕ύX:SPACE\n���[�h�؂�ւ�:F2\n�Z�[�u:F3");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================
//�G�f�B�b�g�r���{�[�h�������
//==============================================
void DrawDebugControlBillboard(void)
{
	RECT rect = { 950,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[700];

	//������ɑ��
	sprintf(&aStr[0], "�r���{�[�h�̎�ޕύX:1\n�g�嗦�̃��Z�b�g:3\n�S�̂̊g��:4\n�S�̂̏k��:5\n�ݒu����X�^�C���̕ύX:6\n"
		"�r���{�[�h�̃A�j���[�V������:7\n�r���{�[�h�̃A�j���[�V�����Đ�:8\n�I�u�W�F�N�g�̍폜:9\n�I�u�W�F�N�g�̐ݒu:0"
		"\n�I�u�W�F�N�g�̈ړ�:W/A/S/D\n�I�u�W�F�N�g��X���̊g��k��:U/J\n�I�u�W�F�N�g��Y���̊g��k��:I/K"
		"\n�F��R�l�̕ύX:LSHIFT+R/F\n�F��G�l�̕ύX:LSHIFT+T/G\n�F��B�l�̕ύX:LSHIFT+Y/H\n"
		"�F�̃��Z�b�gR�l:LSHIFT+V\n�F�̃��Z�b�gG�l:LSHIFT+B\n�F�̃��Z�b�gB�l:LSHIFT+N\n"
		"�r���{�[�h�̃A�j���[�V������\n�p�^�[�����̐ݒ�:LSHIFT+��/��\n�r���{�[�h�̃A�j���[�V������\n�Ԋu�̐ݒ�:LSHIFT+��/��"
		"\n���[�h�؂�ւ�:F2\n�Z�[�u:F3");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//============================================================
//	�f�o�b�O�̏���������
//============================================================
void InitDebug(void)
{
	// �f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont
	( // ����
		g_pD3DDevice,			// �f�o�C�X
		18,						// ����
		0,						// ��
		0,						// ����
		0,						// �~�j�}�b�v��
		FALSE,					// �Α�
		SHIFTJIS_CHARSET,		// �t�H���g�̕����Z�b�g
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&g_pFont				// �t�H���g�|�C���^
	);
}

//============================================================
//	�f�o�b�O�̏I������
//============================================================
void UninitDebug(void)
{
	// �f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{ // �t�H���g�̃|�C���^�� NULL �ł͂Ȃ��ꍇ

		// �t�H���g�̃|�C���^��j������
		g_pFont->Release();

		// �t�H���g�̃|�C���^�� NULL �ɂ���
		g_pFont = NULL;
	}
}

//============================================================
//	�f�o�b�O�̍X�V����
//============================================================
void UpdateDebug(void)
{
	if (GetKeyboardTrigger(DIK_F1) == true)
	{ // [F1] �������ꂽ�ꍇ

		// �ϐ� (g_bDispDebug) �^�U�������� (�O�����Z�q)		// ���������^�Ȃ� true �A�U�Ȃ� false ����
		g_bDispDebug = (g_bDispDebug == false) ? true : false;	// �� �������F() �̒�
	}
}

//============================================================
//	�f�o�b�O�̕`�揈��
//============================================================
void DrawDebug(void)
{
	// �ϐ���錾
	RECT rect =
	{ // �����l
		0,					// �E�C���h�E�̍��� X���W
		0,					// �E�C���h�E�̍��� Y���W
		SCREEN_WIDTH,		// �E�C���h�E�̕�
		SCREEN_HEIGHT		// �E�C���h�E�̍���
	};

	D3DXVECTOR3 cameraPosV   = GetCameraPosV();		// �J�����̎��_
	D3DXVECTOR3 cameraPosR   = GetCameraPosR();		// �J�����̒����_
	D3DXVECTOR3 cameraRot    = GetCameraRot();		// �J�����̌���
	float       cameraDis    = GetCameraDis();		// �J�����̋���
	int         nNumEffect   = GetNumEffect();		// �G�t�F�N�g�̑���
	int         nNumParticle = GetNumParticle();	// �p�[�e�B�N���̑���

	// �ϐ��z���錾
	char aDeb[DEBUG_PRINT];	// �f�o�b�O���̕\���p

	// ������ɑ��
	sprintf
	( // ����
		&aDeb[0],			// ������镶����
		"�@ \n"
		"�@ [F1] �f�o�b�O�\�� ON / OFF\n"
		"�@ [F2] �J�����̈ʒu���Z�b�g\n"
		"�@ [F3] ���x���̕ύX\n"
		"�@ ------------------------------------\n"
		"�@ [FPS] %d\n"
		"�@ [ �J�������_ ] %.1f, %.1f, %.1f\n"
		"�@ [�J���������_] %.1f, %.1f, %.1f\n"
		"�@ [ �J�������� ] %.1f, %.1f, %.1f\n"
		"�@ [ �J�������� ] %.1f\n"
		"�@ [�G�t�F�N�g��] %-4d�@ [�p�[�e�B�N����] %-3d\n",
		g_nCountFPS,		// FPS
		cameraPosV.x,		// �J�����̎��_�̈ʒu (x)
		cameraPosV.y,		// �J�����̎��_�̈ʒu (y)
		cameraPosV.z,		// �J�����̎��_�̈ʒu (z)
		cameraPosR.x,		// �J�����̒����_�̈ʒu (x)
		cameraPosR.y,		// �J�����̒����_�̈ʒu (y)
		cameraPosR.z,		// �J�����̒����_�̈ʒu (z)
		cameraRot.x,		// �J�����̌��� (x)
		cameraRot.y,		// �J�����̌��� (y)
		cameraRot.z,		// �J�����̌��� (z)
		cameraDis,			// �J�����̋���
		nNumEffect,			// �G�t�F�N�g�̑���
		nNumParticle		// �p�[�e�B�N���̑���
	);

	if (g_bDispDebug == true)
	{ // �f�o�b�O�̕\���� ON �̏ꍇ

		// �e�L�X�g�̕`��
		g_pFont->DrawText
		( // ����
			NULL,
			&aDeb[0],
			-1,
			&rect,
			DT_LEFT,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
		);
	}

	if (g_mode == MODE_GAME)
	{ // �Q�[�����[�h�̏ꍇ

		if (GetGameMode() == GAMEMODE_EDIT)
		{ // �G�f�B�b�g���[�h�������ꍇ

			if (GetStyle() == EDITSTYLE_OBJECT)
			{ // �I�u�W�F�N�g�X�^�C���������ꍇ

				// �I�u�W�F�N�g�̑������
				DrawDebugControlObject();

				// �G�f�B�b�g���[�h�̃f�o�b�O�\��
				DrawDebugEditObject();
			}
			else if (GetStyle() == EDITSTYLE_BILLBOARD)
			{ // �r���{�[�h�X�^�C���������ꍇ

				// �r���{�[�h�̑������
				DrawDebugControlBillboard();

				// �G�f�B�b�g���[�h�̃f�o�b�O�\��
				DrawDebugEditBillboard();
			}
		}
	}
}
#endif