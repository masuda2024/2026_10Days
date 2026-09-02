#pragma once
#include"kamataEngine.h"
#include "Fade.h"
class Title
{
public:
	enum class Phase 
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kPose,    // ポーズ
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedTitle_ = false;
	bool IsFinishedT() const { return finishedTitle_; }

	//ゲーム終了フラグ
	bool finishedGame_ = false;
	bool IsFinishedGame() const { return finishedGame_; }







	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~Title();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* titleSprite_ = nullptr;

private:
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;




	uint32_t CreditsHandle_ = 0;
	KamataEngine::Sprite* CreditsSprite_ = nullptr;



	uint32_t Title_Text_H_ = 0;
	KamataEngine::Sprite* Title_Text_S_ = nullptr;
	bool t_texMove = true;



	uint32_t T_StartHandle_ = 0;
	KamataEngine::Sprite* T_StartSprite_ = nullptr;
	uint32_t T_StartHandle_2 = 0;
	KamataEngine::Sprite* T_StartSprite_2 = nullptr;
	int blinkTimer_ = 0; // 点滅用のフレームカウンター






	

	/*
	uint32_t Back_H = 0;
	KamataEngine::Sprite* Back_S = nullptr;
    */



    #pragma region UI
	
	uint32_t Finish_ = 0;
	KamataEngine::Sprite* Finish_S_ = nullptr;
	

	uint32_t Finish_A = 0;
	KamataEngine::Sprite* Finish_S_A = nullptr;
	

	uint32_t Finish_B = 0;
	KamataEngine::Sprite* Finish_S_B = nullptr;
	
	uint32_t Finish_C = 0;
	KamataEngine::Sprite* Finish_S_C = nullptr;


	#pragma endregion






	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	uint32_t Botan_ = 0;
};
