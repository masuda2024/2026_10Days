#pragma once
#include "KamataEngine.h"
#include "MyMath.h"


class Goal;
class Box;
class MapChip;
class Player
{
public:
	


	// デスフラグ
	bool isDead_ = false;
	// デスフラグのgetter
	bool IsDead() const { return isDead_; }




	
	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;





	

	// ゴールフラグ
	bool isGoal_ = false;
	bool IsGoal() const { return isGoal_; }
	 //AABB_Gを取得
	AABB_G GetAABB_G();
	// ゴールとの衝突応答
	void OnCollitionGoal(const Goal* goal);


	//箱
	// AABB_Bを取得
	AABB_B GetAABB_B();
	// ゴールとの衝突応答
	void OnCollitionBox(const Box* box);





	#pragma region マップチップとの当たり判定

	bool buildEnabled_ = false;
	void SetBuildEnabled(bool enabled) { buildEnabled_ = enabled; }

	bool buildEnabled2_ = false;
	void SetBuildEnabled2(bool enabled) { buildEnabled2_ = enabled; }

	bool buildEnabled3_ = false;
	void SetBuildEnabled3(bool enabled) { buildEnabled3_ = enabled; }

	bool buildEnabled4_ = false;
	void SetBuildEnabled4(bool enabled) { buildEnabled4_ = enabled; }





	// マップチップによるフィールド
	MapChip* mapChip_ = nullptr;

	void SetMapChip(MapChip* mapChip) { mapChip_ = mapChip; }

	// マップとの当たり判定情報
	struct CollisionMapInfo 
	{
		bool ceiling = false;            // 天井衝突フラグ
		bool langing = false;            // 着地フラグ
		bool hitwall = false;            // 壁接触フラグ
		KamataEngine::Vector3 move = {}; // 移動量
	};

	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUP(CollisionMapInfo& info);    // 上方向
	void CheckMapCollisionDown(CollisionMapInfo& info);  // 下方向
	void CheckMapCollisionRight(CollisionMapInfo& info); // 右方向
	void CheckMapCollisionLeft(CollisionMapInfo& info);  // 左方向
	// 3.判定結果を反映して移動させる
	void CheckMapMove(const CollisionMapInfo& info);
	// 4.天井に接触している場合の処理
	void CheckMapCeiling(const CollisionMapInfo& info);
	// 5.壁に接触している場合の処理
	void CheckMapWall(CollisionMapInfo& info);
	// 6.接地状態の切り替え
	void CheckMapLanding(const CollisionMapInfo& info);
	

	// 角
	enum Corner 
	{
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上
		kNumCorner // 要素数
	};

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);


	#pragma endregion


	#pragma region 移動
	

	float randomTimer_ = 0.0f;
	float randomInterval_ = 0.5f;


	bool move = false;
	
	// 加速度
	static inline const float kAccleration = 0.5f;
	// 減衰(ブレーキ)
	static inline const float kAttenuation = 0.5f;
	// 制限速度
	static inline const float kLimitRunSpeed = 0.25f;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.8f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 0.6f;
	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 1.5f;
	// 微小な数値
	static inline const float kGroundSearchHeight = 0.1f;
	// 着地時の速度減衰率
	static inline const float kAttenuationWall = 0.9f;

	// 左右
	enum class LRDirection
	{
		kRight,
		kLeft,
	};

	
	// 接地状態フラグ
	bool onGround_ = true;

	

	LRDirection lrDirection_ = LRDirection::kRight;

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	#pragma endregion

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();
	
	
	




	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	void InputMove();

	// 更新
	void Update();

	// 描画
	void Draw();

	static inline const float kBlank = 0.9f;

	
private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera* camera_;

	// モデル
	KamataEngine::Model* model_;

	KamataEngine::Vector3 velocity_ = {};


	//効果音
	uint32_t CarH_ = 0;
	int32_t pushC = 0;
};
