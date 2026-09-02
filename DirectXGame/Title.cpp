#include "Title.h"
using namespace KamataEngine;

void Title::Initialize()
{
	/*
	Back_H = TextureManager::Load("Scenes/space.png");
	Back_S = KamataEngine::Sprite::Create(Back_H, {0, 0 });
	*/

	CreditsHandle_ = TextureManager::Load("Scenes/Credits.png");
	CreditsSprite_ = KamataEngine::Sprite::Create(CreditsHandle_, {10, 518});
	

	Title_Text_H_ = TextureManager::Load("Scenes/Title_text.png");
	Title_Text_S_ = KamataEngine::Sprite::Create(Title_Text_H_, {290, -100});
	Title_Text_S_->SetSize({700, 212});


	#pragma region UI


	T_StartHandle_ = TextureManager::Load("UI/TitleUI.png");
	T_StartSprite_ = KamataEngine::Sprite::Create(T_StartHandle_, {448, 364});

	T_StartHandle_2 = TextureManager::Load("UI/Pushed_TitleUI.png");
	T_StartSprite_2 = KamataEngine::Sprite::Create(T_StartHandle_2, {448, 364});



	Finish_ = TextureManager::Load("UI/FinishUI.png");
	Finish_S_ = KamataEngine::Sprite::Create(Finish_, {234, 164});
	Finish_S_->SetSize({812, 406});




	Finish_A = TextureManager::Load("UI/FinishA.png");//534
	Finish_S_A = KamataEngine::Sprite::Create(Finish_A, {284, 414});
	Finish_S_A->SetSize({250, 125});
	
	Finish_B = TextureManager::Load("UI/FinishB.png");
	Finish_S_B = KamataEngine::Sprite::Create(Finish_B, {746, 414});
	Finish_S_B->SetSize({250, 125});


	Finish_C = TextureManager::Load("UI/Finish_C.png");
	Finish_S_C = KamataEngine::Sprite::Create(Finish_C, {20, 20});

	
	#pragma	endregion	





	//効果音ラボ/ボタン・システム音[1] 決定ボタンを押す2
	Botan_ = Audio::GetInstance()->LoadWave("Sounds/sound/PushDecision2.mp3");

	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void Title::Update()
{




	KamataEngine::Vector2 T_Text_Pos = Title_Text_S_->GetPosition();
	
	if (t_texMove)
	{
		T_Text_Pos.y += 1.0f;
	}
	Title_Text_S_->SetPosition(T_Text_Pos);

	if (T_Text_Pos.y == 100.0f)
	{
		t_texMove = false;
	}

	// タイマーを進める (毎フレーム +1)
	blinkTimer_++;
	if (blinkTimer_ >= 60) 
	{
		blinkTimer_ = 0;
	}


	switch (phase_)
	{
	case Phase::kMain:

		// タイトルシーンの終了条件
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->IsTriggerMouse(0))
		{

			Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedTitle_ = true;
		}

		//ポーズ画面
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))
		{
			Audio::GetInstance()->PlayWave(Botan_);
			phase_ = Phase::kPose;
		}



		break;
	case Phase::kPose:

		if (Input::GetInstance()->TriggerKey(DIK_D))
		{
			Audio::GetInstance()->PlayWave(Botan_);
			phase_ = Phase::kMain;
		}



		if (Input::GetInstance()->TriggerKey(DIK_A)) 
		{
			Audio::GetInstance()->PlayWave(Botan_);
			finishedGame_ = true;
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
			finishedTitle_ = true;
		}
		break;
	}
}

void Title::Draw() 
{
	// DirectXCommonインスタンスの取得
	// DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw();

	//Back_S->Draw();

	CreditsSprite_->Draw();

	Title_Text_S_->Draw();


	// 60で割った余りが30以上の場合のみ描画（点滅処理）
	if (blinkTimer_ % 60 >= 30)
	{
		// マウスが押されている間は押された用の画像、それ以外は通常画像
		if (Input::GetInstance()->IsPressMouse(0))
		{
			T_StartSprite_2->Draw();
		} else
		{
			T_StartSprite_->Draw();
		}
	}
	

	Finish_S_C->Draw();

	if (phase_ == Phase::kPose)
	{
		Finish_S_->Draw();
		Finish_S_A->Draw();
		Finish_S_B->Draw();
	} 





	Sprite::PostDraw();

	// フェード
	fade_->Draw();
}

Title::~Title() 
{

	//  フェード
	delete fade_;
	
	//delete Back_S;
	
	// タイトルのスプライト
	delete titleSprite_;
	delete CreditsSprite_;
	delete Title_Text_S_;



	delete T_StartSprite_;
	delete T_StartSprite_2;


	delete Finish_S_;
	delete Finish_S_A;
	delete Finish_S_B;
	delete Finish_S_C;


}