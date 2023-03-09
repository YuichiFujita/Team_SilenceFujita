//======================================================================================================================
//
//	�T�E���h���� [sound.cpp]
//	Author�F���c�E��
//
//======================================================================================================================
//**********************************************************************************************************************
//	�C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "sound.h"

//**********************************************************************************************************************
//	�}�N����` (�ǉ��œ��ꂽ���)
//**********************************************************************************************************************
#define SOUND_SWITCH_RELEASE	(SOUND_SWITCH_ON)		//�����[�X�Ŏg�p���鉹
#define SOUND_SWITCH_DEBUG		(SOUND_SWITCH_ON)		//�f�o�b�N�Ŏg�p���鉹

//**********************************************************************************************************************
//	�\���̒�` (SOUNDINFO)
//**********************************************************************************************************************
typedef struct
{
	char *pFilename;	// �t�@�C����
	int   nCntLoop;		// ���[�v�J�E���g
} SOUNDINFO;

//**********************************************************************************************************************
//	�v���g�^�C�v�錾
//**********************************************************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//**********************************************************************************************************************
//	�O���[�o���ϐ�
//**********************************************************************************************************************
IXAudio2 *g_pXAudio2 = NULL;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;				// �}�X�^�[�{�C�X
IXAudio2SourceVoice    *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};						// �I�[�f�B�I�f�[�^
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};						// �I�[�f�B�I�f�[�^�T�C�Y

// �T�E���h�̏��
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{ "data/BGM/bgm000.wav", -1 },			// BGM
	{ "data/BGM/game_bgm000.wav", -1 },		// �Q�[����BGM_000
	{ "data/BGM/game_bgm001.wav", -1 },		// �Q�[����BGM_001
	{ "data/BGM/title_bgm000.wav", -1 },	// �^�C�g����BGM_000
	{ "data/BGM/result_bgm000.wav", -1 },	// ���U���g��BGM_000
	{ "data/BGM/tutorial_bgm000.wav", -1 },	// �`���[�g���A����BGM_000
	{ "data/BGM/firecar000.wav", -1 },		// ���h�Ԃ̌��ʉ�BGM_000
	{ "data/BGM/yakiimo000.wav", -1 },		// �Ă������̌��ʉ�BGM_000
	{ "data/BGM/bousoucar000.wav", -1 },	// �\���Ԃ̌��ʉ�BGM_000
	{ "data/BGM/senkyocar000.wav", -1 },	// �I���J�[�̌��ʉ�BGM_000
	{ "data/BGM/police000.wav", -1 },		// �x�@�̌��ʉ�BGM_000
	{ "data/BGM/wind000.wav", -1 },			// �v���C���[�\�́i�����@�j�̌��ʉ�BGM_000
	{ "data/BGM/car000.wav", -1 },			// �ԑ��s����BGM_000
	{ "data/SE/move000.wav", 0 },			// SE
	{ "data/SE/select000.wav", 0 },			// �I����SE_000
	{ "data/SE/decide000.wav", 0 },			// �����SE_000
	{ "data/SE/startcount000.wav", 0 },		// �X�^�[�g�̃J�E���g�_�E���i�s�b�I�j��SE_000
	{ "data/SE/startgo000.wav", 0 },		// �J�n���̃X�^�[�g�i�s�[�I�j��SE_000
	{ "data/SE/startslide000.wav", 0 },		// �X�^�[�g�̃X���C�h�i�V���b�I�j��SE_000
	{ "data/SE/startlanding000.wav", 0 },	// �X�^�[�g�V�O�i���̒��n�̉��i�Y�h���j��SE_000
	{ "data/SE/boost000.wav", 0 },			// �v���C���[�\�́i�u�[�X�g�j��SE_000
	{ "data/SE/bomb000.wav", 0 },			// �v���C���[�\�́i�{���j��SE_000
	{ "data/SE/break000.wav", 0 },			// �j�󉹂�SE_000
	{ "data/SE/damage000.wav", 0 },			// �v���C���[�_���[�W��SE_000
	{ "data/SE/gareki000.wav", 0 },			// �����i�����n�j��SE_000
	{ "data/SE/gareki001.wav", 0 },			// �����i�����n�j��SE_001
	{ "data/SE/score000.wav", 0 },			// �X�R�A�l����SE_000
	
};

