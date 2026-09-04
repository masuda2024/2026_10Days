#include "Tutorial.h"
using namespace KamataEngine;

void Tutorial::Initialize()
{

#pragma region UI
	
	// Skipのスプライト
	Skip_Handle_ = TextureManager::Load("UI/Skip.png");
	{
		Skip_Sprite_ = KamataEngine::Sprite::Create(Skip_Handle_, {576, 656});
	}

	Skip_Handle_2 = TextureManager::Load("UI/Pushed_Skip.png");
	{
		Skip_Sprite_2 = KamataEngine::Sprite::Create(Skip_Handle_2, {576, 656});
	}

	// Startのスプライト
	Start_Handle_ = TextureManager::Load("UI/Start.png");
	{
		Start_Sprite_ = KamataEngine::Sprite::Create(Start_Handle_, {576, 656});
	}

	Start_Handle_2 = TextureManager::Load("UI/Pushed_Start.png");
	{
		Start_Sprite_2 = KamataEngine::Sprite::Create(Start_Handle_2, {576, 656});
	}

	// Leftのスプライト
	Left_Handle_ = TextureManager::Load("UI/Left_Arrow_A.png");
	{
		Left_Sprite_ = KamataEngine::Sprite::Create(Left_Handle_, {412, 656});
	}

	Left_Handle_2 = TextureManager::Load("UI/Pushed_Left_Arrow_A.png");
	{
		Left_Sprite_2 = KamataEngine::Sprite::Create(Left_Handle_2, {412, 656});
	}

	// Rightのスプライト
	Right_Handle_ = TextureManager::Load("UI/Right_Arrow_D.png");
	{
		Right_Sprite_ = KamataEngine::Sprite::Create(Right_Handle_, {740, 656});
	}

	Right_Handle_2 = TextureManager::Load("UI/Pushed_Right_Arrow_D.png");
	{
		Right_Sprite_2 = KamataEngine::Sprite::Create(Right_Handle_2, {740, 656});
	}

	UI_01_Handle_ = TextureManager::Load("UI/UI_01.png");
	{
		UI_01_Sprite_ = KamataEngine::Sprite::Create(UI_01_Handle_, {448, 264});
	}

#pragma endregion

#pragma region 1ページ

	// tutorialSprite_ = KamataEngine::Sprite::Create(t_Handle_, {0,0});

	tutorialPos_.x = 0;
	tutorialPos_.y = 0;
	// tutorialPos_ = {0, 0};

	// チュートリアルのスプライト
	t_Handle_ = TextureManager::Load("Scenes/tutorial.png");
	tutorialSprite_ = KamataEngine::Sprite::Create(t_Handle_, tutorialPos_);

	tutorialSprite_->SetSize({1280, 720});

#pragma endregion

#pragma region 2ページ
	
	// tutorialPos_2.x = 1280;
	tutorialPos_2.x = 0;
	tutorialPos_2.y = 0;

	// チュートリアルのスプライト
	t_Handle2_ = TextureManager::Load("axis/axis.jpg");
	tutorialSprite2_ = KamataEngine::Sprite::Create(t_Handle2_, tutorialPos_2);

#pragma endregion

	// 効果音ラボ/ボタン・システム音[1] 決定ボタンを押す2
	Botan_ = Audio::GetInstance()->LoadWave("Sounds/sound/Koukaon_Lab/PushDecision2.mp3");

	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformPlayer_.Initialize();

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Tutorial::Update()
{

	tutorialSprite_->SetPosition(tutorialPos_);
	//tutorialSprite2_->SetPosition(tutorialPos_2);

	switch (page_) 
	{
	case Pages::p1:

		// 2ページへ
		if (Input::GetInstance()->TriggerKey(DIK_D))
		{
			page_ = Pages::p2;
		}

		// シーンの終了条件(スキップ)
		if (Input::GetInstance()->TriggerKey(DIK_S))
		{
			Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedTU_ = true;
			UI_01 = true;
		}

		break;

	case Pages::p2:

		// 1ページへ
		if (Input::GetInstance()->TriggerKey(DIK_A))
		{
			page_ = Pages::p1;
		}

		// シーンの終了条件
		if (Input::GetInstance()->IsTriggerMouse(0))
		{
			//Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedTU_ = true;
			UI_01 = true;
		}

		break;
	}

	switch (phase_) 
	{
	case Phase::kMain:
		/*
		// シーンの終了条件(チュートリアルをスキップ)
		if (Input::GetInstance()->TriggerKey(DIK_S))
		{
		    Audio::GetInstance()->PlayWave(Botan_);
		    // フェードアウト開始
		    phase_ = Phase::kFadeOut;
		    fade_->Start(Fade::Status::FadeOut, 1.0f);
		    finishedTU_ = true;
		}*/

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
			finishedTU_ = true;
		}
		break;
	}
}

void Tutorial::Draw()
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw(dxCommon->GetCommandList());

	switch (page_) 
	{
	case Pages::p1:

		//tutorialSprite_->Draw();
		
		Right_Sprite_->Draw();

		if (Input::GetInstance()->PushKey(DIK_D)) 
		{
			Right_Sprite_2->Draw();
		}

		Skip_Sprite_->Draw();
		if (Input::GetInstance()->PushKey(DIK_S)) 
		{
			Skip_Sprite_2->Draw();
			UI_01 = true;
		}

		break;
	case Pages::p2:
	
		tutorialSprite2_->Draw();
		Left_Sprite_->Draw();

		if (Input::GetInstance()->PushKey(DIK_A)) 
		{
			Left_Sprite_2->Draw();
		}

		Start_Sprite_->Draw();
		if (Input::GetInstance()->IsPressMouse(0))
		{
			Start_Sprite_2->Draw();
			UI_01 = true;
		}

		break;
	}
	
	if (UI_01)
	{
		UI_01_Sprite_->Draw();
	}

	Sprite::PostDraw();

	// フェード
	fade_->Draw();
}

Tutorial::~Tutorial() 
{
	if (fade_) 
	{
		delete fade_;
		fade_ = nullptr;
	}
	delete tutorialSprite_;
	
	delete tutorialSprite2_;

	delete Skip_Sprite_;
	delete Skip_Sprite_2;

	delete Start_Sprite_;
	delete Start_Sprite_2;

	delete Left_Sprite_;
	delete Left_Sprite_2;

	delete Right_Sprite_;
	delete Right_Sprite_2;

	delete UI_01_Sprite_;
	
}