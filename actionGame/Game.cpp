#include "Game.h"

Game::Game(const InitData& init)
	: IScene{ init }
{
	// Print したメッセージを消去
	ClearPrint();

	//バーの最大値定義
	hitPointBar.Initialize(player.status.maxHitpoint);
	magicPointBar.Initialize(player.status.maxMagicpoint);

	animationTest.position = { Scene::Center() };
	animationTest.ChangeAnimation(&goal_1Animation);

	switch (getData().currentStage)
	{
	case GameData::StageState::Stage1:
		// 背景の色を設定する
		Scene::SetBackground(Color{ 255,248,220 });
		map.MapDataSet(stage1Tile, 187, 1, 20, 32);
		player.position.y = 500;

		for (auto& obj : map.object)
		{
			if (obj.gid == 3404)
			{
				slimes << Object{ &slimeWaitAnimation,slimeStatus, Vec2{ obj.x,obj.y }, Vec2{ 0,0 },0.3 };
			}
		}
		
		break;
	case GameData::StageState::Stage2:
		// 背景の色を設定する
		Scene::SetBackground(Color{ 167,159,183 });
		map.MapDataSet(stage2Tile, 183, 12, 20, 32);
		//map.MapDataSet(stage2Tile, 183, 12, 20, 32);
		player.position.y = 622;
		break;
	case GameData::StageState::Stage3:
		// 背景の色を設定する
		Scene::SetBackground(Color{ 255,248,220 });
		map.MapDataSet(stage3Tile, 183, 8, 20, 32);
		player.position.y = 622;
		break;
	default:
		break;
	}
}

