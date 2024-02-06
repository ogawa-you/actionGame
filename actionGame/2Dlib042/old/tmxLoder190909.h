#pragma once
#include <Siv3D.hpp>

// 2019 9 9

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
class Object_;
class Object_
{
public:
	Object_()
	{
		Init(0, 0, 0, 0, 0, 0, 0);
	}
	Object_(int _id, String _name, int _gid, int _x, int _y, int _width, int _height)
	{
		Init(_id, _name, _gid, _x, _y, _width, _height);
	}
	void Init(int _id, String _name, int _gid, int _x, int _y, int _width, int _height)
	{
		id = _id;
		name = _name;
		gid = _gid;
		x = _x;
		y = _y;
		width = _width;
		height = _height;
	}
	int id;
	String name;
	int gid, x, y, width, height;
};
class ObjectGroup
{
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
	void ObjectArraySet(XMLElement XMLelement) //void ObjectArraySet(XMLElement XMLelement)
	{
		while (XMLelement.name() == U"object")
		{
			object.emplace_back(
				ParseInt<int>(XMLelement.attribute(U"id").value()),
				XMLelement.attribute(U"name").value(),
				ParseInt<int>(XMLelement.attribute(U"gid").value()),
				ParseInt<int>(XMLelement.attribute(U"x").value()),
				ParseInt<int>(XMLelement.attribute(U"y").value()),
				ParseInt<int>(XMLelement.attribute(U"width").value()),
				ParseInt<int>(XMLelement.attribute(U"height").value())
			);
			XMLelement = XMLelement.nextSibling();
		}
	}
	Object_& operator[](int index)
	{
		return object[index];
	}
};

