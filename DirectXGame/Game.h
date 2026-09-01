#pragma once
#include "KamataEngine.h"


#include "MyMath.h"
#include <list>
#include "Fade.h"



#pragma region モデル

#include "MapChip.h"


#include "Player.h"
#include"Goal.h"
#pragma endregion




class Game 
{
public:

	void Initialize();
	void Update();
	void Draw();
	~Game();

	#pragma region ゲームの状態管理

	bool gameActive = true;


	// 終了フラグ
	
	// デス(オーバー)フラグのgetter
	bool finishedGAME_ = false;
	bool IsFinishedGAME() const { return finishedGAME_; } // ゲームオーバー

	// 敵を倒した場合の(クリア)フラグ
	bool finishedGAME2_ = false;
	bool IsFinishedGAME2() const { return finishedGAME2_; } ////ゲームクリア

	// タイトルに戻る
	bool finishedGAME3_ = false;
	bool IsFinishedGAME3() const { return finishedGAME3_; } ////ゲームクリア


	#pragma endregion

	
	
	#pragma region マップチップ


	// マップチップフィールド
	MapChip* mapChip_;
	KamataEngine::Model* modelBlock_;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	void GenerateBlocks();


    #pragma endregion


























	

	// 衝突判定
	void CheckAllCollisions();


	//デバッグカメラ
	bool isDebugCameraActive_ = false;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;


private:
	
	
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// カメラ
	KamataEngine::Camera camera_;







	
	// 自キャラ
	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;


	// ゴール
	Goal* goal_ = nullptr;
	KamataEngine::Model* modelGoal_ = nullptr;











	#pragma region UI・ポーズ画面

	// ESC
	uint32_t ESC_Handle_ = 0;
	KamataEngine::Sprite* ESC_Sprite_ = nullptr;

	// ESC
	uint32_t ESC_Handle_2 = 0;
	KamataEngine::Sprite* ESC_Sprite_2 = nullptr;

	// ポーズ画面時のUI
	uint32_t PoseUI_Handle_ = 0;
	KamataEngine::Sprite* PoseUI_Sprite_ = nullptr;

	uint32_t PoseUI_Handle_2 = 0;
	KamataEngine::Sprite* PoseUI_Sprite_2 = nullptr;

	uint32_t PoseUI2_Handle_ = 0;
	KamataEngine::Sprite* PoseUI2_Sprite_ = nullptr;

	uint32_t PoseUI2_Handle_2 = 0;
	KamataEngine::Sprite* PoseUI2_Sprite_2 = nullptr;





	#pragma endregion

	#pragma region 効果音

	uint32_t explosion_ = 0;
	uint32_t cannon_ = 0;

	#pragma endregion

	#pragma region フェーズ・フェード

	// ゲームのフェーズ(型)
	enum class Phase 
	{
		kFadeIn, // フェードイン
		kPlay,   // ゲームプレイ
		kPose,
		kDeath,      // プレイヤーのデス演出
		kEnemyDeath, // 敵のデス演出
		kFadeOut,    // フェードアウト(オーバー)
		kFadeOut2,   // フェードアウト(クリア)
		kFadeOut3,   // フェードアウト(タイトルに戻る)
	};

	// ゲームの現在フェーズから開始
	Phase phase_;

	// フェーズの切り替え
	// void ChangePhase();

	// フェード
	Fade* fade_ = nullptr;
	#pragma endregion

};
