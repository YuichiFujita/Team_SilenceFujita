//============================================================
//
//	���f���w�b�_�[ [model.h]
//	Author�F���c�E��
//
//============================================================
#ifndef _MODEL_H_	// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MODEL_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//************************************************************
//	�񋓌^��` (MODELTYPE)
//************************************************************
typedef enum
{
	// �I�u�W�F�N�g���f��
	MODELTYPE_OBJECT_TREE = 0,		// ��
	MODELTYPE_OBJECT_ROCK,			// ��
	MODELTYPE_OBJECT_FOREST,		// �X
	MODELTYPE_OBJECT_BIGTREE,		// ���
	MODELTYPE_OBJECT_YELLOWTREE,	// ���F����
	MODELTYPE_OBJECT_GRASS,			// ��
	//MODELTYPE_OBJECT_APART,		// �A�p�[�g
	MODELTYPE_OBJECT_HOUSE,			// ��
	MODELTYPE_OBJECT_CURRYSHOP,		// �J���[��
	MODELTYPE_OBJECT_BILL,			// �r��
	//MODELTYPE_OBJECT_POLICE,		// ���̌x�@

	// �v���C���[���f��
	MODELTYPE_PLAYER_CAR,			// ��
	MODELTYPE_MAX,					// �S���f���̑���
} MODELTYPE;

//************************************************************
//	�}�N����`
//************************************************************
#define INIT_VTX_MIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))		// ���f���̍ŏ��̒��_���W�̏����l
#define INIT_VTX_MAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))		// ���f���̍ő�̒��_���W�̏����l
#define FROM_OBJECT				(0)	// �I�u�W�F�N�g�̃��f���񋓂̊J�n�n�_
#define MODELTYPE_OBJECT_MAX	(9)	// �I�u�W�F�N�g�̃��f���̑���
#define FROM_PLAYER				(9)	// �v���C���[�̃��f���񋓂̊J�n�n�_
#define MODELTYPE_PLAYER_MAX	(1)	// �v���C���[�̃��f���̑���

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
Model GetModelData(int nID);		// ���f�����̎擾����

#endif