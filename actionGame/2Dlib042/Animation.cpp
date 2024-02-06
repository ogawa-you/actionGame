#include "Animation.h"

// 2019 7/09

Animation::Animation() :textureAsset(U""), hit(0, 0, 0, 0), count(0), animSpeed(200) {}
Animation::Animation(String textureAsset , Rect draw , int count , int widthCount , int animSpeed, Kind state, double cutPercent, String audioAsset)
{
//	HitSet();
	Init(textureAsset, audioAsset, draw, count, widthCount, animSpeed, state, cutPercent);
}
// 横1列のみ初期化
Animation::Animation(String _textureAsset, int count, int animSpeed , Kind state, String audioAsset , double cutPercent )
{
	if (!TextureAsset::IsRegistered(_textureAsset))
		TextureAsset::Register(_textureAsset, _textureAsset);
	textureAsset = _textureAsset;

	Init(textureAsset, audioAsset, Rect(0, 0, TextureAsset(_textureAsset).width() / count, TextureAsset(_textureAsset).height())
		, count, count, animSpeed, state, cutPercent);
}

// 横分割数、縦分割数での初期化  ぴったりでない場合、減らす数のパターン数をdecCountに入れる
Animation::Animation(String _textureAsset, Size widthHeightCount, int decCount,
	int _animSpeed , Kind _state, String audioAsset,
	double cutPercent)
{
	widthCount = widthHeightCount.x;
	int heightCount = widthHeightCount.y;
	count = widthCount * heightCount - decCount;
	if (!TextureAsset::IsRegistered(_textureAsset))
		TextureAsset::Register(_textureAsset, _textureAsset);
	textureAsset = _textureAsset;
	texture = TextureAsset(_textureAsset); // 高速化
	//Init(textureAsset, audioAsset, Rect(0, 0, TextureAsset(_textureAsset).width() / widthCount, TextureAsset(_textureAsset).height()/heightCount)
	//	, count, widthCount, _animSpeed, _state, cutPercent);
	Init(textureAsset, audioAsset, Rect(0, 0, texture.width() / widthCount, texture.height()/heightCount)
		, count, widthCount, _animSpeed, _state, cutPercent); // 高速化
}
// 横分割数、縦分割数での初期化  ぴったりでない場合、減らす数のパターン数をdecCountに入れる
Animation::Animation(String _textureAsset, String audioAsset, Size widthHeightCount,
	int _animSpeed , int decCount ,	double cutPercent , 
	Kind _state)
{
	widthCount = widthHeightCount.x;
	int heightCount = widthHeightCount.y;
	count = widthCount * heightCount - decCount;
	if (!TextureAsset::IsRegistered(_textureAsset))
		TextureAsset::Register(_textureAsset, _textureAsset);
	textureAsset = _textureAsset;
	texture = TextureAsset(_textureAsset); // 高速化
	//Init(textureAsset, audioAsset, Rect(0, 0, TextureAsset(_textureAsset).width() / widthCount, TextureAsset(_textureAsset).height() / heightCount)
	//	, count, widthCount, _animSpeed, _state, cutPercent);
	Init(textureAsset, audioAsset, Rect(0, 0, texture.width() / widthCount, texture.height() / heightCount)
		, count, widthCount, _animSpeed, _state, cutPercent);	// 高速化

}


void Animation::HitSet(double cutPercent)
{
	int width = draw.w, height = draw.h;
	int shiftX = int(cutPercent / 100.0 / 2.0 * width), shiftY = int(cutPercent / 100.0 / 2.0 * height);
	hit = Rect(shiftX, shiftY, width - shiftX * 2, height - shiftY * 2);
}
void Animation::Draw(Vec2 position, Rect spriteRect ,bool mirrored,bool flipped)
{
	if (spriteRect.w == 0)
	{
		spriteRect = draw;
	}
	//TextureRegion grapics = TextureAsset(textureAsset)(spriteRect);
	TextureRegion grapics = texture(spriteRect); //高速化
	if (mirrored)
	{
		if(flipped)
			grapics.mirrored().flipped().draw(position);
		else
			grapics.mirrored().draw(position);
	}
	else
	{
		if (flipped)
			grapics.flipped().draw(position);
		else
			grapics.draw(position);
	}
}
Rect Animation::HitRect(Vec2 sprPosition)
{
	int x = (int)sprPosition.x;
	int y = (int)sprPosition.y;
	Rect nowHit = this->hit;
	nowHit.x += x;
	nowHit.y += y;
	return nowHit;
}

void Animation::HitDraw(Vec2 sprPosition)
{
	HitRect(sprPosition).drawFrame(1, 1, RandomHSV());
}

