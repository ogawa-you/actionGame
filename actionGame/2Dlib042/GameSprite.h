#pragma once
# include <Siv3D.hpp>
# include "Animation.h"

//#pragma warning 26444
#pragma warning(disable:26444)

// 2019 7/09	bugfix まだ表示していないスプライトに当たり判定があるのを無いように修正
//				splitelib.h を追加

//using namespace Lambda;
using namespace std;
class TMXmap;
// 2019 9/3
// ゲームスプライト
// 注意：GameSprite::SetScrollPosition(Vec2 pos) でスクロール座標か{0,0} を必ずいれること。
// 注意：正しく表示されない。
class GameSprite
{
private:
	Rect drawRect;
	Animation* animation;
	bool soundFlag;	// 音再生は1体1体で管理するため //※
	inline static Point scrollPosition = {0,0};	//C++17
public:

// AudioAsset TextureAsset のアセット指定だと処理が重くなるので、Audio や Texture に入れて使う
	Audio Audio()
	{
		return 	animation->audio;
	}
	// スクロールポジションの設定、GameSpriteを使う前に必ず行うこと
	static void SetScrollPosition(Vec2 pos = { 0,0 })
	{
		scrollPosition = pos.asPoint();
	}

public:
	Vec2 position, velocity;
	bool mirrored=false,flipped=false;

	double
		rotation=0,
		colorR = 1.0,colorG = 1.0,colorB = 1.0,colorA = 1.0;
	float 
		rotationSpeed,
		scale = 1.0f,
		scaleSpeed = 0.0f;
	//Rect hitRect;
	int startTime;
	bool fadeAnimationRebirth = false,IsDraw = true;
	inline static Stopwatch timer;
	// スプライトのアニメーション開始する　　※これをしないと一切アニメーションしない
	static void Start()
	{
		timer.start();
	}

	GameSprite() :animation(animation) { IsDraw = false; }
	//あらかじめアニメーションデータを設定して置き、その後スプライトにそのポインタを設定する
	GameSprite(Animation* animation, Vec2 position = Vec2(0, 0), Vec2 velocity = Vec2(0, 0),
		double rotation = 0.0, float rotationSpeed = 0.0f)
		:animation(animation), position(position), velocity(velocity), rotation(rotation), rotationSpeed(rotationSpeed), soundFlag(false)
	{
		Init( animation,  position ,  velocity , rotation ,  rotationSpeed );
	}
	~GameSprite() {}

	// flipped と mirrored をリセットする
	void VHrevInit()
	{
		flipped = mirrored = false;
	}

	int Width()
	{
		return animation->draw.w;
	}
	int Height()
	{
		return animation->draw.h;
	}
	int iX()
	{
		return (int)position.x;
	}
	int iY()
	{
		return (int)position.y;
	}
	int AnimeCount()
	{
		return animation->Count();
	}
	int AnimeWidthCount()
	{
		return animation->WidthCount();
	}
	const Animation* GetAnimation()
	{
		return animation;
	}
	bool IsDelay()
	{
		return timer.ms() < startTime;//開始時間より前ならTRUE、trueなら何もしない
	}

	// タイマー初期化、ミリ秒で遅延可能、音再生フラグfalse
	void Init(int delayMS=0)
	{
		startTime = timer.ms() + delayMS;
		soundFlag = false;	// 音再生は1体1体で管理するため //※
		IsDraw = true;
	}
	// コンストラクタで新規生成すると、Siv仕様でおかしくなるので初期化関数で行う
	void Init(Animation* _animation, Vec2 _position = Vec2(0, 0), Vec2 _velocity = Vec2(0, 0),
		double _rotation = 0.0, float _rotationSpeed = 0.0f ,int delayMS=0)
	{
		this->animation=_animation,	position=_position, velocity=_velocity,
			rotation=_rotation, rotationSpeed=_rotationSpeed, soundFlag=false,
			mirrored=false,flipped=false;

		Init(delayMS);
	}
	void Change(Animation *_animation)
	{
		if (animation == _animation)return;
		animation = _animation;
		Init();
	}

	void Update() 
	{
		if (!IsDraw || IsDelay() )return;
		position += velocity;
		rotation += rotationSpeed;
		scale += scaleSpeed;

		int animNowFlame = ElapsedTime() / animation->animSpeed;
		switch (animation->state)
		{
		case Kind::Fade:
			break;
		case Kind::Loop:
			animNowFlame %= animation->Count();
				break;
		case Kind::AndTurn:
			animNowFlame %= (animation->Count()*2-2);
			if (animNowFlame >= animation->Count())
				animNowFlame=(animation->Count()-1)-(animNowFlame-(animation->Count()-1));
				break;
		default:
			break;
		}
		IsDraw = animNowFlame < AnimeCount();
		if (false)animNowFlame = AnimeCount()-1;	// 最後のパターンで止める（未実装）

		int x = animNowFlame % AnimeWidthCount();
		int y = animNowFlame / AnimeWidthCount();

		drawRect = animation->draw;
		drawRect.x += x * drawRect.w;
		drawRect.y += y * drawRect.h;

		if (x==0 && y==0 && soundFlag == false && animation->audioAsset != U"")
		{
			Audio().playOneShot();
			soundFlag = true;
		}
	}
	void UpdateDraw()
	{
		Update();
		Draw();
	}
	void UpdateDraw(Vec2 scrollPos)
	{
		Update();
		Draw(scrollPos);
	}
	// スプライトの当たり判定が当たっているか？（当たっていたら true ）
	bool Intersects(GameSprite spr)
	{
		if (spr.IsDelay() || IsDelay()) return false; // 自分か相手がまだ表示されていなかったら絶対当たらない
		return HitRect().intersects(spr.HitRect());
	}

