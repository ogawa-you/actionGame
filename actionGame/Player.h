#pragma once

class Player:public Object
{
public:
	Player()
	{
		PlayerInitialize();
	};

	InputGroup inputLeft = KeyLeft | KeyA;
	InputGroup inputRight = KeyRight | KeyD;
	InputGroup inputTop = KeyUp | KeyW;
	InputGroup inputBottom = KeyDown | KeyS | KeyLControl;
	InputGroup inputLClick = MouseL | KeyZ;
	InputGroup inputRClick = MouseR | KeyX;
	InputGroup inputJump = KeySpace | KeyUp | KeyW;
	InputGroup inputDush = KeyLShift;
	InputGroup inputFix = KeyZ | KeyEnter;

	/// @brief 状態遷移管理
	enum class PlayerState
	{
		Wait, Jump, Walk, Sky, Dush, Down, GoDown, DownAttack, SkyAttack, Attack, Magic
	};
	//現在の状態
	PlayerState currentState = PlayerState::Wait;

	//画像------------------------------------------------------------------------------
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
	//-----------------------------------------------------------------------------------

	//アニメーション---------------------------------------------------------------------
	mutable AnimationMake cirnoWaitAnimation{ cirnoWaitTex,{7,0},0,7	,1000	,{35,25},{40,100} ,0.9 };//チルノ待機
	mutable AnimationMake cirnoWalkAnimation{ cirnoWalkTex,{7,3},0,11	,3500	,{35,25},{40,100} ,0.9 };//チルノ歩き
	mutable AnimationMake cirnoDushAnimation{ cirnoDushTex,{7,3},3,4	,800	,{35,25},{40,100} ,0.9 };//チルノダッシュ
	//mutable AnimationMake cirnoGoDownAnimation{ cirnoJumpTex,{7,3},25,28	,400	,{35,25},{40,100} ,0.9 };
	mutable AnimationMake cirnoDownAnimation{ cirnoDownTex,{0,0},0	,{35,45},{40,80} ,0.9 };//チルノしゃがみ

	mutable AnimationMake cirnoAttackAnimation{ cirnoAttackTex, {7,3},0,4,400	,{35,25},{40,100} ,0.9 };//チルノ立ち攻撃
	mutable AnimationMake cirnoDownAttackAnimation{ cirnoAttackTex, {7,3},8,12,400	,{35,45},{40,80} ,0.9 };//チルノしゃがみ攻撃
	mutable AnimationMake cirnoSkyAttackAnimation{ cirnoAttackTex, {7,3},16,19,400	,{35,25},{40,85} ,0.9 };//チルノ空中攻撃

	mutable AnimationMake cirnoJumpAnimation{ cirnoJumpTex,{7,3},20,22	,400	,{35,25},{40,85} ,0.9 };//チルノジャンプ
	mutable AnimationMake cirnoTurnJumpAnimation{ cirnoJumpTex,{7,3},9,15	,1200	,{35,25},{40,85} ,0.9 };//チルノ回りながらジャンプ
	mutable AnimationMake cirnoSkyAnimation{ cirnoSkyTex,{0,0},0,{35,25},{40,80} ,0.9 };//チルノ滞空状態

	mutable AnimationMake nomalAttackAnimation{ attackTex,{4,1},0,5,300, {35,35},{45,45},0.5 };//通常攻撃エフェクト
	mutable AnimationMake iceAttackAnimation{ iceTex,{0,0},0, {0,20},{50,10} };//遠距離攻撃（氷）
	//-----------------------------------------------------------------------------------

	//プレイヤーステータス
	mutable Status playerStatus{ 75,100,10,20.0,true };

	//細かい情報
	Vec2 movePoint = { 0.2 ,10 };//横移動力/ジャンプ力
	const double maxWalkSpeed = 4;//歩行時最大速度
	const double maxDushSpeed = 6;//ダッシュ時最高速度
	double maxSpeed = maxWalkSpeed;//入れ物


	//プレイヤー
	//mutable Object player{ &cirnoWaitAnimation,playerStatus, Vec2{ 40,40 }, Vec2{ 0,0 },0.3 };
	//通常攻撃用
	mutable Array<Object> attacks;//チルノ近距離攻撃
	//遠距離攻撃
	mutable Array<Object> ices;//チルノ遠距離攻撃

