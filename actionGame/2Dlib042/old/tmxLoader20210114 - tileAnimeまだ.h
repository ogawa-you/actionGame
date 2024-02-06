#pragma once
#include <Siv3D.hpp>
#include "GameSprite.h"

// 2019 10 15 オブジェクトレイヤーで垂直反転　水平反転　回転の情報追加、複数タイル追加可能 Parse<uint> を使用
// 2019 10 16 オブジェクトレイヤーを配列化
// 2019 10 17 オブジェクトレイヤーの検索メソッド追加、nameのある配列番号を見つけるFindObjectArray(name)
//  　　　　　nameのある配列を見つける FindObjectArray(name)
// 2019 10 21 フォルダ内のTMXファイルのロード可能にした。ファイルロードエラー処理の追加
// 2020 01 23 type属性追加、gid==0 の時、Y座標補正が不要になるように変更、レイヤーごとのスクロールスピード設定機能追加
// 2020 2  14 サーバ起動だと動作しないバグを修正（未確認）。
// 2020/11/30 TSXファイルパスバグ修正（松本）
// 2020/12/8 TSXファイルのTileの<image>タグ対応変更中

//#define ERROR(fmt, ...) err_msg(__FILE__, __FUNCTION__, __LINE__, "error", fmt, ##__VA_ARGS__)

template <class Type>
/// <summary>
/// String文字列を＜T＞に変換して代入する。できない場合は何もしない。
/// </summary>
/// <typeparam name="Type">型</typeparam>
/// <param name="str">変換する文字列</param>
/// <param name="value">代入する変数</param>
/// <returns>変換できると true</returns>
bool TryParse(StringView str, Type& value)
{
	if (auto opt = ParseOpt<Type>(str))
	{
		value = opt.value();
		return true;
	}
	return false;
}

inline void ErrorMessage(String mes)
{
	if (System::ShowMessageBox(mes, MessageBoxButtons::OKCancel)
		== MessageBoxSelection::OK)
	{
		System::Exit();
	}
}//https://scrapbox.io/Siv3D/%E3%82%A6%E3%82%A3%E3%83%B3%E3%83%89%E3%82%A6

/// <summary>
/// 未使用か消す：アトリビュート(属性)があれば指定の変数に代入 ＜Element attribute = attribute value＞
/// </summary>
/// <param name="attribute">attribute(属性)</param>
/// <param name="fix">代入したいString変数</param>
/// <param name="element">XMLの Element</param>
inline void ZAP_ReadAttributeValue(const String attribute, String& fix,XMLElement& element)
{
	const auto attr = element.attribute(attribute);
	if (attr.has_value())
		fix = attr.value();
}

/// <summary>
/// アトリビュート(属性)があれば指定の変数に代入 ＜Element attribute = attribute value＞
/// そうでなかったら何もしない。int変換できなかったらエラーで止まるので注意
/// </summary>
/// <param name="attribute">attribute(属性)</param>
/// <param name="fix">代入したいint変数</param>
/// <param name="element">XMLの Element</param>
inline void ReadAttrbuteValueInt(const String attribute, int& fix,XMLElement& element)
{
	const auto attr = element.attribute(attribute);
	_ASSERT_EXPR(TryParse<int>(attr.value(), fix),
		L"ReadAttrbuteValueInt(～～～)で int変換できませんでした。");
}

//Parse<int>(xmlElement.attribute(U"gid").value_or(U"0")),	// gid=0は通常ない




class Tileset;class Tile;
class Object_;
class TMXfile	// todo
{
private:
	TMXfile() {} //インスタンス生成させないようにわざと privateにしている
public:
	static inline String TMXFolder;
	static inline String TSXFolder;
	static inline String PNGFolder;
	// 
	// TMXFile::TMXFolder に .tmx のフォルダパスを取得
	//
	// 引数：ファイルネーム, 階層（通常は０のまま）
	// 戻り値無し　※ TMXFile::TMXFolder にセットされる
	// 例： U"abcd/efg.tmx" >> TMXFile::UseFolder=U"abcd/"
	static void SetTMXFolder(String fileName, int parent = 0)
	{
		TMXFolder = fileName.substr(parent, fileName.lastIndexOf(U'/') + 1);
	}

	// 
	// TMXFile::TSXFolder に .tsx のフォルダパスを取得
	//
	// 引数：ファイルネーム, 階層（通常は０のまま）
	// 戻り値無し　※ TMXFile::TSXFolder にセットされる
	// 例： U"abcd/efg.tmx" >> TMXFile::UseFolder=U"abcd/"
	static void SetTSXFolder(String fileName, int parent = 0)
	{
		TSXFolder = TMXFolder + fileName.substr(parent, fileName.lastIndexOf(U'/') + 1);
	}

	// 
	// TMXFile::PNGFolder に .tmx のフォルダパスを取得
	//
	// 引数：ファイルネーム, 階層（通常は０のまま）
	// 戻り値無し　※ TMXFile::PNGFolder にセットされる
	// 例： U"abcd/efg.tmx" >> TMXFile::UseFolder=U"abcd/"
	static void SetPNGFolder(String fileName, int parent = 0)
	{
		PNGFolder = TSXFolder + fileName.substr(parent, fileName.lastIndexOf(U'/') + 1);
	}

	// 
	// フォルダ付きファイル名取得
	//
	// 引数：ファイルネーム
	// 戻り値：（TMXFile::TSXFolder + fileName）
	// 例： U"efg.tsx" >> TMXFile::TSXFolder=U"abcd/efg.tsx"（U"abcd/"は既にある前提）
	static String SetTSXName(String fileName)
	{
		return TMXfile::TMXFolder + fileName;
	}

	// 
	// フォルダ付きファイル名取得
	//
	// 引数：ファイルネーム
	// 戻り値：（TMXFile::PNGFolder + fileName）
	// 例： U"efg.png" >> TMXFile::PNGFolder=U"abcd/efg.png"（U"abcd/"は既にある前提）
	static String SetPNGName(String fileName)
	{
		return TMXfile::TSXFolder + fileName;
	}
};


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
	null, point, ellipse, polygon, text,
};