//void Game::AnimationState()
//{
//	//プレイヤーアニメーション処理---------------------------------------------------------------------------------------------------------------
//	switch (currentState)
//	{
//	case Game::PlayerState::Wait:
//		player.ChangeAnimation(&cirnoWaitAnimation);
//		break;
//	case Game::PlayerState::Walk:
//		player.ChangeAnimation(&cirnoWalkAnimation);
//		break;
//	case Game::PlayerState::Dush:
//		player.ChangeAnimation(&cirnoDushAnimation);
//		break;
//	case Game::PlayerState::Down:
//		player.ChangeAnimation(&cirnoDownAnimation);
//		break;
//	case Game::PlayerState::Jump:
//		player.ChangeAnimation(&cirnoJumpAnimation);
//		//一パターン終わったら
//		if (player.IsOnePatternAnimation())
//		{
//			//Skyの状態へ遷移する
//			currentState = PlayerState::Sky;
//			//遷移した次のフレームまで待てないのでここで変える
//			player.ChangeAnimation(&cirnoSkyAnimation);
//		}
//		break;
//	case Game::PlayerState::Sky:
//		player.ChangeAnimation(&cirnoSkyAnimation);
//		break;	
//	case Game::PlayerState::Attack:
//		player.ChangeAnimation(&cirnoAttackAnimation);
//		//一パターン終わったら
//		if (player.IsOnePatternAnimation())
//		{
//			//Waitの状態へ遷移する
//			currentState = PlayerState::Wait;
//			//遷移した次のフレームまで待てないのでここで変える
//			player.ChangeAnimation(&cirnoWaitAnimation);
//		}	
//		break;
//	case Game::PlayerState::DownAttack:
//		player.ChangeAnimation(&cirnoDownAttackAnimation);
//		//一パターン終わったら
//		if (player.IsOnePatternAnimation())
//		{
//			//Downの状態へ遷移する
//			currentState = PlayerState::Down;
//			//遷移した次のフレームまで待てないのでここで変える
//			player.ChangeAnimation(&cirnoDownAnimation);
//		}	
//		break;
//	case Game::PlayerState::SkyAttack:
//		player.ChangeAnimation(&cirnoSkyAttackAnimation);
//		//一パターン終わったら
//		if (player.IsOnePatternAnimation())
//		{
//			//Skyの状態へ遷移する
//			currentState = PlayerState::Sky;
//			//遷移した次のフレームまで待てないのでここで変える
//			player.ChangeAnimation(&cirnoSkyAnimation);
//		}	
//		break;
//	default:
//		break;
//	}
//	//--------------------------------------------------------------------------------------------------------------------------
//
//	//アニメーション切り替え----------------------------------------------------------------------------------------------------
//	//※ジャンプは条件が特別なのでキー入力で切り替え(モーションを切り替えたときに接地フラグを操作するため)
//	auto controller = XInput(0);
//	double stick = 0.5;
//
//	//床についていたら
//	if (player.isOnFloor)
//	{
//		//攻撃
//		if ((getData().inputLClick.down() || controller.buttonY.down()))
//		{
//			//下入力してたらしゃがみ攻撃
//			currentState = (getData().inputBottom.pressed() || controller.buttonDown.pressed() || controller.leftThumbY < -stick)
//				? PlayerState::DownAttack : PlayerState::Attack;;
//		}
//
//		//攻撃モーション
//		bool Attack = currentState == PlayerState::Attack || currentState == PlayerState::DownAttack;
//
//		//攻撃モーションでなければ
//		if (not Attack)
//		{
//			//待機に切り替わる(デフォルト)
//			currentState = PlayerState::Wait;
//
//			//左右入力で歩きモーションに切り替わる
//			if ((getData().inputRight.pressed() || controller.buttonRight.pressed() || controller.leftThumbX > stick) || (getData().inputLeft.pressed() || controller.buttonLeft.pressed() || controller.leftThumbX < -stick))
//			{
//				currentState = PlayerState::Walk;
//
//				//Shiftが押されてればダッシュに切り替わる
//				if ((getData().inputDush.pressed())|| controller.buttonX.pressed())
//				{
//					currentState = PlayerState::Dush;
//				}
//			}
//
//			//左右同時押しで待機モーションへ
//			if ((getData().inputRight.pressed()) && (getData().inputLeft.pressed()))
//			{
//				//待機に切り替わる(デフォルト)
//				currentState = PlayerState::Wait;
//			}
//
//			//下入力でしゃがみに切り替わる
//			if ((getData().inputBottom.pressed() || controller.buttonDown.pressed() || controller.leftThumbY < -stick) && player.isOnFloor == true)
//			{
//				currentState = PlayerState::Down;
//			}
//		}
//	}
//	//空中にいたら
//	else
//	{
//		//空中攻撃
//		if ((getData().inputLClick.down() || controller.buttonY.down()))
//		{
//			currentState = PlayerState::SkyAttack;
//		}
//
//		//空中攻撃モーションでないかつジャンプモーション中じゃなければ空中に切り替わる(デフォルト)
//		if (currentState != PlayerState::SkyAttack && currentState != PlayerState::Jump)
//		{
//			currentState = PlayerState::Sky;
//		}
//	}
//	//--------------------------------------------------------------------------------------------------------------------------
//}
//
//void Game::KeyPut()
//{
//	//キー入力----------------------------------------------------------------------------------------------------
//
//	auto controller = XInput(0);
//	double stick = 0.5;
//
//	//左右を押すと向き反転
//	if (getData().inputLeft.pressed() || controller.buttonLeft.pressed() || controller.leftThumbX < -stick)
//	{
//		player.isMirror = true;
//	}
//	if (getData().inputRight.pressed() || controller.buttonRight.pressed() || controller.leftThumbX > stick)
//	{
//		player.isMirror = false;
//	}
//
//	//左右移動
//	if (currentState != PlayerState::Attack && currentState != PlayerState::Down && currentState != PlayerState::GoDown)
//	{	
//		//ShiftLが押されていたら最高速を変更
//		(getData().inputDush.pressed() || controller.buttonX.pressed()) ? maxSpeed = maxDushSpeed : maxSpeed = maxWalkSpeed;
//		
//		//右が押されていたら
//		if ((getData().inputRight.pressed() || controller.buttonRight.pressed() || controller.leftThumbX > stick) && (getData().inputLeft.pressed() == false))
//		{
//			//velocityXを移動値分加算
//			player.velocity.x += movePoint.x;
//
//			//最高速以上切り捨て
//			if (player.velocity.x > maxSpeed && player.velocity.x > 0)
//			{
//				player.velocity.x = maxSpeed;
//			}
//		}
//
//		//左が押されていたら
//		if ((getData().inputLeft.pressed() || controller.buttonLeft.pressed() || controller.leftThumbX < -stick) && (getData().inputRight.pressed() == false))
//		{
//			//velocityXを移動値分加算
//			player.velocity.x -= movePoint.x;
//
//			//最高速以上切り捨て
//			if (player.velocity.x < -maxSpeed && player.velocity.x < 0)
//			{
//				player.velocity.x = -maxSpeed;
//			}
//		}
//	}
//
//	//地上で移動してない時のみ(WaitとAttack)ゆっくり減速する
//	//何も入力してない待機状態でvelocityXがゆっくり0になるまで減少
//	if (currentState == PlayerState::Wait || currentState == PlayerState::Down || currentState == PlayerState::Attack || currentState == PlayerState::DownAttack)
//	{
//		if (0 < player.velocity.x)player.velocity.x -= movePoint.x;
//		if (player.velocity.x < 0)player.velocity.x += movePoint.x;
//		//0.2以内なら0へ
//		if (Abs(player.velocity.x) < 0.2)player.velocity.x = 0;
//	}
//
//
//	//ジャンプ
//	if ((getData().inputJump.down() || controller.buttonA.down() || controller.buttonUp.down() || controller.leftThumbY > stick) && player.isOnFloor)
//	{
//		player.velocity.y = -movePoint.y;
//		player.isOnFloor = false;
//
//		currentState = PlayerState::Jump;
//	}
//
//	//下入力でしゃがみ
//	if ((getData().inputBottom.pressed() || controller.buttonDown.pressed() || controller.leftThumbY < -stick)&& player.isOnFloor == true)
//	{
//		player.position.y += 20;
//
//		player.velocity.x = 0;
//	}
//
//	//攻撃
//	if ((getData().inputLClick.down() || controller.buttonY.down()) && player.isOnFloor)
//	{
//		attacks << Object{ &nomalAttackAnimation,{ player.isMirror ? player.position + Vec2{-45,15} : player.position + Vec2{45,15} },{0,0},0 };
//	}
//
//	//攻撃
//	if ((getData().inputLClick.down() || controller.buttonY.down()) && player.isOnFloor == false)
//	{
//		attacks << Object{ &nomalAttackAnimation,{ player.isMirror ? player.position + Vec2{-45,50} : player.position + Vec2{45,50} },{0,0},0 };
//	}
//
//	//遠距離攻撃
//	if ((getData().inputRClick.down() || controller.buttonB.down()) && player.status.magicpoint >= 10)
//	{
//		ices << Object{ &iceAttackAnimation,{ player.position + Vec2{10,15} },{player.isMirror ? -8 : 8,0},0 };
//
//		player.status.MpDecrease(10);
//	}
//
//#ifdef _DEBUG
//	//デバッグ切り替え
//	if (KeyEnter.down())
//	{
//		isDebug = !isDebug;
//	}
//#endif // _DEBUG
//	//--------------------------------------------------------------------------------------------------------------------------
//}

