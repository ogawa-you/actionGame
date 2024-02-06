#pragma once

//シーンの名前
enum class SceneName
{
	Title,
	Game,
	Goal
};

//共有データ
struct GameData
{
	enum class StageState
	{
		Stage1, Stage2, Stage3
	};
	StageState currentStage = StageState::Stage1;

	inline static bool isSetUp = false;

	//音量
	inline static double MainVolume = 1.0;  //メイン音量(すべての音量はこの値に依存する)
	inline static double BGMVolume = 0.1;   //BGMの音量(BGMの値はMainVolume ＊ BGMVolumeの計算結果依存)
	inline static double SEVolume = 0.1;//効果音の音量(効果音の値はMainVolume * EffectVolumeの計算結果依存)

	//設定画面で使う音量設定
	inline static int selectMainVolume = 5;   //主音量の大きさ
	inline static int selectBGMVolume = 5;  //BGMの大きさ
	inline static int selectSEVolume = 5; //効果音の大きさ

	InputGroup inputLeft = KeyLeft|KeyA;
	InputGroup inputRight = KeyRight|KeyD;
	InputGroup inputTop = KeyUp|KeyW;
	InputGroup inputBottom = KeyDown|KeyS|KeyLControl;
	InputGroup inputLClick = MouseL|KeyZ;
	InputGroup inputRClick = MouseR|KeyX;
	InputGroup inputJump = KeySpace|KeyUp|KeyW;
	InputGroup inputDush = KeyLShift;
	InputGroup inputFix = KeyZ|KeyEnter;
};

using App = SceneManager<SceneName, GameData>;