// コインやアイテムや敵などのスプライト情報
class Object_
{
public:
	Object_(XMLElement& xmlElement)
	{
		XMLElement childelm = xmlElement.firstChild();
		Init(
			Parse<int>(xmlElement.attribute(U"id").value()),
			xmlElement.attribute(U"name").value_or(U""),
			xmlElement.attribute(U"type").value_or(U""),
			Parse<int>(xmlElement.attribute(U"gid").value_or(U"0")),	// gid=0は通常ない
			Parse<double>(xmlElement.attribute(U"x").value()),
			Parse<double>(xmlElement.attribute(U"y").value()),	// 内部で高さ分をここから引いている
			Parse<double>(xmlElement.attribute(U"width").value_or(U"0")),	//error無い場合がある
			Parse<double>(xmlElement.attribute(U"height").value_or(U"0")),	//error無い場合がある
			Parse<double>(xmlElement.attribute(U"rotation").value_or(U"0")),
			childelm.name()	//objectタグ内の子階層にさらにデータがあるか？
			//value	//TODO******************************************************************
		);
		// object で追加データがある場合
		text_wrap = 0;
		text = U"";
		Array<String> strAry;
		switch (value)
		{
		case ObjectValue::null:
		case ObjectValue::point:
		case ObjectValue::ellipse:
			break;
		case ObjectValue::polygon:
			polygon = childelm.attribute(U"points").value();
			strAry = polygon.split(U' ');
			for (String vec2Str : strAry)
			{
				polygon_points.emplace_back(Parse<Vec2>(U'(' + vec2Str + U')'));
			}
			break;
		case ObjectValue::text:
			text_wrap = Parse<int>(childelm.attribute(U"wrap").value_or(U"0"));
			text = childelm.text();
			break;
		default:
			break;
		}
	}
	Object_()
	{
		Init();
	}
	Object_(int _id, String _name, String _type, int _gid, int _x, int _y, int _width, int _height, double _rotation = 0, String valueName = U"")
	{
		Init(_id, _name, _type, _gid, _x, _y, _width, _height, _rotation, valueName);
	}
private:
	// objectの初期化、注意：valueNameがあっても、さらにそのデータの追加まではここではやらない
	void Init(int _id = 0, String _name = U"", String _type = U"", int _gid = 0, double _x = 0, double _y = 0, double _width = 0, double _height = 0, double _rotation = 0, String valueName = U"")
	{
		id = _id;
		name = _name;
		type = _type;
		Flipped_Horizonally = (_gid & FLIPPED_HORIZONTALLY_FLAG) != 0;
		Flipped_Vertically = (_gid & FLIPPED_VERTICALLY_FLAG) != 0;
		Flipped_Diagonally = (_gid & FLIPPED_DIAGONALLY_FLAG) != 0;
		gid = _gid & JUST_GID_MASK;
		//assert(_gid<1000);	// nameタグが未設定だとエラーだった＞修正済み
		x = _x;
		y = gid ? (_y - _height) : _y;// gidがあれば、高さ分、Y座標を引く(TiledmapEditor のバグ？仕様？)
		width = _width;
		height = _height;
		rotation = _rotation;

		value =
			valueName == U"point" ? ObjectValue::point :
			valueName == U"ellipse" ? ObjectValue::ellipse :
			valueName == U"polygon" ? ObjectValue::polygon :
			valueName == U"text" ? ObjectValue::text :
			ObjectValue::null;
	}
public:
	int id;
	String name;
	String type;
	int gid;// ０ なら存在しない
	double width, height;
	double x, y;
	bool Flipped_Horizonally;
	bool Flipped_Vertically;
	bool Flipped_Diagonally;
	double rotation;
	ObjectValue value;
	String polygon;
	Array<Vec2> polygon_points;
	String text;
	int text_wrap;
};

class ObjectGroup
{
public:
	int id;
	String name;
	Array<Object_> object;
	//String draworder;		//>>>>追加仕様
	ObjectGroup()
	{
		Clear();
	}
	void Clear()
	{
		id = 0; name = U"";
		object.clear();
	}

	ObjectGroup(XMLElement& readElement)
	{
		// sample2 <objectgroup draworder="index" id="2">未実装
		id = Parse<int>(readElement.attribute(U"id").value()), name = readElement.attribute(U"name").value();
		ObjectArraySet(readElement);
		//readElement = readElement.parent().nextSibling();
	}
	void ObjectArraySet(XMLElement& readElement) //void ObjectArraySet(XMLElement XMLelement)
	{
		XMLElement objectElement = readElement.firstChild(); // objectにする //;

		while (objectElement.name() == U"object")
		{
			object.emplace_back(objectElement);

			objectElement = objectElement.nextSibling();
		}
	}
	// object[index] を取得
	Object_& operator[](int index)
	{
		assert(index < object.size() && "object[index]が範囲外です");
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
	// <image width="200" height="200" source=".png/SavePoint.png"/> のロード
	void ReadAttribute(XMLElement& element)
	{
		source = element.attribute(U"source").value();
		width = Parse<int>(element.attribute(U"width").value());
		height = Parse<int>(element.attribute(U"height").value());
#ifdef _DEBUG
		auto value = element.firstChild().isNull();

#endif // _DEBUG


	}
	void Set(String _source, int _width, int _height)
	{
		source = _source, width = _width, height = _height;
		if (source != U"")TileTextureSet();
	}
	// タイルのテキスチャーをセット　※
	void TileTextureSet()
	{
		TextureAsset::Register(source, source);
		for (size_t i = 0; !TextureAsset::IsReady(source); assert(++i < 1000000000));
		// ラグは大丈夫か？
		tileTexture = TextureAsset(source);
	}
	void TileTextureRelease()
	{
		TextureAsset(source).release();
	}
	// <image>タグがあるか？
	bool IsUse()
	{
		return source != U"";
	}
};
// XMLデータの親分  // compressionlevel 追加 2020/12/17
//  compressionlevel はない場合もある
//
class Map
{
public:
	float version;
	String tiledversion, orientation, renderorder;
	int compressionlevel,width, height, tilewidth, tileheight, infinite, nextlayerid, nextobjectid;
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

