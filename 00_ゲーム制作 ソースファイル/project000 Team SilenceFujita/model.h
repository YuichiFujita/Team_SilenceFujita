//============================================================
//
//	���f���w�b�_�[ [model.h]
//	Author�F���c�E��
//
//============================================================
#ifndef _MODEL_H_					// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MODEL_H_					// ��d�C���N���[�h�h�~�̃}�N�����`����

//************************************************************
//	�񋓌^��` (MODELTYPE)
//************************************************************
typedef enum
{
	MODELTYPE_PLAYER_BODY = 0,		// ��
	MODELTYPE_PLAYER_HEAD,			// ��
	MODELTYPE_PLAYER_SLEEVE_L,		// ���r
	MODELTYPE_PLAYER_SLEEVE_R,		// �E�r
	MODELTYPE_PLAYER_WRIST_L,		// �����
	MODELTYPE_PLAYER_WRIST_R,		// �E���
	MODELTYPE_PLAYER_HAND_L,		// ����
	MODELTYPE_PLAYER_HAND_R,		// �E��
	MODELTYPE_PLAYER_PANTS_L,		// ����
	MODELTYPE_PLAYER_PANTS_R,		// �E��
	MODELTYPE_PLAYER_LEG_L,			// ����
	MODELTYPE_PLAYER_LEG_R,			// �E��
	MODELTYPE_PLAYER_SHOES_L,		// ���C
	MODELTYPE_PLAYER_SHOES_R,		// �E�C
	MODELTYPE_PLAYER_KNIFE,			// ���� (�i�C�t)
	MODELTYPE_PLAYER_GUN,			// ���� (�e)
	MODELTYPE_MAX,					// ���̗񋓌^�̑���
} MODELTYPE;

//************************************************************
//	�}�N����`
//************************************************************
#define FROM_PLAYER	(MODELTYPE_PLAYER_BODY)		// �v���C���[�̃��f���񋓂̊J�n�n�_

//************************************************************
//	�\���̒�` (Model)
//************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 *pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH         pMesh;		// ���b�V�� (���_���) �ւ̃|�C���^
	LPD3DXBUFFER       pBuffMat;	// �}�e���A���ւ̃|�C���^
	DWORD              dwNumMat;	// �}�e���A���̐�
	D3DXVECTOR3        vtxMin;		// �ŏ��̒��_���W
	D3DXVECTOR3        vtxMax;		// �ő�̒��_���W
	float              fHeight;		// �c��
	float              fRadius;		// ���a
}Model;

//************************************************************
//	�v���g�^�C�v�錾
//************************************************************
HRESULT InitModel(void);			// ���f���̏���������
void UninitModel(void);				// ���f���̏I������
Model *GetModelData(int nID);		// ���f�����̎擾����

#endif