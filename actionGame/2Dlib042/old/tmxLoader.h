#pragma once

// 2019 9/2

class TSX
{
public:
	String version, tiledversion, name;
	int tilewidth, tileheight, tilecount, columns;
	String image_source;
	int image_width, image_height;
//	<tileset version = "1.2" tiledversion = "1.2.4" name = "13_bank(CUnet)(scale)(x4.000000)" tilewidth = "64" tileheight = "64" tilecount = "33" columns = "11">
//		<image source = "13_bank(CUnet)(scale)(x4.000000).png" width = "704" height = "192" / >
//		< / tileset>
	void Set(XMLReader& tmx)
	{
		version = tmx.attribute(U"version").value();
		tiledversion = tmx.attribute(U"tiledversion").value();
		name = tmx.attribute(U"name").value();
		tilewidth = ParseInt<int>(tmx.attribute(U"tilewidth").value());
		tileheight = ParseInt<int>(tmx.attribute(U"tileheight").value());
		tilecount = ParseInt<int>(tmx.attribute(U"tilecount").value());
		columns = ParseInt<int>(tmx.attribute(U"columns").value());
		// image
		image_width = ParseInt<int>(tmx.firstChild().attribute(U"width").value());
		image_height = ParseInt<int>(tmx.firstChild().attribute(U"height").value());
		image_source = tmx.firstChild().attribute(U"source").value();
		tmx.close();
	}
};

class Map
{
public:
	float version;
	String tiledversion, orientation, renderorder;
	int width, height, tilewidth, tileheight, infinite, nextlayerid, nextobjectid;
	Map()
	{
		version = 0.0f;
		tiledversion= orientation= renderorder=U"";
		width = height = tilewidth = tileheight = infinite = nextlayerid = nextobjectid = 0;
	}

	void Set(XMLReader& tmx)
	{
		version = ParseFloat<float>(tmx.attribute(U"version").value());
		tiledversion = tmx.attribute(U"tiledversion").value();
		orientation = tmx.attribute(U"orientation").value();
		renderorder = tmx.attribute(U"renderorder").value();
		width = ParseInt<int>(tmx.attribute(U"width").value());
		height = ParseInt<int>(tmx.attribute(U"height").value());
		tilewidth = ParseInt<int>(tmx.attribute(U"tilewidth").value());
		tileheight = ParseInt<int>(tmx.attribute(U"tileheight").value());
		infinite = ParseInt<int>(tmx.attribute(U"infinite").value());
		nextlayerid = ParseInt<int>(tmx.attribute(U"nextlayerid").value());
		nextobjectid = ParseInt<int>(tmx.attribute(U"nextobjectid").value());
	}
};
class Tileset
{
public:
	int firstgid;
	String source;
	String& tmxMapfile;
	Tileset() :tmxMapfile(source) {
		firstgid = 0;
		source = U"";
	}
	void Set(XMLReader& tmx)
	{
		XMLElement tileset = tmx.firstChild();
		this->firstgid = ParseInt<int>(tileset.attribute(U"firstgid").value());
		this->source = tileset.attribute(U"source").value();
	}
};
class Layer
{
public:
	int id, width, height;
	String name;
	Layer()
	{
		id = width = height;
		name = U"";
	}
	void Set(XMLReader& tmx)
	{
		XMLElement layer = tmx.firstChild().nextSibling();
		id = ParseInt<int>(layer.attribute(U"id").value());
		name = layer.attribute(U"name").value();
		width = ParseInt<int>(layer.attribute(U"width").value());
		height = ParseInt<int>(layer.attribute(U"height").value());
	}
};
class Data
{
public:
	String encoding;
	Data()
	{
		encoding = U"";
	}
	XMLElement Set(XMLReader& tmx)
	{
		XMLElement data = tmx.firstChild().nextSibling().firstChild();
		encoding = data.attribute(U"encoding").value();
		return data;
	}
};
// 絶対に正の数であまりを出す　引数：割られる数、割る数　　戻り値：余り（正の数）
inline int UnsignedMod(int a,int b)
{
	assert( b>=0 && "割る数は正の数のみ" );
	int mod = a % b;
	if (mod < 0)mod += b;
	return mod;
}
// 絶対に正の数であまりを出す　引数：割られる数、割る数　　戻り値：余り（正の数）
inline int UMod(int a,int b)
{
	return UnsignedMod(a,b);
}