void Game::update()
{
	//Printしたメッセージを消去
	ClearPrint();

	#ifdef _DEBUG
	//デバッグ切り替え
	if (KeyEnter.down())
	{
		isDebug = !isDebug;
	}
#endif // _DEBUG


	//マップスクロール
	map.MapScroll(player.position, { 0,12160 - Scene::Width()/2});


	//プレイヤー関連-------------------------------------------------------------------------------
	//プレイヤー処理
	player.PlayerUpdate();
	//プレイヤーのHPが0になった際の処理
	if (player.IsDead())
	{
		//仮
		changeScene(SceneName::Game);
	}
	//HP,MPバー処理
	hitPointBar.Update(player.status.hitpoint);
	magicPointBar.Update(player.status.magicpoint);
	//------------------------------------------------------------------------------------------------

	//敵関連------------------------------------------------------------------------------------------
	//スライム
	for (auto& slime : slimes)
	{
		AI::SlimeAI(slime,player, map);
	}
	//スライム死亡(hitpoint<=0)
	slimes.remove_if([=](Object n)
		{
			return n.IsDead();
		});
	//------------------------------------------------------------------------------------------------

	//当たり判定-----------------------------------------------------------------------------------------------------------------
	//スライムと通常攻撃の当たり判定
	for (auto& slime : slimes)
	{
		for (auto& attack : player.attacks)
		{
			if (slime.IsObjectHit(attack.hitbox))
			{
				//スライムのHP - プレイヤーの攻撃力
				slime.GetDamage(player.status.strength);
			}
		}
	}
	//スライムと氷の当たり判定
	for (auto& slime : slimes)
	{
		for (int i = player.ices.size() - 1; i >= 0; i--)
		{
			if (slime.IsObjectHit(player.ices[i].hitbox))
			{
				//スライムのHP - プレイヤーの攻撃力
				slime.GetDamage(player.status.strength);
				//氷を消滅
				player.ices.remove_at(i);
			}
		}
		
	}

	//ゴールとの当たり判定
	if (player.IsObjectHit(goal.hitbox))
	{
		changeScene(SceneName::Goal);
	}
	/*if (player.IsHitBlock(map.tiled, map.goalBlock, map.mapChipSize, map.hitLayer))
	{
		changeScene(SceneName::Goal);
	}*/

	//奈落との当たり判定
	if (player.IsHitBlock(map.tiled, 163, map.mapChipSize, map.hitLayer))
	{
		player.status.HpDecrease(player.status.maxHitpoint);
	}

	//マップとの当たり判定
	player.MapHit(map.tiled, map.airBlock, map.mapChipSize, map.hitLayer);
	for (auto& ice : player.ices)
	{
		ice.MapHit(map.tiled, map.airBlock, map.mapChipSize, map.hitLayer);
	}
	//-----------------------------------------------------------------------------------------------------------
}