	void PlayerInitialize()
	{
		animation = &cirnoWaitAnimation;

		position = Vec2{ 40,40 };
		velocity = Vec2{ 0,0 };
		gravity = 0.3;

		status = { 75,100,10,20.0,true };

		hitbox = { position.asPoint(), size };

		move = position + velocity;

		afterHitBox = { move.asPoint(), size };

		//上下左右の短いライン
		topHitLine = { afterHitBox.top().begin + Point{5,1},afterHitBox.top().end - Point{5,-1} };
		bottomHitLine = { afterHitBox.bottom().begin - Point{5,1},afterHitBox.bottom().end + Point{5,-1} };
		rightHitLine = { afterHitBox.right().begin + Point{-1,5},afterHitBox.right().end - Point{1,5} };
		leftHitLine = { afterHitBox.left().begin - Point{-1,5},afterHitBox.left().end + Point{1,5} };

		stopwatch.start();
		invincibleTimer.set(invincibleTime);
	}

	void PlayerUpdate()
	{
		//座標や当たり判定系
		Update();

		//キー入力とPlayerStateの切り替え
		KeyPut();

		//アニメーションの切り替え
		AnimationState();

		//近距離攻撃
		for (auto& attack : attacks)
		{
			attack.deleteTimer.start();
		}

		//遠距離攻撃
		for (auto& ice : ices)
		{
			ice.Update();
			if (ice.velocity.x == 0)
			{
				ice.deleteTimer.start();
			}
		}

		//0.3秒経過で消滅
		attacks.remove_if([=](Object n)
			{
					return n.deleteTimer.ms() > nomalAttackAnimation.motionTime;
			});

		//プレイヤーと接触するか3秒経過で氷消滅
		ices.remove_if([=](Object n)
			{
					return (n.velocity.x == 0 && IsObjectHit(n.hitbox)) || n.deleteTimer.s() > 3;
			});

		//死亡時音切り替え等
		if (IsDead())
		{

		}
	}

	void PlayerDraw(TMXmap& tmxMap)
	{
		//プレイヤー描画
		Draw(GetDrawPos(tmxMap));

		//近距離攻撃描画
		for (auto& attack : attacks)
		{
			attack.Draw(attack.GetDrawPos(tmxMap));
		}

		//遠距離攻撃描画
		for (auto& ice : ices)
		{
			ice.Draw(ice.GetDrawPos(tmxMap));
		}
	}

	/// @brief アニメーション遷移
	void AnimationState()
	{
		//プレイヤーアニメーション処理---------------------------------------------------------------------------------------------------------------
		switch (currentState)
		{
		case PlayerState::Wait:
			ChangeAnimation(&cirnoWaitAnimation);
			break;
		case PlayerState::Walk:
			ChangeAnimation(&cirnoWalkAnimation);
			break;
		case PlayerState::Dush:
			ChangeAnimation(&cirnoDushAnimation);
			break;
		case PlayerState::Down:
			ChangeAnimation(&cirnoDownAnimation);
			break;
		case PlayerState::Jump:
			ChangeAnimation(&cirnoJumpAnimation);
			//一パターン終わったら
			if (IsOnePatternAnimation())
			{
				//Skyの状態へ遷移する
				currentState = PlayerState::Sky;
				//遷移した次のフレームまで待てないのでここで変える
				ChangeAnimation(&cirnoSkyAnimation);
			}
			break;
		case PlayerState::Sky:
			ChangeAnimation(&cirnoSkyAnimation);
			break;
		case PlayerState::Attack:
			ChangeAnimation(&cirnoAttackAnimation);
			//一パターン終わったら
			if (IsOnePatternAnimation())
			{
				//Waitの状態へ遷移する
				currentState = PlayerState::Wait;
				//遷移した次のフレームまで待てないのでここで変える
				ChangeAnimation(&cirnoWaitAnimation);
			}
			break;
		case PlayerState::DownAttack:
			ChangeAnimation(&cirnoDownAttackAnimation);
			//一パターン終わったら
			if (IsOnePatternAnimation())
			{
				//Downの状態へ遷移する
				currentState = PlayerState::Down;
				//遷移した次のフレームまで待てないのでここで変える
				ChangeAnimation(&cirnoDownAnimation);
			}
			break;
		case PlayerState::SkyAttack:
			ChangeAnimation(&cirnoSkyAttackAnimation);
			//一パターン終わったら
			if (IsOnePatternAnimation())
			{
				//Skyの状態へ遷移する
				currentState = PlayerState::Sky;
				//遷移した次のフレームまで待てないのでここで変える
				ChangeAnimation(&cirnoSkyAnimation);
			}
			break;
		default:
			break;
		}
	}

