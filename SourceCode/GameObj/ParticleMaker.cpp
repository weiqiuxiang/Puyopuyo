/*==========================================================================================
	インクルードヘッダ
==========================================================================================*/
#include "ParticleMaker.h"
#include "DxLib.h"
#include "DrawMethod.h"
#include <math.h>

/*==========================================================================================
	マクロ
==========================================================================================*/
#define PI (3.14159265359)
#define PARTICLE_TEX_PASS "data/texture/Particle.png"
#define PARTICLE_EF_TEX_PASS "data/texture/ParticleEffect.png"

/*==========================================================================================
	コンストラクタ
==========================================================================================*/
ParticleMaker::ParticleMaker()
{
	//パーティクル群初期化
	for (int i = 0; i < PARTICLE_MAX; i++) 
	{
		m_Particle[i].m_Use = false;
	}

	//ハンドル初期化
	m_ParticleHandle = -1;
	m_ParticleAddHandle = -1;
}

/*==========================================================================================
	デストラクタ
==========================================================================================*/
ParticleMaker::~ParticleMaker()
{

}

/*==========================================================================================
	初期化処理
==========================================================================================*/
bool ParticleMaker::Init(void)
{
	//パーティクル群初期化
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		m_Particle[i].m_Use = false;
	}

	//パーティクルテクスチャの読み込む
	DeleteGraph(m_ParticleHandle);
	m_ParticleHandle = LoadGraph(PARTICLE_TEX_PASS);
	if (m_ParticleHandle == -1) 
	{
		ErrorLogFmtAdd("[ParticleMaker.cpp][Function:Init],ハンドルm_ParticleHandle,テクスチャ読み込み失敗");
		return false;
	}

	//パーティクルブレンド処理用画像の読み込む
	DeleteGraph(m_ParticleAddHandle);
	m_ParticleAddHandle = LoadGraph(PARTICLE_EF_TEX_PASS);
	if (m_ParticleAddHandle == -1)
	{
		ErrorLogFmtAdd("[ParticleMaker.cpp][Function:Init],ハンドルm_ParticleAddHandle,テクスチャ読み込み失敗");
		return false;
	}

	return true;
}

/*==========================================================================================
	更新処理
==========================================================================================*/
void ParticleMaker::Update(void)
{
	//使用しているパーティクル処理
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].m_Use == true)
		{
			//位置の移動
			m_Particle[i].m_Pos.x += m_Particle[i].m_Speed.x;
			m_Particle[i].m_Pos.y += m_Particle[i].m_Speed.y;

			//速度の減衰
			m_Particle[i].m_Speed.x -= m_Particle[i].m_Speed.x * m_Particle[i].m_SpeedDecayRate;
			m_Particle[i].m_Speed.y -= m_Particle[i].m_Speed.y * m_Particle[i].m_SpeedDecayRate;

			m_Particle[i].m_Alpha -= m_Particle[i].m_AlphaDecayRate;	//アルファ減衰
			m_Particle[i].m_Life--;					//存在時間減少

			//存在時間は0より小さいなら、パーティクルを未使用状態に
			if (m_Particle[i].m_Life < 0)
			{
				m_Particle[i].m_Use = false;
			}
		}
	}
}

/*==========================================================================================
	描画処理
==========================================================================================*/
void ParticleMaker::Draw(void)
{
	//使用しているパーティクル描画
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].m_Use == true)
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, m_Particle[i].m_Alpha);	//ブレンドモードオン
			Vector2 Pos = { (int)m_Particle[i].m_Pos.x, (int)m_Particle[i].m_Pos.y};
			DrawMethod::DrawRect2DCenter(Pos, m_Particle[i].m_Size, m_ParticleAddHandle);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);				//ブレンドモードオン
		}
	}
}

/*==========================================================================================
	終了処理
==========================================================================================*/
void ParticleMaker::Uninit(void)
{
	//読み込まれたテクスチャの解放
	DeleteGraph( m_ParticleHandle );
	m_ParticleHandle = -1;
	DeleteGraph(m_ParticleAddHandle);
	m_ParticleAddHandle = -1;
}

/*==========================================================================================
	パーティクルを作る
	引数説明:
		Pos: 位置
		Size: サイズ
		Speed: 速度
		SpeedDecayRate: 速度減衰率
		Life: 存在時間
		Alpha: アルファ初期値
==========================================================================================*/
void ParticleMaker::MakeParticle(const Vector2f& Pos, const Vector2& Size, const Vector2f& Speed, float SpeedDecayRate, int Life, int Alpha)
{
	//パーティクル群から未使用のパーティクルを探す
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (m_Particle[i].m_Use == false) 
		{
			m_Particle[i].m_Pos = Pos;
			m_Particle[i].m_Size = Size;
			m_Particle[i].m_Speed = Speed;
			m_Particle[i].m_SpeedDecayRate = max(0,SpeedDecayRate);
			m_Particle[i].m_SpeedDecayRate = min(1,SpeedDecayRate);
			m_Particle[i].m_Life = max(0,Life);
			m_Particle[i].m_Alpha = max(0,Alpha);
			m_Particle[i].m_Alpha = min(255, Alpha);
			m_Particle[i].m_AlphaDecayRate = m_Particle[i].m_Alpha / m_Particle[i].m_Life;
			m_Particle[i].m_Use = true;
			return;
		}
	}

	//使えるパーティクルが存在しない場合、ここに着く
	ErrorLogFmtAdd("[ParticleMaker.cpp][Function:MakeParticle]使えるパーティクルが存在しない");
}

/*==========================================================================================
パーティクルを作る
	引数説明:
		Pos: 位置
		Size: サイズ
		SpeedY: Y軸の基準速度
		SpeedDecayRate: 速度減衰率
==========================================================================================*/
void ParticleMaker::PuyoHardDropParticle(const Vector2& Pos, const Vector2 Size,int SpeedY, float SpeedDecayRate)
{
	int ParticleNum = 100;		//出すパーティクルの数
	int ParticleInterVal = 1;	//パーティクル間隔
	int Life = 20;				//継続時間
	int SpeedYPlus = 1;			//補正速度
	float StartPosX;			//開始X位置
	StartPosX = Pos.x - ParticleInterVal * ParticleNum *0.5f;

	//パーティクルを作る
	for (int i = 0; i < ParticleNum; i++) 
	{
		//位置計算
		Vector2f ParticlePos;
		ParticlePos.x = StartPosX + i * ParticleInterVal;
		ParticlePos.y = (float)Pos.y;

		//速度計算
		Vector2f Speed;
		Speed.x = 0;
		int Rand = GetRand(40);
		Speed.y = (float)SpeedY - Rand * 0.1f;

		MakeParticle(ParticlePos, Size, Speed, SpeedDecayRate, Life,255);
	}
}