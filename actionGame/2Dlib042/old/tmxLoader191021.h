#pragma once
#include <Siv3D.hpp>

// 2019 10 15 オブジェクトレイヤーで垂直反転　水平反転　回転の情報追加、複数タイル追加可能 Parse<uint> を使用
// 2019 10 16 オブジェクトレイヤーを配列化
// 2019 10 17 オブジェクトレイヤーの検索メソッド追加、nameのある配列番号を見つけるFindObjectArray(name)
//  　　　　　nameのある配列を見つける FindObjectArray(name)
// 2019 10 21 フォルダ内のTMXファイルのロード可能にした。ファイルロードエラー処理の追加


#pragma once

//#define ERROR(fmt, ...) err_msg(__FILE__, __FUNCTION__, __LINE__, "error", fmt, ##__VA_ARGS__)

inline void ErrorMessage(String mes)
{
	if (System::ShowMessageBox(mes, MessageBoxButtons::OKCancel)
		== MessageBoxSelection::OK)
	{
		System::Exit();
	}
}//https://scrapbox.io/Siv3D/%E3%82%A6%E3%82%A3%E3%83%B3%E3%83%89%E3%82%A6

class Tileset;
class Object_;

//<objectgroup id = "5" name = "objlayer">
//<object id = "1" name = "ruby" gid = "3" x = "1281" y = "160" width = "32" height = "32" / >
// ...
// ...
//</objectgroup>
//< / map>

const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;
const unsigned JUST_GID_MASK = 0xFFFF; // これは推測
// https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#object

enum class ObjectValue
{
	null,point,ellipse, polygon,text,
};

// コインやアイテムや敵などのスプライト情報
class Object_
{
	/*
			object.emplace_back(
				Parse<int>(objectElement.attribute(U"id").value()),
				objectElement.attribute(U"name").value_or(U""),
				Parse<int>(objectElement.attribute(U"gid").value_or(U"-1")),
				Parse<double>(objectElement.attribute(U"x").value()),
				Parse<double>(objectElement.attribute(U"y").value())-height,
				Parse<int>(objectElement.attribute(U"width").value_or(U"0")),	//error無い場合がある
				height,
				Parse<double>(objectElement.attribute(U"rotation").value_or(U"0")),
				value
			);
	*/

public:
	Object_(XMLElement xmlElement)	//TODO //未完成
	{
		// 高さ分、Y座標を引く(TiledmapEditor のバグ？仕様？)
		int height = Parse<int>(xmlElement.attribute(U"height").value_or(U"0"));	// error無い場合がある
		String valueStr = xmlElement.firstChild().name();
		ObjectValue value =
			valueStr == U"point" ? ObjectValue::point :
			valueStr == U"ellipse" ? ObjectValue::ellipse :
			valueStr == U"polygon" ? ObjectValue::polygon :
			valueStr == U"text" ? ObjectValue::text :
			ObjectValue::null;

		//Init(
		//	Parse<int>(xmlElement.attribute(U"id").value()),
		//	xmlElement.attribute(U"name").value_or(U""),
		//	Parse<int>(xmlElement.attribute(U"gid").value_or(U"-1")),
		//	Parse<double>(xmlElement.attribute(U"x").value()),
		//	Parse<double>(xmlElement.attribute(U"y").value()) - height,
		//	Parse<int>(xmlElement.attribute(U"width").value_or(U"0")),	//error無い場合がある
		//	height,
		//	Parse<double>(xmlElement.attribute(U"rotation").value_or(U"0")),
		//	value	//TODO
		//);

	}
	Object_()
	{
		Init(0, 0, 0, 0, 0, 0, 0);
	}
	Object_(int _id, String _name,int _gid, int _x, int _y, int _width, int _height, double _rotation=0, ObjectValue objectValue = ObjectValue::null)
	{
		Init(_id, _name, _gid, _x, _y, _width, _height,_rotation,objectValue);
	}
	void Init(int _id, String _name, int _gid, int _x, int _y, int _width, int _height, double _rotation=0,ObjectValue objectValue=ObjectValue::null)
	{
		id = _id;
		name = _name;
		Flipped_Horizonally=(_gid & FLIPPED_HORIZONTALLY_FLAG) !=0;
		Flipped_Vertically=(_gid & FLIPPED_VERTICALLY_FLAG) != 0;
		Flipped_Diagonally=(_gid & FLIPPED_DIAGONALLY_FLAG) != 0;
		gid = _gid & JUST_GID_MASK;
		//assert(_gid<1000);	// nameタグが未設定だとエラーだった＞修正済み
		x = _x;
		y = _y;
		width = _width;
		height = _height;
		rotation = _rotation;
		value = objectValue;	//後入れ
	}
	int id;
	String name;
	// -1 なら存在しない
	int gid;
	int width, height;
	double x, y;
	bool Flipped_Horizonally;
	bool Flipped_Vertically;
	bool Flipped_Diagonally;
	double rotation;
	ObjectValue value;
};