class TMXmap
{
public:
	TSX tsx;
	Map map;
	Tileset tileset;
	Layer layer;
	Data data;
	String texAsset;
	Array<Array<int>> tileMap;
	Vec2 scrollPosition;
	Point screenArraySize;
	// タイルの画像ファイル名
	String TextureFilename()
	{
		return tsx.image_source;
	}
	int MapWidth()
	{
		return layer.width;
	}
	int MapHeight()
	{
		return layer.height;
	}
	int TileWidth()
	{
		return tsx.tilewidth;
	}
	int TileHeight()
	{
		return tsx.tileheight;
	}
	int operator()(int iy,int ix)
	{
		return tileMap[iy][ix];
	}
/*	Texture& AllTileTexture()
	{
		return TextureAsset(TextureFilename());//	エラー(アクティブ)	E0461	非 const への参照の初期値は左辺値である必要があります	tmxAndSprite040	C : \Users\voldo\source\repos\tmxAndSprite040\tmxAndSprite040\tmxLoader.h	164
	}*/
	TMXmap()
	{}
	TMXmap(String tmxFile)
	{
		LoadTMXfile(tmxFile);
	}
	void LoadTMXfile(String tmxFile)
	{
		assert(SIV3D_VERSION >= U"0.4.0" && "OpenSiv3D 0.4.0以上を使ってください"); 
		XMLReader tmx(tmxFile);
		//map.Set(tmx.attribute(U"version").value());
		map.Set(tmx);
		tileset.Set(tmx);
		layer.Set(tmx);
		dataElement = data.Set(tmx);
		assert(data.encoding == U"csv" && "データはcsv形式のみです。設定変更して保存し直してください");
		ArrayDataSet();
		tmx.close();
		XMLReader tsxfile(tileset.source);
		tsx.Set(tsxfile);
		texAsset = TextureFilename();
		//TextureAsset::Register(texAsset, texAsset);
		TextureAsset::Register(TextureFilename(), TextureFilename());
		scrollPosition = { 0,0 };
		screenArraySize = ToMapArrayPos(Window::ClientSize());
	}
	// タイル番号　表示座標
	//void TileDraw(int tileNumber, int x, int y)
	//{
	//	if (tileNumber == 0)return;
	//	int i = tileNumber - 1;
	//	//TextureAsset(TextureFilename())
	//	//	((i% tsx.columns) *TileWidth(),i/tsx.columns*TileHeight(),TileWidth(),TileHeight()).draw(x,y);

	//	TextureAsset(TextureFilename())
	//		((i % tsx.columns) * TileWidth(), i / tsx.columns * TileHeight(),
	//			TileWidth(), TileHeight()).draw(x, y);
	//}
	void TileDrawTest(int tileNumber, int x, int y)
	{
		TextureAsset( TextureFilename() )
			( 0,0,TileWidth(), TileHeight() ).draw(x, y);
	}
	// タイル番号　表示座標
	void TileDraw(int tileNumber, int x, int y)
	{
		if (tileNumber == 0 || tileNumber == -1 )return;
		TextureAsset(TextureFilename())
			(GetTileRect(tileNumber)).draw(x, y);
	}
	// タイル番号の画像の取り出す矩形情報の取得
	Rect GetTileRect(int tileNumber)
	{
		int tn = tileNumber-1;
		return { tn % tsx.columns * TileWidth(), tn / tsx.columns * TileHeight(), TileWidth(), TileHeight() };
	}
	// マップの範囲外か？
	bool IsOutOfMap(Point arrayPos)
	{
		return (arrayPos.x < 0 || arrayPos.x > layer.width - 1 || arrayPos.y < 0 || arrayPos.y > layer.height -1);
	}
	// タイル番号取得する。引数：マップ配列座標　戻り値：タイル番号、ただし範囲外の場合は－１
	int GetTileNumber(Point arrayPos)
	{
		if (IsOutOfMap(arrayPos))
			return -1;
		return tileMap[arrayPos.y][arrayPos.x];
	}
	// ワールド座標の取得 引数：ピクセル座標　　戻り値：ワールド座標
	Point LocalToWorldPosition(int px,int py)
	{
		return scrollPosition.asPoint()+Point(px,py);
	}
	// ワールド座標の取得 引数：標示座標　　戻り値：ワールド座標
	Point LocalToWorldPosition(Point pixelPos)
	{
		return (scrollPosition.asPoint() + pixelPos);
	}
	// ローカル座標（標示座標）の取得 引数：ピクセル座標　　戻り値：ワールド座標
	Point WorldToLocalPosition(Point pixelPos)
	{
		return pixelPos -scrollPosition.asPoint();
	}
	// 円筒スクロール座標変換 引数：ワールド座標　戻り値：マップの配列座標
	Point RollArrayPosition(Point pos)
	{
		return {
			UMod(pos.x, MapWidth() * TileWidth()) / TileWidth(),
			UMod(pos.y, MapHeight() * TileHeight()) / TileHeight()
		};
	}


