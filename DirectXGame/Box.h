#pragma once
#include "KamataEngine.h"

#include "MyMath.h"
#include <numbers>

class Player;
class Box 
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);
	void Update();
	void Draw();
	~Box();
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();
	const KamataEngine::Vector3& GetPosition() const { return worldTransform_.translation_; }

	static inline const float kWidth = 0.5f;
	static inline const float kHeight = 0.5f;

#pragma region 衝突判定 [ プレイヤー  <<===>>  箱 ]

	// AABB_Bを取得
	AABB_B GetAABB_B();
	// 衝突応答
	void OnCollitionBox(const Player* player);

#pragma endregion


	uint32_t respawnTimer = 120;
	bool B_Used_ = false;
	bool IsB_Used() const { return B_Used_; }

private:
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// カメラ
	KamataEngine::Camera* camera_;
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
};