class ObjectGroup
{
private:
	XMLElement xmlElement;
public:
	int id;
	String name;
	Array<Object_> object;
	ObjectGroup()
	{
		Clear();
	}
	void Clear()
	{
		id = 0; name = U"";
		object.clear();
	}
	//ObjectGroup& oprator=(ObjectGroup&& ojg)
	//{
	//	return ojg;
	//}
	ObjectGroup(XMLElement& readElement)
	{
		id = Parse<int>(readElement.attribute(U"id").value()), name = readElement.attribute(U"name").value();
		ObjectArraySet(readElement);
		//readElement = readElement.parent().nextSibling();
	}
	void ObjectArraySet(XMLElement& readElement) //void ObjectArraySet(XMLElement XMLelement)
	{
		XMLElement objectElement = readElement.firstChild(); // objectにする //;

		while (objectElement.name() == U"object")
		{
			//<point/><ellipse/> など子要素が入っているか？
		// 高さ分、Y座標を引く(TiledmapEditor のバグ？仕様？)
		int height = Parse<int>(objectElement.attribute(U"height").value_or(U"0"));	// error無い場合がある
		String valueName = objectElement.firstChild().name();
		//String valueStr2 = objectElement.firstChild().text();
		//String valueStr3 = xmlElement.firstChild().attribute((xmlElement.firstChild().name());
		//assert(valueStr1 == valueStr2);

		ObjectValue value =
			valueName == U"point" ? ObjectValue::point :
			valueName == U"ellipse" ? ObjectValue::ellipse :
			valueName == U"polygon" ? ObjectValue::polygon :
			valueName == U"text" ? ObjectValue::text :
			ObjectValue::null;

			//object.emplace_back(objectElement);	// ※ TODO
			object.emplace_back(
				Parse<int>(objectElement.attribute(U"id").value()),
				objectElement.attribute(U"name").value_or(U""),
				Parse<int>(objectElement.attribute(U"gid").value_or(U"-1")),
				Parse<double>(objectElement.attribute(U"x").value()),
				Parse<double>(objectElement.attribute(U"y").value())-height, 
				Parse<int>(objectElement.attribute(U"width").value_or(U"0")),	//error無い場合がある
				height,
				Parse<double>(objectElement.attribute(U"rotation").value_or(U"0")),
				value
			);

			objectElement = objectElement.nextSibling();
		}
	}
	Object_& operator[](int index)
	{
		assert(index<object.size() && "object[index]が範囲外です");
		return object[index];
	}
};

class Image_
{
public:
	String source;
	int width, height;
	Texture tileTexture;
	Image_()
	{
		Set(U"", 0, 0);
	}
	~Image_()
	{
		TileTextureRelease();
	}
	void Set(String _source, int _width, int _height)
	{
		source = _source, width = _width, height = _height;
		if(source!=U"")TileTextureSet();
	}
	// タイルのテキスチャーをセット　※
	void TileTextureSet()
	{
		TextureAsset::Register(source,source);
		for (size_t i = 0;  ! TextureAsset::IsReady(source) ;assert(++i<1000000000));
		// ラグは大丈夫か？
		tileTexture = TextureAsset(source);
	}
	void TileTextureRelease()
	{
		TextureAsset(source).release();
	}

};
// XMLデータの親分
class Map
{
public:
	float version;
	String tiledversion, orientation, renderorder;
	int width, height, tilewidth, tileheight, infinite, nextlayerid, nextobjectid;
	Map()
	{
		Clear();
	}
	void Clear()
	{
		version = 0.0f;
		tiledversion = orientation = renderorder = U"";
		width = height = tilewidth = tileheight = infinite = nextlayerid = nextobjectid = 0;
	}

	void Set(XMLReader& tmx)
	{
		version = ParseFloat<float>(tmx.attribute(U"version").value());
		tiledversion = tmx.attribute(U"tiledversion").value();
		orientation = tmx.attribute(U"orientation").value();
		renderorder = tmx.attribute(U"renderorder").value();
		width = Parse<int>(tmx.attribute(U"width").value());
		height = Parse<int>(tmx.attribute(U"height").value());
		tilewidth = Parse<int>(tmx.attribute(U"tilewidth").value());
		tileheight = Parse<int>(tmx.attribute(U"tileheight").value());
		infinite = Parse<int>(tmx.attribute(U"infinite").value());
		nextlayerid = Parse<int>(tmx.attribute(U"nextlayerid").value());
		nextobjectid = Parse<int>(tmx.attribute(U"nextobjectid").value());
	}
};
// Property型のType用 bool,color,float,file,int,string
enum class Type
{
	null, bool_, color, float_, file, int_, string
};
///*　<image/>
//	<tile id="9">
//		<properties>
//			<property name="ground" type="bool" value="true"/>
//		</properties>
//	</tile>

/*
	String name,typeStr,value;
	Type type;
	タイルの属性情報、std::map で管理しないと面倒になる
*/
class Property
{
public:
	String name, typeStr, value;
	Type type;

	Property()
	{
		Set(U"", U"", U"");
	}
	Property(String __name, String __type, String __value)
	{
		Set(__name, __type, __value);
	}
	void Set(String __name, String __type, String __value)
	{
		name = __name;
		typeStr = __type;
		value = __value;
		type =
			typeStr == U"bool" ? Type::bool_ :
			typeStr == U"color" ? Type::color ://<property name="eat" type="color" value="#ffaa0000"/>
			typeStr == U"float" ? Type::float_ :
			typeStr == U"file" ? Type::file :
			typeStr == U"int" ? Type::int_ :
			typeStr == U"string" ? Type::string :
			Type::null;
	}
	// 何もないか？ ない＝true
	bool IsNull()
	{
		return type == Type::null;
	}
	// 未確認
	bool ValueBool()
	{
		return ParseBool(value);
	}
	// 未確認
	Color ValueColor()
	{
		return Color(value);
	}
	// 
	float ValueFloat()
	{
		return ParseFloat<float>(value);
	}
	int ValueInt()
	{
		return Parse<int>(value);
	}
	//int Value()
	//{