	void AttributeSet(XMLReader& tmx)
	{
		version = ParseFloat<float>(tmx.attribute(U"version").value());
		tiledversion = tmx.attribute(U"tiledversion").value();
		orientation  = tmx.attribute(U"orientation").value();
		renderorder  = tmx.attribute(U"renderorder").value();
		compressionlevel = Parse<int>(tmx.attribute(U"compressionlevel").value_or(U"0")); // 無い場合がある
		width	= Parse<int>(tmx.attribute(U"width").value());
		height	= Parse<int>(tmx.attribute(U"height").value());
		tilewidth	= Parse<int>(tmx.attribute(U"tilewidth").value());
		tileheight	= Parse<int>(tmx.attribute(U"tileheight").value());
		infinite	= Parse<int>(tmx.attribute(U"infinite").value());
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
		SetAttribute(U"", U"", U"");
	}
	Property(String __name, String __type, String __value)
	{
		SetAttribute(__name, __type, __value);
	}
	Property(XMLElement& xmlelement)	//@@@@
	{
		SetAttribute(xmlelement);
	}
	// <properties>以下と下層の<property>配列のロードして入れる
	// propertyElm は 次に移動します。//@@@@ 
	void SetAttribute(XMLElement& propertyElm)
	{
		auto _name = propertyElm.attribute(U"name").value();
		auto __type = propertyElm.attribute(U"type").has_value() ?
			propertyElm.attribute(U"type").value() : U"string";
		auto _value = propertyElm.attribute(U"value").value();
		SetAttribute(_name,__type,_value);
		propertyElm = propertyElm.nextSibling(); // 消す？消さない？@@@@
	}
	void SetAttribute(String __name, String __type, String __value)
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
/*	// <properties>以下と下層の<property>配列のロードしてtile連想配列に入れる // 削除予定@@@@@
	void XMLRead(XMLElement& propertiesArrayElm, std::map<int, Tile>& tile, int tileID)
	{
		XMLElement propertyElm = propertiesArrayElm.firstChild();
		while (propertiesArrayElm.name() == U"properties" && propertyElm.name() == U"property") //<tileset～>内すべて
		{
			String _name = propertyElm.attribute(U"name").value();
			String _type = propertyElm.attribute(U"type").has_value() ?
				propertyElm.attribute(U"type").value() : U"string";
			String _value = propertyElm.attribute(U"value").value();
			//tile.properties.emplace_back(_name, type, value);
			//tile[tileID].properties.emplace_back(_name, _type, _value);

			propertyElm = propertyElm.nextSibling();
		}
	}
	// <properties>以下と下層の<property>配列のロードしてtile連想配列に入れる
	// propertyElm は 次に移動します。//@@@ 
	Property XMLRead(XMLElement& propertyElm,int tileID)
	{
			String _name = propertyElm.attribute(U"name").value();
			String _type = propertyElm.attribute(U"type").has_value() ?
				propertyElm.attribute(U"type").value() : U"string";
			String _value = propertyElm.attribute(U"value").value();
			propertyElm = propertyElm.nextSibling();
			return Property(_name, _type, _value);
	}*/

};
/// <summary>
/// tile の animation のframeの属性
/// </summary>
class AnimationFrame
{
public:
	int tileid, duration;
	AnimationFrame():tileid(0),duration(0)
	{}
	//AnimationFrame(int tiledid, int duration) :tileid(tiledid+1), duration(duration)
	//{}
	/// <summary>
	/// Animationの子のframe属性の設定
	/// tileidは１補正される
	/// elementはnextSibling()されるので注意
	/// </summary>
	/// <param name="elm">frameのelement</param>
	AnimationFrame(XMLElement& elm)
	{
	//	assert("Animation");
		auto attrs = elm.attributes();

		bool tileidOK = TryParse<int>(attrs[0].second,tileid);
		++tileid;// エディター表記では１ずれているので直している
		_ASSERT_EXPR(tileidOK,L"<frame tileid>の値がおかしいです");
		bool durationOK = TryParse<int>(attrs[1].second,duration);
		_ASSERT_EXPR(durationOK,L"<frame duration>の値がおかしいです");
		// elm = elm.nextSibling();  "/"が最後にあると自動的に次になるっぽい？
	}
	bool IsNull()
	{
		return tileid == 0 && duration == 0;
	}
};

// データ仕様は変化するので注意
class Tile
{
private:
	int animationTotalTime;
public:
	// attribute
	int id;
	String type;
	int probability;


	Tile()
	{
		Clear();
	}
	Tile(int _id)
	{
		Clear();
		Tile::id = _id;
	}
	/// <summary>
	/// 属性をロードして設定する。
	/// </summary>
	/// <param name="tileElm">読み込む場所</param>
	Tile(XMLElement& tileElm)
	{
		Tile::ReadAttributeValue(tileElm);
	}
	bool IsImageUse()
	{
		return image.IsUse();
	}
	bool IsPropatyUse()
	{
		return properties.size()>0;
	}
	void Clear()
	{
		id = 0;
		properties.clear();
		animation.clear();
		objectGroup.Clear();
		probability = 0;
		type = U"";
		animationTotalTime = 0;
	}
	/* sample1
	// <tile id="0">
	// <image width = "200" height = "200" source = ".png/SavePoint.png" / >
	// sample2
	//   <tile id="2">
	// <properties>
	// <property name = "eat" type = "color" value = "#ffaa0000" / >
	// <property name = "fileTest" type = "file" value = "../../../../../../../Program Files/Tiled/AUTHORS.txt" / >
	// <property name = "floatTest" type = "float" value = "3" / >
	// <property name = "intTest" type = "int" value = "5" / >
	// <property name = "stringTest" value = "hello" / >
	// < / properties>
	// < / tile>
	*/
	/* sample3
	<tile id = "0" type = "種類" probability = "3">
	<image width = "200" height = "200" source = ".png/SavePoint.png" / >
	<objectgroup draworder = "index" id = "2">
	<object id = "1" x = "44" y = "27">
	<polygon points = "0,0 72,134 100,60" / >
	< / object>
	<object id = "2" x = "11" y = "10" width = "169" height = "149" / >
	<object id = "3" x = "83" y = "43" width = "80" height = "111">
	<ellipse / >
	< / object>
	<object id = "4" x = "65" y = "197">
	<point / >
	< / object>
	< / objectgroup>
	< / tile>
	*/
	
	/// <summary>
	/// 属性と内容をロードして設定
	/// </summary>
	/// <param name="element"></param>
	void Set(XMLElement& element)
	{
		ReadAttributeValue(element);
		ReadValue(element);
	}
	// <tile  attribute > のロード
	// <tile>の子はロードしない
	void ReadAttributeValue(XMLElement& element)
	{
		_ASSERT_EXPR(element.attribute(U"id").has_value(), L" \"Tile id\" が 存在しません");
		for (pair<String,String> attr : element.attributes())
		{
			if(attr.first==U"id")
			{
				 bool tryQ = TryParse<int>(attr.second,Tile::id);
				 Tile::id++; //エディタの表示では１ずれているので補正
				 assert( tryQ && "tile id が整数でありません");
			}
			if(attr.first==U"type")
			{
				Tile::type = attr.second;
			}
			if(attr.first==U"probability")
			{
				 bool tryQ = TryParse<int>(attr.second,Tile::probability);
				 assert( tryQ && "tile probability が整数でありません");
			}
		}
	}

	// <tile> Value ////////////////////////////////////////////////////////////////////////////
	Array<AnimationFrame> animation;
	Array<Property> properties;
	Image_ image; // 画像１枚ずつ設定できる
	ObjectGroup objectGroup;
	// Can contain at most one: <properties>, <image> (since 0.9), <objectgroup>, <animation>

