# include <Siv3D.hpp> // OpenSiv3D v0.6.9
# include"2Dlib042/2Dlib.h"

void Main()
{
	//Window::Resize(1280, 720);
	//Scene::SetBackground(ColorF(0.8, 0.9, 1.0));
	ScreenInit(false);

	const Font font(20);
	//TMXmap tiled(U"mylevel1_stage/mylevel1csv.tmx");	//1
	//TMXmap tiled(U"mylevel1csv.tmx");	//2
	//TMXmap tiled(U"Stage/Test/TestStage.tmx");	//3
	
//	TMXmap tiled(U"test0624.tmx");	//4 // これだけは当たり判定などのサンプル動作する
	TMXmap tiled(U"tiled/ttt_ani.tmx");	//4 // これだけは当たり判定などのサンプル動作する
	//TMXmap tiled(U"ttt_ani.tmx");	//4 // これだけは当たり判定などのサンプル動作する

	Vec2 playerPos(200, 300);

	enum class TileStatus
	{
		NoRedBlick,NoPurPleBlick,WhiteBlick,Tile0_Move,FireTile,End
	};
	TileStatus moveCheck = TileStatus::NoRedBlick;

	while (System60::Update())
	{
		// キー入力でスクロール座標を変更
		if (KeyLeft.pressed())	tiled.scrollPosition.x -= 1;
		if (KeyRight.pressed())	tiled.scrollPosition.x += 1;
		if (KeyUp.pressed())	tiled.scrollPosition.y -= 1;
		if (KeyDown.pressed())	tiled.scrollPosition.y += 1;

		// キー入力でプレイヤー移動量を決定
		Vec2 velocity(0, 0);
		if (KeyW.pressed())velocity.y -= 4;
		if (KeyS.pressed())velocity.y += 4;
		if (KeyA.pressed())velocity.x -= 4;
		if (KeyD.pressed())velocity.x += 4;

		// 移動先を計算
		Vec2 move = playerPos + velocity;


		TileStatus tileStatus;
		std::map<TileStatus, String> tileMessage;
		tileMessage[TileStatus::NoRedBlick] = U"４点がすべて赤レンガ(24)以外なら移動できる";	// 24
		tileMessage[TileStatus::NoPurPleBlick] = U"４点がすべて紫レンガ(1)以外なら移動できる"; // 1
		tileMessage[TileStatus::WhiteBlick] = U"４点がすべて白レンガ(23)以外なら移動できる";//23
		tileMessage[TileStatus::Tile0_Move] = U"４点がすべて空白(0)なら移動できる";// 0
		tileMessage[TileStatus::FireTile] = U"４点どれかが炎なら終了(0)";// 31 32 33


		if (KeySpace.down())
		{
			moveCheck = (TileStatus)(((int)moveCheck + 1) % (int)TileStatus::End);
		}
		auto hit = tiled.GetRect4Position(Rect(move.asPoint(), 20, 20), 0);

		switch (moveCheck)
		{
		case TileStatus::NoRedBlick:
			// ４点がすべて赤レンガ(24)以外なら移動できる
			if (hit.Edge4DifferentNumber(24))
				playerPos += velocity;
			break;
		case TileStatus::NoPurPleBlick:
			// ４点がすべて紫レンガ(1)以外なら移動できる
			if (hit.Edge4DifferentNumber(1))
				playerPos += velocity;
			break;
		case TileStatus::WhiteBlick:
			// ４点がすべて白レンガ(23)以外なら移動できる
			if (hit.Edge4DifferentNumber(23))
				playerPos += velocity;
			break;
		case TileStatus::Tile0_Move:
			// ４点がすべて空白(0)なら移動できる
			if (hit.Edge4sameCheck(0))
				playerPos += velocity;
			break;
		case TileStatus::FireTile:
			// ここで書けないの仕方なく
			break;
		default:
			break;
		}

		if (moveCheck == TileStatus::FireTile)
		{
			bool mustExit = false;
			// ４点どれかが炎なら終了(0)
			for (auto& i : hit.edge)
			{
				if (31 - 1 < i && i < 33 + 1)
				{
					mustExit = true;
				}
			}
			if (mustExit)
				break;
		}


		//// ４点がすべて空白(1)なら移動できる
		//if (tiled.GetSame4numberorNot(Rect(move.asPoint(), 20, 20),0)==1)
		//	playerPos += velocity;

		// ワールド座標から表示座標に変換
		Point drawPos = tiled.WorldToLocalPosition(playerPos.asPoint());
		// マップ表示
		//tiled.MapDraw(0);
		//tiled.MapDraw(1);
		//tiled.MapDraw(2);
		//tiled.MapDraw(3);

		tiled.MapDrawAll();

		// オブジェクトレイヤーにスプライトの位置情報が入っているので、
		// それを活用できる。それはTiledMapEditorで編集可能
		tiled.ObjectDraw();// アイテム、自キャラ出現位置、敵キャラ、ゴール・・・

		// タイル情報をTiledMapEditorで追加すると
		// ヒントメッセージやその他文字列情報やその他を複数追加できる
		// その情報を元にゲーム開発を効率よく作ることができる

		// タイル３番の情報が入っているか？
		Property testtile = tiled.GetProperty(3);
		// タイル３番の情報が複数入っているか？
		Array<Property> testtilesArray = tiled.GetPropertyArray(3);

		if (testtilesArray.size() != 0)
		{
			Property prpty = testtilesArray[0];
			Type type = prpty.type;
			//下の行でブレークポイント（赤丸）を入れてマウスオーバーで上記変数を確認
		}
		// タイル200番の情報が入っているか？
		auto testtilesErr = tiled.GetProperty(200);

		//tex.draw();
		// プレイヤー表示
		Rect(drawPos, 20, 20).draw(Palette::White);
		// 各種パラメータ表示
		font(U"player", playerPos.asPoint()).draw(800, 10, Palette::Yellow);
		font(U"scroll", tiled.scrollPosition).draw(800, 35, Palette::Yellow);
		// 移動先のタイルも見える
		font(U"playerHit(左上)", hit.leftTop).draw(800, 65, Palette::Blue);
		font(U" playerHit(右上)", hit.rightTop).draw(800, 85, Palette::Red);
		font(U"playerHit(左下)", hit.leftBottom).draw(800, 105, Palette::Blue);
		font(U" playerHit(右下)", hit.rightBottom).draw(800, 125, Palette::Red);
		font(tileMessage[moveCheck],U" [[[スペースキーで移動チェックが変わる]]]").draw(0, 0, Palette::White);
		int64 ms64 = TMX::timer.ms64();
		int ms = TMX::timer.ms();
		//auto msStr=Parse<String>(ms);
		font(ms64).draw(0, 300, Palette::Black);
	}
}
/*
		font(U"Hello, Siv3D!🐣").drawAt(Window::Center(), Palette::Black);

		font(Cursor::Pos()).draw(20, 500, ColorF(0.6));

		textureCat.resized(80).draw(700, 500);

		Circle(Cursor::Pos(), 60).draw(ColorF(1, 0, 0, 0.5));
		*/
