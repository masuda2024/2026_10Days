#pragma once
#include"KamataEngine.h"
#include"MyMath.h"
#include<array>
#define _USE_MATH_DEFINES
#include<math.h>


class Enemy;
class AllRound
{
public:
	void Initialize(KamataEngine::Model* model,  KamataEngine::Vector3 position);

	void Update();

	void Draw(const KamataEngine::Camera& camera);




	// デスフラグ
	bool isDead_ar_ = false;
	bool IsDead_AR() const { return isDead_ar_; }





	#pragma region 衝突判定の準備

	// 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

    #pragma endregion


	#pragma region 衝突判定 [ プレイヤーの範囲攻撃  <<===>>  敵 ]

	// AABBを取得
	AABB2 GetAABB2(int index);
	// 衝突応答
	void OnCollition2(const Enemy* enemy);

    #pragma endregion


	// パーティクルの個数
	static inline const uint32_t kNumParticles = 16;

	std::array<KamataEngine::WorldTransform, kNumParticles> WorldTransforms_;

	// 存続時間
	static inline const float kDuration = 10.0f;

	// 移動の速さ
	static inline const float kSpeed = 1.0f;

	// 分割した1個分の角度
	static inline const float kAngleUnit = 2 * float(M_PI) / kNumParticles;

	// 終了フラグ
	bool isFinished_ = false;
	// 経過時間カウント
	float counter_ = 0.0f;

	// 色変更オブジェクト
	KamataEngine::ObjectColor objectColor_;
	// 色の数値
	KamataEngine::Vector4 color_;


private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera* camera_;

	// モデル
	KamataEngine::Model* model_;

	KamataEngine::Model* model_AllRound_ = nullptr;
};
