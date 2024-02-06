#pragma once
class Object
{
public:
	Object(){};

	/// @brief オブジェクト定義(重力=0,,ステータス情報なし)
	/// @param _animation アニメーション
	/// @param _position スポーン座標
	/// @param _velocity 初期移動値
	/// @param _size 画像サイズ
	Object(AnimationMake* _animation ,Vec2 _position, Vec2 _velocity)
	{
		animation = _animation;
		size = animation->charaSize;
		position = _position;
		velocity = _velocity;
		gravity = 0;

		hitbox = { position.asPoint(), size};

		move = position + velocity;

		afterHitBox = { move.asPoint(), size };

		//上下左右の短いライン
		topHitLine = { afterHitBox.top().begin + Point{5,1},afterHitBox.top().end - Point{5,-1} };
		bottomHitLine = { afterHitBox.bottom().begin - Point{5,1},afterHitBox.bottom().end + Point{5,-1} };
		rightHitLine = { afterHitBox.right().begin + Point{-1,5},afterHitBox.right().end - Point{1,5} };
		leftHitLine = { afterHitBox.left().begin - Point{-1,5},afterHitBox.left().end + Point{1,5} };

		stopwatch.start();
		invincibleTimer.set(invincibleTime);
	};

	/// @brief オブジェクト定義(重力変更あり,ステータス情報なし)
	/// @param _animation アニメーション
	/// @param _position スポーン座標
	/// @param _velocity 初期移動値
	/// @param _size 画像サイズ
	/// @param _gravity 重力
	Object(AnimationMake* _animation, Vec2 _position, Vec2 _velocity, double _gravity)
	{
		animation = _animation;
		size = animation->charaSize;
		position = _position;
		velocity = _velocity;
		gravity = _gravity;

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
	};

	
	/// @brief オブジェクト定義(重力変更あり,ステータス情報あり)
	/// @param _animation アニメーション
	/// @param _status ステータス
	/// @param _position スポーン座標
	/// @param _velocity 初期移動値
	/// @param _size 画像サイズ
	/// @param _gravity 重力
	Object(AnimationMake* _animation,Status _status, Vec2 _position, Vec2 _velocity, double _gravity)
	{
		animation = _animation;
		status = _status;
		size = animation->charaSize;
		position = _position;
		velocity = _velocity;
		gravity = _gravity;

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
	};

	//オブジェクトのステータス
	Status status;
	//描画するアニメーション(動く場合は切り替える)
	AnimationMake* animation;
	//座標
	Vec2 position;
	//移動値
	Vec2 velocity;
	//画像の横幅と縦幅
	Size size;
	//四角の当たり判定
	Rect hitbox;
	//移動先の座標
	Vec2 move;
	//移動先の当たり判定(線)
	Rect afterHitBox;
	Line topHitLine;
	Line bottomHitLine;
	Line rightHitLine;
	Line leftHitLine;
	//重力
	double gravity;
	//無敵時間の長さ
	Duration invincibleTime = 1.5s;
	//時間管理タイマー
	//アニメーション管理用
	Stopwatch stopwatch;
	//何かを消すとき用
	Stopwatch deleteTimer;
	//無敵時間用
	Stopwatch invincibleTimer;
	//着地してるかフラグ
	bool isOnFloor = false;
	//反転してるかフラグ
	bool isMirror = false;

	/// @brief 毎フレーム更新処理
	void Update()
	{
		//もしマップとオブジェクトが当たってたらvelocityは0になっている
		//座標に移動値を加算
		position += velocity;

		//オブジェクトのサイズをアニメーションサイズから更新
		size = animation->charaSize;

		//座標更新
		hitbox = { position.asPoint(), size};

		move = position + velocity;

		afterHitBox = { move.asPoint(), size };

		//上下左右の短いライン
		topHitLine = { afterHitBox.top().begin + Point{5,1},afterHitBox.top().end - Point{5,-1} };
		bottomHitLine = { afterHitBox.bottom().begin - Point{5,1},afterHitBox.bottom().end + Point{5,-1} };
		rightHitLine = { afterHitBox.right().begin + Point{-1,5},afterHitBox.right().end - Point{1,5} };
		leftHitLine = { afterHitBox.left().begin - Point{-1,5},afterHitBox.left().end + Point{1,5} };

		//yに重力加算、15以上にならない
		if (isOnFloor == false && velocity.y < 15)
		{
			velocity.y += gravity;
		}

		//ステータス更新
		status.Update();
	}

