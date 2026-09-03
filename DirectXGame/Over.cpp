#include "Over.h"
#include <cassert>
#include "MyMath.h"
#include "math.h"

using namespace KamataEngine;

void Over::Initialize()
{
	#pragma region UI

	OverFontHandle_ = TextureManager::Load("UI/GAME_OVER.png");
	OverFontSprite_ = Sprite::Create(OverFontHandle_, {187, 20});

	
	UI_R_Handle_ = TextureManager::Load("UI/R_Retry.png");
	UI_RSprite_ = Sprite::Create(UI_R_Handle_, {246, 614});
	UI_R_Handle_2 = TextureManager::Load("UI/Pushed_R_Retry.png");
	UI_RSprite_2 = Sprite::Create(UI_R_Handle_2, {246, 614});

	UI_T_Handle_ = TextureManager::Load("UI/T_Title.png");
	UI_TSprite_ = Sprite::Create(UI_T_Handle_, {650, 614});
	UI_T_Handle_2 = TextureManager::Load("UI/Pushed_T_Title.png");
	UI_TSprite_2 = Sprite::Create(UI_T_Handle_2, {650, 614});

	#pragma region UI

	// 効果音ラボ/ボタン・システム音[1] 決定ボタンを押す2
	Botan_ = Audio::GetInstance()->LoadWave("Sounds/sound/PushDecision2.mp3");

	
	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// worldTransformPlayer_.Initialize();

	
	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Over::Update()
{

	

	OverFontSprite_->SetSize({896, 352});

	
	switch (phase_)
	{
	case Phase::kMain:

		// タイトルシーンの終了条件
		if (Input::GetInstance()->TriggerKey(DIK_T))
		{
			Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedO_ = true;
		}
		// リトライ
		if (Input::GetInstance()->TriggerKey(DIK_R)) 
		{
			Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedO_2 = true;
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
			finishedO_ = true;
		}
		break;
	}
}

void Over::Draw()
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw();

	
	Model::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	OverFontSprite_->Draw();
	

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

Over::~Over()
{

	//  フェード
	delete fade_;

	
	delete OverFontSprite_;
	
	delete UI_RSprite_;
	delete UI_RSprite_2;
	delete UI_TSprite_;
	delete UI_TSprite_2;
}