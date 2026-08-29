#include "Game.h"
#include <random>
#include <ctime>
#include <cstdlib>

using namespace KamataEngine;
using namespace MathUtility;



std::random_device seedGenerator;
std::mt19937 randomEngine(seedGenerator());
std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);



void Game::Initialize() 
{
	//デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);


	#pragma region フェーズ・フェード
	// フェーズインから開始
	phase_ = Phase::kFadeIn;
	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
    #pragma endregion

	#pragma region UI

	// ESCのスプライト
	ESC_Handle_ = TextureManager::Load("UI/ESC.png");
	ESC_Sprite_ = KamataEngine::Sprite::Create(ESC_Handle_, {10, 100});

	ESC_Handle_2 = TextureManager::Load("UI/Pushed_ESC.png");
	ESC_Sprite_2 = KamataEngine::Sprite::Create(ESC_Handle_2, {10, 100});

	
	PoseUI_Handle_ = TextureManager::Load("UI/Pose_UI.png");
	PoseUI_Sprite_ = KamataEngine::Sprite::Create(PoseUI_Handle_, {448, 164});

	PoseUI_Handle_2 = TextureManager::Load("UI/Pushed_Pose_UI.png");
	PoseUI_Sprite_2 = KamataEngine::Sprite::Create(PoseUI_Handle_2, {448, 164});

	PoseUI2_Handle_ = TextureManager::Load("UI/Pose_UI_2.png");
	PoseUI2_Sprite_ = KamataEngine::Sprite::Create(PoseUI2_Handle_, {448, 364});

	PoseUI2_Handle_2 = TextureManager::Load("UI/Pushed_Pose_UI_2.png");
	PoseUI2_Sprite_2 = KamataEngine::Sprite::Create(PoseUI2_Handle_2, {448, 364});
	


	
    #pragma endregion

	

	
	#pragma region 効果音

	//効果音ラボ/戦闘[2]/爆発1
	//explosion_ = Audio::GetInstance()->LoadWave("Sounds/sound/Explosion1.mp3");
	// 効果音ラボ/戦闘[2]/大爆発1
	explosion_ = Audio::GetInstance()->LoadWave("Sounds/sound/HugeExplosion2.mp3");
	// 効果音ラボ/戦闘[2]/大砲1
	cannon_ = Audio::GetInstance()->LoadWave("Sounds/sound/Cannon1.mp3");
	
	#pragma endregion

	worldTransform_.Initialize();
	camera_.Initialize();
}







void Game::Update()
{

	#pragma region デバッグカメラ
	debugCamera_->Update();

	#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0))
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
	}



	#endif // DEBUG
	// ChangePhase();
	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.TransferMatrix();
		camera_.UpdateMatrix();
	}
	#pragma endregion


	if (gameActive) 
	{




		
	}

	switch (phase_)
	{
	case Phase::kPlay: 
	{
		CheckAllCollisions();

		#pragma region 仮設コード
		
		/**/
		// ゲームクリア(仮)
		if (Input::GetInstance()->TriggerKey(DIK_C))
		{
		    phase_ = Phase::kEnemyDeath;
		}
		// ゲームオーバー(仮)
		if (Input::GetInstance()->TriggerKey(DIK_O))
		{
		    phase_ = Phase::kDeath;
		}
		
		#pragma endregion




		

		//ESCAPEキーを押してポーズ画面へ移行
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))
		{
			phase_ = Phase::kPose;
		}



		break;
	}

	case Phase::kPose: 
	{

		gameActive = false;
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) 
		{
			phase_ = Phase::kPlay;
			gameActive = true;
		}
		if (Input::GetInstance()->TriggerKey(DIK_T)) 
		{
			phase_ = Phase::kFadeOut3;
		}
		break;
	}

	case Phase::kDeath: 
	{
		// フェードアウト開始
		phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);

		break;
	}

	case Phase::kEnemyDeath:
	{
		// フェードアウト開始
		phase_ = Phase::kFadeOut2;
		fade_->Start(Fade::Status::FadeOut, 1.0f);

		break;
	}
	
	case Phase::kFadeIn:
	{	
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			phase_ = Phase::kPlay;
		}
		break;
	}
	
	case Phase::kFadeOut:
	{
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finishedGAME_ = true;
		}
		break;
	}
	
	case Phase::kFadeOut2:
	{
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			finishedGAME2_ = true;
		}
		break;
	}
	
	case Phase::kFadeOut3:
	{
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finishedGAME3_ = true;
		}
		break;
	}
	}

}


void Game::Draw() 
{ 
	//ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();



	#pragma region スプライト(背景)

	Sprite::PreDraw(dxCommon->GetCommandList());

	//stage_->Draw();

	
	Sprite::PostDraw();

	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();

	#pragma endregion




	#pragma region モデル

	Model::PreDraw();
	
	
	// ここに3Dモデルインスタンスの描画処理を記述する
	//player_->Draw();

	
	


	
	
	Model::PostDraw();
	
	#pragma endregion
	
	

	#pragma region スプライト(UI)

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	

	#pragma region UI
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kPose || phase_ == Phase::kDeath || phase_ == Phase::kEnemyDeath) 
	{
		ESC_Sprite_->Draw();

		if (Input::GetInstance()->PushKey(DIK_ESCAPE)) 
		{
			ESC_Sprite_2->Draw();
		}
	}

	// ポーズ画面
	if (phase_ == Phase::kPose)
	{
		PoseUI_Sprite_->Draw();
		PoseUI2_Sprite_->Draw();

		if (Input::GetInstance()->PushKey(DIK_ESCAPE)) 
		{
			PoseUI_Sprite_2->Draw();
		}

		if (Input::GetInstance()->PushKey(DIK_T))
		{
			PoseUI2_Sprite_2->Draw();
		}
	}

#pragma endregion

	// スプライト描画後処理
	Sprite::PostDraw();

	#pragma endregion

}


void Game::CheckAllCollisions()
{ 
	
	


}




Game::~Game() 
{ 
    delete debugCamera_;

	
	

	
	#pragma region UI

	delete ESC_Sprite_;
	delete ESC_Sprite_2;

	
	delete PoseUI_Sprite_;
	delete PoseUI_Sprite_2;

	delete PoseUI2_Sprite_;
	delete PoseUI2_Sprite_2;

	
	#pragma endregion




}