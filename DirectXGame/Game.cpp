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

	
	// Springin/機械・乗り物[1]/大型ロボットの駆動音3
	build_S_ = Audio::GetInstance()->LoadWave("Sounds/sound/Springin/LargeRobotOperatingSound3.mp3");
	
	#pragma endregion


	#pragma region マップチップ
	
	// 3Dモデルの生成
	modelBlock_ = Model::CreateFromOBJ("block");
	
	modelConst_ = Model::CreateFromOBJ("block");
	modelConst2_ = Model::CreateFromOBJ("block");
	modelConst3_ = Model::CreateFromOBJ("block");
	modelConst4_ = Model::CreateFromOBJ("block");

	// マップチップフィールドの生成
	mapChip_ = new MapChip;
	// マップチップフィールドの初期化
	mapChip_->LoadMapchipCsv("Resources/blocks.csv");
	
	GenerateBlocks();
    #pragma endregion


	#pragma region プレイヤー

	// 3Dモデルデータの生成
	modelPlayer_ = Model::CreateFromOBJ("Player", true);
	// 自キャラの生成
	player_ = new Player();
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChip_->GetMapChipPositionByIndex(5, 18);
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	player_->SetMapChip(mapChip_); // 自キャラの生成と初期化

	#pragma endregion


	#pragma region 箱

	modelBox_ = Model::CreateFromOBJ("box");
	// 座標をマップチップ番号で指定
	std::vector<KamataEngine::Vector2> boxTilePositions =
	{
	    {16,  17}, // 1つ目
	    {27,  17}, // 2つ目
	    {38, 17}, // 3つ目
	    {55, 17}  // 4つ目
	};
	for (const auto& tilePos : boxTilePositions)
	{
		Box* box = new Box();

		Vector3 b_pos = mapChip_->GetMapChipPositionByIndex(static_cast<uint32_t>(tilePos.x), static_cast<uint32_t>(tilePos.y));

		box->Initialize(modelBox_, &camera_, b_pos);

		boxes_.push_back(box);
	}
	

    #pragma endregion


	#pragma region ゴール

	// ゴール
	modelGoal_ = Model::CreateFromOBJ("goal", true);
	// ゴールの生成
	goal_ = new Goal();
	// ゴールの座標
	Vector3 goalPosition = mapChip_->GetMapChipPositionByIndex(75, 18);
	goal_->Initialize(modelGoal_, &camera_, goalPosition);
	goal_->SetMapChip(mapChip_);

    #pragma endregion




	worldTransform_.Initialize();
	
	camera_.Initialize();
	camera_.translation_ = {40.0f, 30.0f, -50.0f};
	camera_.UpdateMatrix();
}