	/// <summary>
	/// アニメーション設定されているタイルか
	/// </summary>
	/// <returns>アニメーションされている = true</returns>
	bool IsAnimation()
	{
		return animation.size() > 0;
	}
	// <animation>下層の<frame>配列のロードして配列に入れる
	void AnimationRead(XMLElement& animeElm)
	{
		//XMLElement frameElm = frameElm.firstChild();	//error
		//while (frameElm.name() == U"frame") //<frame>内すべて
		//{
		//	animation.emplace_back(frameElm);
		//	frameElm = frameElm.nextSibling();
		//}
	}
	/// <summary>
	/// アニメーションのトータルタイム
	/// </summary>
	void SetAnimationTotalTime()
	{
		animationTotalTime = 0;
		for (auto& frame : animation)
		{
			animationTotalTime += frame.duration;
		}
	}


	/// <summary>
	/// ＜tile＞～～～～＜/tile＞ の読み込み 
	/// image objectgroup properties animation(未実装) のいずれかがある。
	/// </summary>
	/// <param name="element"></param>
	void ReadValue(XMLElement& element)
	{
		// assert("まだ未完成"); // 実行確認まだ

		auto child=element.firstChild();
		for (String tag = child.name(); tag != U""; child = child.nextSibling(), tag = child.name())
		{
			if (tag == U"image")
			{
				image.ReadAttribute(child);
			}
			if (tag == U"objectgroup")
			{
				objectGroup.ObjectArraySet(element);
			}
			if (tag == U"properties")
			{
				XMLElement propertyElm = child.firstChild();
				while (propertyElm.name() = U"property")
				{
					// <property>
					properties.emplace_back(propertyElm);
				}
			}
			if (tag == U"animation")
			{		// error 
				XMLElement frameElm = child.firstChild();
				while (frameElm.name() == U"frame")
				{
					animation.emplace_back(frameElm);
					frameElm = frameElm.nextSibling();
				}
			}


		}


	}

/*
		auto TrySetElementIntValue = [&](String name,int &value)
		{
			bool parse = TryParse<int>(element.attribute(name).value(), value);
			_ASSERT_EXPR(parse, L"int変換できない文字列です。 ");
		};
		auto TrySetStringValue = [&](String name,String &value)
		{
			auto elementAttr = element.attribute(name);
			if(elementAttr.has_value())
					value = elementAttr.value();
		};
*/
};

enum class Orientation // orthogonal or isometric, defaults to orthogonal)
{
	Orthogonal,Isometric
};
// sample   TSX にある
// <grid orientation="orthogonal" width="1" height="1"/>

// Gridは被っているので、Grid_
class Grid_
{
public:
	Orientation orientation;
	int width, height;
	Grid_() :orientation(Orientation::Orthogonal),width(0),height(0)
	{}
	// <grid orientation="?????" width="???" height="??"/> のロード
	// element は nextSibiling に変化するので注意
	void ReadAttribute(XMLElement& element )
	{
		auto value = element.attribute(U"orientation").value();
		orientation = value == U"isometric" ? Orientation::Isometric : Orientation::Orthogonal;
		width = Parse<int>(element.attribute(U"width").value());
		height = Parse<int>(element.attribute(U"height").value());
		//element = element.nextSibling(); //>>>> " ～/>"だと・・・？
	}
};


// sample
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
	Grid_ grid;

	std::map<int, Tile> tile;

	String& tsxFileName;
	Tileset() :tsxFileName(source) {
		firstgid = 0;
		source = U"";
		tile.clear();
	}
	Tileset(XMLElement& tmxElement) :tsxFileName(source)
	{
		ReadAttributeValue(tmxElement);
		ReadValue(tmxElement);
	}
	// TSXファイルが存在するか　ある＝true
	bool IsSource()
	{
		return source != U"";
	}
private:
	/*
	sampe1
	<tileset firstgid="1" name="stage1tile1" tilewidth="32" tileheight="32" tilecount="192" columns="8">
	<image source="mylevel1_tiles.png" width="256" height="768"/>
	</tileset>

	sample2
	<tileset firstgid="1" source="../TileData/ObjectElement.tsx"/>
	*/
	// <tileset ～>
	//TMX   firstgid="1" name="stage1tile1" tilewidth="32" tileheight="32" tilecount="192" columns="8">
	//TSX   version="1.2" tiledversion="1.3.0" name="ObjectElement" tilewidth="3100" tileheight="1050" tilecount="35" columns="0">
	//TMX(TSXあり) <tileset firstgid="1" source="../TileData/ObjectElement.tsx"/>

	// TMXファイルのみ <tileset ～Attribute～> Attributeを読み取り、設定する
	// TSXファイルにもある場合、後ほど一括で読み取る
	void ReadAttributeValue(XMLElement& tilesetElm)
	{
		// 必ず先にTMXファイルをロード
		// firstgid と source のみなら TSXファイル(source)ロード してTSXからアトリビュート追加
		//                TSX では tilesetの中身のみ設定する
		// 
		// firstgid
		auto& tileset = tilesetElm;
		auto firstgidAttr = tileset.attribute(U"firstgid");
		if (firstgidAttr.has_value())
			Tileset::firstgid = Parse<int>(firstgidAttr.value());
		auto sourceAttr = tileset.attribute(U"source");
//		if (sourceAttr.has_value())  // これでは問題が起きる 
		if (tilesetElm.attributes().size()<3) //  属性数でチェック
		{
			Tileset::source = sourceAttr.value();
			return;		// 戻ってまとめてTSXファイルのロードして続きをする
		}
		else
		{	// TMXファイルのみ
			Tileset::name = tilesetElm.attribute(U"name").value();
			Tileset::tilewidth = Parse<int>(tilesetElm.attribute(U"tilewidth").value());
			Tileset::tileheight = Parse<int>(tilesetElm.attribute(U"tileheight").value());
			Tileset::tilecount = Parse<int>(tilesetElm.attribute(U"tilecount").value());
			Tileset::columns = Parse<int>(tilesetElm.attribute(U"columns").value());
		}
		return;
	}

	/// <summary>
	/// TSXファイルのtileset
	/// </summary>
	/// <param name="tilesetElm"></param>
	void ReadAttributeTSX(XMLElement& tilesetElm)
	{
		assert(IsSource() && "TSXファイルからの読み込みでないです");
		Tileset::version		=	tilesetElm.attribute(U"version").value_or(U"");
		Tileset::tiledversion	=	tilesetElm.attribute(U"tiledversion").value_or(U"");
		Tileset::name			=	tilesetElm.attribute(U"name").value_or(U"");
		Tileset::tilewidth		=	Parse<int>(tilesetElm.attribute(U"tilewidth").value());
		Tileset::tileheight		=	Parse<int>(tilesetElm.attribute(U"tileheight").value());
		Tileset::tilecount		=	Parse<int>(tilesetElm.attribute(U"tilecount").value());
		Tileset::columns		=	Parse<int>(tilesetElm.attribute(U"columns").value());
	}