	/// @brief HPが0ならtrue
	/// @return 
	bool IsDead()
	{
		return status.hitpoint <= 0;
	}

	/// @brief 描画座標に変換
	/// @param tmxMap マップ
	/// @return 座標
	Point GetDrawPos(TMXmap& tmxMap)
	{
		return tmxMap.WorldToLocalPosition(position.asPoint());
	}

	/// @brief 特定のブロックに当たったか判定
	/// @param tmxMap マップ
	/// @param hitBlock 判定したいブロックのタイル番号
	/// @param MapChipSize 1マスのサイズ
	/// @param layer 当たり判定のレイヤー
	/// @return true
	bool IsHitBlock(TMXmap& tmxMap, int hitBlock, int MapChipSize,int layer = 0)
	{
		//移動先の情報
		//プレイヤーの当たり判定の四隅
		auto hit = tmxMap.GetRect4Position(afterHitBox, layer);

		//4点のどこかがHitBlockと当たってたらtrueを返す
		if(hit.rightTop == hitBlock || hit.rightBottom == hitBlock || hit.leftTop == hitBlock || hit.leftBottom == hitBlock)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/// @brief 特定のブロックに当たってないか判定
	/// @param tmxMap マップ
	/// @param notHitBlock 判定したいブロックのタイル番号
	/// @param MapChipSize 1マスのサイズ
	/// @param layer 当たり判定のレイヤー
	/// @return true
	bool IsNotHitBlock(TMXmap& tmxMap, int notHitBlock, int MapChipSize, int layer = 0)
	{
		return not IsHitBlock(tmxMap,notHitBlock,MapChipSize,layer);
	}

	/// @brief マップとオブジェクトの当たり判定
	/// @param tmxMap マップ
	/// @param NotHitBlock 透過タイル番号
	/// @param MapChipSize 1マスのサイズ
	/// @param layer 当たり判定のレイヤー
	void MapHit(TMXmap &tmxMap,int NotHitBlock,int MapChipSize,int layer = 0)
	{
		//移動先の情報
		//プレイヤーの当たり判定の四隅
		auto hit = tmxMap.GetRect4Position(afterHitBox,layer);

		int startPosX = (afterHitBox.top().begin.asPoint().x);
		int startPosY = (afterHitBox.top().begin.asPoint().y);

		//天井の当たり判定二点、1/3地点と2/3地点
		auto ceilingLeft = tmxMap.GetRect4Position(Rect((afterHitBox.w * 1 / 3) + startPosX, afterHitBox.y, size), layer).leftTop;
		auto ceilingRight = tmxMap.GetRect4Position(Rect((afterHitBox.w * 2 / 3) + startPosX, afterHitBox.y, size), layer).leftTop;

		//床の当たり判定二点、1/4地点と3/4地点
		auto floorLeft = tmxMap.GetRect4Position(Rect((afterHitBox.w * 2 / 7) + startPosX, afterHitBox.y, size), layer).leftBottom;
		auto floorRight = tmxMap.GetRect4Position(Rect((afterHitBox.w * 3 / 4) + startPosX, afterHitBox.y, size), layer).leftBottom;

		//左右の当たり判定二点、1/4地点と3/4地点
		auto LeftTop		= tmxMap.GetRect4Position(Rect( afterHitBox.x, startPosY + (afterHitBox.h * 1 / 3),  size), layer).leftTop;
		auto LeftBottom		= tmxMap.GetRect4Position(Rect( afterHitBox.x, startPosY + (afterHitBox.h * 3 / 4),  size), layer).leftTop;
		auto RightTop		= tmxMap.GetRect4Position(Rect( afterHitBox.x, startPosY + (afterHitBox.h * 1 / 3),  size), layer).rightTop;
		auto RightBottom	= tmxMap.GetRect4Position(Rect( afterHitBox.x, startPosY + (afterHitBox.h * 3 / 4),  size), layer).rightTop;

		//左右の当たり判定追加二点、1/2地点
		auto AddLeft  = tmxMap.GetRect4Position(Rect( afterHitBox.x, startPosY + (afterHitBox.h * 1 / 2),  size), layer).leftTop;
		auto AddRight = tmxMap.GetRect4Position(Rect( afterHitBox.x, startPosY + (afterHitBox.h * 1 / 2),  size), layer).rightTop;

		//プレイヤーの下のタイルが空中か床か判別するための変数
		//プレイヤーの下の当たり判定からy+1した地点のタイル番号
		auto hitCheckLeft = tmxMap.GetRect4Position(Rect((afterHitBox.w * 1 / 4) + startPosX, afterHitBox.y + 1, size), layer).leftBottom;
		auto hitCheckRight = tmxMap.GetRect4Position(Rect((afterHitBox.w * 3 / 4) + startPosX, afterHitBox.y + 1, size), layer).leftBottom;

		//座標が0より小さかったら0に
		//マップ外での座標補正
		if (move.y < 0)
		{
			velocity.y = 0;
		}
		if (move.x < 0)
		{
			velocity.x = 0;
		}

		//プレイヤーの下の当たり判定の下が空気ならフラグを折る
		if (hitCheckLeft == NotHitBlock && hitCheckRight == NotHitBlock)
		{
			isOnFloor = false;
		}

		//床と当たった処理
		if ((floorLeft != NotHitBlock || floorRight != NotHitBlock) && velocity.y >= 0)
		{
			//移動値を0にして床踏んでるフラグを立てる
			velocity.y = 0;
			isOnFloor = true;

			//座標補正、ギリギリ浮く
			//触れている床の座標を求めて、そこからプレイヤー画像の高さ-1減算
			position.y = ((((int)(move.y + size.y) / MapChipSize) * MapChipSize) - size.y - 1);
		}
		//天井と当たった処理
		if ((ceilingLeft != NotHitBlock || ceilingRight != NotHitBlock) && velocity.y < 0)
		{
			//移動値を0にする
			velocity.y = 0;

			//座標補正
			//触れている天井の座標を求めて、そこからプレイヤー画像の縦幅-1減算
			position.y = ((((int)(move.y) / MapChipSize) * MapChipSize) + MapChipSize);
		}
		//壁(左)と当たった処理
		if ((LeftTop != NotHitBlock || LeftBottom != NotHitBlock || AddLeft != NotHitBlock)&&velocity.x < 0)
		{
			//移動値を0にする
			velocity.x = 0;

			//座標補正
			//触れている壁の座標を求めて、そこからプレイヤー画像の横幅-1減算
			position.x = ((((int)(move.x) / MapChipSize) * MapChipSize) + MapChipSize);
		}	
		//壁(右)と当たった処理
		if ((RightTop != NotHitBlock || RightBottom != NotHitBlock || AddRight != NotHitBlock)&&velocity.x > 0)
		{
			//移動値を0にする
			velocity.x = 0;

			//座標補正
			//触れている壁の座標を求めて、そこからプレイヤー画像の横幅-1減算
			position.x = ((((int)(move.x + size.x) / MapChipSize) * MapChipSize) - size.x - 1);
		}		
	}

	/// @brief オブジェクト同士が当たってるか判定
	/// @param _hitBox 当たってる四角
	/// @return true
	bool IsObjectHit(Rect _hitBox)
	{	
		return hitbox.intersects(_hitBox);
	}

	/// @brief オブジェクト同士が当たった際のめり込まないようにする座標補正
	/// @param _hitBox 当たってる四角
	void PositionCorrection(Rect _hitBox)
	{

		//下に触れたら移動値を0にして座標補正
		if (bottomHitLine.intersects(_hitBox))
		{
			position.y = _hitBox.topY() - size.y;
			velocity.y = 0;

			return;
		}

		//上に触れたら移動値を0にして座標補正
		if (topHitLine.intersects(_hitBox))
		{
			position.y = _hitBox.bottomY();
			velocity.y = 0;
		}

		//右に触れたら移動値を0にして座標補正
		if (rightHitLine.intersects(_hitBox))
		{
			position.x = _hitBox.leftX() - size.x;
			velocity.x = 0;
		}

		//左に触れたら移動値を0にして座標補正
		if (leftHitLine.intersects(_hitBox))
		{
			position.x = _hitBox.rightX();
			velocity.x = 0;
		}
	}

	

	/// @brief アニメーション切り替え
	/// @param _animation 切り替えるアニメーション
	void ChangeAnimation(AnimationMake* _animation)
	{
		//切り替えて初期化
		if (animation != _animation)
		{
			animation = _animation;
			animation->AnimationInitialize();
			stopwatch.restart();
		}
	}


	/// @brief 1パターン分のアニメーションが終わったらtrueを返す
	/// @return true
	bool IsOnePatternAnimation()
	{
		//return animation->OnePatternAnimation(stopwatddch);
		return animation->IsOnePatternAnimation(stopwatch) ;
	}

	/// @brief ダメージを受けた際の処理
	/// @param damage 受けるダメージ量
	void GetDamage(int damage)
	{
		//無敵状態でないなら
		if (not IsInvincible())
		{
			//HP減少
			status.HpDecrease(damage);

			//無敵処理
			Invincible();
		}
	}

	/// @brief 今が無敵常態かどうか
	/// @return 
	bool IsInvincible()
	{
		return invincibleTimer < invincibleTime;
	}

	/// @brief 無敵に入った時の処理
	void Invincible()
	{
		//タイマー再スタート
		invincibleTimer.restart();
	}

	/// @brief オブジェクトの描画
	/// @param drawPos 
	void Draw(Point drawPos)
	{
		//無敵なら色が変わる
		//画像描画
		if (IsInvincible())
		{
			//被ダメ時
			animation->AnimationDraw(drawPos, stopwatch, Palette::Red, isMirror);
		}
		else
		{
			//通常時
			animation->AnimationDraw(drawPos, stopwatch, Palette::White, isMirror);
		}
	}

	/// @brief デバッグ情報の描画
	/// @param drawPos 
	void DebugDraw(Point drawPos)
	{
		//描画座標補正したヒットボックス
		Rect drawHitbox = { drawPos,size };

		//本体の当たり判定描画、今は使用はしてない
		drawHitbox.drawFrame(1, Palette::Red);

		//上下左右のライン(短め)
		Line{ drawHitbox.top().begin + Point{5,1},drawHitbox.top().end - Point{5,-1} }.draw(4, Palette::Limegreen);
		Line{ drawHitbox.bottom().begin - Point{5,1},drawHitbox.bottom().end + Point{5,-1} }.draw(4, Palette::Purple);
		Line{ drawHitbox.right().begin + Point{-1,5},drawHitbox.right().end - Point{1,5} }.draw(4, Palette::Yellow);
		Line{ drawHitbox.left().begin - Point{-1,5},drawHitbox.left().end + Point{1,5} }.draw(4, Palette::Pink);

		//判定してる点の描画
		//Circle{ drawHitbox.top().begin,5 }	.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });
		//Circle{ drawHitbox.top().end,5 }		.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });
		//Circle{ drawHitbox.bottom().end,5 }	.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });
		//Circle{ drawHitbox.bottom().begin,5 }.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });

		Circle{ drawHitbox.x, (drawHitbox.top().begin.asPoint().y) + (drawHitbox.h * 1 / 2),  5 }.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });
		Circle{ drawHitbox.x + drawHitbox.w, (drawHitbox.top().begin.asPoint().y) + (drawHitbox.h * 1 / 2),  5 }.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });
		Circle{ drawHitbox.x, (drawHitbox.top().begin.asPoint().y) + (drawHitbox.h * 1 / 4),  5 }.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });
		Circle{ drawHitbox.x + drawHitbox.w, (drawHitbox.top().begin.asPoint().y) + (drawHitbox.h * 1 / 4),  5 }.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });
		Circle{ drawHitbox.x, (drawHitbox.top().begin.asPoint().y) + (drawHitbox.h * 3 / 4),  5 }.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });
		Circle{ drawHitbox.x + drawHitbox.w, (drawHitbox.top().begin.asPoint().y) + (drawHitbox.h * 3 / 4),  5 }.draw(HSV{ Scene::Time() * 320, 0.6, 1.0 });

		Circle{ (drawHitbox.w * 1 / 3) + (drawHitbox.top().begin.asPoint().x), drawHitbox.y,3 }	.draw(Palette::Black);
		Circle{ (drawHitbox.w * 2 / 3) + (drawHitbox.top().begin.asPoint().x), drawHitbox.y,3 }	.draw(Palette::Black);
		Circle{ (drawHitbox.w * 2 / 7) + (drawHitbox.top().begin.asPoint().x), drawHitbox.y + drawHitbox.h,3 }	.draw(Palette::Black);
		Circle{ (drawHitbox.w * 3 / 4) + (drawHitbox.top().begin.asPoint().x), drawHitbox.y + drawHitbox.h,3 }	.draw(Palette::Black);
	}
private:

};