void Game::GenerateBlocks()
{

	uint32_t numBlockVirtical = mapChip_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChip_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i)
	{
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) 
	{
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) 
		{
			MapChipType type = mapChip_->GetMapChipTypeByIndex(j, i);

			if 
			(
				type == MapChipType::kBlock || 
				type == MapChipType::kConst || 
				type == MapChipType::kConst2 || 
				type == MapChipType::kConst3 || 
				type == MapChipType::kConst4 
			) 
			{
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();

				worldTransformBlocks_[i][j] = worldTransform;

				worldTransformBlocks_[i][j]->translation_ = mapChip_->GetMapChipPositionByIndex(j, i);
				
			}
		}
	}



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


		
		// 自キャラの更新
		player_->Update();
		player_->SetBuildEnabled(Build == 1);
		player_->SetBuildEnabled2(Build2 == 1);
		player_->SetBuildEnabled3(Build3 == 1);
		player_->SetBuildEnabled4(Build4 == 1);



		#pragma region 敵の更新

		for (Box* boxes : boxes_)
		{
			boxes->Update();
		}

		boxes_.remove_if
		([](Box* boxes) {
			if (boxes->IsB_Used())
			{
				delete boxes;
				return true;
			}
			return false;
		});
	

        #pragma endregion

	}

	switch (phase_)
	{
	case Phase::kPlay: 
	{
		CheckAllCollisions();

		
		//ESCAPEキーを押してポーズ画面へ移行
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))
		{
			phase_ = Phase::kPose;
		}

		
	    if (player_->IsDead())
		{
			phase_ = Phase::kDeath;
		}

		if (player_->IsGoal())
		{
			phase_ = Phase::kEnemyDeath;
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









	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) 
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) 
		{
			if (!worldTransformBlock)
				continue;
			// アフィン変換行列の作成
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}



	
	if (Input::GetInstance()->IsTriggerMouse(0) && boxCount == 1)
	{
		if (!buildSoundPlayed1) 
		{
			Audio::GetInstance()->PlayWave(build_S_);
			buildSoundPlayed1 = true;
		}
		
		Build = true;
	}
	if (Input::GetInstance()->IsTriggerMouse(0) && boxCount == 2)
	{
		if (!buildSoundPlayed2)
		{
			Audio::GetInstance()->PlayWave(build_S_);
			buildSoundPlayed2 = true;
		}
		Build2 = true;
	}
	if (Input::GetInstance()->IsTriggerMouse(0) && boxCount == 3)
	{
		if (!buildSoundPlayed3)
		{
			Audio::GetInstance()->PlayWave(build_S_);
			buildSoundPlayed3 = true;
		}
		Build3 = true;
	}
	if (Input::GetInstance()->IsTriggerMouse(0) && boxCount == 4)
	{
		if (!buildSoundPlayed4)
		{
			Audio::GetInstance()->PlayWave(build_S_);
			buildSoundPlayed4 = true;
		}
		boxCount = 5;
		Build4 = true;
	}


	goal_->Update();



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
	

	
	// ブロックの描画
	for (uint32_t i = 0; i < worldTransformBlocks_.size(); ++i)
	{
		for (uint32_t j = 0; j < worldTransformBlocks_[i].size(); ++j) 
		{
			WorldTransform* worldTransformBlock = worldTransformBlocks_[i][j];

			if (!worldTransformBlock)
			{
				continue;
			}

			MapChipType type = mapChip_->GetMapChipTypeByIndex(j, i);

			if (type == MapChipType::kBlock) 
			{
				modelBlock_->Draw(*worldTransformBlock, camera_);
			} else if (type == MapChipType::kConst && Build == 1 )
			{
				modelConst_->Draw(*worldTransformBlock, camera_);
			} 

			if (type == MapChipType::kConst2 && Build2 == 1)
			{
				modelConst_->Draw(*worldTransformBlock, camera_);
			} 

			if (type == MapChipType::kConst3 && Build3 == 1)
			{
				modelConst_->Draw(*worldTransformBlock, camera_);
			} 

			if (type == MapChipType::kConst4 && Build4 == 1)
			{
				modelConst_->Draw(*worldTransformBlock, camera_);
			} 

		}
	}


	




    player_->Draw();
	
	goal_->Draw();

	for (Box* boxes : boxes_)
	{
		boxes->Draw();
	}

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
	AABB_G aabb1, aabb2;

	aabb1 = player_->GetAABB_G();
	aabb2 = goal_->GetAABB_G();

	if (IsCollition_G(aabb1, aabb2))
	{
		player_->OnCollitionGoal(goal_);
		goal_->OnCollitionGoal(player_);
	}



	AABB_B aabb3, aabb4;
	aabb3 = player_->GetAABB_B();
	for (Box* boxes : boxes_) 
	{
		aabb4 = boxes->GetAABB_B();

		if (IsCollition_B(aabb3, aabb4))
		{
			player_->OnCollitionBox(boxes);
			boxes->OnCollitionBox(player_);

			canUse = true;
			boxCount++;
		}else
		{
			canUse = false;
		}
	}




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



	#pragma region マップチップの解放

	delete modelBlock_;
	

	delete mapChip_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_)
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine)
		{
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();






	#pragma endregion


	delete player_;
	delete goal_;
	for (Box* boxes : boxes_)
	{
		delete boxes;
	}
}