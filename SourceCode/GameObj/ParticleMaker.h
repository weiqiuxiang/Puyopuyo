/*==========================================================================================
ヘッダファイル説明:
	パーティクルの発生器のクラスのヘッダファイル
==========================================================================================*/
#ifndef PARTICLE_H_
#define PARTICLE_H_

/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "Common.h"

/*==========================================================================================
	マクロ
==========================================================================================*/
#define  PARTICLE_MAX (512)

/*==========================================================================================
	構造体定義
==========================================================================================*/
//パーティクル構造体
typedef struct 
{
public:
	Vector2f	m_Pos;				//位置
	Vector2		m_Size;				//サイズ
	Vector2f	m_Speed;			//速度
	float		m_SpeedDecayRate;	//速度の減衰率
	int			m_Life;				//存在時間
	int			m_Alpha;			//アルファ
	int			m_AlphaDecayRate;	//アルファ減衰率
	bool		m_Use;				//使用フラグ
}PARTICLE;

/*==========================================================================================
	クラス定義
==========================================================================================*/
class ParticleMaker
{
public:
	ParticleMaker();
	~ParticleMaker();

	bool Init(void);
	void Update(void);
	void Draw(void);
	void Uninit(void);

	void MakeParticle(const Vector2f& Pos,const Vector2& Size, const Vector2f& Speed, float SpeedDecayRate, int Life, int Alpha);		//パーティクルを作る
	void PuyoHardDropParticle(const Vector2& Pos,const Vector2 Size,int SpeedY, float SpeedDecayRate);			//ぷよをセットする時のパーティクル

private:
	int			m_ParticleHandle;			//パーティクルハンドル
	int			m_ParticleAddHandle;		//パーティクルブレンド処理用画像のハンドル
	Vector2		m_Pos;						//座標
	PARTICLE	m_Particle[PARTICLE_MAX];	//パーティクル群
};

#endif