//**********************************************************************************************************************
//	�O���[�o���ϐ��i�ǉ��œ��ꂽ��j
//**********************************************************************************************************************
SOUND_SWITCH g_soundSwitch;				//�T�E���h�̗L���̐ݒ�
float g_aSoundVolume[SOUND_LABEL_MAX];	//�T�E���h�{�����[���̒l

//======================================================================================================================
//	�T�E���h�̏���������
//======================================================================================================================
HRESULT InitSound(HWND hWnd)
{
	////�ǉ��œ��ꂽ�ϐ��̏�����
	g_soundSwitch = SOUND_SWITCH_RELEASE;

	for (int nCut = 0; nCut < SOUND_LABEL_MAX; nCut++)
	{
		g_aSoundVolume[nCut] = 0.0f;
	}

#ifdef _DEBUG	// �f�o�b�O����

	//�f�o�b�N�p�̃T�E���h�ݒ�
	g_soundSwitch = SOUND_SWITCH_DEBUG;

#endif


	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//======================================================================================================================
//	�T�E���h�̏I������
//======================================================================================================================
void UninitSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2 != NULL)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//======================================================================================================================
//	�Z�O�����g�Đ� (�Đ����Ȃ��~)
//======================================================================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aSoundInfo[label].nCntLoop;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	g_apSourceVoice[label]->Start(0);

	//// �ǉ��œ��ꂽ���
	// label�̃T�E���h��ݒ�
	g_aSoundVolume[label] = 1.0f;

	return S_OK;
}

//======================================================================================================================
//	�Z�O�����g��~ (���x���w��)
//======================================================================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();

		//// �ǉ��œ��ꂽ���
		// label�̃T�E���h��ݒ�
		g_aSoundVolume[label] = 0.0f;
	}
}

//======================================================================================================================
//	�Z�O�����g��~ (�S��)
//======================================================================================================================
void StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);

			//// �ǉ��œ��ꂽ���
			// label�̃T�E���h��ݒ�
			g_aSoundVolume[nCntSound] = 0.0f;
		}
	}
}

//======================================================================================================================
//	���ʒ��� (���x���w��)
//======================================================================================================================
void SetSoundVolume(SOUND_LABEL label,float fVolume)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		g_apSourceVoice[label]->SetVolume(fVolume);

		// label�̃T�E���h��ݒ�
		g_aSoundVolume[label] = fVolume;
	}
}

//======================================================================================================================
//	���ʂ̎擾 (���x���w��)
//======================================================================================================================
float GetSoundVolume(SOUND_LABEL label)
{
	return g_aSoundVolume[label];
}

//======================================================================================================================
//	�`�����N�̃`�F�b�N
//======================================================================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//======================================================================================================================
//	�`�����N�f�[�^�̓ǂݍ���
//======================================================================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//======================================================================================================================
//	�T�E���h�̎擾
//======================================================================================================================
bool GetSoundType(SOUND_TYPE SoundType)
{
	bool bSound = false;

	switch (SoundType)
	{
		//���C����BGM
	case SOUND_TYPE_MAIN_BGM:
		if (g_soundSwitch == SOUND_SWITCH_ON)
		{//�T�E���h�̐ݒ肪BGM���I���̂Ƃ�
			
			//�Đ�
			bSound = true;

		}
		break;

		//���ʉ��n��BGM
	case SOUND_TYPE_SUB_BGM:
		if (g_soundSwitch == SOUND_SWITCH_ON ||
			g_soundSwitch == SOUND_SWITCH_NO_BGM)
		{//�T�E���h�̐ݒ肪���ʉ��nBGM���I���̂Ƃ�

			//�Đ�
			bSound = true;

		}
		break;

	case SOUND_TYPE_SE:
		if (g_soundSwitch == SOUND_SWITCH_ON ||
			g_soundSwitch == SOUND_SWITCH_NO_BGM)
		{//�T�E���h�̐ݒ肪SE���I���̂Ƃ�

			//�Đ�
			bSound = true;

		}
		break;
	}
	 
	return bSound;
}