	//}

};
class Tile
{
public:
	int id;
	Array<Property> properties;
	Tile()
	{
		id = 0;
		properties.clear();
	}
	Tile(int id) :id(id)
	{
		properties.clear();
	}
};

//	<tileset version = "1.2" tiledversion = "1.2.4" name = "13_bank(CUnet)(scale)(x4.000000)" tilewidth = "64" tileheight = "64" tilecount = "33" columns = "11">
//		<image source = "13_bank(CUnet)(scale)(x4.000000).png" width = "704" height = "192" / >
//		< / tileset>

// タイルデータ一式管理
class Tileset
{
public:
	String version, tiledversion;
	int firstgid;
	// TSX ファイルパス
	String source // 別TSXファイルで読み込む場合ファイル名がある。 それ以外使わない
		, name;
	int tilewidth, tileheight, tilecount, columns;
	Image_ image;
//	Texture texture;	// うまく読めない
	//Tile tile; //bug err
	std::map<int, Tile> tileProperty;

	String& tsxFileName;
	Tileset() :tsxFileName(source) {
		firstgid = 0;
		source = U"";
		tileProperty.clear();
	}
	Tileset(XMLElement& tmxElement) :tsxFileName(source)
	{
		SetElement(tmxElement);
	}
	// TSXファイルが存在するか　ある＝true
	bool IsSource()
	{
		return source != U"";
	}
	//// タイル画像ファイルを登録
	//void SetTexture()
	//{
	//	_ASSERT_EXPR(TextureAsset::Register(image.source, image.source),L"タイルファイルロードエラー");	//
	//	while(TextureAsset(image.source).isEmpty());
	//	//texture = TextureAsset(image.source);	//読めない、失敗
	//	//assert(false);
	//}


	// 消す予定 　　　消す予定の理由を失念
	//void Set(XMLReader& tmx)
	//{
	//	XMLElement tileset = tmx.firstChild();//<tileset ～>
	//	SetElement(tileset);
	//}
	void SetElement(XMLElement& tmxElement)
	{
		auto& tileset = tmxElement;
		this->firstgid = Parse<int>(tileset.attribute(U"firstgid").value());
		if (tileset.attribute(U"source").has_value())
		{
			this->source = tileset.attribute(U"source").value();//TSXファイルがある

			_ASSERT_EXPR(FileSystem::IsFile(FilePath(source)), L"TSXファイルが見つかりません。");
			XMLReader tsxfile(source);
			TSXFile_Set_Tileset_and_Image(tsxfile); // TSXファイルから値をセット
			tsxfile.close();

		}
		else //TSXファイルがない----すぐにtilesetのデータをセット---------------------
		{
			/*
			<tileset firstgid="1" name="stage1tile1" tilewidth="32" tileheight="32" tilecount="192" columns="8">
			  <image source="mylevel1_tiles.png" width="256" height="768"/>
			 </tileset>
			*/
			this->name = tileset.attribute(U"name").value();
			this->tilewidth = Parse<int>(tileset.attribute(U"tilewidth").value());
			this->tileheight = Parse<int>(tileset.attribute(U"tileheight").value());
			this->tilecount = Parse<int>(tileset.attribute(U"tilecount").value());
			this->columns = Parse<int>(tileset.attribute(U"columns").value());
			//this-> = tileset.attribute(U"").value();
			XMLElement imageChld = tileset.firstChild();	//XMLElement& imageChld = tileset.firstChild();
			this->image.source = imageChld.attribute(U"source").value();
			this->image.width = Parse<int>(imageChld.attribute(U"width").value());
			this->image.height = Parse<int>(imageChld.attribute(U"height").value());
		}
		// <tileset<image> まで完了>
		// <tile xxx >～</tile>を設定----------------------------------------
		XMLElement tileElemnt = tileset.firstChild().nextSibling();
		while (tileElemnt.name() == U"tile")
		{
			auto debugStr = tileElemnt.name();
			// 1ずれているので補正
			//tile.id = 1+ Parse<int>(tileElemnt.attribute(U"id").value()); 
			int tileID = 1 + Parse<int>(tileElemnt.attribute(U"id").value());

			tileProperty[tileID] = Tile(tileID);

			XMLElement propertiesArrayElm = tileElemnt.firstChild();// XMLElement& propertiesArrayElm = tileElemnt.firstChild()
			String propertiesArrayElmName = propertiesArrayElm.name(); //check
			XMLElement propertyElm = propertiesArrayElm.firstChild();// XMLElement& propertyElm = propertiesArrayElm.firstChild()
			String propertyElmName = propertyElm.name(); //check
			while (propertiesArrayElm.name() == U"properties" && propertyElm.name() == U"property") //<tileset～>内すべて
			{
				String _name = propertyElm.attribute(U"name").value();
				String _type = propertyElm.attribute(U"type").has_value() ?
					propertyElm.attribute(U"type").value() : U"string";
				String _value = propertyElm.attribute(U"value").value();
				//tile.properties.emplace_back(_name, type, value);
				tileProperty[tileID].properties.emplace_back(_name, _type, _value);

				propertyElm = propertyElm.nextSibling();
			}
			tileElemnt = tileElemnt.nextSibling();
		}
	}