	// 	エラー	C2027	 認識できない型 'TMXmap' が使われています。(ソース ファイルをコンパイルしています Main.cpp)	tmxAndSprite040	C : \Users\voldo\source\repos\tmxAndSprite040\tmxAndSprite040\2Dlib040\GameSprite.h	189
	//Array<int> HitTile(TMXmap& tiled, int layerID)
	//{
 //		return tiled.GetPixel4PositionAddVelocity(*this,layerID);
	//}

	Vec2 DrawPosition(Vec2 pos,Vec2 scrollPos=scrollPosition)
	{
		return pos.asPoint()-scrollPos.asPoint();
	}
	// スプライトの描画
	//                  引数：スクロール座標　（無しだとスクロール無し）
	void Draw(Vec2 scrollPos)
	{
		this->scrollPosition = scrollPos.asPoint();
		Draw();
	}

	Vec2 DrawPosition()
	{
		return position.asPoint() - scrollPosition;
	}
	// スプライトの描画
	//                  前回指定したスクロール座標を使用して描画する
	void Draw()
	{
		if (!IsDraw || IsDelay())
		{
			return;
		}
		//TextureAsset(callAsset).scaled(scale).rotated(rotation).drawAt(position,ColorF(colorR, colorG, colorB, colorA));
		animation->Draw(DrawPosition(), drawRect, mirrored, flipped);
	}
	void HitDraw()
	{
		if(! IsDelay() )animation->HitDraw(DrawPosition());
	}
	Rect HitRect()
	{
		return animation->HitRect(position);
	}

	int ElapsedTime()
	{
		return timer.ms() - startTime;
	}
	void Invisible()
	{
		IsDraw = false;
	}
	void Visible()
	{
		IsDraw = true;
	}
};

class SpriteArray
{
public:
	SpriteArray()
	{}
	~SpriteArray()
	{
		sprite.release();
	}
	Array<GameSprite> sprite;

	void operator<<(GameSprite spr)
	{
		sprite << spr;
	}
	void Add(GameSprite spr)
	{
		sprite << spr;
	}
	Array<GameSprite>& operator()()
	{
		return sprite;
	}
	GameSprite& operator[](int i)
	{
		assert(i >= 0 && i<Count() && "配列外エラー");
		return sprite[i];
	}
	int Count()
	{
		return (int)sprite.size();
	}
	// OpenSiv用remove_if 説明 https://qiita.com/Reputeless/items/aa96f356fb66fdecb418
	//
	// スプライトとぶつかったスプライトを消す　　
	// 引数：チェックしたいスプライト　戻り値：消したスプライト情報(スコア加算、アイテム、エフェクトの追加などで使うと便利)
	Array<GameSprite> RemoveHit(GameSprite& hitcheckSprite )
	{
		Array<GameSprite> spriteArray;
		sprite.remove_if(
			[&](const GameSprite& a) //sprite配列からプレイヤーとぶつかったものを削除
			{
				bool deleteFlag = hitcheckSprite.Intersects(a);
				if (deleteFlag)
				{
					spriteArray << a;
				}
				return deleteFlag; //remove_if用のreturn
			}); 
		return spriteArray;	// ぶつかったスプライトを保存
	}

	void Update()
	{
		sprite.remove_if([&](const GameSprite& a) { return a.IsDraw == false; });

		for ( auto& a:sprite)
		{
			a.Update();
		}
	}
	void Draw()
	{
		for (auto& a : sprite)
		{
			a.Draw();
		}
	}
	void UPdateDraw()
	{
		Update();
		Draw();
	}
	void HitDraw()
	{
		for (auto& a : sprite)
		{
			a.HitDraw();
		}
	}
	void Delete(int i)
	{
		sprite.remove_at(i);
	}

};

// 画面更新をFPS60にする // 消去予定
void SetFps(int fps = 60);
// 画面を1280＊720にして、画面更新をFPS60にして、スクロール座標を初期化 // 消去予定
void Start2Dlib(bool fullscreen=false, int fps = 60, Size resolution = DisplayResolution::HD_1280x720);
/// <summary>
/// 初期画面設定する
/// </summary>
/// <param name="fullscreen">全画面(true)</param>
/// <param name="fps">FPS(60)</param>
/// <param name="resolution">画素数(1280*720)</param>
void ScreenInit(bool fullscreen = true, int fps = 60, Size resolution = DisplayResolution::HD_1280x720);
/// <summary>
/// 背景色の設定
/// </summary>
/// <param name="BGcolor">背景色ColorF(0.8, 0.9, 1.0)</param>
inline void ScreenBackGroundColor(ColorF BGcolor=ColorF(0.8, 0.9, 1.0))
{
	Scene::SetBackground(BGcolor);
}

