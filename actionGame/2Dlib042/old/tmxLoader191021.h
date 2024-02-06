#pragma once
#include <Siv3D.hpp>

// 2019 10 15 �I�u�W�F�N�g���C���[�Ő������]�@�������]�@��]�̏��ǉ��A�����^�C���ǉ��\ Parse<uint> ���g�p
// 2019 10 16 �I�u�W�F�N�g���C���[��z��
// 2019 10 17 �I�u�W�F�N�g���C���[�̌������\�b�h�ǉ��Aname�̂���z��ԍ���������FindObjectArray(name)
//  �@�@�@�@�@name�̂���z��������� FindObjectArray(name)
// 2019 10 21 �t�H���_����TMX�t�@�C���̃��[�h�\�ɂ����B�t�@�C�����[�h�G���[�����̒ǉ�


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
const unsigned JUST_GID_MASK = 0xFFFF; // ����͐���
// https://doc.mapeditor.org/en/stable/reference/tmx-map-format/#object

enum class ObjectValue
{
	null,point,ellipse, polygon,text,
};

// �R�C����A�C�e����G�Ȃǂ̃X�v���C�g���
class Object_
{
	/*
			object.emplace_back(
				Parse<int>(objectElement.attribute(U"id").value()),
				objectElement.attribute(U"name").value_or(U""),
				Parse<int>(objectElement.attribute(U"gid").value_or(U"-1")),
				Parse<double>(objectElement.attribute(U"x").value()),
				Parse<double>(objectElement.attribute(U"y").value())-height,
				Parse<int>(objectElement.attribute(U"width").value_or(U"0")),	//error�����ꍇ������
				height,
				Parse<double>(objectElement.attribute(U"rotation").value_or(U"0")),
				value
			);
	*/

public:
	Object_(XMLElement xmlElement)	//TODO //������
	{
		// �������AY���W������(TiledmapEditor �̃o�O�H�d�l�H)
		int height = Parse<int>(xmlElement.attribute(U"height").value_or(U"0"));	// error�����ꍇ������
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
		//	Parse<int>(xmlElement.attribute(U"width").value_or(U"0")),	//error�����ꍇ������
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
		//assert(_gid<1000);	// name�^�O�����ݒ肾�ƃG���[���������C���ς�
		x = _x;
		y = _y;
		width = _width;
		height = _height;
		rotation = _rotation;
		value = objectValue;	//�����
	}
	int id;
	String name;
	// -1 �Ȃ瑶�݂��Ȃ�
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
		XMLElement objectElement = readElement.firstChild(); // object�ɂ��� //;