	// tilewidth tileheight tilecount columns width height source の値をXMLから設定
	// TSXファイルがある場合のみ呼ばれる。微妙に設定値の種類が変わるので
	// 頭から通しで設定
	void TSXFile_Set_Tileset_and_Image(XMLReader& tmx)
	{
		//version = tmx.attribute(U"version").value();
		//tiledversion = tmx.attribute(U"tiledversion").value();
		//name = tmx.attribute(U"name").value();
		tilewidth = Parse<int>(tmx.attribute(U"tilewidth").value());
		tileheight = Parse<int>(tmx.attribute(U"tileheight").value());
		tilecount = Parse<int>(tmx.attribute(U"tilecount").value());
		columns = Parse<int>(tmx.attribute(U"columns").value());
		// image
		image.width = Parse<int>(tmx.firstChild().attribute(U"width").value());
		image.height = Parse<int>(tmx.firstChild().attribute(U"height").value());
		image.source = tmx.firstChild().attribute(U"source").value();
	}

};
class Data
{
public:
	String encoding;
	String tiles;
	Data()
	{
		encoding = U"";
	}
	XMLElement Set(XMLReader& tmx)
	{
		XMLElement dataElement = tmx.firstChild().nextSibling().firstChild();
		encoding = dataElement.attribute(U"encoding").value();
		tiles = dataElement.text();
		return dataElement;
	}
	void SetXMLElement(XMLElement element) //void SetXMLElement(XMLElement& element)
	{
		encoding = element.attribute(U"encoding").value();
		tiles = element.text();
	}

};
// layer の中に必ず data が入っている
class Layer
{
public:
	int id, width, height;
	String name;
	Data data;
	Array<Array<int>> tileMap;
	Layer()
	{
		id = width = height;
		name = U"";
	}
	Layer(XMLReader& tmx)
	{
		Set(tmx);
	}
	Layer(XMLElement& layerElement)
	{
		LayerElementSet(layerElement);
	}
	int operator()(int iy, int ix)
	{
		return tileMap[iy][ix];
	}
	// <layer ~~>
	void LayerElementSet(XMLElement& tmxElement)
	{
		id = Parse<int>(tmxElement.attribute(U"id").value());
		name = tmxElement.attribute(U"name").value();
		width = Parse<int>(tmxElement.attribute(U"width").value());
		height = Parse<int>(tmxElement.attribute(U"height").value());
		// <data>データをセット
		data.SetXMLElement(tmxElement.firstChild()); //bug
		// イメージ　int tileMap[height][width];
		tileMap = Array<Array<int>>(height, Array<int>(width, 0));
		DataArraySet();
	}
	void Set(XMLReader& tmx)
	{
		XMLElement layer = tmx.firstChild().nextSibling();
		LayerElementSet(layer);
	}
	void DataArraySet()
	{
		String newMapstr = U"";// ややこしいので変な文字を消す
		for (auto ch : data.tiles)
		{
			if (!(ch >= U'0' && ch <= U'9') && ch != U',')continue;
			newMapstr += ch;
		}
		newMapstr += U'\0';
		String mapStr = newMapstr;

		unsigned int istr = 0;
		for (int iy = 0; iy < height; iy++)
		{
			for (int ix = 0; ix < width; ix++)
			{
				String dataNumber = U"";
				while (istr < mapStr.length())
				{
					assert(istr < mapStr.length() && "マップの文字列が不足しています");
					auto chara = mapStr[istr];
					++istr;
					if (chara >= U'0' && chara <= U'9')
						dataNumber.push_back(chara);
					else
						break;// chara == U','
				}
				// Print << mes;
				tileMap[iy][ix] = Parse<int>(dataNumber);
			}
		}
	}
};

// 絶対に正の数であまりを出す　引数：割られる数、割る数　　戻り値：余り（正の数）
inline int UnsignedMod(int a, int b)
{
	assert(b >= 0 && "割る数は正の数のみ");
	int mod = a % b;
	if (mod < 0)mod += b;
	return mod;
}
// 絶対に正の数であまりを出す　引数：割られる数、割る数　　戻り値：余り（正の数）
inline int UMod(int a, int b)
{
	return UnsignedMod(a, b);
}

// 
// 2019 09 17 矩形の角のタイル情報保存用構造体
// constにしていないのは加工しても使えるようにするため
struct RectEdgeTileNo
{
	int leftTop;
	int rightTop;
	int leftBottom;
	int rightBottom;
	RectEdgeTileNo() {};
	RectEdgeTileNo(int leftTop, int rightTop, int leftBottom, int rightBottom)
		:leftTop(leftTop), rightTop(rightTop), leftBottom(leftBottom), rightBottom(rightBottom)
	{
		EdgeSet();
	}
	// ４角分
	int edge[4];
	void EdgeSet()
	{
		edge[0] = leftTop;
		edge[1] = rightTop;
		edge[2] = leftBottom;
		edge[3] = rightBottom;
	}
};


