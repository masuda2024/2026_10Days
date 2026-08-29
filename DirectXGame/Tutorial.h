#pragma once
#include"kamataEngine.h"
#include "Fade.h"
class Tutorial
{
public:
	enum class Phase
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedTU_ = false;
	// デスフラグのgetter
	bool IsFinishedTU() const { return finishedTU_; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~Tutorial();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	
	
	KamataEngine::Vector2 tutorialPos_ = {0, 0};
	KamataEngine::Vector2 tutorialPos_2 = {0, 0};
	KamataEngine::Vector2 tutorialPos_3 = {0, 0};

	
	enum class Pages 
	{
		p1,
		p2,
	};
	Pages page_ = Pages ::p1;

private:
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;
	// モデルプレイヤー
	// KamataEngine::Model* modelPlayer_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;


	/**/
	// Skip
	uint32_t Skip_Handle_ = 0;
	KamataEngine::Sprite* Skip_Sprite_ = nullptr;

	uint32_t Skip_Handle_2 = 0;
	KamataEngine::Sprite* Skip_Sprite_2 = nullptr;

	// Start
	uint32_t Start_Handle_ = 0;
	KamataEngine::Sprite* Start_Sprite_ = nullptr;

	uint32_t Start_Handle_2 = 0;
	KamataEngine::Sprite* Start_Sprite_2 = nullptr;

	// Left
	uint32_t Left_Handle_ = 0;
	KamataEngine::Sprite* Left_Sprite_ = nullptr;

	uint32_t Left_Handle_2 = 0;
	KamataEngine::Sprite* Left_Sprite_2 = nullptr;

	// Right
	uint32_t Right_Handle_ = 0;
	KamataEngine::Sprite* Right_Sprite_ = nullptr;

	uint32_t Right_Handle_2 = 0;
	KamataEngine::Sprite* Right_Sprite_2 = nullptr;
	
	
	bool UI_01 = false;
	uint32_t UI_01_Handle_ = 0;
	KamataEngine::Sprite* UI_01_Sprite_ = nullptr;


	// 1ページ
	uint32_t t_Handle_ = 0;
	KamataEngine::Sprite* tutorialSprite_ = nullptr;
	
	// 2ページ
	uint32_t t_Handle2_ = 0;
	KamataEngine::Sprite* tutorialSprite2_ = nullptr;
	
	
	uint32_t Botan_ = 0;
	
};
