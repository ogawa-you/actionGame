#pragma once
class MapCreate
{
public:
	MapCreate() {};

	/// @brief 最初の定義
	/// @param _mapData TMXファイルのパス
	/// @param _goalBlock 触れたらゴールするブロックの番号
	/// @param _hitLayer 当たり判定のレイヤーの番号
	/// @param _airBlock 透過できるブロックの番号
	/// @param _mapChipSize マップチップのサイズ
	/// @param _isScroll スクロールするかどうか
	MapCreate(String _mapData, int _goalBlock, int _hitLayer, int _airBlock = 20 , int _mapChipSize = 32 , bool _isScroll = true)
	{
		//ファイルを読み込み
		tiled.LoadTMXfile(_mapData);

		goalBlock = _goalBlock;

		hitLayer = _hitLayer;

		airBlock = _airBlock;

		mapChipSize = _mapChipSize;

		isScroll = _isScroll;

		//オブジェクト定義
		object.clear();
		for (Object_& obj : tiled.ObjectArray())
		{
			object << obj;
		}
		Vec2 test(object[0].x, object[0].y);// この行を●ブレークポイントにして実行して、
	};

	//マップ
	mutable TMXmap tiled;
	//オブジェクト
	Array<Object_> object;
	//マップチップ一辺のサイズ
	int mapChipSize;
	//マップチップの通過可能な空気ブロック
	int airBlock;
	//マップチップのゴールブロック
	int goalBlock;
	//マップチップの当たり判定のレイヤー
	int hitLayer;
	//画面がスクロールするか
	bool isScroll;

	/// @brief 情報定義
	void MapDataSet(String _mapData, int _goalBlock, int _hitLayer, int _airBlock = 20, int _mapChipSize = 32, bool _isScroll = true)
	{
		//ファイルを読み込み
		tiled.LoadTMXfile(_mapData);

		goalBlock = _goalBlock;

		hitLayer = _hitLayer;

		airBlock = _airBlock;

		mapChipSize = _mapChipSize;

		isScroll = _isScroll;

		//オブジェクト定義
		object.clear();
		
		for (Object_& obj : tiled.ObjectArray())
		{
			object << obj;
		}
		Vec2 test(object[0].x, object[0].y);// この行を●ブレークポイントにして実行して、 	
	}

	void MapScroll(Vec2 position,Point screenEnd)
	{
		if (isScroll)
		{
			//マップがプレイヤーを真ん中に見据えて移動する
			tiled.scrollPosition.x = (int)position.x - Scene::Width() / 2;

			//マップが画面外を表示しないようにする
			if (tiled.scrollPosition.x < screenEnd.x)
			{
				tiled.scrollPosition.x = screenEnd.x;
			}
			if (tiled.scrollPosition.x > screenEnd.y)
			{
				tiled.scrollPosition.x = screenEnd.y;
			}
		}
	}

private:

};