	void KeyPut()
	{
		//アニメーション切り替え----------------------------------------------------------------------------------------------------
		//キー入力----------------------------------------------------------------------------------------------------
		auto controller = XInput(0);
		double stick = 0.5;
		//攻撃モーション
		bool AttackMotion = currentState == PlayerState::Attack || currentState == PlayerState::DownAttack;

		//キー入力
		bool Left = inputLeft.pressed() || controller.buttonLeft.pressed() || controller.leftThumbX < -stick;
		bool Right = inputRight.pressed() || controller.buttonRight.pressed() || controller.leftThumbX > stick;
		bool Dush = inputDush.pressed() || controller.buttonX.pressed();
		bool Jump = inputJump.down() || controller.buttonA.down() || controller.buttonUp.down() || controller.leftThumbY > stick;
		bool meleeAttack = inputLClick.down() || controller.buttonY.down();
		bool longRangeAttack = inputRClick.down() || controller.buttonB.down();
		bool Down = inputBottom.pressed() || controller.buttonDown.pressed() || controller.leftThumbY < -stick;

		//左右を押すと向き反転
		if (Left)
		{
			isMirror = true;
		}
		if (Right)
		{
			isMirror = false;
		}

		//待機
		if (not AttackMotion && isOnFloor)
		{
			//待機に切り替わる(デフォルト)
			currentState = PlayerState::Wait;
		}

		//空中
		if (currentState != PlayerState::SkyAttack && currentState != PlayerState::Jump && isOnFloor == false)
		{
			//空中攻撃モーションでないかつジャンプモーション中じゃなければ空中に切り替わる(デフォルト)
			currentState = PlayerState::Sky;
		}

		//左右移動
		if (currentState != PlayerState::Attack && currentState != PlayerState::Down)
		{
			//Dushが押されていたら最高速を変更
			(Dush) ? maxSpeed = maxDushSpeed : maxSpeed = maxWalkSpeed;

			//右が押されていたら
			if (Right && (inputLeft.pressed() == false))
			{
				//velocityXを移動値分加算
				velocity.x += movePoint.x;

				//最高速以上切り捨て
				if (velocity.x > maxSpeed && velocity.x > 0)
				{
					velocity.x = maxSpeed;
				}
			}

			//左が押されていたら
			if (Left && (inputRight.pressed() == false))
			{
				//velocityXを移動値分加算
				velocity.x -= movePoint.x;

				//最高速以上切り捨て
				if (velocity.x < -maxSpeed && velocity.x < 0)
				{
					velocity.x = -maxSpeed;
				}
			}

			//左右入力(モーション)※移動値等とは条件が異なる
			if (not AttackMotion && isOnFloor)
			{
				//左右入力で歩きモーションに切り替わる
				if (Right || Left)
				{
					currentState = PlayerState::Walk;

					//Shiftが押されてればダッシュに切り替わる
					if (Dush)
					{
						currentState = PlayerState::Dush;
					}
				}

				//左右同時押しで待機モーションへ
				if ((inputRight.pressed()) && (inputLeft.pressed()))
				{
					//待機に切り替わる(デフォルト)
					currentState = PlayerState::Wait;
				}
			}
		}

		//しゃがみ
		if (Down && isOnFloor == true)
		{
			currentState = PlayerState::Down;

			velocity.x = 0;
		}

		//ジャンプ
		if (Jump && isOnFloor)
		{
			velocity.y = -movePoint.y;
			isOnFloor = false;

			currentState = PlayerState::Jump;
		}


		//地上攻撃
		if (meleeAttack && isOnFloor)
		{
			//下入力してたらしゃがみ攻撃
			currentState = currentState == PlayerState::Down ? PlayerState::DownAttack : PlayerState::Attack;;

			attacks << Object{ &nomalAttackAnimation,{ isMirror ? position + Vec2{-45,15} : position + Vec2{45,15} },{0,0},0 };
		}

		//空中攻撃
		if (meleeAttack && isOnFloor == false)
		{
			currentState = PlayerState::SkyAttack;

			attacks << Object{ &nomalAttackAnimation,{ isMirror ? position + Vec2{-45,50} : position + Vec2{45,50} },{0,0},0 };
		}

		//遠距離攻撃
		if (longRangeAttack && status.magicpoint >= 10)
		{
			ices << Object{ &iceAttackAnimation,{ position + Vec2{10,15} },{isMirror ? -8 : 8,0},0 };

			status.MpDecrease(10);
		}

		///地上で移動してない時のみ(WalkとDush以外)ゆっくり減速する
		//何も入力してない待機状態でvelocityXがゆっくり0になるまで減少
		//if (isOnFloor && (currentState != PlayerState::Walk || currentState != PlayerState::Dush))
		if (currentState == PlayerState::Wait || currentState == PlayerState::Down || currentState == PlayerState::Attack || currentState == PlayerState::DownAttack)
		{
			if (0 < velocity.x)velocity.x -= movePoint.x;
			if (velocity.x < 0)velocity.x += movePoint.x;
			//0.2以内なら0へ
			if (Abs(velocity.x) < 0.2)velocity.x = 0;
		}

		if ((inputBottom.down() || controller.buttonDown.down() || controller.leftThumbY < -stick) && isOnFloor == true)
		{
			position.y += 20;
		}
		//--------------------------------------------------------------------------------------------------------------------------
	}

private:

};


