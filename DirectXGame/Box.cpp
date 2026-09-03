#include "Box.h"
using namespace KamataEngine;
using namespace MathUtility;

void Box::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position) 
{
	model_ = model;
	camera_ = camera;

	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;
	worldTransform_.translation_ = position;

	worldTransform_.UpdateMatrix();
}

void Box::Update() 
{


	// アフィン変換行列
	// worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	// worldTransform_.TransferMatrix();

	worldTransform_.UpdateMatrix();
}

void Box::Draw() 
{
	if (B_Used_)
	{
		return;
	}

	model_->Draw(worldTransform_, *camera_);
}

Box::~Box() {}

KamataEngine::Vector3 Box::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

#pragma region 衝突判定 [ プレイヤー  <<===>>  箱 ]

AABB_B Box::GetAABB_B() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB_B aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 衝突応答
void Box::OnCollitionBox(const Player* player)
{
	(void)player;
	B_Used_ = true;
}

#pragma endregion
