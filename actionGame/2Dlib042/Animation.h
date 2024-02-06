#pragma once
# include <Siv3D.hpp>

// 2020 8/1

enum class Kind
{
	Loop, Scale,Fade,AndTurn
};
/// <summary>
/// アニメーションと当たり判定のデータクラス
/// </summary>
class Animation
{
private:
	int count,widthCount;
	Texture texture;		// 高速化
public:
	Audio audio;		// 高速化
	Kind state;
	String textureAsset = U"" , audioAsset = U"";
	Rect draw,hit;
	int animSpeed;

	Animation();

	//Animation(String textureAsset, String audioAsset, Rect draw, int count, int widthCount, int animSpeed , Kind state);
	Animation(String _textureAsset, Rect draw = Rect(0, 0, 0, 0),
		int count = 0, int widthCount = 0, int animSpeed = 200, Kind state = Kind::Loop, double cutPercent = 30, String audioAsset = U"");
	// 横1列のみ初期化
	Animation(String _textureAsset, int count = 1, int animSpeed = 200, Kind state = Kind::Loop, String audioAsset = U"",
		double cutPercent = 30);
	// 横分割数、縦分割数での初期化  ぴったりでない場合、減らす数のパターン数decCountを入れる
	Animation(String _textureAsset, Size widthHeightCount, int decCount = 0,
		int animSpeed = 200, Kind state = Kind::Loop, String audioAsset = U"",
		double cutPercent = 30);
	// 横分割数、縦分割数での初期化  ぴったりでない場合、減らす数のパターン数decCountを入れる
	Animation(String _textureAsset, String audioAsset, Size widthHeightCount,
		int _animSpeed = 100, int decCount = 0, double cutPercent = 30,
		Kind _state = Kind::Fade);

	~Animation() {}

	void HitSet(double cutPercent = 30);

	int Count()
	{
		return count;
	}
	int WidthCount()
	{
		return widthCount;
	}

	void Init(String _textureAsset, String _audioAsset = U"", Rect _rect = Rect(0, 0, 0, 0), int _count = 1, int _wCount = 1, int _animSpeed = 200, Kind _state = Kind::Loop, double cutPercent = 30)
	{
		if (!TextureAsset::IsRegistered(_textureAsset))
		{
			TextureAsset::Register(_textureAsset, _textureAsset);
		}
		texture = TextureAsset(_textureAsset);	// 高速化

		if (!AudioAsset::IsRegistered(_audioAsset))
			AudioAsset::Register(_audioAsset, _audioAsset);
		audio = AudioAsset(_audioAsset);	// 高速化

		textureAsset = _textureAsset,audioAsset = _audioAsset,
			draw = _rect, count = _count, widthCount = _wCount, animSpeed = _animSpeed, state = _state;
		if (count < 3)
			state = Kind::Loop;
		HitSet(cutPercent);
	}
	// アニメーションしない1枚絵の初期化
	void InitNotAnime(String _textureAsset, Rect _rect = Rect(0, 0, 0, 0), double cutPercent = 30)
	{
		Init(_textureAsset, U"", _rect, 1, 1, 1000, Kind::Loop, cutPercent);
	}


