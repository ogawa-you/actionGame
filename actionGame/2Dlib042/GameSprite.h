#pragma once
# include <Siv3D.hpp>
# include "Animation.h"

//#pragma warning 26444
#pragma warning(disable:26444)

// 2019 7/09	bugfix �܂��\�����Ă��Ȃ��X�v���C�g�ɓ����蔻�肪����̂𖳂��悤�ɏC��
//				splitelib.h ��ǉ�

//using namespace Lambda;
using namespace std;
class TMXmap;
// 2019 9/3
// �Q�[���X�v���C�g
// ���ӁFGameSprite::SetScrollPosition(Vec2 pos) �ŃX�N���[�����W��{0,0} ��K������邱�ƁB
// ���ӁF�������\������Ȃ��B
class GameSprite
{
private:
	Rect drawRect;
	Animation* animation;
	bool soundFlag;	// ���Đ���1��1�̂ŊǗ����邽�� //��
	inline static Point scrollPosition = {0,0};	//C++17
public:

// AudioAsset TextureAsset �̃A�Z�b�g�w�肾�Ə������d���Ȃ�̂ŁAAudio �� Texture �ɓ���Ďg��
	Audio Audio()
	{
		return 	animation->audio;
	}
	// �X�N���[���|�W�V�����̐ݒ�AGameSprite���g���O�ɕK���s������
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
	// �X�v���C�g�̃A�j���[�V�����J�n����@�@����������Ȃ��ƈ�؃A�j���[�V�������Ȃ�
	static void Start()
	{
		timer.start();
	}

	GameSprite() :animation(animation) { IsDraw = false; }
	//���炩���߃A�j���[�V�����f�[�^��ݒ肵�Ēu���A���̌�X�v���C�g�ɂ��̃|�C���^��ݒ肷��
	GameSprite(Animation* animation, Vec2 position = Vec2(0, 0), Vec2 velocity = Vec2(0, 0),
		double rotation = 0.0, float rotationSpeed = 0.0f)
		:animation(animation), position(position), velocity(velocity), rotation(rotation), rotationSpeed(rotationSpeed), soundFlag(false)
	{
		Init( animation,  position ,  velocity , rotation ,  rotationSpeed );
	}
	~GameSprite() {}

	// flipped �� mirrored �����Z�b�g����
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
		return timer.ms() < startTime;//�J�n���Ԃ��O�Ȃ�TRUE�Atrue�Ȃ牽�����Ȃ�
	}

	// �^�C�}�[�������A�~���b�Œx���\�A���Đ��t���Ofalse
	void Init(int delayMS=0)
	{
		startTime = timer.ms() + delayMS;
		soundFlag = false;	// ���Đ���1��1�̂ŊǗ����邽�� //��
		IsDraw = true;
	}
	// �R���X�g���N�^�ŐV�K��������ƁASiv�d�l�ł��������Ȃ�̂ŏ������֐��ōs��
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
		if (false)animNowFlame = AnimeCount()-1;	// �Ō�̃p�^�[���Ŏ~�߂�i�������j

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
	// �X�v���C�g�̓����蔻�肪�������Ă��邩�H�i�������Ă����� true �j
	bool Intersects(GameSprite spr)
	{
		if (spr.IsDelay() || IsDelay()) return false; // ���������肪�܂��\������Ă��Ȃ��������Γ�����Ȃ�
		return HitRect().intersects(spr.HitRect());
	}

	// 	�G���[	C2027	 �F���ł��Ȃ��^ 'TMXmap' ���g���Ă��܂��B(�\�[�X �t�@�C�����R���p�C�����Ă��܂� Main.cpp)	tmxAndSprite040	C : \Users\voldo\source\repos\tmxAndSprite040\tmxAndSprite040\2Dlib040\GameSprite.h	189
	//Array<int> HitTile(TMXmap& tiled, int layerID)
	//{
 //		return tiled.GetPixel4PositionAddVelocity(*this,layerID);
	//}

	Vec2 DrawPosition(Vec2 pos,Vec2 scrollPos=scrollPosition)
	{
		return pos.asPoint()-scrollPos.asPoint();
	}
	// �X�v���C�g�̕`��
	//                  �����F�X�N���[�����W�@�i�������ƃX�N���[�������j
	void Draw(Vec2 scrollPos)
	{
		this->scrollPosition = scrollPos.asPoint();
		Draw();
	}

	Vec2 DrawPosition()
	{
		return position.asPoint() - scrollPosition;
	}
	// �X�v���C�g�̕`��
	//                  �O��w�肵���X�N���[�����W���g�p���ĕ`�悷��
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
		assert(i >= 0 && i<Count() && "�z��O�G���[");
		return sprite[i];
	}
	int Count()
	{
		return (int)sprite.size();
	}
	// OpenSiv�premove_if ���� https://qiita.com/Reputeless/items/aa96f356fb66fdecb418
	//
	// �X�v���C�g�ƂԂ������X�v���C�g�������@�@
	// �����F�`�F�b�N�������X�v���C�g�@�߂�l�F�������X�v���C�g���(�X�R�A���Z�A�A�C�e���A�G�t�F�N�g�̒ǉ��ȂǂŎg���ƕ֗�)
	Array<GameSprite> RemoveHit(GameSprite& hitcheckSprite )
	{
		Array<GameSprite> spriteArray;
		sprite.remove_if(
			[&](const GameSprite& a) //sprite�z�񂩂�v���C���[�ƂԂ��������̂��폜
			{
				bool deleteFlag = hitcheckSprite.Intersects(a);
				if (deleteFlag)
				{
					spriteArray << a;
				}
				return deleteFlag; //remove_if�p��return
			}); 
		return spriteArray;	// �Ԃ������X�v���C�g��ۑ�
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

// ��ʍX�V��FPS60�ɂ��� // �����\��
void SetFps(int fps = 60);
// ��ʂ�1280��720�ɂ��āA��ʍX�V��FPS60�ɂ��āA�X�N���[�����W�������� // �����\��
void Start2Dlib(bool fullscreen=false, int fps = 60, Size resolution = DisplayResolution::HD_1280x720);
/// <summary>
/// ������ʐݒ肷��
/// </summary>
/// <param name="fullscreen">�S���(true)</param>
/// <param name="fps">FPS(60)</param>
/// <param name="resolution">��f��(1280*720)</param>
void ScreenInit(bool fullscreen = true, int fps = 60, Size resolution = DisplayResolution::HD_1280x720);
/// <summary>
/// �w�i�F�̐ݒ�
/// </summary>
/// <param name="BGcolor">�w�i�FColorF(0.8, 0.9, 1.0)</param>
inline void ScreenBackGroundColor(ColorF BGcolor=ColorF(0.8, 0.9, 1.0))
{
	Scene::SetBackground(BGcolor);
}

