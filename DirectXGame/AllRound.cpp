#include "AllRound.h"

#include "cassert"
#include <algorithm>
#include <numbers>

#define NOMINMAX

using namespace KamataEngine;
using namespace MathUtility;

void AllRound::Initialize(Model* model,  KamataEngine::Vector3 position)
{
	// NULLポイントチェック
	//assert(model);
	
	model_AllRound_ = KamataEngine::Model::CreateFromOBJ("SkyBlue_Ball");

	model_ = model;

	// textureHandle_ = textureHandle;
	for (KamataEngine::WorldTransform& worldTransform : WorldTransforms_)
	{
		worldTransform.translation_ = position;
		worldTransform.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	}

	//camera_ = camera;
	
	// worldTransform_.Initialize();

	// ワールド変換の初期化
	for (KamataEngine::WorldTransform& worldTransform : WorldTransforms_)
	{
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void AllRound::Update()
{

	for (KamataEngine::WorldTransform& worldTransform : WorldTransforms_)
	{
		// アフィン変換行列
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix(); // プレイヤーの座標の計算
	}

	for (uint32_t i = 0; i < kNumParticles; i++)
	{
		// 基本となる速度ベクトル
		KamataEngine::Vector3 velocity = {kSpeed, 0, 0};
		// 回転角を計算する
		float angle = kAngleUnit * i;
		// Z軸まわり回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		// 移動処理
		WorldTransforms_[i].translation_ += velocity;
	}
	// カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);

	// 存続時間の上限に達したら
	if (counter_ >= kDuration)
	{
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}
	// 終了なら何もしない
	if (isFinished_) 
	{
		return;
	}
}

void AllRound::Draw(const KamataEngine::Camera& camera)
{

	for (KamataEngine::WorldTransform& worldTransform : WorldTransforms_)
	{
		model_AllRound_->Draw(worldTransform, camera, &objectColor_);
	}

	// 終了なら何もしない
	if (isFinished_)
	{
		return;
	}
}

KamataEngine::Vector3 AllRound::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}


#pragma region 衝突判定 [ プレイヤーの範囲攻撃  <<===>>  敵 ]

AABB2 AllRound::GetAABB2(int index) 
{
	Vector3 worldPos;

	worldPos.x = WorldTransforms_[index].matWorld_.m[3][0];
	worldPos.y = WorldTransforms_[index].matWorld_.m[3][1];
	worldPos.z = WorldTransforms_[index].matWorld_.m[3][2];

	AABB2 aabb;

	aabb.min = {worldPos.x - kWidth / 2, worldPos.y - kHeight / 2, worldPos.z - kWidth / 2};

	aabb.max = {worldPos.x + kWidth / 2, worldPos.y + kHeight / 2, worldPos.z + kWidth / 2};

	return aabb;
}

// 弾と敵の衝突応答
void AllRound::OnCollition2(const Enemy* enemy)
{
	(void)enemy;
	isDead_ar_ = true;
}

#pragma endregion