class Image_
{
public:
	String source;
	int width, height;
	Image_()
	{
		Set(U"", 0, 0);
	}
	void Set(String _source, int _width, int _height)
	{
		source = _source, width = _width, height = _height;
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
		width = ParseInt<int>(tmx.attribute(U"width").value());
		height = ParseInt<int>(tmx.attribute(U"height").value());
		tilewidth = ParseInt<int>(tmx.attribute(U"tilewidth").value());
		tileheight = ParseInt<int>(tmx.attribute(U"tileheight").value());
		infinite = ParseInt<int>(tmx.attribute(U"infinite").value());
		nextlayerid = ParseInt<int>(tmx.attribute(U"nextlayerid").value());
		nextobjectid = ParseInt<int>(tmx.attribute(U"nextobjectid").value());
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
		return ParseInt<int>(value);
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
class Tileset
{
public:
	String version, tiledversion;
	int firstgid;
	String source // ��TSX�t�@�C���œǂݍ��ޏꍇ�t�@�C����������
		, name;
	int tilewidth, tileheight, tilecount, columns;
	Image_ image;
	//Tile tile; //bug err
	std::map<int, Tile> tileProperty;

	String& tmxMapfile;
	Tileset() :tmxMapfile(source) {
		firstgid = 0;
		source = U"";
		tileProperty.clear();
	}
	Tileset(XMLElement& tmxElement) :tmxMapfile(source)
	{
		SetElement(tmxElement);
	}
	// TSX�t�@�C�������݂��邩�@���遁true
	bool IsSource()
	{
		return source != U"";
	}
	// �����\��
	void Set(XMLReader& tmx)
	{
		XMLElement tileset = tmx.firstChild();//<tileset �`>
		SetElement(tileset);
	}
	void SetElement(XMLElement& tmxElement)
	{
		auto& tileset = tmxElement;
		this->firstgid = ParseInt<int>(tileset.attribute(U"firstgid").value());
		if (tileset.attribute(U"source").has_value())
		{
			this->source = tileset.attribute(U"source").value();//TSX�t�@�C��������
			// ��Ńt�@�C��source�̃t�@�C�������[�h����tileset�̃f�[�^��ǉ�
			// bug ��₱�����̂ł����Ń��[�h���Ă��܂��@@@
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
			this->tilewidth = ParseInt<int>(tileset.attribute(U"tilewidth").value());
			this->tileheight = ParseInt<int>(tileset.attribute(U"tileheight").value());
			this->tilecount = ParseInt<int>(tileset.attribute(U"tilecount").value());
			this->columns = ParseInt<int>(tileset.attribute(U"columns").value());
			//this-> = tileset.attribute(U"").value();
			XMLElement imageChld = tileset.firstChild();	//XMLElement& imageChld = tileset.firstChild();
			this->image.source = imageChld.attribute(U"source").value();
			this->image.width = ParseInt<int>(imageChld.attribute(U"width").value());
			this->image.height = ParseInt<int>(imageChld.attribute(U"height").value());
		}
		// <tileset<image> �܂Ŋ���>
		// <tile xxx >�`</tile>��ݒ�----------------------------------------
		XMLElement tileElemnt = tileset.firstChild().nextSibling();
		while (tileElemnt.name() == U"tile")
		{
			auto debugStr = tileElemnt.name();
			// 1����Ă���̂ŕ␳
			//tile.id = 1+ ParseInt<int>(tileElemnt.attribute(U"id").value()); 
			int tileID = 1 + ParseInt<int>(tileElemnt.attribute(U"id").value());

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
		tilewidth = ParseInt<int>(tmx.attribute(U"tilewidth").value());
		tileheight = ParseInt<int>(tmx.attribute(U"tileheight").value());
		tilecount = ParseInt<int>(tmx.attribute(U"tilecount").value());
		columns = ParseInt<int>(tmx.attribute(U"columns").value());
		// image
		image.width = ParseInt<int>(tmx.firstChild().attribute(U"width").value());
		image.height = ParseInt<int>(tmx.firstChild().attribute(U"height").value());
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
		id = ParseInt<int>(tmxElement.attribute(U"id").value());
		name = tmxElement.attribute(U"name").value();
		width = ParseInt<int>(tmxElement.attribute(U"width").value());
		height = ParseInt<int>(tmxElement.attribute(U"height").value());
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
				tileMap[iy][ix] = ParseInt<int>(dataNumber);
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

// TMXmap: ���̃N���X�ŁATMX�t�@�C�����w�肵�ăC���X�^���X��ݒ肷��
//
// �g������F	TMXmap tiled(U"mylevel1csv.tmx");
// App�t�H���_���i�������͎w��t�H���_���j��TMX��TSX�Ɖ摜�̃t�@�C����u������
class TMXmap
{

public:
	Map map;
	Array<Tileset> tileset;
	Array<Layer> layer;	// Array �S�^�C����񂪃��C���[���Ƃɓ����Ă���
	ObjectGroup objectGroup;	// ����Object_��Array �@�@objectGroup���z�񉻂��K�v�H
	// �I�u�W�F�N�g�̔z��
	Array<Object_>& ObjectArray;
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
	// �^�C���̉摜�t�@�C����
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
	Texture& AllTileTexture()
	{
		return texture;
	}
	void Clear()
	{
		scrollPosition = { 0,0 };
		screenArraySize = { 0,0 };
		map.Clear();
		tileset.clear();
		layer.clear();	// Array �S�^�C����񂪃��C���[���Ƃɓ����Ă���
		objectGroup.Clear();	// ����Object_��Array
	}
	~TMXmap()
	{
		TextureAsset(TextureFilename()).release();	// �o�^�摜�f�[�^���������J��
	}

	TMXmap() :ObjectArray(objectGroup.object)
	{
		Clear();
	}
	TMXmap(String tmxFile) :ObjectArray(objectGroup.object)
	{
		LoadTMXfile(tmxFile);
	}
	void LoadTMXfile(String tmxFile)
	{
		XMLReader tmx(tmxFile);
		//map.Set(tmx.attribute(U"version").value());
		map.Set(tmx);//<map ~>
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
		//<objectGroup>
		while (readElement.name() == U"objectgroup")
		{
			objectGroup.id = ParseInt<int>(readElement.attribute(U"id").value());
			objectGroup.name = readElement.attribute(U"name").value();

			objectGroup.ObjectArraySet(readElement.firstChild());
			readElement = readElement.nextSibling();
			break; // �z�񉻗\��@����ЂƂ�
		}

		auto testElm = tmx.firstChild().nextSibling();// debug
		auto testStr = testElm.nextSibling().name();// debug ������U"" 

		tmx.close();

		texAsset = TextureFilename();
		TextureAsset::Register(TextureFilename(), TextureFilename());
		//int ibak = 0;
		//for (int i = 0; TextureAsset(TextureFilename()).isEmpty(); i++)
		//{
		//	TextureAsset(TextureFilename()).getDesc();
		//	ibak = i;
		//}

		texture = TextureAsset(TextureFilename());// ������

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
	// �z�񂪑�����₱�����̂ŁA�����[0]���K��Ƃ���
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
		texture
		(GetTileRect(tileNumber)).draw(x, y);
	}
	// �^�C���ԍ��̉摜�̎��o����`���̎擾
	Rect GetTileRect(int tileNumber, int layerId = 0)
	{
		int tn = tileNumber - 1;
		return { tn % tileset[layerId].columns * TileWidth(), tn / tileset[layerId].columns * TileHeight(), TileWidth(), TileHeight() };
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
	// ���� �E�� ���� �E�� �̃^�C���ԍ��̎擾
	// �����F���ׂ��`���  �@�߂�l�F�S�̒��_�̓������Ă���^�C�����
	// �@layerId�@���͈͊O���Ɨ����܂��B
	Array<int> GetPixel4Position(Rect rect, int layerID = 0)
	{
		Array<int> hit(4);
		hit[0] = GetPixelPosTileNumber(rect.pos, layerID);
		hit[1] = GetPixelPosTileNumber(rect.right().begin.asPoint(), layerID);
		hit[2] = GetPixelPosTileNumber(rect.bottom().begin.asPoint(), layerID);
		hit[3] = GetPixelPosTileNumber(rect.bottom().end.asPoint(), layerID);
		return hit;
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
	void MapDraw(int layerIndex)
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
	void ObjectDraw()
	{
		for (auto& ob : objectGroup.object)
		{
			Point drawPos = WorldToLocalPosition(Point(ob.x, ob.y));
			TileDraw(ob.gid, drawPos.x, drawPos.y);
		}
	}



private:
	String mapStr;
	XMLElement dataElement;
	Texture texture;	///�������H�H�H

	void Register()
	{
		TextureAsset::Register(TextureFilename(), TextureFilename());
	}
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