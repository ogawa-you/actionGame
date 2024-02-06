#pragma once
#include <Siv3D.hpp>
#include "GameSprite.h"

// 2019 10 15 �I�u�W�F�N�g���C���[�Ő������]�@�������]�@��]�̏��ǉ��A�����^�C���ǉ��\ Parse<uint> ���g�p
// 2019 10 16 �I�u�W�F�N�g���C���[��z��
// 2019 10 17 �I�u�W�F�N�g���C���[�̌������\�b�h�ǉ��Aname�̂���z��ԍ���������FindObjectArray(name)
//  �@�@�@�@�@name�̂���z��������� FindObjectArray(name)
// 2019 10 21 �t�H���_����TMX�t�@�C���̃��[�h�\�ɂ����B�t�@�C�����[�h�G���[�����̒ǉ�
// 2020 01 23 type�����ǉ��Agid==0 �̎��AY���W�␳���s�v�ɂȂ�悤�ɕύX�A���C���[���Ƃ̃X�N���[���X�s�[�h�ݒ�@�\�ǉ�
// 2020 2  14 �T�[�o�N�����Ɠ��삵�Ȃ��o�O���C���i���m�F�j�B
// 2020/11/30 TSX�t�@�C���p�X�o�O�C���i���{�j
// 2020/12/8 TSX�t�@�C����Tile��<image>�^�O�Ή��ύX��

//#define ERROR(fmt, ...) err_msg(__FILE__, __FUNCTION__, __LINE__, "error", fmt, ##__VA_ARGS__)

template <class Type>
/// <summary>
/// String���������T���ɕϊ����đ������B�ł��Ȃ��ꍇ�͉������Ȃ��B
/// </summary>
/// <typeparam name="Type">�^</typeparam>
/// <param name="str">�ϊ����镶����</param>
/// <param name="value">�������ϐ�</param>
/// <returns>�ϊ��ł���� true</returns>
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
/// �A�g���r���[�g(����)������Ύw��̕ϐ��ɑ�� ��Element attribute = attribute value��
/// </summary>
/// <param name="attribute">attribute(����)</param>
/// <param name="fix">���������String�ϐ�</param>
/// <param name="element">XML�� Element</param>
inline void ReadAttrbuteValue(const String attribute, String& fix,XMLElement& element)
{
	const auto attr = element.attribute(attribute);
	if (attr.has_value())
		fix = attr.value();
}

/// <summary>
/// �A�g���r���[�g(����)������Ύw��̕ϐ��ɑ�� ��Element attribute = attribute value��
/// �����łȂ������牽�����Ȃ��Bint�ϊ��ł��Ȃ�������G���[�Ŏ~�܂�̂Œ���
/// </summary>
/// <param name="attribute">attribute(����)</param>
/// <param name="fix">���������int�ϐ�</param>
/// <param name="element">XML�� Element</param>
inline void ReadAttrbuteValueInt(const String attribute, int& fix,XMLElement& element)
{
	const auto attr = element.attribute(attribute);
	_ASSERT_EXPR(TryParse<int>(attr.value(), fix),
		L"ReadAttrbuteValueInt(�`�`�`)�� int�ϊ��ł��܂���ł����B");
}

//Parse<int>(xmlElement.attribute(U"gid").value_or(U"0")),	// gid=0�͒ʏ�Ȃ�




class Tileset;class Tile;
class Object_;
class TMXfile	// todo
{
private:
	TMXfile() {} //�C���X�^���X���������Ȃ��悤�ɂ킴�� private�ɂ��Ă���
public:
	static inline String TMXFolder;
	static inline String TSXFolder;
	static inline String PNGFolder;
	// 
	// TMXFile::TMXFolder �� .tmx �̃t�H���_�p�X���擾
	//
	// �����F�t�@�C���l�[��, �K�w�i�ʏ�͂O�̂܂܁j
	// �߂�l�����@�� TMXFile::TMXFolder �ɃZ�b�g�����
	// ��F U"abcd/efg.tmx" >> TMXFile::UseFolder=U"abcd/"
	static void SetTMXFolder(String fileName, int parent = 0)
	{
		TMXFolder = fileName.substr(parent, fileName.lastIndexOf(U'/') + 1);
	}

	// 
	// TMXFile::TSXFolder �� .tsx �̃t�H���_�p�X���擾
	//
	// �����F�t�@�C���l�[��, �K�w�i�ʏ�͂O�̂܂܁j
	// �߂�l�����@�� TMXFile::TSXFolder �ɃZ�b�g�����
	// ��F U"abcd/efg.tmx" >> TMXFile::UseFolder=U"abcd/"
	static void SetTSXFolder(String fileName, int parent = 0)
	{
		TSXFolder = TMXFolder + fileName.substr(parent, fileName.lastIndexOf(U'/') + 1);
	}

	// 
	// TMXFile::PNGFolder �� .tmx �̃t�H���_�p�X���擾
	//
	// �����F�t�@�C���l�[��, �K�w�i�ʏ�͂O�̂܂܁j
	// �߂�l�����@�� TMXFile::PNGFolder �ɃZ�b�g�����
	// ��F U"abcd/efg.tmx" >> TMXFile::UseFolder=U"abcd/"
	static void SetPNGFolder(String fileName, int parent = 0)
	{
		PNGFolder = TSXFolder + fileName.substr(parent, fileName.lastIndexOf(U'/') + 1);
	}

	// 
	// �t�H���_�t���t�@�C�����擾
	//
	// �����F�t�@�C���l�[��
	// �߂�l�F�iTMXFile::TSXFolder + fileName�j
	// ��F U"efg.tsx" >> TMXFile::TSXFolder=U"abcd/efg.tsx"�iU"abcd/"�͊��ɂ���O��j
	static String SetTSXName(String fileName)
	{
		return TMXfile::TMXFolder + fileName;
	}

