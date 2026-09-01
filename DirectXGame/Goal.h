#pragma once
#include "KamataEngine.h"
#include "MyMath.h"

class MapChip;
class Player;
class Goal
{
public:
	
	// 敵の当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// マップチップによるフィールド
	MapChip* mapChip_ = nullptr;

	void SetMapChip(MapChip* mapChip) { mapChip_ = mapChip; }

	// マップとの当たり判定情報
	struct CollisionMapInfo
	{
		bool ceiling = false; // 天井衝突フラグ
		bool langing = false; // 着地フラグ
		bool hitwall = false; // 壁接触フラグ
		                      // KamataEngine::Vector3 move = {}; // 移動量
	};
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();
	
	
	
	
	// AABBを取得
	AABB_G GetAABB_G();
	// 衝突応答
	void OnCollitionGoal(const Player* player);






	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Camera* camera_;
	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;

	KamataEngine::Model* model_;

	KamataEngine::Vector3 velocity_ = {};
};
