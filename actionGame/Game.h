#pragma once
/// ゲームシーン
class Game : public App::Scene
{
public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;

	/// @brief アニメーション切り替え管理
	void AnimationState();

	/// @brief キー入力管理
	void KeyPut();

	/// @brief 状態遷移管理
	enum class PlayerState
	{
		Wait,Jump,Walk,Sky,Dush,Down,GoDown,DownAttack,SkyAttack,Attack,Magic
	};

	//現在の状態
	PlayerState currentState = PlayerState::Wait;

	//MAP-----------------------------------------------------------------------------------
	//ステージ
	String stage1Tile{ U"Map/Test0001.tmx" };
	String stage2Tile{ U"Map/Stage1.tmx" };
	String stage3Tile{ U"Map/Stage2.tmx" };
	//マップ
	MapCreate map;
	//---------------------------------------------------------------------------------------

	//プレイヤー-------------------------------------------------------------------
	//画像
	Texture cirnoWaitTex{ U"image/cirnowait.png" };
	Texture cirnoWalkTex{ U"image/cirnowalk.png" };
	Texture cirnoJumpTex{ U"image/cirnojump.png" };
	Texture cirnoDushTex{ U"image/cirnodush.png" };
	Texture cirnoAttackTex{ U"image/cirno_attack.png" };
	Texture cirnoAttack2Tex{ U"image/cirno_attack2.png" };
	Texture cirnoDownTex{ U"image/cirno_down.png" };
	Texture cirnoSkyTex{ U"image/cirno_sky.png" };
	Texture attackTex{ U"image/attackeffect.png" };
	Texture iceTex{ U"image/ice_1.png" };

	//その他
	Texture slimeWaitTex{ U"image/slimeWait.png" };
	Texture slimeJumpTex{ U"image/slimeJump.png" };
	Texture goal_1Tex{ U"image/goal_1.png" };


	//音楽
	Audio stage1BGM{ Audio::Stream, U"sound/ファンタジー-異変-.mp3" , Loop::Yes };
	Audio stage2BGM{ Audio::Stream, U"sound/ファンタジー4-進展-.mp3" , Loop::Yes };
	Audio stage3BGM{ Audio::Stream, U"sound/MusicBox_03.mp3" , Loop::Yes };

	//アニメーション定義
	mutable AnimationMake slimeWaitAnimation{ slimeWaitTex,{2,0},0,2 ,800,{20,35} , {40,40} ,1};//スライム待機
	mutable AnimationMake slimeJumpAnimation{ slimeJumpTex,{3,0},0,3 ,800,{6,19} , {34,24} ,1};//スライムジャンプ


	mutable AnimationMake cirnoWaitAnimation{ cirnoWaitTex,{7,0},0,7	,1000	,{35,25},{40,100} ,0.9 };//チルノ待機
	mutable AnimationMake cirnoWalkAnimation{ cirnoWalkTex,{7,3},0,11	,3500	,{35,25},{40,100} ,0.9 };//チルノ歩き
	mutable AnimationMake cirnoDushAnimation{ cirnoDushTex,{7,3},3,4	,800	,{35,25},{40,100} ,0.9 };//チルノダッシュ
	//mutable AnimationMake cirnoGoDownAnimation{ cirnoJumpTex,{7,3},25,28	,400	,{35,25},{40,100} ,0.9 };
	mutable AnimationMake cirnoDownAnimation{ cirnoDownTex,{0,0},0	,{35,45},{40,80} ,0.9 };//チルノしゃがみ

	mutable AnimationMake cirnoAttackAnimation{ cirnoAttackTex, {7,3},0,4,400	,{35,25},{40,100} ,0.9};//チルノ立ち攻撃
	mutable AnimationMake cirnoDownAttackAnimation{ cirnoAttackTex, {7,3},8,12,400	,{35,45},{40,80} ,0.9};//チルノしゃがみ攻撃
	mutable AnimationMake cirnoSkyAttackAnimation{ cirnoAttackTex, {7,3},16,19,400	,{35,25},{40,85} ,0.9};//チルノ空中攻撃

	mutable AnimationMake cirnoJumpAnimation{ cirnoJumpTex,{7,3},20,22	,400	,{35,25},{40,85} ,0.9 };//チルノジャンプ
	mutable AnimationMake cirnoTurnJumpAnimation{ cirnoJumpTex,{7,3},9,15	,1200	,{35,25},{40,85} ,0.9 };//チルノ回りながらジャンプ
	mutable AnimationMake cirnoSkyAnimation{ cirnoSkyTex,{0,0},0,{35,25},{40,80} ,0.9};//チルノ滞空状態

	mutable AnimationMake nomalAttackAnimation{ attackTex,{4,1},0,5,300, {35,35},{45,45},0.5 };//通常攻撃エフェクト
	mutable AnimationMake iceAttackAnimation{ iceTex,{0,0},0, {0,20},{50,10} };//遠距離攻撃（氷）

	mutable AnimationMake goal_1Animation{ goal_1Tex,{4,3},0,19,1000, {15,0},{30,60},0.5 };//ゴール(白)


	//プレイヤーを定義
	mutable Status playerStatus{ 75,100,10,20.0,true };//プレイヤーステータス
	mutable Status slimeStatus{ 10,0,25,0,false };//スライムステータス
	//mutable Object player{ &cirnoWaitAnimation,playerStatus, Vec2{ 40,40 }, Vec2{ 0,0 },0.3 };//プレイヤー
	mutable Player player;
	mutable Object goal{ &goal_1Animation,playerStatus, Vec2{ 12600,400 }, Vec2{ 0,0 },0 };//ゴール
	mutable Object animationTest{ &goal_1Animation,playerStatus, Vec2{ 40,40 }, Vec2{ 0,0 },0.3 };//テスト用オブジェクト
	//通常攻撃用
	mutable Array<Object> attacks;//チルノ近距離攻撃
	//遠距離攻撃
	mutable Array<Object> ices;//チルノ遠距離攻撃
	//敵定義
	mutable Array<Object> slimes;//スライム

	//プレイヤーの移動量x/y
	Vec2 movePoint = { 0.2 ,10 };//横移動力/ジャンプ力
	const double maxWalkSpeed = 4;//歩行時最大速度
	const double maxDushSpeed = 6;//ダッシュ時最高速度
	double maxSpeed = maxWalkSpeed;//入れ物
	//敵の移動量
	double slimeMovePoint = 3;//スライム速度
	//HPバー
	Bar hitPointBar;
	//MPバー
	Bar magicPointBar{Palette::Lightblue};
	//------------------------------------------------------------------------------

	//デバッグ用--------------------------------------------------------------------
	//フォント
	Font font{ 20 };
	//デバッグフラグ
	bool isDebug = false;
	//------------------------------------------------------------------------------

private:
};