// TMXmap: このクラスで、TMXファイルを指定してインスタンスを設定する
//
// 使い方例：	TMXmap tiled(U"mylevel1csv.tmx");
// Appフォルダ内（もしくは指定フォルダ内）にTMXとTSXと画像のファイルを置くこと
class TMXmap
{
private:
	String parentFolder;
	String rootFolder;
public:
	// TMXファイルのあるフォルダー内のファイル名パスにする
	//String SetPath(String fileName)
	//{
	//	return parentFolder + fileName;
	//}
	Map map;
	Array<Tileset> tileset;
	Array<Layer> layer;	// Array 全タイル情報がレイヤーごとに入っている
	Array<ObjectGroup> objectGroup;	// 中でObject_のArray 　　objectGroupも配列化が必要？※
	ObjectGroup& ObjGroup(int i=0)
	{
		assert(i<objectGroup.size() && "objectGroup[i] が範囲内ではないです！");
		return objectGroup[i];
	}
	// オブジェクトグループ内のオブジェクトの取得
	Object_& Object(int group,int i)
	{
		return objectGroup[group][i];
	}
	/*
	{
		return objectGroup.object;
	}
	*/
	//
	Data* GetData(int layerId = 0)
	{
		return &(layer[layerId].data);
	}
	String texAsset;
	//Array<Array<int>> tileMap;
	Point scrollPosition;
	Point screenArraySize;
	// タイルの画像ファイル名 　変更か削除の可能性あり ※
	String TextureFilename(int layerId = 0)
	{
		return tileset[layerId].image.source;
	}
	int MapWidth(int layerId = 0)
	{
		return layer[layerId].width;
	}
	int MapHeight(int layerId = 0)
	{
		return layer[layerId].height;
	}
	int TileWidth(int layerId = 0)
	{
		return tileset[layerId].tilewidth;
	}
	int TileHeight(int layerId = 0)
	{
		return tileset[layerId].tileheight;
	}
	int operator()(int iy, int ix, int layerId = 0)
	{
		return layer[layerId].tileMap[iy][ix];
	}
	//Texture& AllTileTexture()
	//{
	//	return texture;
	//}
	void Clear()
	{
		scrollPosition = { 0,0 };
		screenArraySize = { 0,0 };
		map.Clear();
		tileset.clear();
		layer.clear();	// Array 全タイル情報がレイヤーごとに入っている
		objectGroup.clear();	// 中でObject_のArray
	}
	~TMXmap()
	{
//		TextureAsset(TextureFilename()).release();	// 登録画像データをメモリ開放
	}

	TMXmap()
	{
		objectGroup.clear();
	}
	TMXmap(String tmxFile)
	{
		LoadTMXfile(tmxFile);
	}
	void LoadTMXfile(String tmxFile)
	{
		Clear();
		rootFolder = FileSystem::CurrentDirectory();
		parentFolder = FileSystem::ParentPath(tmxFile);
		//auto Relativefolder = FileSystem::RelativePath(parentFolder);//テスト
		//if (Relativefolder ==U"./")Relativefolder = U"";//テスト
		_ASSERT_EXPR(FileSystem::IsFile(FilePath(tmxFile)), L"TMXファイルが見つかりません。");
		XMLReader tmx(tmxFile);
		FileSystem::ChangeCurrentDirectory(parentFolder);
		//map.Set(tmx.attribute(U"version").value());//テスト
		map.Set(tmx);//<map ~>
		XMLElement parentElement = tmx.firstChild(); // tilesetにする //XMLElement& readElement = tmx.firstChild();
		XMLElement readElement = tmx.firstChild(); // tilesetにする //XMLElement& readElement = tmx.firstChild();
		
//		String nameTileset= readElement.name(); // check

		while (readElement.name() == U"tileset") //<tileset～>内すべて
		{
			tileset.emplace_back(readElement);//tileset.push_back(readElement);これでいけてた！

			readElement = readElement.nextSibling();
		}
		for (int i = 0; readElement.name() == U"layer"; i++)
		{
			layer.emplace_back(readElement); // レイヤーは複数ある
			assert(layer[i].data.encoding == U"csv" && "データはcsv形式のみです。設定変更して保存し直してください");
			readElement = readElement.nextSibling();
		}
		//<objectGroup>配列化
		while (readElement.name() == U"objectgroup")
		{

			objectGroup.emplace_back(readElement);
			readElement = readElement.nextSibling();
			auto test = readElement.name();
 		}

		auto testElm1 = readElement.name();// debug
		auto testELm2 = readElement.nextSibling().name();// debug 無いとU"" 

		tmx.close();

		texAsset = TextureFilename();
		for (auto& a: tileset)
		{
			String source = a.image.source;
			_ASSERT_EXPR(FileSystem::IsFile(FilePath(source)),L"タイルの画像が読み込めません。ファイル名ミスの可能性があります。" );
			//source = U"Fooooooo"; String mes; const std::wstring mesL= L"ハローワールド";
			//_ASSERT_EXPR(FileSystem::IsFile(FilePath(source)), // 失敗、L"～"は変数で行うと文字化けする
			//	[&](String source) {
			//		mes = source + U"が見つかりません。";	//注意
			//		return &mesL;
			//	}(source));
			TextureAsset::Register(source,source);
			a.image.tileTexture = TextureAsset(source);
		}
		FileSystem::ChangeCurrentDirectory(rootFolder);	// フォルダパスを戻す

		scrollPosition = { 0,0 };
		screenArraySize = ToMapArrayPos(Window::ClientSize());
	}
	// 配列が多くややこしいので、現状は[0]を規定とする
	// タイルのプロパティ複数情報の配列の取得
	// タイルに複数の情報を埋め込まない限りはこれは使わない。
	// 無い場合 nullptr を返す
	Array<Property> GetPropertyArray(int tileID, int tilesetNumber = 0)
	{
		if (tilesetNumber >= (int)tileset.size() || tilesetNumber < 0)
			return Array<Property>();

		return
			(tileset[tilesetNumber].tileProperty.count(tileID) == 0) ?
			Array<Property>() :
			tileset[tilesetNumber].tileProperty[tileID].properties;
	}
	// 配列が多くややこしいので、現状は[0]を規定とする　※
	// タイルのプロパティ情報の取得
	Property GetProperty(int tileID, int tilesetNumber = 0)
	{
		if (tileID >= (int)tileset.size() || tileID < 0)
			return Property();
		ErrorWindow(tileset.size() <= tilesetNumber || tilesetNumber < 0, U"tilesetNumberが配列外です。");
		auto& tmap = tileset[tilesetNumber].tileProperty;
		if (tmap.count(tileID) == 0)return Property();
		return tileset[tilesetNumber].tileProperty[tileID].properties[0];
	}