		//test
/*
		auto ReadAttrbValue = [&](String &fix,const String attribute) 
		{
			const auto aa=tilesetElm.attribute(attribute);
			if(aa.has_value())
				fix=aa.value();
		};
		do {} while (false);

		const auto aa=tilesetElm.attribute(U"test");
		String fix;
		if(aa.has_value())
			fix=aa.value();
*/

	private:
	/// <summary>
	/// ＜tileset＞～～～～～＜tileset＞のロード  属性(attribute)は読まない
	/// 子が無い場合、次のエレメントへ移動する
	/// </summary>
	/// <param name="tmxTileset">XMLエレメント</param>
	void ReadValue(XMLElement& tmxTileset)
	{
		if (IsSource())return; //TSXファイルがある場合、ここでtilesetValueがなく別ファイル読み込みになる
		XMLElement tilesetChild = tmxTileset.firstChild();
		if (tilesetChild.isNull())
		{
			tmxTileset.nextSibling();
			return;
		}
		if(tilesetChild.name()==U"image")
			image.ReadAttribute(tilesetChild);

		while (tilesetChild.name() == U"tile")
		{
			auto debugStr = tilesetChild.name();
			int tileID = 1 + Parse<int>(tilesetChild.attribute(U"id").value());
			tile[tileID].Set(tilesetChild);

			// ここで２択、<image>と<properties>で処理を分ける @@@

			XMLElement propertiesArrayElm = tilesetChild.firstChild();// XMLElement& propertiesArrayElm = tileElemnt.firstChild()
			String propertiesArrayElmName = propertiesArrayElm.name(); //check
			XMLElement propertyElm = propertiesArrayElm.firstChild();// XMLElement& propertyElm = propertiesArrayElm.firstChild()
			String propertyElmName = propertyElm.name(); //check

			PropertiesRead(propertiesArrayElm,tileID);
			//tile[tileID].properties[0].Set(propertiesArrayElm); //NG @@@
			tilesetChild = tilesetChild.nextSibling();
		}
	}
	// <properties>以下と下層の<property>配列のロードしてtile連想配列に入れる
	void PropertiesRead(XMLElement& propertiesArrayElm,int tileID)
	{
		XMLElement propertyElm = propertiesArrayElm.firstChild();
		while (propertiesArrayElm.name() == U"properties" && propertyElm.name() == U"property") //<tileset～>内すべて
		{			
			String _name = propertyElm.attribute(U"name").value();
			String _type = propertyElm.attribute(U"type").has_value() ?
				propertyElm.attribute(U"type").value() : U"string";
			String _value = propertyElm.attribute(U"value").value();
			Tileset::tile[tileID].properties.emplace_back(_name, _type, _value);

			propertyElm = propertyElm.nextSibling();
		}
	}

	public:
	/// <summary>
	/// TSXファイルごとにデータを追加セット
	/// ただし、source が設定されていないなら何もしない
	/// </summary>
	void TSXfileSet()
	{
		if (!IsSource()) return;
		// TSXファイルのロード
		XMLReader tsx((FilePathView)(TMXfile::TMXFolder+source));

		XMLElement& element = tsx;

		String elementName = element.name();

//#pragma region tilsetの子の属性をすべてロード
//		// <tileset>の子 のエレメントの属性をすべて保存
//		// データが壊れる前に退避
//
//		auto t1=tsx.firstChild().name();
//		auto t2=tsx.firstChild().nextSibling().name();
//
//		XMLElement child = element.firstChild();	//tilesetの子
//		using AttributeArray = Array < std::pair<String, String> >;
//		Array< std::pair<String, AttributeArray > > attrArray;
//
//		for (; child; child = child.nextSibling())
//		{
//			String name1 = child.name();
//			auto attr1 = child.attributes();
//			attrArray.emplace_back(name1, attr1);
//		}
//		child = element.firstChild();//一旦戻す
//#pragma endregion

		// tileset の属性をロード
		ReadAttributeTSX(element);
		// tilesetの中身をロード
		TilesetValueSet(element);


		tsx.close();

	}

	/// <summary>
	/// ＜tileset＞～～～＜tileset＞ 中身の設定
	/// </summary>
	/// <param name="element">XMLelement</param>
	void TilesetValueSet(XMLElement& element)
	{
		// grid image Chack
		element = element.firstChild();

#ifdef _DEBUG
		auto elementAttrs = element.attributes(); //debug
		auto tt = elementAttrs[0].first;	//db

		auto tileElm = element.nextSibling();	//db
		auto tileElmName = tileElm.name();	//db
		auto tileAttrs = tileElm.attributes();	//db
#endif

		while (element)
		{
			auto tag = element.name();
			auto attribute = element.attributes();

			// <tileset>の中、<grid> か <image> か <tile> で分岐する
			if (tag == U"image") // <image >
			{
				image.ReadAttribute(element);
				TMXfile::SetPNGFolder(image.source); // .pngフォルダ階層があればフォルダ名のみ取得
			}
			if (tag == U"grid")	// <grid>
			{
				//<grid orientation = "orthogonal" width = "1" height = "1" / >
	//			XMLElement* xml=&element;
				grid.ReadAttribute(element);	//@@@ <grid orientation="????" width="?" height="?"/>
			}
			/*
			<tile id="0" type="種類" probability="3">
			  <image width="200" height="200" source=".png/SavePoint.png"/>
			  <objectgroup draworder="index" id="2">
			   <object id="1" x="44" y="27">
				<polygon points="0,0 72,134 100,60"/>
			   </object>
			   <object id="2" x="11" y="10" width="169" height="149"/>
			   <object id="3" x="83" y="43" width="80" height="111">
				<ellipse/>
			   </object>
			   <object id="4" x="65" y="197">
				<point/>
			   </object>
			  </objectgroup>
			 </tile>
			*/
			// tile[0]からロードしていく

			//
			// タイル情報ロードループ
			auto test = tag; // <tile>

			// element.nextSibling()でループさせて@@@ 
			if (tag == U"tile") //<tile～>内すべて
			{
				int id;
				String type;
				auto attrs = element.attributes();
				bool ok = TryParse<int>(element.attribute(U"id").value(), id);
				++id;// エディター上での表記が1ずれているので、それに合わせるため補正
				_ASSERT_EXPR(ok, L"int変換できない文字列です。 ");

				tile[id].Set(element);
				element = element.nextSibling();

#ifdef _DEBUG
				tag = element.name();

#endif // _DEBUG

			}


			// image にも追加データがある場面もある
			//<tile id = "16">
			//	<image width = "1133" height = "895" source = ".png/Enemy10.png" / >
			//	<objectgroup draworder = "index" id = "2">
			//	<object id = "1" x = "555" y = "505">
			//	<point / >
			//	< / object>
			//	< / objectgroup>
			//	< / tile>
			//<image width = "3100" height = "1050" source = ".png/Enemy15.png" / >
			//	<objectgroup draworder = "index" id = "2">
			//	<object id = "1" x = "2710" y = "768">
			//	<point / >
			//	< / object>

	//		assert(false && "デバグテスト");
			element = element.nextSibling();
		}
	}


