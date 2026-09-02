#pragma once
#include "Fade.h"
#include "KamataEngine.h"

class Clear
{
public:
	enum class Phase 
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedC_ = false;
	bool IsFinishedC() const { return finishedC_; }

	// リトライ
	bool finishedC_2 = false;
	bool IsFinishedC2() const { return finishedC_2; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~Clear();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト

	// マップチップフィールド
	// MapChipField* mapChipField_;



private:
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;


	/*
	uint32_t Back_H = 0;
	KamataEngine::Sprite* Back_S = nullptr;
    */


	#pragma region UI
	uint32_t ClearFontHandle_ = 0;
	KamataEngine::Sprite* ClearFontSprite_ = nullptr;

	
	uint32_t UI_R_Handle_ = 0;
	KamataEngine::Sprite* UI_RSprite_ = nullptr;
	uint32_t UI_R_Handle_2 = 0;
	KamataEngine::Sprite* UI_RSprite_2 = nullptr;

	uint32_t UI_T_Handle_ = 0;
	KamataEngine::Sprite* UI_TSprite_ = nullptr;
	uint32_t UI_T_Handle_2 = 0;
	KamataEngine::Sprite* UI_TSprite_2 = nullptr;
	
	#pragma endregion





	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	uint32_t Botan_ = 0;

	
};