	// タイル番号　表示座標
	void TileDraw(int tileNumber, int x, int y)
	{
		if (tileNumber == 0 || tileNumber == -1)return;
		//ここで　タイル画像番号、描画用番号、を計算
		int tileLayer;
		for (tileLayer = (int)tileset.size() - 1; tileLayer > 0; --tileLayer)
		{
			if (tileNumber > tileset[tileLayer].firstgid)
				break;
		}
		assert(tileLayer>=0 && " タイル番号が異常です ");

		//texture
		//(GetTileRect(tileNumber)).draw(x, y);	// ※ 下のは配列外エラーになる

		auto& tile = tileset[tileLayer];
		tile.image.tileTexture
		(GetTileRect(tile.firstgid,tileNumber,tileLayer)).draw(x, y);	// ※ 下のは配列外エラーになる

		// ※　タイル画像配列数を調べて逆順でチェック
		//int i;
		//for (i = tileset.size()-1; i >= 0; i--)
		//	if (tileset[i].firstgid < tileNumber)
		//	{
		//		tileNumber -= tileset[i].firstgid;
		//		break;
		//	}
		////tileset[i].image.tileTexture( GetTileRect(tileNumber,i) ).draw(x, y);
		//tileset[0].image.tileTexture( GetTileRect(tileNumber,i) ).draw(x, y);
	}
	// タイル番号の画像の取り出す矩形情報の取得
	Rect GetTileRect(int firstGid,int tileNumber, int layerId = 0)
	{
		int tn = tileNumber - firstGid;
		return { tn % tileset[layerId].columns * TileWidth(), tn / tileset[layerId].columns * TileHeight(), TileWidth(), TileHeight() };
	}
	// layerIdが存在するか?
	bool IsLayerID(int layerId)
	{
		return (size_t)layerId < layer.size()  &&  layerId >=0 ;
	}
	// layerIdが存在するか?
	int TryLayerID(int layerId)
	{
		int layerSize = (int)layer.size();
		if (! (layerId < layerSize) || layerId < 0)
		{
			//_ASSERT_EXPR(isClear == true, _T("クリア条件を満たしていない時にこの関数を呼ばないで下さい！"));
			char str[40]; sprintf_s(str, "layerの範囲を超えています。layerサイズは %d です", layerSize);
			assert(str && 0);
		}
		return layerId;
	}
	// マップの範囲外か？
	bool IsOutOfMap(Point arrayPos, int layerId = 0)
	{
		return (arrayPos.x < 0 || arrayPos.x > layer[layerId].width - 1 || arrayPos.y < 0 || arrayPos.y > layer[layerId].height - 1);
	}
	// タイル番号取得する。引数：マップ配列座標　戻り値：タイル番号、ただし範囲外の場合は－１
	int GetTileNumber(Point arrayPos, int layerId = 0)
	{
		if (IsOutOfMap(arrayPos))
			return -1;
		return layer[layerId].tileMap[arrayPos.y][arrayPos.x];
	}
	// ワールド座標の取得 引数：ピクセル座標　　戻り値：ワールド座標
	Point LocalToWorldPosition(int px, int py)
	{
		return scrollPosition + Point(px, py);
	}
	// ワールド座標の取得 引数：標示座標　　戻り値：ワールド座標
	Point LocalToWorldPosition(Point pixelPos)
	{
		return (scrollPosition + pixelPos);
	}
	// ローカル座標（標示座標）の取得 引数：ピクセル座標　　戻り値：ワールド座標
	Point WorldToLocalPosition(Point pixelPos)
	{
		return pixelPos - scrollPosition;
	}
	// 円筒スクロール座標変換 引数：ワールド座標　戻り値：マップの配列座標
	Point RollArrayPosition(Point pos)
	{
		return {
			UMod(pos.x, MapWidth() * TileWidth()) / TileWidth(),
			UMod(pos.y, MapHeight() * TileHeight()) / TileHeight()
		};
	}
	//Array<int> GetRect4Position(Rect rect, int layerID = 0 )
	//{
	//	Array<int> hit(4);
	//	hit[0] = GetPixelPosTileNumber(rect.pos, layerID);
	//	hit[1] = GetPixelPosTileNumber(rect.right().begin.asPoint(), layerID);
	//	hit[2] = GetPixelPosTileNumber(rect.bottom().begin.asPoint(), layerID);
	//	hit[3] = GetPixelPosTileNumber(rect.bottom().end.asPoint(), layerID);
	//	return hit;
	//}

