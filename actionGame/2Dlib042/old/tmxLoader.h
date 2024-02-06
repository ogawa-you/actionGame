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
// ��΂ɐ��̐��ł��܂���o���@�����F�����鐔�A���鐔�@�@�߂�l�F�]��i���̐��j
inline int UnsignedMod(int a,int b)
{
	assert( b>=0 && "���鐔�͐��̐��̂�" );
	int mod = a % b;
	if (mod < 0)mod += b;
	return mod;
}
// ��΂ɐ��̐��ł��܂���o���@�����F�����鐔�A���鐔�@�@�߂�l�F�]��i���̐��j
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
	// �^�C���̉摜�t�@�C����
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
		return TextureAsset(TextureFilename());//	�G���[(�A�N�e�B�u)	E0461	�� const �ւ̎Q�Ƃ̏����l�͍��Ӓl�ł���K�v������܂�	tmxAndSprite040	C : \Users\voldo\source\repos\tmxAndSprite040\tmxAndSprite040\tmxLoader.h	164
	}*/
	TMXmap()
	{}
	TMXmap(String tmxFile)
	{
		LoadTMXfile(tmxFile);
	}
	void LoadTMXfile(String tmxFile)
	{
		assert(SIV3D_VERSION >= U"0.4.0" && "OpenSiv3D 0.4.0�ȏ���g���Ă�������"); 
		XMLReader tmx(tmxFile);
		//map.Set(tmx.attribute(U"version").value());
		map.Set(tmx);
		tileset.Set(tmx);
		layer.Set(tmx);
		dataElement = data.Set(tmx);
		assert(data.encoding == U"csv" && "�f�[�^��csv�`���݂̂ł��B�ݒ�ύX���ĕۑ��������Ă�������");
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
	// �^�C���ԍ��@�\�����W
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
	// �^�C���ԍ��@�\�����W
	void TileDraw(int tileNumber, int x, int y)
	{
		if (tileNumber == 0 || tileNumber == -1 )return;
		TextureAsset(TextureFilename())
			(GetTileRect(tileNumber)).draw(x, y);
	}
	// �^�C���ԍ��̉摜�̎��o����`���̎擾
	Rect GetTileRect(int tileNumber)
	{
		int tn = tileNumber-1;
		return { tn % tsx.columns * TileWidth(), tn / tsx.columns * TileHeight(), TileWidth(), TileHeight() };
	}
	// �}�b�v�͈̔͊O���H
	bool IsOutOfMap(Point arrayPos)
	{
		return (arrayPos.x < 0 || arrayPos.x > layer.width - 1 || arrayPos.y < 0 || arrayPos.y > layer.height -1);
	}
	// �^�C���ԍ��擾����B�����F�}�b�v�z����W�@�߂�l�F�^�C���ԍ��A�������͈͊O�̏ꍇ�́|�P
	int GetTileNumber(Point arrayPos)
	{
		if (IsOutOfMap(arrayPos))
			return -1;
		return tileMap[arrayPos.y][arrayPos.x];
	}
	// ���[���h���W�̎擾 �����F�s�N�Z�����W�@�@�߂�l�F���[���h���W
	Point LocalToWorldPosition(int px,int py)
	{
		return scrollPosition.asPoint()+Point(px,py);
	}
	// ���[���h���W�̎擾 �����F�W�����W�@�@�߂�l�F���[���h���W
	Point LocalToWorldPosition(Point pixelPos)
	{
		return (scrollPosition.asPoint() + pixelPos);
	}
	// ���[�J�����W�i�W�����W�j�̎擾 �����F�s�N�Z�����W�@�@�߂�l�F���[���h���W
	Point WorldToLocalPosition(Point pixelPos)
	{
		return pixelPos -scrollPosition.asPoint();
	}
	// �~���X�N���[�����W�ϊ� �����F���[���h���W�@�߂�l�F�}�b�v�̔z����W
	Point RollArrayPosition(Point pos)
	{
		return {
			UMod(pos.x, MapWidth() * TileWidth()) / TileWidth(),
			UMod(pos.y, MapHeight() * TileHeight()) / TileHeight()
		};
	}


	// �^�C���ԍ��̎擾 �����Fx,y �s�N�Z�����W  �߂�l�F�^�C���ԍ�
	int GetPixelTileNumber(int px,int py)
	{
		return GetPixelPosTileNumber({px,py});
	}
	// �^�C���ԍ��̎擾 ���� �s�N�Z�����W�@�@�@�߂�l�F�s�N�Z�����W
	int GetPixelPosTileNumber(Point pixelPos)
	{
		return GetTileNumber(ToMapArrayPos(pixelPos));
	}
	// �}�b�v�z����W�̃s�N�Z���ł̋�`���̌v�Z�@�����F�}�b�v�z����W�@�߂�l�F�s�N�Z���P�ʂ̋�`���
	Rect GetRect(Point arrayPos)
	{
		return Rect(ToPixelPos(ToMapArrayPos(arrayPos)), TileWidth(), TileHeight());
	}
	// �}�b�v�z����W�̃s�N�Z���ł̕\�����W�ł̋�`���̌v�Z�@�����F�}�b�v�z����W�@�߂�l�F�s�N�Z���P�ʂ̋�`���
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
	// �X�N���[���̍���̔����Ȃ��ꕪ�̌v�Z
	Point Shift()
	{
		Point _shift(-UMod(scrollPosition.asPoint().x , TileWidth()), -UMod(scrollPosition.asPoint().y , TileHeight()));
		return _shift;
	}

	// �}�b�v�z����W����s�N�Z�����W�ɂ���@�����F�}�b�v�z����W�@�@�߂�l�F�s�N�Z�����W
	Point ToPixelPos(Point mapPos)
	{
		return Point(mapPos.x * TileWidth(), mapPos.y * TileHeight());
	}
	//Point ToMapArrayPos(Point pixelPos)
	//{
	//	Point arrayPos(pixelPos.x / TileWidth(),pixelPos.y / TileHeight());
	//	// ���̍��W�ɂȂ�ƌv�Z��1�}�X����Ă��܂��̂ŕ␳���� //bug
	//	arrayPos.x -= pixelPos.x < 0 ? 1 : 0 ;
	//	arrayPos.y -= pixelPos.y < 0 ? 1 : 0 ;
	//	return arrayPos;
	//}
	Point ToMapArrayPos(Point pixelPos)
	{
		//auto widthSize=TileWidth()*MapWidth();
		//auto heightSize=TileHeight()*MapHeight();

		Point arrayPos(pixelPos.x / TileWidth(),pixelPos.y / TileHeight());
		// ���̍��W�ɂȂ�ƌv�Z��1�}�X����Ă��܂��̂ŕ␳����
		arrayPos.x -= pixelPos.x < 0 ? 1 : 0 ;
		arrayPos.y -= pixelPos.y < 0 ? 1 : 0 ;


		return arrayPos;
	}
	Rect ToTileRect(Point pixelPos)
	{
		return Rect(ToPixelPos((ToMapArrayPos(pixelPos))),TileWidth(),TileHeight());
	}
	// ���[���h���W����`����W���v�Z�A�����F���[���h���W�@�@�߂�l�F�`����W
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
	// �~���X�N���[���̃}�b�v�`��
	void MapRollDraw()
	{
		// �~���X�N���[���Ȃ̂ŁA�������}�b�v�͈͓̔��Ɏ��߂�v�Z������
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
	// �X�N���[���̃}�b�v�`��
	void MapDraw()
	{
		// �~���X�N���[���͊m���ɓ��삷��̂ŁA����̍�ł�������ɂ���
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
		// �C���[�W�@int tileMap[layer.height][layer.width];
		tileMap = Array<Array<int>>(layer.height, Array<int>(layer.width, 0));
		/*String*/ mapStr = dataElement.text();
		DataIntSet();
	}
	void DataIntSet()
	{
		String newMapstr = U"";// ��₱�����̂ŕςȕ���������
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
					assert(istr < mapStr.length() && "�}�b�v�̕����񂪕s�����Ă��܂�");
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