	// 
	// �t�H���_�t���t�@�C�����擾
	//
	// �����F�t�@�C���l�[��
	// �߂�l�F�iTMXFile::PNGFolder + fileName�j
	// ��F U"efg.png" >> TMXFile::PNGFolder=U"abcd/efg.png"�iU"abcd/"�͊��ɂ���O��j
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
const unsigned JUST_GID_MASK = 0xFFFF; // ����͐���
// https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#object

enum class ObjectValue
{
	null, point, ellipse, polygon, text,
};

// �R�C����A�C�e����G�Ȃǂ̃X�v���C�g���
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
			Parse<int>(xmlElement.attribute(U"gid").value_or(U"0")),	// gid=0�͒ʏ�Ȃ�
			Parse<double>(xmlElement.attribute(U"x").value()),
			Parse<double>(xmlElement.attribute(U"y").value()),	// �����ō�������������������Ă���
			Parse<double>(xmlElement.attribute(U"width").value_or(U"0")),	//error�����ꍇ������
			Parse<double>(xmlElement.attribute(U"height").value_or(U"0")),	//error�����ꍇ������
			Parse<double>(xmlElement.attribute(U"rotation").value_or(U"0")),
			childelm.name()	//object�^�O���̎q�K�w�ɂ���Ƀf�[�^�����邩�H
			//value	//TODO******************************************************************
		);
		// object �Œǉ��f�[�^������ꍇ
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
	// object�̏������A���ӁFvalueName�������Ă��A����ɂ��̃f�[�^�̒ǉ��܂ł͂����ł͂��Ȃ�
	void Init(int _id = 0, String _name = U"", String _type = U"", int _gid = 0, double _x = 0, double _y = 0, double _width = 0, double _height = 0, double _rotation = 0, String valueName = U"")
	{
		id = _id;
		name = _name;
		type = _type;
		Flipped_Horizonally = (_gid & FLIPPED_HORIZONTALLY_FLAG) != 0;
		Flipped_Vertically = (_gid & FLIPPED_VERTICALLY_FLAG) != 0;
		Flipped_Diagonally = (_gid & FLIPPED_DIAGONALLY_FLAG) != 0;
		gid = _gid & JUST_GID_MASK;
		//assert(_gid<1000);	// name�^�O�����ݒ肾�ƃG���[���������C���ς�
		x = _x;
		y = gid ? (_y - _height) : _y;// gid������΁A�������AY���W������(TiledmapEditor �̃o�O�H�d�l�H)
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
	int gid;// �O �Ȃ瑶�݂��Ȃ�
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
	//String draworder;		//>>>>�ǉ��d�l
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
		// sample2 <objectgroup draworder="index" id="2">������
		id = Parse<int>(readElement.attribute(U"id").value()), name = readElement.attribute(U"name").value();
		ObjectArraySet(readElement);
		//readElement = readElement.parent().nextSibling();
	}
	void ObjectArraySet(XMLElement& readElement) //void ObjectArraySet(XMLElement XMLelement)
	{
		XMLElement objectElement = readElement.firstChild(); // object�ɂ��� //;

		while (objectElement.name() == U"object")
		{
			object.emplace_back(objectElement);

			objectElement = objectElement.nextSibling();
		}
	}
	// object[index] ���擾
	Object_& operator[](int index)
	{
		assert(index < object.size() && "object[index]���͈͊O�ł�");
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
	// <image width="200" height="200" source=".png/SavePoint.png"/> �̃��[�h
	// element �� nextSibiling �ɕω�����̂Œ��� //@@@
	void ReadElement(XMLElement& element)
	{
		source = element.attribute(U"source").value();
		width = Parse<int>(element.attribute(U"width").value());
		height = Parse<int>(element.attribute(U"height").value());
		element = element.nextSibling();
	}
	void Set(String _source, int _width, int _height)
	{
		source = _source, width = _width, height = _height;
		if (source != U"")TileTextureSet();
	}
	// �^�C���̃e�L�X�`���[���Z�b�g�@��
	void TileTextureSet()
	{
		TextureAsset::Register(source, source);
		for (size_t i = 0; !TextureAsset::IsReady(source); assert(++i < 1000000000));
		// ���O�͑��v���H
		tileTexture = TextureAsset(source);
	}
	void TileTextureRelease()
	{
		TextureAsset(source).release();
	}
	// <image>�^�O�����邩�H
	bool IsUse()
	{
		return source != U"";
	}
};
// XML�f�[�^�̐e��  // compressionlevel �ǉ� 2020/12/17
//  compressionlevel �͂Ȃ��ꍇ������
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

	void Set(XMLReader& tmx)
	{
		version = ParseFloat<float>(tmx.attribute(U"version").value());
		tiledversion = tmx.attribute(U"tiledversion").value();
		orientation  = tmx.attribute(U"orientation").value();
		renderorder  = tmx.attribute(U"renderorder").value();
		compressionlevel = Parse<int>(tmx.attribute(U"compressionlevel").value_or(U"0")); // �����ꍇ������
		width	= Parse<int>(tmx.attribute(U"width").value());
		height	= Parse<int>(tmx.attribute(U"height").value());
		tilewidth	= Parse<int>(tmx.attribute(U"tilewidth").value());
		tileheight	= Parse<int>(tmx.attribute(U"tileheight").value());
		infinite	= Parse<int>(tmx.attribute(U"infinite").value());
		nextlayerid = Parse<int>(tmx.attribute(U"nextlayerid").value());
		nextobjectid = Parse<int>(tmx.attribute(U"nextobjectid").value());
	}
};
// Property�^��Type�p bool,color,float,file,int,string
enum class Type
{
	null, bool_, color, float_, file, int_, string
};
///*�@<image/>
//	<tile id="9">
//		<properties>
//			<property name="ground" type="bool" value="true"/>
//		</properties>
//	</tile>

