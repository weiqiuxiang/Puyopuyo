/*==========================================================================================
�w�b�_�t�@�C������:
	�Q�[�����f�[�^�̂������i��N���X�̃w�b�_
==========================================================================================*/
#ifndef DATA_EXCHANGE_H_
#define DATA_EXCHANGE_H_

/*==========================================================================================
	����錾
==========================================================================================*/
class Puyo;
class Stage;

/*==========================================================================================
	�N���X��`
==========================================================================================*/
class DataExchange 
{
public:
	static int WriteDataInPuyo(Puyo* pOut, const Puyo* pIn);				//pIn�Ղ�̃f�[�^��pOut�Ղ�ɏ�������
	static int WritePuyoDataInStage(Stage* pStage, const Puyo* pPuyo);		//�Ղ�̃f�[�^���X�e�[�W�ɏ�������
};

#endif