	// タイル番号の取得 引数：x,y ピクセル座標  戻り値：タイル番号
	int GetPixelTileNumber(int px,int py)
	{
		return GetPixelPosTileNumber({px,py});
	}
	// タイル番号の取得 引数 ピクセル座標　　　戻り値：ピクセル座標
	int GetPixelPosTileNumber(Point pixelPos)
	{
		return GetTileNumber(ToMapArrayPos(pixelPos));
	}
	// マップ配列座標のピクセルでの矩形情報の計算　引数：マップ配列座標　戻り値：ピクセル単位の矩形情報
	Rect GetRect(Point arrayPos)
	{
		return Rect(ToPixelPos(ToMapArrayPos(arrayPos)), TileWidth(), TileHeight());
	}
	// マップ配列座標のピクセルでの表示座標での矩形情報の計算　引数：マップ配列座標　戻り値：ピクセル単位の矩形情報
	Rect GetLocalRect(Point arrayPos)
	{
		Point local(ToPixelPos(ToMapArrayPos(arrayPos) - scrollPosition.asPoint()));
		return Rect(local, TileWidth(), TileHeight());
	}

	void MapTileDrawTest(int x, int y)
	{
		int tileNumber = tileMap[y][x];
		if (tileNumber == 0)return;
		int tn = tileNumber - 1;
		Point home = scrollPosition.asPoint();
		//Point drawPos=leftTop+
		//TextureAsset(TextureFilename())
		//	(GetTileRect()).draw(x, y);
	}
	// スクロールの左上の微妙なずれ分の計算
	Point Shift()
	{
		Point _shift(-UMod(scrollPosition.asPoint().x , TileWidth()), -UMod(scrollPosition.asPoint().y , TileHeight()));
		return _shift;
	}

