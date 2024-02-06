#pragma once

class AI
{
public:
	AI() {};

	enum ActionTile
	{
		RJump = 3391,
		LJump = 3392,
		RTurn = 3393,
		LTurn = 3394,
	};

	/*static void GetGuidVelocity(Vec2 player,Vec2 enemey)
	{
		return Vec2();
	}*/

	static void ChangeMirror(Object& object)
	{
		//velocityが+なら反転フラグtrue
		if (object.velocity.x < 0)object.isMirror = true;
		else if (0 <= object.velocity.x)object.isMirror = false;
	}

	/// @brief スライム独自の処理まとめ
	/// @param slime スライムの配列
	/// @param player プレイヤー
	/// @param Map ステージ情報
	static void SlimeAI(Object& slime, Object& player, MapCreate& Map)
	{
		//スライムの移動速度
		int slimeSpeed = 3;
		int slimeJumpPower = 6;

		//プレイヤーから1000以内になったら動き出す
		if (abs(player.GetDrawPos(Map.tiled).x - slime.GetDrawPos(Map.tiled).x) > 1000)
		{
			slime.velocity = { 0,0 };
		}	

		//スライムの処理
		slime.Update();

		//常に動く(右へ)
		if (slime.velocity.x == 0 && slime.isOnFloor)
		{
			slime.velocity.x = slimeSpeed;
		}

		//移動値によって反転フラグ切り替え
		ChangeMirror(slime);
		//スライムの画像は向きが逆
		slime.isMirror = !slime.isMirror;

		//移動指示用オブジェクトと当たった際の処理
		for (auto& obj : Map.object)
		{
			//ワールド座標補正した四角
			//Rect rect{ Map.tiled.WorldToLocalPosition(Point((int)obj.x,(int)obj.y)),int(obj.width),int(obj.height) };
			Rect rect{ (Point((int)obj.x,(int)obj.y)),int(obj.width),int(obj.height) };

			//当たってなかったらコンティニュー
			if (not slime.hitbox.intersects(rect))
			{
				continue;
			}

			switch (obj.gid)
			{
			case ActionTile::RJump:
				//右向きに移動してる||床に接していたら
				if (slime.velocity.x > 0 && slime.isOnFloor)
				{
					//ジャンプ
					slime.velocity.y = -slimeJumpPower;
				}	
				break;
			case ActionTile::LJump:
				//左向きに移動してる||床に接していたら
				if (slime.velocity.x < 0 && slime.isOnFloor)
				{
					//ジャンプ
					slime.velocity.y = -slimeJumpPower;
				}	
				break;
			case ActionTile::LTurn:
				//反転
				if (slime.velocity.x > 0)
				{
					slime.velocity.x *= -1;
				}	
				break;
			case ActionTile::RTurn:
				//反転
				if (slime.velocity.x < 0)
				{
					slime.velocity.x *= -1;
				}	
				break;
			default:
				break;
			}
		}

		//スライムとマップの当たり判定
		slime.MapHit(Map.tiled, Map.airBlock, Map.mapChipSize, Map.hitLayer);

		//プレイヤーとの当たり判定
		if (slime.IsObjectHit(player.hitbox))
		{
			player.GetDamage(slime.status.strength);
			//slime.velocity.x *= -1;
		}
		//player.PositionCorrection(slime.hitbox);

	}
private:

};

