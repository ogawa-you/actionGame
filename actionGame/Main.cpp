
///当たり判定の状態
enum class HitBoxState
{
	NONE,RIGHTHIT,LEFTHIT,TOPHIT,BOTTOMHIT
};

void Main()
{	
	//windowサイズを変更する
	ScreenInit(false);
	//エスケープキーを押しても終了しないようになる
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	//設定
	SetUp setUp;
	setUp.InitialDefinition();

	App manager;
	manager.add<Title>(SceneName::Title);
	manager.add<Game>(SceneName::Game);
	manager.add<Goal>(SceneName::Goal);

	// 開始位置
	manager.init(SceneName::Title);

	while (System60::Update())
	{
		if (not manager.update())
		{
			break;
		}

		if (KeyEscape.down()||GameData::isSetUp == true)
		{
			setUp.isEnable = true;
		}
		setUp.SetUpEnable();
		if (setUp.isChangeTitle)
		{
			manager.changeScene(SceneName::Title);
			setUp.isChangeTitle = false;
		}
	}
}