	// マップ配列座標からピクセル座標にする　引数：マップ配列座標　　戻り値：ピクセル座標
	Point ToPixelPos(Point mapPos)
	{
		return Point(mapPos.x * TileWidth(), mapPos.y * TileHeight());
	}
	//Point ToMapArrayPos(Point pixelPos)
	//{
	//	Point arrayPos(pixelPos.x / TileWidth(),pixelPos.y / TileHeight());
	//	// 負の座標になると計算上1マスずれてしまうので補正する //bug
	//	arrayPos.x -= pixelPos.x < 0 ? 1 : 0 ;
	//	arrayPos.y -= pixelPos.y < 0 ? 1 : 0 ;
	//	return arrayPos;
	//}
	Point ToMapArrayPos(Point pixelPos)
	{
		//auto widthSize=TileWidth()*MapWidth();
		//auto heightSize=TileHeight()*MapHeight();

		Point arrayPos(pixelPos.x / TileWidth(),pixelPos.y / TileHeight());
		// 負の座標になると計算上1マスずれてしまうので補正する
		arrayPos.x -= pixelPos.x < 0 ? 1 : 0 ;
		arrayPos.y -= pixelPos.y < 0 ? 1 : 0 ;


		return arrayPos;
	}
	Rect ToTileRect(Point pixelPos)
	{
		return Rect(ToPixelPos((ToMapArrayPos(pixelPos))),TileWidth(),TileHeight());
	}
	// ワールド座標から描画座標を計算、引数：ワールド座標　　戻り値：描画座標
	Rect ToLocalTileRect(Point pixelPos)
	{
		Rect rect = ToTileRect(pixelPos);
		rect.pos=WorldToLocalPosition(rect.pos);
		return rect;
	}
	void MapDrawTest()
	{
		Point  leftTop = ToMapArrayPos(scrollPosition.asPoint());

		for (int y = leftTop.y; y < leftTop.y + screenArraySize.y +1; y++)
		{
			for (int x = leftTop.x; x < leftTop.x + screenArraySize.x +1; x++)
			{
				Point pixelPos = ToPixelPos(Point( x, y )-leftTop) + Shift();
				if (IsOutOfMap({ x,y }))continue;
				TileDraw(tileMap[y][x], pixelPos.x, pixelPos.y);
			}
		}
	}
	// 円筒スクロールのマップ描画
	void MapRollDraw()
	{
		// 円筒スクロールなので、無理やりマップの範囲内に収める計算をする
		Point leftTop = RollArrayPosition(scrollPosition.asPoint());

		Point drawHomePos = Shift();
		Point drawPos = drawHomePos;
		for (int y = leftTop.y; y < leftTop.y + screenArraySize.y + 2; y++)
		{
			drawPos.x = drawHomePos.x;
			for (int x = leftTop.x; x < leftTop.x + screenArraySize.x + 1; x++, drawPos.x += TileWidth())
			{
				int x0 = x % MapWidth();
				int y0 = y % MapHeight();
				if (IsOutOfMap({ x0,y0 }))continue;
				TileDraw(tileMap[y0][x0], drawPos.x, drawPos.y);
			}
			drawPos.y += TileHeight();
		}
	}
	// スクロールのマップ描画
	void MapDraw()
	{
		// 円筒スクロールは確実に動作するので、苦肉の策でこれを元にした
		Point leftTop = RollArrayPosition(scrollPosition.asPoint());
		if (scrollPosition.asPoint().x < 0)leftTop.x -= MapWidth();
		if (scrollPosition.asPoint().y < 0)leftTop.y -= MapHeight();

		Point drawHomePos = Shift();
		Point drawPos = drawHomePos;
		for (int y = leftTop.y; y < leftTop.y + screenArraySize.y + 2; y++)
		{
			drawPos.x = drawHomePos.x;
			for (int x = leftTop.x; x < leftTop.x + screenArraySize.x + 1; x++, drawPos.x += TileWidth())
			{
				if (IsOutOfMap({ x,y }))continue;
				TileDraw(tileMap[y][x], drawPos.x, drawPos.y);
			}
			drawPos.y += TileHeight();
		}
	}



private:
	String mapStr;
	XMLElement dataElement;
	void Register()
	{
		TextureAsset::Register(TextureFilename(), TextureFilename());
	}
	void ArrayDataSet()
	{
		// イメージ　int tileMap[layer.height][layer.width];
		tileMap = Array<Array<int>>(layer.height, Array<int>(layer.width, 0));
		/*String*/ mapStr = dataElement.text();
		DataIntSet();
	}
	void DataIntSet()
	{
		String newMapstr = U"";// ややこしいので変な文字を消す
		for (auto ch : mapStr)
		{
			if ( !(ch >=U'0'&& ch <=U'9')&& ch != U',')continue;
			newMapstr += ch;
		}
		newMapstr += U'\0';
		mapStr = newMapstr;
		
		unsigned int istr = 0;
		for (int iy = 0; iy < layer.height; iy++)
		{
			for (int ix = 0; ix < layer.width; ix++)
			{
				String dataNumber = U"";
				bool next = true;
				while (next)
				{
					assert(istr < mapStr.length() && "マップの文字列が不足しています");
					auto chara = mapStr[istr];
					++istr;
					if (chara >= U'0' && chara <= U'9')
						dataNumber.push_back(chara);
					else
						if (chara == 13 || chara== U' ')
							continue;
						else
							break;// chara == U','
				}
				// Print << mes;
				tileMap[iy][ix] = ParseInt<int>(dataNumber);
			}
		}
	}
};