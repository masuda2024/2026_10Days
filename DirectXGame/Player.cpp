#include "Player.h"
#include"KamataEngine.h"




#include"cassert"
#include<numbers>
#include<algorithm>

#define NOMINMAX
#include "MapChip.h"
using namespace KamataEngine;
using namespace MathUtility;





void Player::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position)
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	worldTransform_.translation_ = position;

	camera_ = camera;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	worldTransform_.Initialize();


	// Springin/機械・乗り物/近未来動作音2
	CarH_ = Audio::GetInstance()->LoadWave("Sounds/sound/NearFutureOperationSound2.mp3");

}

void Player::Update() 
{
	// 1.移動入力
	InputMove();
	// 2.移動量を加速して衝突判定する
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	// マップ衝突チェック
	CheckMapCollision(collisionMapInfo);
	// 3.判定結果を反映して移動させる
	CheckMapMove(collisionMapInfo);
	// 4.天井に接触している場合の処理
	CheckMapCeiling(collisionMapInfo);
	// 5.壁に接触している場合の処理
	CheckMapWall(collisionMapInfo);
	// 6.接地状態の切り替え
	CheckMapLanding(collisionMapInfo);

	

	if (worldTransform_.translation_.y < -1.0f) 
	{
		isDead_ = true;
	}




	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); // プレイヤーの座標の計算
}

void Player::Draw()
{
	if (isDead_)
	{
		return;
	}

	model_->Draw(worldTransform_, *camera_);
}





// 移動入力
void Player::InputMove()
{
	// エンジン音
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)&& pushC == 0)
	{
		pushC = 1;
		Audio::GetInstance()->PlayWave(CarH_);
	}

		


	// 移動操作
	if (onGround_) 
	{
		// 加速
		Vector3 acceleration = {};



		if (Input::GetInstance()->PushKey(DIK_SPACE))
		{
			move = true;
		}
		if (move) 
		{
			acceleration.x += kAccleration;
		
			// 加速/減速
			velocity_ += acceleration;
			
			if (Input::GetInstance()->TriggerKey(DIK_SPACE) && move)
			{
				velocity_.y = kJumpAcceleration;
				onGround_ = false;
			}
		}
		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	}
	// 空中
	else 
	{
		if (move)
		{
			// 加速
			Vector3 acceleration = {};
			acceleration.x += kAccleration;

			// 加速/減速
			velocity_ += acceleration;
			
			
		}
		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = max(velocity_.y, -kLimitFallSpeed);
	}




	


}

#pragma region マップ衝突チェック

// 2.マップ衝突チェック
void Player::CheckMapCollision(CollisionMapInfo& info)
{
	//CheckMapCollisionUP(info);
	CheckMapCollisionDown(info);
	//CheckMapCollisionRight(info);
	//CheckMapCollisionLeft(info);
}

// マップ衝突チェック　上
/*
void Player::CheckMapCollisionUP(CollisionMapInfo& info)
{
	// 上昇あり?
	if (info.move.y <= 0) 
	{
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChip::IndexSet indexSet;
	indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	// 隣接セルがともにブロックであればヒット
	if 
	(
		mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || 
		mapChipType == MapChipType::kConst &&  buildEnabled_ ||
	    mapChipType == MapChipType::kConst2 &&  buildEnabled2_ ||
	    mapChipType == MapChipType::kConst3 && buildEnabled3_ ||
		mapChipType == MapChipType::kConst4 &&  buildEnabled4_
	)
	{
		hit = true;
	}


	////右上点の判定
	indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if 
	(
		mapChipType == MapChipType::kBlock || 
		mapChipType == MapChipType::kConst && buildEnabled_|| 
	    mapChipType == MapChipType::kConst2 && buildEnabled2_ ||
		mapChipType == MapChipType::kConst3 && buildEnabled3_ || 
		mapChipType == MapChipType::kConst4 && buildEnabled4_
	) 
	{
		hit = true;
	}


	// ブロックにヒット
	if (hit) 
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChip_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		MapChip::IndexSet indexSetNow;
		indexSetNow = mapChip_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));

		// めり込み先ブロックの範囲短形
		MapChip::Rect rect = mapChip_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}
*/
// マップ衝突チェック 下
void Player::CheckMapCollisionDown(CollisionMapInfo& info)
{
	// 下降あり?
	if (info.move.y >= 0) 
	{
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真下の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	MapChip::IndexSet indexSet;
	indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if
	(
		mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || 
		mapChipType == MapChipType::kConst &&  buildEnabled_ || 
		mapChipType == MapChipType::kConst2 && buildEnabled2_ ||
	    mapChipType == MapChipType::kConst3 && buildEnabled3_ || 
		mapChipType == MapChipType::kConst4 && buildEnabled4_
	)
	{
		hit = true;
	}
	

	////右下点の判定
	indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if 
	(
		mapChipType == MapChipType::kBlock ||
		mapChipType == MapChipType::kConst && buildEnabled_ ||
		mapChipType == MapChipType::kConst2 && buildEnabled2_ || 
		mapChipType == MapChipType::kConst3 && buildEnabled3_ || 
		mapChipType == MapChipType::kConst4 && buildEnabled4_
	) 
	{
		hit = true;
	}
	

	// ブロックにヒット?
	if (hit) 
	{
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChip_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
		// 現在座標が壁の外が判定
		MapChip::IndexSet indexSetNow;
		indexSetNow = mapChip_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));

		// めり込み先ブロックの範囲短形
		MapChip::Rect rect = mapChip_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		// 地面に当たったことを記録する
		info.langing = true;
	}

	// 着地フラグ
	if (info.langing)
	{
		// 着地状態に切り替える(落下を止める)
		onGround_ = true;
		// 着地時にX速度を減衰
		velocity_.x *= (1.0f - kAttenuationLanding);
		
	
		//velocity_.y = 0.01f;
		// Y速度をゼロにする
		velocity_.y = 0.0f;
	}
}