	// 左上 右上 左下 右下 のタイル番号の取得
	// 引数：調べる矩形情報  マップレイヤーのID  　戻り値：４つの頂点の当たっているタイル情報
	// 　layerId　が範囲外だと落ちます。
	RectEdgeTileNo GetRect4Position(Rect rect, int layerID = 0)
	{
		RectEdgeTileNo hit(
			GetPixelPosTileNumber(rect.pos, layerID),
			GetPixelPosTileNumber(rect.right().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().end.asPoint(), layerID)
		);
		return hit;
	}
	// 左上 右上 左下 右下 のタイル番号の取得
	// 引数：スプライト  マップレイヤーのID  　　戻り値：４つの頂点の当たっているタイル情報
	// 　layerId　が範囲外だと落ちます。
	//Array<int> GetPixel4Position(GameSprite&& spr, int layerID = 0)
	RectEdgeTileNo GetPixel4Position(GameSprite&& spr, int layerID = 0)
	{
		Rect rect = spr.GetAnimation()->hit;
		rect.x += spr.iX(); rect.y += spr.iY();
		return GetRect4Position(rect, layerID);
	}
	// １フレーム先の、左上 右上 左下 右下 のタイル番号の取得
	// 引数：スプライト  マップレイヤーのID  　　戻り値：４つの頂点の当たっているタイル情報
	// 　layerId　が範囲外だと落ちます。
	//Array<int> GetPixel4PositionAddVelocity(GameSprite& spr, int layerID = 0)
	RectEdgeTileNo GetPixel4PositionAddVelocity(GameSprite& spr, int layerID = 0)
	{
		Rect rect = spr.GetAnimation()->hit;
		rect.x += (int)(spr.iX()+spr.velocity.x); rect.y += (int)(spr.iY()+spr.velocity.y);
		return GetRect4Position(rect, layerID);
	}


	// タイル番号の取得 引数：x,y ピクセル座標  戻り値：タイル番号
	int GetPixelPosTileNumber(int px, int py)
	{
		return GetPixelPosTileNumber(Point(px, py));
	}
	// タイル番号の取得 引数：pos ピクセル座標  戻り値：タイル番号
	int GetPixelPosTileNumber(Vec2 pos, int layerID = 0)
	{
		return GetPixelPosTileNumber(pos.asPoint(), layerID);
	}
	// タイル番号の取得 引数 ピクセル座標　　　戻り値：ピクセル座標
	int GetPixelPosTileNumber(Point pixelPos, int layerID)
	{
		return GetTileNumber(ToMapArrayPos(pixelPos), layerID);
	}
	// マップ配列座標のピクセルでの矩形情報の計算　引数：マップ配列座標　戻り値：ピクセル単位の矩形情報
	Rect GetRect(Point arrayPos)
	{
		return Rect(ToPixelPos(ToMapArrayPos(arrayPos)), TileWidth(), TileHeight());
	}
	// マップ配列座標のピクセルでの表示座標での矩形情報の計算　引数：マップ配列座標　戻り値：ピクセル単位の矩形情報
	Rect GetLocalRect(Point arrayPos)
	{
		Point local(ToPixelPos(ToMapArrayPos(arrayPos) - scrollPosition));
		return Rect(local, TileWidth(), TileHeight());
	}

	// スクロールの左上の微妙なずれ分の計算
	Point Shift()
	{
		Point _shift(-UMod(scrollPosition.x, TileWidth()), -UMod(scrollPosition.y, TileHeight()));
		return _shift;
	}