	// 消す予定
	// ----------------  TSX ファイルのロード  ------------------
	// <Tileset属性 tilewidth tileheight tilecount　columns　width　height　source > の値をXMLから設定
	// TSXファイルがある場合のみ呼ばれる。微妙に設定値の種類が変わるので
	// 頭から通しで設定 <tileset ～>
	void TSXFile_Set_TilesetAttr_and_Tile(XMLReader& tsx_xml)
	{
		// <tileset 属性の設定
		XMLElement& tilesetElm = tsx_xml;
		version =		tilesetElm.attribute(U"version").value_or(U"");
		tiledversion =	tilesetElm.attribute(U"tiledversion").value_or(U"");
		name = tilesetElm.attribute(U"name").value_or(U"");
		tilewidth	=	Parse<int>(tilesetElm.attribute(U"tilewidth").value());
		tileheight	=	Parse<int>(tilesetElm.attribute(U"tileheight").value());
		tilecount	=	Parse<int>(tilesetElm.attribute(U"tilecount").value());
		columns		=	Parse<int>(tilesetElm.attribute(U"columns").value());
		// grid image Chack
		auto element = tilesetElm.firstChild();

		// <tileset>中身</tileset>
		//  中身の設定
		// 
		auto elementAttrs = element.attributes(); //debug
		auto tt = elementAttrs[0].first;	//db

		auto tileElm = element.nextSibling();	//db
		auto tileElmName = tileElm.name();	//db
		auto tileAttrs = tileElm.attributes();	//db

		//auto tileElm = element.firstChild();

		while (element)
		{
			auto tag = element.name();

			// <tileset>の中、<grid> か <image> か <tile> で分岐する
			if (tag == U"image") // <image >
			{
				image.width = Parse<int>(element.attribute(U"width").value());
				image.height = Parse<int>(element.attribute(U"height").value());
				image.source = element.attribute(U"source").value();

				TMXfile::SetPNGFolder(image.source); // .pngフォルダ階層があればフォルダ名のみ取得
			}
			if (tag == U"grid")	// <grid>
			{
				//<grid orientation = "orthogonal" width = "1" height = "1" / >
	//			XMLElement* xml=&element;
				grid.ReadAttribute(element);	//@@@ <grid orientation="????" width="?" height="?"/>
			}
			/*
			<tile id="0" type="種類" probability="3">
			  <image width="200" height="200" source=".png/SavePoint.png"/>
			  <objectgroup draworder="index" id="2">
			   <object id="1" x="44" y="27">
				<polygon points="0,0 72,134 100,60"/>
			   </object>
			   <object id="2" x="11" y="10" width="169" height="149"/>
			   <object id="3" x="83" y="43" width="80" height="111">
				<ellipse/>
			   </object>
			   <object id="4" x="65" y="197">
				<point/>
			   </object>
			  </objectgroup>
			 </tile>
			*/
			// tile[0]からロードしていく

			//
			// タイル情報ロードループ
			auto test = tilesetElm.name(); // <tile>
			int a = 0;

			// element.nextSibling()でループさせて@@@ 
			while (tag == U"tile") //<tile～>内すべて
			{
				Tile tileTmp;
				int id, probability;
				String type;
				auto attrs = element.attributes();
				bool ok = TryParse<int>(element.attribute(U"id").value(), id);
				++id;// エディター上での表記が1ずれているので、合わせるため補正
				_ASSERT_EXPR(ok, L"int変換できない文字列です。 ");
				tile[id] = Tile(id);
				auto typeAttr = element.attribute(U"type");
				String typeAttrStr = element.attribute(U"type").value();
				String test = typeAttr.value_or(U"xxxxxxxx");

				if (typeAttr.has_value())
					tile[id].type = typeAttr.value();
				else
				{
					assert(false);
				}
				auto probabilityAttr = element.attribute(U"probability");
				if (probabilityAttr.has_value())
					TryParse<int>(probabilityAttr.value(), tile[id].probability);
				//tile[tileid].ReadAttribute(childElement);

				//>>>>
				// <tile属性は無いのもあるので注意> >>>>
				// id=絶対ある

			}


			// image にも追加データがある場面もある
			//<tile id = "16">
			//	<image width = "1133" height = "895" source = ".png/Enemy10.png" / >
			//	<objectgroup draworder = "index" id = "2">
			//	<object id = "1" x = "555" y = "505">
			//	<point / >
			//	< / object>
			//	< / objectgroup>
			//	< / tile>
			//<image width = "3100" height = "1050" source = ".png/Enemy15.png" / >
			//	<objectgroup draworder = "index" id = "2">
			//	<object id = "1" x = "2710" y = "768">
			//	<point / >
			//	< / object>

	//		assert(false && "デバグテスト");
			element = element.nextSibling();
		}
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

// 絶対に正の数であまりを出す
// 引数：割られる数、割る数(正の数限定)　　戻り値：余り（正の数）
inline int UnsignedMod(int a, int b)
{
	assert(b >= 0 && "割る数は正の数のみ");
	int mod = a % b;
	if (mod < 0)mod += b;
	return mod;
}
// 絶対に正の数であまりを出す
//　引数：割られる数、割る数(正の数限定)　　戻り値：余り（正の数）
inline int UMod(int a, int b)
{
	return UnsignedMod(a, b);
}

//
// 2019 09 17 矩形の角の4点のタイル情報保存用構造体
// constにしていないのは加工しても使えるようにするため
struct RectEdgeTileNumber
{
	int leftTop;
	int rightTop;
	int leftBottom;
	int rightBottom;
	RectEdgeTileNumber() {};
	RectEdgeTileNumber(int leftTop, int rightTop, int leftBottom, int rightBottom)
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
	/// <summary>
	/// checkTileNumber と矩形頂点4点すべて同じか
	/// </summary>
	/// <param name="checkTileNumber">4点チェックしたいタイル番号</param>
	/// <returns>すべて checkTileNumber なら true</returns>
	bool Edge4sameCheck(int checkTileNumber)
	{
		return Edge4sameCheck() == checkTileNumber;
		//			&& leftTop == rightTop && rightTop == rightBottom && leftBottom == rightBottom;
	}
	/// <summary>
	/// 矩形4点のタイル番号がすべて同じかどうか
	/// </summary>
	/// <returns>同じならそのタイル番号、違うなら、-1</returns>
	int Edge4sameCheck()
	{
		return
			leftTop == rightTop && rightTop == rightBottom && leftBottom == rightBottom ?
			leftTop : -1;
	}
	// 矩形4点のタイル番号がすべて指定値と違うか？
	// 引数： 指定値
	// 戻り値：指定値とすべて違うなら true
	bool Edge4DifferentNumber(int tileNumber)
	{
		return
			leftTop != tileNumber && rightTop != tileNumber
			&& leftBottom != tileNumber && rightBottom != tileNumber;
	}
};



/// <summary>
/// TMXmap: このクラスで、TMXファイルを指定してインスタンスを設定する
///
/// 使い方例：	TMXmap tiled(U"mylevel1csv.tmx");
/// Appフォルダ内（もしくは指定フォルダ内）にTMXとTSXと画像のファイルを置くこと
/// String やそれ系は重いので仕方なく、最小限にとどめるため高速化のため別変数に代入した
/// </summary>
class TMXmap
{
private:
	inline static bool tileAnimationFlag;// 未使用
public:
	inline static Stopwatch tileAnimationTimer;
	/// <summary>
	/// タイルのアニメーションスタート
	/// </summary>
	void TileAnimationStart()
	{
		tileAnimationTimer.start();
	}

	Map map;
	Array<Tileset> tileset;
	Array<Layer> layer;	// Array 全タイル情報がレイヤーごとに入っている
	Array<ObjectGroup> objectGroup;	// 中でObject_のArray 　　objectGroupも配列化が必要？※
	ObjectGroup& ObjGroup(int i = 0)
	{
		_ASSERT_EXPR(i < objectGroup.size() , L"objectGroup[i] が範囲内ではないです！");
		return objectGroup[i];
	}
	// オブジェクトグループ内のオブジェクトの取得
	Object_& Object(int group, int i)
	{
		return objectGroup[group][i];
	}
	Data* GetData(int layerId = 0)
	{
		return &(layer[layerId].data);
	}
	String texAsset;
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
		_ASSERT_EXPR(FileSystem::IsFile(tmxFile), L"TMXファイルが見つかりません。");
		TMXfile::SetTMXFolder(tmxFile); // フォルダ階層があればフォルダ名のみ取得

		XMLReader tmx((FilePathView)tmxFile);
		map.AttributeSet(tmx);//<map ~>
		XMLElement readElement = tmx.firstChild();
#ifdef _DEBUG
		String readName = readElement.name();
		String tag = readElement.name();
		auto attrs = readElement.attributes(); //debug
#endif
		while (readElement.name() == U"tileset") //<tileset～>内すべて
		{
			tileset.emplace_back(readElement);

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
		/////////////////////////////////////////////////////////////////////////////////////
		////////////////// TSX ファイルのロード /////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////
		for (auto& ts :tileset)
		{
			ts.TSXfileSet();
		}


		///////////////////////////////////////////////////////////////
		texAsset = TextureFilename();
		for (auto& a : tileset)
		{
			String source = TMXfile::SetPNGName(a.image.source);
			_ASSERT_EXPR(FileSystem::IsFile(FilePath(source)), L"タイルの画像が読み込めません。ファイル名ミスの可能性があります。");
			TextureAsset::Register(source, source);
			a.image.tileTexture = TextureAsset(source);
		}

		scrollPosition = { 0,0 };
		screenArraySize = ToMapArrayPos(Window::ClientSize());

		// マップロードが終わったのでアニメーションタイマー開始
		TileAnimationStart();
		// この後、いろいろやって時間がずれるなら、再度、TileAnimationStart();を呼ぶといい
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
			(tileset[tilesetNumber].tile.count(tileID) == 0) ?
			Array<Property>() :
			tileset[tilesetNumber].tile[tileID].properties;
	}
	// 配列が多くややこしいので、現状は[0]を規定とする　※
	// タイルのプロパティ情報の取得
	Property GetProperty(int tileID, int tilesetNumber = 0)
	{
		if (tileID >= (int)tileset.size() || tileID < 0)
			return Property();
		ErrorWindow(tileset.size() <= tilesetNumber || tilesetNumber < 0, U"tilesetNumberが配列外です。");
		auto& tmap = tileset[tilesetNumber].tile;
		if (tmap.count(tileID) == 0)return Property();
		return tileset[tilesetNumber].tile[tileID].properties[0];
	}

	// タイル番号　表示座標
	void TileDraw(int tileNumber, int x, int y)
	{
		if (tileNumber == 0 || tileNumber == -1)return;
		//ここで　タイル画像番号、描画用番号、を計算
		int tileLayer;
		for (tileLayer = (int)tileset.size() - 1; tileLayer > 0; --tileLayer)
		{
			if (tileNumber >= tileset[tileLayer].firstgid)
				break;
		}
		assert(tileLayer >= 0 && " タイル番号が異常です ");

		// アニメーションタイルのチェック 開発中
		// 
		if(tileset[tileLayer].tile[tileNumber].IsAnimation())
		{
			auto elapset=TMXmap::tileAnimationTimer.ms64();

		}


		auto& tile = tileset[tileLayer];
		tile.image.tileTexture
		(GetTileRect(tile.firstgid, tileNumber, tileLayer)).draw(x, y);	// ※ 下のは配列外エラーになる

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
	Rect GetTileRect(int firstGid, int tileNumber, int layerId = 0)
	{
		int tn = tileNumber - firstGid;
		return { tn % tileset[layerId].columns * TileWidth(), tn / tileset[layerId].columns * TileHeight(), TileWidth(), TileHeight() };
	}
	// layerIdが存在するか?
	bool IsLayerID(int layerId)
	{
		return (size_t)layerId < layer.size() && layerId >= 0;
	}
	// layerIdが存在するか?
	int TryLayerID(int layerId)
	{
		int layerSize = (int)layer.size();
		if (!(layerId < layerSize) || layerId < 0)
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
	// ローカル座標（標示座標）の取得 引数：ピクセル座標　　戻り値：ワールド座標
	Point WorldToLocalPosition(Vec2 pixelPos)
	{
		return pixelPos.asPoint() - scrollPosition;
	}
	// 円筒スクロール座標変換 引数：ワールド座標　戻り値：マップの配列座標
	Point RollArrayPosition(Point pos)
	{
		return {
			UMod(pos.x, MapWidth() * TileWidth()) / TileWidth(),
			UMod(pos.y, MapHeight() * TileHeight()) / TileHeight()
		};
	}
	// 円筒スクロール座標変換 引数：ワールド座標　戻り値：マップの配列座標
	Point RollArrayPosition(Vec2 pos)
	{
		return RollArrayPosition(pos.asPoint());
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

	// 説明：左上 右上 左下 右下 のタイル番号の取得
	// 引数：調べる矩形情報  マップレイヤーのID
	// 戻り値：４つの頂点の当たっているタイル情報
	// 　layerId　が範囲外だと落ちます。
	RectEdgeTileNumber GetRect4Position(Rect rect, int layerID = 0)
	{
		RectEdgeTileNumber hit(
			GetPixelPosTileNumber(rect.pos, layerID),
			GetPixelPosTileNumber(rect.right().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().end.asPoint(), layerID)
		);
		return hit;
	}
	// 説明：矩形の頂点のタイル番号の取得してすべて同じ値ならその数値を返す
	// 引数：調べる矩形情報  マップレイヤーのID
	// 戻り値：４つの頂点のタイル情報が全く同じなら、その値。
	//　　　　 そうでないなら、-1。
	// 　layerId　が範囲外だと落ちます。
	int GetSame4numberorNot(Rect rect, int layerID = 0)
	{
		return RectEdgeTileNumber(
			GetPixelPosTileNumber(rect.pos, layerID),
			GetPixelPosTileNumber(rect.right().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().end.asPoint(), layerID)
		).Edge4sameCheck();
	}
	// 説明：矩形の頂点のタイル番号の取得してタイル番号とすべて違うか
	// 引数：調べる矩形情報  タイル番号　マップレイヤーのID
	// 戻り値：４つの頂点のタイル情報が全く違うなら true
	//
	// 　layerId　が範囲外だと落ちます。
	bool GetDifferent4Number(Rect rect, int tileNumber, int layerID = 0)
	{
		return RectEdgeTileNumber(
			GetPixelPosTileNumber(rect.pos, layerID),
			GetPixelPosTileNumber(rect.right().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().end.asPoint(), layerID)
		).Edge4DifferentNumber(tileNumber);
	}
	// 左上 右上 左下 右下 のタイル番号の取得
	// 引数：スプライト  マップレイヤーのID  　　戻り値：４つの頂点の当たっているタイル情報
	// 　layerId　が範囲外だと落ちます。
	//Array<int> GetPixel4Position(GameSprite&& spr, int layerID = 0)
	RectEdgeTileNumber GetPixel4Position(GameSprite&& spr, int layerID = 0)
	{
		Rect rect = spr.GetAnimation()->hit;
		rect.x += spr.iX(); rect.y += spr.iY();
		return GetRect4Position(rect, layerID);
	}
	// １フレーム先の、左上 右上 左下 右下 のタイル番号の取得
	// 引数：スプライト  マップレイヤーのID  　　戻り値：４つの頂点の当たっているタイル情報
	// 　layerId　が範囲外だと落ちます。
	//Array<int> GetPixel4PositionAddVelocity(GameSprite& spr, int layerID = 0)
	RectEdgeTileNumber GetPixel4PositionAddVelocity(GameSprite& spr, int layerID = 0)
	{
		Rect rect = spr.GetAnimation()->hit;
		rect.x += (int)(spr.iX() + spr.velocity.x); rect.y += (int)(spr.iY() + spr.velocity.y);
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
	Point Shift(Vec2 scroll)
	{
		Point _shift(-UMod((int)scroll.x, TileWidth()), -UMod((int)scroll.y, TileHeight()));
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
	// スクロールスピードでスクロール座標を計算
	Point ScrollSpeedCalc(Vec2 scrollSpeed)
	{
		return Point((int)(scrollPosition.x * scrollSpeed.x), (int)(scrollPosition.y * scrollSpeed.y));
	}

	// 円筒スクロールのマップ描画
	void MapRollDraw(int layerIndex,Vec2 scrollSpeed = Vec2(1,1))
	{
		// 円筒スクロールなので、無理やりマップの範囲内に収める計算をする
		Point scroll=ScrollSpeedCalc(scrollSpeed);
		Point leftTop = RollArrayPosition(scroll);

		Point drawHomePos = Shift(scroll);
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
	void MapDraw(int layerIndex = 0, Vec2 scrollSpeed = Vec2(1,1))
	{
		
		// 円筒スクロールは確実に動作するので、苦肉の策でこれを元にした
		Point scroll = ScrollSpeedCalc(scrollSpeed);
		Point leftTop = RollArrayPosition(scroll);
		if (scroll.x < 0)leftTop.x -= MapWidth();
		if (scroll.y < 0)leftTop.y -= MapHeight();

		Point drawHomePos = Shift(scroll);
		Point drawPos = drawHomePos;

		assert((int)layer.size() > layerIndex&& layerIndex >= 0 && "MapDraw(int layerIndex) でlayerが範囲を超えています。");

		//for (int layerIndex =0 ;layerIndex<layer.size(); ++layerIndex)
		//{
		int top = leftTop.y, left = leftTop.x;
		//		for (int y = leftTop.y; y < leftTop.y + screenArraySize.y + 2; y++)
		for (int y = top; y < top + screenArraySize.y + 2; y++)
		{
			drawPos.x = drawHomePos.x;
			//			for (int x = leftTop.x; x < leftTop.x + screenArraySize.x + 2; x++, drawPos.x += TileWidth())
			for (int x = left; x < left + screenArraySize.x + 2; x++, drawPos.x += TileWidth())
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
	// Objectレイヤーの表示、デバッグ表示用、確認用 objectGroupが無い場合、表示されない
	void ObjectDraw(int i = 0)
	{
		if (!objectGroup.size()) return;
		assert(i < objectGroup.size() && "objectGroup が範囲外");
		for (auto&& ob : objectGroup[i].object)
		{
			Point drawPos = WorldToLocalPosition(Point((int)ob.x, (int)ob.y));
			TileDraw(ob.gid, drawPos.x, drawPos.y);
		}
	}
	// ObjectGroupのnameから検索してobject配列を取得, 無いとエラーで落ちます。
	// 引数を入れない場合、配列０番目を戻します。
	Array<Object_>& ObjectArray(String objectGroupName = U"")
	{
		if (objectGroupName == U"")
			return objectGroup[0].object;
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
		return Rect(scrollPosition.x, scrollPosition.y, Window::ClientSize()).intersects(pos);
	}
	// スプライトの描画範囲が描画面内かチェック
	// 引数：座標　　戻り値
	bool InScreen(GameSprite spr)
	{
		Rect drawRect(spr.position.asPoint(), spr.GetAnimation()->draw.w, spr.GetAnimation()->draw.h);
		return Rect(scrollPosition.x, scrollPosition.y, Window::ClientSize()).intersects(drawRect);
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
