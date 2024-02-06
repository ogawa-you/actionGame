#include "Title.h"

Title::Title(const InitData& init)
	: IScene{ init }
{
	//メニュー座標定義
	gameMenu.StringSet(gameName, Scene::Center() - Point(100,-40));

	operationExplanation.StringSet(operationExplanationString, Point(580, 680));

	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	titleBGM.play();
}

void Title::update()
{
	//コントローラー
	auto controller = XInput(0);
	double stick = 0.5;

	//メニュー情報更新
	gameMenu.Update();


	operationExplanation.Update();

	bool fix = getData().inputFix.down() || gameMenu.IsMouseOver() && MouseL.down()
		|| controller.buttonA.down() || controller.buttonB.down();
	
	//メニュー処理
	int sceneChange = gameMenu.IsCurrent();
	if (not fix)
	{
		sceneChange = -9999;
	}

	switch (sceneChange)
	{
	case 0:
		//ステージ情報更新
		getData().currentStage = GameData::StageState::Stage1;
		//横スクロールに遷移
		changeScene(SceneName::Game);
		break;
	case 1:
		//ステージ情報更新
		//getData().currentStage = GameData::StageState::Stage2;
		//横スクロールに遷移
		//changeScene(SceneName::Game);
		break;
	case 2:
		//ステージ情報更新
		//getData().currentStage = GameData::StageState::Stage3;
		//横スクロールに遷移
		//changeScene(SceneName::Game);
		break;
	case 3:
		//設定画面を開く
		//GameData::isSetUp = true;
		break;
	case 4:
		//プログラムを終了
		System::Exit();
		break;
	default:
		break;
	}
}

void Title::draw() const
{
	//背景画像を描画
	backTex.draw();
	//白黒四角の箱を描画
	Rect{ 40,150,methodOfOperation.width(),methodOfOperation .height()}.drawFrame(10, Palette::Red).draw(Palette::Black);
	Rect{ 840,150,methodOfOperation2.width(),methodOfOperation2 .height()}.drawFrame(10, Palette::Red).draw(Palette::Black);
	//操作説明画像描画
	methodOfOperation.draw(40,150);
	methodOfOperation2.draw(840,150);
	//タイトルロゴを描画
	titleTex.scaled(0.6).drawAt(Scene::Center() + Point{0,-150});
	//白黒四角の箱を描画
	Rect{ 530,380,210,255 }.drawFrame(10, Palette::White).draw(Palette::Black);
	Rect{ 580,675,800,500 }.drawFrame(10, Palette::Red).draw(Palette::Black);
	//メニュー描画
	gameMenu.InRectDraw();

	batu.resized(165,25).draw(560, 450);
	batu.resized(165,25).draw(560, 495);
	batu.resized(165,25).draw(560, 540);

	operationExplanation.InRectDraw(false);
}