	// マップ配列座標からピクセル座標にする　引数：マップ配列座標　　戻り値：ピクセル座標
	Point ToPixelPos(Point mapPos)
	{
		return Point(mapPos.x * TileWidth(), mapPos.y * TileHeight());
	}
	Point ToMapArrayPos(Point pixelPos)
	{
		//auto widthSize=TileWidth()*MapWidth();
		//auto heightSize=TileHeight()*MapHeight();

		Point arrayPos(pixelPos.x / TileWidth(), pixelPos.y / TileHeight());
		// 負の座標になると計算上1マスずれてしまうので補正する
		arrayPos.x -= pixelPos.x < 0 ? 1 : 0;
		arrayPos.y -= pixelPos.y < 0 ? 1 : 0;

		return arrayPos;
	}
	Rect ToTileRect(Point pixelPos)
	{
		return Rect(ToPixelPos((ToMapArrayPos(pixelPos))), TileWidth(), TileHeight());
	}
	// ワールド座標から描画座標を計算、引数：ワールド座標　　戻り値：描画座標
	Rect ToLocalTileRect(Point pixelPos)
	{
		Rect rect = ToTileRect(pixelPos);
		rect.pos = WorldToLocalPosition(rect.pos);
		return rect;
	}
	// 円筒スクロールのマップ描画
	void MapRollDraw(int layerIndex)
	{

		// 円筒スクロールなので、無理やりマップの範囲内に収める計算をする
		Point leftTop = RollArrayPosition(scrollPosition);

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
				TileDraw(layer[layerIndex].tileMap[y0][x0], drawPos.x, drawPos.y);
			}
			drawPos.y += TileHeight();
		}
	}
	void MapDrawAll()
	{
		for (int layerIndex = 0; layerIndex < (int)layer.size(); ++layerIndex)
		{
			MapDraw(layerIndex);
		}
	}


	// スクロールのマップ描画
	void MapDraw(int layerIndex=0)
	{
		// 円筒スクロールは確実に動作するので、苦肉の策でこれを元にした
		Point leftTop = RollArrayPosition(scrollPosition);
		if (scrollPosition.x < 0)leftTop.x -= MapWidth();
		if (scrollPosition.y < 0)leftTop.y -= MapHeight();

		Point drawHomePos = Shift();
		Point drawPos = drawHomePos;

		assert((int)layer.size() > layerIndex && layerIndex >= 0 && "MapDraw(int layerIndex) でlayerが範囲を超えています。");

		//for (int layerIndex =0 ;layerIndex<layer.size(); ++layerIndex)
		//{
		for (int y = leftTop.y; y < leftTop.y + screenArraySize.y + 2; y++)
		{
			drawPos.x = drawHomePos.x;
			for (int x = leftTop.x; x < leftTop.x + screenArraySize.x + 2; x++, drawPos.x += TileWidth())
			{

				if (IsOutOfMap({ x,y }))continue;
				assert(x >= 0);
				assert(y >= 0);

				if (layer[layerIndex].tileMap[y][x] == 0)
				{
					continue;
				}
				TileDraw(layer[layerIndex].tileMap[y][x], drawPos.x, drawPos.y);
			}
			drawPos.y += TileHeight();
		}
		//break;
	//}
	}
	// Objectレイヤーの表示、デバッグ表示用、確認用
	void ObjectDraw(int i)
	{
		assert(i < objectGroup.size());
		for (auto&& ob : objectGroup[i].object)
		{
			Point drawPos = WorldToLocalPosition(Point((int)ob.x, (int)ob.y));
			TileDraw(ob.gid, drawPos.x, drawPos.y);
		}
	}
	// ObjectGroupのnameからobject配列を取得, 無いとエラーで落ちます。
	Array<Object_>& ObjectArray(String objectGroupName)
	{
		for (int i = 0; i < objectGroup.size(); i++)
		{
			auto& objGrp = objectGroup[i];
			if (objGrp.name == objectGroupName)
				return objGrp.object;
		}
		_ASSERT_EXPR(false, L"ObjectArray(String objectGroupName)で見つかりません。中止してください");
		return objectGroup[objectGroup.size()].object;	// わざと範囲外エラーを出して終了
	}
	// ObjectGroupのnameからobject配列番号を取得, 無いと-1を返します。
	// 戻り値：object配列番号　 無い場合は -1
	int FindObjectArray(String objectGroupName)
	{
		for (int i = 0; i < objectGroup.size(); i++)
		{
			auto& objGrp = objectGroup[i];
			if (objGrp.name == objectGroupName)
				return i;
		}
		return -1;	// 見つからない
	}


	// すべてのObjectレイヤーの表示、デバッグ表示用、確認用
	//void ObjectDrawAll()
	//{
	//	for (size_t i = 0; i < objectGroup.size(); i++)
	//	{
	//		for (auto& ob : objectGroup[i].object)
	//		{
	//			Point drawPos = WorldToLocalPosition(Point((int)ob.x, (int)ob.y));
	//			TileDraw(ob.gid, drawPos.x, drawPos.y);
	//		}
	//	}
	//}

	// 座標が描画面内かチェック
	// 引数：座標　　戻り値
	bool InScreen(Vec2 pos)
	{
		return Rect(scrollPosition.x, scrollPosition.y,Window::ClientSize()).intersects(pos);
	}
	// スプライトの描画範囲が描画面内かチェック
	// 引数：座標　　戻り値
	bool InScreen(GameSprite spr)
	{
		Rect drawRect(spr.position.asPoint(), spr.GetAnimation()->draw.w, spr.GetAnimation()->draw.h);
		return Rect(scrollPosition.x, scrollPosition.y,Window::ClientSize()).intersects(drawRect);
	}




private:
	String mapStr;
	XMLElement dataElement;
	//Texture texture;	///高速化？？？ // 配列化必要 ※
	//Array<Texture> tileTexture; // 配列化必要 ※ 未実装
	//void Register()
	//{
	//	TextureAsset::Register(TextureFilename(), TextureFilename());
	//}
	// check がtrue なら　エラーメッセージウィンドウを出して終了
	void ErrorWindow(bool check, String message)
	{
		if (check)
			ErrorMessage(message);
	}
	void tilesetNumberが配列外？(bool check)
	{
		ErrorWindow(check, U"tilesetNumberが配列外です。");
	}
};