// マップ衝突チェック 右
/*
void Player::CheckMapCollisionRight(CollisionMapInfo& info)
{
	// 右移動あり?
	if (info.move.x <= 0) 
	{
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真右の当たり判定を行う
	bool hit = false;
	// 右上点の判定
	MapChip::IndexSet indexSet;
	indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if 
	(
		mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || 
		mapChipType == MapChipType::kConst &&  buildEnabled_ || 
		mapChipType == MapChipType::kConst2 && buildEnabled2_ ||
	    mapChipType == MapChipType::kConst3 && buildEnabled3_ || 
		mapChipType == MapChipType::kConst4 && buildEnabled4_
	) 
	{
		hit = true;
	}
	

	////右下点の判定
	indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	if 
	(
		mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || 
		mapChipType == MapChipType::kConst &&  buildEnabled_ || 
		mapChipType == MapChipType::kConst2 && buildEnabled2_ ||
	    mapChipType == MapChipType::kConst3 && buildEnabled3_ || 
		mapChipType == MapChipType::kConst4 && buildEnabled4_
	)
	{
		hit = true;
	}
	


	// ブロックにヒット?
	if (hit)
	{
		// 現在座標が壁の外が判定
		MapChip::IndexSet indexSetNow;
		indexSetNow = mapChip_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChip_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲短形
		MapChip::Rect rect = mapChip_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
		// 壁に当たったことを判定結果に記録する
		info.hitwall = true;
	}
}
*/
// マップ衝突チェック 左
/*
void Player::CheckMapCollisionLeft(CollisionMapInfo& info)
{
	// 左移動あり?
	if (info.move.x >= 0)
	{
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++)
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真左の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChip::IndexSet indexSet;
	indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if 
	(
		mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || 
		mapChipType == MapChipType::kConst && buildEnabled_ || 
		mapChipType == MapChipType::kConst2 && buildEnabled2_ ||
	    mapChipType == MapChipType::kConst3 && buildEnabled3_ || 
		mapChipType == MapChipType::kConst4 && buildEnabled4_
	) 
	{
		hit = true;
	}
	


	////左下点の判定
	indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	if 
	(
		mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock || 
		mapChipType == MapChipType::kConst && buildEnabled_ || 
		mapChipType == MapChipType::kConst2 && buildEnabled2_ ||
	    mapChipType == MapChipType::kConst3 && buildEnabled3_ || 
		mapChipType == MapChipType::kConst4 && buildEnabled4_
	)
	{
		hit = true;
	}
	

	// ブロックにヒット?
	if (hit) 
	{

		MapChip::IndexSet indexSetNow;
		indexSetNow = mapChip_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChip_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲短形
		MapChip::Rect rect = mapChip_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = min(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
		// 壁に当たったことを判定結果に記録する
		info.hitwall = true;
	}
}
*/
#pragma endregion

#pragma region マップ衝突後の処理


// 3.判定結果を反映して移動させる
void Player::CheckMapMove(const CollisionMapInfo& info)
{
	// 移動
	worldTransform_.translation_ += info.move;
}


// 4.天井に接触している場合の処理
void Player::CheckMapCeiling(const CollisionMapInfo& info) 
{
	if (info.ceiling) 
	{
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

// 5.壁に接触している場合の処理
void Player::CheckMapWall(CollisionMapInfo& info)
{
	// 壁接触による減速
	if (info.hitwall) 
	{
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}



// 6.接地状態の切り替え処理
void Player::CheckMapLanding(const CollisionMapInfo& info)
{
	// 自キャラが接地状態
	if (onGround_)
	{
		// 接地状態の処理

		// ジャンプ開始
		if (velocity_.y > 0.0f)
		{
			onGround_ = false;
		} else 
		{
			// 落下判定
			//  移動後の4つの角の座標
			std::array<Vector3, kNumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); i++)
			{
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			// 真下の当たり判定を行う
			bool hit = false;
			// 左下点の判定

			MapChip::IndexSet indexSet;
			indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) 
			{
				hit = true;
			}
			////右上点の判定
			indexSet = mapChip_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChip_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) 
			{
				hit = true;
			}

			// 落下なら空中状態に切り替え
			if (!hit)
			{
				// 空中状態の処理
				onGround_ = false;
			}
		}
	} else 
	{
		// 空中状態の処理
		if (info.langing)
		{
			// 着地状態に切り替える(落下を止める)
			onGround_ = true;
			// 着地時にX速度を減衰
			//velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}


KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) 
{
	KamataEngine::Vector3 offsetTable[kNumCorner] = 
	{
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  //  kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}


#pragma endregion









KamataEngine::Vector3 Player::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// ゴールとの衝突応答
AABB_G Player::GetAABB_G() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB_G aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
void Player::OnCollitionGoal(const Goal* goal) 
{
	(void)goal;
	// ゴールフラグを立てる
	isGoal_ = true;
}



AABB_B Player::GetAABB_B() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB_B aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
void Player::OnCollitionBox(const Box* box)
{
	(void)box;
}