/*
	String name,typeStr,value;
	Type type;
	�^�C���̑������Astd::map �ŊǗ����Ȃ��Ɩʓ|�ɂȂ�
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
	// <properties>�ȉ��Ɖ��w��<property>�z��̃��[�h���ē����
	// propertyElm �� ���Ɉړ����܂��B//@@@@ 
	void SetAttribute(XMLElement& propertyElm)
	{
			auto _name = propertyElm.attribute(U"name").value();
			auto __type = propertyElm.attribute(U"type").has_value() ?
				propertyElm.attribute(U"type").value() : U"string";
			auto _value = propertyElm.attribute(U"value").value();
			SetAttribute(_name,__type,_value);
			propertyElm = propertyElm.nextSibling();
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
	// �����Ȃ����H �Ȃ���true
	bool IsNull()
	{
		return type == Type::null;
	}
	// ���m�F
	bool ValueBool()
	{
		return ParseBool(value);
	}
	// ���m�F
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
/*	// <properties>�ȉ��Ɖ��w��<property>�z��̃��[�h����tile�A�z�z��ɓ���� // �폜�\��@@@@@
	void XMLRead(XMLElement& propertiesArrayElm, std::map<int, Tile>& tile, int tileID)
	{
		XMLElement propertyElm = propertiesArrayElm.firstChild();
		while (propertiesArrayElm.name() == U"properties" && propertyElm.name() == U"property") //<tileset�`>�����ׂ�
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
	// <properties>�ȉ��Ɖ��w��<property>�z��̃��[�h����tile�A�z�z��ɓ����
	// propertyElm �� ���Ɉړ����܂��B//@@@ 
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


// �f�[�^�d�l�͕ω�����̂Œ���
class Tile
{
public:
	// attribute
	int id;
	String type;
	int probability;
	// TileValue
	Array<Property> properties;
	Image_ image; // �摜�P�����ݒ�ł���
	ObjectGroup objectGroup;


	Tile()
	{
		Clear();
	}
	Tile(int _id)
	{
		Clear();
		Tile::id = _id;
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
		objectGroup;
		probability = 0;
		type = U"";
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
	<tile id = "0" type = "���" probability = "3">
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

	// sample1 <tile id = "0" type = "���" probability = "3">
	// sample2 <tile id="2">

	// <tile> elementValue </tile> �̃��[�h
	// element �� nextSibiling �ɕω�����̂Œ���
	void ReadElementValue(XMLElement& element)
	{
		assert(false);
		//<properties>	<<<<
		//<image> <objectgroup>
		// 
	}
	// <tile id = "����" type = "???" probability = "����"> �̃��[�h
	// element �� nextSibiling �ɕω�����̂Œ���
	void ReadAttribute(XMLElement& element)
	{
		//assert(false);
		//auto FixAttribute = [&](String attribute,String &str)
		//{
		//	auto attrName = element.attribute(attribute);
		//	if (attrName.has_value())
		//		type = attrName.value();
		//};

		auto TrySetElementIntValue = [&](String name,int &value)
		{
			bool parse = TryParse<int>(element.attribute(name).value(), value);
			_ASSERT_EXPR(parse, L"int�ϊ��ł��Ȃ�������ł��B ");
		};
		auto TrySetStringValue = [&](String name,String &value)
		{
			auto elementAttr = element.attribute(name);
			if(elementAttr.has_value())
					value = elementAttr.value();
		};

		_ASSERT_EXPR(element.attribute(U"id").has_value(),L" \"Tile id\" �� ���݂��܂���");
		TrySetElementIntValue(U"id", Tile::id);
		TrySetStringValue(U"type", Tile::type);
		TrySetElementIntValue(U"probability", Tile::probability);

//		TryParse<int>()
		//auto value = element.attribute(U"orientation").value();
		//orientation = value == U"isometric" ? Orientation::Isometric : Orientation::Orthogonal;
		//width = Parse<int>(element.attribute(U"width").value());
		//height = Parse<int>(element.attribute(U"width").value());
		//element = element.nextSibling();
	}
};

enum class Orientation // orthogonal or isometric, defaults to orthogonal)
{
	Orthogonal,Isometric
};
// sample   TSX �ɂ���
// <grid orientation="orthogonal" width="1" height="1"/>

// Grid�͔���Ă���̂ŁAGrid_
class Grid_
{
public:
	Orientation orientation;
	int width, height;
	Grid_() :orientation(Orientation::Orthogonal),width(0),height(0)
	{}
	// <grid orientation="?????" width="???" height="??"/> �̃��[�h
	// element �� nextSibiling �ɕω�����̂Œ���
	void ReadAttribute(XMLElement& element )
	{
		auto value = element.attribute(U"orientation").value();
		orientation = value == U"isometric" ? Orientation::Isometric : Orientation::Orthogonal;
		width = Parse<int>(element.attribute(U"width").value());
		height = Parse<int>(element.attribute(U"height").value());
		element = element.nextSibling(); //>>>>
	}
};


// sample
//	<tileset version = "1.2" tiledversion = "1.2.4" name = "13_bank(CUnet)(scale)(x4.000000)" tilewidth = "64" tileheight = "64" tilecount = "33" columns = "11">
//		<image source = "13_bank(CUnet)(scale)(x4.000000).png" width = "704" height = "192" / >
//		< / tileset>

// �^�C���f�[�^�ꎮ�Ǘ�
class Tileset
{
public:
	String version, tiledversion;
	int firstgid;
	// TSX �t�@�C���p�X
	String source // ��TSX�t�@�C���œǂݍ��ޏꍇ�t�@�C����������B ����ȊO�g��Ȃ�
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
		SetElementValue(tmxElement);
	}
	// TSX�t�@�C�������݂��邩�@���遁true
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
	// <tileset �`>
	//TMX   firstgid="1" name="stage1tile1" tilewidth="32" tileheight="32" tilecount="192" columns="8">
	//TSX   version="1.2" tiledversion="1.3.0" name="ObjectElement" tilewidth="3100" tileheight="1050" tilecount="35" columns="0">
	//TMX(TSX����) <tileset firstgid="1" source="../TileData/ObjectElement.tsx"/>

	// TMX�t�@�C���� <tileset �`Attribute�`> Attribute��ǂݎ��A�ݒ肷��
	// TSX�t�@�C���̏ꍇ�A�����I�ɐ؂�ւ���
	void ReadAttributeValue(XMLElement& tilesetElm)
	{
		// �K�����TMX�t�@�C�������[�h
		// firstgid �� source �݂̂Ȃ� TSX�t�@�C��(source)���[�h ����TSX����A�g���r���[�g�ǉ�
		//                TSX �ł� tileset�̒��g�̂ݐݒ肷��
		// 
		// firstgid
		auto& tileset = tilesetElm;
		auto firstgidAttr = tileset.attribute(U"firstgid");
		if(firstgidAttr.has_value())
			Tileset::firstgid = Parse<int>(firstgidAttr.value());
		auto sourceAttr = tileset.attribute(U"source");
		if (sourceAttr.has_value())
		{
			Tileset::source = sourceAttr.value();	//@@@@@
			return;		// �߂���TSX�t�@�C���̃��[�h���đ���������
		}
		else
		{	// TMX�t�@�C���̂�
			Tileset::name = tilesetElm.attribute(U"name").value();
			Tileset::tilewidth = Parse<int>(tilesetElm.attribute(U"tilewidth").value());
			Tileset::tileheight = Parse<int>(tilesetElm.attribute(U"tileheight").value());
			Tileset::tilecount = Parse<int>(tilesetElm.attribute(U"tilecount").value());
			Tileset::columns = Parse<int>(tilesetElm.attribute(U"columns").value());
		}
		return;

		//////�@�ȉ��͍폜�\��

		bool IsTSXfile = Tileset::IsSource();
		if (IsTSXfile) // TSX�t�@�C������Tileset����
		{
			Tileset::version		=	tilesetElm.attribute(U"version").value_or(U"");
			Tileset::tiledversion =	tilesetElm.attribute(U"tiledversion").value_or(U"");
			Tileset::name	=		tilesetElm.attribute(U"name").value_or(U"");
			Tileset::tilewidth	=	Parse<int>(tilesetElm.attribute(U"tilewidth").value());
			Tileset::tileheight	=	Parse<int>(tilesetElm.attribute(U"tileheight").value());
			Tileset::tilecount	=	Parse<int>(tilesetElm.attribute(U"tilecount").value());
			Tileset::columns		=	Parse<int>(tilesetElm.attribute(U"columns").value());
			return;
		}

		// TMX�t�@�C������Tileset���� TSX�t�@�C���L���ŕς��
		Tileset::firstgid = Parse<int>(tilesetElm.attribute(U"firstgid").value());
		auto TSXfilePath = tilesetElm.attribute(U"source");
		if (TSXfilePath.has_value())
		{
			Tileset::source = TSXfilePath.value();//TSX�t�@�C��������
		}
		else
		{
			Tileset::name	= tilesetElm.attribute(U"name").value();
			Tileset::tilewidth		= Parse<int>(tilesetElm.attribute(U"tilewidth").value());
			Tileset::tileheight	= Parse<int>(tilesetElm.attribute(U"tileheight").value());
			Tileset::tilecount		= Parse<int>(tilesetElm.attribute(U"tilecount").value());
			Tileset::columns		= Parse<int>(tilesetElm.attribute(U"columns").value());
		}


		//this->name = tileset.attribute(U"name").value();
//this->tilewidth = Parse<int>(tileset.attribute(U"tilewidth").value());
//this->tileheight = Parse<int>(tileset.attribute(U"tileheight").value());
//this->tilecount = Parse<int>(tileset.attribute(U"tilecount").value());
//this->columns = Parse<int>(tileset.attribute(U"columns").value());
//this-> = tileset.attribute(U"").value();

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
	}

	// TSX�t�@�C���� <tileset �`Attribute�`> Attribute��ǂݎ��A�ݒ肷��
	// TMX�t�@�C���̏ꍇ�A�G���[�ɂȂ�̂Œ���
	void ReadAttributeValueTSXfile(XMLElement& TSXfileElm)
	{
		Tileset::version = TSXfileElm.attribute(U"version").value_or(U"");
		Tileset::tiledversion = TSXfileElm.attribute(U"tiledversion").value_or(U"");
		Tileset::name = TSXfileElm.attribute(U"name").value_or(U"");
		Tileset::tilewidth = Parse<int>(TSXfileElm.attribute(U"tilewidth").value());
		Tileset::tileheight = Parse<int>(TSXfileElm.attribute(U"tileheight").value());
		Tileset::tilecount = Parse<int>(TSXfileElm.attribute(U"tilecount").value());
		Tileset::columns = Parse<int>(TSXfileElm.attribute(U"columns").value());
	}

	void SetElementValue(XMLElement& tmxElement)
	{
		// �΍�ς݂̂���F�F�F������ <tileset firstgid="1" source="../TileData/ObjectElement.tsx"/> �̂悤�Ȋ������ƃG���[
		ReadAttributeValue(tmxElement);
		auto& tileset = tmxElement;
		//if (tileset.attribute(U"source").has_value())//TSX�t�@�C��������----
		if (Tileset::IsSource())//TSX�t�@�C��������----
		{
			//Tileset::source = tileset.attribute(U"source").value();//TSX�t�@�C��������
			TMXfile::SetTSXFolder(source); // .tsx�t�H���_�K�w������΃t�H���_���̂ݎ擾

			auto TSXpath = TMXfile::SetTSXName(source); // TMX�����[�U�[�쐬�t�H���_���Ȃ�t�H���_�K�w���[���Ȃ�A�t�@�C�����Ƀt�H���_���Ă��Ȃ���΂Ȃ�Ȃ��Ȃ邽��
			_ASSERT_EXPR(FileSystem::IsFile(FilePath(TSXpath)), L"TSX�t�@�C����������܂���B");
			XMLReader TSX_xml((StringView)TSXpath);	// TSX�t�@�C���̃��[�h
			Tileset::ReadAttributeValueTSXfile(TSX_xml);

			TSXFile_Set_TilesetAttr_and_Tile(TSX_xml); // TSX�t�@�C������l���Z�b�g
			TSX_xml.close();
		}
		else //TSX�t�@�C�����Ȃ�----������tileset�̃f�[�^���Z�b�g---------------------
		{
			/* 
			sampe1
			<tileset firstgid="1" name="stage1tile1" tilewidth="32" tileheight="32" tilecount="192" columns="8">
			<image source="mylevel1_tiles.png" width="256" height="768"/>
			</tileset>
			 
			sample2
			<tileset firstgid="1" source="../TileData/ObjectElement.tsx"/>

			*/
			ReadAttributeValue(tileset);
			//this->name = tileset.attribute(U"name").value();
			//this->tilewidth = Parse<int>(tileset.attribute(U"tilewidth").value());
			//this->tileheight = Parse<int>(tileset.attribute(U"tileheight").value());
			//this->tilecount = Parse<int>(tileset.attribute(U"tilecount").value());
			//this->columns = Parse<int>(tileset.attribute(U"columns").value());
			//this-> = tileset.attribute(U"").value();
			XMLElement imageChld = tileset.firstChild();	//XMLElement& imageChld = tileset.firstChild();
			image.ReadElement(imageChld);
			//this->image.source = imageChld.attribute(U"source").value();
			//this->image.width = Parse<int>(imageChld.attribute(U"width").value());
			//this->image.height = Parse<int>(imageChld.attribute(U"height").value());
		}
		// <tileset<image> �܂Ŋ���>
		// <tile xxx >�`</tile>��ݒ�----------------------------------------<tile>
		XMLElement tileElemnt = tileset.firstChild().nextSibling();
		while (tileElemnt.name() == U"tile")
		{
			auto debugStr = tileElemnt.name();
			// 1����Ă���̂ŕ␳
			//tile.id = 1+ Parse<int>(tileElemnt.attribute(U"id").value());
			int tileID = 1 + Parse<int>(tileElemnt.attribute(U"id").value());
			
			// �����łQ���A<image>��<properties>�ŏ����𕪂��� @@@

			tile[tileID] = Tile(tileID);

			XMLElement propertiesArrayElm = tileElemnt.firstChild();// XMLElement& propertiesArrayElm = tileElemnt.firstChild()
			String propertiesArrayElmName = propertiesArrayElm.name(); //check
			XMLElement propertyElm = propertiesArrayElm.firstChild();// XMLElement& propertyElm = propertiesArrayElm.firstChild()
			String propertyElmName = propertyElm.name(); //check

			PropertiesRead(propertiesArrayElm,tileID);
			//tile[tileID].properties[0].Set(propertiesArrayElm); //NG @@@
			tileElemnt = tileElemnt.nextSibling();
		}
	}
	// <properties>�ȉ��Ɖ��w��<property>�z��̃��[�h����tile�A�z�z��ɓ����
	void PropertiesRead(XMLElement& propertiesArrayElm,int tileID)
	{
		XMLElement propertyElm = propertiesArrayElm.firstChild();
		while (propertiesArrayElm.name() == U"properties" && propertyElm.name() == U"property") //<tileset�`>�����ׂ�
		{			
			String _name = propertyElm.attribute(U"name").value();
			String _type = propertyElm.attribute(U"type").has_value() ?
				propertyElm.attribute(U"type").value() : U"string";
			String _value = propertyElm.attribute(U"value").value();
			Tileset::tile[tileID].properties.emplace_back(_name, _type, _value);

			propertyElm = propertyElm.nextSibling();
		}
	}

	// ----------------  TSX �t�@�C���̃��[�h  ------------------
	// <Tileset���� tilewidth tileheight tilecount�@columns�@width�@height�@source > �̒l��XML����ݒ�
	// TSX�t�@�C��������ꍇ�̂݌Ă΂��B�����ɐݒ�l�̎�ނ��ς��̂�
	// ������ʂ��Őݒ� <tileset �`>
	void TSXFile_Set_TilesetAttr_and_Tile(XMLReader& tsx_xml)
	{
		// <tileset �����̐ݒ�
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
		auto elementAttrs = element.attributes();
		auto tt = elementAttrs[0].first;	

		auto tileElm = element.nextSibling();
		auto tileElmName = tileElm.name();
		auto tileAttrs = tileElm.attributes();

		//auto tileElm = element.firstChild();

		while (element)
		{
			auto tag = element.name();

			// <tileset>�̒��A<grid> �� <image> �� <tile> �ŕ��򂷂�
			if (tag == U"image") // <image >
			{
				image.width = Parse<int>(element.attribute(U"width").value());
				image.height = Parse<int>(element.attribute(U"height").value());
				image.source = element.attribute(U"source").value();

				TMXfile::SetPNGFolder(image.source); // .png�t�H���_�K�w������΃t�H���_���̂ݎ擾
			}
			if (tag == U"grid")
			{
				//<grid orientation = "orthogonal" width = "1" height = "1" / >
	//			XMLElement* xml=&element;
				grid.ReadAttribute(element);	//@@@ <grid orientation="????" width="?" height="?"/>
				element.nextSibling();
			}
			/*
			<tile id="0" type="���" probability="3">
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
			// tile[0]���烍�[�h���Ă���

			//
			// �^�C����񃍁[�h���[�v
			auto test = tilesetElm.name(); // <tile>
			int a = 0;

			// element.nextSibling()�Ń��[�v������@@@ 
			while (tag == U"tile") //<tile�`>�����ׂ�
			{
				Tile tileTmp;
				int id, probability;
				String type;
				auto attrs = element.attributes();
				bool ok = TryParse<int>(element.attribute(U"id").value(), id);
				++id;// �G�f�B�^�[��ł̕\�L��1����Ă���̂ŁA���킹�邽�ߕ␳
				_ASSERT_EXPR(ok, L"int�ϊ��ł��Ȃ�������ł��B ");
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
				// <tile�����͖����̂�����̂Œ���> >>>>
				// id=��΂���

			}


			// image �ɂ��ǉ��f�[�^�������ʂ�����
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

	//		assert(false && "�f�o�O�e�X�g");
			element.nextSibling();
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
// layer �̒��ɕK�� data �������Ă���
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
		// <data>�f�[�^���Z�b�g
		data.SetXMLElement(tmxElement.firstChild()); //bug
		// �C���[�W�@int tileMap[height][width];
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
		String newMapstr = U"";// ��₱�����̂ŕςȕ���������
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
					assert(istr < mapStr.length() && "�}�b�v�̕����񂪕s�����Ă��܂�");
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

// ��΂ɐ��̐��ł��܂���o��
// �����F�����鐔�A���鐔(���̐�����)�@�@�߂�l�F�]��i���̐��j
inline int UnsignedMod(int a, int b)
{
	assert(b >= 0 && "���鐔�͐��̐��̂�");
	int mod = a % b;
	if (mod < 0)mod += b;
	return mod;
}
// ��΂ɐ��̐��ł��܂���o��
//�@�����F�����鐔�A���鐔(���̐�����)�@�@�߂�l�F�]��i���̐��j
inline int UMod(int a, int b)
{
	return UnsignedMod(a, b);
}

//
// 2019 09 17 ��`�̊p��4�_�̃^�C�����ۑ��p�\����
// const�ɂ��Ă��Ȃ��͉̂��H���Ă��g����悤�ɂ��邽��
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
	// �S�p��
	int edge[4];
	void EdgeSet()
	{
		edge[0] = leftTop;
		edge[1] = rightTop;
		edge[2] = leftBottom;
		edge[3] = rightBottom;
	}
	/// <summary>
	/// checkTileNumber �Ƌ�`���_4�_���ׂē�����
	/// </summary>
	/// <param name="checkTileNumber">4�_�`�F�b�N�������^�C���ԍ�</param>
	/// <returns>���ׂ� checkTileNumber �Ȃ� true</returns>
	bool Edge4sameCheck(int checkTileNumber)
	{
		return Edge4sameCheck() == checkTileNumber;
		//			&& leftTop == rightTop && rightTop == rightBottom && leftBottom == rightBottom;
	}
	/// <summary>
	/// ��`4�_�̃^�C���ԍ������ׂē������ǂ���
	/// </summary>
	/// <returns>�����Ȃ炻�̃^�C���ԍ��A�Ⴄ�Ȃ�A-1</returns>
	int Edge4sameCheck()
	{
		return
			leftTop == rightTop && rightTop == rightBottom && leftBottom == rightBottom ?
			leftTop : -1;
	}
	// ��`4�_�̃^�C���ԍ������ׂĎw��l�ƈႤ���H
	// �����F �w��l
	// �߂�l�F�w��l�Ƃ��ׂĈႤ�Ȃ� true
	bool Edge4DifferentNumber(int tileNumber)
	{
		return
			leftTop != tileNumber && rightTop != tileNumber
			&& leftBottom != tileNumber && rightBottom != tileNumber;
	}
};



/// <summary>
/// TMXmap: ���̃N���X�ŁATMX�t�@�C�����w�肵�ăC���X�^���X��ݒ肷��
///
/// �g������F	TMXmap tiled(U"mylevel1csv.tmx");
/// App�t�H���_���i�������͎w��t�H���_���j��TMX��TSX�Ɖ摜�̃t�@�C����u������
/// String �₻��n�͏d���̂Ŏd���Ȃ��A�ŏ����ɂƂǂ߂邽�ߍ������̂��ߕʕϐ��ɑ������
/// </summary>
class TMXmap
{
public:
	Map map;
	Array<Tileset> tileset;
	Array<Tileset*> tsxTileset;	// tsx�̃t�@�C�����X�g�i�[   >>>>>
	Array<Layer> layer;	// Array �S�^�C����񂪃��C���[���Ƃɓ����Ă���
	Array<ObjectGroup> objectGroup;	// ����Object_��Array �@�@objectGroup���z�񉻂��K�v�H��
	ObjectGroup& ObjGroup(int i = 0)
	{
		_ASSERT_EXPR(i < objectGroup.size() , L"objectGroup[i] ���͈͓��ł͂Ȃ��ł��I");
		return objectGroup[i];
	}
	// �I�u�W�F�N�g�O���[�v���̃I�u�W�F�N�g�̎擾
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
	// �^�C���̉摜�t�@�C���� �@�ύX���폜�̉\������ ��
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
		
		layer.clear();	// Array �S�^�C����񂪃��C���[���Ƃɓ����Ă���
		objectGroup.clear();	// ����Object_��Array
	}
	~TMXmap()
	{
		//		TextureAsset(TextureFilename()).release();	// �o�^�摜�f�[�^���������J��
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
		_ASSERT_EXPR(FileSystem::IsFile(tmxFile), L"TMX�t�@�C����������܂���B");
		TMXfile::SetTMXFolder(tmxFile); // �t�H���_�K�w������΃t�H���_���̂ݎ擾

		XMLReader tmx((FilePathView)tmxFile);
		map.Set(tmx);//<map ~>
		XMLElement parentElement = tmx.firstChild();
		XMLElement readElement = tmx.firstChild();

		while (readElement.name() == U"tileset") //<tileset�`>�����ׂ�
		{
			tileset.emplace_back(readElement);

			readElement = readElement.nextSibling();
		}

		for (int i = 0; readElement.name() == U"layer"; i++)
		{
			layer.emplace_back(readElement); // ���C���[�͕�������
			assert(layer[i].data.encoding == U"csv" && "�f�[�^��csv�`���݂̂ł��B�ݒ�ύX���ĕۑ��������Ă�������");
			readElement = readElement.nextSibling();
		}
		//<objectGroup>�z��
		while (readElement.name() == U"objectgroup")
		{
			objectGroup.emplace_back(readElement);
			readElement = readElement.nextSibling();
			auto test = readElement.name();
		}

		auto testElm1 = readElement.name();// debug
		auto testELm2 = readElement.nextSibling().name();// debug ������U""

		tmx.close();

		texAsset = TextureFilename();
		for (auto& a : tileset)
		{
			String source = TMXfile::SetPNGName(a.image.source);
			_ASSERT_EXPR(FileSystem::IsFile(FilePath(source)), L"�^�C���̉摜���ǂݍ��߂܂���B�t�@�C�����~�X�̉\��������܂��B");
			//source = U"Fooooooo"; String mes; const std::wstring mesL= L"�n���[���[���h";
			//_ASSERT_EXPR(FileSystem::IsFile(FilePath(source)), // ���s�AL"�`"�͕ϐ��ōs���ƕ�����������
			//	[&](String source) {
			//		mes = source + U"��������܂���B";	//����
			//		return &mesL;
			//	}(source));
			TextureAsset::Register(source, source);
			a.image.tileTexture = TextureAsset(source);
		}

		scrollPosition = { 0,0 };
		screenArraySize = ToMapArrayPos(Window::ClientSize());
	}
	// �z�񂪑�����₱�����̂ŁA�����[0]���K��Ƃ���
	// �^�C���̃v���p�e�B�������̔z��̎擾
	// �^�C���ɕ����̏��𖄂ߍ��܂Ȃ�����͂���͎g��Ȃ��B
	// �����ꍇ nullptr ��Ԃ�
	Array<Property> GetPropertyArray(int tileID, int tilesetNumber = 0)
	{
		if (tilesetNumber >= (int)tileset.size() || tilesetNumber < 0)
			return Array<Property>();

		return
			(tileset[tilesetNumber].tile.count(tileID) == 0) ?
			Array<Property>() :
			tileset[tilesetNumber].tile[tileID].properties;
	}
	// �z�񂪑�����₱�����̂ŁA�����[0]���K��Ƃ���@��
	// �^�C���̃v���p�e�B���̎擾
	Property GetProperty(int tileID, int tilesetNumber = 0)
	{
		if (tileID >= (int)tileset.size() || tileID < 0)
			return Property();
		ErrorWindow(tileset.size() <= tilesetNumber || tilesetNumber < 0, U"tilesetNumber���z��O�ł��B");
		auto& tmap = tileset[tilesetNumber].tile;
		if (tmap.count(tileID) == 0)return Property();
		return tileset[tilesetNumber].tile[tileID].properties[0];
	}

	// �^�C���ԍ��@�\�����W
	void TileDraw(int tileNumber, int x, int y)
	{
		if (tileNumber == 0 || tileNumber == -1)return;
		//�����Ł@�^�C���摜�ԍ��A�`��p�ԍ��A���v�Z
		int tileLayer;
		for (tileLayer = (int)tileset.size() - 1; tileLayer > 0; --tileLayer)
		{
			if (tileNumber >= tileset[tileLayer].firstgid)
				break;
		}
		assert(tileLayer >= 0 && " �^�C���ԍ����ُ�ł� ");

		//texture
		//(GetTileRect(tileNumber)).draw(x, y);	// �� ���͔̂z��O�G���[�ɂȂ�

		auto& tile = tileset[tileLayer];
		tile.image.tileTexture
		(GetTileRect(tile.firstgid, tileNumber, tileLayer)).draw(x, y);	// �� ���͔̂z��O�G���[�ɂȂ�

		// ���@�^�C���摜�z�񐔂𒲂ׂċt���Ń`�F�b�N
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
	// �^�C���ԍ��̉摜�̎��o����`���̎擾
	Rect GetTileRect(int firstGid, int tileNumber, int layerId = 0)
	{
		int tn = tileNumber - firstGid;
		return { tn % tileset[layerId].columns * TileWidth(), tn / tileset[layerId].columns * TileHeight(), TileWidth(), TileHeight() };
	}
	// layerId�����݂��邩?
	bool IsLayerID(int layerId)
	{
		return (size_t)layerId < layer.size() && layerId >= 0;
	}
	// layerId�����݂��邩?
	int TryLayerID(int layerId)
	{
		int layerSize = (int)layer.size();
		if (!(layerId < layerSize) || layerId < 0)
		{
			//_ASSERT_EXPR(isClear == true, _T("�N���A�����𖞂����Ă��Ȃ����ɂ��̊֐����Ă΂Ȃ��ŉ������I"));
			char str[40]; sprintf_s(str, "layer�͈̔͂𒴂��Ă��܂��Blayer�T�C�Y�� %d �ł�", layerSize);
			assert(str && 0);
		}
		return layerId;
	}
	// �}�b�v�͈̔͊O���H
	bool IsOutOfMap(Point arrayPos, int layerId = 0)
	{
		return (arrayPos.x < 0 || arrayPos.x > layer[layerId].width - 1 || arrayPos.y < 0 || arrayPos.y > layer[layerId].height - 1);
	}
	// �^�C���ԍ��擾����B�����F�}�b�v�z����W�@�߂�l�F�^�C���ԍ��A�������͈͊O�̏ꍇ�́|�P
	int GetTileNumber(Point arrayPos, int layerId = 0)
	{
		if (IsOutOfMap(arrayPos))
			return -1;
		return layer[layerId].tileMap[arrayPos.y][arrayPos.x];
	}
	// ���[���h���W�̎擾 �����F�s�N�Z�����W�@�@�߂�l�F���[���h���W
	Point LocalToWorldPosition(int px, int py)
	{
		return scrollPosition + Point(px, py);
	}
	// ���[���h���W�̎擾 �����F�W�����W�@�@�߂�l�F���[���h���W
	Point LocalToWorldPosition(Point pixelPos)
	{
		return (scrollPosition + pixelPos);
	}
	// ���[�J�����W�i�W�����W�j�̎擾 �����F�s�N�Z�����W�@�@�߂�l�F���[���h���W
	Point WorldToLocalPosition(Point pixelPos)
	{
		return pixelPos - scrollPosition;
	}
	// ���[�J�����W�i�W�����W�j�̎擾 �����F�s�N�Z�����W�@�@�߂�l�F���[���h���W
	Point WorldToLocalPosition(Vec2 pixelPos)
	{
		return pixelPos.asPoint() - scrollPosition;
	}
	// �~���X�N���[�����W�ϊ� �����F���[���h���W�@�߂�l�F�}�b�v�̔z����W
	Point RollArrayPosition(Point pos)
	{
		return {
			UMod(pos.x, MapWidth() * TileWidth()) / TileWidth(),
			UMod(pos.y, MapHeight() * TileHeight()) / TileHeight()
		};
	}
	// �~���X�N���[�����W�ϊ� �����F���[���h���W�@�߂�l�F�}�b�v�̔z����W
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

	// �����F���� �E�� ���� �E�� �̃^�C���ԍ��̎擾
	// �����F���ׂ��`���  �}�b�v���C���[��ID
	// �߂�l�F�S�̒��_�̓������Ă���^�C�����
	// �@layerId�@���͈͊O���Ɨ����܂��B
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
	// �����F��`�̒��_�̃^�C���ԍ��̎擾���Ă��ׂē����l�Ȃ炻�̐��l��Ԃ�
	// �����F���ׂ��`���  �}�b�v���C���[��ID
	// �߂�l�F�S�̒��_�̃^�C����񂪑S�������Ȃ�A���̒l�B
	//�@�@�@�@ �����łȂ��Ȃ�A-1�B
	// �@layerId�@���͈͊O���Ɨ����܂��B
	int GetSame4numberorNot(Rect rect, int layerID = 0)
	{
		return RectEdgeTileNumber(
			GetPixelPosTileNumber(rect.pos, layerID),
			GetPixelPosTileNumber(rect.right().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().end.asPoint(), layerID)
		).Edge4sameCheck();
	}
	// �����F��`�̒��_�̃^�C���ԍ��̎擾���ă^�C���ԍ��Ƃ��ׂĈႤ��
	// �����F���ׂ��`���  �^�C���ԍ��@�}�b�v���C���[��ID
	// �߂�l�F�S�̒��_�̃^�C����񂪑S���Ⴄ�Ȃ� true
	//
	// �@layerId�@���͈͊O���Ɨ����܂��B
	bool GetDifferent4Number(Rect rect, int tileNumber, int layerID = 0)
	{
		return RectEdgeTileNumber(
			GetPixelPosTileNumber(rect.pos, layerID),
			GetPixelPosTileNumber(rect.right().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().begin.asPoint(), layerID),
			GetPixelPosTileNumber(rect.bottom().end.asPoint(), layerID)
		).Edge4DifferentNumber(tileNumber);
	}
	// ���� �E�� ���� �E�� �̃^�C���ԍ��̎擾
	// �����F�X�v���C�g  �}�b�v���C���[��ID  �@�@�߂�l�F�S�̒��_�̓������Ă���^�C�����
	// �@layerId�@���͈͊O���Ɨ����܂��B
	//Array<int> GetPixel4Position(GameSprite&& spr, int layerID = 0)
	RectEdgeTileNumber GetPixel4Position(GameSprite&& spr, int layerID = 0)
	{
		Rect rect = spr.GetAnimation()->hit;
		rect.x += spr.iX(); rect.y += spr.iY();
		return GetRect4Position(rect, layerID);
	}
	// �P�t���[����́A���� �E�� ���� �E�� �̃^�C���ԍ��̎擾
	// �����F�X�v���C�g  �}�b�v���C���[��ID  �@�@�߂�l�F�S�̒��_�̓������Ă���^�C�����
	// �@layerId�@���͈͊O���Ɨ����܂��B
	//Array<int> GetPixel4PositionAddVelocity(GameSprite& spr, int layerID = 0)
	RectEdgeTileNumber GetPixel4PositionAddVelocity(GameSprite& spr, int layerID = 0)
	{
		Rect rect = spr.GetAnimation()->hit;
		rect.x += (int)(spr.iX() + spr.velocity.x); rect.y += (int)(spr.iY() + spr.velocity.y);
		return GetRect4Position(rect, layerID);
	}

	// �^�C���ԍ��̎擾 �����Fx,y �s�N�Z�����W  �߂�l�F�^�C���ԍ�
	int GetPixelPosTileNumber(int px, int py)
	{
		return GetPixelPosTileNumber(Point(px, py));
	}
	// �^�C���ԍ��̎擾 �����Fpos �s�N�Z�����W  �߂�l�F�^�C���ԍ�
	int GetPixelPosTileNumber(Vec2 pos, int layerID = 0)
	{
		return GetPixelPosTileNumber(pos.asPoint(), layerID);
	}
	// �^�C���ԍ��̎擾 ���� �s�N�Z�����W�@�@�@�߂�l�F�s�N�Z�����W
	int GetPixelPosTileNumber(Point pixelPos, int layerID)
	{
		return GetTileNumber(ToMapArrayPos(pixelPos), layerID);
	}
	// �}�b�v�z����W�̃s�N�Z���ł̋�`���̌v�Z�@�����F�}�b�v�z����W�@�߂�l�F�s�N�Z���P�ʂ̋�`���
	Rect GetRect(Point arrayPos)
	{
		return Rect(ToPixelPos(ToMapArrayPos(arrayPos)), TileWidth(), TileHeight());
	}
	// �}�b�v�z����W�̃s�N�Z���ł̕\�����W�ł̋�`���̌v�Z�@�����F�}�b�v�z����W�@�߂�l�F�s�N�Z���P�ʂ̋�`���
	Rect GetLocalRect(Point arrayPos)
	{
		Point local(ToPixelPos(ToMapArrayPos(arrayPos) - scrollPosition));
		return Rect(local, TileWidth(), TileHeight());
	}

	// �X�N���[���̍���̔����Ȃ��ꕪ�̌v�Z
	Point Shift(Vec2 scroll)
	{
		Point _shift(-UMod((int)scroll.x, TileWidth()), -UMod((int)scroll.y, TileHeight()));
		return _shift;
	}

	// �}�b�v�z����W����s�N�Z�����W�ɂ���@�����F�}�b�v�z����W�@�@�߂�l�F�s�N�Z�����W
	Point ToPixelPos(Point mapPos)
	{
		return Point(mapPos.x * TileWidth(), mapPos.y * TileHeight());
	}
	Point ToMapArrayPos(Point pixelPos)
	{
		//auto widthSize=TileWidth()*MapWidth();
		//auto heightSize=TileHeight()*MapHeight();

		Point arrayPos(pixelPos.x / TileWidth(), pixelPos.y / TileHeight());
		// ���̍��W�ɂȂ�ƌv�Z��1�}�X����Ă��܂��̂ŕ␳����
		arrayPos.x -= pixelPos.x < 0 ? 1 : 0;
		arrayPos.y -= pixelPos.y < 0 ? 1 : 0;

		return arrayPos;
	}
	Rect ToTileRect(Point pixelPos)
	{
		return Rect(ToPixelPos((ToMapArrayPos(pixelPos))), TileWidth(), TileHeight());
	}
	// ���[���h���W����`����W���v�Z�A�����F���[���h���W�@�@�߂�l�F�`����W
	Rect ToLocalTileRect(Point pixelPos)
	{
		Rect rect = ToTileRect(pixelPos);
		rect.pos = WorldToLocalPosition(rect.pos);
		return rect;
	}
	// �X�N���[���X�s�[�h�ŃX�N���[�����W���v�Z
	Point ScrollSpeedCalc(Vec2 scrollSpeed)
	{
		return Point((int)(scrollPosition.x * scrollSpeed.x), (int)(scrollPosition.y * scrollSpeed.y));
	}

	// �~���X�N���[���̃}�b�v�`��
	void MapRollDraw(int layerIndex,Vec2 scrollSpeed = Vec2(1,1))
	{
		// �~���X�N���[���Ȃ̂ŁA�������}�b�v�͈͓̔��Ɏ��߂�v�Z������
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

	// �X�N���[���̃}�b�v�`��
	void MapDraw(int layerIndex = 0, Vec2 scrollSpeed = Vec2(1,1))
	{
		
		// �~���X�N���[���͊m���ɓ��삷��̂ŁA����̍�ł�������ɂ���
		Point scroll = ScrollSpeedCalc(scrollSpeed);
		Point leftTop = RollArrayPosition(scroll);
		if (scroll.x < 0)leftTop.x -= MapWidth();
		if (scroll.y < 0)leftTop.y -= MapHeight();

		Point drawHomePos = Shift(scroll);
		Point drawPos = drawHomePos;

		assert((int)layer.size() > layerIndex&& layerIndex >= 0 && "MapDraw(int layerIndex) ��layer���͈͂𒴂��Ă��܂��B");

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
	// Object���C���[�̕\���A�f�o�b�O�\���p�A�m�F�p objectGroup�������ꍇ�A�\������Ȃ�
	void ObjectDraw(int i = 0)
	{
		if (!objectGroup.size()) return;
		assert(i < objectGroup.size() && "objectGroup ���͈͊O");
		for (auto&& ob : objectGroup[i].object)
		{
			Point drawPos = WorldToLocalPosition(Point((int)ob.x, (int)ob.y));
			TileDraw(ob.gid, drawPos.x, drawPos.y);
		}
	}
	// ObjectGroup��name���猟������object�z����擾, �����ƃG���[�ŗ����܂��B
	// ���������Ȃ��ꍇ�A�z��O�Ԗڂ�߂��܂��B
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
		_ASSERT_EXPR(false, L"ObjectArray(String objectGroupName)�Ō�����܂���B���~���Ă�������");
		return objectGroup[objectGroup.size()].object;	// �킴�Ɣ͈͊O�G���[���o���ďI��
	}
	// ObjectGroup��name����object�z��ԍ����擾, ������-1��Ԃ��܂��B
	// �߂�l�Fobject�z��ԍ��@ �����ꍇ�� -1
	int FindObjectArray(String objectGroupName)
	{
		for (int i = 0; i < objectGroup.size(); i++)
		{
			auto& objGrp = objectGroup[i];
			if (objGrp.name == objectGroupName)
				return i;
		}
		return -1;	// ������Ȃ�
	}

	// ���ׂĂ�Object���C���[�̕\���A�f�o�b�O�\���p�A�m�F�p
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

	// ���W���`��ʓ����`�F�b�N
	// �����F���W�@�@�߂�l
	bool InScreen(Vec2 pos)
	{
		return Rect(scrollPosition.x, scrollPosition.y, Window::ClientSize()).intersects(pos);
	}
	// �X�v���C�g�̕`��͈͂��`��ʓ����`�F�b�N
	// �����F���W�@�@�߂�l
	bool InScreen(GameSprite spr)
	{
		Rect drawRect(spr.position.asPoint(), spr.GetAnimation()->draw.w, spr.GetAnimation()->draw.h);
		return Rect(scrollPosition.x, scrollPosition.y, Window::ClientSize()).intersects(drawRect);
	}

private:
	String mapStr;
	XMLElement dataElement;
	//Texture texture;	///�������H�H�H // �z�񉻕K�v ��
	//Array<Texture> tileTexture; // �z�񉻕K�v �� ������
	//void Register()
	//{
	//	TextureAsset::Register(TextureFilename(), TextureFilename());
	//}
	// check ��true �Ȃ�@�G���[���b�Z�[�W�E�B���h�E���o���ďI��
	void ErrorWindow(bool check, String message)
	{
		if (check)
			ErrorMessage(message);
	}
	void tilesetNumber���z��O�H(bool check)
	{
		ErrorWindow(check, U"tilesetNumber���z��O�ł��B");
	}
};