void Game::draw() const
{
	//マップ描画
	//オブジェクト描画
	//BGM
	switch (getData().currentStage)
	{
	case GameData::StageState::Stage1:
		//BGM再生(ループ)
		stage1BGM.play();
		//当たり判定のマップ描画
		map.tiled.MapDraw(0, { 0, 0 });
		map.tiled.MapDraw(1);
		//敵描画
		for (auto& slime : slimes)
		{
			slime.Draw(slime.GetDrawPos(map.tiled));
		}

		//プレイヤー画像描画
		player.PlayerDraw(map.tiled);

		goal.Draw(goal.GetDrawPos(map.tiled));

		//マップ描画(草)
		map.tiled.MapDraw(2);
		break;
	case GameData::StageState::Stage2:
		//BGM再生(ループ)
		stage2BGM.play();

		//マップ描画
		map.tiled.MapDraw(0, { 0,0 });//最奥
		map.tiled.MapDraw(1, { 0,0 });
		map.tiled.MapDraw(2, { 0,0 });
		map.tiled.MapDraw(3, { 0.1,0 });
		map.tiled.MapDraw(4, { 0.2,0 });
		map.tiled.MapDraw(5, { 0.5,0 });
		map.tiled.MapDraw(6, { 0.6,0 });
		map.tiled.MapDraw(7, { 0.7,0 });
		map.tiled.MapDraw(8, { 0.8,0 });
		map.tiled.MapDraw(9, { 0.8,0 });
		map.tiled.MapDraw(10, { 0.9,0 });
		map.tiled.MapDraw(11, { 0.9,0 });
		map.tiled.MapDraw(12, { 1,0 });//当たり判定
	
		//プレイヤー画像描画
		player.PlayerDraw(map.tiled);
		//天井描画
		map.tiled.MapDraw(13, { 1,0 });
	
		break;
	case GameData::StageState::Stage3:
		//BGM再生(ループ)
		stage3BGM.play();

		//マップ描画
		map.tiled.MapDraw(0, { 0,0 });//最奥
		map.tiled.MapDraw(1, { 0.05,0 });
		map.tiled.MapDraw(2, { 0.1,0 });
		map.tiled.MapDraw(3, { 0.2,0 });
		map.tiled.MapDraw(4, { 0.4,0 });
		map.tiled.MapDraw(5, { 0.6,0 });
		map.tiled.MapDraw(6, { 0.8,0 });
		map.tiled.MapDraw(7, { 1,0 });
		map.tiled.MapDraw(8, { 1,0 });//当たり判定
	
		//プレイヤー画像描画
		player.PlayerDraw(map.tiled);

		break;
	default:
		break;
	}

	//バー描画
	hitPointBar.DrawHex(Rect(10, 10, 250, 20));
	magicPointBar.DrawHex(Rect(10, 30, 250, 20));
	
	//デバッグ用
	if (isDebug)
	{
		//情報
		player.DebugDraw(player.GetDrawPos(map.tiled));
		goal.DebugDraw(goal.GetDrawPos(map.tiled));
		for (auto& slime : slimes)
		{
			slime.DebugDraw(slime.GetDrawPos(map.tiled));
		}
		for (auto& attack : attacks)
		{
			attack.DebugDraw(attack.GetDrawPos(map.tiled));
			Print << U"deleteTimer" << attack.deleteTimer.ms();
		}
		for (auto& ice : ices)
		{
			ice.DebugDraw(ice.GetDrawPos(map.tiled));
		}

		//アニメーションテスト描画
		animationTest.Draw(animationTest.GetDrawPos(map.tiled));

		animationTest.DebugDraw(animationTest.GetDrawPos(map.tiled));

		//オブジェクト描画
		map.tiled.ObjectDraw();

		//諸情報描画
		Print << U"エンターキーでデバッグモード切り替え";
		Print << U"座標" << player.position;
		Print << U"移動値" << player.velocity;
		Print << U"ストップウォッチ " << slimes[0].stopwatch.ms();
		Print << U"1パターン " << slimes[0].IsOnePatternAnimation();
		Print << U"MP " << player.status.magicpoint;
		Print << U"SlimeHP " << slimes[0].status.hitpoint;
		Print << U"timer" << player.invincibleTimer;
	}
}

