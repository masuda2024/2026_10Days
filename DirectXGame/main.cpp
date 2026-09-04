#include <Windows.h>
#include "KamataEngine.h"
using namespace KamataEngine;

#include "Title.h"
#include "Tutorial.h"
#include"Game.h"
#include "Clear.h"
#include "Over.h"



#pragma region シーン

enum class Scene 
{
	kUnknown = 0,
	kTitle,
	kTutorial,
	kGame,
	kClear,
	kOver,
};
Scene scene = Scene::kUnknown;
void ChangeScene();
void UpdateScene();
void DrawScene();


// タイトルシーンの生成
Title* title = nullptr;

// チュートリアルシーンの生成
Tutorial* tutorial = nullptr;

// ゲームシーンのインスタンス生成
Game* game = nullptr;


// ゲームクリアシーンの生成
Clear* clear = nullptr;

// ゲームオーバーシーンの生成
Over* over = nullptr;


#pragma endregion



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// エンジンの初期化
	KamataEngine::Initialize(L"3068_注意!!落とし穴!");

	// DirectXCommonインスタンスを取得する
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	#pragma region シーンの初期化
	// 最初のシーンの初期化
	scene = Scene::kTitle;


	// タイトルのインスタンス生成
	title = new Title;
	// タイトルの初期化
	title->Initialize();


	tutorial = new Tutorial();
	tutorial->Initialize();


	//ゲームのインスタンス生成
	//Game* game = new Game();
	game = new Game();
	// ゲームの初期化
	game->Initialize();

	
	clear = new Clear;
	clear->Initialize();

	over = new Over;
	over->Initialize();
	

	#pragma endregion



	// メインループ
	while (true)
	{
		// エンジンの更新
		if (KamataEngine::Update())
		{
			break;
		}

		// シーン切り替え
		ChangeScene();

		// 現在シーン更新
		UpdateScene();


		#pragma region 描画

		// 描画開始
		dxCommon->PreDraw();

		// 現在シーンの描画
		DrawScene();

		// 描画終了
		dxCommon->PostDraw();

		#pragma endregion


		//ゲームを終了してウインドウを閉じる
		if (title && title->IsFinishedGame())
		{
			break;
		}
	}



	#pragma region 解放

	delete title;
	title = nullptr;
	delete tutorial;
	delete game;
	delete clear;
	delete over;
	game = nullptr;

	#pragma endregion

	
	// エンジンの終了処理
	KamataEngine::Finalize();


	return 0;
}

void UpdateScene()
{

	switch (scene)
	{
	case Scene::kTitle:
		title->Update();
		break;

	case Scene::kTutorial:
		tutorial->Update();
		break;
		
	case Scene::kGame:
		game->Update();

		break;

	case Scene::kClear:
		clear->Update();
		break;

	case Scene::kOver:
		over->Update();
		break;
		
	}
}


void ChangeScene()
{
	switch (scene) 
	{
	case Scene::kTitle:

		if (title->IsFinishedT())
		{

			// シーンの変更
			scene = Scene::kTutorial;
			// 新シーンの生成と初期化
			tutorial = new Tutorial();
			tutorial->Initialize();
		}

		break;
	case Scene::kTutorial:
		if (tutorial->IsFinishedTU())
		{

			// 音声停止
			//Audio::GetInstance()->StopWave(T_Voice_);

			// シーンの変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete tutorial;
			tutorial = nullptr;
			title = nullptr;
			// 新シーンの生成と初期化
			game = new Game();
			game->Initialize();

			// ゲームシーンの音楽を再生
			//G_Voice_ = Audio::GetInstance()->PlayWave(G_Handle_, true);
		}
		break;

	case Scene::kGame:

		
		if (game->IsFinishedGAME()) 
		{

			// 音声停止
			//Audio::GetInstance()->StopWave(G_Voice_);

			// シーンの変更
			scene = Scene::kOver;

			// 旧シーンの解放
			delete game;
			game = nullptr;
			// クリアオーバーの生成
			over = new Over;
			// クリアオーバーの初期化
			over->Initialize();

			// ゲームオーバーシーンの音楽を再生
			// O_Voice_ = Audio::GetInstance()->PlayWave(O_Handle_, true);

		} else if (game->IsFinishedGAME2())
		{
			// 音声停止
			//Audio::GetInstance()->StopWave(G_Voice_);
			
			
			// シーンの変更
			scene = Scene::kClear;

			
			// クリアシーンの生成
			clear = new Clear;
			// クリアシーンの初期化
			clear->Initialize();
			
			
			// 旧シーンの解放
			delete game;
			game = nullptr;
			
			
			
			// ゲームクリアシーンの音楽を再生
			// C_Voice_ = Audio::GetInstance()->PlayWave(C_Handle_, true);
		} else if (game->IsFinishedGAME3())
		{
			// 音声停止
			//Audio::GetInstance()->StopWave(G_Voice_);

			
			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete game;
			game = nullptr;

			// タイトルシーンの生成
			title = new Title;
			// タイトルシーンの初期化
			title->Initialize();

			// タイトルの音楽を再生
			//T_Voice_ = Audio::GetInstance()->PlayWave(T_Handle_, true);
		}

		break;
		
		
	case Scene::kClear:

		if (clear->IsFinishedC())
		{
			// 音声停止
			// Audio::GetInstance()->StopWave(C_Voice_);

			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete clear;
			clear = nullptr;

			// タイトルシーンの生成
			title = new Title;
			// タイトルシーンの初期化
			title->Initialize();

			// タイトルの音楽を再生
			//T_Voice_ = Audio::GetInstance()->PlayWave(T_Handle_, true);
		} else if (clear->IsFinishedC2())
		{
			// シーンの変更
			scene = Scene::kGame;

			// 旧シーンの解放
			delete clear;
			clear = nullptr;

			// 新シーンの生成と初期化
			game = new Game();
			game->Initialize();

			// ゲームシーンの音楽を再生
			//G_Voice_ = Audio::GetInstance()->PlayWave(G_Handle_, true);
		}

		break;

	case Scene::kOver:

		if (over->IsFinishedO())
		{

			// 音声停止
			// Audio::GetInstance()->StopWave(O_Voice_);

			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete over;
			over = nullptr;

			// タイトルシーンの生成
			title = new Title;
			// タイトルシーンの初期化
			title->Initialize();

			// タイトルの音楽を再生
			//T_Voice_ = Audio::GetInstance()->PlayWave(T_Handle_, true);
		} else if (over->IsFinishedO2())
		{
			// シーンの変更
			scene = Scene::kGame;

			// 旧シーンの解放
			delete over;
			over = nullptr;

			// 新シーンの生成と初期化
			game = new Game();
			game->Initialize();

			// ゲームシーンの音楽を再生
			//G_Voice_ = Audio::GetInstance()->PlayWave(G_Handle_, true);
		}
		break;
	}
}

void DrawScene() 
{

	switch (scene)
	{
	case Scene::kTitle:
		title->Draw();
		break;

	case Scene::kTutorial:
		tutorial->Draw();
		break;

	case Scene::kGame:
		game->Draw();
		break;
		
	case Scene::kClear:
		clear->Draw();
		break;

	case Scene::kOver:
		over->Draw();
		break;
		
	}
}