#include "Clear.h"
#include <cassert>
#include "MyMath.h"
#include "math.h"

using namespace KamataEngine;

void Clear::Initialize()
{
	
	/*
	Back_H = TextureManager::Load("Scenes/space.png");
	Back_S = KamataEngine::Sprite::Create(Back_H, {0, 0});
	*/
	#pragma region UI

	ClearFontHandle_ = TextureManager::Load("UI/GAME_CLEAR.png");
	ClearFontSprite_ = Sprite::Create(ClearFontHandle_, {187, 20});

	
	UI_R_Handle_ = TextureManager::Load("UI/R_Retry.png");
	UI_RSprite_ = Sprite::Create(UI_R_Handle_, {246, 614});
	UI_R_Handle_2 = TextureManager::Load("UI/Pushed_R_Retry.png");
	UI_RSprite_2 = Sprite::Create(UI_R_Handle_2, {246, 614});

	UI_T_Handle_ = TextureManager::Load("UI/T_Title.png");
	UI_TSprite_ = Sprite::Create(UI_T_Handle_, {650, 614});
	UI_T_Handle_2 = TextureManager::Load("UI/Pushed_T_Title.png");
	UI_TSprite_2 = Sprite::Create(UI_T_Handle_2, {650, 614});

	#pragma endregion

	
	// 効果音ラボ/ボタン・システム音[1] 決定ボタンを押す2
	Botan_ = Audio::GetInstance()->LoadWave("Sounds/sound/Koukaon_Lab/PushDecision2.mp3");

	

	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();

	
	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Clear::Update() 
{
	

	
	
	
	
	
	ClearFontSprite_->SetSize({896, 352});
	ClearFontSprite_->SetPosition({192, 10});
	

	switch (phase_)
	{
	case Phase::kMain:

		// シーンの終了条件
		if (Input::GetInstance()->TriggerKey(DIK_T)) 
		{
			Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedC_ = true;
		}
		// リトライ
		if (Input::GetInstance()->TriggerKey(DIK_R))
		{
			Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedC_2 = true;
		}

		break;
	case Phase::kFadeIn:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kFadeOut:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finishedC_ = true;
		}
		break;
	}
}

void Clear::Draw()
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw();
	
	Model::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());
	

	//Back_S->Draw();

	ClearFontSprite_->Draw();
	
	
	UI_RSprite_->Draw();
	if (Input::GetInstance()->PushKey(DIK_R))
	{
		UI_RSprite_2->Draw();
	}

	UI_TSprite_->Draw();
	if (Input::GetInstance()->PushKey(DIK_T))
	{
		UI_TSprite_2->Draw();
	}

	Sprite::PostDraw();

	// フェード
	fade_->Draw();
}

Clear::~Clear() 
{
	//  フェード
	delete fade_;
	
	//delete Back_S;
	
	delete ClearFontSprite_;
	
	delete UI_RSprite_;
	delete UI_RSprite_2;
	delete UI_TSprite_;
	delete UI_TSprite_2;

	
}