	void Draw(Vec2 position = Vec2(0, 0), Rect spriterect = Rect(0, 0, 0, 0), bool mirrored = false,bool flipped=false);
	Rect HitRect(Vec2 sprPosition);
	void HitDraw(Vec2 sprPosition);
};
/// <summary>
/// RPGツクール用キャラ
/// 1キャラ分
/// </summary>
class RPGtkChar
{
	int TXwidth, TXheight;
public:
	String textureAsset;
	Animation Down, Left, Right, Up;
	Animation DownStop, LeftStop, RightStop, UpStop;
	RPGtkChar()
	{}
	RPGtkChar(String asset, int frameSpeed = 300, double cutPercent = 30)
	{
		textureAsset = asset;
		Init(asset, frameSpeed , cutPercent);
	}
	RPGtkChar(String asset, int TXwidth, int TXheight, int startX, int startY, int frameSpeed = 300, double cutPercent = 30)
	{
		textureAsset = asset;
		Init( asset,  TXwidth,  TXheight,  startX,  startY,  frameSpeed ,  cutPercent);
	}
	void Init(String _asset, int frameSpeed = 300, double cutPercent = 30)
	{
		textureAsset = _asset;

		if (!TextureAsset::IsRegistered(_asset))
			TextureAsset::Register(_asset, _asset);

		TXwidth = TextureAsset(_asset).width(),
			TXheight = TextureAsset(_asset).height();

		Init(_asset,TXwidth,TXheight,0,0,frameSpeed,cutPercent);
	}
	void Init(String _asset, int _1TXwidth, int _1TXheight,int startX,int startY,int frameSpeed = 300, double cutPercent = 30)
	{
		textureAsset = _asset;
		if (!TextureAsset::IsRegistered(_asset))
			TextureAsset::Register(_asset, _asset);

		TXwidth = TextureAsset(_asset).width(),
			TXheight = TextureAsset(_asset).height();
		int width = _1TXwidth / 3,
			height = _1TXheight / 4;
		Down.Init(_asset, U"", Rect(startX, startY+height * 0, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Left.Init(_asset, U"", Rect(startX, startY + height * 1, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Right.Init(_asset, U"", Rect(startX, startY + height * 2, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Up.Init(_asset, U"", Rect(startX, startY + height * 3, width, height), 3, 3, frameSpeed, Kind::AndTurn);

		DownStop.InitNotAnime(_asset, Rect(startX + width, startY + height * 0, width, height));
		LeftStop.InitNotAnime(_asset, Rect(startX + width, startY + height * 1, width, height));
		RightStop.InitNotAnime(_asset, Rect(startX + width, startY + height * 2, width, height));
		UpStop.InitNotAnime(_asset, Rect(startX + width, startY + height * 3, width, height));

		Up.HitSet(cutPercent);
		Down.hit = Left.hit = Right.hit = Up.hit;
	}
	int GetTXwidth()
	{
		return TXwidth;
	}
	int GetTXheight()
	{
		return TXheight;
	}
};

/// <summary>
///  8キャラ用
///　RPGツクールキャラアニメセット
///　配列8個分ある 
/// </summary>
class RPGtkChar8
{
public:
	RPGtkChar chara[8];
	RPGtkChar8()
	{}
	RPGtkChar& operator[](int index)
	{
		return chara[index];
	}

	RPGtkChar8(String filename, int frameSpeed = 300, double cutPercent = 30, int charCount = 8, int widthCount = 4, int heightCount = 2)
	{
		if (!TextureAsset::IsRegistered(filename))
			TextureAsset::Register(filename, filename);
		int width = TextureAsset(filename).width() / widthCount;
		int height = TextureAsset(filename).height() / heightCount;

		for (int i = 0; i < charCount; i++)
		{
			int getX = i % widthCount, getY = i/widthCount ;
			chara[i].Init(filename, width, height, getX * width , getY * height ,frameSpeed,cutPercent);
		}
	}
};

//
// RPGツクールサイドビュー未実装
//
class RPGtkSideviewBattler
{
public:
	//	String asset;
	Animation 
		Idle,			Stabbing,	Escape,
		ReadyPhysical,	Swinging,	Victory,
		ReadyMagical,	Shooting,	CrisisDanger,
		Guard,		UsePhysicalSkill,AbnormalState,
		Damage,		UseMagicSkill,	Sleeping,
		Evade,		UseItem,		Dead;

	int index = 0;	//デバッグ用
	Animation *NextAnime()	//デバッグ用
	{
		return anime[++index %= animeCount];
	}
	static const int animeCount = 18;
	Animation* anime[animeCount] =		// 使い道はデバッグ用ぐらい 
	{
		&Idle,			&Stabbing,	&Escape,
		&ReadyPhysical,	&Swinging,	&Victory,
		&ReadyMagical,	&Shooting,	&CrisisDanger,
		&Guard,		&UsePhysicalSkill,&AbnormalState,
		&Damage,		&UseMagicSkill,	&Sleeping,
		&Evade,		&UseItem,		&Dead
	};
	Animation* operator[](int i)
	{
		return anime[i];
	}

	RPGtkSideviewBattler()
	{}
	RPGtkSideviewBattler(String asset, int frameSpeed = 200, double cutPercent = 30)
	{
		if (!TextureAsset::IsRegistered(asset))
			TextureAsset::Register(asset, asset);
		int width = TextureAsset(asset).width() / 9,
			height = TextureAsset(asset).height() / 6;
		int hn=0;
		Idle.Init(asset, U"",		Rect(width * 0, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Stabbing.Init(asset, U"",	Rect(width * 3, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Escape.Init(asset, U"",		Rect(width * 6, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		hn++;
		ReadyPhysical.Init(asset, U"",	Rect(width * 0, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Swinging.Init(asset, U"",		Rect(width * 3, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Victory.Init(asset, U"",		Rect(width * 6, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		hn++;
		ReadyMagical.Init(asset, U"", Rect(width * 0, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Shooting.Init(asset, U"", Rect(width * 3, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		CrisisDanger.Init(asset, U"", Rect(width * 6, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		hn++;
		Guard.Init(asset, U"", Rect(width * 0, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		UsePhysicalSkill.Init(asset, U"", Rect(width * 3, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		AbnormalState.Init(asset, U"", Rect(width * 6, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		hn++;
		Damage.Init(asset, U"", Rect(width * 0, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		UseMagicSkill.Init(asset, U"", Rect(width * 3, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Sleeping.Init(asset, U"", Rect(width * 6, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		hn++;
		Evade.Init(asset, U"", Rect(width * 0, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		UseItem.Init(asset, U"", Rect(width * 3, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		Dead.Init(asset, U"", Rect(width * 6, height * hn, width, height), 3, 3, frameSpeed, Kind::AndTurn);
		
		Dead.HitSet(cutPercent);
		Idle.hit= Stabbing.hit= Escape.hit=
			ReadyPhysical.hit= Swinging.hit= Victory.hit=
			ReadyMagical.hit= Shooting.hit= CrisisDanger.hit=
			Guard.hit= UsePhysicalSkill.hit= AbnormalState.hit=
			Damage.hit= UseMagicSkill.hit= Sleeping.hit=
			Evade.hit= UseItem.hit= Dead.hit;
	}
};