		while (objectElement.name() == U"object")
		{
			//<point/><ellipse/> �Ȃǎq�v�f�������Ă��邩�H
		// �������AY���W������(TiledmapEditor �̃o�O�H�d�l�H)
		int height = Parse<int>(objectElement.attribute(U"height").value_or(U"0"));	// error�����ꍇ������
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

			//object.emplace_back(objectElement);	// �� TODO
			object.emplace_back(
				Parse<int>(objectElement.attribute(U"id").value()),
				objectElement.attribute(U"name").value_or(U""),
				Parse<int>(objectElement.attribute(U"gid").value_or(U"-1")),
				Parse<double>(objectElement.attribute(U"x").value()),
				Parse<double>(objectElement.attribute(U"y").value())-height, 
				Parse<int>(objectElement.attribute(U"width").value_or(U"0")),	//error�����ꍇ������
				height,
				Parse<double>(objectElement.attribute(U"rotation").value_or(U"0")),
				value
			);

			objectElement = objectElement.nextSibling();
		}
	}
	Object_& operator[](int index)
	{
		assert(index<object.size() && "object[index]���͈͊O�ł�");
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
	// �^�C���̃e�L�X�`���[���Z�b�g�@��
	void TileTextureSet()
	{
		TextureAsset::Register(source,source);
		for (size_t i = 0;  ! TextureAsset::IsReady(source) ;assert(++i<1000000000));
		// ���O�͑��v���H
		tileTexture = TextureAsset(source);
	}
	void TileTextureRelease()
	{
		TextureAsset(source).release();
	}

};
// XML�f�[�^�̐e��
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
//	Texture texture;	// ���܂��ǂ߂Ȃ�
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
	// TSX�t�@�C�������݂��邩�@���遁true
	bool IsSource()
	{
		return source != U"";
	}
	//// �^�C���摜�t�@�C����o�^
	//void SetTexture()
	//{
	//	_ASSERT_EXPR(TextureAsset::Register(image.source, image.source),L"�^�C���t�@�C�����[�h�G���[");	//
	//	while(TextureAsset(image.source).isEmpty());
	//	//texture = TextureAsset(image.source);	//�ǂ߂Ȃ��A���s
	//	//assert(false);
	//}


	// �����\�� �@�@�@�����\��̗��R�����O
	//void Set(XMLReader& tmx)
	//{
	//	XMLElement tileset = tmx.firstChild();//<tileset �`>
	//	SetElement(tileset);
	//}
	void SetElement(XMLElement& tmxElement)
	{
		auto& tileset = tmxElement;
		this->firstgid = Parse<int>(tileset.attribute(U"firstgid").value());
		if (tileset.attribute(U"source").has_value())
		{
			this->source = tileset.attribute(U"source").value();//TSX�t�@�C��������

			_ASSERT_EXPR(FileSystem::IsFile(FilePath(source)), L"TSX�t�@�C����������܂���B");
			XMLReader tsxfile(source);
			TSXFile_Set_Tileset_and_Image(tsxfile); // TSX�t�@�C������l���Z�b�g
			tsxfile.close();

		}
		else //TSX�t�@�C�����Ȃ�----������tileset�̃f�[�^���Z�b�g---------------------
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
		// <tileset<image> �܂Ŋ���>
		// <tile xxx >�`</tile>��ݒ�----------------------------------------
		XMLElement tileElemnt = tileset.firstChild().nextSibling();
		while (tileElemnt.name() == U"tile")
		{
			auto debugStr = tileElemnt.name();
			// 1����Ă���̂ŕ␳
			//tile.id = 1+ Parse<int>(tileElemnt.attribute(U"id").value()); 
			int tileID = 1 + Parse<int>(tileElemnt.attribute(U"id").value());

			tileProperty[tileID] = Tile(tileID);

			XMLElement propertiesArrayElm = tileElemnt.firstChild();// XMLElement& propertiesArrayElm = tileElemnt.firstChild()
			String propertiesArrayElmName = propertiesArrayElm.name(); //check
			XMLElement propertyElm = propertiesArrayElm.firstChild();// XMLElement& propertyElm = propertiesArrayElm.firstChild()
			String propertyElmName = propertyElm.name(); //check
			while (propertiesArrayElm.name() == U"properties" && propertyElm.name() == U"property") //<tileset�`>�����ׂ�
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

	// tilewidth tileheight tilecount columns width height source �̒l��XML����ݒ�
	// TSX�t�@�C��������ꍇ�̂݌Ă΂��B�����ɐݒ�l�̎�ނ��ς��̂�
	// ������ʂ��Őݒ�
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

// ��΂ɐ��̐��ł��܂���o���@�����F�����鐔�A���鐔�@�@�߂�l�F�]��i���̐��j
inline int UnsignedMod(int a, int b)
{
	assert(b >= 0 && "���鐔�͐��̐��̂�");
	int mod = a % b;
	if (mod < 0)mod += b;
	return mod;
}
// ��΂ɐ��̐��ł��܂���o���@�����F�����鐔�A���鐔�@�@�߂�l�F�]��i���̐��j
inline int UMod(int a, int b)
{
	return UnsignedMod(a, b);
}

// 
// 2019 09 17 ��`�̊p�̃^�C�����ۑ��p�\����
// const�ɂ��Ă��Ȃ��͉̂��H���Ă��g����悤�ɂ��邽��
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
	// �S�p��
	int edge[4];
	void EdgeSet()
	{
		edge[0] = leftTop;
		edge[1] = rightTop;
		edge[2] = leftBottom;
		edge[3] = rightBottom;
	}
};


// TMXmap: ���̃N���X�ŁATMX�t�@�C�����w�肵�ăC���X�^���X��ݒ肷��
//
// �g������F	TMXmap tiled(U"mylevel1csv.tmx");
// App�t�H���_���i�������͎w��t�H���_���j��TMX��TSX�Ɖ摜�̃t�@�C����u������
class TMXmap
{
private:
	String parentFolder;
	String rootFolder;
public:
	// TMX�t�@�C���̂���t�H���_�[���̃t�@�C�����p�X�ɂ���
	//String SetPath(String fileName)
	//{
	//	return parentFolder + fileName;
	//}
	Map map;
	Array<Tileset> tileset;
	Array<Layer> layer;	// Array �S�^�C����񂪃��C���[���Ƃɓ����Ă���
	Array<ObjectGroup> objectGroup;	// ����Object_��Array �@�@objectGroup���z�񉻂��K�v�H��
	ObjectGroup& ObjGroup(int i=0)
	{
		assert(i<objectGroup.size() && "objectGroup[i] ���͈͓��ł͂Ȃ��ł��I");
		return objectGroup[i];
	}
	// �I�u�W�F�N�g�O���[�v���̃I�u�W�F�N�g�̎擾
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
		rootFolder = FileSystem::CurrentDirectory();
		parentFolder = FileSystem::ParentPath(tmxFile);
		//auto Relativefolder = FileSystem::RelativePath(parentFolder);//�e�X�g
		//if (Relativefolder ==U"./")Relativefolder = U"";//�e�X�g
		_ASSERT_EXPR(FileSystem::IsFile(FilePath(tmxFile)), L"TMX�t�@�C����������܂���B");
		XMLReader tmx(tmxFile);
		FileSystem::ChangeCurrentDirectory(parentFolder);
		//map.Set(tmx.attribute(U"version").value());//�e�X�g
		map.Set(tmx);//<map ~>
		XMLElement parentElement = tmx.firstChild(); // tileset�ɂ��� //XMLElement& readElement = tmx.firstChild();
		XMLElement readElement = tmx.firstChild(); // tileset�ɂ��� //XMLElement& readElement = tmx.firstChild();
		
//		String nameTileset= readElement.name(); // check

		while (readElement.name() == U"tileset") //<tileset�`>�����ׂ�
		{
			tileset.emplace_back(readElement);//tileset.push_back(readElement);����ł����Ă��I

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
		for (auto& a: tileset)
		{
			String source = a.image.source;
			_ASSERT_EXPR(FileSystem::IsFile(FilePath(source)),L"�^�C���̉摜���ǂݍ��߂܂���B�t�@�C�����~�X�̉\��������܂��B" );
			//source = U"Fooooooo"; String mes; const std::wstring mesL= L"�n���[���[���h";
			//_ASSERT_EXPR(FileSystem::IsFile(FilePath(source)), // ���s�AL"�`"�͕ϐ��ōs���ƕ�����������
			//	[&](String source) {
			//		mes = source + U"��������܂���B";	//����
			//		return &mesL;
			//	}(source));
			TextureAsset::Register(source,source);
			a.image.tileTexture = TextureAsset(source);
		}
		FileSystem::ChangeCurrentDirectory(rootFolder);	// �t�H���_�p�X��߂�

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
			(tileset[tilesetNumber].tileProperty.count(tileID) == 0) ?
			Array<Property>() :
			tileset[tilesetNumber].tileProperty[tileID].properties;
	}
	// �z�񂪑�����₱�����̂ŁA�����[0]���K��Ƃ���@��
	// �^�C���̃v���p�e�B���̎擾
	Property GetProperty(int tileID, int tilesetNumber = 0)
	{
		if (tileID >= (int)tileset.size() || tileID < 0)
			return Property();
		ErrorWindow(tileset.size() <= tilesetNumber || tilesetNumber < 0, U"tilesetNumber���z��O�ł��B");
		auto& tmap = tileset[tilesetNumber].tileProperty;
		if (tmap.count(tileID) == 0)return Property();
		return tileset[tilesetNumber].tileProperty[tileID].properties[0];
	}

	// �^�C���ԍ��@�\�����W
	void TileDraw(int tileNumber, int x, int y)
	{
		if (tileNumber == 0 || tileNumber == -1)return;
		//�����Ł@�^�C���摜�ԍ��A�`��p�ԍ��A���v�Z
		int tileLayer;
		for (tileLayer = (int)tileset.size() - 1; tileLayer > 0; --tileLayer)
		{
			if (tileNumber > tileset[tileLayer].firstgid)
				break;
		}
		assert(tileLayer>=0 && " �^�C���ԍ����ُ�ł� ");

		//texture
		//(GetTileRect(tileNumber)).draw(x, y);	// �� ���͔̂z��O�G���[�ɂȂ�

		auto& tile = tileset[tileLayer];
		tile.image.tileTexture
		(GetTileRect(tile.firstgid,tileNumber,tileLayer)).draw(x, y);	// �� ���͔̂z��O�G���[�ɂȂ�

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
	Rect GetTileRect(int firstGid,int tileNumber, int layerId = 0)
	{
		int tn = tileNumber - firstGid;
		return { tn % tileset[layerId].columns * TileWidth(), tn / tileset[layerId].columns * TileHeight(), TileWidth(), TileHeight() };
	}
	// layerId�����݂��邩?
	bool IsLayerID(int layerId)
	{
		return (size_t)layerId < layer.size()  &&  layerId >=0 ;
	}
	// layerId�����݂��邩?
	int TryLayerID(int layerId)
	{
		int layerSize = (int)layer.size();
		if (! (layerId < layerSize) || layerId < 0)
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
	// �~���X�N���[�����W�ϊ� �����F���[���h���W�@�߂�l�F�}�b�v�̔z����W
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

	// ���� �E�� ���� �E�� �̃^�C���ԍ��̎擾
	// �����F���ׂ��`���  �}�b�v���C���[��ID  �@�߂�l�F�S�̒��_�̓������Ă���^�C�����
	// �@layerId�@���͈͊O���Ɨ����܂��B
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
	// ���� �E�� ���� �E�� �̃^�C���ԍ��̎擾
	// �����F�X�v���C�g  �}�b�v���C���[��ID  �@�@�߂�l�F�S�̒��_�̓������Ă���^�C�����
	// �@layerId�@���͈͊O���Ɨ����܂��B
	//Array<int> GetPixel4Position(GameSprite&& spr, int layerID = 0)
	RectEdgeTileNo GetPixel4Position(GameSprite&& spr, int layerID = 0)
	{
		Rect rect = spr.GetAnimation()->hit;
		rect.x += spr.iX(); rect.y += spr.iY();
		return GetRect4Position(rect, layerID);
	}
	// �P�t���[����́A���� �E�� ���� �E�� �̃^�C���ԍ��̎擾
	// �����F�X�v���C�g  �}�b�v���C���[��ID  �@�@�߂�l�F�S�̒��_�̓������Ă���^�C�����
	// �@layerId�@���͈͊O���Ɨ����܂��B
	//Array<int> GetPixel4PositionAddVelocity(GameSprite& spr, int layerID = 0)
	RectEdgeTileNo GetPixel4PositionAddVelocity(GameSprite& spr, int layerID = 0)
	{
		Rect rect = spr.GetAnimation()->hit;
		rect.x += (int)(spr.iX()+spr.velocity.x); rect.y += (int)(spr.iY()+spr.velocity.y);
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
	Point Shift()
	{
		Point _shift(-UMod(scrollPosition.x, TileWidth()), -UMod(scrollPosition.y, TileHeight()));
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
	// �~���X�N���[���̃}�b�v�`��
	void MapRollDraw(int layerIndex)
	{

		// �~���X�N���[���Ȃ̂ŁA�������}�b�v�͈͓̔��Ɏ��߂�v�Z������
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


	// �X�N���[���̃}�b�v�`��
	void MapDraw(int layerIndex=0)
	{
		// �~���X�N���[���͊m���ɓ��삷��̂ŁA����̍�ł�������ɂ���
		Point leftTop = RollArrayPosition(scrollPosition);
		if (scrollPosition.x < 0)leftTop.x -= MapWidth();
		if (scrollPosition.y < 0)leftTop.y -= MapHeight();

		Point drawHomePos = Shift();
		Point drawPos = drawHomePos;

		assert((int)layer.size() > layerIndex && layerIndex >= 0 && "MapDraw(int layerIndex) ��layer���͈͂𒴂��Ă��܂��B");

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
	// Object���C���[�̕\���A�f�o�b�O�\���p�A�m�F�p
	void ObjectDraw(int i)
	{
		assert(i < objectGroup.size());
		for (auto&& ob : objectGroup[i].object)
		{
			Point drawPos = WorldToLocalPosition(Point((int)ob.x, (int)ob.y));
			TileDraw(ob.gid, drawPos.x, drawPos.y);
		}
	}
	// ObjectGroup��name����object�z����擾, �����ƃG���[�ŗ����܂��B
	Array<Object_>& ObjectArray(String objectGroupName)
	{
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
		return Rect(scrollPosition.x, scrollPosition.y,Window::ClientSize()).intersects(pos);
	}
	// �X�v���C�g�̕`��͈͂��`��ʓ����`�F�b�N
	// �����F���W�@�@�߂�l
	bool InScreen(GameSprite spr)
	{
		Rect drawRect(spr.position.asPoint(), spr.GetAnimation()->draw.w, spr.GetAnimation()->draw.h);
		return Rect(scrollPosition.x, scrollPosition.y,Window::ClientSize()).intersects(drawRect);
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
