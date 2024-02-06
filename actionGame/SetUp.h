#pragma once

class SetUp
{
public:
	SetUp(){};

	enum class NowScene
	{
		MenuSelect, AudioConfig, KeyConfig, Save, Load, Explanation
	};
	NowScene selectScene = NowScene::MenuSelect;

	Array<String> firstMenuChar = { U"おんりょう設定",U"キー設定",U"セーブ",U"ロード",U"せつめい",U"タイトルへもどる",U"おわる" };//選択メニュー内文字
	Menu firstMenu;

	Font font30{ 30 ,U"font/kkm_analogtv.ttf" };//フォント

	bool isEnable = false;
	bool isFirst = true;
	bool isChangeTitle = false;

	/*ConfigGauge mainVolumeGauge;
	ConfigGauge BGMVolumeGauge;
	ConfigGauge SEVolumeGauge;*/
	int gaugeID = 0;

	Rect mainVolumeGaugeRect = { 481 ,255 ,371, 26 };//ゲージの情報
	Rect BGMVolumeGaugeRect = { 481 ,375 ,371, 26 };//ゲージの情報
	Rect SEVolumeGaugeRect = { 481 ,495 ,371, 26 };//ゲージの情報
	RoundRect gaugeBackRect = { 350 ,190 ,600, 390 ,10 };//ゲージの情報

	Audio selectAudio{ U"Material/4.SE/16.ゲームを一時停止した時1.mp3" };

	/// @brief 設定の処理
	void Update()
	{
		//ConfigGauge* gauges[] = { &mainVolumeGauge ,&BGMVolumeGauge,&SEVolumeGauge };

		//シーン管理
		switch (selectScene)
		{
		case SetUp::NowScene::MenuSelect:

			firstMenu.Update();

			switch (firstMenu.IsCurrent())
			{
			case 0:
				if (KeyZ.down() || KeyEnter.down() || firstMenu.IsMouseOver() && MouseL.down())
				{
					selectScene = NowScene::AudioConfig;
					selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				}
				break;
			case 1:
				if (KeyZ.down() || KeyEnter.down() || firstMenu.IsMouseOver() && MouseL.down())
				{
					selectScene = NowScene::KeyConfig;
					selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				}
				break;
			case 2:
				if (KeyZ.down() || KeyEnter.down() || firstMenu.IsMouseOver() && MouseL.down())
				{
					selectScene = NowScene::Save;
					selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				}
				break;
			case 3:
				if (KeyZ.down() || KeyEnter.down() || firstMenu.IsMouseOver() && MouseL.down())
				{
					selectScene = NowScene::Load;
					selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				}
				break;
			case 4:
				if (KeyZ.down() || KeyEnter.down() || firstMenu.IsMouseOver() && MouseL.down())
				{
					selectScene = NowScene::Explanation;
					selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				}
				break;
			case 5:
				if (KeyZ.down() || KeyEnter.down() || firstMenu.IsMouseOver() && MouseL.down())
				{
					selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
					isChangeTitle = true;
					isEnable = false;
					isFirst = true;
					GameData::isSetUp = false;
				}
				break;
			case 6:
				if (KeyZ.down() || KeyEnter.down() || firstMenu.IsMouseOver() && MouseL.down())
				{
					selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
					System::Exit();
				}
				break;
			}
			break;
		case SetUp::NowScene::AudioConfig:
			//戻る
			if (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)))
			{
				selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				selectScene = NowScene::MenuSelect;
			}


			/*if (KeyS.down() || KeyDown.down())
			{
				gaugeID += 1;

				if (gaugeID > 2)
				{
					gaugeID = 0;
				}
			}

			if (KeyW.down() || KeyUp.down())
			{
				gaugeID -= 1;

				if (gaugeID < 0)
				{
					gaugeID = 2;
				}
			}

			for (auto& gauge : gauges)
			{
				gauge->CreateGauge();

				gauge->isSelect = false;
			}

			gauges[gaugeID]->isSelect = true;



			for (int i = 0; i < 3; i++)
			{

				if (gauges[i]->gaugeRect.mouseOver())
				{
					gaugeID = i;
				}
			}

			GameData::selectMainVolume = mainVolumeGauge.selectVolume;
			GameData::selectSEVolume = BGMVolumeGauge.selectVolume;
			GameData::selectBGMVolume = SEVolumeGauge.selectVolume;*/
			break;
		case SetUp::NowScene::KeyConfig:
			//戻る
			if (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)))
			{
				selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				selectScene = NowScene::MenuSelect;
			}
			break;
		case SetUp::NowScene::Save:
			//戻る
			if (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)))
			{
				selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				selectScene = NowScene::MenuSelect;
			}
			break;
		case SetUp::NowScene::Load:
			//戻る
			if (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)))
			{
				selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				selectScene = NowScene::MenuSelect;
			}
			break;
		case SetUp::NowScene::Explanation:
			//戻る
			if (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)))
			{
				selectAudio.playOneShot(GameData::MainVolume * GameData::SEVolume);
				selectScene = NowScene::MenuSelect;
			}
			break;
		default:
			break;
		}
	}
	
	/// @brief 設定の描画
	void Draw()const
	{
		//※要検証
	//Scene::SetBackground(Color(85, 85, 85,1));//シーンの色を半透明な灰色に設定
		Rect BackGround{ 0,0,Scene::Width(),Scene::Height() };
		BackGround.draw(ColorF(0.3, 0.7));

		bool dmy;

		switch (selectScene)
		{
		case SetUp::NowScene::MenuSelect:

			//メニュー描画
			firstMenu.InRectDraw(true);

			break;
		case SetUp::NowScene::AudioConfig:

			gaugeBackRect.draw().drawFrame(2, Palette::Black);
			//ゲージ描画,デバッグモードなら当たり判定も描画
			//mainVolumeGauge.GaugeDraw();
			//BGMVolumeGauge.GaugeDraw();
			//SEVolumeGauge.GaugeDraw();

			dmy = (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)));

			break;
		case SetUp::NowScene::KeyConfig:
			dmy = (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)));
			break;
		case SetUp::NowScene::Save:
			dmy = (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)));
			break;
		case SetUp::NowScene::Load:
			dmy = (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)));
			break;
		case SetUp::NowScene::Explanation:
			dmy = (SimpleGUI::ButtonAt(U"戻る", Vec2(Scene::Width() - 70, Scene::Height() - 30)));
			break;
		default:
			break;
		}
	}

	/// @brief 初期定義
	void InitialDefinition()
	{
		firstMenu.StringSet(firstMenuChar);
	}

	/// @brief 初期化
	void Initialize()
	{
		firstMenu.Initialize();

		isEnable = true;
		isFirst = false;
	}

	/// @brief 有効化切り替え
	void SetUpEnable()
	{
		//最初のループでは読まない,ESCAPEが押されたら設定画面を閉じて初期化フラグを立てる
		if (KeyEscape.down() && not isFirst)
		{
			isEnable = false;
			isFirst = true;
			GameData::isSetUp = false;
		}

		if (isEnable)
		{
			if (isFirst)
			{
				Initialize();
			}
			Update();
			Draw();
		}
	}

private:

};


