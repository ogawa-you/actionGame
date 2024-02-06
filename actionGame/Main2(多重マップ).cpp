# include <Siv3D.hpp> // OpenSiv3D v0.4.2
# include"2Dlib042/2Dlib.h"

// 2020 多重スクロール、レイヤーによりスクロール速度を変更した
// マップの上に配置したオブジェクトも表示される
// ScreenInit関数を新規に作成して使用

void Main()
{

	ScreenInit(false);

	const Font font(20);
	TMXmap tiled(U"多重マップ/mylevel1csv.tmx");

		// ここでのイメージは、オブジェクトグループからデータを取り出し、スプライトを設定すること
	Array<Object_> testObj; //イメージ Array<GameObject> testObj;
	testObj.clear();
	for (Object_& obj : tiled.ObjectArray())
	{
		testObj.push_back(obj);
	}
	Vec2 test(testObj[0].x, testObj[0].y);// この行を●ブレークポイントにして実行して、
	// マウスオーバーで testObj にデータが入っていることを確認してください

	Vec2 playerPos(200, 200);

	bool HitMapView = true;

	while (System60::Update())
	{
		// キー入力でスクロール座標を変更
		int scrollAdd = KeyControl.pressed() ?	10:2;
		if (KeyLeft.pressed())	tiled.scrollPosition.x -= scrollAdd;
		if (KeyRight.pressed())	tiled.scrollPosition.x += scrollAdd;
		if (KeyUp.pressed())	tiled.scrollPosition.y -= scrollAdd;
		if (KeyDown.pressed())	tiled.scrollPosition.y += scrollAdd;
		if (KeyHome.down())
		{
			tiled.scrollPosition = { 0,0 };
			playerPos.set(200, 200);
		}

		// キー入力でプレイヤー移動量を決定
		Vec2 velocity(0, 0);
		if (KeyW.pressed())velocity.y -= 4;
		if (KeyS.pressed())velocity.y += 4;
		if (KeyA.pressed())velocity.x -= 4;
		if (KeyD.pressed())velocity.x += 4;

		if (KeyH.down())
		{
			HitMapView = !HitMapView;	// 当たり判定専用マップの表示フラグ設定
		}

		// 移動先を計算
		Vec2 move = playerPos + velocity;

		_ASSERT_EXPR(tiled.IsLayerID(3), L"レイヤー３は存在しません");//test 3をありえない数値にするとエラー表示される

		int hitLayer = 3;
		// move座標から幅２０高さ２０の矩形頂点にあるタイル番号がすべて同じなら、その値の取得
#ifdef _DEBUG
		int hitNumber = tiled.GetSame4numberorNot(Rect(move.asPoint(), 20, 20), hitLayer);

#endif // _DEBUG
		// move座標から幅２０高さ２０の矩形頂点にあるタイル番号の取得
		RectEdgeTileNumber hit = tiled.GetRect4Position(Rect(move.asPoint().x, move.asPoint().y, 20, 20), 3);

		// 矩形４点がすべて空白(0)なら移動できる
		if (hit.Edge4sameCheck(0))
			playerPos += velocity;

		Point gpLT = move.asPoint();					//左上
		Point gpRT = (move + Vec2(20, 0)).asPoint();	//右上
		Point gpLB = (move + Vec2(0, 20)).asPoint();	//左下
		Point gpRB = (move + Vec2(20, 20)).asPoint();	//右下

		// ワールド座標から表示座標に変換
		Point drawPos = tiled.WorldToLocalPosition(playerPos.asPoint());
		// マップ表示
		tiled.MapDraw(0, { 0,0 });			// レイヤー0、スクロールスピード（X,Y）=(0,0)
		tiled.MapDraw(1, { 0.36,0.36 });	// レイヤー1、スクロールスピード（X,Y）=(0.36,0.36)
		tiled.MapDraw(2);					// レイヤー2、スクロールスピード（X,Y）=(1,1)スクロールスピードは書かないと１倍になる
		// 当たり判定マップの表示、非表示
		if (HitMapView)
		{
			tiled.MapDraw(3);
		}
		//tiled.MapDrawAll();	// 全マップ表示

		// オブジェクトレイヤーにスプライトの位置情報が入っているので、
		// それを活用できる。それはTiledMapEditorで編集可能
		tiled.ObjectDraw();// アイテム、自キャラ出現位置、敵キャラ、ゴール・・・

		// タイル情報をTiledMapEditorで追加すると
		// ヒントメッセージやその他文字列情報やその他を複数追加できる
		// その情報を元にゲーム開発を効率よく作ることができる

		// タイル３番の情報が入っているか？
		Property testtile = tiled.GetProperty(3);
		/// タイル３番の情報が複数入っているか？
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
		Rect(drawPos, 20, 20).draw(Palette::Blue);
		// 各種パラメータ表示
		font(U"player", playerPos.asPoint()).draw(800, 10, Palette::Skyblue);
		font(U"scroll", tiled.scrollPosition).draw(800, 35, Palette::Antiquewhite);
		//font(U"playerHit(左上)",tiled.GetPixelPosTileNumber(playerPos			)).draw(800,65, Palette::Black);
		//font(U"playerHit(右上)",tiled.GetPixelPosTileNumber(playerPos + Vec2(20,0))).draw(800, 85, Palette::Black);
		//font(U"playerHit(左下)",tiled.GetPixelPosTileNumber(playerPos + Vec2(0,20))).draw(800,105, Palette::Black);
		//font(U"playerHit(右下)",tiled.GetPixelPosTileNumber(playerPos + Vec2(20, 20))).draw(800,125, Palette::Black);
		//hit = tiled.GetPixel4Position(Rect(playerPos.asPoint(), 20, 20), 3);
		// 移動先のタイルも見える
		font(U"playerHit(左上)", hit.leftTop).draw(800, 65, Palette::White);
		font(U"playerHit(右上)", hit.rightTop).draw(800, 85, Palette::White);
		font(U"playerHit(左下)", hit.leftBottom).draw(800, 105, Palette::White);
		font(U"playerHit(右下)", hit.rightBottom).draw(800, 125, Palette::White);
		font(U"操作 WSAD Home ↑←↓→ H(当たり判定の表示＆非表示) ").draw(700, 150, Palette::Yellow);
	}
}
/*
		font(U"Hello, Siv3D!🐣").drawAt(Window::Center(), Palette::Black);

		font(Cursor::Pos()).draw(20, 500, ColorF(0.6));

		textureCat.resized(80).draw(700, 500);

		Circle(Cursor::Pos(), 60).draw(ColorF(1, 0, 0, 0.5));
		*/
